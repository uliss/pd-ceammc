#include "flow_demultiplex.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

static const size_t MIN_OUTLETS = 2;
static const size_t MAX_OUTLETS = 24;

FlowDemultiplex::FlowDemultiplex(const PdArgs& a)
    : BaseObject(a)
    , index_(0)
{
    size_t n = clip((size_t)positionalFloatArgument(0, 2), MIN_OUTLETS, MAX_OUTLETS);
    index_ = new SizeTProperty("@index", 0);
    createProperty(index_);

    for (size_t i = 0; i < n; i++)
        createOutlet();
}

void FlowDemultiplex::onBang()
{
    if (!checkIndex())
        return;

    bangTo(index_->value());
}

void FlowDemultiplex::onFloat(t_float f)
{
    if (!checkIndex())
        return;

    floatTo(index_->value(), f);
}

void FlowDemultiplex::onSymbol(t_symbol* s)
{
    if (!checkIndex())
        return;

    symbolTo(index_->value(), s);
}

void FlowDemultiplex::onList(const AtomList& l)
{
    if (!checkIndex())
        return;

    listTo(index_->value(), l);
}

void FlowDemultiplex::onAny(t_symbol* s, const AtomList& l)
{
    if (!checkIndex())
        return;

    anyTo(index_->value(), s, l);
}

void FlowDemultiplex::onData(const DataPtr& d)
{
    if (!checkIndex())
        return;

    dataTo(index_->value(), d);
}

bool FlowDemultiplex::checkIndex() const
{
    if (index_->value() >= numOutlets()) {
        OBJ_ERR << "invalid index: " << index_->value();
        return false;
    }

    return true;
}

void setup_flow_demultiplex()
{
    ObjectFactory<FlowDemultiplex> obj("flow.demultiplex");
    obj.addAlias("flow.demux");
}
