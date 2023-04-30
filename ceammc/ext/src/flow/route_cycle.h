/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef ROUTE_CYCLE_H
#define ROUTE_CYCLE_H

#include "ceammc_object.h"
#include "ceammc_proxy.h"
using namespace ceammc;

class RouteCycle : public BaseObject {
    IntProperty* n_ { 0 };
    IntProperty* step_ { 0 };
    IntProperty* offset_ { 0 };
    long cur_idx_ { 0 };
    InletProxy<RouteCycle> inlet1_;

public:
    RouteCycle(const PdArgs& args);
    void initDone() final;

    void onBang() final;
    void onFloat(t_float f) final;
    void onSymbol(t_symbol* s) final;
    void onList(const AtomListView& lv) final;
    void onAny(t_symbol* s, const AtomListView& lv) final;
    void onProxyBang(int idx);

    const char* annotateOutlet(size_t n) const final;

public:
    size_t calcCurrentOutlet() const;
    void nextIndex();
    void normalizeIndex();
};

void setup_route_cycle();

#endif // ROUTE_CYCLE_H
