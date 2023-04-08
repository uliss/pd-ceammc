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
#include "ceammc_music_theory_tempo.h"

#include "catch.hpp"

using namespace ceammc::music;

TEST_CASE("MusicTheory::Tempo", "[ceammc::music]")
{
    SECTION("init")
    {
        Tempo t;
        REQUIRE(t.bpm() == 60);
        REQUIRE(t.dots() == 0);
        REQUIRE(t.division() == 4);
        REQUIRE(t.beatDuration() == Duration { 1, 4 });
        REQUIRE(t.toString() == "60|4bpm");

        REQUIRE(t.setDots(0));
        REQUIRE(t.setDots(1));
        REQUIRE(t.dots() == 1);
        REQUIRE(t.toString() == "60|4.bpm");
        REQUIRE(t.setDots(2));
        REQUIRE(t.dots() == 2);
        REQUIRE(t.toString() == "60|4..bpm");
        REQUIRE(t.setDots(3));
        REQUIRE(t.dots() == 3);
        REQUIRE(t.toString() == "60|4...bpm");

        REQUIRE_FALSE(t.setDots(-1));
        REQUIRE(t.dots() == 3);
        REQUIRE_FALSE(t.setDots(4));
        REQUIRE(t.dots() == 3);
        REQUIRE(t.beatDuration() == Duration { 1, 4, 3 });
        REQUIRE(t.beatSubDivDuration() == Duration(1, 32));

        REQUIRE(t.set(60, 4, 0));
        REQUIRE(t.beatDuration() == Duration(1, 4));
        REQUIRE(t.beatSubDivDuration() == Duration(1, 4));
        REQUIRE(t.beatDurationMs() == 1000);
        REQUIRE(t.beatSubDivision() == 1);
        REQUIRE(t.subBeatDurationMs() == 1000);

        REQUIRE(t.set(60, 4, 1));
        REQUIRE(t.beatDuration() == Duration(1, 4, 1));
        REQUIRE(t.beatSubDivDuration() == Duration(1, 8));
        REQUIRE(t.beatDurationMs() == 1000);
        REQUIRE(t.beatSubDivision() == 3);
        REQUIRE(t.subBeatDurationMs() == Approx(1000 / 3.0));

        REQUIRE(t.set(60, 4, 2));
        REQUIRE(t.beatDuration() == Duration(1, 4, 2));
        REQUIRE(t.beatSubDivDuration() == Duration(1, 16));
        REQUIRE(t.beatDurationMs() == 1000);
        REQUIRE(t.beatSubDivision() == 7);
        REQUIRE(t.subBeatDurationMs() == Approx(1000 / 7.0));

        REQUIRE(t.set(60, 4, 3));
        REQUIRE(t.beatDuration() == Duration(1, 4, 3));
        REQUIRE(t.beatSubDivDuration() == Duration(1, 32));
        REQUIRE(t.beatDurationMs() == 1000);
        REQUIRE(t.beatSubDivision() == 15);
        REQUIRE(t.subBeatDurationMs() == Approx(1000 / 15.0));
    }

    SECTION("normalize")
    {
        REQUIRE(Tempo(60, 4).normalized() == Tempo(60, 4));
        REQUIRE(Tempo(60, 4, 1).normalized() == Tempo(180, 8));
        REQUIRE(Tempo(60, 4, 2).normalized() == Tempo(420, 16));
    }
}
