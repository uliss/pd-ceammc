#ifndef FLOW_DEMULTIPLEX_H
#define FLOW_DEMULTIPLEX_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowDemultiplex : public BaseObject {
    SizeTProperty* index_;

public:
    FlowDemultiplex(const PdArgs& args);

    void onBang();
    void onFloat(float f);
    void onSymbol(t_symbol* s);
    void onList(const AtomList& l);
    void onAny(t_symbol* sel, const AtomList& args);
    void onData(const DataPtr& ptr);
    void onInlet(size_t idx, const AtomList& l);
};

void setup_flow_demultiplex();

#endif // FLOW_DEMULTIPLEX_H
