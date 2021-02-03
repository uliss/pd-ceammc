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
#ifndef MIDI_KBD_H
#define MIDI_KBD_H

#include "ceammc_object.h"
#include "ceammc_proxy.h"

#include <cstdint>
#include <unordered_map>

using namespace ceammc;
using KbdLayout = std::unordered_map<uint32_t, int8_t>;

class MidiKbd : public BaseObject {
    using Proxy = PdFloatProxy<MidiKbd>;
    Proxy keypress_, keyrelease_;
    IntProperty* octave_;
    FloatProperty* vel_;
    const KbdLayout* kbd_;

public:
    MidiKbd(const PdArgs& args);

public:
    void onKeyPress(t_float key);
    void onKeyRelease(t_float key);

private:
    int findNote(int key) const;
};

void setup_midi_kbd();

#endif // MIDI_KBD_H
