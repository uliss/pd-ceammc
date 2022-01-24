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
#ifndef MIDI_MODUS_H
#define MIDI_MODUS_H

#include "ceammc_music_scale.h"
#include "ceammc_music_theory_pitch_class.h"
#include "ceammc_object.h"
#include "ceammc_property_enum.h"
using namespace ceammc;

class MidiModus : public BaseObject {
    const music::Scale* scale_;
    music::PitchClass root_;

    SymbolEnumProperty* prop_scale_;
    SymbolEnumProperty* prop_mode_;

public:
    MidiModus(const PdArgs& args);

    void onFloat(t_float f) override;
    void onList(const AtomList& lst) override;

private:
    t_float mapNote(t_float note) const;
};

void setup_midi_modus();

#endif // MIDI_MODUS_H
