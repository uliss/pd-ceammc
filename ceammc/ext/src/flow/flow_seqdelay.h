/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#ifndef FLOW_SEQDELAY_H
#define FLOW_SEQDELAY_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
#include "ceammc_proxy.h"

using namespace ceammc;

class FlowSeqDelay : public BaseObject {
    std::vector<t_float> time_;
    std::vector<std::string> outlet_tooltips_;
    ClockLambdaFunction clock_;
    Message msg_;
    BoolProperty* block_;
    InletProxy<FlowSeqDelay> main_inlet_, ctl_inlet_;
    size_t idx_;
    bool in_process_;

public:
    FlowSeqDelay(const PdArgs& args);

    void initDone() override;
    const char* annotateOutlet(size_t n) const final;

    void m_reset(t_symbol* s, const AtomListView& lv);
    void on_proxy_any(int idx, t_symbol* s, const AtomListView& lv);

private:
    void handleNewMessage();
    bool scheduleNext();
    void updateOutletTooltips();
};

void setup_flow_seqdelay();

#endif // FLOW_SEQDELAY_H
