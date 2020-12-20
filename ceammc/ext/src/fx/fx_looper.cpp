#include "fx_looper.h"
#include "ceammc_factory.h"
#include "ceammc_units.h"

#include <algorithm>
#include <array>
#include <functional>
#include <unordered_map>

static const float DEFAULT_CAPACITY_SEC = 5;
static const float MAX_CAPACITY_SEC = 120;

#define CHECK_PHASE(p, b)                                                          \
    {                                                                              \
        if (p >= b.size()) {                                                       \
            LIB_ERR << "phase overflow: " << #p << " " << p << " >= " << b.size(); \
            return;                                                                \
        }                                                                          \
    }

template <class C>
static bool applyLinFadeIn(C& containter, size_t samples)
{
    if (samples == 0)
        return true;

    auto b = std::begin(containter);
    size_t N = std::distance(b, std::end(containter));

    if (samples >= N)
        return false;

    for (size_t i = 0; i < samples; i++) {
        auto amp = i / typename C::value_type(samples);
        b[i] = b[i] * amp;
    }

    return true;
}

template <class C>
static bool applyLinFadeOut(C& containter, size_t length, size_t samples)
{
    if (samples == 0 || length == 0)
        return true;

    auto b = std::begin(containter);

    if (samples >= length)
        return false;

    for (size_t i = 0; i < samples; i++)
        b[length - 1 - i] *= i / typename C::value_type(samples);

    return true;
}

static void fillZero8(t_sample** out, size_t n)
{
    for (size_t i = 0; i < n; i += 8) {
        out[0][i + 0] = 0;
        out[0][i + 1] = 0;
        out[0][i + 2] = 0;
        out[0][i + 3] = 0;
        out[0][i + 4] = 0;
        out[0][i + 5] = 0;
        out[0][i + 6] = 0;
        out[0][i + 7] = 0;
    }
}

