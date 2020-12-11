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
#include "ceammc_music_temperament.h"
#include "test_base.h"

#include "catch.hpp"

using namespace ceammc;
using namespace ceammc::music;

TEST_CASE("temperament", "[ceammc::music]")
{
    SECTION("to_freq")
    {
        REQUIRE(to_freq(69, TemperamentType::EQT_12_PERFECT_OCTAVE, 440) == Approx(440));
        REQUIRE(to_freq(57, TemperamentType::EQT_12_PERFECT_OCTAVE, 440) == Approx(220));
        REQUIRE(to_freq(45, TemperamentType::EQT_12_PERFECT_OCTAVE, 440) == Approx(110));

        REQUIRE(to_freq(69, TemperamentType::EQT_12_PERFECT_OCTAVE, 442) == Approx(442));
    }

    SECTION("to_freq_base")
    {
        REQUIRE(PitchClass::A.absolutePitch() == 9);
        REQUIRE(to_freq_base(69, TemperamentType::EQT_12_PERFECT_OCTAVE, PitchClass::A, 440) == Approx(440));
        REQUIRE(to_freq_base(69, TemperamentType::EQT_12_PERFECT_OCTAVE, PitchClass::A, 442) == Approx(442));

        REQUIRE(to_freq_base(60, TemperamentType::EQT_12_PERFECT_OCTAVE, PitchClass::C, 260) == Approx(260));
        REQUIRE(to_freq_base(48, TemperamentType::EQT_12_PERFECT_OCTAVE, PitchClass::C, 260) == Approx(130));

        REQUIRE(to_freq_base(60, TemperamentType::JUST, PitchClass::C, 260) == Approx(260));
        REQUIRE(to_freq_base(69, TemperamentType::JUST, PitchClass::A, 415) == Approx(415));

        REQUIRE(to_freq_base(60, TemperamentType::PYTHAGOREAN, PitchClass::C, 200) == Approx(200));
        REQUIRE(to_freq_base(67, TemperamentType::PYTHAGOREAN, PitchClass::C, 200) == Approx(300));
        REQUIRE(to_freq_base(74, TemperamentType::PYTHAGOREAN, PitchClass::C, 200) == Approx(450));
        REQUIRE(to_freq_base(81, TemperamentType::PYTHAGOREAN, PitchClass::C, 200) == Approx(675));
    }
}
