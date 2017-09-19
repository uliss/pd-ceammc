#include "flow_gate.h"
#include "ceammc_factory.h"

static t_symbol* PROP_STATE = gensym("@state");
static t_symbol* PROP_GET_STATE = gensym("@state?");

FlowGate::FlowGate(const PdArgs& args)
    : BaseObject(args)
{
    state_ = new IntProperty("@state", positionalFloatArgument(0, 0));
    createProperty(state_);

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

void FlowGate::onAny(t_symbol* s, const AtomList& l)
{
    if (!state_->value())
        return;

    anyTo(0, s, l);
}

void FlowGate::onData(const DataPtr& ptr)
{
    if (!state_->value())
        return;

    dataTo(0, ptr);
}

void FlowGate::onInlet(size_t n, const AtomList& l)
{
    if (n != 1)
        return;

    state_->set(l);
}

bool FlowGate::processAnyProps(t_symbol* s, const AtomList& l)
{
    // get
    if (s == PROP_GET_STATE) {
        anyTo(0, PROP_STATE, state_->value());
        return true;
    }

    // set
    if (s == PROP_STATE) {
        state_->set(l);
        return true;
    }

    // pass thru others
    return false;
}

void setup_flow_gate()
{
    ObjectFactory<FlowGate> obj("flow.gate");
    obj.addAlias("gate");
}
