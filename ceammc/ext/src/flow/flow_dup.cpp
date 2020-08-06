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

static t_class* inlet_proxy;
struct t_proxy {
    t_pd x_obj;
    FlowDup* dest;
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
    , clock_([this]() { messageTo(0, msg_); })
{
    t_proxy* p = (t_proxy*)pd_new(inlet_proxy);
    p->dest = this;
    inlet_new(owner(), &p->x_obj, nullptr, &s_);

    createOutlet();

    delay_ = new FloatProperty("@delay", 0);
    delay_->checkMinEq(-1);
    delay_->setArgIndex(0);
    delay_->setUnits(PropValueUnits::MSEC);
    addProperty(delay_);
}

void FlowDup::onInlet(size_t n, const AtomList& l)
{
    delay_->set(l);
}

void FlowDup::onBang()
{
    bangTo(0);
    msg_.setBang();
    delay();
}

void FlowDup::onFloat(t_float f)
{
    floatTo(0, f);
    msg_.setFloat(f);
    delay();
}

void FlowDup::onSymbol(t_symbol* s)
{
    symbolTo(0, s);
    msg_.setSymbol(s);
    delay();
}

void FlowDup::onList(const AtomList& l)
{
    listTo(0, l);
    msg_.setList(l);
    delay();
}

void FlowDup::onAny(t_symbol* s, const AtomListView& l)
{
    anyTo(0, s, l);
    msg_.setAny(s, l);
    delay();
}

void FlowDup::delay()
{
    if (delay_->value() >= 0)
        clock_.delay(delay_->value());
}

bool FlowDup::processAnyProps(t_symbol* sel, const AtomListView& lst)
{
    return false;
}

void FlowDup::reset()
{
    clock_.unset();
}

void setup_flow_dup()
{
    ObjectFactory<FlowDup> obj("flow.dup");
    obj.setAnyFn(nullptr);

    obj.setXletsInfo({ "any: input flow", "float: set delay time\n"
                                          "reset: cancel scheduled delay" },
        { "output flow" });

    inlet_proxy = class_new(gensym("inlet_proxy"), 0, 0, sizeof(t_proxy), CLASS_PD, A_NULL);
    class_doaddfloat(inlet_proxy, (t_method)inlet_proxy_float);
    class_addanything(inlet_proxy, (t_method)inlet_proxy_any);
}
