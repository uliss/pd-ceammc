#include "flow_multiplex.h"
#include "ceammc_factory.h"

#include <array>

constexpr size_t DEF_INLETS = 2;
constexpr size_t MIN_NCHAN = 2;
constexpr size_t MAX_NCHAN = 24;

static std::array<std::string, MAX_NCHAN> inlets_info {};

FlowMultiplex::FlowMultiplex(const PdArgs& args)
    : BaseObject(args)
    , index_(nullptr)
    , total_(positionalConstant<DEF_INLETS, MIN_NCHAN, MAX_NCHAN>(0))
    , ctrl_({ this })
    , control_(&ctrl_)
{
    inlets_.reserve(total_ - 1);

    for (size_t i = 1; i < total_; i++) {
        inlets_.emplace_back(this, i);
        inlet_new(owner(), &inlets_.back().x_obj, nullptr, nullptr);
    }

    inlet_new(owner(), &control_.x_obj, nullptr, nullptr);
    createOutlet();

    index_ = new IntProperty("@index", 0);
    index_->checkClosedRange(0, MAX_NCHAN - 1);
    index_->setArgIndex(1);
    addProperty(index_);
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

void FlowMultiplex::onList(const AtomListView& lv)
{
    onInlet(0, lv);
}

void FlowMultiplex::onAny(t_symbol* sel, const AtomListView& lv)
{
    if (0 != index_->value())
        return;

    anyTo(0, sel, lv);
}

void FlowMultiplex::onData(const Atom& data)
{
    if (0 != index_->value())
        return;

    atomTo(0, data);
}

void FlowMultiplex::onInlet(size_t idx, const AtomListView& lv)
{
    if (idx != index_->value())
        return;

    if (lv.empty())
        bangTo(0);
    else if (lv.isFloat())
        floatTo(0, lv[0].asFloat());
    else if (lv.isSymbol())
        symbolTo(0, lv[0].asSymbol());
    else if (lv[0].isData())
        atomTo(0, lv[0]);
    else
        listTo(0, lv);
}

const char* FlowMultiplex::annotateInlet(size_t n) const
{
    if (n < total_ && n < inlets_info.size())
        return inlets_info[n].c_str();
    else if (n == total_)
        return "int: set input inlet";
    else
        return nullptr;
}

void FlowMultiplex::proxy_any(int idx, t_symbol* s, const AtomListView& lv)
{
    for (size_t i = 0; i < inlets_.size(); i++) {
        if (idx == index_->value())
            return anyTo(0, s, lv);
    }
}

void FlowMultiplex::proxy_float(t_float f)
{
    index_->setValue(f);
}

void FlowMultiplex::initXletInfo()
{
    char buf[32];
    for (size_t i = 0; i < inlets_info.size(); i++) {
        snprintf(buf, sizeof(buf), "input\\[%d\\]", (int)i);
        inlets_info[i] = buf;
    }
}

void setup_flow_multiplex()
{
    FlowMultiplex::initXletInfo();

    ObjectFactory<FlowMultiplex> obj("flow.multiplex");
    obj.addAlias("flow.mux");
    obj.noPropsDispatch();
    obj.noInletsDispatch();

    InletProxy<FlowMultiplex>::init();
    InletProxy<FlowMultiplex>::set_any_callback(&FlowMultiplex::proxy_any);
    FlowMultiplex::ControlProxy::init();
    FlowMultiplex::ControlProxy::set_float_callback(&FlowMultiplex::ControlInlet::on_float);

    obj.addOutletInfo("any: multiplexed flow");

    obj.setDescription("control flow multiplexer");
    obj.setCategory("flow");
    obj.setKeywords({"multiplex"});
}
