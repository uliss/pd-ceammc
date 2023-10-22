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
#include "flow_append.h"
#include "ceammc_factory.h"

FlowAppend::FlowAppend(const PdArgs& args)
    : BaseObject(args)
    , delay_fn_([this]() { output(); })
    , inlet2_(this)
{
    delay_ = new FloatProperty("@delay", -1);
    delay_->checkMinEq(-1);
    delay_->setUnits(PropValueUnits::MSEC);
    addProperty(delay_);

    bindProxyInlet(inlet2_, 1);
    createOutlet();

    msg_ = new MessageProperty("@msg");
    msg_->setArgIndex(0);
    addProperty(msg_);
}

void FlowAppend::onBang()
{
    bangTo(0);
    append();
}

void FlowAppend::onFloat(t_float v)
{
    floatTo(0, v);
    append();
}

void FlowAppend::onSymbol(t_symbol* s)
{
    symbolTo(0, s);
    append();
}

void FlowAppend::onList(const AtomListView& lv)
{
    listTo(0, lv);
    append();
}

void FlowAppend::onAny(t_symbol* s, const AtomListView& lv)
{
    anyTo(0, s, lv);
    append();
}

void FlowAppend::onProxyAny(int /*x*/, t_symbol* s, const AtomListView& v)
{
    msg_->value().setAny(s, v);
}

void FlowAppend::append()
{
    const auto dt = delay_->value();
    if (dt < 0)
        output();
    else
        delay_fn_.delay(dt);
}

void FlowAppend::output()
{
    if (msg_->value().isNone())
        bangTo(0);
    else
        messageTo(0, msg_->value());
}

void setup_flow_append()
{
    ObjectFactory<FlowAppend> obj("flow.append");
    obj.noPropsDispatch();
    obj.setXletsInfo({ "any: input flow", "any: set append message" }, { "any: output" });

    obj.useProxyAny();

    obj.setDescription("append message to flow stream");
    obj.setCategory("flow");
    obj.setKeywords({ "flow", "append" });
}
