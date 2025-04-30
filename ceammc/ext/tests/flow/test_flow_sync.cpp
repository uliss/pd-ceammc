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
#include "flow_sync.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowSync, flow, sync)

TEST_CASE("flow.sync", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("flow.sync");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @n, 2);
        }

        SECTION("args")
        {
            TObj t("flow.append", LF(3));
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 3);
            REQUIRE_PROPERTY(t, @n, 3);
        }

        SECTION("props")
        {
            TObj t("flow.append", LA("@n", 4));
            REQUIRE(t.numInlets() == 4);
            REQUIRE(t.numOutlets() == 4);
            REQUIRE_PROPERTY(t, @n, 4);
        }
    }

    SECTION("process")
    {
        TObj t("flow.sync", LF(3));
        t << 1;
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        REQUIRE_BANG_AT_OUTLET(1, t);
        REQUIRE_BANG_AT_OUTLET(2, t);

        t.sendSymbol(SYM("ABC"), 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "ABC");
        REQUIRE_BANG_AT_OUTLET(2, t);

        t.sendFloat(200, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "ABC");
        REQUIRE_FLOAT_AT_OUTLET(2, t, 200);

        t.sendList(LF(1, 2, 3), 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "ABC");
        REQUIRE_LIST_AT_OUTLET(2, t, LF(1, 2, 3));
    }
}
