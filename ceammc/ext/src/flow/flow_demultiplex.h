#ifndef FLOW_DEMULTIPLEX_H
#define FLOW_DEMULTIPLEX_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowDemultiplex : public BaseObject {
    SizeTProperty* index_ { nullptr };
    IntProperty* n_ { nullptr };

public:
    FlowDemultiplex(const PdArgs& a);
    void initDone() final;

    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomListView& lv) override;
    void onAny(t_symbol* s, const AtomListView& lv) override;
    void onData(const Atom& d) override;
    void onInlet(size_t n, const AtomListView& l) override;

    bool processAnyProps(t_symbol* sel, const AtomListView& lv) override;
    const char* annotateOutlet(size_t n) const override;

    bool checkIndex() const;
};

void setup_flow_demultiplex();

#endif // FLOW_DEMULTIPLEX_H
