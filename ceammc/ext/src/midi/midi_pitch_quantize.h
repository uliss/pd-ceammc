/*****************************************************************************
 * Copyright 2026 Serge Poltavsky. All rights reserved.
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
#ifndef MIDI_PITCH_QUANTIZE_H
#define MIDI_PITCH_QUANTIZE_H

#include "ceammc_object.h"
#include "ceammc_property.h"
#include "ceammc_property_enum.h"

namespace ceammc {

class MidiPitchQuantize : public BaseObject {
public:
    MidiPitchQuantize(const PdArgs& args);

    void onFloat(t_float note) final;
    void onList(const AtomListView& lv) final;

private:
    MaybeFloat quantize(t_float note) const;
    MaybeFloat quantize(const Atom& note) const;

    IntProperty* min_;
    IntProperty* max_;
    SymbolEnumProperty* mode_;
};

} // namespace ceammc

void setup_midi_pitch_quantize();

#endif // MIDI_PITCH_QUANTIZE_H
