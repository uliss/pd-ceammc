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
#ifndef PROTO_MIDI_H
#define PROTO_MIDI_H

#include "ceammc_object.h"
#include "proto_midi_parser.h"
using namespace ceammc;

class ProtoMidi : public BaseObject {
    midi::MidiParser parser_;

public:
    ProtoMidi(const PdArgs& args);
    void onFloat(t_float f) override;
    void onList(const AtomList& lst) override;

    void m_afterTouchMono(t_symbol* s, const AtomListView& lv);
    void m_afterTouchPoly(t_symbol* s, const AtomListView& lv);
    void m_cc(t_symbol* s, const AtomListView& lv);
    void m_clock(t_symbol*, const AtomListView&);
    void m_continue(t_symbol*, const AtomListView&);
    void m_noteOff(t_symbol* s, const AtomListView& lv);
    void m_noteOn(t_symbol* s, const AtomListView& lv);
    void m_pitchWheel(t_symbol* s, const AtomListView& lv);
    void m_programChange(t_symbol* s, const AtomListView& lv);
    void m_start(t_symbol*, const AtomListView&);
    void m_stop(t_symbol*, const AtomListView&);

private:
    bool checkMethodByte2(t_symbol* m, const AtomListView& lv, int from = 0, int to = 127);
    bool checkMethodByte3(t_symbol* m, const AtomListView& lv);

    void byteStatus(uint8_t st, int chan) { floatTo(0, st | uint8_t(0x0F & chan)); }
    void byteData(uint8_t data) { floatTo(0, 0x7F & data); }
    void msgTo(t_symbol* s, const Atom* a, size_t n) { anyTo(0, s, AtomListView(a, n)); }
};

void setup_proto_midi();

#endif // PROTO_MIDI_H
