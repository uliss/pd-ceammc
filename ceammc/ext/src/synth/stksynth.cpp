#include "ADSR.h"
#include "Instrmnt.h"
#include "SineWave.h"

#include "ceammc_convert.h"
#include "stksynth.h"

#include "stksynth_p.h"

StkBase::StkBase(const PdArgs& args, stk::Instrmnt* instr)
    : SoundExternal(args)
    , synth_(instr)
{
    createSignalOutlet();
}

StkBase::~StkBase()
{
    delete synth_;
}

void StkBase::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    stk::Stk::setSampleRate(samplerate());
}

void StkBase::processBlock(const t_sample**, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t N = synth_->channelsOut();

    if (N == 1) {
        for (size_t i = 0; i < BS; i++)
            out[0][i] = synth_->tick();
    } else {
        for (size_t i = 0; i < BS; i++) {
            out[0][i] = synth_->tick();

            size_t T = std::min<size_t>(numOutputChannels(), N);
            for (size_t j = 0; j < T; j++)
                out[j][i] = synth_->lastFrame()[j];
        }
    }
}

StkSynth::StkSynth(const PdArgs& args, stk::Instrmnt* instr)
    : StkBase(args, instr)
    , freq_(nullptr)
    , gate_(0)
{
    freq_ = new FloatPropertyMin("@freq", mtof(48), 0);
    createProperty(freq_);

    createCbProperty("@gate", &StkSynth::propGate, &StkSynth::propSetGate);
}

AtomList StkSynth::propGate() const
{
    return Atom(gate_);
}

void StkSynth::propSetGate(const AtomList& lst)
{
    if (!checkArgs(lst, ARG_FLOAT)) {
        OBJ_ERR << "gate value expected: " << lst;
        return;
    }

    t_float amp = ceammc::clip<t_float>(lst.floatAt(0, 60), 0, 1);

    if (amp > 0)
        synth_->noteOn(freq_->value(), amp);
    else
        synth_->noteOff(freq_->value());

    gate_ = amp;
}

void StkBase::m_cc(t_symbol* s, const AtomList& lst)
{
    if (!checkArgs(lst, ARG_INT, ARG_FLOAT, s))
        return;

    int ctl_num = lst[0].asInt();
    stk::StkFloat ctl_val = lst[1].asFloat();

    OBJ_DBG << lst;

    synth_->controlChange(ctl_num, ctl_val);
}

class FreqGetter : public stk::SineWave {
public:
    FreqGetter(const stk::SineWave* ptr)
        : stk::SineWave(*ptr)
    {
    }

    t_float freq() const
    {
        return (rate_ * stk::Stk::sampleRate()) / TABLE_SIZE;
    }
};

class ADSRGetter : public stk::ADSR {
public:
    ADSRGetter(const stk::ADSR* ptr)
        : stk::ADSR(*ptr)
    {
    }

    t_float target() const
    {
        return target_;
    }
};

SinWaveFreq::SinWaveFreq(const stk::SineWave* ptr)
    : ptr_(ptr)
{
}

t_float SinWaveFreq::getFrequency() const
{
    return FreqGetter(ptr_).freq();
}

ADSRProp::ADSRProp(const stk::ADSR* ptr)
    : ptr_(ptr)
{
}

t_float ADSRProp::getTarget() const
{
    return ADSRGetter(ptr_).target();
}
