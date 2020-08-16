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
#include "flow_prepend.h"
#include "ceammc_factory.h"

FlowPrepend::FlowPrepend(const PdArgs& args)
    : BaseObject(args)
    , prepend_msg_(nullptr)
    , delay_time_(0)
    , as_msg_(0)
    , clock_(this, &FlowPrepend::tick)
{
    delay_time_ = new FloatProperty("@delay", 0);
    delay_time_->setFloatCheck(PropValueConstraints::GREATER_EQUAL, -1);
    delay_time_->setUnits(PropValueUnits::MSEC);
    addProperty(delay_time_);

    as_msg_ = new FlagProperty("@msg");
    addProperty(as_msg_);

    prepend_msg_ = new ListProperty("@value");
    prepend_msg_->setArgIndex(0);
    addProperty(prepend_msg_);

    createInlet();
    createInlet();

    createOutlet();
}

void FlowPrepend::onBang()
{
    outputPrepend();
    src_msg_.setBang();
    process();
}

void FlowPrepend::onFloat(t_float f)
{
    outputPrepend();
    src_msg_.setFloat(f);
    process();
}

void FlowPrepend::onSymbol(t_symbol* s)
{
    outputPrepend();
    src_msg_.setSymbol(s);
    process();
}

void FlowPrepend::onList(const AtomList& lst)
{
    outputPrepend();
    src_msg_.setList(lst);
    process();
}

void FlowPrepend::onAny(t_symbol* s, const AtomListView& lst)
{
    outputPrepend();
    src_msg_.setAny(s, lst);
    process();
}

void FlowPrepend::onInlet(size_t n, const AtomList& lst)
{
    if (n == 1)
        prepend_msg_->set(lst);
    else if (n == 2)
        delay_time_->set(lst);
    else
        OBJ_ERR << "invalid inlet: " << n;
}

void FlowPrepend::process()
{
    if (delay_time_->value() >= 0)
        clock_.delay(delay_time_->value());
    else
        messageTo(0, src_msg_);
}

void FlowPrepend::tick()
{
    messageTo(0, src_msg_);
}

void FlowPrepend::outputPrepend()
{
    if (as_msg_->value()) {
        anyTo(0, prepend_msg_->get());
    } else {
        listTo(0, prepend_msg_->get());
    }
}

void setup_flow_prepend()
{
    ObjectFactory<FlowPrepend> obj("flow.prepend");
    obj.addAlias("flow.before");

    obj.noPropsDispatch();

    obj.setXletsInfo(
        { "any: target message", "list: message before", "float: delay (ms)" },
        { "prepended message then after delay original message" });
}