FxLooper::FxLooper(const PdArgs& args)
    : SoundExternal(args)
    , state_(STATE_INIT)
    , capacity_sec_(0)
    , round_(0)
    , loop_bang_(0)
    , x_play_to_stop_(nullptr)
    , x_stop_to_play_(nullptr)
    , x_rec_to_play_(nullptr)
    , x_play_to_dub_(nullptr)
    , x_dub_to_play_(nullptr)
    , x_dub_to_stop_(nullptr)
    , loop_smooth_ms_(nullptr)
    , max_samples_(0)
    , loop_len_(0)
    , play_phase_(0)
    , rec_phase_(0)
    , clock_(this, &FxLooper::clockTick)
    , array_name_(nullptr)
{
    initTansitionTable();

    capacity_sec_ = new FloatProperty("@capacity", DEFAULT_CAPACITY_SEC);
    capacity_sec_->setArgIndex(0);
    capacity_sec_->setUnitsSec();
    addProperty(capacity_sec_);

    loop_bang_ = new BoolProperty("@loop_bang", false);
    addProperty(loop_bang_);

    loop_smooth_ms_ = new FloatProperty("@loop_smooth", 10);
    loop_smooth_ms_->checkMinEq(0);
    loop_smooth_ms_->setUnits(PropValueUnits::MSEC);
    addProperty(loop_smooth_ms_);

    x_play_to_stop_ = new LinFadeoutProperty("@play_to_stop_time", 10);
    addProperty(x_play_to_stop_);

    x_play_to_dub_ = new LinFadeinProperty("@play_to_dub_time", 10);
    addProperty(x_play_to_dub_);

    x_stop_to_play_ = new LinFadeinProperty("@stop_to_play_time", 10);
    addProperty(x_stop_to_play_);

    x_rec_to_play_ = new PowXFadeProperty("@rec_to_play_time", 30);
    addProperty(x_rec_to_play_);

    x_dub_to_play_ = new LinFadeoutProperty("@dub_to_play_time", 20);
    addProperty(x_dub_to_play_);

    x_dub_to_stop_ = new LinFadeoutProperty("@dub_to_stop_time", 20);
    addProperty(x_dub_to_stop_);

    round_ = new IntProperty("@round", 0);
    round_->setUnits(PropValueUnits::SAMP);
    addProperty(round_);

    {
        Property* p = createCbFloatProperty(
            "@length",
            [this]() -> t_float {
                const auto sr = sys_getsr();
                return sr > 0 ? (loop_len_ / sr) : 0;
            });
        p->setUnitsSec();
        p->checkNonNegative();
    }

    {
        Property* p = createCbFloatProperty(
            "@play_pos",
            [this]() -> t_float {
                const auto sr = sys_getsr();
                return sr > 0 ? (play_phase_ / sys_getsr()) : 0;
            });
        p->setUnitsSec();
        p->checkNonNegative();
    }

    {
        Property* p = createCbFloatProperty(
            "@play_phase",
            [this]() -> t_float { return (loop_len_ == 0) ? 0 : (play_phase_ / t_float(loop_len_)); });
        p->setFloatCheck(PropValueConstraints::CLOSED_RANGE, 0, 1);
    }

    {
        Property* p = createCbSymbolProperty("@state",
            [this]() -> t_symbol* {
                static t_symbol* states[] = {
                    gensym("init"),
                    gensym("record"),
                    gensym("rec->play"),
                    gensym("rec->stop"),
                    gensym("rec->dub"),
                    gensym("overdub"),
                    gensym("dub->stop"),
                    gensym("dub->play"),
                    gensym("pause"),
                    gensym("play"),
                    gensym("play->stop"),
                    gensym("play->dub"),
                    gensym("stop"),
                    gensym("stop->play")
                };

                static_assert((sizeof(states) / sizeof(states[0])) == (STATE_COUNT_), "invalid state count");

                return states[state_];
            });

        if (!p->infoT().setConstraints(PropValueConstraints::ENUM))
            OBJ_ERR << "can't set @state contraints";
        else if (!p->infoT().addEnums({ "init", "stop", "record", "play", "overdub" }))
            OBJ_ERR << "can't set @state enum values";
    }

    array_name_ = new SymbolProperty("@array", &s_);
    addProperty(array_name_);

    createSignalOutlet();
    createOutlet();

    // parse all properties
    parseProperties();

    float len_sec = capacity_sec_->value();
    if (len_sec <= 0 || MAX_CAPACITY_SEC <= len_sec) {
        OBJ_ERR << "invalid loop length: " << len_sec;

        len_sec = DEFAULT_CAPACITY_SEC;
        capacity_sec_->setValue(len_sec);

        OBJ_DBG << "using default loop length: " << len_sec;
    }

    resizeBuffer();
    calcXFades();
    OBJ_DBG << "max loop length " << max_samples_ << " samples (" << capacity_sec_->value() << " sec)";
}

void FxLooper::onBang()
{
    if (state_ == STATE_PLAY)
        play_phase_ = 0;
}

void FxLooper::processBlock(const t_sample** in, t_sample** out)
{
    const size_t bs = blockSize();

    switch (state_) {
    case STATE_STOP:
    case STATE_PAUSE:
        stateStop(out);
        break;
    case STATE_STOP_XFADE_PLAY:
        stateStopToPlay(in, out);
        break;
    case STATE_INIT:
        fillZero8(out, bs);
        break;
    case STATE_PLAY_XFADE_STOP:
        statePlayToStop(in, out);
        break;
    case STATE_PLAY:
        statePlay(in, out);
        break;
    case STATE_PLAY_XFADE_DUB:
        statePlayToDub(in, out);
        break;
    case STATE_REC:
        stateRecord(in, out);
        break;
    case STATE_REC_XFADE_PLAY:
        stateRecordToPlay(in, out);
        break;
    case STATE_REC_XFADE_STOP:
        stateRecordToStop(in, out);
        break;
    case STATE_REC_XFADE_DUB:
        stateRecordToDub(in, out);
        break;
    case STATE_DUB:
        stateDub(in, out);
        break;
    case STATE_DUB_XFADE_STOP:
        stateDubToStop(in, out);
        break;
    case STATE_DUB_XFADE_PLAY:
        stateDubToPlay(in, out);
        break;
    default:
        break;
    }
}

