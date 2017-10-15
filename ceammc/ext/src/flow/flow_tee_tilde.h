#ifndef FLOW_TEE_TILDE_H
#define FLOW_TEE_TILDE_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowTeeTilde : public SoundExternal {
public:
    FlowTeeTilde(const PdArgs& a);
    void onBang();
    void onSymbol(t_symbol* s);
    void onFloat(float f);
    void onList(const AtomList& l);
    void onAny(t_symbol* s, const AtomList& l);
    void processBlock(const t_sample** in, t_sample** out);

    bool processAnyProps(t_symbol*, const AtomList&);
};

extern "C" void setup_flow0x2etee_tilde();

#endif // FLOW_TEE_TILDE_H
