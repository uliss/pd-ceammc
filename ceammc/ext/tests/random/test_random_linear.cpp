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
#include "random_linear.h"
#include "test_random_base.h"

PD_COMPLETE_TEST_SETUP(RandomLinear, random, linear)

TEST_CASE("random_linear", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("random.linear");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_THAT(t, hasProperty(&t, "@v0", 0));
            REQUIRE_THAT(t, hasProperty(&t, "@v1", 1));

            REQUIRE_THAT(t, hasProperty(&t, "@p0", 1));
            REQUIRE_THAT(t, hasProperty(&t, "@p1", 0));
        }

        SECTION("properties")
        {
            TObj t("random.linear", LA("@v0", 2, "@v1", 20, "@p0", 1.5, "@p1", 0.125));

            REQUIRE_THAT(t, hasProperty(&t, "@v0", 2));
            REQUIRE_THAT(t, hasProperty(&t, "@v1", 20));

            REQUIRE_THAT(t, hasProperty(&t, "@p0", 1.5));
            REQUIRE_THAT(t, hasProperty(&t, "@p1", 0.125));
        }
    }

    SECTION("gen")
    {
        TObj t("random.linear");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_THAT(t, outputInRange(&t, 0, 1));

        t.setProperty("@v1", A(0.5f));
        int n = 100;
        while (n--) {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_THAT(t, outputInRange(&t, 0, 0.5));
        }
    }

    SECTION("error")
    {
        TObj t("random.linear", LA("@v0", 3, "@v1", 1));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_THAT(t, !hasOutput(&t));
    }
}
