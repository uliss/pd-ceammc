/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "route_bang.h"
#include "ceammc_factory.h"

RouteBang::RouteBang(const PdArgs& args)
    : BaseObject(args)
    , strict_(nullptr)
{
    strict_ = new BoolProperty("@strict", false);
    addProperty(strict_);

    createOutlet();
    createOutlet();
}

void RouteBang::onBang()
{
    bangTo(0);
}

void RouteBang::onFloat(t_float f)
{
    floatTo(1, f);
}

void RouteBang::onSymbol(t_symbol* s)
{
    symbolTo(1, s);
}

void RouteBang::onList(const AtomListView& lv)
{
    if (!strict_->value() && lv.empty())
        bangTo(0);
    else
        listTo(1, lv);
}

void RouteBang::onAny(t_symbol* s, const AtomListView& lv)
{
    anyTo(1, s, lv);
}

void setup_route_bang()
{
    ObjectFactory<RouteBang> obj("route.bang");
    obj.addAlias("route.b");

    obj.setXletsInfo({ "any" }, { "bang", "other messages" });
    obj.noPropsDispatch();

    obj.setDescription("separate bang messages from other types");
    obj.setCategory("flow");
    obj.setKeywords({"route", "bang"});
}
