#include "fx_looper.h"
#include "ceammc_factory.h"

#include <algorithm>
#include <boost/static_assert.hpp>

static const float DEFAULT_CAPACITY_SEC = 5;
static const float MAX_CAPACITY_SEC = 120;

static t_symbol* states[] = {
    gensym("init"),
    gensym("record"),
    gensym("overdub"),
    gensym("play"),
    gensym("stop")
};

BOOST_STATIC_ASSERT((sizeof(states) / sizeof(states[0])) == (STATE_STOP + 1));

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
    , loop_bang_(0)
    , xfade_(0)
    , max_samples_(0)
    , loop_len_(0)
    , play_phase_(0)
    , rec_phase_(0)
{
    capacity_sec_ = new FloatProperty("@capacity", positionalFloatArgument(0, DEFAULT_CAPACITY_SEC));
    createProperty(capacity_sec_);

    loop_bang_ = new BoolProperty("@loop_bang", false);
    createProperty(loop_bang_);

    xfade_ = new BoolProperty("@xfade", false);
    createProperty(xfade_);

    createCbProperty("@length", &FxLooper::p_length);
    createCbProperty("@play_pos", &FxLooper::p_play_pos);
    createCbProperty("@play_phase", &FxLooper::p_play_phase);
    createCbProperty("@state", &FxLooper::p_state);

    createSignalOutlet();

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
    case STATE_INIT:
        fillZero8(out, bs);
        break;
    case STATE_PLAY: {

        const size_t samples_left = loop_len_ - play_phase_;

        // enough samples until loop end
        if (samples_left >= bs) {

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
            for (size_t i = 0; i < samples_left; i++)
                out[0][i] = buffer_[play_phase_ + i];

            play_phase_ = 0;

            // bang on new cycle
            if (loop_bang_->value())
                bangTo(0);

            for (size_t i = samples_left; i < bs; i++)
                out[0][i] = buffer_[play_phase_ + i];

            play_phase_ += (bs - samples_left);
        }

    } break;
    case STATE_REC: {

        const size_t samples_left = max_samples_ - rec_phase_;

        if (samples_left >= bs) {
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
            state_ = STATE_STOP;
            loop_len_ = rec_phase_;
        }

    } break;
    case STATE_DUB: {

        const size_t samples_left = loop_len_ - play_phase_;

        // enough samples until loop end
        if (samples_left >= bs) {

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
    } break;
    default:
        break;
    }
}

void FxLooper::m_record(t_symbol*, const AtomList& lst)
{
    OBJ_DBG << "starting loop record: max " << max_samples_ << " samples";
    state_ = STATE_REC;
    rec_phase_ = 0;
}

void FxLooper::m_stop(t_symbol*, const AtomList& lst)
{
    OBJ_DBG << "stopping loop";
    state_ = STATE_STOP;
    play_phase_ = 0;
    rec_phase_ = 0;
}

void FxLooper::m_play(t_symbol*, const AtomList& lst)
{
    switch (state_) {
    case STATE_PLAY:
        OBJ_ERR << "already playing";
        break;
    case STATE_REC: {
        state_ = STATE_PLAY;
        loop_len_ = rec_phase_;
        rec_phase_ = 0;
        play_phase_ = 0;
        OBJ_DBG << "start loop: " << loop_len_ << " samples";

        if (xfade_->value()) {
            for (size_t i = 0; i < 64; i++) {
                size_t idx = 64 - i;
                buffer_[loop_len_ - idx] *= idx / 64.f;
            }

            for (size_t i = 0; i < 64; i++) {
                buffer_[i] *= i / 64.f;
            }
        }

    } break;
    case STATE_STOP:
        state_ = STATE_PLAY;
        play_phase_ = 0;
        break;
    case STATE_DUB:
        state_ = STATE_PLAY;
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
        state_ = STATE_DUB;
        break;
    case STATE_REC:
        state_ = STATE_DUB;
        loop_len_ = rec_phase_;
        rec_phase_ = 0;
        play_phase_ = 0;
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
    return AtomList(t_float(play_phase_) / t_float(loop_len_));
}

AtomList FxLooper::p_state() const
{
    return AtomList(states[state_]);
}

void setup_fx_looper()
{
    SoundExternalFactory<FxLooper> obj("fx.looper~");
    obj.addMethod("record", &FxLooper::m_record);
    obj.addMethod("stop", &FxLooper::m_stop);
    obj.addMethod("play", &FxLooper::m_play);
    obj.addMethod("overdub", &FxLooper::m_overdub);
    obj.addMethod("clear", &FxLooper::m_clear);
    obj.addMethod("adjust", &FxLooper::m_adjust);
}