void FxLooper::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    calcXFades();
    if (arraySpecified() && !array_.open(array_name_->value())) {
        state_ = STATE_STOP;
        OBJ_ERR << "can't find array: " << array_name_->value();
    }
}

void FxLooper::stateDub(const t_sample** in, t_sample** out)
{
    processPlayLoop(in, out, [](t_sample samp_in, t_sample& samp_out, t_sample& samp_rec) {
        // output current
        samp_out = samp_rec;
        // add new record value
        samp_rec += samp_in;
    });
}

void FxLooper::stateDubToStop(const t_sample** in, t_sample** out)
{
    if (x_dub_to_stop_->isRunning()) {
        processPlayLoop(in, out, [this](t_sample samp_in, t_sample& samp_out, t_sample& samp_rec) {
            // fadeout value
            auto amp = this->x_dub_to_stop_->amp();
            // fadeout output current
            samp_out = amp * samp_rec;
            // fadeout overdub
            samp_rec += amp * samp_in;

            // move fader phase
            x_dub_to_stop_->next();
        });
    } else {
        state_ = STATE_STOP;
        stateStop(out);
    }
}

void FxLooper::stateDubToPlay(const t_sample** in, t_sample** out)
{
    if (x_dub_to_play_->isRunning()) {
        processPlayLoop(in, out, [this](t_sample samp_in, t_sample& samp_out, t_sample& samp_rec) {
            auto amp = x_dub_to_play_->amp();
            // play current
            samp_out = samp_rec;
            // overdub fadeout
            samp_rec += amp * samp_in;

            // move fader phase
            x_dub_to_play_->next();
        });
    } else {
        state_ = STATE_PLAY;
        statePlay(in, out);
    }
}

void FxLooper::stateStop(t_sample** out)
{
    fillZero8(out, blockSize());
}

void FxLooper::statePlay(const t_sample** in, t_sample** out)
{
    processPlayLoop(in, out, [this](t_sample, t_sample& samp_out, t_sample& samp_rec) {
        samp_out = samp_rec;
    });
}

void FxLooper::statePlayToStop(const t_sample** in, t_sample** out)
{
    if (x_play_to_stop_->isRunning()) {
        processPlayLoop(in, out, [this](t_sample, t_sample& samp_out, t_sample& samp_rec) {
            // fadeout value
            auto amp = this->x_play_to_stop_->amp();
            // play fadeout
            samp_out = amp * samp_rec;
            // move fader phase
            x_play_to_stop_->next();
        });
    } else {
        state_ = STATE_STOP;
        stateStop(out);
    }
}

void FxLooper::statePlayToDub(const t_sample** in, t_sample** out)
{
    if (x_play_to_dub_->isRunning()) {
        processPlayLoop(in, out, [this](t_sample samp_in, t_sample& samp_out, t_sample& samp_rec) {
            // get rec [0->1]
            auto amp = x_play_to_dub_->amp();
            // play current
            samp_out = samp_rec;
            // overdub
            samp_rec += amp * samp_in;
            // move phase
            x_play_to_dub_->next();
        });
    } else {
        state_ = STATE_DUB;
        stateDub(in, out);
    }
}

void FxLooper::stateStopToPlay(const t_sample** in, t_sample** out)
{
    if (x_stop_to_play_->isRunning()) {
        processPlayLoop(in, out, [this](t_sample, t_sample& samp_out, t_sample& samp_rec) {
            // get play fadein [0->1]
            auto amp = x_stop_to_play_->amp();
            // play current
            samp_out = amp * samp_rec;
            // move phase
            x_stop_to_play_->next();
        });
    } else {
        state_ = STATE_PLAY;
        statePlay(in, out);
    }
}

void FxLooper::stateRecord(const t_sample** in, t_sample** out)
{
    auto done = processRecLoop(in, out, [this](t_sample samp_in, t_sample& samp_out, t_sample& samp_rec) {
        samp_rec = samp_in;
        samp_out = 0;
    });

    if (done) {
        applyFades();
        stateStop(out);
        loopCycleFinish();
    }
}

