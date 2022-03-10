#ifndef FLOW_MULTIPLEX_H
#define FLOW_MULTIPLEX_H

#include "ceammc_object.h"
#include "ceammc_proxy.h"
using namespace ceammc;

class FlowMultiplex : public BaseObject {
public:
    struct ControlInlet {
        FlowMultiplex* pimpl;
        void on_float(int, t_float f) { pimpl->proxy_float(f); }
    };

    using Inlet = InletProxy<FlowMultiplex>;
    using ControlProxy = InletProxy<ControlInlet>;

private:
    IntProperty* index_;
    const size_t total_;
    std::vector<Inlet> inlets_;
    ControlInlet ctrl_;
    ControlProxy control_;

public:
    FlowMultiplex(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* sel, const AtomListView& lv) override;
    void onData(const Atom& data) override;
    void onInlet(size_t idx, const AtomListView& lv) override;

    const char* annotateInlet(size_t n) const override;

    void proxy_any(Inlet* x, t_symbol* s, const AtomListView& lv);
    void proxy_float(t_float f);

public:
    static void initXletInfo();
};

void setup_flow_multiplex();

#endif // FLOW_MULTIPLEX_H
