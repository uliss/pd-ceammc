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
#include "flow_space.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowSpace, flow, space)

TEST_CASE("flow.space", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    SECTION("init")
    {
        SECTION("empty")
        {
            TObj t("flow.space");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);

            REQUIRE_PROPERTY(t, @delay, 1);
            REQUIRE_PROPERTY(t, @done, 1);
        }

        SECTION("args")
        {
            TObj t("flow.space", LF(10, 4));
            REQUIRE_PROPERTY(t, @delay, 10);
            REQUIRE_PROPERTY(t, @done, 4);
        }

        SECTION("props")
        {
            TObj t("flow.space", LA("@delay", 11, "@done", 2));
            REQUIRE_PROPERTY(t, @delay, 11);
            REQUIRE_PROPERTY(t, @done, 2);
        }
    }

    SECTION("single")
    {
        TExt t("flow.space");

        // BANG
        t.sendBang();
        // no output
        REQUIRE_FALSE(t.hasOutput());

        t.schedTicks(1);
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputBangAt(0));

        // FLOAT
        t << 100.5;
        // no output
        REQUIRE_FALSE(t.hasOutput());

        t.schedTicks(1);
        REQUIRE(t.outputFloatAt(0) == 100.5);

        // SYMBOL
        t << "test";
        // no output
        REQUIRE_FALSE(t.hasOutput());

        t.schedTicks(1);
        REQUIRE(t.outputSymbolAt(0) == SYM("test"));

        // list
        t << LF(1, 2, 3);
        // no output
        REQUIRE_FALSE(t.hasOutput());

        t.schedTicks(1);
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3));

        // any
        t.clearAll();
        t.sendMessage("@delay", LF(1, 2, 3));
        // no output
        REQUIRE_FALSE(t.hasOutput());

        t.schedTicks(1);
        REQUIRE(t.outputAnyAt(0) == LA("@delay", 1, 2, 3));

        t.sendFloatTo(5, 1);
        REQUIRE_PROPERTY(t, @delay, 5);
        t.sendFloatTo(-1, 1);
        REQUIRE_PROPERTY(t, @delay, 5);

        t.sendMessageTo(Message(SYM("@delay"), LF(2)), 1);
        REQUIRE_PROPERTY(t, @delay, 2);

        t.sendMessageTo(Message(SYM("@done"), LF(10)), 1);
        REQUIRE_PROPERTY(t, @done, 10);
    }

    SECTION("space")
    {
        TExt t("flow.space", 2);

        t << 1;
        t << 2;
        t << 3;
        // no output
        REQUIRE_FALSE(t.hasOutput());

        t.schedTicks(1);
        REQUIRE(t.outputFloatAt(0) == 1);

        t.schedTicks(2);
        REQUIRE(t.outputFloatAt(0) == 2);

        t.schedTicks(2);
        REQUIRE(t.outputFloatAt(0) == 3);
        REQUIRE_FALSE(t.hasOutputAt(1));

        t.schedTicks(2);
        REQUIRE(t.isOutputBangAt(1));
    }

    SECTION("space")
    {
        TExt t("flow.space", 2, 3);

        t << 1;
        t << 2;
        // no output
        REQUIRE_FALSE(t.hasOutput());

        t.schedTicks(1);
        REQUIRE(t.outputFloatAt(0) == 1);

        t.schedTicks(2);
        REQUIRE(t.outputFloatAt(0) == 2);
        REQUIRE_FALSE(t.hasOutputAt(1));

        t.schedTicks(2);
        REQUIRE_FALSE(t.hasOutputAt(1));
        t.schedTicks(2);
        REQUIRE_FALSE(t.hasOutputAt(1));
        t.schedTicks(2);
        REQUIRE(t.isOutputBangAt(1));
    }

    SECTION("reset")
    {
        TExt t("flow.space", 2);

        t << 1;
        REQUIRE_FALSE(t.hasOutput());
        t.sendMessageTo(Message(SYM("reset"), L()), 1);

        t.schedTicks(10);
        REQUIRE_FALSE(t.hasOutput());

        t << 1;
        t << 2;
        REQUIRE_FALSE(t.hasOutput());

        t.schedTicks(1);
        REQUIRE(t.outputFloatAt(0) == 1);
        t.clearAt(0);
        t.sendMessageTo(Message(SYM("reset"), L()), 1);

        t.schedTicks(10);
        REQUIRE_FALSE(t.hasOutputAt(0));
        REQUIRE_FALSE(t.hasOutputAt(1));
    }
}
