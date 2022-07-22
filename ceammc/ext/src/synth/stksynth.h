#ifndef STKSYNTH_H
#define STKSYNTH_H

#include "ceammc_sound_external.h"

using namespace ceammc;

namespace stk {
class Instrmnt;
}

class StkBase : public SoundExternal {
protected:
    stk::Instrmnt* synth_;

public:
    StkBase(const PdArgs& args, stk::Instrmnt* instr);
    ~StkBase() override;

    void setupDSP(t_signal** sp) override;
    void processBlock(const t_sample**, t_sample** out) override;

    void m_cc(t_symbol* s, const AtomListView& lv);
    void controlChange(int n, t_float val);

    template <class T>
    T* synth() { return static_cast<T*>(synth_); }

public:
    static void initRawWaves(t_class* c);
};

class StkSynth : public StkBase {
    FloatProperty* freq_;
    t_float gate_;

public:
    StkSynth(const PdArgs& args, stk::Instrmnt* instr);

    void onList(const AtomListView& lv) override;
    bool propSetGate(t_float f);

    void m_note(t_symbol* s, const AtomListView& lv);
};

template <class T>
class ControlChangeProperty : public Property {
    StkBase& synth_;
    int channel_;
    t_float value_;

public:
    ControlChangeProperty(const char* name, int ch, StkBase& synth)
        : Property(PropertyInfo(name, PropValueType::INTEGER))
        , synth_(synth)
        , channel_(ch)
        , value_(0)
    {
        info().setDefault(int(0));
        setIntCheck(PropValueConstraints::CLOSED_RANGE, 0, 127);
        value_ = synth_.synth<T>()->getControlChange(ch);
    }

    AtomList get() const override
    {
        return Atom(value_);
    }

    bool setList(const AtomListView& lv) override
    {
        if (lv.empty())
            return false;

        value_ = lv[0].asFloat();
        synth_.controlChange(channel_, value_);
        return true;
    }
};

#endif // STKSYNTH_H
