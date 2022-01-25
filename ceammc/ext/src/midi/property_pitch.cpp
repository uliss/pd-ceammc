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
#include "property_pitch.h"
#include "ceammc_music_theory_names.h"
#include "lex/parser_music.h"

namespace ceammc {

static Atom pitchToAtom(music::PitchClass p)
{
    using namespace music;

    char buf[4] = { 0, 0, 0, 0 };
    buf[0] = p.pitchName().letterName();

    switch (p.alteration().type()) {
    case ALTERATION_DOUBLE_FLAT:
        buf[1] = 'b';
        buf[2] = 'b';
        break;
    case ALTERATION_FLAT:
        buf[1] = 'b';
        break;
    case ALTERATION_DOUBLE_SHARP:
        buf[1] = '#';
        buf[2] = '#';
        break;
    case ALTERATION_SHARP:
        buf[1] = '#';
        break;
    default:
        break;
    }

    return gensym(buf);
}

PropertyPitch::PropertyPitch(const std::string& name, const music::PitchClass& def, PropValueAccess access)
    : AtomProperty(name, pitchToAtom(def), access)
    , pitch_(def)
{
    if (info().setConstraints(PropValueConstraints::ENUM)) {
        for (auto& p : music::PitchClass::all) {
            if (!info().addEnum(pitchToAtom(p)))
                continue;
        }
    }

    setAtomCheckFn([this](const Atom& a) -> bool {
        parser::PitchFullMatch parser;
        if (!parser.parse(a))
            return false;

        const auto spn = parser.spn();
        pitch_.setPitchName(music::PitchName::C + spn.note);

        auto alt = music::Alteration::NATURAL;
        alt.alterate(spn.alt);
        pitch_.setAlteration(alt);
        return true;
    });
}

}
