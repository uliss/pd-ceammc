#include "flow_gate.h"
#include "ceammc_factory.h"

FlowGate::FlowGate(const PdArgs& args)
    : BaseObject(args)
{
    state_ = new BoolProperty("@state", false);
    state_->setArgIndex(0);
    addProperty(state_);

    createInlet();
    createOutlet();
}

void FlowGate::onBang()
{
    if (!state_->value())
        return;

    bangTo(0);
}

void FlowGate::onFloat(t_float f)
{
    if (!state_->value())
        return;

    floatTo(0, f);
}

void FlowGate::onSymbol(t_symbol* s)
{
    if (!state_->value())
        return;

    symbolTo(0, s);
}

void FlowGate::onList(const AtomList& l)
{
    if (!state_->value())
        return;

    listTo(0, l);
}

void FlowGate::onAny(t_symbol* s, const AtomListView& l)
{
    if (!state_->value())
        return;

    anyTo(0, s, l);
}

void FlowGate::onData(const Atom& data)
{
    if (!state_->value())
        return;

    atomTo(0, data);
}

void FlowGate::onInlet(size_t n, const AtomListView& l)
{
    if (n != 1)
        return;

    state_->set(l);
}

bool FlowGate::processAnyProps(t_symbol* s, const AtomListView& l)
{
    return false;
}

void setup_flow_gate()
{
    ObjectFactory<FlowGate> obj("flow.gate");
    obj.addAlias("gate");

    obj.setXletsInfo({ "any: input data flow", "int: 1 or 0 to open/close  the gate" }, { "any: output data flow" });
}
