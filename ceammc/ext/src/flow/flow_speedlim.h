#ifndef FLOW_SPEEDLIM_H
#define FLOW_SPEEDLIM_H

#include "ceammc_clock.h"
#include "ceammc_object.h"

using namespace ceammc;

class FlowSpeedLimit : public BaseObject {
    IntProperty* period_;
    bool is_closed_;
    ClockMemberFunction<FlowSpeedLimit> clock_;

public:
    FlowSpeedLimit(const PdArgs& a);
    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* s, const AtomListView& l) override;
    void onData(const Atom& data) override;

    void m_reset(t_symbol*, const AtomListView&);

    void accept();
    void clock_handler();
};

void setup_flow_speedlim();

#endif // FLOW_SPEEDLIM_H
