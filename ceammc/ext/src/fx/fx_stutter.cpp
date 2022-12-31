#include "fx_stutter.h"
#include "ceammc_factory.h"

constexpr auto PROP_MAXSIZE_MIN = 1;
constexpr auto PROP_MAXSIZE_DEF = 1000;
constexpr auto PROP_MAXSIZE_MAX = 10000;
constexpr auto PROP_T_MIN = PROP_MAXSIZE_MIN;
constexpr auto PROP_T_DEF = 50;
constexpr auto PROP_T_MAX = PROP_MAXSIZE_MAX;

namespace {
size_t ms2samp(t_sample ms, t_sample sr)
{
    return ms * sr * 0.001;
}

}

FxStutter::FxStutter(const PdArgs& args)
    : SoundExternal(args)
    , state_(ST_PASS)
    , max_size_(nullptr)
    , t_(nullptr)
    , clock_([this]() {
        state_ = ST_FROM_FX;
    })
{
    createInlet();

    createSignalOutlet();

    max_size_ = new FloatProperty("@maxsize", PROP_MAXSIZE_DEF);
    max_size_->setUnits(PropValueUnits::MSEC);
    max_size_->checkClosedRange(PROP_MAXSIZE_MIN, PROP_MAXSIZE_MAX);
    max_size_->setInitOnly();
    addProperty(max_size_);

    t_ = new FloatProperty("@t", PROP_T_DEF);
    t_->checkClosedRange(PROP_T_MIN, PROP_T_MAX);
    t_->setArgIndex(0);
    t_->setUnits(PropValueUnits::MSEC);
    addProperty(t_);
}

void FxStutter::initDone()
{
    adjustBufferSize();
    t_->checkClosedRange(PROP_T_MIN, max_size_->value());
}

void FxStutter::onInlet(size_t n, const AtomListView& lv)
{
    if (lv.empty()) {
        state_ = ST_TO_FX;
        clock_.delay(t_->value() * 2);
    } else {
        clock_.unset();
        const bool on = lv.boolAt(0, false);
        switch (state_) {
        case ST_TO_FX:
            if (!on)
                state_ = ST_FROM_FX;
            break;
        case ST_FROM_FX:
            if (on)
                state_ = ST_TO_FX;
            break;
        case ST_FX:
            if (!on)
                state_ = ST_FROM_FX;
            break;
        case ST_PASS:
        default:
            if (on)
                state_ = ST_TO_FX;
            break;
        }
    }
}

void FxStutter::setupDSP(t_signal** sig)
{
    SoundExternal::setupDSP(sig);

    adjustBufferSize();
    buf_phase_ = 0;
}

void FxStutter::processBlock(const t_sample** in, t_sample** out)
{
    const auto bs = blockSize();
    const size_t n = std::min<size_t>(buffer_.size(), ms2samp(t_->value(), samplerate()));

    switch (state_) {
    case ST_PASS:

        for (size_t i = 0; i < bs; i++) {
            out[0][i] = in[0][i];

            buffer_[buf_phase_] = in[0][i];
            buf_phase_ = (buf_phase_ + 1) % n;
        }

        break;
    case ST_TO_FX:

        for (size_t i = 0; i < bs; i++) {
            auto in0 = buffer_[buf_phase_];
            auto in1 = in[0][i];

            const auto k = t_sample(i) / (bs - 1);
            out[0][i] = (1 - k) * in0;
            buffer_[buf_phase_] = k * in1;
            buf_phase_ = (buf_phase_ + 1) % n;
        }

        state_ = ST_FX;

        break;
    case ST_FROM_FX:

        for (size_t i = 0; i < bs; i++) {
            auto in0 = in[0][i];
            auto in1 = buffer_[buf_phase_];

            const auto k = t_sample(i) / (bs - 1);
            out[0][i] = k * in0;
            buffer_[buf_phase_] = (1 - k) * in1;
            buf_phase_ = (buf_phase_ + 1) % n;
        }

        state_ = ST_PASS;

        break;
    case ST_FX:
        for (size_t i = 0; i < bs; i++) {
            out[0][i] = buffer_[buf_phase_];
            buf_phase_ = (buf_phase_ + 1) % n;
        }

        break;
    }
}

void FxStutter::adjustBufferSize()
{
    const size_t N = ms2samp(max_size_->value(), samplerate());
    if (buffer_.size() != N)
        buffer_.assign(N, 0);
}

void setup_fx_stutter_tilde()
{
    SoundExternalFactory<FxStutter> obj("fx.stutter~");
    obj.setXletsInfo(
        { "signal: input", "1|0: turn fx on/off\n"
                           "bang: run fx twice" },
        { "signal: output" });
}
