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
#include "../flow/flow_speedlim.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<FlowSpeedLimit> FlowSpeedLimitTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("flow.speedlim", "[externals]")
{
    SECTION("init")
    {
        FlowSpeedLimitTest t("flow.speedlim");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @limit, 0.f);

        setup_flow_speedlim();

        {
            FlowSpeedLimitTest t("flow.speedlim", L1(100));
            REQUIRE_PROPERTY(t, @limit, 100);
        }

        {
            FlowSpeedLimitTest t("flow.speedlim", L1("ABC"));
            REQUIRE_PROPERTY(t, @limit, 0.0f);
        }

        {
            FlowSpeedLimitTest t("flow.speedlim", L1(-100));
            REQUIRE_PROPERTY(t, @limit, -100);
        }

        {
            FlowSpeedLimitTest t("flow.speedlim", L2("@limit", 500));
            REQUIRE_PROPERTY(t, @limit, 500);
        }
    }

    SECTION("partial do")
    {
        SECTION("no limit")
        {
            FlowSpeedLimitTest t("flow.speedlim");
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_BANG_AT_OUTLET(0, t);
            WHEN_SEND_FLOAT_TO(0, t, 200);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 200);
            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "ABC");
            WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, L3(1, 2, 3));
            WHEN_SEND_ANY_TO(t, "msg", L2(100, 200));
            REQUIRE_ANY_AT_OUTLET(0, t, L3("msg", 100, 200));
            DataPtr dp(new IntData(123));
            WHEN_SEND_DATA_TO(0, t, dp);
            REQUIRE_DATA_AT_OUTLET(0, t, dp.asAtom());
        }

        SECTION("limit")
        {
            FlowSpeedLimitTest t("flow.speedlim", L1(20));
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

            WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            t.clock_handler();
            WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, L3(1, 2, 3));

            WHEN_SEND_ANY_TO(t, "msg", L2(100, 200));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            t.clock_handler();
            WHEN_SEND_ANY_TO(t, "msg", L2(100, 200));
            REQUIRE_ANY_AT_OUTLET(0, t, L3("msg", 100, 200));

            DataPtr dp(new IntData(123));
            WHEN_SEND_DATA_TO(0, t, dp);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            t.clock_handler();
            WHEN_SEND_DATA_TO(0, t, dp);
            REQUIRE_DATA_AT_OUTLET(0, t, dp.asAtom());
        }
    }
}
