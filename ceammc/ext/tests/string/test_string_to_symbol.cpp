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
#include "ceammc_pd.h"
#include "string_to_symbol.h"
#include "test_external.h"

using namespace ceammc;

PD_COMPLETE_TEST_SETUP(StringToSymbol, string, to_symbol)

TEST_CASE("string->symbol", "[external]")
{
    pd_test_init();

    SECTION("create")
    {
        TestStringToSymbol t("str->sym");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, -121);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_NO_MSG(t);

        WHEN_SEND_ANY_TO(t, "a", LA("b", "c"));
        REQUIRE_NO_MSG(t);
    }

    SECTION("data")
    {
        TestStringToSymbol t("str->sym");

        WHEN_SEND_TDATA_TO(0, t, DataTypeString("ABC"));
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "ABC");

        WHEN_SEND_TDATA_TO(0, t, DataTypeString("A B  C"));
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "A B  C");

        WHEN_SEND_DATA_TO(0, t, IntData(100));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_DATA_TO(0, t, IntData(-10));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("external")
    {
        TestExtStringToSymbol t("string2symbol");
    }

    SECTION("alias")
    {
        TestExtStringToSymbol t("str->sym");
    }
}
