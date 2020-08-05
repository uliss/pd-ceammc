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
    , clock_([this]() {
        if (!last_msg_.isNone())
            messageTo(0, last_msg_);
    })
{
    createOutlet();

    delay_ = new FloatProperty("@delay");
    delay_->setArgIndex(0);
    delay_->checkMinEq(0);
    addProperty(delay_);

    createInlet();
}

void FlowDelay::onBang()
{
    last_msg_.setBang();
    clock_.delay(delay_->value());
}

void FlowDelay::onFloat(t_float f)
{
    last_msg_.setFloat(f);
    clock_.delay(delay_->value());
}

void FlowDelay::onSymbol(t_symbol* s)
{
    last_msg_.setSymbol(s);
    clock_.delay(delay_->value());
}

void FlowDelay::onList(const AtomList& l)
{
    last_msg_.setList(l);
    clock_.delay(delay_->value());
}

void FlowDelay::onAny(t_symbol* s, const AtomListView& l)
{
    last_msg_.setAny(s, l);
    clock_.delay(delay_->value());
}

void FlowDelay::onInlet(size_t n, const AtomList& l)
{
    delay_->set(l);
}

bool FlowDelay::processAnyProps(t_symbol* sel, const AtomListView& lst)
{
    return false;
}

void setup_flow_delay()
{
    ObjectFactory<FlowDelay> obj("flow.delay");
    obj.addAlias("flow.del");

    obj.setXletsInfo({ "any: input message", "float: set delay time" }, { "delayed message" });
}
