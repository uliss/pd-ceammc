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
#include "symbol_equal.h"
#include "test_string_base.h"

PD_COMPLETE_TEST_SETUP(SymbolEqual, symbol, equal)

TEST_CASE("symbol.equal", "[externals]")
{
    pd_test_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            TObj t("symbol.equal", L());
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.pattern() == 0);

            WHEN_SEND_SYMBOL_TO(0, t, "123");
        }

        SECTION("positional args")
        {
            TObj t("symbol.equal", A("abc"));
            REQUIRE(t.pattern() == gensym("abc"));

            WHEN_SEND_SYMBOL_TO(0, t, "a");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

            WHEN_SEND_SYMBOL_TO(0, t, "ab");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

            WHEN_SEND_SYMBOL_TO(0, t, "abc");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_SYMBOL_TO(0, t, "abcd");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        }
    }

    SECTION("list propagation")
    {
        TObj t("symbol.equal", L());

        WHEN_SEND_LIST_TO(0, t, LA("a", "b"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, LA("b", "b"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, LA("b", "b", "c", "d"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, LA(1, "b"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_SYMBOL_TO(0, t, "b");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, LA("a", 1));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, LF(1, 1));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, LA("b", "b"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
    }
}
