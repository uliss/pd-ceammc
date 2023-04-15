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
#include "catch.hpp"

#include "ceammc_music_theory_tempo.h"
#include "ceammc_music_theory_timesig.h"

using namespace ceammc::music;

TEST_CASE("MusicTheory::TimeSig", "[ceammc::music]")
{
    SECTION("init")
    {
        TimeSignature ts;
        REQUIRE(ts.toString() == "|4/4|");
        REQUIRE(ts.isSimple());
        REQUIRE(!ts.isCompound());
        REQUIRE(ts.duration() == Duration { 4, 4 });

        REQUIRE_THROWS_AS(TimeSignature(1, 0), std::invalid_argument);

        REQUIRE(TimeSignature { 3, 4 }.toString() == "|3/4|");
    }

    SECTION("compound")
    {
        TimeSignature ts;
        REQUIRE(ts.duration() == Duration { 4, 4 });

        REQUIRE(ts.append(1, 16));
        REQUIRE(ts.signatures()[1] == Duration(1, 16));
        REQUIRE(ts.toString() == "|4/4+1/16|");
        REQUIRE(!ts.isSimple());
        REQUIRE(ts.isCompound());
        REQUIRE(ts.duration() == Duration { 17, 16 });
    }

    SECTION("timeMs")
    {
        TimeSignature ts;
        REQUIRE(ts.timeMs({ 60, 4 }) == 4000);
        REQUIRE(ts.timeMs({ 120, 4 }) == 2000);

        ts.set(5, 4);
        REQUIRE(ts.timeMs({ 60, 4 }) == 5000);
    }

    SECTION("subdivision")
    {
        TimeSignature ts(4, 4);
        REQUIRE(ts.subDivision() == 4);
        ts.append(3, 8);
        REQUIRE(ts.subDivision() == 8);
        ts.append(1, 2);
        REQUIRE(ts.subDivision() == 8);
        ts.append(1, 8);
        REQUIRE(ts.duration() == Duration { 2, 1 });
        REQUIRE(ts.subDivision() == 8);
        ts.append(1, 16);
        REQUIRE(ts.subDivision() == 16);
    }

    SECTION("beatList")
    {
        TimeSignature ts(4, 4);
        auto bl = ts.beatList();
        REQUIRE(bl.size() == 4);
        REQUIRE(bl[0] == BeatData { BEAT_DOWN, 4 });
        REQUIRE(bl[1] == BeatData { BEAT_OFF, 4 });
        REQUIRE(bl[2] == BeatData { BEAT_OFF, 4 });
        REQUIRE(bl[3] == BeatData { BEAT_OFF, 4 });

        ts.set(5, 4);
        bl = ts.beatList();
        REQUIRE(bl.size() == 5);
        REQUIRE(bl[0] == BeatData { BEAT_DOWN, 4 });
        REQUIRE(bl[1] == BeatData { BEAT_OFF, 4 });
        REQUIRE(bl[2] == BeatData { BEAT_OFF, 4 });
        REQUIRE(bl[3] == BeatData { BEAT_ON, 4 });
        REQUIRE(bl[4] == BeatData { BEAT_OFF, 4 });

        ts.set(3, 4);
        ts.append(4, 4);
        bl = ts.beatList();
        REQUIRE(bl.size() == 7);

        ts.set(8, 8);
        bl = ts.beatList();
        REQUIRE(bl.size() == 8);

        ts.set(16, 16);
        bl = ts.beatList();
        REQUIRE(bl.size() == 16);

        ts.set(9, 4);
        ts.append(5, 8);
        bl = ts.beatList();
        REQUIRE(bl.size() == 14);

        ts.append(1, 16);
        bl = ts.beatList();
        REQUIRE(bl.size() == 15);

        auto D = BEAT_DOWN;
        auto x = BEAT_ON;
        auto _ = BEAT_OFF;

        REQUIRE(bl[0] == BeatData { D, 4 });
        REQUIRE(bl[1] == BeatData { _, 4 });
        REQUIRE(bl[2] == BeatData { _, 4 });
        REQUIRE(bl[3] == BeatData { x, 4 });
        REQUIRE(bl[4] == BeatData { _, 4 });
        REQUIRE(bl[5] == BeatData { _, 4 });
        REQUIRE(bl[6] == BeatData { x, 4 });
        REQUIRE(bl[7] == BeatData { _, 4 });
        REQUIRE(bl[8] == BeatData { _, 4 });
        REQUIRE(bl[9] == BeatData { x, 8 });
        REQUIRE(bl[10] == BeatData { _, 8 });
        REQUIRE(bl[11] == BeatData { _, 8 });
        REQUIRE(bl[12] == BeatData { x, 8 });
        REQUIRE(bl[13] == BeatData { _, 8 });
        REQUIRE(bl[14] == BeatData { x, 16 });
    }

    SECTION("parse")
    {
        TimeSignature ts(4, 4);
        REQUIRE(ts.parse("|3/8|"));
        REQUIRE(ts.toString() == "|3/8|");
        REQUIRE(ts.duration() == Duration { 3, 8 });

        REQUIRE(ts.parse("|3/8+1/7|"));
        REQUIRE(ts.toString() == "|3/8+1/7|");
        REQUIRE(ts.duration() == Duration { 3, 8 } + Duration { 1, 7 });
    }

    SECTION("==")
    {
        REQUIRE(TimeSignature(1, 4) == TimeSignature(1, 4));
        REQUIRE_FALSE(TimeSignature(1, 4) != TimeSignature(1, 4));
        REQUIRE(TimeSignature(2, 8) == TimeSignature(2, 8));
        REQUIRE(TimeSignature(6, 8) == TimeSignature(3, 4));
        REQUIRE_FALSE(TimeSignature(6, 8) != TimeSignature(3, 4));
    }

    SECTION("strictEqual")
    {
        REQUIRE(TimeSignature(1, 4).strictEqual({ 1, 4 }));
        REQUIRE_FALSE(TimeSignature(1, 4).strictEqual({ 2, 8 }));
        REQUIRE_FALSE(TimeSignature(3, 4).strictEqual({ 6, 8 }));
    }
}
