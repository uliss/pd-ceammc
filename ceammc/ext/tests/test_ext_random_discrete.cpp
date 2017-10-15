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
#include "../random/random_discrete.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<RandomDiscrete> RandomDiscreteTest;

#define REQUIRE_OUTPUT_RANGE(obj, from, to)                   \
    {                                                         \
        REQUIRE(obj.hasNewMessages(0));                       \
        REQUIRE(obj.lastMessage(0).isFloat());                \
        t_float v = obj.lastMessage(0).atomValue().asFloat(); \
        REQUIRE(from <= v);                                   \
        REQUIRE(v < to);                                      \
    }

TEST_CASE("random.discrete", "[PureData]")
{
    SECTION("init")
    {
        SECTION("default")
        {
            RandomDiscreteTest t("random.discrete");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_LIST(t, @weights, AtomList());

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }

        SECTION("properties")
        {
            RandomDiscreteTest t("random.discrete", L6("@weights", 1, 2, 3, 4, 5));
            REQUIRE_PROPERTY_LIST(t, @weights, L5(1, 2, 3, 4, 5));
        }

        SECTION("args")
        {
            RandomDiscreteTest t("random.discrete", L5(1, 2, 3, 4, 5));
            REQUIRE_PROPERTY_LIST(t, @weights, L5(1, 2, 3, 4, 5));
        }
    }

    SECTION("onList")
    {
        RandomDiscreteTest t("random.discrete");
        REQUIRE_PROPERTY_LIST(t, @weights, AtomList());

        WHEN_SEND_LIST_TO(0, t, AtomList());
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, L5(1, 2, 3, 4, 5));
        REQUIRE_PROPERTY_LIST(t, @weights, L5(1, 2, 3, 4, 5));
        REQUIRE_OUTPUT_RANGE(t, 0, 5);

        int n = 100;
        while (n--) {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_OUTPUT_RANGE(t, 0, 5);
        }

        SECTION("error few")
        {
            // to few values
            WHEN_SEND_LIST_TO(0, t, AtomList());
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            // values are not changed
            REQUIRE_PROPERTY_LIST(t, @weights, L5(1, 2, 3, 4, 5));
        }

        SECTION("error negative weights")
        {
            WHEN_SEND_LIST_TO(0, t, L3(1, 2, -3));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            // values are not changed
            REQUIRE_PROPERTY_LIST(t, @weights, L5(1, 2, 3, 4, 5));
        }
    }

    SECTION("gen")
    {
        RandomDiscreteTest t("random.discrete", L4(1, 3, 8, 16));

        int b0 = 0;
        int b1 = 0;
        int b2 = 0;
        int b3 = 0;
        int n = 1000;
        while (n--) {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_OUTPUT_RANGE(t, 0, 4);
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
            RandomDiscreteTest t("random.discrete", AtomList(1));
            int n = 100;
            while (n--) {
                WHEN_SEND_BANG_TO(0, t);
                float v = t.lastMessage(0).atomValue().asFloat();
                REQUIRE(v == 0);
            }
        }
    }
}
