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
#include "../math/math_div.h"
#include "test_base.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExternal<MathDiv> MathDivTest;

TEST_CASE("math.div", "[externals]")
{
    obj_init();

    SECTION("init")
    {
        {
            MathDivTest t("math.div");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @div, 1.f);
        }

        {
            MathDivTest t("math.div", LF(5.f));
            REQUIRE_PROPERTY(t, @div, 5.f);
        }

        {
            MathDivTest t("math.div", LA("@div", 3));
            REQUIRE_PROPERTY(t, @div, 3);
        }
    }

    SECTION("do")
    {
        MathDivTest t("math.div");
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

        t.setProperty("@div", LF(2));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(0.5, 1));

        WHEN_SEND_LIST_TO(0, t, LA("A", "B"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("A", "B"));

        WHEN_SEND_LIST_TO(0, t, LA("A", 100));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("A", 50));
    }
}
