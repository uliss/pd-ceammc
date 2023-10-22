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
#include "route_cycle.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(RouteCycle, route, cycle)

TEST_CASE("route.cycle", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    using M = Message;
    using ML = std::vector<Message>;
    const auto b = M::makeBang();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("route.cycle");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY_FLOAT(t, @n, 2);
            REQUIRE_PROPERTY_FLOAT(t, @offset, 0);
            REQUIRE_PROPERTY_FLOAT(t, @step, 1);
            REQUIRE_PROPERTY_FLOAT(t, @index, 0);
        }

        SECTION("args")
        {
            TExt t("route.cycle", LA(10));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 10);
            REQUIRE_PROPERTY_FLOAT(t, @n, 10);
            REQUIRE_PROPERTY_FLOAT(t, @offset, 0);
            REQUIRE_PROPERTY_FLOAT(t, @step, 1);
            REQUIRE_PROPERTY_FLOAT(t, @index, 0);
        }

        SECTION("args 2")
        {
            TExt t("route.cycle", LA(10, -2));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 10);
            REQUIRE_PROPERTY_FLOAT(t, @n, 10);
            REQUIRE_PROPERTY_FLOAT(t, @offset, -2);
            REQUIRE_PROPERTY_FLOAT(t, @step, 1);
            REQUIRE_PROPERTY_FLOAT(t, @index, 8);
        }

        SECTION("args 3")
        {
            TExt t("route.cycle", LA(10, 2, -1));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 10);
            REQUIRE_PROPERTY_FLOAT(t, @n, 10);
            REQUIRE_PROPERTY_FLOAT(t, @offset, 2);
            REQUIRE_PROPERTY_FLOAT(t, @step, -1);
            REQUIRE_PROPERTY_FLOAT(t, @index, 2);
        }
    }

    SECTION("simple")
    {
        TExt t("route.cycle", LF(3));

        t.bang();
        REQUIRE_BANG_AT_OUTLET(0, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(1, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(2, t);

        t << 1;
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        t << 2;
        REQUIRE_FLOAT_AT_OUTLET(1, t, 2);
        t << 3;
        REQUIRE_FLOAT_AT_OUTLET(2, t, 3);
        t << 4;
        REQUIRE_FLOAT_AT_OUTLET(0, t, 4);
        t << 5;
        REQUIRE_FLOAT_AT_OUTLET(1, t, 5);
        t << 6;
        REQUIRE_FLOAT_AT_OUTLET(2, t, 6);

        t << "A";
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");
        t << "B";
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "B");
        t << "C";
        REQUIRE_SYMBOL_AT_OUTLET(2, t, "C");
        t << "D";
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "D");

        t << LF(1, 2);
        REQUIRE_LIST_AT_OUTLET(1, t, LF(1, 2));
        t << LF(3, 4);
        REQUIRE_LIST_AT_OUTLET(2, t, LF(3, 4));
        t << LF(5, 6);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(5, 6));
        t << LF(7, 8);
        REQUIRE_LIST_AT_OUTLET(1, t, LF(7, 8));

        t.sendMessage("msg1");
        REQUIRE_ANY_AT_OUTLET(2, t, LA("msg1"));
        t.sendMessage("msg2", LF(1));
        REQUIRE_ANY_AT_OUTLET(0, t, LA("msg2", 1));
        t.sendMessage("@msg3");
        REQUIRE_ANY_AT_OUTLET(1, t, LA("@msg3"));
        REQUIRE_PROPERTY_FLOAT(t, @index, 2);

        t.sendBangTo(1);
        REQUIRE_PROPERTY_FLOAT(t, @index, 0);
    }

    SECTION("backward")
    {
        TExt t("route.cycle", LF(3, 0, -1));

        t.bang();
        REQUIRE_BANG_AT_OUTLET(0, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(2, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(1, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(0, t);
    }

    SECTION("@step +2")
    {
        TExt t("route.cycle", LF(5, 0, 2));

        t.bang();
        REQUIRE_BANG_AT_OUTLET(0, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(2, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(4, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(1, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(3, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(0, t);
    }

    SECTION("@step +2")
    {
        TExt t("route.cycle", LF(5, 0, -2));

        t.bang();
        REQUIRE_BANG_AT_OUTLET(0, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(3, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(1, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(4, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(2, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(0, t);
    }

    SECTION("@offset 1")
    {
        TExt t("route.cycle", LF(3, 1));

        t.bang();
        REQUIRE_BANG_AT_OUTLET(1, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(2, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(0, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(1, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(2, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(0, t);
    }

    SECTION("@offset -1")
    {
        TExt t("route.cycle", LF(4, -1));
        REQUIRE_PROPERTY_FLOAT(t, @index, 3);

        t.bang();
        REQUIRE_BANG_AT_OUTLET(3, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(0, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(1, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(2, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(3, t);
        t.bang();
        REQUIRE_BANG_AT_OUTLET(0, t);

        t.sendBangTo(1);
        REQUIRE_PROPERTY_FLOAT(t, @index, 3);
    }
}
