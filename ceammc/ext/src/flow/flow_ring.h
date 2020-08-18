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
#ifndef FLOW_RING_H
#define FLOW_RING_H

#include "ceammc_object.h"
#include "ceammc_proxy.h"
using namespace ceammc;

class FlowRing : public BaseObject {
    IntProperty* length_;
    std::vector<Message> line_;
    int end_, total_;
    InletProxy<FlowRing> inlet_;

public:
    FlowRing(const PdArgs& a);
    void initDone() override;

    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* s, const AtomListView& lv) override;

    void proxy_bang();
    void proxy_get(const AtomListView& lv);
    void proxy_fill(const AtomListView& lv);

private:
    void addMessage(const Message& m);
    void output();
};

void setup_flow_ring();

#endif // FLOW_RING_H
