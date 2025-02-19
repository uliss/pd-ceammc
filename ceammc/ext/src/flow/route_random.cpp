/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "route_random.h"
#include "ceammc_factory.h"

constexpr int MIN_NUM_OUTLETS = 2;
constexpr int MAX_NUM_OUTLETS = 64;
constexpr int DEFAULT_NUM_OUTLETS = 2;

RouteRandom::RouteRandom(const PdArgs& args)
    : BaseObject(args)
    , n_(nullptr)
{
    n_ = new IntProperty("@n", DEFAULT_NUM_OUTLETS, PropValueAccess::INITONLY);
    n_->checkClosedRange(MIN_NUM_OUTLETS, MAX_NUM_OUTLETS);
    n_->setArgIndex(0);
    addProperty(n_);

    addProperty(new random::SeedProperty(gen_));
}

void RouteRandom::initDone()
{
    for (int i = 0; i < n_->value(); i++)
        createOutlet();
}

void RouteRandom::onBang()
{
    bangTo(genOutletIdx());
}

void RouteRandom::onFloat(t_float f)
{
    floatTo(genOutletIdx(), f);
}

void RouteRandom::onSymbol(t_symbol* s)
{
    symbolTo(genOutletIdx(), s);
}

void RouteRandom::onList(const AtomListView& lv)
{
    listTo(genOutletIdx(), lv);
}

void RouteRandom::onAny(t_symbol* s, const AtomListView& lv)
{
    anyTo(genOutletIdx(), s, lv);
}

void setup_route_random()
{
    ObjectFactory<RouteRandom> obj("route.random");
    obj.addAlias("route.r");

    obj.setDescription("routes input flow to random outlet");
    obj.setCategory("flow");
    obj.setKeywords({"route"});
}
