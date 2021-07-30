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
#include "msg_sched.h"
#include "test_base.h"
#include "test_catch2.hpp"

PD_COMPLETE_TEST_SETUP(MsgSched, msg, sched)

TEST_CASE("msg.sched", "[extension]")
{
    pd_test_init();
    setTestSampleRate(64000); // 1tick == 1ms
    test::pdPrintToStdError(true);

    SECTION("construct")
    {
        TExt t("msg.sched");
    }

    SECTION("do")
    {
        TExt t("msg.sched");

        t.bang();
        REQUIRE(t.isOutputBangAt(0));

        t << "ABC";
        REQUIRE(t.outputSymbolAt(0) == SYM("ABC"));

        t << 100.25;
        REQUIRE(t.outputFloatAt(0) == 100.25);
        t << 0.0;
        REQUIRE(t.outputFloatAt(0) == 0.0);
        t << -10.0;
        REQUIRE(t.outputFloatAt(0) == -10);

        t.sendMessage("any");
        REQUIRE(t.outputAnyAt(0) == LA("any"));

        t.sendMessage("any", LA(1, 2, 3, "X"));
        REQUIRE(t.outputAnyAt(0) == LA("any", 1, 2, 3, "X"));

        t << LA(-100, 1, 2, 3);
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3));

        t << LF(0, 1, 2, 3);
        REQUIRE_FALSE(t.hasOutputAt(0));
        t.schedTicks(1);
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3));

        t << LF(16, 1, 2, 3);
        REQUIRE_FALSE(t.hasOutputAt(0));
        t.schedTicks(16);
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3));

        t << LF(64, 64);
        t << LF(32, 32);
        t << LF(16, 16);

        REQUIRE_FALSE(t.hasOutputAt(0));
        t.schedTicks(16);
        REQUIRE(t.outputListAt(0) == LF(16));
        t.clearAll();
        t.schedTicks(16);
        REQUIRE(t.outputListAt(0) == LF(32));
        t.clearAll();
        t.schedTicks(16);
        REQUIRE_FALSE(t.hasOutputAt(0));
        t.schedTicks(16);
        REQUIRE(t.outputListAt(0) == LF(64));
    }
}
