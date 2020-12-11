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
#include "flow_count.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowCount, flow, count)

TEST_CASE("flow.count", "[externals]")
{
    pd_test_init();
    setup_flow_count();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("flow.count");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @value, 0.);
        }

        SECTION("arg")
        {
            TObj t("flow.count", LF(2));
            REQUIRE_PROPERTY(t, @value, 2);
        }

        SECTION("props")
        {
            TObj t("flow.count", LA("@value", 3));
            REQUIRE_PROPERTY(t, @value, 3);
        }
    }

    SECTION("process")
    {
        TObj t("flow.count");
        REQUIRE_PROPERTY(t, @value, 0.f);

        // bang
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);
        REQUIRE_PROPERTY(t, @value, 2);

        // float
        WHEN_SEND_FLOAT_TO(0, t, 22);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 3);
        WHEN_SEND_FLOAT_TO(0, t, 22);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 4);

        // symbol
        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 5);
        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 6);

        // list
        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 7);

        // any
        WHEN_SEND_ANY_TO(t, "test", LA("A"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 8);

        // data
        WHEN_SEND_DATA_TO(0, t, IntData(123));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 9);
        REQUIRE_PROPERTY(t, @value, 9);

        t.storeAllMessageCount();
        t.sendBang(1);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY(t, @value, 0.);
    }

    SECTION("reset init")
    {
        TObj t("flow.count", LF(2));
        REQUIRE_PROPERTY(t, @value, 2);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 3);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 4);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 5);

        t.sendBang(1);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 3);
    }

    SECTION("pd ext")
    {
        TExt t("flow.count");

        t << IntA(1024);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(0) == 1);
    }
}
