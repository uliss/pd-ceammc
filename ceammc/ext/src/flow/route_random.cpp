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

#include <ctime>

constexpr int MIN_NUM_OUTLETS = 2;
constexpr int MAX_NUM_OUTLETS = 64;
constexpr int DEFAULT_NUM_OUTLETS = 2;

RouteRandom::RouteRandom(const PdArgs& args)
    : BaseObject(args)
    , n_(nullptr)
    , seed_(nullptr)
    , rnd_dev_(std::time(nullptr))
{
    n_ = new IntProperty("@n", DEFAULT_NUM_OUTLETS, PropValueAccess::INITONLY);
    n_->checkClosedRange(MIN_NUM_OUTLETS, MAX_NUM_OUTLETS);
    n_->setArgIndex(0);
    addProperty(n_);

    seed_ = new SizeTProperty("@seed", 0);
    addProperty(seed_);
}

void RouteRandom::initDone()
{
    if (seed_->value() != 0)
        rnd_dev_.seed(seed_->value());

    dist_ = Distribution(0, n_->value() - 1);

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

void RouteRandom::onList(const AtomList& lst)
{
    listTo(genOutletIdx(), lst);
}

void RouteRandom::onAny(t_symbol* s, const AtomListView& lv)
{
    anyTo(genOutletIdx(), s, lv);
}

void setup_route_random()
{
    ObjectFactory<RouteRandom> obj("route.random");
    obj.addAlias("route.r");
}
