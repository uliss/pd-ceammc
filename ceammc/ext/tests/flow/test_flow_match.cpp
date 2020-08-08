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
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "flow_match.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowMatch, flow, match)

TEST_CASE("flow.match", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            TObj t("flow.match");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("args")
        {
            TObj t("flow.route", LA("test"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
        }

        SECTION("args")
        {
            TObj t("flow.route", LA("a", "b", "c", "d"));
            REQUIRE(t.numInlets() == 5);
            REQUIRE(t.numOutlets() == 5);
        }

        SECTION("props")
        {
            TObj t("flow.route", LA("\"@a\""));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
        }
    }

    SECTION("run")
    {
        TExt t("flow.match", "p+");

        t << BANG;
        REQUIRE(!t.hasOutput());
        t << 100;
        REQUIRE(!t.hasOutput());

        // symbol
        t << "abc";
        REQUIRE(t.outputSymbolAt(1) == S("abc"));

        t << "pp";
        REQUIRE(t.outputSymbolAt(0) == S("pp"));
        t << "pppp";
        REQUIRE(t.outputSymbolAt(0) == S("pppp"));

        // any
        t.sendMessage("ppppp", LF(1, 2, 3));
        REQUIRE(t.outputAnyAt(0) == LA("ppppp", 1, 2, 3));

        // properties
        TExt t1("flow.match", "\"@a.+\"");
        t1 << "@abc";
        REQUIRE(t1.outputSymbolAt(0) == S("@abc"));
        t1.sendMessage("@abc", LF(1, 2, 3));
        REQUIRE(t1.outputAnyAt(0) == LA("@abc", 1, 2, 3));
        t1.sendMessage("@abcde", LF(1, 2, 3));
        REQUIRE(t1.outputAnyAt(0) == LA("@abcde", 1, 2, 3));
    }
}
