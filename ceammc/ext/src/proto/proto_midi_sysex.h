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
#ifndef PROTO_MIDI_SYSEX_H
#define PROTO_MIDI_SYSEX_H

#include "ceammc_object.h"
#include <array>
#include <cstdint>
#include <initializer_list>
#include <limits>

using namespace ceammc;

class ProtoMidiSysex : public BaseObject {
    static const size_t N = 8192;

    using Short = uint16_t;
    using Byte = uint8_t;
    using Buf = std::array<Byte, N>;

    enum State {
        STATE_INIT,
        STATE_SYSEX
    };

    Buf buf_, out_;
    uint16_t n_;
    State state_;

public:
    ProtoMidiSysex(const PdArgs& args);

    void onFloat(t_float f) override;
    void onList(const AtomListView& lv) override;

    void m_id_request(t_symbol*, const AtomListView&);
    void m_id_reply(t_symbol*, const AtomListView&);

    void m_mvolume(t_symbol* s, const AtomListView& lv);
    void m_mbalance_float(t_symbol* s, const AtomListView& lv);

    void m_mtune_fine(t_symbol* s, const AtomListView& lv);
    void m_mtune_coarse(t_symbol* s, const AtomListView& lv);
    void m_mtune(t_symbol* s, const AtomListView& lv);

private:
    bool appendByte(Byte b);
    void output();
    void reset();

    bool startsWith(std::initializer_list<Byte> l) const;

    void output_mtune_coarse(int v);
    void output_mtune_fine(t_float v);
};

void setup_proto_midi_sysex();

#endif // PROTO_MIDI_SYSEX_H
