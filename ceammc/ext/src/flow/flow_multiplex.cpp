#include "flow_multiplex.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

static const size_t MIN_OUTLETS = 2;
static const size_t MAX_OUTLETS = 24;

FlowMultiplex::FlowMultiplex(const PdArgs& a)
    : BaseObject(a)
    , index_(0)
{
    createInlet();

    size_t n = clip(positionalArguments().size(), MIN_OUTLETS, MAX_OUTLETS);
    index_ = new SizeTProperty("@index", n);
    createProperty(index_);

    for (size_t i = 0; i < n; i++)
        createOutlet();
}

void FlowMultiplex::onBang()
{
    if (!checkIndex())
        return;

    bangTo(index_->value());
}

void FlowMultiplex::onFloat(t_float f)
{
    if (!checkIndex())
        return;

    floatTo(index_->value(), f);
}

void FlowMultiplex::onSymbol(t_symbol* s)
{
    if (!checkIndex())
        return;

    symbolTo(index_->value(), s);
}

void FlowMultiplex::onList(const AtomList& l)
{
    if (!checkIndex())
        return;

    listTo(index_->value(), l);
}

void FlowMultiplex::onAny(t_symbol* s, const AtomList& l)
{
    if (!checkIndex())
        return;

    anyTo(index_->value(), s, l);
}

void FlowMultiplex::onData(const DataPtr& d)
{
    if (!checkIndex())
        return;

    dataTo(index_->value(), d);
}

bool FlowMultiplex::checkIndex() const
{
    if (index_->value() >= numOutlets()) {
        OBJ_ERR << "invalid index: " << index_->value();
        return false;
    }

    return true;
}

void setup_flow_multiplex()
{
    ObjectFactory<FlowMultiplex> obj("flow.multiplex");
    obj.addAlias("flow.mux");
}
