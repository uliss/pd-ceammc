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
#ifndef FLOW_SPACE_H
#define FLOW_SPACE_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
#include "ceammc_proxy.h"

#include <list>

using namespace ceammc;

class FlowSpace : public BaseObject {
    struct ClockEntry {
        ClockLambdaFunction clock;
        size_t id;
        bool new_one = { false };
        ClockEntry(const std::function<void()>& fn)
            : clock(fn)
        {
        }
    };

    ClockLambdaFunction packet_sched_;
    FloatProperty* delay_;
    IntProperty* done_;
    std::list<ClockEntry> pipe_;
    ClockLambdaFunction done_fn_;
    InletProxy<FlowSpace> inlet_;
    int packet_count_ = { 0 };
    int num_active_ = { 0 };

public:
    FlowSpace(const PdArgs& a);
    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* s, const AtomListView& lv) override;

    void m_reset(const AtomListView&);
    void m_prop_delay(const AtomListView& lv);
    void m_prop_done(const AtomListView& lv);


    void setInterval(t_float f);

private:
    void packetSchedule();
    void clockDone();
    void addClock(const std::function<void()>& fn);
};

void setup_flow_space();

#endif // FLOW_SPACE_H
