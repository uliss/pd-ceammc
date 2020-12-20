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
#include "math_gcd.h"
#include "test_math_base.h"


typedef TestExternal<MathGCD> MathGCDTest;

#define REQUIRE_GCD(in, t, out)             \
    {                                       \
        WHEN_SEND_FLOAT_TO(0, t, in);       \
        REQUIRE_FLOAT_AT_OUTLET(0, t, out); \
    }

#define REQUIRE_GCD_2(in1, in2, t, out)        \
    {                                          \
        WHEN_SEND_LIST_TO(0, t, LA(in1, in2)); \
        REQUIRE_FLOAT_AT_OUTLET(0, t, out);    \
    }

TEST_CASE("math.gcd", "[externals]")
{
    SECTION("default")
    {
        MathGCDTest t("math.gcd", LF(10));
        REQUIRE_GCD(2, t, 2);
        REQUIRE_GCD(3, t, 1);
        REQUIRE_GCD(4, t, 2);

        REQUIRE_GCD_2(14, 9, t, 1);
        REQUIRE_GCD_2(18, 9, t, 9);

        {
            MathGCDTest t("math.gcd");
            REQUIRE_GCD(1, t, 1);
            REQUIRE_GCD(2, t, 2);
            REQUIRE_GCD(20, t, 20);

            REQUIRE_GCD_2(15, 20, t, 5);
        }
    }
}
