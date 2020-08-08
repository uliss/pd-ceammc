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
#include "flow_group.h"
#include "test_flow_base.h"

#define REQUIRE_SIZE(obj, n) REQUIRE_PROPERTY(t, @size, float(n));

#define REQUIRE_LIST(obj, lst)               \
    {                                        \
        WHEN_SEND_BANG_TO(0, obj);           \
        REQUIRE_SIZE(obj, lst.size());       \
        REQUIRE_LIST_AT_OUTLET(0, obj, lst); \
    }

PD_COMPLETE_TEST_SETUP(FlowGroup, flow, group)

TEST_CASE("flow.group", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TObj t("flow.group");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @by, 1);
            REQUIRE_PROPERTY(t, @free, 0.f);
        }

        SECTION("raw args")
        {
            TObj t("flow.group", LF(4, 5));
            REQUIRE_PROPERTY(t, @by, 4);
        }

        SECTION("props")
        {
            TObj t("flow.group", LA("@by", 5));
            REQUIRE_PROPERTY(t, @by, 5);
        }

        SECTION("alias")
        {
            TExt t("group");
            REQUIRE_PROPERTY(t, @by, 1);
        }
    }

    SECTION("onFloat")
    {
        TObj t("flow.group", LF(2));

        WHEN_SEND_FLOAT_TO(0, t, 12);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 13);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @free, 2);

        WHEN_SEND_FLOAT_TO(0, t, 14);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(12, 13));
        REQUIRE_PROPERTY(t, @free, 1);

        WHEN_SEND_FLOAT_TO(0, t, 15);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 16);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(14, 15));
    }

    SECTION("onSymbol")
    {
        TObj t("flow.group", LF(1));

        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_SYMBOL_TO(0, t, "B");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");

        WHEN_SEND_SYMBOL_TO(0, t, "C");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "B");
    }

    SECTION("onList")
    {
        TObj t("flow.group", LF(3));

        WHEN_SEND_LIST_TO(0, t, LA("A"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LA("B", "C"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LA("D", "E", "F"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("A", "B", "C"));

        WHEN_SEND_LIST_TO(0, t, LA("G", "H", "I", "J"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("G", "H", "I"));
    }

    SECTION("onData")
    {
        TExt t("flow.group", 2);

        t << IntA(1);
        REQUIRE(!t.hasOutput());

        t << IntA(2);
        REQUIRE(!t.hasOutput());

        t << IntA(3);
        REQUIRE(listAt(t) == LA(IntA(1), IntA(2)));
    }

    SECTION("on inlet")
    {
        TExt t("flow.group", 6);
        REQUIRE_PROPERTY(t, @by, 6);

        t.sendFloatTo(3, 1);
        REQUIRE_PROPERTY(t, @by, 3);
    }
}
