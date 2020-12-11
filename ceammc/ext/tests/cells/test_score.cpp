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
#include "ceammc_score.h"
#include "test_common.h"

TEST_CASE("score", "[ceammc::Score]")
{
    SECTION("Bar")
    {
        Bar b;
        REQUIRE(b.numBeats() == 4);
        REQUIRE(b.numSubBeats() == 8);
        REQUIRE(b.tempoAt(0) == 60);
        REQUIRE(b.tempoAt(100) == 60);
        REQUIRE(b.beatDurationMs(0) == 1000);
        REQUIRE(b.subbeatDurationMs(0) == 500);
        REQUIRE(b.durationMs() == 4000);

        REQUIRE(b.timeMsAt(0) == 0);
        REQUIRE(b.timeMsAt(1) == 1000);
        REQUIRE(b.timeMsAt(2) == 2000);
        REQUIRE(b.timeMsAt(3) == 3000);

        REQUIRE(b.setAccel(60, 90));
        REQUIRE(b.tempoAt(0) == 60);
        REQUIRE(b.tempoAt(1) == 70);
        REQUIRE(b.tempoAt(2) == 80);
        REQUIRE(b.tempoAt(3) == 90);

        REQUIRE(b.setAccel(60, 180, 1));
        REQUIRE(b.tempoAt(0) == 60);
        REQUIRE(b.tempoAt(1) == 60);
        REQUIRE(b.tempoAt(2) == 120);
        REQUIRE(b.tempoAt(3) == 180);

        REQUIRE(b.beatDurationMs(0) == 1000);
        REQUIRE(b.beatDurationMs(1) == 1000);
        REQUIRE(b.beatDurationMs(2) == Approx(1000 / 2.0));
        REQUIRE(b.beatDurationMs(3) == Approx(1000 / 3.0));
        REQUIRE(b.durationMs() == Approx(2000 + 5000 / 6.0));

        REQUIRE(b.timeMsAt(0) == 0);
        REQUIRE(b.timeMsAt(1) == 1000);
        REQUIRE(b.timeMsAt(2) == 2000);
        REQUIRE(b.timeMsAt(3) == 2500);

        REQUIRE(b.setAccel(60, 80, 2));
        REQUIRE(b.tempoAt(0) == 60);
        REQUIRE(b.tempoAt(1) == 60);
        REQUIRE(b.tempoAt(2) == 60);
        REQUIRE(b.tempoAt(3) == 80);

        REQUIRE(b.timeMsAt(0) == 0);
        REQUIRE(b.timeMsAt(1) == 1000);
        REQUIRE(b.timeMsAt(2) == 2000);
        REQUIRE(b.timeMsAt(3) == 3000);

        REQUIRE_FALSE(b.setAccel(60, 80, 3));
    }

    SECTION("Score")
    {
        Score s0;
        REQUIRE(s0.numBars() == 0);

        Score s1(TimeSignature { 3, 2 }, 120);
        REQUIRE(s1.numBars() == 60);
        REQUIRE(s1.barAt(0).sig() == TimeSignature(3, 2));
    }
}
