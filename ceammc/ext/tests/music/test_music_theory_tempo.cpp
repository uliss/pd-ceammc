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
        REQUIRE_FALSE(t.isNull());

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

    SECTION("parse")
    {
        Tempo t;
        REQUIRE(t.parse("12.5|1/4bpm"));
        REQUIRE(t.bpm() == 12.5);
        REQUIRE(t.division() == 4);
        REQUIRE(t.dots() == 0);

        REQUIRE(t.parse("65|16bpm"));
        REQUIRE(t.bpm() == 65);
        REQUIRE(t.division() == 16);
        REQUIRE(t.dots() == 0);

        REQUIRE(t.parse("20|4.bpm"));
        REQUIRE(t.bpm() == 20);
        REQUIRE(t.division() == 8);
        REQUIRE(t.dots() == 0);
    }

    SECTION("whole note duration")
    {
        REQUIRE(Tempo { 60, 4 }.wholeNoteDurationMs() == 4000);
        REQUIRE(Tempo { 60, 8 }.wholeNoteDurationMs() == 8000);
        REQUIRE(Tempo { 60, 2 }.wholeNoteDurationMs() == 2000);
        REQUIRE(Tempo { 60, 1 }.wholeNoteDurationMs() == 1000);
        REQUIRE(Tempo { 60, 4, 1 }.wholeNoteDurationMs() == Approx(1000.0 / 3 * 8));
    }

    SECTION("zero")
    {
        const Tempo zero(0, 4);
        REQUIRE(zero.isNull());
        REQUIRE(zero.normalized() == zero);
        REQUIRE(zero.normalized().strictEqual(zero));
        REQUIRE(zero.beatDurationMs() == 0);
        REQUIRE(zero.beatDuration() == Duration(1, 4));
        REQUIRE(zero.beatSubDivDuration() == Duration(1, 4));
        REQUIRE(zero.subBeatDurationMs() == 0);
        REQUIRE(zero.beatSubDivision() == 1);
        REQUIRE(zero.division() == 4);
        REQUIRE(zero.toString() == "0|4bpm");
        REQUIRE(zero.wholeNoteDurationMs() == 0);
    }
}
