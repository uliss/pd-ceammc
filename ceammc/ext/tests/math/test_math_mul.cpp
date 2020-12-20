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
#include "math_mul.h"
#include "test_math_base.h"


typedef TestExternal<MathMul> MathMulTest;

TEST_CASE("math.mul", "[externals]")
{
    SECTION("init")
    {
        {
            MathMulTest t("math.mul");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @mul, 1.f);
        }

        {
            MathMulTest t("math.mul", LF(5.f));
            REQUIRE_PROPERTY(t, @mul, 5.f);
        }

        {
            MathMulTest t("math.mul", LA("@mul", 3));
            REQUIRE_PROPERTY(t, @mul, 3);
        }
    }

    SECTION("do")
    {
        MathMulTest t("math.mul");
        WHEN_SEND_FLOAT_TO(0, t, 0.f);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_FLOAT_TO(0, t, 100.f);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(100));

        WHEN_SEND_FLOAT_TO(0, t, -100.f);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(-100));

        // pass thru
        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2));

        WHEN_SEND_LIST_TO(0, t, LA("A", "B"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("A", "B"));

        WHEN_SEND_LIST_TO(0, t, LA("A", 100));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("A", 100));

        t.setProperty("@mul", LF(2.5));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(2.5, 5));

        WHEN_SEND_LIST_TO(0, t, LA("A", "B"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("A", "B"));

        WHEN_SEND_LIST_TO(0, t, LA("A", 100));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("A", 250));
    }
}
