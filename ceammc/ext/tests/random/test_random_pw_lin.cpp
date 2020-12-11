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
#include "random_pwlin.h"
#include "test_random_base.h"

PD_COMPLETE_TEST_SETUP(RandomPwLinear, random, pw_lin)

TEST_CASE("random.pw_lin", "[PureData]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("random.pw_lin");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_LIST(t, @v, L());
            REQUIRE_PROPERTY_LIST(t, @bounds, L());
            REQUIRE_PROPERTY_LIST(t, @weights, L());
        }

        SECTION("properties")
        {
            TObj t("random.pw_lin", LA("@v", 1, 0.1, 2, 0.2));

            REQUIRE_PROPERTY_LIST(t, @v, LX(1, 0.1, 2, 0.2));
            REQUIRE_PROPERTY_LIST(t, @bounds, LX(1, 2));
            REQUIRE_PROPERTY_LIST(t, @weights, LX(0.1, 0.2));
        }

        SECTION("args")
        {
            TObj t("random.pw_lin", LA(1, 0.1, 2, 0.2));

            REQUIRE_PROPERTY_LIST(t, @v, LX(1, 0.1, 2, 0.2));
            REQUIRE_PROPERTY_LIST(t, @bounds, LX(1, 2));
            REQUIRE_PROPERTY_LIST(t, @weights, LX(0.1, 0.2));
        }
    }

    SECTION("onList")
    {
        TObj t("random.pw_lin");
        REQUIRE_PROPERTY_LIST(t, @v, L());
        REQUIRE_PROPERTY_LIST(t, @bounds, L());
        REQUIRE_PROPERTY_LIST(t, @weights, L());

        WHEN_SEND_LIST_TO(0, t, LA(0.f, 10, 3, 1));
        REQUIRE_PROPERTY_LIST(t, @v, LA(0.f, 10, 3, 1));
        REQUIRE_PROPERTY_LIST(t, @bounds, AtomList(0.f, 3));
        REQUIRE_PROPERTY_LIST(t, @weights, AtomList(10, 1));
        REQUIRE_THAT(t, outputInRange(&t, 0, 3));

        int n = 100;
        while (n--) {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_THAT(t, outputInRange(&t, 0, 3));
        }

        SECTION("error odd")
        {
            // odd value count
            WHEN_SEND_LIST_TO(0, t, LF(0.f, 10, 3));
            REQUIRE_THAT(t, !hasOutput(&t));

            // values not changed
            REQUIRE_PROPERTY_LIST(t, @v, LA(0.f, 10, 3, 1));
            REQUIRE_PROPERTY_LIST(t, @bounds, AtomList(0.f, 3));
            REQUIRE_PROPERTY_LIST(t, @weights, AtomList(10, 1));
        }

        SECTION("error few")
        {
            // to few values
            WHEN_SEND_LIST_TO(0, t, LF(0.f, 10));
            REQUIRE_THAT(t, !hasOutput(&t));

            // values not changed
            REQUIRE_PROPERTY_LIST(t, @v, LA(0.f, 10, 3, 1));
            REQUIRE_PROPERTY_LIST(t, @bounds, AtomList(0.f, 3));
            REQUIRE_PROPERTY_LIST(t, @weights, AtomList(10, 1));
        }

        SECTION("error negative weights")
        {
            WHEN_SEND_LIST_TO(0, t, LA(0.f, 10, 2, -1));
            REQUIRE_THAT(t, !hasOutput(&t));

            // values not changed
            REQUIRE_PROPERTY_LIST(t, @v, LA(0.f, 10, 3, 1));
            REQUIRE_PROPERTY_LIST(t, @bounds, AtomList(0.f, 3));
            REQUIRE_PROPERTY_LIST(t, @weights, AtomList(10, 1));
        }

        SECTION("error bounds not strictly increasing")
        {
            WHEN_SEND_LIST_TO(0, t, LA(0.f, 10, 2, 10, 2, 10));
            REQUIRE_THAT(t, !hasOutput(&t));

            // values not changed
            REQUIRE_PROPERTY_LIST(t, @v, LA(0.f, 10, 3, 1));
            REQUIRE_PROPERTY_LIST(t, @bounds, AtomList(0.f, 3));
            REQUIRE_PROPERTY_LIST(t, @weights, AtomList(10, 1));
        }
    }

    SECTION("gen")
    {
        TObj t("random.pw_lin", LA(0.f, 1, 2, 2, 4, 50));

        int lt2 = 0;
        int ge2 = 0;
        int n = 100;
        while (n--) {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_THAT(t, outputInRange(&t, 0, 4));
            float v = t.lastMessage(0).atomValue().asFloat();
            if (v < 2)
                lt2++;
            else
                ge2++;
        }

        REQUIRE(lt2 < ge2);
        REQUIRE(lt2 + ge2 == 100);
    }

    SECTION("error")
    {
        TObj t("random.pw_lin", LA("@v0", 3, "@v1", 1));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_THAT(t, !hasOutput(&t));
    }
}
