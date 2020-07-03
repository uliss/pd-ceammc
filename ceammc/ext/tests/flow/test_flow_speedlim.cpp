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
#include "flow_speedlim.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowSpeedLimit, flow, speedlim)

TEST_CASE("flow.speedlim", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            TObj t("flow.speedlim");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @limit, 0.f);
        }

        {
            TObj t("flow.speedlim", LF(100));
            REQUIRE_PROPERTY(t, @limit, 100);
        }

        {
            TObj t("flow.speedlim", LA("ABC"));
            REQUIRE_PROPERTY(t, @limit, 0.0f);
        }

        {
            TObj t("flow.speedlim", LF(-100));
            REQUIRE_PROPERTY(t, @limit, -100);
        }

        {
            TObj t("flow.speedlim", LA("@limit", 500));
            REQUIRE_PROPERTY(t, @limit, 500);
        }
    }

    SECTION("partial do")
    {
        SECTION("no limit")
        {
            TObj t("flow.speedlim");
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_BANG_AT_OUTLET(0, t);
            WHEN_SEND_FLOAT_TO(0, t, 200);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 200);
            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "ABC");
            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3));
            WHEN_SEND_ANY_TO(t, "msg", LF(100, 200));
            REQUIRE_ANY_AT_OUTLET(0, t, LA("msg", 100, 200));
            WHEN_SEND_DATA_TO(0, t, IntData(123));
            REQUIRE_DATA_AT_OUTLET(0, t, IntA(123));
        }

        SECTION("limit")
        {
            TObj t("flow.speedlim", LF(20));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_BANG_AT_OUTLET(0, t);
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_CALL(t, reset);
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_BANG_AT_OUTLET(0, t);

            WHEN_SEND_FLOAT_TO(0, t, 200);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_CALL(t, reset);
            WHEN_SEND_FLOAT_TO(0, t, 200);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 200);

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            t.clock_handler();
            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "ABC");

            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            t.clock_handler();
            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3));

            WHEN_SEND_ANY_TO(t, "msg", LF(100, 200));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            t.clock_handler();
            WHEN_SEND_ANY_TO(t, "msg", LF(100, 200));
            REQUIRE_ANY_AT_OUTLET(0, t, LA("msg", 100, 200));

            WHEN_SEND_DATA_TO(0, t, IntData(200));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            t.clock_handler();
            WHEN_SEND_DATA_TO(0, t, IntData(300));
            REQUIRE_DATA_AT_OUTLET(0, t, IntA(300));
        }
    }
}
