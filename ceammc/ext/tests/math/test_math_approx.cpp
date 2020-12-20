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
#include "math_approx.h"
#include "test_math_base.h"

PD_COMPLETE_TEST_SETUP(MathApprox, math, approx)

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
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty args")
        {
            TObj t("approx");
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @value, 0.);
            REQUIRE_PROPERTY(t, @epsilon, 0.01);
        }

        SECTION("properties")
        {
            TObj t("approx", LA("@value", 10));
            REQUIRE_PROPERTY(t, @value, 10);
            REQUIRE_PROPERTY(t, @epsilon, 0.01);

            {
                TObj t("approx", LA("@epsilon", 10));
                REQUIRE_PROPERTY(t, @value, 0.);
                REQUIRE_PROPERTY(t, @epsilon, 10);
            }

            {
                TObj t("approx", LA("@epsilon", 10, "@value", -100));
                REQUIRE_PROPERTY(t, @value, -100);
                REQUIRE_PROPERTY(t, @epsilon, 10);
            }
        }

        SECTION("positional args")
        {
            {
                TObj t("approx", LF(20, 10));
                REQUIRE_PROPERTY(t, @value, 20);
                REQUIRE_PROPERTY(t, @epsilon, 10);
            }

            {
                TObj t("approx", LA(20, "a"));
                REQUIRE_PROPERTY(t, @value, 20);
                REQUIRE_PROPERTY(t, @epsilon, 0.01);
            }
        }
    }

    SECTION("float")
    {
        TObj t("approx", LF(2));
        REQUIRE_APPROX(t, 2);
        REQUIRE_APPROX(t, 2.01);
        REQUIRE_APPROX(t, 2.001);
        REQUIRE_APPROX(t, 1.99);
        REQUIRE_APPROX(t, 1.999);

        REQUIRE_NO_APPROX(t, 2.02);
        REQUIRE_NO_APPROX(t, 1.98);

        // set negative pattern
        t.sendFloat(-2, 1);
        REQUIRE_APPROX(t, -2.01);
        REQUIRE_APPROX(t, -1.99);
        REQUIRE_NO_APPROX(t, -2.02);
        REQUIRE_NO_APPROX(t, -1.98);

        // set negative epsilon
        t.sendFloat(-0.5, 2);
        REQUIRE_APPROX(t, -2);
        REQUIRE_APPROX(t, -2.5);
        REQUIRE_APPROX(t, -1.5);
        REQUIRE_NO_APPROX(t, -1.49);
        REQUIRE_NO_APPROX(t, -2.51);
    }

    SECTION("list")
    {
        TObj t("approx");

        WHEN_SEND_LIST_TO(0, t, LF(2, 2, 0.));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, LF(2.09, 2, t_float(0.1)));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, LF(2.11, 2, 0.1));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, LF(1.99, 2, 0.1));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, LF(1.89, 2, 0.1));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
    }
}
