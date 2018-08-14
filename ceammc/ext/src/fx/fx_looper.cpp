#include "fx_looper.h"
#include "ceammc_factory.h"

#include <algorithm>
#include <boost/static_assert.hpp>

static const float DEFAULT_CAPACITY_SEC = 5;
static const float MAX_CAPACITY_SEC = 120;

static t_symbol* states[] = {
    gensym("init"),
    gensym("record"),
    &s_,
    &s_,
    &s_,
    gensym("overdub"),
    &s_,
    &s_,
    gensym("pause"),
    gensym("play"),
    &s_,
    &s_,
    gensym("stop"),
    &s_
};

BOOST_STATIC_ASSERT((sizeof(states) / sizeof(states[0])) == (STATE_STOP_XFADE_PLAY + 1));

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
    auto e = std::end(containter);
    size_t N = e - b;

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
    , x_rec_to_stop_(nullptr)
    , x_rec_to_dub_(nullptr)
    , x_play_to_dub_(nullptr)
    , x_dub_to_play_(nullptr)
    , smooth_ms_(nullptr)
    , max_samples_(0)
    , loop_len_(0)
    , play_phase_(0)
    , rec_phase_(0)
    , clock_(this, &FxLooper::clockTick)
{
    capacity_sec_ = new FloatProperty("@capacity", positionalFloatArgument(0, DEFAULT_CAPACITY_SEC));
    createProperty(capacity_sec_);

    loop_bang_ = new BoolProperty("@loop_bang", false);
    createProperty(loop_bang_);

    smooth_ms_ = new FloatPropertyMinEq("@smooth", 10, 0);
    createProperty(smooth_ms_);

    x_play_to_stop_ = new LinFadeoutProperty("@play_to_stop_time", 10);
    createProperty(x_play_to_stop_);

    x_play_to_dub_ = new LinFadeinProperty("@play_to_dub_time", 10);
    createProperty(x_play_to_dub_);

    x_stop_to_play_ = new LinFadeinProperty("@stop_to_play_time", 10);
    createProperty(x_stop_to_play_);

    x_rec_to_play_ = new PowXFadeProperty("@rec_to_play_time", 30);
    createProperty(x_rec_to_play_);

    x_rec_to_stop_ = new LinFadeoutProperty("@rec_to_stop_time", 10);
    createProperty(x_rec_to_stop_);

    x_rec_to_dub_ = new LinFadeinProperty("@rec_to_dub_time", 10);
    createProperty(x_rec_to_dub_);

    x_dub_to_play_ = new LinFadeoutProperty("@dub_to_play_time", 20);
    createProperty(x_dub_to_play_);

    round_ = new IntProperty("@round", 0);
    createProperty(round_);

    createCbProperty("@length", &FxLooper::p_length);
    createCbProperty("@play_pos", &FxLooper::p_play_pos);
    createCbProperty("@play_phase", &FxLooper::p_play_phase);
    createCbProperty("@state", &FxLooper::p_state);

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

    max_samples_ = capacity_sec_->value() * sys_getsr();

    buffer_.resize(max_samples_);
    OBJ_DBG << "max loop length " << max_samples_ << " samples";
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
        stateStopToPlay(out);
        break;
    case STATE_INIT:
        fillZero8(out, bs);
        break;
    case STATE_PLAY_XFADE_STOP:
        statePlayToStop(out);
        break;
    case STATE_PLAY:
        statePlay(out);
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

void FxLooper::stateDub(const t_sample** in, t_sample** out)
{
    const size_t bs = blockSize();
    const size_t samples_left = loop_len_ - play_phase_;

    // enough samples until loop end
    if (samples_left >= bs) {
        CHECK_PHASE(play_phase_ + bs - 1, buffer_);

        for (size_t i = 0; i < bs; i++) {
            // copy input
            t_sample tmp = in[0][i];
            // output current
            out[0][i] = buffer_[play_phase_ + i];
            // overdub
            buffer_[play_phase_ + i] += tmp;
        }

        play_phase_ += bs;

    } else {
        CHECK_PHASE(play_phase_ + samples_left - 1, buffer_);

        for (size_t i = 0; i < samples_left; i++) {
            // copy input
            t_sample tmp = in[0][i];
            // output current
            out[0][i] = buffer_[play_phase_ + i];
            // overdub
            buffer_[play_phase_ + i] += tmp;
        }

        // reset phase
        play_phase_ = 0;
        loopCycleFinish();

        for (size_t i = samples_left; i < bs; i++) {
            // copy input
            t_sample tmp = in[0][i];
            // output current
            out[0][i] = buffer_[play_phase_ + i];
            // overdub
            buffer_[play_phase_ + i] += tmp;
        }

        // move phase
        play_phase_ += (bs - samples_left);
    }
}

void FxLooper::stateDubToStop(const t_sample** in, t_sample** out)
{
    if (x_rec_to_stop_->isRunning()) {
        const size_t bs = blockSize();
        const size_t samples_left = loop_len_ - play_phase_;

        // enough samples until loop end
        if (samples_left >= bs) {
            CHECK_PHASE(play_phase_ + bs - 1, buffer_);

            for (size_t i = 0; i < bs; i++) {
                auto amp = x_rec_to_stop_->amp() * 0.5;
                // copy input
                t_sample tmp = amp * in[0][i];
                // output current
                out[0][i] = amp * buffer_[play_phase_ + i];
                // overdub
                buffer_[play_phase_ + i] += tmp;

                // move fader phase
                x_rec_to_stop_->next();
            }

            play_phase_ += bs;

        } else {
            CHECK_PHASE(play_phase_ + samples_left - 1, buffer_);

            for (size_t i = 0; i < samples_left; i++) {
                auto amp = x_rec_to_stop_->amp() * 0.5;
                // copy input
                t_sample tmp = amp * in[0][i];
                // output current
                out[0][i] = amp * buffer_[play_phase_ + i];
                // overdub
                buffer_[play_phase_ + i] += tmp;
                // move fader phase
                x_rec_to_stop_->next();
            }

            // reset phase
            play_phase_ = 0;
            loopCycleFinish();

            for (size_t i = samples_left; i < bs; i++) {
                auto amp = x_rec_to_stop_->amp() * 0.5;
                // copy input
                t_sample tmp = amp * in[0][i];
                // output current
                out[0][i] = amp * buffer_[play_phase_ + i];
                // overdub
                buffer_[play_phase_ + i] += tmp;
                // move fader phase
                x_rec_to_stop_->next();
            }

            // move phase
            play_phase_ += (bs - samples_left);
        }
    } else {
        state_ = STATE_STOP;
        stateStop(out);
    }
}

void FxLooper::stateDubToPlay(const t_sample** in, t_sample** out)
{
    if (x_dub_to_play_->isRunning()) {
        const size_t bs = blockSize();
        const size_t samples_left = loop_len_ - play_phase_;

        // enough samples until loop end
        if (samples_left >= bs) {
            CHECK_PHASE(play_phase_ + bs - 1, buffer_);

            for (size_t i = 0; i < bs; i++) {
                auto amp = x_dub_to_play_->amp();
                // copy input
                t_sample tmp = amp * in[0][i];
                // output current
                out[0][i] = amp * buffer_[play_phase_ + i];
                // overdub
                buffer_[play_phase_ + i] += tmp;

                // move fader phase
                x_dub_to_play_->next();
            }

            play_phase_ += bs;

        } else {
            CHECK_PHASE(play_phase_ + samples_left - 1, buffer_);

            for (size_t i = 0; i < samples_left; i++) {
                auto amp = x_dub_to_play_->amp();
                // copy input
                t_sample tmp = amp * in[0][i];
                // output current
                out[0][i] = amp * buffer_[play_phase_ + i];
                // overdub
                buffer_[play_phase_ + i] += tmp;
                // move fader phase
                x_dub_to_play_->next();
            }

            // reset phase
            play_phase_ = 0;
            loopCycleFinish();

            for (size_t i = samples_left; i < bs; i++) {
                auto amp = x_dub_to_play_->amp();
                // copy input
                t_sample tmp = amp * in[0][i];
                // output current
                out[0][i] = amp * buffer_[play_phase_ + i];
                // overdub
                buffer_[play_phase_ + i] += tmp;
                // move fader phase
                x_dub_to_play_->next();
            }

            // move phase
            play_phase_ += (bs - samples_left);
        }
    } else {
        state_ = STATE_PLAY;
        statePlay(out);
    }
}

void FxLooper::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);

    max_samples_ = capacity_sec_->value() * sys_getsr();
    buffer_.resize(max_samples_);

    x_play_to_stop_->calc(samplerate(), blockSize());
    x_stop_to_play_->calc(samplerate(), blockSize());
    x_rec_to_play_->calc(samplerate(), blockSize());
    x_rec_to_stop_->calc(samplerate(), blockSize());

    x_rec_to_dub_->calc(samplerate(), blockSize());
    x_play_to_dub_->calc(samplerate(), blockSize());
    x_dub_to_play_->calc(samplerate(), blockSize());
}

