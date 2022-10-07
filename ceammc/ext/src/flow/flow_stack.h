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
#ifndef FLOW_STACK_H
#define FLOW_STACK_H

#include "ceammc_object.h"
#include "ceammc_proxy.h"
using namespace ceammc;

#include <vector>

class FlowStack : public BaseObject {
    InletProxy<FlowStack> inlet_;
    std::vector<Message> stack_;
    IntProperty* max_size_;
    ListProperty* on_full_;
    ListProperty* on_empty_;

public:
    FlowStack(const PdArgs& a);
    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomListView& lv) override;
    void onAny(t_symbol* s, const AtomListView& lv) override;

    void initDone() override;
    void dump() const override;

    void m_pop(const AtomListView&);
    void m_clear(const AtomListView&);
    void m_top(const AtomListView&);
    void m_flush(const AtomListView&);
    void m_poptop(int);

private:
    void check_empty();
    void check_full();
};

void setup_flow_stack();

#endif // FLOW_STACK_H
