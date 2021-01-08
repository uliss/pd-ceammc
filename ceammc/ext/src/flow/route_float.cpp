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
#include "route_float.h"
#include "ceammc_factory.h"

RouteFloat::RouteFloat(const PdArgs& args)
    : BaseObject(args)
    , strict_(nullptr)
{
    strict_ = new BoolProperty("@strict", false);
    addProperty(strict_);

    createOutlet();
    createOutlet();
}

void RouteFloat::onBang()
{
    bangTo(1);
}

void RouteFloat::onFloat(t_float f)
{
    floatTo(0, f);
}

void RouteFloat::onSymbol(t_symbol* s)
{
    symbolTo(1, s);
}

void RouteFloat::onList(const AtomList& lst)
{
    if (!strict_->value() && lst.isFloat())
        floatTo(0, lst[0].asT<t_float>());
    else
        listTo(1, lst);
}

void RouteFloat::onAny(t_symbol* s, const AtomListView& lv)
{
    anyTo(1, s, lv);
}

void setup_route_float()
{
    ObjectFactory<RouteFloat> obj("route.float");
    obj.addAlias("route.f");

    obj.setXletsInfo({ "any" }, { "float", "other messages" });
    obj.noPropsDispatch();
}
