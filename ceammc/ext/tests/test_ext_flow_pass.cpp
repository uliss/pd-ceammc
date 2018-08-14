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
#include "../flow/flow_pass.h"
#include "test_base.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExternal<FlowPass> FlowPassTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("flow.pass", "[externals]")
{
    SECTION("init")
    {
        SECTION("empty")
        {
            FlowPassTest t("flow.pass");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_LIST(t, @values, L());
        }

        SECTION("args")
        {
            FlowPassTest t("flow.pass", LA(1, 2, "b", "@c"));
            REQUIRE_PROPERTY_LIST(t, @values, LA(1, 2, "b", "@c"));
        }

        SECTION("properties")
        {
            FlowPassTest t("flow.pass", LA(1, 2, "@values", "@c"));
            REQUIRE_PROPERTY_LIST(t, @values, LA(1, 2, "@values", "@c"));
        }
    }

    SECTION("float")
    {
        FlowPassTest t("flow.pass", LA(1, "a", "@c"));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_BANG_AT_OUTLET(0, t);
    }

    SECTION("float")
    {
        FlowPassTest t("flow.pass", LF(1, 0.f, -1));

        WHEN_SEND_FLOAT_TO(0, t, 1.1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, -1.1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 0.1f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_FLOAT_TO(0, t, -1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);

        WHEN_SEND_FLOAT_TO(0, t, 0.f);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0.f);
    }

    SECTION("symbol")
    {
        FlowPassTest t("flow.pass", LA("a", "b"));

        WHEN_SEND_SYMBOL_TO(0, t, "c");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_SYMBOL_TO(0, t, "d");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "a");

        WHEN_SEND_SYMBOL_TO(0, t, "b");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "b");
    }

    SECTION("list")
    {
        // all lists are passed
        FlowPassTest t("flow.pass", LA("a", "b"));

        WHEN_SEND_LIST_TO(0, t, LA("a", "b"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("a", "b"));

        WHEN_SEND_LIST_TO(0, t, LA("c", "d"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("c", "d"));
    }

    SECTION("any")
    {
        FlowPassTest t("flow.pass", LA("c", "@d"));

        WHEN_SEND_ANY_TO(t, "a", LF(1, 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_ANY_TO(t, "a", L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_ANY_TO(t, "b", LF(1, 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_ANY_TO(t, "b", L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_ANY_TO(t, "c", LF(1, 2));
        REQUIRE_ANY_AT_OUTLET(0, t, LA("c", 1, 2));

        WHEN_SEND_ANY_TO(t, "c", L());
        REQUIRE_ANY_AT_OUTLET(0, t, LA("c"));

        WHEN_SEND_ANY_TO(t, "@d", L());
        REQUIRE_ANY_AT_OUTLET(0, t, LA("@d"));
    }

    SECTION("real")
    {
        setup_flow0x2epass();

        pd::External flow_pass("flow.pass", LA(1, "c", "@prop"));
        REQUIRE_FALSE(flow_pass.isNull());
    }
}
