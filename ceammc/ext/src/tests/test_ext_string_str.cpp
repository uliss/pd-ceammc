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
#include "../string/string_str.h"
#include "base_extension_test.h"
#include "ceammc_pd.h"

#include "catch.hpp"

using namespace ceammc;

typedef TestExtension<StringStr> StringStrTest;

#define REQUIRE_STRING_OUTPUT(t, str_)                            \
    {                                                             \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);                         \
        DataTypeString* s = t.typedLastDataAt<DataTypeString>(0); \
        REQUIRE(s != 0);                                          \
        REQUIRE(s->str() == str_);                                \
    }

#define NO_DATA(t) REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("string.str", "[external]")
{
    string_setup();

    SECTION("create")
    {
        SECTION("empty")
        {
            StringStrTest t("string.str");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_STRING_OUTPUT(t, "");

            t.dump();
        }

        SECTION("float arg")
        {
            StringStrTest t("string.str", L1(111));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_STRING_OUTPUT(t, "111");
        }

        SECTION("list args")
        {
            StringStrTest t("string.str", L3(1, 2, "?"));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_STRING_OUTPUT(t, "1 2 ?");
        }
    }

    SECTION("onFloat")
    {
        StringStrTest t("string.str");
        WHEN_SEND_FLOAT_TO(0, t, -200);
        REQUIRE_STRING_OUTPUT(t, "-200");

        WHEN_SEND_FLOAT_TO(0, t, 100.1f);
        REQUIRE_STRING_OUTPUT(t, "100.1");
    }

    SECTION("onSymbol")
    {
        StringStrTest t("string.str");
        WHEN_SEND_SYMBOL_TO(0, t, "ABC");
        REQUIRE_STRING_OUTPUT(t, "ABC");

        WHEN_SEND_SYMBOL_TO(0, t, "XYZ");
        REQUIRE_STRING_OUTPUT(t, "XYZ");
    }

    SECTION("onData")
    {
        StringStrTest t("string.str");

        WHEN_SEND_DATA_TO(0, t, DataTypeString("test string"));
        REQUIRE_STRING_OUTPUT(t, "test string");
    }

    SECTION("onList")
    {
        StringStrTest t("string.str");

        WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
        REQUIRE_STRING_OUTPUT(t, "1 2 3");

        Data p(new DataTypeString("a b c"));
        Atom a = p.toAtom();

        WHEN_SEND_LIST_TO(0, t, L3(1, a, 3));
        REQUIRE_STRING_OUTPUT(t, "1 a b c 3");

        WHEN_SEND_LIST_TO(0, t, L3(1, Atom(DataDesc(100, 200)), 3));
        REQUIRE_STRING_OUTPUT(t, "1 ??? 3");
    }

    SECTION("clear")
    {
        StringStrTest t("string.str", L2("a", "b"));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "a b");

        WHEN_CALL(t, clear);
        NO_DATA(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "");

        WHEN_SEND_SYMBOL_TO(0, t, "ABC");
        REQUIRE_STRING_OUTPUT(t, "ABC");
    }

    SECTION("set")
    {
        StringStrTest t("string.str", L2("a", "b"));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "a b");

        WHEN_CALL_1(t, set, "a");
        NO_DATA(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "a");

        WHEN_CALL_2(t, set, "a", "b");
        NO_DATA(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "a b");

        WHEN_CALL_3(t, set, "a", "b", Atom(DataDesc(100, 200)));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "a b ???");

        Data p(new DataTypeString("STRING"));

        WHEN_CALL_3(t, set, "a", "b", p.toAtom());
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "a b STRING");

        WHEN_CALL_4(t, set, "a", "b", p.toAtom(), p.toAtom());
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "a b STRING STRING");
    }

    SECTION("append")
    {
        StringStrTest t("string.str");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "");

        WHEN_CALL(t, append);
        NO_DATA(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "");

        WHEN_CALL_1(t, append, 1000);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "1000");

        WHEN_CALL_1(t, append, 2000);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "10002000");

        WHEN_CALL_2(t, append, "a", "b");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "10002000a b");

        WHEN_CALL_1(t, append, Atom(DataDesc(100, 200)));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "10002000a b???");

        Data p(new DataTypeString("STRING"));
        WHEN_CALL_1(t, append, p.toAtom());
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_STRING_OUTPUT(t, "10002000a b???STRING");
    }
}