void FxLooper::stateRecordToPlay(const t_sample** in, t_sample** out)
{
    if (x_rec_to_play_->isRunning()) {
        processPlayLoop(in, out, [this](t_sample samp_in, t_sample& samp_out, t_sample& samp_rec) {
            samp_rec *= x_rec_to_play_->fadeinAmp();
            samp_rec += x_rec_to_play_->fadeoutAmp() * samp_in;
            samp_out = samp_rec;
            x_rec_to_play_->next();
        });
    } else {
        // move to PLAY state
        state_ = STATE_PLAY;
        statePlay(in, out);
    }
}

void FxLooper::stateRecordToStop(const t_sample** in, t_sample** out)
{
    state_ = STATE_STOP;
    rec_phase_ = 0;
    play_phase_ = 0;

    applyFades();
    stateStop(out);
    loopCycleFinish();
}

void FxLooper::stateRecordToDub(const t_sample** in, t_sample** out)
{
    // one block transition
    size_t i = 0;

    processPlayLoop(in, out, [this, &i](t_sample samp_in, t_sample& samp_out, t_sample& samp_rec) {
        // linear fadein
        auto amp = t_sample(i) / t_sample(blockSize());
        // copy input
        t_sample tmp = samp_in * amp;
        // output current
        samp_out = samp_rec;
        // overdub
        samp_rec += tmp;
    });

    state_ = STATE_DUB;
}

void FxLooper::m_record(t_symbol*, const AtomListView&)
{
    toState(STATE_REC);
}

void FxLooper::m_stop(t_symbol*, const AtomListView&)
{
    toState(STATE_STOP);
}

void FxLooper::m_pause(t_symbol* s, const AtomListView&)
{
    toState(STATE_PAUSE);
}

void FxLooper::m_play(t_symbol*, const AtomListView&)
{
    toState(STATE_PLAY);
}

void FxLooper::m_overdub(t_symbol*, const AtomListView&)
{
    toState(STATE_DUB);
}

void FxLooper::m_clear(t_symbol*, const AtomListView&)
{
    if (arraySpecified()) {
        if (!array_.isValid()) {
            OBJ_ERR << "can't find array: " << array_name_->value();
            state_ = STATE_STOP;
            return;
        }

        array_.fillWith(0.f);
        array_.redraw();
    } else {
        buffer_.assign(buffer_.size(), 0);
    }

    loop_len_ = 0;
    rec_phase_ = 0;
    play_phase_ = 0;
    state_ = STATE_STOP;
}

void FxLooper::m_adjust(t_symbol* s, const AtomListView& lst)
{
    if (!checkArgs(lst, ARG_FLOAT))
        return;

    long samples = lst[0].asFloat() * sys_getsr();

    if (labs(samples) >= loop_len_) {
        METHOD_ERR(s) << "adjust value is too big: " << lst[0];
        return;
    }

    loop_len_ = long(loop_len_) + samples;
}

void FxLooper::m_smooth(t_symbol* s, const AtomListView& lst)
{
    using namespace ceammc::units;
    auto res = TimeValue::parse(lst);
    UnitParseError err;
    if (res.matchError(err)) {
        METHOD_ERR(s) << err.msg;
        return;
    }

    TimeValue& tm = res.value();
    tm.setSamplerate(samplerate());
    const size_t N = std::abs(tm.toSamples());
    doApplyFades(N);
}

std::vector<t_sample> FxLooper::loop() const
{
    return std::vector<t_sample>(buffer_.begin(), buffer_.begin() + loop_len_);
}

void FxLooper::loopCycleFinish()
{
    rec_phase_ = 0;
    play_phase_ = 0;

    if (loop_bang_->value())
        clock_.delay(0);
}

void FxLooper::clockTick()
{
    bangTo(1);
}

void FxLooper::calcXFades()
{
    const auto BS = blockSize();
    const auto SR = samplerate();

    x_play_to_stop_->calc(SR, BS);
    x_play_to_dub_->calc(SR, BS);

    x_stop_to_play_->calc(SR, BS);

    x_rec_to_play_->calc(SR, BS);

    x_dub_to_play_->calc(SR, BS);
    x_dub_to_stop_->calc(SR, BS);
}

