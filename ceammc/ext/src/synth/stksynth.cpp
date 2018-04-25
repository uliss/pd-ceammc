#include "stksynth.h"
#include "Instrmnt.h"
#include "ceammc_convert.h"

StkSynth::StkSynth(const PdArgs& args, stk::Instrmnt* instr)
    : SoundExternal(args)
    , synth_(instr)
    , freq_(nullptr)
    , gate_(0)
{
    freq_ = new FloatPropertyMin("@freq", mtof(48), 0);
    createProperty(freq_);

    createCbProperty("@gate", &StkSynth::propGate, &StkSynth::propSetGate);

    createSignalOutlet();
}

StkSynth::~StkSynth()
{
    delete synth_;
}

void StkSynth::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    stk::Stk::setSampleRate(samplerate());
}

void StkSynth::processBlock(const t_sample**, t_sample** out)
{
    const size_t BS = blockSize();

    for (size_t i = 0; i < BS; i++)
        out[0][i] = synth_->tick();
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
        synth_->noteOff(amp);

    gate_ = amp;
}
