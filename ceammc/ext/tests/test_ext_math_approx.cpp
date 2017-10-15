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
#include "../math/math_approx.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<MathApprox> ApproxTest;

#define REQUIRE_APPROX(obj, v)            \
    {                                     \
        WHEN_SEND_FLOAT_TO(0, obj, v);    \
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1); \
    }

#define REQUIRE_NO_APPROX(obj, v)         \
    {                                     \
        WHEN_SEND_FLOAT_TO(0, obj, v);    \
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0); \
    }

TEST_CASE("approx", "[externals]")
{
    obj_init();
    SECTION("init")
    {
        SECTION("empty args")
        {
            ApproxTest t("approx");
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @value, 0.f);
            REQUIRE_PROPERTY(t, @epsilon, 0.01f);
        }

        SECTION("properties")
        {
            ApproxTest t("approx", L2("@value", 10));
            REQUIRE_PROPERTY(t, @value, 10);
            REQUIRE_PROPERTY(t, @epsilon, 0.01f);

            {
                ApproxTest t("approx", L2("@epsilon", 10));
                REQUIRE_PROPERTY(t, @value, 0.f);
                REQUIRE_PROPERTY(t, @epsilon, 10);
            }

            {
                ApproxTest t("approx", L4("@epsilon", 10, "@value", -100));
                REQUIRE_PROPERTY(t, @value, -100);
                REQUIRE_PROPERTY(t, @epsilon, 10);
            }
        }

        SECTION("positional args")
        {
            {
                ApproxTest t("approx", L2(20, 10));
                REQUIRE_PROPERTY(t, @value, 20);
                REQUIRE_PROPERTY(t, @epsilon, 10);
            }

            {
                ApproxTest t("approx", L2(20, "a"));
                REQUIRE_PROPERTY(t, @value, 20);
                REQUIRE_PROPERTY(t, @epsilon, 0.01f);
            }
        }
    }

    SECTION("float")
    {
        ApproxTest t("approx", L1(2));
        REQUIRE_APPROX(t, 2);
        REQUIRE_APPROX(t, 2.01f);
        REQUIRE_APPROX(t, 2.001f);
        REQUIRE_APPROX(t, 1.99f);
        REQUIRE_APPROX(t, 1.999f);

        REQUIRE_NO_APPROX(t, 2.02f);
        REQUIRE_NO_APPROX(t, 1.98f);

        // set negative pattern
        t.sendFloat(-2, 1);
        REQUIRE_APPROX(t, -2.01f);
        REQUIRE_APPROX(t, -1.99f);
        REQUIRE_NO_APPROX(t, -2.02f);
        REQUIRE_NO_APPROX(t, -1.98f);

        // set negative epsilon
        t.sendFloat(-0.5, 2);
        REQUIRE_APPROX(t, -2);
        REQUIRE_APPROX(t, -2.5f);
        REQUIRE_APPROX(t, -1.5f);
        REQUIRE_NO_APPROX(t, -1.49f);
        REQUIRE_NO_APPROX(t, -2.51f);
    }

    SECTION("list")
    {
        ApproxTest t("approx");

        WHEN_SEND_LIST_TO(0, t, L3(2, 2, 0.f));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, L3(2.1f, 2, 0.1f));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, L3(2.11f, 2, 0.1f));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, L3(1.99f, 2, 0.1f));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, L3(1.89f, 2, 0.1f));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
    }
}
