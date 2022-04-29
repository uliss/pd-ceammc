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
#ifndef PROTO_WHAMMY_H
#define PROTO_WHAMMY_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
#include "parser_whammy_common.h"

#include <cstdint>
using namespace ceammc;

class ProtoWhammy : public BaseObject {
public:
    struct State {
        std::int8_t pedal_up, pedal_down;
        bool operator==(const State& s) const { return pedal_up == s.pedal_up && pedal_down == s.pedal_down; }
    };

    struct Info {
        State state;
        std::uint8_t active, bypass;
    };

    using MidiMap = std::vector<Info>;

public:
    ProtoWhammy(const PdArgs& args);

    void m_reset(t_symbol*, const AtomListView& lv);
    void m_toggle(t_symbol*, const AtomListView& lv);
    void m_random(t_symbol*, const AtomListView& lv);
    void m_set(t_symbol*, const AtomListView& lv);
    void m_next(t_symbol*, const AtomListView& lv);
    void m_prev(t_symbol*, const AtomListView& lv);

    void output();

private:
    SymbolEnumProperty* mode_;
    IntProperty* chan_;
    size_t idx_;
    bool active_;
    static const MidiMap midi_classic_map_;
};

void setup_proto_whammy();

#endif // PROTO_WHAMMY_H
