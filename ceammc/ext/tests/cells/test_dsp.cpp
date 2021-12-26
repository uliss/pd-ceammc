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
#include "ceammc_dsp.h"
#include "test_common.h"

using namespace ceammc::dsp;

TEST_CASE("dsp", "[ceammc::dsp]")
{
    SECTION("Delay")
    {
        REQUIRE_THROWS_AS(Delay(0, 0), std::invalid_argument);

        Delay d0(1, 0);
        REQUIRE(d0.maxSize() == 1);
        REQUIRE(d0.delay() == 0);

        REQUIRE(d0.setDelay(0));
        REQUIRE_FALSE(d0.setDelay(1));
        REQUIRE(d0.delay() == 0);
        REQUIRE(d0.push(123) == 123);
        REQUIRE(d0.push(-0.5) == -0.5);

        Delay d2(2, 1);
        REQUIRE(d2.maxSize() == 2);
        REQUIRE(d2.delay() == 1);
        REQUIRE(d2.setDelay(0));
        REQUIRE(d2.setDelay(1));
        REQUIRE(d2.delay() == 1);
        REQUIRE_FALSE(d2.setDelay(2));
        REQUIRE(d2.delay() == 1);

        REQUIRE(d2.push(100) == 0);
        REQUIRE(d2.push(200) == 100);
        REQUIRE(d2.push(300) == 200);

        d2.setDelay(0);
        REQUIRE(d2.push(400) == 400);

        Delay d3(3, 101, 1);
        REQUIRE(d3.delay() == 0);
        REQUIRE(d3.setDelay(2));

        REQUIRE(d3.push(100) == 1);
        REQUIRE(d3.push(200) == 1);
        REQUIRE(d3.push(300) == 100);
        d3.fillWith(0);
        REQUIRE(d3.push(400) == 0);
    }
}
