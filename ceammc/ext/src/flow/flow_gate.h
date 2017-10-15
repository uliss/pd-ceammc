#ifndef FLOW_GATE_H
#define FLOW_GATE_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowGate : public BaseObject {
    IntProperty* state_;

public:
    FlowGate(const PdArgs& args);

    void onBang();
    void onFloat(t_float f);
    void onSymbol(t_symbol* s);
    void onList(const AtomList& l);
    void onAny(t_symbol* s, const AtomList& l);
    void onData(const DataPtr& ptr);

    void onInlet(size_t n, const AtomList& l);

    bool processAnyProps(t_symbol* s, const AtomList& l);
};

void setup_flow_gate();

#endif // FLOW_GATE_H
