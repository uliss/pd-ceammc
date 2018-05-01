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
        synth_->noteOff(freq_->value());

    gate_ = amp;
}

void StkSynth::m_cc(t_symbol* s, const AtomList& lst)
{
    if (!checkArgs(lst, ARG_INT, ARG_FLOAT, s))
        return;

    int ctl_num = lst[0].asInt();
    stk::StkFloat ctl_val = lst[1].asFloat();

    synth_->controlChange(ctl_num, ctl_val);
}

ControlChangeProperty::ControlChangeProperty(const char* name, int ch, StkSynth& synth)
    : Property(name)
    , synth_(synth)
    , channel_(ch)
    , value_(0)
{
}

AtomList ControlChangeProperty::get() const
{
    return Atom(value_);
}

bool ControlChangeProperty::set(const AtomList& lst)
{
    if (lst.empty())
        return false;

    value_ = lst[0].asFloat();
    synth_.m_cc(gensym("cc"), AtomList(channel_, value_));
    return true;
}
