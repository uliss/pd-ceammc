#include <cmath>

#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "pan_spread.h"

#include <cassert>

constexpr int DEF_NCHAN = 2;
constexpr int MIN_NCHAN = 2;
constexpr int MAX_NCHAN = 16;
constexpr t_float SPREAD_DEFAULT = 1;
constexpr t_float CENTER_DEFAULT = 0;

PanSpread::PanSpread(const PdArgs& args)
    : SoundExternal(args)
    , channels_(nullptr)
    , compensate_(nullptr)
{
    spread_.setTargetValue(SPREAD_DEFAULT);
    center_.setTargetValue(CENTER_DEFAULT);

    channels_ = new IntProperty("@ch", DEF_NCHAN);
    channels_->checkClosedRange(MIN_NCHAN, MAX_NCHAN);
    channels_->setArgIndex(0);
    channels_->setReadOnly();
    addProperty(channels_);

    createCbFloatProperty(
        "@center",
        [this]() -> t_float { return center_.target(); },
        [this](t_float v) -> bool { center_.setTargetValue(v); return true; })
        ->setFloatCheck(PropValueConstraints::CLOSED_RANGE, -0.5, 0.5);

    createCbFloatProperty(
        "@spread",
        [this]() -> t_float { return spread_.target(); },
        [this](t_float f) -> bool { spread_.setTargetValue(f); return true; })
        ->setFloatCheck(PropValueConstraints::CLOSED_RANGE, 0, 1);

    compensate_ = new BoolProperty("@compensate", false);
    addProperty(compensate_);

    createCbProperty("@coeffs", &PanSpread::propCoeffs);
}

void PanSpread::initDone()
{
    BaseObject::initDone();

    const size_t N = channels_->value();
    assert(N > 1);

    // calc coefficents
    positions_.assign(N, 0);
    coefs_l_.assign(N, 0);
    coefs_r_.assign(N, 0);
    // default block size
    block_l_.assign(64, 0);
    block_r_.assign(64, 0);

    // create inlets
    for (size_t i = 1; i < N; i++)
        createSignalInlet();

    createSignalOutlet();
    createSignalOutlet();

    spread_.setDurationMs(5, samplerate());
    center_.setDurationMs(5, samplerate());

    calcCoefficents();
}

void PanSpread::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    block_l_.resize(blockSize());
    block_r_.resize(blockSize());
    spread_.setDurationMs(5, samplerate());
    center_.setDurationMs(5, samplerate());
}

void PanSpread::processBlock(const t_sample** in, t_sample** out)
{
    const size_t bs = blockSize();
    const size_t N = channels_->value();

    calcCoefficents();

    std::fill(block_l_.begin(), block_l_.end(), 0);
    std::fill(block_r_.begin(), block_r_.end(), 0);

    for (size_t chan = 0; chan < N; chan++) {

        const t_float cl = coefs_l_[chan];
        const t_float cr = coefs_r_[chan];

        for (size_t i = 0; i < bs; i++) {
            t_float v = in[chan][i];
            block_l_[i] += v * cl;
            block_r_[i] += v * cr;
        }
    }

    std::copy(block_l_.begin(), block_l_.begin() + bs, out[0]);
    std::copy(block_r_.begin(), block_r_.begin() + bs, out[1]);
}

void PanSpread::calcCoefficents()
{
    static const t_float SMALL_FLOAT = 0.0001f;

    const size_t N = channels_->value();
    const t_float len = spread_();
    const t_float y0 = center_() + 0.5 - len / 2;

    const t_float comp = compensate_->value() ? (1.f / N) : 1;

    // calc coefficents
    for (size_t i = 0; i < N; i++) {
        t_float pos = clip<t_float>(convert::lin2lin<t_float>(t_float(i) / (N - 1), 0, 1, y0, y0 + len), 0, 1);

        auto left = cosf(pos * M_PI_2) * comp;
        auto right = sinf(pos * M_PI_2) * comp;

        if (left <= SMALL_FLOAT)
            left = 0.f;
        if (right <= SMALL_FLOAT)
            right = 0.f;

        coefs_l_[i] = left;
        coefs_r_[i] = right;
    }
}

AtomList PanSpread::propCoeffs() const
{
    const size_t N = std::min<size_t>(coefs_l_.size(), channels_->value());
    AtomList res;
    res.reserve(N * 2);

    for (size_t i = 0; i < N; i++) {
        res.append(Atom(coefs_l_[i]));
        res.append(Atom(coefs_r_[i]));
    }

    return res;
}

void setup_pan_spread()
{
    SoundExternalFactory<PanSpread> obj("pan.spread~");
}
