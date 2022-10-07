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
    , delay_(nullptr)
    , delay_fn_([this]() { output(); })
    , inlet2_(this)
{
    delay_ = new FloatProperty("@delay", -1);
    delay_->checkMinEq(-1);
    delay_->setUnits(PropValueUnits::MSEC);
    addProperty(delay_);

    inlet_new(owner(), &inlet2_.x_obj, nullptr, nullptr);
    createOutlet();

    auto& pargs = parsedPosArgs();
    if (pargs.size() >= 1 && pargs[0].isSymbol())
        msg_.setAny(pargs[0].asT<t_symbol*>(), pargs.view(1));
    else if (pargs.empty())
        msg_.setBang();
    else
        msg_.setList(pargs);
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

void FlowAppend::proxy_any(int /*x*/, t_symbol* s, const AtomListView& v)
{
    msg_.setAny(s, v);
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
    if (msg_.isNone())
        bangTo(0);
    else
        messageTo(0, msg_);
}

void setup_flow_append()
{
    ObjectFactory<FlowAppend> obj("flow.append");
    obj.noPropsDispatch();
    obj.setXletsInfo({ "any: input flow", "any: set append message" }, { "any: output" });

    InletProxy<FlowAppend>::init();
    InletProxy<FlowAppend>::set_any_callback(&FlowAppend::proxy_any);
}
