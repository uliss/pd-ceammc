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
#include "route_prop.h"
#include "ceammc_factory.h"

RouteProp::RouteProp(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    createOutlet();
}

void RouteProp::onBang()
{
    bangTo(1);
}

void RouteProp::onFloat(t_float f)
{
    floatTo(1, f);
}

void RouteProp::onSymbol(t_symbol* s)
{
    symbolTo(1, s);
}

void RouteProp::onList(const AtomListView& lv)
{
    listTo(1, lv);
}

void RouteProp::onAny(t_symbol* s, const AtomListView& lv)
{
    anyTo(s->s_name[0] != '@', s, lv);
}

void setup_route_prop()
{
    ObjectFactory<RouteProp> obj("route.prop");

    obj.setXletsInfo({ "any" }, { "properties", "other messages" });
    obj.noPropsDispatch();

    obj.setDescription("separate property messages from other types");
    obj.setCategory("flow");
    obj.setKeywords({"route"});
}
