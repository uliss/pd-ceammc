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
#include "flow_reject.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowReject, flow, reject)

TEST_CASE("flow.reject", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            TObj t("flow.reject");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_LIST(t, @values, L());
        }

        SECTION("args")
        {
            TObj t("flow.reject", LA(1, 2, "b", "\"@c\""));
            REQUIRE_PROPERTY_LIST(t, @values, LA(1, 2, "b", "@c"));
        }

        SECTION("properties")
        {
            TObj t("flow.reject", LA(1, 2, "@values", "\"@c\""));
            REQUIRE_PROPERTY_LIST(t, @values, LA("@c"));
        }
    }

    SECTION("bang")
    {
        TObj t("flow.reject", LA(1, "a", "\"@c\""));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_BANG_AT_OUTLET(0, t);
    }

    SECTION("float")
    {
        TObj t("flow.reject", LF(1, 0.f, -1));

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, -1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 0.f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);

        WHEN_SEND_FLOAT_TO(0, t, -2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -2);

        WHEN_SEND_FLOAT_TO(0, t, 0.1f);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0.1);
    }

    SECTION("symbol")
    {
        TObj t("flow.reject", LA("a", "b"));

        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_SYMBOL_TO(0, t, "b");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_SYMBOL_TO(0, t, "c");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "c");

        WHEN_SEND_SYMBOL_TO(0, t, "d");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "d");
    }

    SECTION("list")
    {
        TObj t("flow.reject", LA("a", "b"));

        WHEN_SEND_LIST_TO(0, t, LA("a", "b"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LA("b", "b"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LA("a", "a"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LA("c", "d"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("c", "d"));
    }

    SECTION("any")
    {
        TObj t("flow.reject", LA("a", "b"));

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

        WHEN_SEND_ANY_TO(t, "@c", L());
        REQUIRE_ANY_AT_OUTLET(0, t, LA("@c"));
    }

    SECTION("real")
    {
        TExt t("flow.reject", LA(1, "\"@prop\""));
        REQUIRE_PROPERTY_LIST(t, @values, LA(1, "@prop"));

        t.sendListTo(LF(1, 2, 3), 1);
        REQUIRE_PROPERTY_LIST(t, @values, LA(1, 2, 3));
    }

    SECTION("alias")
    {
        TExt t("reject");
    }
}
