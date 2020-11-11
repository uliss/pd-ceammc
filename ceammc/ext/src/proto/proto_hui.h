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
#ifndef PROTO_HUI_H
#define PROTO_HUI_H

#include "ceammc_object.h"
using namespace ceammc;

class ProtoHUI : public BaseObject {

public:
    ProtoHUI(const PdArgs& args);

    void onFloat(t_float f) override;

    void m_vu(t_symbol* s, const AtomListView& lv);
    void m_vpot(t_symbol* s, const AtomListView& lv);
    void m_led(t_symbol* s, const AtomListView& lv);
    void m_rec(t_symbol* s, const AtomListView& lv);
    void m_solo(t_symbol* s, const AtomListView& lv);
    void m_mute(t_symbol* s, const AtomListView& lv);
    void m_select(t_symbol* s, const AtomListView& lv);
    void m_fader(t_symbol* s, const AtomListView& lv);

private:
    bool select_zone(int zone);
    bool port_switch(int port, bool on);
    bool led_switch(int zone, int port, bool on);
};

void setup_proto_hui();

#endif // PROTO_HUI_H
