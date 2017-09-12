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
#include "../random/random_linear.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<RandomLinear> RandomLinearTest;

#define REQUIRE_OUTPUT_RANGE(obj, from, to)                   \
    {                                                         \
        REQUIRE(obj.hasNewMessages(0));                       \
        REQUIRE(obj.lastMessage(0).isFloat());                \
        t_float v = obj.lastMessage(0).atomValue().asFloat(); \
        REQUIRE(from <= v);                                   \
        REQUIRE(v < to);                                      \
    }

TEST_CASE("random_linear", "[PureData]")
{
    SECTION("init")
    {
        SECTION("default")
        {
            RandomLinearTest t("random.linear");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @v0, 0.f);
            REQUIRE_PROPERTY(t, @v1, 1.f);

            REQUIRE_PROPERTY(t, @p0, 1.f);
            REQUIRE_PROPERTY(t, @p1, 0.f);
        }

        SECTION("properties")
        {
            RandomLinearTest t("random.linear", L8("@v0", 2, "@v1", 20, "@p0", 1.2f, "@p1", 0.1f));

            REQUIRE_PROPERTY(t, @v0, 2);
            REQUIRE_PROPERTY(t, @v1, 20);

            REQUIRE_PROPERTY(t, @p0, 1.2f);
            REQUIRE_PROPERTY(t, @p1, 0.1f);
        }
    }

    SECTION("gen")
    {
        RandomLinearTest t("random.linear");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_OUTPUT_RANGE(t, 0, 1);

        t.setProperty("@v1", A(0.5f));
        int n = 100;
        while (n--) {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_OUTPUT_RANGE(t, 0, 0.5f);
        }
    }

    SECTION("error")
    {
        RandomLinearTest t("random.linear", L4("@v0", 3, "@v1", 1));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }
}
