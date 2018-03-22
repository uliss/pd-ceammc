#include "flow_speedlim.h"
#include "ceammc_factory.h"

#include <cassert>

FlowSpeedLimit::FlowSpeedLimit(const PdArgs& a)
    : BaseObject(a)
    , period_(0)
    , is_closed_(false)
    , clock_(this, &FlowSpeedLimit::clock_handler)
{
    createOutlet();

    period_ = new IntProperty("@limit", positionalFloatArgument(0, 0));
    createProperty(period_);
}

void FlowSpeedLimit::onBang()
{
    if (is_closed_)
        return;

    accept();
    bangTo(0);
}

void FlowSpeedLimit::onFloat(t_float f)
{
    if (is_closed_)
        return;

    accept();
    floatTo(0, f);
}

void FlowSpeedLimit::onSymbol(t_symbol* s)
{
    if (is_closed_)
        return;

    accept();
    symbolTo(0, s);
}

void FlowSpeedLimit::onList(const AtomList& l)
{
    if (is_closed_)
        return;

    accept();
    listTo(0, l);
}

void FlowSpeedLimit::onAny(t_symbol* s, const AtomList& l)
{
    if (is_closed_)
        return;

    accept();
    anyTo(0, s, l);
}

void FlowSpeedLimit::onData(const DataPtr& p)
{
    if (is_closed_)
        return;

    accept();
    dataTo(0, p);
}

void FlowSpeedLimit::accept()
{
    if (period_->value() < 1)
        return;

    is_closed_ = true;
    clock_.delay(period_->value());
}

void FlowSpeedLimit::clock_handler()
{
    is_closed_ = false;
}

void FlowSpeedLimit::m_reset(t_symbol*, const AtomList&)
{
    is_closed_ = false;
}

void setup_flow_speedlim()
{
    ObjectFactory<FlowSpeedLimit> obj("flow.speedlim");
    obj.addAlias("speedlim");
    obj.addMethod("reset", &FlowSpeedLimit::m_reset);
}
