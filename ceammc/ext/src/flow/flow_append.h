/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef FLOW_APPEND_H
#define FLOW_APPEND_H

#include "ceammc_clock.h"
#include "ceammc_object.h"

using namespace ceammc;

class FlowAppend : public BaseObject {
    ListProperty* msg_;
    FloatProperty* delay_time_;
    FlagProperty* as_msg_;
    ClockMemberFunction<FlowAppend> clock_;

public:
    FlowAppend(const PdArgs& args);
    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& lst) override;
    void onAny(t_symbol* s, const AtomList& lst) override;
    void onInlet(size_t n, const AtomList& lst) override;

    bool processAnyProps(t_symbol* s, const AtomList& lst) override;

private:
    void process();
    void tick();
    void outputAppend();
};

void setup_flow_append();

#endif // FLOW_APPEND_H
