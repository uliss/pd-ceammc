/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "catch.hpp"
#include "string_join.h"
#include "test_string_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(StringJoin, string, join)

#define REQUIRE_STRING(obj, cstr)                                        \
    {                                                                    \
        REQUIRE(obj.hasOutputAt(0));                                     \
        REQUIRE(t.outputAtomAt(0).isA<DataTypeString>());                \
        REQUIRE(t.outputAtomAt(0).asD<DataTypeString>()->str() == cstr); \
        t.clearAll();                                                    \
    }

TEST_CASE("string.join", "[external]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("empty")
        {
            TestExtStringJoin t("string.join");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @sep, "");

            // single symbol
            t.sendSymbol(SYM("test"));
            REQUIRE_STRING(t, "test");

            // invalid float
            t.sendFloat(0);
            REQUIRE(!t.hasOutput());

            // output previous
            t.sendBang();
            REQUIRE_STRING(t, "test");

            // list
            t.sendList(LF(1, 2, 3));
            REQUIRE_STRING(t, "123");
            t.sendList(LA(100, "100"));
            REQUIRE_STRING(t, "100100");

            // data
            t.sendList(LA(100, new IntData(2000)));
            REQUIRE_STRING(t, "1002000");

            t.sendList(LA(new IntData(1000), new IntData(2000)));
            REQUIRE_STRING(t, "10002000");
        }

        SECTION("empty string")
        {
            TestExtStringJoin t("string.join", LA("\"\""));
            REQUIRE_PROPERTY_LIST(t, @sep, LA(&s_));
            t.sendList(LF(1, 2, 3));
            REQUIRE_STRING(t, "123");
        }

        SECTION("space")
        {
            TestExtStringJoin t("string.join", LA("\"", "\""));
            REQUIRE_PROPERTY_LIST(t, @sep, LA(" "));
            t.sendList(LF(1, 2, 3));
            REQUIRE_STRING(t, "1 2 3");
        }

        SECTION("space")
        {
            TestExtStringJoin t("string.join", LA("\" \""));
            REQUIRE_PROPERTY_LIST(t, @sep, LA(" "));
            t.sendList(LF(1, 2, 3));
            REQUIRE_STRING(t, "1 2 3");
        }

        SECTION(",")
        {
            TestExtStringJoin t("string.join", LA("\"`.\""));
            REQUIRE_PROPERTY_LIST(t, @sep, LA(","));
            t.sendList(LF(1, 2, 3));
            REQUIRE_STRING(t, "1,2,3");
        }

        SECTION(",")
        {
            TestExtStringJoin t("string.join", LA("\"`. \""));
            REQUIRE_PROPERTY_LIST(t, @sep, LA(", "));
            t.sendList(LF(1, 2, 3));
            REQUIRE_STRING(t, "1, 2, 3");
        }

        SECTION(";")
        {
            TestExtStringJoin t("string.join", LA("\"`:\""));
            REQUIRE_PROPERTY_LIST(t, @sep, LA(";"));
            t.sendList(LF(1, 2, 3));
            REQUIRE_STRING(t, "1;2;3");
        }

        SECTION("@sep")
        {
            TestExtStringJoin t("string.join", LA("@sep", "."));
            REQUIRE_PROPERTY_LIST(t, @sep, LA("."));
            t.sendList(LF(1, 2, 3));
            REQUIRE_STRING(t, "1.2.3");
        }
    }

    SECTION("@sep")
    {
        TestExtStringJoin t("string.join");

        REQUIRE_PROPERTY_LIST(t, @sep, LA(""));
        t.sendList(LF(1, 2, 3));
        REQUIRE_STRING(t, "123");

        t->setProperty("@sep", LA("|"));
        REQUIRE_PROPERTY(t, @sep, "|");

        t.sendList(LF(1, 2, 3));
        REQUIRE_STRING(t, "1|2|3");

        t->setProperty("@sep", LA("\"", "\""));
        REQUIRE_PROPERTY(t, @sep, "|");

        t.sendListTo(LA("'`'", "`''"), 1);
        REQUIRE_PROPERTY(t, @sep, "|");
    }

    SECTION("mlist")
    {
        TestExtStringJoin t("string.join", "\":\"");

        t.sendList(MLA(1, 2, 3));
        REQUIRE_STRING(t, "1:2:3");

        t.sendList(MLA("a", "b", "c"));
        REQUIRE_STRING(t, "a:b:c");
    }
}
