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
#ifndef SEQ_ARP_H
#define SEQ_ARP_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
using namespace ceammc;

struct AsrData {
    enum State {
        ATTACK,
        RELEASE,
        DONE
    };

    t_float step_duration_ms;
    int hold_steps;
    t_symbol* on_mode;
    t_symbol* off_mode;
    State state;

    t_float holdDurationMs() const { return step_duration_ms * hold_steps; }
};

class SeqArp : public BaseObject {
    ListProperty* chord_;
    std::vector<uint8_t> on_offs_;
    int nactive_;
    AsrData asr_data_;
    ClockLambdaFunction asr_clock_;

public:
    SeqArp(const PdArgs& args);
    void onBang() override;

    void m_on(t_symbol* mode, const AtomListView& lv);
    void m_off(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView& lv);

    void m_asr(t_symbol* s, const AtomListView& lv);

private:
    bool allOn() const { return nactive_ == on_offs_.size(); }
    bool allOff() const { return nactive_ == 0; }
};

void setup_seq_arp();

#endif // SEQ_ARP_H