void FxLooper::stateStop(t_sample** out)
{
    fillZero8(out, blockSize());
}

void FxLooper::statePlay(t_sample** out)
{
    const size_t bs = blockSize();
    const size_t samples_left = loop_len_ - play_phase_;

    // enough samples until loop end
    if (samples_left >= bs) {
        CHECK_PHASE(play_phase_ + bs - 1, buffer_);

        // manual loop unrolling
        for (size_t i = 0; i < bs; i += 8) {
            out[0][i + 0] = buffer_[play_phase_ + i + 0];
            out[0][i + 1] = buffer_[play_phase_ + i + 1];
            out[0][i + 2] = buffer_[play_phase_ + i + 2];
            out[0][i + 3] = buffer_[play_phase_ + i + 3];
            out[0][i + 4] = buffer_[play_phase_ + i + 4];
            out[0][i + 5] = buffer_[play_phase_ + i + 5];
            out[0][i + 6] = buffer_[play_phase_ + i + 6];
            out[0][i + 7] = buffer_[play_phase_ + i + 7];
        }

        play_phase_ += bs;
    } else {
        CHECK_PHASE((play_phase_ + samples_left) - 1, buffer_);

        for (size_t i = 0; i < samples_left; i++)
            out[0][i] = buffer_[play_phase_ + i];

        loopCycleFinish();

        CHECK_PHASE(play_phase_ + bs - 1, buffer_);

        for (size_t i = samples_left; i < bs; i++)
            out[0][i] = buffer_[play_phase_ + i];

        play_phase_ += (bs - samples_left);
    }
}

