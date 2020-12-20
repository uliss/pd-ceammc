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
#include "datatype_string.h"
#include "string_contains.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(StringContains, string, contains)

TEST_CASE("string.contains", "[external]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("empty")
        {
            TestExtStringContains t("string.contains");
            REQUIRE_PROPERTY(t, @subj, StringAtom(""));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendSymbol(SYM("test"));
            REQUIRE(t.outputFloatAt(0) == 1);
            t.sendSymbol(SYM("any"));
            REQUIRE(t.outputFloatAt(0) == 1);

            t.sendList(Atom(new DataTypeString("none")));
            REQUIRE(t.outputFloatAt(0) == 1);
            t.sendList(Atom(new DataTypeString(" abc")));
            REQUIRE(t.outputFloatAt(0) == 1);
        }

        SECTION("args")
        {
            TestExtStringContains t("string.contains", LA("abc"));
            REQUIRE_PROPERTY(t, @subj, StringAtom("abc"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendSymbol(SYM("test"));
            REQUIRE(t.outputFloatAt(0) == 0);
            t.sendSymbol(SYM("abc"));
            REQUIRE(t.outputFloatAt(0) == 1);
            t.sendSymbol(SYM("bcdef"));
            REQUIRE(t.outputFloatAt(0) == 0);
            t.sendSymbol(SYM("abcdef"));
            REQUIRE(t.outputFloatAt(0) == 1);
            t.sendSymbol(SYM("12313abcdef"));
            REQUIRE(t.outputFloatAt(0) == 1);

            t.sendList(Atom(new DataTypeString("none")));
            REQUIRE(t.outputFloatAt(0) == 0);
            t.sendList(Atom(new DataTypeString(" abc")));
            REQUIRE(t.outputFloatAt(0) == 1);
        }

        SECTION("quoted list")
        {
            TestExtStringContains t("string.contains", LA("\"", "\""));
            REQUIRE_PROPERTY(t, @subj, StringAtom(" "));
            t.sendSymbol(SYM("abc"));
            REQUIRE(t.outputFloatAt(0) == 0);
            t.sendSymbol(SYM(" "));
            REQUIRE(t.outputFloatAt(0) == 1);
        }

        SECTION("quoted atom")
        {
            TestExtStringContains t("string.contains", LA("\" \""));
            REQUIRE_PROPERTY(t, @subj, StringAtom(" "));
            t.sendSymbol(SYM("abc"));
            REQUIRE(t.outputFloatAt(0) == 0);
            t.sendSymbol(SYM(" "));
            REQUIRE(t.outputFloatAt(0) == 1);
        }

        SECTION("empty string")
        {
            TestExtStringContains t("string.contains", LA("\"\""));
            REQUIRE_PROPERTY(t, @subj, StringAtom(""));
            t.sendSymbol(SYM(""));
            REQUIRE(t.outputFloatAt(0) == 1);
            t.sendSymbol(SYM(" "));
            REQUIRE(t.outputFloatAt(0) == 1);
            t.sendSymbol(SYM("abc"));
            REQUIRE(t.outputFloatAt(0) == 1);
        }
    }

    SECTION("change pattern")
    {
        TestExtStringContains t("string.contains", LA("#"));

        t.sendSymbol(SYM("abc%"));
        REQUIRE(t.outputFloatAt(0) == 0);
        t.sendSymbol(SYM("abc#"));
        REQUIRE(t.outputFloatAt(0) == 1);

        t.sendListTo(LA("%"), 1);

        t.sendSymbol(SYM("abc%"));
        REQUIRE(t.outputFloatAt(0) == 1);
        t.sendSymbol(SYM("abc#"));
        REQUIRE(t.outputFloatAt(0) == 0);

        t.sendListTo(Atom(new DataTypeString("***")), 1);

        t.sendSymbol(SYM("abc%"));
        REQUIRE(t.outputFloatAt(0) == 0);
        t.sendSymbol(SYM("abc#"));
        REQUIRE(t.outputFloatAt(0) == 0);
        t.sendSymbol(SYM("abc***def"));
        REQUIRE(t.outputFloatAt(0) == 1);
    }
}
