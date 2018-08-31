#ifndef FLOW_DEMULTIPLEX_H
#define FLOW_DEMULTIPLEX_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowDemultiplex : public BaseObject {
    SizeTProperty* index_;
    FlagProperty* no_props_;

public:
    FlowDemultiplex(const PdArgs& a);

    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* s, const AtomList& l) override;
    void onData(const DataPtr& d) override;
    void onInlet(size_t n, const AtomList& l) override;

    bool processAnyProps(t_symbol* sel, const AtomList& lst) override;

    bool checkIndex() const;
};

void setup_flow_demultiplex();

#endif // FLOW_DEMULTIPLEX_H