void FxLooper::statePlayToStop(t_sample** out)
{
    const size_t bs = blockSize();

    if (x_play_to_stop_->isRunning()) {
        const size_t samples_left = loop_len_ - play_phase_;

        // enough samples until loop end
        // loop in the middle
        if (samples_left >= bs) {
            CHECK_PHASE(play_phase_ + bs - 1, buffer_);

            for (size_t i = 0; i < bs; i++) {
                auto amp = x_play_to_stop_->amp();
                // move phase
                x_play_to_stop_->next();

                out[0][i] = amp * buffer_[play_phase_ + i];
            }

            play_phase_ += bs;

        } else {
            CHECK_PHASE(play_phase_ + samples_left - 1, buffer_);

            // perform loop crossing
            for (size_t i = 0; i < samples_left; i++) {
                auto amp = x_play_to_stop_->amp();
                // move phase
                x_play_to_stop_->next();

                out[0][i] = amp * buffer_[play_phase_ + i];
            }

            loopCycleFinish();

            CHECK_PHASE(play_phase_ + bs - 1, buffer_);

            for (size_t i = samples_left; i < bs; i++) {
                auto amp = x_play_to_stop_->amp();
                // move phase
                x_play_to_stop_->next();

                out[0][i] = amp * buffer_[play_phase_ + i];
            }

            play_phase_ += (bs - samples_left);
        }
    } else {
        state_ = STATE_STOP;
        fillZero8(out, bs);
    }
}

