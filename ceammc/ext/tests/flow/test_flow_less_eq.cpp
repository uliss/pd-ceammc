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
#include "flow_less_eq.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowLessEq, flow, less_eq)

TEST_CASE("flow.less_eq", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        setup_flow_less_eq();

        SECTION("default")
        {
            TObj t("flow.less_eq");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 0);

            WHEN_SEND_FLOAT_TO(0, t, 123);
        }

        SECTION("single")
        {
            TObj t("flow.less_eq", LF(1));
            REQUIRE(t.numOutlets() == 2);

            WHEN_SEND_FLOAT_TO(0, t, -100);
            REQUIRE_FLOAT_AT_OUTLET(0, t, -100);

            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_FLOAT_TO(0, t, 1.0001);
            REQUIRE_FLOAT_AT_OUTLET(1, t, 1.0001);

            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_FLOAT_AT_OUTLET(1, t, 100);
        }

        SECTION("multiple")
        {
            TObj t("flow.less_eq", LF(-10, 1, 10)); // [. -10 . 1 . 10 .]
            REQUIRE(t.numOutlets() == 4);

            // 1st outlet
            WHEN_SEND_FLOAT_TO(0, t, -100);
            REQUIRE_FLOAT_AT_OUTLET(0, t, -100);

            WHEN_SEND_FLOAT_TO(0, t, -10);
            REQUIRE_FLOAT_AT_OUTLET(0, t, -10);

            // 2nd
            WHEN_SEND_FLOAT_TO(0, t, -9);
            REQUIRE_FLOAT_AT_OUTLET(1, t, -9);

            WHEN_SEND_FLOAT_TO(0, t, 0.f);
            REQUIRE_FLOAT_AT_OUTLET(1, t, 0.f);

            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(1, t, 1);

            // 3rd
            WHEN_SEND_FLOAT_TO(0, t, 1.1);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 1.1);

            WHEN_SEND_FLOAT_TO(0, t, 2);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 2);

            WHEN_SEND_FLOAT_TO(0, t, 9);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 9);

            WHEN_SEND_FLOAT_TO(0, t, 10);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 10);

            // 4th
            WHEN_SEND_FLOAT_TO(0, t, 10.1);
            REQUIRE_FLOAT_AT_OUTLET(3, t, 10.1);
        }

        SECTION("wrong args")
        {
            TObj t("flow.less_eq", LF(1, 10, 10)); // [. -10 . 1 . 10 .]
            REQUIRE(t.numOutlets() == 3);
        }
    }
}
