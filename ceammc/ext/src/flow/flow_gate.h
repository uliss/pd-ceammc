#ifndef FLOW_GATE_H
#define FLOW_GATE_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowGate : public BaseObject {
    BoolProperty* state_;

public:
    FlowGate(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* s, const AtomListView& l) override;
    void onData(const Atom& data) override;

    void onInlet(size_t n, const AtomListView& l) override;

    bool processAnyProps(t_symbol* s, const AtomListView& l) override;
};

void setup_flow_gate();

#endif // FLOW_GATE_H
