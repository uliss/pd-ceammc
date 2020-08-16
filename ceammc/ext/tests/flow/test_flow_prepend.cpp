/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "flow_prepend.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowPrepend, flow, prepend)

TEST_CASE("flow.prepend", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("flow.prepend");
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @delay, 0);
            REQUIRE_PROPERTY(t, @msg, 0);
            REQUIRE_PROPERTY(t, @value);
        }

        SECTION("args")
        {
            TObj t("flow.prepend", LA(100));
            REQUIRE_PROPERTY(t, @delay, 0);
            REQUIRE_PROPERTY(t, @msg, 0);
            REQUIRE_PROPERTY(t, @value, 100);
        }

        SECTION("args")
        {
            TObj t("flow.prepend", LA("\"@delay\""));
            REQUIRE_PROPERTY(t, @delay, 0);
            REQUIRE_PROPERTY(t, @msg, 0);
            REQUIRE_PROPERTY(t, @value, "@delay");
        }

        SECTION("args")
        {
            TObj t("flow.prepend", LA("\"@delay\"", "@delay", 10, "@msg"));
            REQUIRE_PROPERTY(t, @delay, 10);
            REQUIRE_PROPERTY(t, @msg, 1);
            REQUIRE_PROPERTY(t, @value, "@delay");
        }
    }

    SECTION("do")
    {
        TExt t("flow.prepend", "\"@delay\"");

        t.sendFloat(100);
        REQUIRE(symbolAt(t) == "@delay");

        test::pdRunMainLoopMs(5);
        REQUIRE(floatAt(t) == 100);
    }

    SECTION("do")
    {
        TExt t("flow.prepend", 1, 2, 3, "@delay", -1);
        REQUIRE_PROPERTY(t, @value, LF(1, 2, 3));

        t.sendFloat(100);
        REQUIRE(t.messagesAt(0).size() == 2);
        REQUIRE(t.messagesAt(0)[0].listValue() == LF(1, 2, 3));
        REQUIRE(t.messagesAt(0)[1].atomValue() == A(100));

        t.clearAll();
        t << "ABC";
        REQUIRE(t.messagesAt(0)[0].listValue() == LF(1, 2, 3));
        REQUIRE(t.messagesAt(0)[1].atomValue() == S("ABC"));

        t.clearAll();
        t << LF(3, 2, 1);
        REQUIRE(t.messagesAt(0)[0].listValue() == LF(1, 2, 3));
        REQUIRE(t.messagesAt(0)[1].listValue() == LF(3, 2, 1));

        t.clearAll();
        t.sendMessage("reset");
        REQUIRE(t.messagesAt(0)[0].listValue() == LF(1, 2, 3));
        REQUIRE(t.messagesAt(0)[1].anyValue() == LA("reset"));

        t.clearAll();
        t.sendMessage("@value");
        REQUIRE(t.messagesAt(0)[0].listValue() == LF(1, 2, 3));
        REQUIRE(t.messagesAt(0)[1].anyValue() == LA("@value"));
    }

    SECTION("do") // invalid message
    {
        TExt t("flow.prepend", 1, 2, 3, "@delay", -1, "@msg");

        t.sendFloat(100);
        REQUIRE(t.messagesAt(0).size() == 1);
        REQUIRE(t.messagesAt(0)[0].atomValue() == A(100));
    }

    SECTION("do")
    {
        TExt t("flow.prepend", "@delay", -1, "@msg");

        t.sendFloat(100);
        REQUIRE(t.messagesAt(0).size() == 1);
        REQUIRE(t.messagesAt(0)[0].atomValue() == A(100));
    }

    SECTION("do")
    {
        TExt t("flow.prepend", "TEST", "@delay", 1);

        t.bang();
        REQUIRE(t.messagesAt(0).size() == 1);
        REQUIRE(t.messagesAt(0)[0].atomValue() == S("TEST"));
        test::pdRunMainLoopMs(5);
        REQUIRE(t.messagesAt(0).size() == 2);
        REQUIRE(t.messagesAt(0)[1].isBang());
        t.clearAll();

        t << 10.5;
        REQUIRE(t.messagesAt(0).size() == 1);
        REQUIRE(t.messagesAt(0)[0].atomValue() == S("TEST"));
        test::pdRunMainLoopMs(5);
        REQUIRE(t.messagesAt(0).size() == 2);
        REQUIRE(t.messagesAt(0)[1].atomValue().asFloat() == 10.5);
        t.clearAll();

        t << "ABC";
        REQUIRE(t.messagesAt(0).size() == 1);
        REQUIRE(t.messagesAt(0)[0].atomValue() == S("TEST"));
        test::pdRunMainLoopMs(5);
        REQUIRE(t.messagesAt(0).size() == 2);
        REQUIRE(t.messagesAt(0)[1].atomValue() == S("ABC"));
        t.clearAll();

        t << LF(1, 2, 3);
        REQUIRE(t.messagesAt(0).size() == 1);
        REQUIRE(t.messagesAt(0)[0].atomValue() == S("TEST"));
        test::pdRunMainLoopMs(5);
        REQUIRE(t.messagesAt(0).size() == 2);
        REQUIRE(t.messagesAt(0)[1].listValue() == LF(1, 2, 3));
        t.clearAll();

        t.sendMessage("@delay", 400);
        REQUIRE(t.messagesAt(0).size() == 1);
        REQUIRE(t.messagesAt(0)[0].atomValue() == S("TEST"));
        test::pdRunMainLoopMs(5);
        REQUIRE(t.messagesAt(0).size() == 2);
        REQUIRE(t.messagesAt(0)[1].anyValue() == LA("@delay", 400));
        t.clearAll();
    }

    SECTION("inlet")
    {
        TExt t("flow.prepend");

        t.sendFloatTo(200, 1);
        REQUIRE_PROPERTY(t, @value, 200);
        t.sendFloatTo(20, 2);
        REQUIRE_PROPERTY(t, @delay, 20);
    }

    SECTION("flow.before")
    {
        TExt t0("flow.before");
    }
}
