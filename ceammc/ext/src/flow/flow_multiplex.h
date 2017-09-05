#ifndef FLOW_MULTIPLEX_H
#define FLOW_MULTIPLEX_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowMultiplex : public BaseObject {
    SizeTProperty* index_;

public:
    FlowMultiplex(const PdArgs& a);

    void onBang();
    void onFloat(t_float f);
    void onSymbol(t_symbol* s);
    void onList(const AtomList& l);
    void onAny(t_symbol* s, const AtomList& l);
    void onData(const DataPtr& d);

    bool checkIndex() const;
};

void setup_flow_multiplex();

#endif // FLOW_MULTIPLEX_H
