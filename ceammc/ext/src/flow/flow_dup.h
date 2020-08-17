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
#ifndef FLOW_DUP_H
#define FLOW_DUP_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
#include "ceammc_proxy.h"
using namespace ceammc;

struct t_proxy;

class FlowDup : public BaseObject {
    FloatProperty* delay_;
    BoolProperty* block_;
    ClockLambdaFunction clock_;
    Message msg_;
    InletProxy<FlowDup> inlet_proxy_;
    bool in_process_;

public:
    FlowDup(const PdArgs& a);

    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onData(const Atom& a) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* s, const AtomListView& l) override;

    void reset();
    void proxy_set_delay(t_float f) { delay_->setValue(f); }
    void proxy_reset(const AtomListView&) { reset(); }

private:
    void delay();
};

void setup_flow_dup();

#endif // FLOW_DUP_H
