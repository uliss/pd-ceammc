#include "flow_demultiplex.h"
#include "ceammc_factory.h"

constexpr int MIN_OUTLETS = 2;
constexpr int DEF_OUTLETS = 2;
constexpr int MAX_OUTLETS = 24;

using TxtBuf = char[32];
static TxtBuf ANNOT[MAX_OUTLETS] = {};

FlowDemultiplex::FlowDemultiplex(const PdArgs& a)
    : BaseObject(a)
{
    n_ = new IntProperty("@n", DEF_OUTLETS, PropValueAccess::INITONLY);
    n_->checkClosedRange(MIN_OUTLETS, MAX_OUTLETS);
    n_->setArgIndex(0);
    addProperty(n_);

    index_ = new IntProperty("@index", 0);
    index_->checkMinEq(0);
    addProperty(index_);

    createInlet();
}

void FlowDemultiplex::initDone()
{
    auto N = n_->value();
    for (int i = 0; i < N; i++)
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

void FlowDemultiplex::onList(const AtomListView& lv)
{
    if (!checkIndex())
        return;

    listTo(index_->value(), lv);
}

void FlowDemultiplex::onAny(t_symbol* s, const AtomListView& lv)
{
    if (!checkIndex())
        return;

    anyTo(index_->value(), s, lv);
}

void FlowDemultiplex::onData(const Atom& d)
{
    if (!checkIndex())
        return;

    atomTo(index_->value(), d);
}

void FlowDemultiplex::onInlet(size_t /*n*/, const AtomListView& l)
{
    index_->set(l);
}

bool FlowDemultiplex::processAnyProps(t_symbol* sel, const AtomListView& lst)
{
    return false;
}

const char* FlowDemultiplex::annotateOutlet(size_t n) const
{
    if (n < MAX_OUTLETS)
        return ANNOT[n];
    else
        return nullptr;
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
    for (size_t i = 0; i < MAX_OUTLETS; i++)
        snprintf((char*)ANNOT[i], sizeof(ANNOT[0]), "output\\[%ld\\]", i);

    ObjectFactory<FlowDemultiplex> obj("flow.demultiplex");
    obj.addAlias("flow.demux");
    obj.addInletInfo("any: input data flow");
    obj.addInletInfo("int: select output number");

    obj.setDescription("control flow demultiplexer");
    obj.setCategory("flow");
    obj.setKeywords({ "demultiplex" });
}
