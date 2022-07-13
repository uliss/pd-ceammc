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
#include "ceammc_format.h"
#include "ceammc_pd.h"
#include "string_str.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(StringStr, string, str)

using namespace ceammc;

#define REQUIRE_STRING(obj, cstr)                         \
    {                                                     \
        REQUIRE(obj.hasOutputAt(0));                      \
        REQUIRE(t.outputAtomAt(0).isA<DataTypeString>()); \
        REQUIRE(to_string(t.outputAtomAt(0)) == cstr);    \
        t.clearAll();                                     \
    }

#define NO_DATA(t) REQUIRE_FALSE(t.hasOutputAt(0));

TEST_CASE("string", "[external]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TExt t("string");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            t.bang();
            REQUIRE_STRING(t, "S\"\"");

            t->dump();
        }

        SECTION("float arg")
        {
            TExt t("str", LF(111));
            t.bang();
            REQUIRE_STRING(t, "S\"111\"");
        }

        SECTION("list args")
        {
            TExt t("string", LA("\"1", 2, "?\""));
            t.bang();
            REQUIRE_STRING(t, "S\"1\\ 2\\ ?\"");
        }

        SECTION("list args")
        {
            TExt t("string", LF(1, 2, 3));
            t.bang();
            REQUIRE_STRING(t, "S\"1\\ 2\\ 3\"");
        }
    }

    SECTION("onFloat")
    {
        TExt t("string");
        t.sendFloat(-200);
        REQUIRE_STRING(t, "S\"-200\"");

        t.sendFloat(10.5);
        REQUIRE_STRING(t, "S\"10.5\"");
    }

    SECTION("onSymbol")
    {
        TExt t("string");
        t.sendSymbol("ABC");
        REQUIRE_STRING(t, "S\"ABC\"");

        t.sendSymbol("XYZ");
        REQUIRE_STRING(t, "S\"XYZ\"");
    }

    SECTION("onData")
    {
        TExt t("string");

        t.sendList(new DataTypeString("test string"));
        REQUIRE_STRING(t, "S\"test\\ string\"");
    }

    SECTION("onList")
    {
        TExt t("string");

        t.sendList(1, 2, 3);
        REQUIRE_STRING(t, "S\"1\\ 2\\ 3\"");

        t.sendList(1, StringAtom("a b c"), 3);
        REQUIRE_STRING(t, "1 a b c 3");

        t.sendList(1, new IntData(100), 3);
        REQUIRE_STRING(t, "1 IntData(100) 3");
    }

    SECTION("clear")
    {
        TExt t("string", LA("\"a", "b\""));
        t.bang();
        REQUIRE_STRING(t, "S\"a\\ b\"");

        t.call("clear");
        NO_DATA(t);
        t.bang();
        REQUIRE_STRING(t, "");

        t.sendSymbol("ABC");
        REQUIRE_STRING(t, "S\"ABC\"");
    }

    SECTION("set")
    {
        TExt t("string", LA("a", "b"));
        t.bang();
        REQUIRE_STRING(t, "String(\"a b\")");

        t.call("set", LA("a"));
        NO_DATA(t);
        t.bang();
        REQUIRE_STRING(t, "String(\"a\")");

        t.call("set", "a", "b");
        NO_DATA(t);
        t.bang();
        REQUIRE_STRING(t, "String(\"a b\")");

        t.call("set", "a", "b", new StrData("STRING"));
        t.bang();
        REQUIRE_STRING(t, "String(\"a b StrData(STRING)\")");

        t.call("set", "a", "b", new DataTypeString("STRING"), new IntData(1000));
        t.bang();
        REQUIRE_STRING(t, "String(\"a b STRING IntData(1000)\")");
    }

    SECTION("append")
    {
        TExt t("string");

        t.bang();
        REQUIRE_STRING(t, "S\"\"");

        t.call("append");
        NO_DATA(t);
        t.bang();
        REQUIRE_STRING(t, "S\"\"");

        t.call("append", 1000);
        t.bang();
        REQUIRE_STRING(t, "S\"1000\"");

        t.call("append", 2000);
        t.bang();
        REQUIRE_STRING(t, "S\"10002000\"");

        t.call("append", "a", "b");
        t.bang();
        REQUIRE_STRING(t, "S\"10002000a\\ b\"");

        t.call("append", new DataTypeString("STRING"));
        t.bang();
        REQUIRE_STRING(t, "S\"10002000a\\ bSTRING\"");
    }
}
