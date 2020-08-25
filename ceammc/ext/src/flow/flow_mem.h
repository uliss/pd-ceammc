/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef FLOW_MEM_H
#define FLOW_MEM_H

#include <cstdint>
#include <vector>

#include "ceammc_object.h"
#include "ceammc_proxy.h"
using namespace ceammc;

class FlowMem : public BaseObject {
    std::vector<Message> mem_;
    std::vector<InletProxy<FlowMem>> inlets_;
    std::vector<uint16_t> mem_ttl_;
    IntProperty* num_;
    IntProperty* ttl_;
    BoolProperty* free_;

public:
    FlowMem(const PdArgs& a);
    void initDone() override;

    void onInlet(size_t, const AtomList& v) override;

    void proxy_any(InletProxy<FlowMem>* x, t_symbol* s, const AtomListView& v);

    const char* annotateInlet(size_t n) const override;
    const char* annotateOutlet(size_t n) const override;
};

void setup_flow_mem();

#endif // FLOW_MEM_H
