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

PD_COMPLETE_TEST_SETUP(StringStr, string, str)

using namespace ceammc;

#define REQUIRE_STRING(obj, cstr)                                        \
    {                                                                    \
        REQUIRE(obj.hasOutputAt(0));                                     \
        REQUIRE(t.outputAtomAt(0).isA<DataTypeString>());                \
        REQUIRE(t.outputAtomAt(0).asD<DataTypeString>()->str() == cstr); \
        t.clearAll();                                                    \
    }

#define NO_DATA(t) REQUIRE_FALSE(t.hasOutputAt(0));

TEST_CASE("string", "[external]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TestExtStringStr t("string");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            t.bang();
            REQUIRE_STRING(t, "");

            t->dump();
        }

        SECTION("float arg")
        {
            TestExtStringStr t("str", LF(111));
            t.bang();
            REQUIRE_STRING(t, "111");
        }

        SECTION("list args")
        {
            TestExtStringStr t("string", LA("\"1", 2, "?\""));
            t.bang();
            REQUIRE_STRING(t, "1 2 ?");
        }

        SECTION("list args")
        {
            TestExtStringStr t("string", LF(1, 2, 3));
            t.bang();
            REQUIRE_STRING(t, "1 2 3");
        }
    }

    SECTION("onFloat")
    {
        TestExtStringStr t("string");
        t.sendFloat(-200);
        REQUIRE_STRING(t, "-200");

        t.sendFloat(10.5);
        REQUIRE_STRING(t, "10.5");
    }

    SECTION("onSymbol")
    {
        TestExtStringStr t("string");
        t.sendSymbol("ABC");
        REQUIRE_STRING(t, "ABC");

        t.sendSymbol("XYZ");
        REQUIRE_STRING(t, "XYZ");
    }

    SECTION("onData")
    {
        TestExtStringStr t("string");

        t.sendList(new DataTypeString("test string"));
        REQUIRE_STRING(t, "test string");
    }

    SECTION("onList")
    {
        TestExtStringStr t("string");

        t.sendList(1, 2, 3);
        REQUIRE_STRING(t, "1 2 3");

        t.sendList(1, StringAtom("a b c"), 3);
        REQUIRE_STRING(t, "1 a b c 3");

        t.sendList(1, new IntData(100), 3);
        REQUIRE_STRING(t, "1 100 3");
    }

    SECTION("clear")
    {
        TestExtStringStr t("string", LA("\"a", "b\""));
        t.bang();
        REQUIRE_STRING(t, "a b");

        t.call("clear");
        NO_DATA(t);
        t.bang();
        REQUIRE_STRING(t, "");

        t.sendSymbol("ABC");
        REQUIRE_STRING(t, "ABC");
    }

    SECTION("set")
    {
        TestExtStringStr t("string", LA("a", "b"));
        t.bang();
        REQUIRE_STRING(t, "a b");

        t.call("set", LA("a"));
        NO_DATA(t);
        t.bang();
        REQUIRE_STRING(t, "a");

        t.call("set", "a", "b");
        NO_DATA(t);
        t.bang();
        REQUIRE_STRING(t, "a b");

        t.call("set", "a", "b", new StrData("STRING"));
        t.bang();
        REQUIRE_STRING(t, "a b STRING");

        t.call("set", "a", "b", new DataTypeString("STRING"), new IntData(1000));
        t.bang();
        REQUIRE_STRING(t, "a b STRING 1000");
    }

    SECTION("append")
    {
        TestExtStringStr t("string");

        t.bang();
        REQUIRE_STRING(t, "");

        t.call("append");
        NO_DATA(t);
        t.bang();
        REQUIRE_STRING(t, "");

        t.call("append", 1000);
        t.bang();
        REQUIRE_STRING(t, "1000");

        t.call("append", 2000);
        t.bang();
        REQUIRE_STRING(t, "10002000");

        t.call("append", "a", "b");
        t.bang();
        REQUIRE_STRING(t, "10002000a b");

        t.call("append", new DataTypeString("STRING"));
        t.bang();
        REQUIRE_STRING(t, "10002000a bSTRING");
    }
}
