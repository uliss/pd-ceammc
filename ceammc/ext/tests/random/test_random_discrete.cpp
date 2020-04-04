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
#include "random_discrete.h"
#include "test_random_base.h"

PD_COMPLETE_TEST_SETUP(RandomDiscrete, random, discrete)

TEST_CASE("random.discrete", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("random.discrete");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_THAT(t, hasProperty(&t, "@weights"));

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_THAT(t, !hasOutput(&t));
        }

        SECTION("properties")
        {
            TObj t("random.discrete", LA("@weights", 1, 2, 3, 4, 5));
            REQUIRE_THAT(t, hasProperty(&t, "@weights", 1, 2, 3, 4, 5));
        }

        SECTION("args")
        {
            TObj t("random.discrete", LA(1, 2, 3, 4, 5));
            REQUIRE_THAT(t, hasProperty(&t, "@weights", 1, 2, 3, 4, 5));
        }
    }

    SECTION("onList")
    {
        TObj t("random.discrete");
        REQUIRE_PROPERTY_LIST(t, @weights, L());

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_THAT(t, !hasOutput(&t));

        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5));
        REQUIRE_THAT(t, hasProperty(&t, "@weights", 1, 2, 3, 4, 5));
        REQUIRE_THAT(t, outputInRange(&t, 0, 5));

        int n = 100;
        while (n--) {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_THAT(t, outputInRange(&t, 0, 5));
        }

        SECTION("error few")
        {
            // to few values
            WHEN_SEND_LIST_TO(0, t, L());
            REQUIRE_THAT(t, !hasOutput(&t));

            // values are not changed
            REQUIRE_THAT(t, hasProperty(&t, "@weights", 1, 2, 3, 4, 5));
        }

        SECTION("error negative weights")
        {
            WHEN_SEND_LIST_TO(0, t, LF(1, 2, -3));
            REQUIRE_THAT(t, !hasOutput(&t));

            // values are not changed
            REQUIRE_THAT(t, hasProperty(&t, "@weights", 1, 2, 3, 4, 5));
        }
    }

    SECTION("gen")
    {
        TObj t("random.discrete", LF(1, 3, 8, 16));

        int b0 = 0;
        int b1 = 0;
        int b2 = 0;
        int b3 = 0;
        int n = 1000;
        while (n--) {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_THAT(t, outputInRange(&t, 0, 4));
            float v = t.lastMessage(0).atomValue().asFloat();
            if (v < 1) {
                b0++;
                continue;
            }

            if (v < 2) {
                b1++;
                continue;
            }

            if (v < 3) {
                b2++;
                continue;
            }

            if (v < 4) {
                b3++;
                continue;
            }
        }

        REQUIRE(b0 < b1);
        REQUIRE(b1 < b2);
        REQUIRE(b2 < b3);
        REQUIRE(b0 + b1 + b2 + b3 == 1000);

        SECTION("const")
        {
            TObj t("random.discrete", LF(1));
            int n = 100;
            while (n--) {
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_THAT(t, outputFloat(&t, 0));
            }
        }
    }
}
