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
#ifndef FLOW_PASS_IF_H
#define FLOW_PASS_IF_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowPassIf : public BaseObject {
    size_t pass_;

public:
    FlowPassIf(const PdArgs& a);
    void onBang();
    void onFloat(float v);
    void onSymbol(t_symbol* s);
    void onList(const AtomList& l);
    void onAny(t_symbol* s, const AtomList& l);

    void onInlet(size_t n, const AtomList& l);
};

extern "C" void setup_flow0x2epass_if();

#endif // FLOW_PASS_IF_H
