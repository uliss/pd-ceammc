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
#ifndef PROPERTY_PITCH_H
#define PROPERTY_PITCH_H

#include "ceammc_music_theory_pitch_class.h"
#include "ceammc_property.h"

namespace ceammc {

class PropertyPitch : public AtomProperty {
    music::PitchClass pitch_;

public:
    PropertyPitch(const std::string& name, const music::PitchClass& def = music::PitchClass::C, PropValueAccess access = PropValueAccess::READWRITE);

    const music::PitchClass& pitch() const { return pitch_; }

    bool setAtom(const Atom& a) override;
};

}

#endif // PROPERTY_PITCH_H