void FxLooper::statePlayToDub(const t_sample** in, t_sample** out)
{
    const size_t bs = blockSize();
    const size_t samples_left = loop_len_ - play_phase_;

    // overdub fadein
    if (x_play_to_dub_->isRunning()) {
        // enough samples until loop end
        if (samples_left >= bs) {
            CHECK_PHASE(play_phase_ + bs - 1, buffer_);

            for (size_t i = 0; i < bs; i++) {
                // get amp
                auto amp = x_play_to_dub_->amp();
                // move phase
                x_play_to_dub_->next();

                // copy input
                t_sample tmp = in[0][i] * amp;
                // output current
                out[0][i] = buffer_[play_phase_ + i];
                // overdub
                buffer_[play_phase_ + i] += tmp;
            }

            play_phase_ += bs;

        } else {
            CHECK_PHASE(play_phase_ + samples_left - 1, buffer_);

            for (size_t i = 0; i < samples_left; i++) {
                // linear fadein
                auto amp = x_play_to_dub_->amp();
                // move phase
                x_play_to_dub_->next();
                // copy input
                t_sample tmp = in[0][i] * amp;
                // output current
                out[0][i] = buffer_[play_phase_ + i];
                // overdub
                buffer_[play_phase_ + i] += tmp;
            }

            // reset phase
            play_phase_ = 0;
            loopCycleFinish();

            for (size_t i = samples_left; i < bs; i++) {
                // linear fadein
                auto amp = x_play_to_dub_->amp();
                // move phase
                x_play_to_dub_->next();
                // copy input
                t_sample tmp = in[0][i] * amp;
                // output current
                out[0][i] = buffer_[play_phase_ + i];
                // overdub
                buffer_[play_phase_ + i] += tmp;
            }

            // move phase
            play_phase_ += (bs - samples_left);
        }
    } else {
        state_ = STATE_DUB;
        stateDub(in, out);
    }
}

void FxLooper::stateStopToPlay(t_sample** out)
{
    const size_t bs = blockSize();

    if (x_stop_to_play_->isRunning()) {
        const size_t samples_left = loop_len_ - play_phase_;

        // enough samples until loop end
        // loop in the middle
        if (samples_left >= bs) {
            CHECK_PHASE(play_phase_ + bs - 1, buffer_);

            for (size_t i = 0; i < bs; i++) {
                auto amp = x_stop_to_play_->amp();
                // move phase
                x_stop_to_play_->next();

                out[0][i] = amp * buffer_[play_phase_ + i];
            }

            play_phase_ += bs;

        } else {
            CHECK_PHASE((play_phase_ + samples_left) - 1, buffer_);

            // perform loop crossing
            for (size_t i = 0; i < samples_left; i++) {
                auto amp = x_stop_to_play_->amp();
                // move phase
                x_stop_to_play_->next();

                out[0][i] = amp * buffer_[play_phase_ + i];
            }

            loopCycleFinish();

            CHECK_PHASE(play_phase_ + bs - 1, buffer_);

            for (size_t i = samples_left; i < bs; i++) {
                auto amp = x_stop_to_play_->amp();
                // move phase
                x_stop_to_play_->next();

                out[0][i] = amp * buffer_[play_phase_ + i];
            }

            play_phase_ += (bs - samples_left);
        }
    } else {
        state_ = STATE_PLAY;
        statePlay(out);
    }
}

void FxLooper::stateRecord(const t_sample** in, t_sample** out)
{
    const size_t bs = blockSize();
    const size_t samples_left = max_samples_ - rec_phase_;

    if (samples_left >= bs) {
        CHECK_PHASE(rec_phase_ + bs - 1, buffer_);

        for (size_t i = 0; i < bs; i += 8) {
            // record
            buffer_[rec_phase_ + i + 0] = in[0][i + 0];
            buffer_[rec_phase_ + i + 1] = in[0][i + 1];
            buffer_[rec_phase_ + i + 2] = in[0][i + 2];
            buffer_[rec_phase_ + i + 3] = in[0][i + 3];
            buffer_[rec_phase_ + i + 4] = in[0][i + 4];
            buffer_[rec_phase_ + i + 5] = in[0][i + 5];
            buffer_[rec_phase_ + i + 6] = in[0][i + 6];
            buffer_[rec_phase_ + i + 7] = in[0][i + 7];

            // output zero
            out[0][i + 0] = 0;
            out[0][i + 1] = 0;
            out[0][i + 2] = 0;
            out[0][i + 3] = 0;
            out[0][i + 4] = 0;
            out[0][i + 5] = 0;
            out[0][i + 6] = 0;
            out[0][i + 7] = 0;
        }

        rec_phase_ += bs;

    } else {
        // reach max loop length
        state_ = STATE_STOP;
        loop_len_ = rec_phase_;
        rec_phase_ = 0;
        play_phase_ = 0;

        const size_t N = std::abs(smooth_ms_->value() * samplerate() * 0.001f);
        applyLinFadeIn(buffer_, N);
        applyLinFadeOut(buffer_, loop_len_, N);

        fillZero8(out, bs);
        loopCycleFinish();
    }
}

