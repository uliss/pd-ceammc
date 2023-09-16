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
        TExt t("flow.count");
        REQUIRE_PROPERTY(t, @value, 0.f);

        // bang
        t.sendBang();
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        t.sendBang();
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);
        REQUIRE_PROPERTY(t, @value, 2);

        // float
        t << 22;
        REQUIRE_FLOAT_AT_OUTLET(0, t, 3);
        t << 22;
        REQUIRE_FLOAT_AT_OUTLET(0, t, 4);

        // symbol
        t << "A";
        REQUIRE_FLOAT_AT_OUTLET(0, t, 5);
        t << "A";
        REQUIRE_FLOAT_AT_OUTLET(0, t, 6);

        // list
        t << LF(1, 2, 3);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 7);

        // any
        t.sendMessage("test", LA("A"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 8);

        // data
        t << IntA(123);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 9);
        REQUIRE_PROPERTY(t, @value, 9);

        t.sendBangTo(1);
        REQUIRE(!t.hasOutputAt(0));
        REQUIRE_PROPERTY(t, @value, 0.);
    }

    SECTION("reset init")
    {
        TExt t("flow.count", LF(2));
        REQUIRE_PROPERTY(t, @value, 2);

        t.sendBang();
        REQUIRE_FLOAT_AT_OUTLET(0, t, 3);
        t.sendBang();
        REQUIRE_FLOAT_AT_OUTLET(0, t, 4);
        t.sendBang();
        REQUIRE_FLOAT_AT_OUTLET(0, t, 5);

        t.sendBangTo(1);
        t.sendBang();
        REQUIRE_FLOAT_AT_OUTLET(0, t, 3);

        t.sendFloatTo(100, 1);
        t.sendBang();
        REQUIRE_FLOAT_AT_OUTLET(0, t, 101);

        t.sendMessageTo(Message(gensym("@value"), LF(25)), 1);
        t.sendBang();
        REQUIRE_FLOAT_AT_OUTLET(0, t, 26);
    }

    SECTION("pd ext")
    {
        TExt t("flow.count");

        t << IntA(1024);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(0) == 1);
    }

    SECTION("+")
    {
        TExt t("flow.count");
        REQUIRE_PROPERTY(t, @value, 0);

        t.sendMessageTo(Message(SYM("+"), AtomList()), 1);
        REQUIRE_PROPERTY(t, @value, 0);

        t.sendMessageTo(Message("+", 2), 1);
        REQUIRE_PROPERTY(t, @value, 2);

        t.sendMessageTo(Message("+", 3), 1);
        REQUIRE_PROPERTY(t, @value, 5);

        t.sendMessageTo(Message(SYM("-"), AtomList()), 1);
        REQUIRE_PROPERTY(t, @value, 5);

        t.sendMessageTo(Message("-", 1), 1);
        REQUIRE_PROPERTY(t, @value, 4);

        t.sendMessageTo(Message("-", 3), 1);
        REQUIRE_PROPERTY(t, @value, 1);
    }
}
