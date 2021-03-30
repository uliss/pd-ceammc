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
#include "ceammc_data.h"
#include "datatype_string.h"
#include "symbol_to_any.h"
#include "test_string_base.h"

PD_COMPLETE_TEST_SETUP(SymbolToAny, symbol, to_any)

TEST_CASE("symbol.to_any", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("symbol2any", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("ext")
        {
            TExt t("symbol2any", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("alias")
        {
            TExt t("sym->any", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("alias 2")
        {
            TExt t("str->any", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("alias 3")
        {
            TExt t("string2any", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }
    }

    SECTION("process")
    {
        TExt t("string2any");

        t << "bang";
        REQUIRE(t.isOutputBangAt(0));

        t << "-123.5";
        REQUIRE(t.outputFloatAt(0) == -123.5);

        t << "float 123.5";
        REQUIRE(t.outputFloatAt(0) == 123.5);

        t << "symbol ABC";
        REQUIRE(t.outputSymbolAt(0) == SYM("ABC"));

        t << "symbol ABC DEF";
        REQUIRE(t.outputSymbolAt(0) == SYM("ABC"));

        t << "1 2 3";
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3));

        t << "list 1 2";
        REQUIRE(t.outputListAt(0) == LF(1, 2));

        t << "list A B C";
        REQUIRE(t.outputListAt(0) == LA("A", "B", "C"));

        t << "A B C";
        REQUIRE(t.outputAnyAt(0) == LA("A", "B", "C"));

        t << StringAtom("@prop");
        REQUIRE(t.outputAnyAt(0) == LA("@prop"));
    }
}
