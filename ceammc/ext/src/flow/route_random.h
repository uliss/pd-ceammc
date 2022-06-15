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
#ifndef ROUTE_RANDOM_H
#define ROUTE_RANDOM_H

#include <random>

#include "ceammc_object.h"
using namespace ceammc;

class RouteRandom : public BaseObject {
    using Distribution = std::uniform_int_distribution<int>;

private:
    IntProperty* n_;
    SizeTProperty* seed_;
    std::mt19937 rnd_dev_;
    Distribution dist_;

public:
    RouteRandom(const PdArgs& args);

    void initDone() final;
    void onBang() final;
    void onFloat(t_float f) final;
    void onSymbol(t_symbol* s) final;
    void onList(const AtomList& lst) final;
    void onAny(t_symbol* s, const AtomListView& lv) override;

private:
    inline size_t genOutletIdx() { return dist_(rnd_dev_); }
};

void setup_route_random();

#endif // ROUTE_RANDOM_H
