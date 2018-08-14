#ifndef FLOW_DEMULTIPLEX_H
#define FLOW_DEMULTIPLEX_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowDemultiplex : public BaseObject {
    SizeTProperty* index_;

public:
    FlowDemultiplex(const PdArgs& a);

    void onBang();
    void onFloat(t_float f);
    void onSymbol(t_symbol* s);
    void onList(const AtomList& l);
    void onAny(t_symbol* s, const AtomList& l);
    void onData(const DataPtr& d);

    bool checkIndex() const;
};

void setup_flow_demultiplex();

#endif // FLOW_DEMULTIPLEX_H
