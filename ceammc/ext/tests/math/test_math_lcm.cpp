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
#include "math_lcm.h"
#include "test_math_base.h"


typedef TestExternal<MathLCM> MathLCMTest;

#define REQUIRE_LCM(in, t, out)             \
    {                                       \
        WHEN_SEND_FLOAT_TO(0, t, in);       \
        REQUIRE_FLOAT_AT_OUTLET(0, t, out); \
    }

#define REQUIRE_LCM_2(in1, in2, t, out)        \
    {                                          \
        WHEN_SEND_LIST_TO(0, t, LA(in1, in2)); \
        REQUIRE_FLOAT_AT_OUTLET(0, t, out);    \
    }

TEST_CASE("math.lcm", "[externals]")
{
    SECTION("default")
    {
        MathLCMTest t("math.lcm", LF(5));
        REQUIRE_LCM(2, t, 10);
        REQUIRE_LCM(3, t, 15);
        REQUIRE_LCM(4, t, 20);

        REQUIRE_LCM_2(2, 3, t, 6);
        REQUIRE_LCM_2(2, 4, t, 4);

        {
            MathLCMTest t("math.lcm");
            REQUIRE_LCM(1, t, 0);
            REQUIRE_LCM(2, t, 0);
            REQUIRE_LCM(1000, t, 0);

            REQUIRE_LCM_2(15, 20, t, 60);
        }
    }
}