void FxLooper::stateRecordToPlay(const t_sample** in, t_sample** out)
{
    if (x_rec_to_play_->isRunning()) {
        const size_t bs = blockSize();

        CHECK_PHASE(play_phase_ + bs - 1, buffer_);

        for (size_t i = 0; i < bs; i++) {
            auto& buf = buffer_[play_phase_ + i];
            // record
            buf *= x_rec_to_play_->amp2();
            buf += x_rec_to_play_->amp() * in[0][i];
            out[0][i] = buf * x_stop_to_play_->amp();

            // move phase
            x_rec_to_play_->next();
            x_stop_to_play_->next();
        }

        play_phase_ += bs;
    } else {
        // move to PLAY state
        state_ = STATE_PLAY;
        statePlay(out);
    }
}

void FxLooper::stateRecordToStop(const t_sample** in, t_sample** out)
{
    // fades
    const size_t N = std::abs(smooth_ms_->value() * samplerate() * 0.001f);
    applyLinFadeIn(buffer_, N);
    applyLinFadeOut(buffer_, loop_len_, N);

    state_ = STATE_STOP;
    rec_phase_ = 0;
    play_phase_ = 0;

    stateStop(out);
    loopCycleFinish();
}

void FxLooper::stateRecordToDub(const t_sample** in, t_sample** out)
{
    const size_t bs = blockSize();
    const size_t samples_left = loop_len_ - play_phase_;

    // enough samples until loop end
    if (samples_left >= bs) {
        CHECK_PHASE(play_phase_ + bs - 1, buffer_);

        for (size_t i = 0; i < bs; i++) {
            // linear fadein
            auto amp = t_sample(i) / t_sample(bs);
            // copy input
            t_sample tmp = in[0][i] * amp;
            // output current
            out[0][i] = buffer_[play_phase_ + i];
            // overdub
            buffer_[play_phase_ + i] += tmp;
        }

        play_phase_ += bs;

    } else {
        CHECK_PHASE(play_phase_ + samples_left - 1, buffer_);

        for (size_t i = 0; i < samples_left; i++) {
            // linear fadein
            auto amp = t_sample(i) / t_sample(bs);
            // copy input
            t_sample tmp = in[0][i] * amp;
            // output current
            out[0][i] = buffer_[play_phase_ + i];
            // overdub
            buffer_[play_phase_ + i] += tmp;
        }

        // reset phase
        play_phase_ = 0;
        loopCycleFinish();

        for (size_t i = samples_left; i < bs; i++) {
            // linear fadein
            auto amp = t_sample(i) / t_sample(bs);
            // copy input
            t_sample tmp = in[0][i] * amp;
            // output current
            out[0][i] = buffer_[play_phase_ + i];
            // overdub
            buffer_[play_phase_ + i] += tmp;
        }

        // move phase
        play_phase_ += (bs - samples_left);
    }

    state_ = STATE_DUB;
}

void FxLooper::m_record(t_symbol*, const AtomList& lst)
{
    OBJ_DBG << "starting loop record: max " << max_samples_ << " samples ("
            << float(max_samples_) / samplerate() << " sec)";

    state_ = STATE_REC;
    rec_phase_ = 0;
    play_phase_ = 0;
}

void FxLooper::m_stop(t_symbol*, const AtomList& lst)
{
    switch (state_) {
    case STATE_PLAY:
        state_ = STATE_PLAY_XFADE_STOP;
        x_play_to_stop_->calc(samplerate(), blockSize());
        break;
    case STATE_REC: {
        state_ = STATE_REC_XFADE_STOP;
        x_rec_to_stop_->calc(samplerate(), blockSize());
        loop_len_ = rec_phase_;

        OBJ_DBG << "recorded loop (" << float(loop_len_) / samplerate() << " sec)";

    } break;
    case STATE_DUB:
        state_ = STATE_DUB_XFADE_STOP;
        play_phase_ = 0;
        rec_phase_ = 0;
        x_rec_to_stop_->calc(samplerate(), blockSize());
        break;
    case STATE_PAUSE:
    default:
        state_ = STATE_STOP;
        play_phase_ = 0;
        rec_phase_ = 0;
        break;
    }
}

