#include "flow_multiplex.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

constexpr size_t DEF_INLETS = 2;
constexpr size_t MIN_NCHAN = 2;
constexpr size_t MAX_NCHAN = 24;

FlowMultiplex::FlowMultiplex(const PdArgs& args)
    : BaseObject(args)
    , index_(0)
{
    index_ = new SizeTProperty("@index", 0);
    addProperty(index_);

    const size_t N = positionalConstant<DEF_INLETS, MIN_NCHAN, MAX_NCHAN>(0);
    for (size_t i = 1; i < N; i++)
        createInlet();

    createOutlet();
}

void FlowMultiplex::onBang()
{
    if (0 != index_->value())
        return;

    bangTo(0);
}

void FlowMultiplex::onFloat(t_float f)
{
    if (0 != index_->value())
        return;

    floatTo(0, f);
}

void FlowMultiplex::onSymbol(t_symbol* s)
{
    if (0 != index_->value())
        return;

    symbolTo(0, s);
}

void FlowMultiplex::onList(const AtomList& l)
{
    onInlet(0, l);
}

void FlowMultiplex::onAny(t_symbol* sel, const AtomListView& args)
{
    if (0 != index_->value())
        return;

    anyTo(0, sel, args);
}

void FlowMultiplex::onData(const Atom& data)
{
    if (0 != index_->value())
        return;

    atomTo(0, data);
}

void FlowMultiplex::onInlet(size_t idx, const AtomList& l)
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
        atomTo(0, l[0]);
    else
        listTo(0, l);
}

void setup_flow_multiplex()
{
    ObjectFactory<FlowMultiplex> obj("flow.multiplex");
    obj.addAlias("flow.mux");
}
