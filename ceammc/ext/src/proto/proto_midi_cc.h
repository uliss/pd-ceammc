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
#ifndef PROTO_MIDI_CC_H
#define PROTO_MIDI_CC_H

#include "ceammc_object.h"
#include "proto_midi_cc_rpn_parser.h"
#include "proto_midi_parser.h"
using namespace ceammc;

#include <cstdint>

class ProtoMidiCC : public BaseObject {
    midi::MidiParser parser_;
    midi::RPNParser rpn_parser_;
    uint8_t mod_wheel0_, mod_wheel1_;
    uint8_t pan_pos0_, pan_pos1_;
    uint8_t rpn0_, rpn1_;

public:
    ProtoMidiCC(const PdArgs& args);

    void onFloat(t_float f) override;
    void onList(const AtomList& lst) override;

    void m_bend_sens(t_symbol* s, const AtomListView& lv);
    void m_tune_bank_select(t_symbol* s, const AtomListView& lv);
    void m_tune_prog_change(t_symbol* s, const AtomListView& lv);

private:
    void onCC(int b, int c, int v);
    void sendCC(int chan, int cc, int v);
};

void setup_proto_midi_cc();

#endif // PROTO_MIDI_CC_H
