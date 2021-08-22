/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "flow_change.h"
#include "../base/function.h"
#include "ceammc_factory.h"

FlowChange::FlowChange(const PdArgs& a)
    : BaseObject(a)
    , on_repeat_(NULL)
    , ctrl_(this)
{
    inlet_new(owner(), &ctrl_.x_obj, nullptr, nullptr);

    on_repeat_ = new SymbolProperty("@onrepeat", &s_);
    addProperty(on_repeat_);
    createOutlet();
}

void FlowChange::onBang()
{
    if (msg_.isNone())
        return;

    messageTo(0, msg_);
}

void FlowChange::onFloat(t_float f)
{
    if (msg_.isEqual(f)) {
        onRepeat();
        return;
    }

    msg_ = f;
    floatTo(0, f);
}

void FlowChange::onSymbol(t_symbol* s)
{
    if (msg_.isEqual(s)) {
        onRepeat();
        return;
    }

    msg_ = s;
    symbolTo(0, s);
}

void FlowChange::onList(const AtomList& l)
{
    if (msg_.isEqual(l)) {
        onRepeat();
        return;
    }

    msg_ = l;
    listTo(0, l);
}

void FlowChange::onAny(t_symbol* s, const AtomListView& lv)
{
    if (msg_.isEqual(Message(s, lv))) {
        onRepeat();
        return;
    }

    msg_ = Message(s, lv);
    anyTo(0, s, lv);
}

void FlowChange::m_reset(const AtomListView&)
{
    msg_ = Message();
}

void FlowChange::m_set(const AtomListView& lv)
{
    if (lv.size() == 1)
        msg_ = Message(lv[0]);
    else
        msg_ = Message(lv);
}

void FlowChange::onRepeat()
{
    if (on_repeat_->value() == &s_)
        return;

    Function* fn = Function::function(on_repeat_->value());

    if (!fn) {
        OBJ_ERR << "function is not found: " << on_repeat_->value()->s_name;
        return;
    }

    fn->onBang();
}

void setup_flow_change()
{
    ObjectFactory<FlowChange> obj("flow.change");

    obj.noInletsDispatch();
    obj.noPropsDispatch();

    obj.setXletsInfo({ "any", "reset: reset object state\n"
                              "set: set current value" },
        { "any" });

    InletProxy<FlowChange>::init();
    InletProxy<FlowChange>::set_method_callback(gensym("reset"), &FlowChange::m_reset);
    InletProxy<FlowChange>::set_method_callback(gensym("set"), &FlowChange::m_set);
}
