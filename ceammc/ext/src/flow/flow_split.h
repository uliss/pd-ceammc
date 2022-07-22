/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef FLOW_SPLIT_H
#define FLOW_SPLIT_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowSplit : public BaseObject {
    size_t split_;

public:
    FlowSplit(const PdArgs& a);
    void onBang() override;
    void onFloat(t_float v) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomListView& lv) override;
    void onAny(t_symbol* s, const AtomListView& lv) override;

    void onInlet(size_t n, const AtomListView& lv) override;
};

void setup_flow_split();

#endif // FLOW_SPLIT_H
