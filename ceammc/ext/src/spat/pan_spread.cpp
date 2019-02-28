#include "pan_spread.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <boost/math/constants/constants.hpp>
#include <cassert>

PanSpread::PanSpread(const PdArgs& args)
    : SoundExternal(args)
    , channels_(0)
    , compensate_(0)
{
    channels_ = new InitIntPropertyClosedRange(new IntPropertyClosedRange("@ch", 2, 2, 16));
    setPropertyFromPositionalArg(channels_, 0);
    createProperty(channels_);

    createCbProperty("@center", &PanSpread::propCenter, &PanSpread::propSetCenter);
    property("@center")->info().setType(PropertyInfoType::FLOAT);
    createCbProperty("@spread", &PanSpread::propSpread, &PanSpread::propSetSpread);
    property("@spread")->info().setType(PropertyInfoType::FLOAT);
    property("@spread")->info().setDefault(1);
    property("@spread")->info().setRange(0, 1);

    compensate_ = new BoolProperty("@compensate", false);
    createProperty(compensate_);

    createCbProperty("@coeffs", &PanSpread::propCoeffs);

    parseProperties();

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
    for (size_t i = 0; i < N - 1; i++)
        createSignalInlet();

    createSignalOutlet();
    createSignalOutlet();

    spread_.setDurationMs(5, samplerate());
    center_.setDurationMs(5, samplerate());
    spread_.setTargetValue(1);
    center_.setTargetValue(0);
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
    using namespace boost::math::float_constants;

    static const t_float SMALL_FLOAT = 0.0001f;

    const size_t N = channels_->value();
    const t_float len = spread_();
    const t_float y0 = center_() + 0.5 - len / 2;

    const t_float comp = compensate_->value() ? (1.f / N) : 1;

    // calc coefficents
    for (size_t i = 0; i < N; i++) {
        t_float pos = clip<t_float>(convert::lin2lin<t_float>(t_float(i) / (N - 1), 0, 1, y0, y0 + len), 0, 1);

        auto left = cosf(pos * half_pi) * comp;
        auto right = sinf(pos * half_pi) * comp;

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
    const size_t N = channels_->value();
    AtomList res;
    res.reserve(N * 2);

    for (size_t i = 0; i < N; i++) {
        res.append(Atom(coefs_l_[i]));
        res.append(Atom(coefs_r_[i]));
    }

    return res;
}

AtomList PanSpread::propSpread() const
{
    return Atom(spread_.target());
}

void PanSpread::propSetSpread(const AtomList& lst)
{
    t_float v = clip<t_float>(lst.floatAt(0, 0), 0, 1);
    spread_.setTargetValue(v);
}

AtomList PanSpread::propCenter() const
{
    return Atom(center_.target());
}

void PanSpread::propSetCenter(const AtomList& lst)
{
    t_float v = clip<t_float>(lst.floatAt(0, 0), -0.5, 0.5);
    center_.setTargetValue(v);
}

void setup_pan_spread()
{
    SoundExternalFactory<PanSpread> obj("pan.spread~");
}
