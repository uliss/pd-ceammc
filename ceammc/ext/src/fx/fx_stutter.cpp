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
constexpr auto PROP_ENV_ATTACK_DEF = 5;
constexpr auto PROP_ENV_RELEASE_DEF = PROP_T_DEF - PROP_ENV_ATTACK_DEF;

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
    , clock_([this]() {
        state_ = ST_PASS;
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

    {
        DataTypeEnv env;
        env.setAR(PROP_ENV_ATTACK_DEF * 1000, PROP_ENV_RELEASE_DEF * 1000);
        env_ = new DataPropertyT<DataTypeEnv>("@env", env);
        env_->setSuccessFn([this](Property*) {
            env_->value().render(env_buf_.begin(), env_buf_.end());
        });
        addProperty(env_);
        env_->callSuccessFn(); // to update env_buf_
    }
}

void FxStutter::initDone()
{
    adjustBufferSize();
    t_->checkClosedRange(PROP_T_MIN, max_size_->value());
}

void FxStutter::onInlet(size_t n, const AtomListView& lv)
{
    if (lv.empty()) {
        state_ = ST_FX;
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
}

void FxStutter::processBlock(const t_sample** in, t_sample** out)
{
    const auto BS = blockSize();
    const size_t N = stutterSizeSamp();

    if (N < 2)
        return;

    t_sample buf_out[BS];

    if (state_ == ST_PASS) {
        for (size_t i = 0; i < BS; i++) {
            out[0][i] = in[0][i]; // copy to output buffer

            buffer_[write_phase_] = in[0][i];
            (++write_phase_) %= N;
        }
    } else {
        const auto speed = speed_->value();
        switch (crc32_hash(mode_->value())) {
        case hash_back:
            if (speed == 1) {
                for (size_t i = 0; i < BS; i++) {
                    auto phase = (write_phase_ + ((N - 1) - read_phase_++)) % N;
                    buf_out[i] = buffer_[phase] * curveValueAt(read_phase_ % N);
                }
            } else {
                for (size_t i = 0; i < BS; i++) {
                    auto phase = (write_phase_ + ((N - 1) - (read_phase_++ * speed)));
                    auto p0 = size_t(std::ceil(phase)) % N;
                    auto p1 = (N + p0 - 1) % N;
                    auto x0 = buffer_[p0];
                    auto x1 = buffer_[p1];
                    buf_out[i] = interpolate::linear(x0, x1, frac_part(phase)) * curveValueAt(read_phase_ % N);
                }
            }

            // adjust phase
            read_phase_ %= N;

            break;
        case hash_tri:

            if (speed == 1) {
                for (size_t i = 0; i < BS; i++) {
                    auto phase = N - std::abs(std::int64_t((write_phase_ + read_phase_++) % (2 * N)) - std::int64_t(N));
                    buf_out[i] = buffer_[phase] * curveValueAt(read_phase_ % N);
                }
            } else {
                for (size_t i = 0; i < BS; i++) {
                    auto phase = N - std::abs(std::int64_t(std::fmod(write_phase_ + (read_phase_++ * speed), (2 * N))) - std::int64_t(N));
                    auto p0 = size_t(std::ceil(phase)) % N;
                    auto p1 = (N + p0 - 1) % N;
                    auto x0 = buffer_[p0];
                    auto x1 = buffer_[p1];
                    buf_out[i] = interpolate::linear(x0, x1, frac_part(phase)) * curveValueAt(read_phase_ % N);
                }
            }

            // adjust phase
            read_phase_ %= (2 * N);

            break;
        case hash_fwd:
        default:
            if (speed == 1) {
                for (size_t i = 0; i < BS; i++) {
                    auto phase = (write_phase_ + read_phase_++) % N;
                    buf_out[i] = buffer_[phase] * curveValueAt(read_phase_ % N);
                }
            } else {
                for (size_t i = 0; i < BS; i++) {
                    auto phase = (write_phase_ + (read_phase_++ * speed));
                    auto p0 = size_t(phase) % N;
                    auto p1 = (p0 + 1) % N;
                    auto x0 = buffer_[p0];
                    auto x1 = buffer_[p1];
                    buf_out[i] = interpolate::linear(x0, x1, frac_part(phase)) * curveValueAt(read_phase_ % N);
                }
            }

            // adjust phase
            read_phase_ %= N;

            break;
        }
    }

    if (state_ == ST_FROM_FX) {
        for (size_t i = 0; i < BS; i++) {
            const auto k = t_sample(i) / (BS - 1);
            out[0][i] = interpolate::linear(buf_out[i], in[0][i], k);
        }

        state_ = ST_PASS;
        read_phase_ = 0;
    } else if (state_ == ST_TO_FX) {
        for (size_t i = 0; i < BS; i++) {
            const auto k = t_sample(i) / (BS - 1);
            out[0][i] = interpolate::linear(in[0][i], buf_out[i], k);
        }

        state_ = ST_FX;
        read_phase_ = 0;
    } else if (state_ == ST_FX) {
        for (size_t i = 0; i < BS; i++)
            out[0][i] = buf_out[i];
    }
}

void FxStutter::onDataT(const EnvAtom& env)
{
    if (env->empty()) {
        env_->value().clear();
    } else {
        env_->setValue(*env);
        env_->callSuccessFn();
    }
}

void FxStutter::adjustBufferSize()
{
    const size_t N = ms2samp(max_size_->value(), samplerate());
    if (buffer_.size() != N)
        buffer_.assign(N, 0);
}

float FxStutter::curveValueAt(size_t pos) const
{
    if (env_->value().empty())
        return 1;

    const auto N = stutterSizeSamp();
    const auto BS = env_buf_.size();
    auto t = pos / double(N - 1);

    size_t idx = t * (BS - 1);
    auto x0 = (idx < BS) ? env_buf_[idx] : 0;
    auto x1 = (idx + 1 < BS) ? env_buf_[idx + 1] : 0;

    return interpolate::linear<float>(x0, x1, t);
}

void setup_fx_stutter_tilde()
{
    SoundExternalFactory<FxStutter> obj("fx.stutter~");
    obj.setXletsInfo(
        { "signal: input", "1|0: turn fx on/off\n"
                           "bang: run fx twice" },
        { "signal: output" });
    obj.processData<DataTypeEnv>();

    obj.setDescription("stutter effect");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "stutter", "glitch" });
}
