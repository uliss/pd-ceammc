/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "flow_dup.h"
#include "ceammc_factory.h"

static t_class* inlet_proxy_class;
struct t_proxy {
    t_pd x_obj;
    FlowDup* dest;

    t_proxy(FlowDup* d)
        : dest(d)
    {
        x_obj = inlet_proxy_class;
    }
};

void inlet_proxy_float(t_proxy* x, t_float f)
{
    t_atom a;
    SETFLOAT(&a, f);
    x->dest->setProperty("@delay", AtomListView(&a, 1));
}

void inlet_proxy_any(t_proxy* x, t_symbol* s, int argc, t_atom* argv)
{
    if (s == gensym("reset"))
        x->dest->reset();
    else
        LogPdObject(x, LOG_ERROR).stream() << "invalid message: " << s << " " << AtomListView(argv, argc);
}

FlowDup::FlowDup(const PdArgs& a)
    : BaseObject(a)
    , delay_(nullptr)
    , block_(nullptr)
    , clock_([this]() { messageTo(0, msg_); in_process_ = false; })
    , inlet_proxy_(new t_proxy(this))
    , in_process_(false)
{
    inlet_new(owner(), &inlet_proxy_->x_obj, nullptr, &s_);

    createOutlet();

    delay_ = new FloatProperty("@delay", 0);
    delay_->checkMinEq(-1);
    delay_->setArgIndex(0);
    delay_->setUnits(PropValueUnits::MSEC);
    addProperty(delay_);

    block_ = new BoolProperty("@block", false);
    addProperty(block_);
}

FlowDup::~FlowDup()
{
    delete inlet_proxy_;
}

void FlowDup::onInlet(size_t n, const AtomList& l)
{
    delay_->set(l);
}

void FlowDup::onBang()
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();

    bangTo(0);
    msg_.setBang();
    delay();
}

void FlowDup::onFloat(t_float f)
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();

    floatTo(0, f);
    msg_.setFloat(f);
    delay();
}

void FlowDup::onSymbol(t_symbol* s)
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();

    symbolTo(0, s);
    msg_.setSymbol(s);
    delay();
}

void FlowDup::onData(const Atom& a)
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();

    atomTo(0, a);
    msg_.setList(a);
    delay();
}

void FlowDup::onList(const AtomList& l)
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();

    listTo(0, l);
    msg_.setList(l);
    delay();
}

void FlowDup::onAny(t_symbol* s, const AtomListView& l)
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();

    anyTo(0, s, l);
    msg_.setAny(s, l);
    delay();
}

void FlowDup::delay()
{
    if (delay_->value() >= 0)
        clock_.delay(delay_->value());
}

void FlowDup::reset()
{
    clock_.unset();
    in_process_ = false;
}

void setup_flow_dup()
{
    ObjectFactory<FlowDup> obj("flow.dup");
    obj.processData();
    obj.noPropsDispatch();

    obj.setXletsInfo({ "any: input flow", "float: set delay time\n"
                                          "reset: cancel scheduled delay" },
        { "output flow" });

    inlet_proxy_class = class_new(gensym("inlet_proxy"), 0, 0, sizeof(t_proxy), CLASS_PD, A_NULL);
    class_doaddfloat(inlet_proxy_class, (t_method)inlet_proxy_float);
    class_addanything(inlet_proxy_class, (t_method)inlet_proxy_any);
}
