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
#ifndef FLOW_APPEND_H
#define FLOW_APPEND_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
#include "ceammc_proxy.h"

using namespace ceammc;

class FlowAppend : public BaseObject {
    FloatProperty* delay_;
    ListProperty* msg_value_;
    ClockLambdaFunction delay_fn_;
    Message msg_;
    InletProxy<FlowAppend> inlet2_;

public:
    FlowAppend(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float v) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* s, const AtomListView& lv) override;

    void proxy_any(InletProxy<FlowAppend>* x, t_symbol* s, const AtomListView& v);

private:
    void append();
    void output();
};

void setup_flow_append();

#endif // FLOW_APPEND_H
