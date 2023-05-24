/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef PROTO_ARTURIA_MINILAB_H
#define PROTO_ARTURIA_MINILAB_H

#include "proto_midi.h"

class ProtoArturiaMinilab : public ProtoMidi {

    enum { KNOB_COUNT = 16 };

    std::array<std::uint8_t, KNOB_COUNT> knobs_;

public:
    ProtoArturiaMinilab(const PdArgs& args);

    void m_pad_color(t_symbol* s, const AtomListView& lv);
    void m_knob(t_symbol* s, const AtomListView& lv);
    void m_knob_req(t_symbol* s, const AtomListView& lv);
    void m_knob_req_all(t_symbol* s, const AtomListView& lv);
};

void setup_proto_arturia_minilab();

#endif // PROTO_ARTURIA_MINILAB_H
