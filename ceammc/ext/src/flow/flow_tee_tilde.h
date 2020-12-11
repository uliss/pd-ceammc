#ifndef FLOW_TEE_TILDE_H
#define FLOW_TEE_TILDE_H

#include "ceammc_sound_external.h"

using namespace ceammc;

class FlowTeeTilde : public SoundExternal {
public:
    FlowTeeTilde(const PdArgs& a);
    void onBang() override;
    void onSymbol(t_symbol* s) override;
    void onFloat(t_float f) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* s, const AtomListView& l) override;
    void processBlock(const t_sample** in, t_sample** out) override;

    bool processAnyProps(t_symbol*, const AtomListView&) override;
};

void setup_flow_tee_tilde();

#endif // FLOW_TEE_TILDE_H
