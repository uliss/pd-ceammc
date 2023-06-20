/*****************************************************************************
 * Copyright 2023 Serge Poltavsky. All rights reserved.
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
#include "ceammc_music_spn.h"
#include "test_base.h"

using namespace ceammc;
using namespace ceammc::music;

TEST_CASE("music::Spn", "[ceammc::music]")
{
    SECTION("fromMidi")
    {
        SECTION("C4")
        {
            Spn spn(60);
            REQUIRE(spn.octave() == Octave(4, OCTAVE_ABS));
            REQUIRE_FALSE(spn.isRest());
            REQUIRE(spn.deviation() == 0);
            REQUIRE(spn.pitch() == PitchClass::C);
            REQUIRE(spn.asMidi() == 60);
            REQUIRE(spn.toString() == "C4");
        }

        SECTION("C#4")
        {
            Spn spn(61);
            REQUIRE(spn.octave() == Octave(4, OCTAVE_ABS));
            REQUIRE_FALSE(spn.isRest());
            REQUIRE(spn.deviation() == 0);
            REQUIRE(spn.pitch() == PitchClass::Cs);
            REQUIRE(spn.asMidi() == 61);
            REQUIRE(spn.toString() == "C#4");
        }

        SECTION("Cb3")
        {
            Spn spn(48);
            REQUIRE(spn.octave() == Octave(3, OCTAVE_ABS));
            REQUIRE_FALSE(spn.isRest());
            REQUIRE(spn.deviation() == 0);
            REQUIRE(spn.pitch() == PitchClass::C);
            REQUIRE(spn.asMidi() == 48);
            REQUIRE(spn.toString() == "C3");
        }

        SECTION("Cb3")
        {
            Spn spn(PitchClass::Cf, Octave(3));
            REQUIRE(spn.octave() == Octave(3, OCTAVE_ABS));
            REQUIRE_FALSE(spn.isRest());
            REQUIRE(spn.deviation() == 0);
            REQUIRE(spn.pitch() == PitchClass::Cf);
            REQUIRE(spn.asMidi() == 47);
            REQUIRE(spn.toString() == "Cb3");
        }
    }
}
