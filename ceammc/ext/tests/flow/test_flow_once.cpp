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
#include "flow_once.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowOnce, flow, once)

TEST_CASE("flow.once", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TestFlowOnce t("flow.once");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
        }
    }

    SECTION("ext")
    {
        TestExtFlowOnce t("flow.once");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);

        TestExtFlowOnce t0("once");
        REQUIRE(t0.numInlets() == 2);
        REQUIRE(t0.numOutlets() == 1);
    }

    SECTION("run")
    {
        TestExtFlowOnce t("flow.once");

        t << BANG;
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == L());

        t << BANG;
        REQUIRE_FALSE(t.hasOutput());

        t.call("reset");
        t << BANG;
        REQUIRE(t.outputListAt(0) == L());

        t << 100;
        REQUIRE_FALSE(t.hasOutput());

        t.sendBangTo(1);
        t << 200.f;
        REQUIRE(t.outputFloatAt(0) == Approx(200));

        t << "text";
        REQUIRE_FALSE(t.hasOutput());

        t.sendBangTo(1);
        t << "reset";
        REQUIRE(t.outputSymbolAt(0) == gensym("reset"));

        t << LA("a", "b", 3);
        REQUIRE_FALSE(t.hasOutput());

        t.sendBangTo(1);
        t << LA("a", "b", 4);
        REQUIRE(t.outputListAt(0) == LA("a", "b", 4));

        t <<= LA("message", "ABC");
        REQUIRE_FALSE(t.hasOutput());

        t.sendBangTo(1);
        t <<= LA("message", "ABC");
        REQUIRE(t.outputAnyAt(0) == LA("message", "ABC"));

        t <<= LA("@prop", "ABC");
        REQUIRE_FALSE(t.hasOutput());

        t.sendBangTo(1);
        t <<= LA("@prop", "ABC");
        REQUIRE(t.outputAnyAt(0) == LA("@prop", "ABC"));

        t << IntA(-100);
        REQUIRE_FALSE(t.hasOutput());

        t.sendBangTo(1);
        t << IntA(200);
        REQUIRE(dataAt(t) == IntA(200));
    }

    SECTION("run opened")
    {
        TestExtFlowOnce t("flow.once", LF(1));
        t << BANG;
        REQUIRE(t.hasOutput());
    }

    SECTION("run closed")
    {
        TestExtFlowOnce t("flow.once", LF(0));
        t << BANG;
        REQUIRE_FALSE(t.hasOutput());
    }
}