void FxLooper::initTansitionTable()
{
    state_table_.fill(StateTransition());

    // INIT
    // init->rec
    state_table_[STATE_INIT][STATE_REC] = [this]() {
        if (!resizeBuffer())
            return false;

        state_ = STATE_REC;
        rec_phase_ = 0;
        play_phase_ = 0;
        loop_len_ = 0;

        OBJ_DBG << "write loop record: max " << max_samples_ << " samples ("
                << float(max_samples_) / samplerate() << " sec)";

        return true;
    };

    // init->play
    state_table_[STATE_INIT][STATE_PLAY] = [this]() {
        OBJ_ERR << "loop is not recorded yet...";
        return false;
    };

    // init->stop
    state_table_[STATE_INIT][STATE_STOP] = [this]() {
        OBJ_ERR << "loop is not recorded nor playing yet...";
        return false;
    };

    // init->dub
    state_table_[STATE_INIT][STATE_DUB] = [this]() {
        OBJ_ERR << "loop is not recorded yet...";
        return false;
    };

    // init->pause
    state_table_[STATE_INIT][STATE_PAUSE] = [this]() {
        OBJ_ERR << "loop is not recorded yet...";
        return false;
    };

    // PLAY
    // play->play
    state_table_[STATE_PLAY][STATE_PLAY] = [this]() {
        OBJ_ERR << "already playing...";
        return false;
    };

    // play->rec
    state_table_[STATE_PLAY][STATE_REC] = [this]() {
        OBJ_DBG << "overwrite loop record: max " << max_samples_ << " samples ("
                << float(max_samples_) / samplerate() << " sec)";

        if (!resizeBuffer())
            return false;

        state_ = STATE_REC;
        rec_phase_ = 0;
        play_phase_ = 0;
        loop_len_ = 0;
        return true;
    };

    // play->stop
    state_table_[STATE_PLAY][STATE_STOP] = [this]() {
        state_ = STATE_PLAY_XFADE_STOP;
        x_play_to_stop_->reset();
        return true;
    };

    // play->pause
    state_table_[STATE_PLAY][STATE_PAUSE] = [this]() {
        state_ = STATE_PAUSE;
        return true;
    };

    // play->dub
    state_table_[STATE_PLAY][STATE_DUB] = [this]() {
        state_ = STATE_PLAY_XFADE_DUB;
        x_play_to_dub_->reset();
        return true;
    };

    // STOP
    // stop->stop
    state_table_[STATE_STOP][STATE_STOP] = [this]() {
        OBJ_ERR << "already stopped...";
        return false;
    };

    // stop->play
    state_table_[STATE_STOP][STATE_PLAY] = [this]() {
        if (loop_len_ == 0) {
            OBJ_ERR << "loop is not recorded yet...";
            return false;
        }

        state_ = STATE_STOP_XFADE_PLAY;
        x_stop_to_play_->reset();
        play_phase_ = 0;
        return true;
    };

    // stop->rec
    state_table_[STATE_STOP][STATE_REC] = [this]() {
        OBJ_DBG << "starting loop record: max " << max_samples_ << " samples ("
                << float(max_samples_) / samplerate() << " sec)";

        if (!resizeBuffer())
            return false;

        state_ = STATE_REC;
        rec_phase_ = 0;
        play_phase_ = 0;
        loop_len_ = 0;
        return true;
    };

    // stop->pause
    state_table_[STATE_STOP][STATE_PAUSE] = [this]() {
        OBJ_ERR << "is not playing...";
        return false;
    };

    // PAUSE
    // pause->pause
    state_table_[STATE_PAUSE][STATE_PAUSE] = [this]() {
        OBJ_ERR << "already paused...";
        return false;
    };

    // pause->stop
    state_table_[STATE_PAUSE][STATE_STOP] = [this]() {
        state_ = STATE_STOP;
        play_phase_ = 0;
        rec_phase_ = 0;
        return true;
    };

    // pause->play
    state_table_[STATE_PAUSE][STATE_PLAY] = [this]() {
        state_ = STATE_PLAY;
        return true;
    };

    // pause->dub
    state_table_[STATE_PAUSE][STATE_DUB] = [this]() {
        OBJ_ERR << "can't overdub from pause...";
        return false;
    };

    // pause->rec
    state_table_[STATE_PAUSE][STATE_REC] = [this]() {
        OBJ_ERR << "can't record from pause...";
        return false;
    };

    // RECORD
    // rec->rec
    state_table_[STATE_REC][STATE_REC] = [this]() {
        OBJ_ERR << "already recording...";
        return false;
    };

    // rec->stop
    state_table_[STATE_REC][STATE_STOP] = [this]() {
        state_ = STATE_REC_XFADE_STOP;
        finishRecord();
        return true;
    };

    // rec->pause
    state_table_[STATE_REC][STATE_PAUSE] = [this]() {
        state_ = STATE_PAUSE;
        loop_len_ = rec_phase_;
        return true;
    };

    // rec->play
    state_table_[STATE_REC][STATE_PLAY] = [this]() {
        state_ = STATE_REC_XFADE_PLAY;
        x_rec_to_play_->reset();

        finishRecord();

        rec_phase_ = 0;
        play_phase_ = 0;
        return true;
    };

    // rec->dub
    state_table_[STATE_REC][STATE_DUB] = [this]() {
        state_ = STATE_DUB;
        rec_phase_ = 0;
        play_phase_ = 0;
        OBJ_DBG << "start loop dub: " << loop_len_ << " samples";
        return true;
    };

    // DUB
    // dub->play
    state_table_[STATE_DUB][STATE_PLAY] = [this]() {
        state_ = STATE_DUB_XFADE_PLAY;
        x_dub_to_play_->reset();
        return true;
    };

    // dub->stop
    state_table_[STATE_DUB][STATE_STOP] = [this]() {
        state_ = STATE_DUB_XFADE_STOP;
        return true;
    };

    // dub->dub
    state_table_[STATE_DUB][STATE_DUB] = [this]() {
        OBJ_ERR << "already overdubbing...";
        return false;
    };

    // dub->rec
    state_table_[STATE_DUB][STATE_REC] = [this]() {
        OBJ_DBG << "starting loop record: max " << max_samples_ << " samples ("
                << float(max_samples_) / samplerate() << " sec)";

        state_ = STATE_REC;
        rec_phase_ = 0;
        play_phase_ = 0;
        return true;
    };

    auto stop_fn = [this]() {
        state_ = STATE_STOP;
        rec_phase_ = 0;
        play_phase_ = 0;
        return true;
    };

    state_table_[STATE_REC_XFADE_PLAY][STATE_STOP] = stop_fn;
    state_table_[STATE_REC_XFADE_STOP][STATE_STOP] = stop_fn;
    state_table_[STATE_REC_XFADE_DUB][STATE_STOP] = stop_fn;
    state_table_[STATE_DUB_XFADE_STOP][STATE_STOP] = stop_fn;
    state_table_[STATE_DUB_XFADE_PLAY][STATE_STOP] = stop_fn;
    state_table_[STATE_PLAY_XFADE_STOP][STATE_STOP] = stop_fn;
    state_table_[STATE_PLAY_XFADE_DUB][STATE_STOP] = stop_fn;
    state_table_[STATE_STOP_XFADE_PLAY][STATE_STOP] = stop_fn;
}

