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
#ifndef MSG_AFTER_H
#define MSG_AFTER_H

#include "ceammc_clock.h"
#include "ceammc_object.h"

using namespace ceammc;

class MessageAfter : public BaseObject {
    ClockMemberFunction<MessageAfter> clock_;
    t_float delay_;
    AtomList msg_;

public:
    MessageAfter(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomListView& lv) override;
    void onAny(t_symbol* s, const AtomListView& lv) override;
    void dump() const override;

    void parseProperties() override;
    bool processAnyProps(t_symbol* sel, const AtomListView&) override;

private:
    void run();
    void tick();
};

void setup_msg_after();

#endif // MSG_AFTER_H
