/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ceammc_timeline.h"

using namespace ceammc::tl;

TEST_CASE("timeline", "[ceammc::timelime]")
{
    SECTION("CueData")
    {
        CueData c1(0, 0);
        REQUIRE(c1.canvas() == 0);
        REQUIRE(c1.xPos() == 0);
        REQUIRE(c1.index() == -1);
        REQUIRE(c1.name() == "cue_x");

        c1.setIndex(11);
        REQUIRE(c1.index() == 11);
        REQUIRE(c1.name() == "cue_11");

        c1.setXPos(103);
        REQUIRE(c1.xPos() == 103);

        SECTION("==")
        {
            CueData c1(0, 0);
            CueData c2(0, 0);
            REQUIRE(c1 == c2);

            CueData c3(0, (t_object*)100);
            REQUIRE_FALSE(c1 == c3);
        }

        SECTION("<")
        {
            CueData c1(0, 0);
            CueData c2(0, 0);
            REQUIRE_FALSE(c1 < c2);

            c1.setXPos(100);
            REQUIRE(c1.xPos() == 100);
            REQUIRE(c2.xPos() == 0);
            REQUIRE(c2 < c1);
            c2.setXPos(200);
            REQUIRE(c1.xPos() == 100);
            REQUIRE(c2.xPos() == 200);
            REQUIRE(c1 < c2);
        }
    }
}
