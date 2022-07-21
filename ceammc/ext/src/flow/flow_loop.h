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
#ifndef FLOW_LOOP_H
#define FLOW_LOOP_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
using namespace ceammc;

class FlowLoop : public BaseObject {
    IntProperty* n_;
    IntProperty* quant_;
    BoolProperty* echo_;
    std::vector<Message> messages_;
    std::vector<double> msg_offsets_;
    ClockLambdaFunction clock_;

    enum Mode {
        MODE_REC,
        MODE_PLAY
    };

    Mode mode_;
    double last_event_time_;
    size_t play_idx_;

public:
    FlowLoop(const PdArgs& a);
    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomListView& lv) override;
    void onInlet(size_t n, const AtomListView&) override;
    void play();

    void messageTo(size_t n, const Message& msg) final;

private:
    void quant();
    void appendMessage(const Message& m);
};

void setup_flow_loop();

#endif // FLOW_LOOP_H
