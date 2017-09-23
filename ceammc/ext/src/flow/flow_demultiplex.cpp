#include "flow_demultiplex.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

static const size_t MIN_INLETS = 2;
static const size_t MAX_INLETS = 24;

FlowDemultiplex::FlowDemultiplex(const PdArgs& args)
    : BaseObject(args)
    , index_(0)
{
    size_t n = clip((size_t)positionalFloatArgument(0, 2), MIN_INLETS, MAX_INLETS);
    index_ = new SizeTProperty("@index", 0);
    createProperty(index_);

    for (size_t i = 1; i < n; i++)
        createInlet();

    createOutlet();
}

void FlowDemultiplex::onBang()
{
    if (0 != index_->value())
        return;

    bangTo(0);
}

void FlowDemultiplex::onFloat(float f)
{
    if (0 != index_->value())
        return;

    floatTo(0, f);
}

void FlowDemultiplex::onSymbol(t_symbol* s)
{
    if (0 != index_->value())
        return;

    symbolTo(0, s);
}

void FlowDemultiplex::onList(const AtomList& l)
{
    onInlet(0, l);
}

void FlowDemultiplex::onAny(t_symbol* sel, const AtomList& args)
{
    if (0 != index_->value())
        return;

    anyTo(0, sel, args);
}

void FlowDemultiplex::onData(const DataPtr& ptr)
{
    if (0 != index_->value())
        return;

    dataTo(0, ptr);
}

void FlowDemultiplex::onInlet(size_t idx, const AtomList& l)
{
    if (idx != index_->value())
        return;

    if (l.empty())
        bangTo(0);
    else if (l.isFloat())
        floatTo(0, l[0].asFloat());
    else if (l.isSymbol())
        symbolTo(0, l[0].asSymbol());
    else if (l[0].isData())
        atomTo(0, Atom(l[0].getData()));
    else
        listTo(0, l);
}

void setup_flow_demultiplex()
{
    ObjectFactory<FlowDemultiplex> obj("flow.demultiplex");
    obj.addAlias("flow.demux");
}
