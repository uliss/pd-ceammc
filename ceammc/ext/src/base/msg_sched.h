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
#ifndef MSG_SCHED_H
#define MSG_SCHED_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
#include "ceammc_proxy.h"

#include <list>

using namespace ceammc;

class MsgSched : public BaseObject {
public:
    using ClockList = std::list<ClockLambdaFunction>;

private:
    ClockList queue_;
    InletProxy<MsgSched> inlet_proxy_;
    ClockLambdaFunction cleanup_;

public:
    MsgSched(const PdArgs& args);

    void onBang() override { bangTo(0); }
    void onSymbol(t_symbol* s) override { symbolTo(0, s); }
    void onFloat(t_float f) override { floatTo(0, f); }
    void onList(const AtomList& lst) override;
    void onAny(t_symbol* s, const AtomListView& lv) override { anyTo(0, s, lv); }

    void proxy_bang(int);
    void proxy_flush(const AtomListView& lv);
    void proxy_reset(const AtomListView& lv);

    void dump() const override;

private:
    void output(const AtomListView& lv);
};

void setup_msg_sched();

#endif // MSG_SCHED_H
