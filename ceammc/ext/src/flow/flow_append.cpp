/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "flow_append.h"
#include "ceammc_factory.h"

FlowAppend::FlowAppend(const PdArgs& args)
    : BaseObject(args)
    , msg_(nullptr)
    , delay_time_(0)
    , as_msg_(0)
    , clock_(this, &FlowAppend::tick)
{
    delay_time_ = new FloatProperty("@delay", 0);
    delay_time_->setFloatCheck(PropValueConstraints::GREATER_EQUAL, -1);
    delay_time_->setUnits(PropValueUnits::MSEC);
    addProperty(delay_time_);

    as_msg_ = new FlagProperty("@msg");
    addProperty(as_msg_);

    msg_ = new ListProperty("@value");
    msg_->setArgIndex(0);
    addProperty(msg_);

    createInlet();
    createInlet();

    createOutlet();
}

void FlowAppend::onBang()
{
    bangTo(0);
    process();
}

void FlowAppend::onFloat(t_float f)
{
    floatTo(0, f);
    process();
}

void FlowAppend::onSymbol(t_symbol* s)
{
    symbolTo(0, s);
    process();
}

void FlowAppend::onList(const AtomList& lst)
{
    listTo(0, lst);
    process();
}

void FlowAppend::onAny(t_symbol* s, const AtomListView& lst)
{
    anyTo(0, s, lst);
    process();
}

void FlowAppend::onInlet(size_t n, const AtomList& lst)
{
    if (n == 1)
        msg_->set(lst);
    else if (n == 2)
        delay_time_->set(lst);
    else
        OBJ_ERR << "invalid inlet: " << n;
}

bool FlowAppend::processAnyProps(t_symbol* s, const AtomListView& lst)
{
    return false;
}

void FlowAppend::process()
{
    if (delay_time_->value() >= 0) {
        clock_.delay(delay_time_->value());
    } else {

        outputAppend();
    }
}

void FlowAppend::tick()
{
    outputAppend();
}

void FlowAppend::outputAppend()
{
    if (as_msg_->value()) {
        anyTo(0, msg_->get());
    } else {
        listTo(0, msg_->get());
    }
}

void setup_flow_append()
{
    ObjectFactory<FlowAppend> obj("flow.append");
}
