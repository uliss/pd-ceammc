#ifndef FLOW_SPEEDLIM_H
#define FLOW_SPEEDLIM_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
#include "ceammc_proxy.h"

using namespace ceammc;

class FlowSpeedLimit : public BaseObject {
    IntProperty* period_;
    bool is_closed_;
    ClockMemberFunction<FlowSpeedLimit> clock_;
    InletProxy<FlowSpeedLimit> inlet_;

public:
    FlowSpeedLimit(const PdArgs& a);
    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* s, const AtomListView& lv) override;
    void onData(const Atom& data) override;

public:
    void onInletBang(int) { onInletReset({}); }
    void onInletFloat(int, t_float f) { period_->setValue(f); }
    void onInletReset(const AtomListView&);

    void accept();
    void clock_handler();
};

void setup_flow_speedlim();

#endif // FLOW_SPEEDLIM_H
