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
#ifndef MIDI_VRAND_H
#define MIDI_VRAND_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"

#include <random>

using namespace ceammc;

class MidiVRrand : public BaseObject {
    FloatProperty *min_, *max_;
    SymbolEnumProperty* dist_;
    BoolProperty* rel_;
    SizeTProperty* seed_;
    std::mt19937 gen_;

public:
    MidiVRrand(const PdArgs& args);

    void onFloat(t_float note) override;
    void onList(const AtomList& lst) override;

private:
    t_float generate();
    t_float velocity(t_float orig);
};

void setup_midi_vrand();

#endif // MIDI_VRAND_H
