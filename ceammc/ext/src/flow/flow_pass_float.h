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
#ifndef FLOW_PASS_FLOAT_H
#define FLOW_PASS_FLOAT_H

#include "ceammc_object.h"
using namespace ceammc;

class FlowPassFloat : public BaseObject {
public:
    FlowPassFloat(const PdArgs& args);

    void onFloat(t_float f) override;
    void onBang() final { }
    void onSymbol(t_symbol*) final { }
    void onList(const AtomList&) final { }
    void onAny(t_symbol*, const AtomListView&) final { }
};

void setup_flow_pass_float();

#endif // FLOW_PASS_FLOAT_H
