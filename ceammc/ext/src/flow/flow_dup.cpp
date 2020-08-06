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

extern "C" {
#include "m_imp.h"
}

// note: should be synced with t_inlet
struct t_inlet_ceammc {
    t_pd i_pd;
    t_inlet_ceammc* i_next;
    t_object* i_owner;
    t_pd* i_dest;
    t_symbol* i_symfrom;
};

FlowDup::FlowDup(const PdArgs& a)
    : BaseObject(a)
    , delay_(nullptr)
    , clock_([this]() { messageTo(0, msg_); })
{
    // add reset message to inlet class
    static bool init_done = false;
    static _class inl_class = { 0 };

    t_inlet_ceammc* in0 = reinterpret_cast<t_inlet_ceammc*>(createInlet());

    if (!init_done) {
        // copy inlet class
        inl_class = *in0->i_pd;
        // add reset method
        inl_class.c_anymethod = reinterpret_cast<t_anymethod>(FlowDup::reset);
    }

    // change new inlet class
    in0->i_pd = &inl_class;

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

void FlowDup::reset(t_inlet_ceammc* x, t_symbol* s, int argc, t_atom* argv)
{
    PdObject<FlowDup>* p = (PdObject<FlowDup>*)x->i_owner;
    if (s == gensym("reset"))
        p->impl->clock_.unset();
    else
        Error(p->impl).stream() << "unsupported message: " << s;
}

void setup_flow_dup()
{
    ObjectFactory<FlowDup> obj("flow.dup");
    obj.setAnyFn(nullptr);

    obj.setXletsInfo({ "any: input flow", "float: set delay time\n"
                                          "reset: cancel scheduled delay" },
        { "output flow" });
}