void FxLooper::toState(FxLooperState st)
{
    auto fn = state_table_[state_][st];
    if (!fn) {
        OBJ_ERR << "unhandled transition from state " << state_ << " to " << st;
        return;
    }

    fn();
}

bool FxLooper::resizeBuffer()
{
    max_samples_ = capacity_sec_->value() * sys_getsr();
    if (arraySpecified()) {
        if (!array_.open(array_name_->value())) {
            OBJ_ERR << "can't find array: " << array_name_->value();
            return false;
        }

        return array_.resize(max_samples_);
    } else {
        try {
            buffer_.resize(max_samples_);
        } catch (std::exception& e) {
            OBJ_ERR << "resize failed: " << e.what();
            return false;
        }
    }

    return true;
}

void FxLooper::finishRecord()
{
    // loop align
    if (round_->value() > 0) {
        const size_t ALIGN = round_->value();
        size_t div = loop_len_ % ALIGN;

        if (div < ALIGN / 2) {
            // round to minimal align
            OBJ_DBG << "shorten loop by " << div << "samples";
            loop_len_ = (loop_len_ / ALIGN) * ALIGN;
        }
    } else
        loop_len_ = rec_phase_;

    OBJ_DBG << "recorded loop (" << float(loop_len_) / samplerate() << " sec)";

    if (arraySpecified() && array_.isValid())
        array_.resize(loop_len_);
}

