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
#include "flow_delay.h"
#include "ceammc.h"
#include "ceammc_factory.h"

FlowDelay::FlowDelay(const PdArgs& args)
    : BaseObject(args)
    , delay_(nullptr)
    , block_(nullptr)
    , clock_([this]() {
        if (!last_msg_.isNone())
            messageTo(0, last_msg_);

        in_process_ = false;
    })
    , inlet_proxy_(this)
    , in_process_(false)
{
    createOutlet();

    delay_ = new FloatProperty("@delay");
    delay_->setArgIndex(0);
    delay_->checkMinEq(0);
    delay_->setUnits(PropValueUnits::MSEC);
    addProperty(delay_);

    block_ = new BoolProperty("@block", false);
    addProperty(block_);

    inlet_new(owner(), &inlet_proxy_.x_obj, nullptr, nullptr);
}

void FlowDelay::onBang()
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();
    last_msg_.setBang();
    clock_.delay(delay_->value());
}

void FlowDelay::onFloat(t_float f)
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();
    last_msg_.setFloat(f);
    clock_.delay(delay_->value());
}

void FlowDelay::onSymbol(t_symbol* s)
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();
    last_msg_.setSymbol(s);
    clock_.delay(delay_->value());
}

void FlowDelay::onList(const AtomList& l)
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();
    last_msg_.setList(l);
    clock_.delay(delay_->value());
}

void FlowDelay::onAny(t_symbol* s, const AtomListView& l)
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();
    last_msg_.setAny(s, l);
    clock_.delay(delay_->value());
}

void FlowDelay::proxy_delay(t_float f)
{
    delay_->setValue(f);
}

void FlowDelay::proxy_reset()
{
    clock_.unset();
    in_process_ = false;
}

void FlowDelay::proxy_reset(const AtomListView&)
{
    proxy_reset();
}

void setup_flow_delay()
{
    ObjectFactory<FlowDelay> obj("flow.delay");
    obj.addAlias("flow.del");
    obj.noPropsDispatch();
    obj.noInletsDispatch();

    obj.setXletsInfo({ "any: input message", "float: set delay time" }, { "delayed message" });

    InletProxy<FlowDelay>::init();
    InletProxy<FlowDelay>::set_bang_callback(&FlowDelay::proxy_reset);
    InletProxy<FlowDelay>::set_float_callback(&FlowDelay::proxy_delay);
    InletProxy<FlowDelay>::set_method_callback(gensym("reset"), &FlowDelay::proxy_reset);
}
