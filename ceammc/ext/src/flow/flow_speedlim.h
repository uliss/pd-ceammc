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
    void onBang();
    void onFloat(t_float f);
    void onSymbol(t_symbol* s);
    void onList(const AtomList& l);
    void onAny(t_symbol* s, const AtomList& l);
    void onData(const DataPtr& p);

    void m_reset(t_symbol*, const AtomList&);

    void accept();
    void clock_handler();
};

void setup_flow_speedlim();

#endif // FLOW_SPEEDLIM_H
