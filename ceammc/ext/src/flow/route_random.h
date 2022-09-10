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

#include "ceammc_object.h"
#include "ceammc_random.h"
using namespace ceammc;

class RouteRandom : public BaseObject {
    IntProperty* n_;
    random::RandomGen gen_;

public:
    RouteRandom(const PdArgs& args);

    void initDone() final;
    void onBang() final;
    void onFloat(t_float f) final;
    void onSymbol(t_symbol* s) final;
    void onList(const AtomListView& lv) final;
    void onAny(t_symbol* s, const AtomListView& lv) override;

private:
    inline size_t genOutletIdx() { return gen_.gen_uniform_uint(0, n_->value() - 1); }
};

void setup_route_random();

#endif // ROUTE_RANDOM_H
