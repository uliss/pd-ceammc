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
#include "symbol_to_int.h"
#include "test_string_base.h"

PD_COMPLETE_TEST_SETUP(SymbolToInt, symbol, to_int)

TEST_CASE("symbol.to_int", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("symbol2intlist", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("ext")
        {
            TExt t("symbol2intlist", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("alias")
        {
            TExt t("sym->ilist", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }
    }

    SECTION("onSymbol")
    {
        TObj t("symbol2intlist");

        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 'A')
        WHEN_SEND_SYMBOL_TO(0, t, "AB");
        REQUIRE_LIST_AT_OUTLET(0, t, LF('A', 'B'))
        WHEN_SEND_SYMBOL_TO(0, t, "Я");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0xd0af)
        WHEN_SEND_SYMBOL_TO(0, t, "∑");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0xe28891)
        WHEN_SEND_SYMBOL_TO(0, t, "🍩");
        REQUIRE_FLOAT_AT_OUTLET(0, t, t_float(0xf09f8da9))
    }
}