void FxLooper::m_pause(t_symbol* s, const AtomList& lst)
{
    switch (state_) {
    case STATE_PAUSE:
        METHOD_ERR(s) << "already paused";
        break;
    case STATE_INIT:
    case STATE_STOP:
        METHOD_ERR(s) << "is not running";
        break;
    case STATE_REC:
        state_ = STATE_PAUSE;
        loop_len_ = rec_phase_;
        break;
    default:
        state_ = STATE_PAUSE;
        break;
    }
}

void FxLooper::m_play(t_symbol*, const AtomList& lst)
{
    switch (state_) {
    case STATE_PLAY:
        OBJ_ERR << "already playing";
        break;
    case STATE_REC: {
        state_ = STATE_REC_XFADE_PLAY;
        x_rec_to_play_->calc(samplerate(), blockSize());
        x_stop_to_play_->calc(samplerate(), blockSize());

        loop_len_ = rec_phase_;
        // loop align
        if (round_->value() > 0) {
            const size_t ALIGN = round_->value();
            size_t div = loop_len_ % ALIGN;

            if (div < ALIGN / 2) {
                // round to minimal align
                OBJ_DBG << "shorten loop by " << div << "samples";
                loop_len_ = (loop_len_ / ALIGN) * ALIGN;
            }
        }

        rec_phase_ = 0;
        play_phase_ = 0;

        OBJ_DBG << "playing recorded loop: " << loop_len_ << " samples ("
                << float(loop_len_) / samplerate() << " sec)";

    } break;
    case STATE_STOP:
        state_ = STATE_STOP_XFADE_PLAY;
        x_stop_to_play_->calc(samplerate(), blockSize());
        play_phase_ = 0;
        break;
    case STATE_PAUSE:
        state_ = STATE_PLAY;
        break;
    case STATE_DUB:
        state_ = STATE_DUB_XFADE_PLAY;
        x_dub_to_play_->calc(samplerate(), blockSize());
        break;
    case STATE_INIT:
        OBJ_DBG << "no loop recorded";
        break;
    default:
        OBJ_DBG << "unhandled state: " << state_;
        break;
    }
}

void FxLooper::m_overdub(t_symbol*, const AtomList& lst)
{
    switch (state_) {
    case STATE_INIT:
    case STATE_STOP:
        OBJ_ERR << "loop is not running...";
        break;
    case STATE_DUB:
        OBJ_ERR << "already overdubbing...";
        break;
    case STATE_PLAY:
        state_ = STATE_PLAY_XFADE_DUB;
        x_play_to_dub_->calc(samplerate(), blockSize());
        break;
    case STATE_REC:
        state_ = STATE_DUB;
        rec_phase_ = 0;
        play_phase_ = 0;
        x_rec_to_dub_->calc(samplerate(), blockSize());
        OBJ_DBG << "start loop dub: " << loop_len_ << " samples";
        break;
    default:
        OBJ_DBG << "unhandled state: " << state_;
        break;
    }
}

void FxLooper::m_clear(t_symbol*, const AtomList& lst)
{
    buffer_.assign(buffer_.size(), 0);
    loop_len_ = 0;
    rec_phase_ = 0;
    play_phase_ = 0;
}

void FxLooper::m_adjust(t_symbol* s, const AtomList& lst)
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

AtomList FxLooper::p_length() const
{
    return AtomList(loop_len_ / sys_getsr());
}

AtomList FxLooper::p_play_pos() const
{
    return AtomList(play_phase_ / sys_getsr());
}

AtomList FxLooper::p_play_phase() const
{
    if (loop_len_ == 0)
        return Atom(0.f);

    return AtomList(t_float(play_phase_) / t_float(loop_len_));
}

AtomList FxLooper::p_state() const
{
    return AtomList(states[state_]);
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
}

XFadeProperty::XFadeProperty(const std::string& name, float ms)
    : FloatProperty(name, ms)
    , length_(0)
    , phase_(0)
{
}

void XFadeProperty::calc(size_t sr, size_t bs)
{
    if (bs == 0)
        return;

    auto samples = std::round(0.001f * sr * std::fabs(value()));
    length_ = (size_t(samples) / bs) * bs;
    phase_ = 0;
}

void XFadeProperty::reset()
{
    phase_ = 0;
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

t_float PowXFadeProperty::amp2() const
{
    auto p = length_ - phase_;
    return (-1 * double(p * p) / double(length_ * length_)) + 1;
}
