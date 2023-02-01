/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef MIDI_TUNING_H
#define MIDI_TUNING_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"

#include <array>

using namespace ceammc;

class MidiTuning : public BaseObject {
    std::array<t_float, 12> deviations_;
    IntProperty* base_;

public:
    MidiTuning(const PdArgs& args);

    void onList(const AtomListView& lv) override;
};

void setup_midi_tuning();

#endif // MIDI_TUNING_H
