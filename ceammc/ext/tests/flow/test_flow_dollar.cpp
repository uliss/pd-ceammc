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
#include "flow_dollar.h"
#include "test_flow_base.h"

extern "C" {
#include "g_all_guis.h"
}

PD_COMPLETE_TEST_SETUP(FlowDollar, flow, dollar)

TEST_CASE("flow.dollar", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("flow.dollar");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("alias")
        {
            TExt t("flow.$$");
        }
    }

    SECTION("process")
    {
        TExt t("flow.$$");

        t.sendBang();
        REQUIRE(t.isOutputBangAt(0));

        t << 100;
        REQUIRE(t.outputFloatAt(0) == 100);

        t << "ABC";
        REQUIRE(t.outputSymbolAt(0) == SYM("ABC"));

        auto d = canvas_getdollarzero();
        std::string res = "a";
        res += std::to_string(d);

        t << "a$0";
        REQUIRE(t.outputSymbolAt(0)->s_name == res);

        t << "a$1";
        REQUIRE(t.outputSymbolAt(0) == SYM("a$1"));

        t << LF(1, 2, 3);
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3));

        t << LA(1, "B", "C$0");
        auto cdollar = std::string("C") + std::to_string(d);
        REQUIRE(t.outputListAt(0) == LA(1, "B", SYM(cdollar.c_str())));

        t.call("method", LA("C$0", 2));
        REQUIRE(t.outputAnyAt(0) == LA("method", SYM(cdollar.c_str()), 2));

        t.call("C$0", 2);
        REQUIRE(t.outputAnyAt(0) == LA(SYM(cdollar.c_str()), 2));
    }
}
