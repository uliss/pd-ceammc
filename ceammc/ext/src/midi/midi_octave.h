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
#ifndef MIDI_OCTAVE_H
#define MIDI_OCTAVE_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"

#include <cstdint>

using namespace ceammc;

class MidiOctave : public BaseObject {
    IntProperty* oct_;
    SymbolEnumProperty* mode_;
    int8_t a_, b_;
    bool random_;

public:
    MidiOctave(const PdArgs& args);
    void onFloat(t_float note) override;
    void onList(const AtomListView& lv) override;
    void onInlet(size_t, const AtomListView& lv) override;

private:
    t_float octave(t_float note) const;
};

void setup_midi_octave();

#endif // MIDI_OCTAVE_H