bool FxLooper::arraySpecified() const
{
    return array_name_->value() != &s_;
}

void FxLooper::applyFades()
{
    const size_t N = std::abs(loop_smooth_ms_->value() * samplerate() * 0.001f);
    doApplyFades(N);
}

void FxLooper::doApplyFades(size_t N)
{
    // using array
    if (arraySpecified()) {
        if (!array_.isValid()) {
            OBJ_ERR << "invalid array: " << array_.name();
            return;
        }

        applyLinFadeIn(array_, N);
        applyLinFadeOut(array_, loop_len_, N);
        array_.redraw();
    } else {
        applyLinFadeIn(buffer_, N);
        applyLinFadeOut(buffer_, loop_len_, N);
    }
}

XFadeProperty::XFadeProperty(const std::string& name, float ms)
    : FloatProperty(name, ms)
    , length_(0)
    , phase_(0)
    , sr_(10000)
    , bs_(32)
{
    checkMinEq(0);
    setUnitsMs();
}

void XFadeProperty::calc(size_t sr, size_t bs)
{
    if (bs == 0)
        return;

    auto samples = std::round(0.001f * sr * std::fabs(value()));
    length_ = (size_t(samples) / bs) * bs;
    phase_ = 0;
    sr_ = sr;
    bs_ = bs;
}

void XFadeProperty::reset()
{
    phase_ = 0;
}

bool XFadeProperty::set(const AtomListView& lst)
{
    auto rc = FloatProperty::set(lst);
    if (rc)
        calc(sr_, bs_);

    return rc;
}

LinFadeoutProperty::LinFadeoutProperty(const std::string& name, float ms)
    : XFadeProperty(name, ms)
{
}

t_float LinFadeoutProperty::amp() const
{
    return double(length_ - phase_) / double(length_);
}

LinFadeinProperty::LinFadeinProperty(const std::string& name, float ms)
    : XFadeProperty(name, ms)
{
}

t_float LinFadeinProperty::amp() const
{
    return double(phase_) / double(length_);
}

PowXFadeProperty::PowXFadeProperty(const std::string& name, float ms)
    : XFadeProperty(name, ms)
{
}

t_float PowXFadeProperty::amp() const
{
    return (-1 * double(phase_ * phase_) / double(length_ * length_)) + 1;
}

t_float PowXFadeProperty::fadeinAmp() const
{
    return amp();
}

t_float PowXFadeProperty::fadeoutAmp() const
{
    auto p = length_ - phase_;
    return (-1 * double(p * p) / double(length_ * length_)) + 1;
}

void setup_fx_looper()
{
    SoundExternalFactory<FxLooper> obj("fx.looper~");
    obj.addMethod("record", &FxLooper::m_record);
    obj.addMethod("rec", &FxLooper::m_record);
    obj.addMethod("stop", &FxLooper::m_stop);
    obj.addMethod("play", &FxLooper::m_play);
    obj.addMethod("overdub", &FxLooper::m_overdub);
    obj.addMethod("clear", &FxLooper::m_clear);
    obj.addMethod("adjust", &FxLooper::m_adjust);
    obj.addMethod("pause", &FxLooper::m_pause);
    obj.addMethod("smooth", &FxLooper::m_smooth);
}
