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

FlowDup::FlowDup(const PdArgs& a)
    : BaseObject(a)
    , delay_(nullptr)
    , block_(nullptr)
    , clock_([this]() { messageTo(0, msg_); in_process_ = false; })
    , inlet_proxy_(this)
    , in_process_(false)
{
    inlet_new(owner(), &inlet_proxy_.x_obj, nullptr, nullptr);

    createOutlet();

    delay_ = new FloatProperty("@delay", 0);
    delay_->checkMinEq(-1);
    delay_->setArgIndex(0);
    delay_->setUnits(PropValueUnits::MSEC);
    addProperty(delay_);

    block_ = new BoolProperty("@block", false);
    addProperty(block_);
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
    obj.noInletsDispatch();

    obj.setXletsInfo({ "any: input flow", "float: set delay time\n"
                                          "reset: cancel scheduled delay" },
        { "output flow" });

    InletProxy<FlowDup>::init();
    InletProxy<FlowDup>::set_bang_callback(&FlowDup::reset);
    InletProxy<FlowDup>::set_float_callback(&FlowDup::proxy_set_delay);
    InletProxy<FlowDup>::set_method_callback(gensym("reset"), &FlowDup::proxy_reset);
}
