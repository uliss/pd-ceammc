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
#include "random_pwconst.h"
#include "test_random_base.h"

PD_COMPLETE_TEST_SETUP(RandomPWConst, random, pw_const)

TEST_CASE("random.pw_const", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("random.pw_const");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_LIST(t, @v, L());
            REQUIRE_PROPERTY_LIST(t, @bounds, L());
            REQUIRE_PROPERTY_LIST(t, @weights, L());

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_THAT(t, !hasOutput(&t));
        }

        SECTION("properties")
        {
            TObj t("random.pw_const", LA("@v", 1, 0.1, 2, 0.2, 3));

            REQUIRE_PROPERTY_LIST(t, @v, LX(1, 0.1, 2, 0.2, 3));
            REQUIRE_PROPERTY_LIST(t, @bounds, LX(1, 2, 3));
            REQUIRE_PROPERTY_LIST(t, @weights, LX(0.1, 0.2));
        }

        SECTION("args")
        {
            TObj t("random.pw_const", LA(1, 0.1, 2, 0.2, 3));

            REQUIRE_PROPERTY_LIST(t, @v, LX(1, 0.1, 2, 0.2, 3));
            REQUIRE_PROPERTY_LIST(t, @bounds, LX(1, 2, 3));
            REQUIRE_PROPERTY_LIST(t, @weights, LX(0.1, 0.2));
        }
    }

    SECTION("onList")
    {
        TObj t("random.pw_const");
        REQUIRE_PROPERTY_LIST(t, @v, L());
        REQUIRE_PROPERTY_LIST(t, @bounds, L());
        REQUIRE_PROPERTY_LIST(t, @weights, L());

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LF(2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LA(1, 0.f, 3, 1, 6));
        REQUIRE_PROPERTY_LIST(t, @v, LA(1, 0.f, 3, 1, 6));
        REQUIRE_PROPERTY_LIST(t, @bounds, LF(1, 3, 6));
        REQUIRE_PROPERTY_LIST(t, @weights, LF(0.f, 1));
        REQUIRE_THAT(t, outputInRange(&t, 3, 6));

        int n = 100;
        while (n--) {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_THAT(t, outputInRange(&t, 3, 6));
        }

        SECTION("error odd")
        {
            // even value count
            WHEN_SEND_LIST_TO(0, t, LA(1, 0.f, 3, 4, 5, 6));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            // values not changed
            REQUIRE_PROPERTY_LIST(t, @v, LA(1, 0.f, 3, 1, 6));
            REQUIRE_PROPERTY_LIST(t, @bounds, LF(1, 3, 6));
            REQUIRE_PROPERTY_LIST(t, @weights, LF(0.f, 1));
        }

        SECTION("error few")
        {
            // to few values
            WHEN_SEND_LIST_TO(0, t, LA(0.f, 10, 2, 3));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            // values not changed
            REQUIRE_PROPERTY_LIST(t, @v, LA(1, 0.f, 3, 1, 6));
            REQUIRE_PROPERTY_LIST(t, @bounds, LF(1, 3, 6));
            REQUIRE_PROPERTY_LIST(t, @weights, LF(0.f, 1));
        }

        SECTION("error negative weights")
        {
            WHEN_SEND_LIST_TO(0, t, LA(0.f, 10, 2, -1, 2));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            // values not changed
            REQUIRE_PROPERTY_LIST(t, @v, LA(1, 0.f, 3, 1, 6));
            REQUIRE_PROPERTY_LIST(t, @bounds, LF(1, 3, 6));
            REQUIRE_PROPERTY_LIST(t, @weights, AtomList(0.f, 1));
        }

        SECTION("error bounds not strictly increasing")
        {
            WHEN_SEND_LIST_TO(0, t, LF(0.f, 10, 2, 10, 2, 10, 11));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            // values not changed
            REQUIRE_PROPERTY_LIST(t, @v, LA(1, 0.f, 3, 1, 6));
            REQUIRE_PROPERTY_LIST(t, @bounds, LF(1, 3, 6));
            REQUIRE_PROPERTY_LIST(t, @weights, LF(0.f, 1));
        }
    }

    SECTION("gen")
    {
        TObj t("random.pw_const", LF(1, 1, 2, 0.f, 3, 10, 4));

        int b0 = 0;
        int b1 = 0;
        int b2 = 0;
        int n = 100;
        while (n--) {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_THAT(t, outputInRange(&t, 0, 4));
            float v = t.lastMessage(0).atomValue().asFloat();
            if (v < 2) {
                b0++;
                continue;
            }

            if (v < 3) {
                b1++;
                continue;
            }

            if (v < 4) {
                b2++;
                continue;
            }
        }

        REQUIRE(b1 == 0);
        REQUIRE(b0 != 0);
        REQUIRE(b2 != 0);
        REQUIRE(b0 < b2);
        REQUIRE(b0 + b2 == 100);
    }
}
