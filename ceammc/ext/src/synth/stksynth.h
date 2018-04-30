#ifndef STKSYNTH_H
#define STKSYNTH_H

#include "ceammc_sound_external.h"

using namespace ceammc;

namespace stk {
class Instrmnt;
}

class StkSynth;

class ControlChangeProperty : public Property {
    StkSynth& synth_;
    int channel_;

public:
    ControlChangeProperty(const char* name, int ch, StkSynth& synth);

    bool set(const AtomList& lst) override;
};

class StkSynth : public SoundExternal {
protected:
    stk::Instrmnt* synth_;

private:
    FloatPropertyMin* freq_;
    t_float gate_;

public:
    StkSynth(const PdArgs& args, stk::Instrmnt* instr);
    ~StkSynth() override;

    void setupDSP(t_signal** sp) override;
    void processBlock(const t_sample**, t_sample** out) override;

    AtomList propGate() const;
    void propSetGate(const AtomList& lst);

    void m_cc(t_symbol* s, const AtomList& lst);
};

#endif // STKSYNTH_H
