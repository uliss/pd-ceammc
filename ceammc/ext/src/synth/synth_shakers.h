/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef SYNTH_SHAKERS_H
#define SYNTH_SHAKERS_H

#include "stksynth.h"

enum ShakerType {
    UNKNOWN = -1,
    MARACA = 0,
    CABASA = 1,
    SEKERE = 2,
    TAMBOURINE = 3,
    SLEIGH_BELLS = 4,
    BAMBOO_CHIMES = 5,
    SAND_PAPER = 6,
    COKE_CAN = 7,
    STICKS = 8,
    CRUNCH = 9,
    BIG_ROCKS = 10,
    LITTLE_ROCKS = 11,
    NEXT_MUG = 12,
    PENNY_MUG = 13,
    NICKLE_MUG = 14,
    DIME_MUG = 15,
    QUARTER_MUG = 16,
    FRANC_MUG = 17,
    PESO_MUG = 18,
    GUIRO = 19,
    WRENCH = 20,
    WATER_DROPS = 21,
    TUNED_BAMBOO_CHIMES = 22,
    SHAKER_TYPE_MAX
};

class TypeProperty;

class SynthShakers : public StkBase {
    TypeProperty* type_;
    t_float gate_;

public:
    SynthShakers(const PdArgs& args);

    AtomList propTypes() const;

public:
    static ShakerType nameToType(t_symbol* s);
    static bool nameExists(t_symbol* s);
};

void setup_synth_shakers();

#endif // SYNTH_SHAKERS_H
