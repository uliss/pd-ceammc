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
#include "../random/random_pwlin.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<RandomPwLinear> RandomPWLTest;

#define REQUIRE_OUTPUT_RANGE(obj, from, to)                   \
    {                                                         \
        REQUIRE(obj.hasNewMessages(0));                       \
        REQUIRE(obj.lastMessage(0).isFloat());                \
        t_float v = obj.lastMessage(0).atomValue().asFloat(); \
        REQUIRE(from <= v);                                   \
        REQUIRE(v < to);                                      \
    }

TEST_CASE("random.pw_lin", "[PureData]")
{
    SECTION("init")
    {
        SECTION("default")
        {
            RandomPWLTest t("random.pw_lin");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_LIST(t, @v, AtomList());
            REQUIRE_PROPERTY_LIST(t, @bounds, AtomList());
            REQUIRE_PROPERTY_LIST(t, @weights, AtomList());
        }

        SECTION("properties")
        {
            RandomPWLTest t("random.pw_lin", L5("@v", 1, 0.1, 2, 0.2));

            REQUIRE_PROPERTY_LIST(t, @v, L4(1, 0.1, 2, 0.2));
            REQUIRE_PROPERTY_LIST(t, @bounds, AtomList(1, 2));
            REQUIRE_PROPERTY_LIST(t, @weights, AtomList(0.1, 0.2));
        }

        SECTION("args")
        {
            RandomPWLTest t("random.pw_lin", L4(1, 0.1, 2, 0.2));

            REQUIRE_PROPERTY_LIST(t, @v, L4(1, 0.1, 2, 0.2));
            REQUIRE_PROPERTY_LIST(t, @bounds, AtomList(1, 2));
            REQUIRE_PROPERTY_LIST(t, @weights, AtomList(0.1, 0.2));
        }
    }

    SECTION("onList")
    {
        RandomPWLTest t("random.pw_lin");
        REQUIRE_PROPERTY_LIST(t, @v, AtomList());
        REQUIRE_PROPERTY_LIST(t, @bounds, AtomList());
        REQUIRE_PROPERTY_LIST(t, @weights, AtomList());

        WHEN_SEND_LIST_TO(0, t, L4(0.f, 10, 3, 1));
        REQUIRE_PROPERTY_LIST(t, @v, L4(0.f, 10, 3, 1));
        REQUIRE_PROPERTY_LIST(t, @bounds, AtomList(0.f, 3));
        REQUIRE_PROPERTY_LIST(t, @weights, AtomList(10, 1));
        REQUIRE_OUTPUT_RANGE(t, 0, 3);

        int n = 100;
        while (n--) {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_OUTPUT_RANGE(t, 0, 3);
        }

        SECTION("error odd")
        {
            // odd value count
            WHEN_SEND_LIST_TO(0, t, L3(0.f, 10, 3));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            // values not changed
            REQUIRE_PROPERTY_LIST(t, @v, L4(0.f, 10, 3, 1));
            REQUIRE_PROPERTY_LIST(t, @bounds, AtomList(0.f, 3));
            REQUIRE_PROPERTY_LIST(t, @weights, AtomList(10, 1));
        }

        SECTION("error few")
        {
            // to few values
            WHEN_SEND_LIST_TO(0, t, L2(0.f, 10));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            // values not changed
            REQUIRE_PROPERTY_LIST(t, @v, L4(0.f, 10, 3, 1));
            REQUIRE_PROPERTY_LIST(t, @bounds, AtomList(0.f, 3));
            REQUIRE_PROPERTY_LIST(t, @weights, AtomList(10, 1));
        }

        SECTION("error negative weights")
        {
            WHEN_SEND_LIST_TO(0, t, L4(0.f, 10, 2, -1));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            // values not changed
            REQUIRE_PROPERTY_LIST(t, @v, L4(0.f, 10, 3, 1));
            REQUIRE_PROPERTY_LIST(t, @bounds, AtomList(0.f, 3));
            REQUIRE_PROPERTY_LIST(t, @weights, AtomList(10, 1));
        }

        SECTION("error bounds not strictly increasing")
        {
            WHEN_SEND_LIST_TO(0, t, L6(0.f, 10, 2, 10, 2, 10));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            // values not changed
            REQUIRE_PROPERTY_LIST(t, @v, L4(0.f, 10, 3, 1));
            REQUIRE_PROPERTY_LIST(t, @bounds, AtomList(0.f, 3));
            REQUIRE_PROPERTY_LIST(t, @weights, AtomList(10, 1));
        }
    }

    SECTION("gen")
    {
        RandomPWLTest t("random.pw_lin", L6(0.f, 1, 2, 2, 4, 50));

        int lt2 = 0;
        int ge2 = 0;
        int n = 100;
        while (n--) {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_OUTPUT_RANGE(t, 0, 4);
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
        RandomPWLTest t("random.pw_lin", L4("@v0", 3, "@v1", 1));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }
}
