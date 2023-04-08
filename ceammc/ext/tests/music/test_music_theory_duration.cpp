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
#include "ceammc_music_theory_duration.h"

#include "catch.hpp"

using namespace ceammc::music;

TEST_CASE("MusicTheory::Duration", "[ceammc::music]")
{
    SECTION("init")
    {
        Duration d;
        REQUIRE(d.numerator() == 1);
        REQUIRE(d.division() == 4);
        REQUIRE(d.dots() == 0);
        REQUIRE(d.toString() == "1/4");

        REQUIRE(d == Duration {});
        REQUIRE(d == Duration { 4 });
        REQUIRE(d == Duration { 1, 4 });
        REQUIRE(d == Duration { 1, 4, 0 });
        REQUIRE(d == Duration { 2, 8 });
        REQUIRE(d == Duration { 25, 100 });

        REQUIRE(d != Duration { 5 });
        REQUIRE(d != Duration { 1, 5 });
        REQUIRE(d != Duration { 0, 4 });
        REQUIRE(d != Duration { 1, 4, 1 });

        REQUIRE(d.strictEqual({ 1, 4 }));
        REQUIRE(!d.strictEqual({ 2, 8 }));

        REQUIRE(Duration { 2, 8 }.normalized().strictEqual({ 1, 4 }));
        REQUIRE(Duration { 25, 100 }.normalized().strictEqual({ 1, 4 }));
        REQUIRE(Duration { 1, 4, 1 }.normalized().strictEqual({ 3, 8 }));
        REQUIRE(Duration { 1, 4, 2 }.normalized().strictEqual({ 7, 16 }));
        REQUIRE(Duration { 1, 4, 3 }.normalized().strictEqual({ 15, 32 }));
        REQUIRE(Duration { 2, 4, 1 }.normalized().strictEqual({ 3, 4 }));
        REQUIRE(Duration { 2, 4, 2 }.normalized().strictEqual({ 7, 8 }));

        REQUIRE(Duration { 3, 4 }.ratio() == 0.75);
        REQUIRE(Duration { 6, 8 }.ratio() == 0.75);
        REQUIRE(Duration { 1, 1, 1 }.ratio() == 1.5);
        REQUIRE(Duration { 1, 1, 2 }.ratio() == 1.75);
        REQUIRE(Duration { 1, 1, 3 }.ratio() == 1.875);

        REQUIRE_FALSE(Duration(1, 4) > Duration(1, 4));
        REQUIRE(Duration(1, 4) > Duration(1, 8));
        REQUIRE(Duration(1, 4, 1) > Duration(1, 4));
        REQUIRE(Duration(2, 4, 2) > Duration(2, 4, 1));

        REQUIRE(Duration(1, 4) >= Duration(1, 4));
        REQUIRE(Duration(1, 4) >= Duration(1, 8));
        REQUIRE(Duration(1, 4, 1) >= Duration(1, 4));
        REQUIRE(Duration(2, 4, 2) >= Duration(2, 4, 1));

        REQUIRE(Duration(1, 4) <= Duration(1, 4));
        REQUIRE(Duration(1, 16) <= Duration(1, 8));
        REQUIRE(Duration(1, 4) <= Duration(1, 4, 1));
        REQUIRE(Duration(2, 4, 2) <= Duration(2, 4, 3));

        REQUIRE_FALSE(Duration(1, 4) < Duration(1, 4));
        REQUIRE(Duration(1, 16) < Duration(1, 8));
        REQUIRE(Duration(1, 4) < Duration(1, 4, 1));
        REQUIRE(Duration(2, 4, 2) < Duration(2, 4, 3));

        REQUIRE(Duration(1, 4) + Duration(1, 8) == Duration(3, 8));
        REQUIRE(Duration(1, 4) + Duration(1, 16) == Duration(5, 16));
        REQUIRE(Duration(1, 4, 1) + Duration(1, 16) == Duration(7, 16));

        REQUIRE(Duration(1, 4) - Duration(1, 8) == Duration(1, 8));
        REQUIRE(Duration(1, 4) - Duration(1, 16) == Duration(3, 16));
        REQUIRE(Duration(1, 4) - Duration(1, 4) == Duration(0, 4));
        REQUIRE(Duration(1, 4) - Duration(2, 8) == Duration(0, 4));

        REQUIRE(Duration(1, 4) * 2 == Duration(1, 2));
        REQUIRE(Duration(1, 4, 1) * 2 == Duration(3, 4));
        REQUIRE(Duration(3, 8) * -1 == Duration(-3, 8));

        REQUIRE(Duration(1, 4) * Duration(1, 1) == Duration(1, 4));
        REQUIRE(Duration(1, 4) * Duration(1, 2) == Duration(1, 8));
        REQUIRE(Duration(4, 4) * Duration(5, 5) == Duration(6, 6));

        REQUIRE(Duration().parse("3/32"));
        REQUIRE(Duration().parse("4"));
        REQUIRE(Duration().parse("4.."));
        REQUIRE(Duration().parse("3/8..."));
        REQUIRE_FALSE(Duration().parse("???"));
        REQUIRE_FALSE(Duration().parse(""));
    }
}
