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
#include "string_str.h"
#include "test_external.h"

#include "catch.hpp"

PD_COMPLETE_TEST_SETUP(StringStr, string, str)

using namespace ceammc;

#define REQUIRE_STRING(obj, cstr)                                        \
    {                                                                    \
        REQUIRE(obj.hasOutputAt(0));                                     \
        REQUIRE(t.outputAtomAt(0).isA<DataTypeString>());                \
        REQUIRE(t.outputAtomAt(0).asD<DataTypeString>()->str() == cstr); \
        t.clearAll();                                                    \
    }

#define NO_DATA(t) REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

TEST_CASE("string.str", "[external]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("create")
    {
        SECTION("empty")
        {
            TestExtStringStr t("string.str");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_STRING(t, "");

            t->dump();
        }

        SECTION("float arg")
        {
            TestExtStringStr t("string.str", LF(111));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_STRING(t, "111");
        }

        SECTION("list args")
        {
            TestExtStringStr t("string.str", LA(1, 2, "?"));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_STRING(t, "1 2 ?");
        }
    }

    SECTION("onFloat")
    {
        TestExtStringStr t("string.str");
        WHEN_SEND_FLOAT_TO(0, t, -200);
        REQUIRE_STRING(t, "-200");

        WHEN_SEND_FLOAT_TO(0, t, 100.1f);
        REQUIRE_STRING(t, "100.1");
    }

    SECTION("onSymbol")
    {
        TestExtStringStr t("string.str");
        WHEN_SEND_SYMBOL_TO(0, t, "ABC");
        REQUIRE_STRING(t, "ABC");

        WHEN_SEND_SYMBOL_TO(0, t, "XYZ");
        REQUIRE_STRING(t, "XYZ");
    }

    SECTION("onData")
    {
        TestExtStringStr t("string.str");

        WHEN_SEND_DATA_TO(0, t, DataTypeString("test string"));
        REQUIRE_STRING(t, "test string");
    }

    SECTION("onList")
    {
        TestExtStringStr t("string.str");

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_STRING(t, "1 2 3");

        WHEN_SEND_LIST_TO(0, t, LA(1, StringAtom("a b c"), 3));
        REQUIRE_STRING(t, "1 a b c 3");

        WHEN_SEND_LIST_TO(0, t, LA(1, new IntData(100), 3));
        REQUIRE_STRING(t, "1 100 3");
    }

    SECTION("clear")
    {
        TestExtStringStr t("string.str", LA("a", "b"));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "a b");

        WHEN_CALL(t, clear);
        NO_DATA(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "");

        WHEN_SEND_SYMBOL_TO(0, t, "ABC");
        REQUIRE_STRING(t, "ABC");
    }

    SECTION("set")
    {
        TestExtStringStr t("string.str", LA("a", "b"));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "a b");

        WHEN_CALL_N(t, set, "a");
        NO_DATA(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "a");

        WHEN_CALL_N(t, set, "a", "b");
        NO_DATA(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "a b");

        WHEN_CALL_N(t, set, "a", "b", Atom(DataDesc(100, 200)));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "a b ???");

        DataPtr p(new DataTypeString("STRING"));

        WHEN_CALL_N(t, set, "a", "b", p.asAtom());
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "a b STRING");

        WHEN_CALL_N(t, set, "a", "b", p.asAtom(), p.asAtom());
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "a b STRING STRING");
    }

    SECTION("append")
    {
        TestExtStringStr t("string.str");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "");

        WHEN_CALL(t, append);
        NO_DATA(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "");

        WHEN_CALL_N(t, append, 1000);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "1000");

        WHEN_CALL_N(t, append, 2000);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "10002000");

        WHEN_CALL_N(t, append, "a", "b");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "10002000a b");

        WHEN_CALL_N(t, append, Atom(DataDesc(100, 200)));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "10002000a b???");

        DataPtr p(new DataTypeString("STRING"));
        WHEN_CALL_N(t, append, p.asAtom());
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING(t, "10002000a b???STRING");
    }
}
