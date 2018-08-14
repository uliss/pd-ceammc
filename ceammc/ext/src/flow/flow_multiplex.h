#ifndef FLOW_MULTIPLEX_H
#define FLOW_MULTIPLEX_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowMultiplex : public BaseObject {
    SizeTProperty* index_;

public:
    FlowMultiplex(const PdArgs& args);

    void onBang();
    void onFloat(float f);
    void onSymbol(t_symbol* s);
    void onList(const AtomList& l);
    void onAny(t_symbol* sel, const AtomList& args);
    void onData(const DataPtr& ptr);
    void onInlet(size_t idx, const AtomList& l);
};

void setup_flow_multiplex();

#endif // FLOW_MULTIPLEX_H
