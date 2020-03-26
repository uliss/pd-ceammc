#ifndef FLOW_MULTIPLEX_H
#define FLOW_MULTIPLEX_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowMultiplex : public BaseObject {
    SizeTProperty* index_;

public:
    FlowMultiplex(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* sel, const AtomList& args) override;
    void onData(const DataPtr& ptr) override;
    void onInlet(size_t idx, const AtomList& l) override;
};

void setup_flow_multiplex();

#endif // FLOW_MULTIPLEX_H
