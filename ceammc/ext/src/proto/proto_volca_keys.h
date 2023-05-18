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
#ifndef PROTO_VOLCA_KEYS_H
#define PROTO_VOLCA_KEYS_H

#include "ceammc_object.h"
#include "proto_midi_cc.h"
using namespace ceammc;

class ProtoVolcaKeys : public ProtoMidiCC {
    IntProperty* chan_ { nullptr };

public:
    ProtoVolcaKeys(const PdArgs& args);

    void m_detune(t_symbol* s, const AtomListView& lv);
    void m_octave(t_symbol* s, const AtomListView& lv);
    void m_panic(t_symbol* s, const AtomListView& lv);
    void m_voice(t_symbol* s, const AtomListView& lv);

private:
    void sendMidi(std::uint8_t msg, std::uint8_t data1, std::uint8_t data2);
    bool sendParameterValue(t_symbol* name, std::uint8_t data);
};

void setup_proto_volca_keys();

#endif // PROTO_VOLCA_KEYS_H
