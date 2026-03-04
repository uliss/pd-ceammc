/*****************************************************************************
 * Copyright 2025 Serge Poltavsky. All rights reserved.
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
#include "ceammc_property_enum.h"
#include "test_property.h"

TEST_CASE("SymbolEnumProperty", "[core]")
{
    test::pdPrintToStdError();

    SECTION("SymbolEnumProperty")
    {
        SymbolEnumProperty p("@s", { "a", "b", "c" });

        SECTION("init")
        {
            REQUIRE(p.name() == SYM("@s"));
            REQUIRE(p.value() == SYM("a"));
            REQUIRE(p.index() == 0);
            REQUIRE(p.str() == std::string("a"));
            REQUIRE(p.defaultValue() == SYM("a"));

            REQUIRE(p.setList(LA("b")));
            REQUIRE(p.value() == SYM("b"));
            REQUIRE(p.index() == 1);

            REQUIRE(p.setSymbol(SYM("c")));
            REQUIRE(p.value() == SYM("c"));
            REQUIRE(p.index() == 2);
        }
    }

    SECTION("SymbolFloatEnumProperty")
    {
        SymbolFloatEnumProperty p("@s", { { "a", 1 }, { "b", 2 }, { "c", 3 } });

        SECTION("init")
        {
            REQUIRE(p.name() == SYM("@s"));
            REQUIRE(p.value() == SYM("a"));
            REQUIRE(p.index() == 0);
            REQUIRE(p.valuePair() == 1);
            REQUIRE(p.str() == std::string("a"));
            REQUIRE(p.defaultValue() == SYM("a"));

            REQUIRE(p.setList(LA("b")));
            REQUIRE(p.value() == SYM("b"));
            REQUIRE(p.index() == 1);
            REQUIRE(p.valuePair() == 2);

            REQUIRE(p.setSymbol(SYM("c")));
            REQUIRE(p.value() == SYM("c"));
            REQUIRE(p.index() == 2);
            REQUIRE(p.valuePair() == 3);
        }
    }
}
