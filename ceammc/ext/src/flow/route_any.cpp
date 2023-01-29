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
#include "route_any.h"
#include "ceammc_factory.h"

RouteAny::RouteAny(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    createOutlet();
}

void RouteAny::onBang()
{
    bangTo(1);
}

void RouteAny::onFloat(t_float f)
{
    floatTo(1, f);
}

void RouteAny::onSymbol(t_symbol* s)
{
    symbolTo(1, s);
}

void RouteAny::onList(const AtomListView& lv)
{
    listTo(1, lv);
}

void RouteAny::onAny(t_symbol* s, const AtomListView& lv)
{
    anyTo(0, s, lv);
}

void setup_route_any()
{
    ObjectFactory<RouteAny> obj("route.any");

    obj.setXletsInfo({ "any" }, { "any messages", "bangs, floats, symbols and lists" });
    obj.noPropsDispatch();
}
