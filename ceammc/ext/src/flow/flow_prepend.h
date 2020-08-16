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
#ifndef FLOW_PREPEND_H
#define FLOW_PREPEND_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
using namespace ceammc;

class FlowPrepend : public BaseObject {
    ListProperty* prepend_msg_;
    FloatProperty* delay_time_;
    FlagProperty* as_msg_;
    ClockMemberFunction<FlowPrepend> clock_;
    Message src_msg_;

public:
    FlowPrepend(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& lst) override;
    void onAny(t_symbol* s, const AtomListView& lst) override;
    void onInlet(size_t n, const AtomList& lst) override;

private:
    void process();
    void tick();
    void outputPrepend();
};

void setup_flow_prepend();

#endif // FLOW_PREPEND_H
