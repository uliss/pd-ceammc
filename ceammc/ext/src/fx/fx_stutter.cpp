#include "fx_stutter.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_signal.h"

constexpr auto PROP_MAXSIZE_MIN = 1;
constexpr auto PROP_MAXSIZE_DEF = 1000;
constexpr auto PROP_MAXSIZE_MAX = 10000;
constexpr auto PROP_T_MIN = PROP_MAXSIZE_MIN;
constexpr auto PROP_T_DEF = 50;
constexpr auto PROP_T_MAX = PROP_MAXSIZE_MAX;

CEAMMC_DEFINE_HASH(fwd);
CEAMMC_DEFINE_HASH(back);
CEAMMC_DEFINE_HASH(tri);

namespace {
size_t ms2samp(t_sample ms, t_sample sr)
{
    return ms * sr * 0.001;
}

t_sample frac_part(t_sample f)
{
    t_sample ip;
    return std::modf(f, &ip);
}

}

FxStutter::FxStutter(const PdArgs& args)
    : SoundExternal(args)
    , state_(ST_PASS)
    , max_size_(nullptr)
    , t_(nullptr)
    , mode_(nullptr)
    , speed_(nullptr)
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

    mode_ = new SymbolEnumProperty("@mode", { str_fwd, str_back, str_tri });
    addProperty(mode_);

    speed_ = new FloatProperty("@speed", 1);
    speed_->checkClosedRange(0.25, 4);
    addProperty(speed_);
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

    if (n < 2)
        return;

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
            const auto xin = in[0][i];
            const auto k = t_sample(i) / (bs - 1);
            out[0][i] = (1 - k) * xin; // fade out input

            buffer_[buf_phase_] = k * xin; // fade in record
            buf_phase_ = (buf_phase_ + 1) % n;
        }

        state_ = ST_FX;
        play_phase_ = 0;

        break;
    case ST_FROM_FX:

        for (size_t i = 0; i < bs; i++) {
            const auto xin = in[0][i];
            const auto k = t_sample(i) / (bs - 1);

            out[0][i] = k * xin; // fade in input
            buffer_[buf_phase_] = (1 - k) * xin; // fade out record

            buf_phase_ = (buf_phase_ + 1) % n;
        }

        state_ = ST_PASS;
        play_phase_ = 0;

        break;
    case ST_FX: {
        const auto speed = speed_->value();
        switch (crc32_hash(mode_->value())) {
        case hash_back:
            if (speed == 1) {
                for (size_t i = 0; i < bs; i++) {
                    auto phase = (buf_phase_ + ((n - 1) - play_phase_++)) % n;
                    out[0][i] = buffer_[phase];
                }
            } else {
                for (size_t i = 0; i < bs; i++) {
                    auto phase = (buf_phase_ + ((n - 1) - (play_phase_++ * speed)));
                    auto p0 = size_t(std::ceil(phase)) % n;
                    auto p1 = (n + p0 - 1) % n;
                    auto x0 = buffer_[p0];
                    auto x1 = buffer_[p1];
                    out[0][i] = interpolate::linear(x0, x1, frac_part(phase));
                }
            }

            // adjust phase
            play_phase_ %= n;

            break;
        case hash_tri:

            if (speed == 1) {
                for (size_t i = 0; i < bs; i++) {
                    auto phase = n - std::abs(std::int64_t((buf_phase_ + play_phase_++) % (2 * n)) - std::int64_t(n));
                    out[0][i] = buffer_[phase];
                }
            } else {
                for (size_t i = 0; i < bs; i++) {
                    auto phase = n - std::abs(std::int64_t(std::fmod(buf_phase_ + (play_phase_++ * speed), (2 * n))) - std::int64_t(n));
                    auto p0 = size_t(std::ceil(phase)) % n;
                    auto p1 = (n + p0 - 1) % n;
                    auto x0 = buffer_[p0];
                    auto x1 = buffer_[p1];
                    out[0][i] = interpolate::linear(x0, x1, frac_part(phase));
                }
            }

            // adjust phase
            play_phase_ %= (2 * n);

            break;
        case hash_fwd:
        default:
            if (speed == 1) {
                for (size_t i = 0; i < bs; i++) {
                    auto phase = (buf_phase_ + play_phase_++) % n;
                    out[0][i] = buffer_[phase];
                }
            } else {
                for (size_t i = 0; i < bs; i++) {
                    auto phase = (buf_phase_ + (play_phase_++ * speed));
                    auto p0 = size_t(phase) % n;
                    auto p1 = (p0 + 1) % n;
                    auto x0 = buffer_[p0];
                    auto x1 = buffer_[p1];
                    out[0][i] = interpolate::linear(x0, x1, frac_part(phase));
                }
            }

            // adjust phase
            play_phase_ %= n;

            break;
        }

    } break;
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

    obj.setDescription("stutter effect");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "stutter", "glitch"});
}
