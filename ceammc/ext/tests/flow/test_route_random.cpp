/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "route_random.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(RouteRandom, route, random)

TEST_CASE("route.random", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("route.random");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @n, 2);
            REQUIRE_PROPERTY(t, @seed, 0);
        }

        SECTION("args")
        {
            TExt t("route.r", LF(4));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 4);
            REQUIRE_PROPERTY(t, @n, 4);
            REQUIRE_PROPERTY(t, @seed, 0);
        }

        SECTION("props")
        {
            TExt t("route.r", LP("@n 16 @seed 24"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 16);
            REQUIRE_PROPERTY(t, @n, 16);
            REQUIRE_PROPERTY(t, @seed, 24);
        }
    }

    SECTION("run")
    {
        TExt t("route.r", LA(3, "@seed", 24));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 3);

#ifdef __APPLE__
        t.bang();
        REQUIRE_BANG_AT_OUTLET(2, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(0, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(1, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(1, t);
#endif
    }
}
