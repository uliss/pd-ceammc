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
#include "string_replace.h"
#include "test_external.h"
#include "test_string_base.h"

PD_COMPLETE_TEST_SETUP(StringReplace, string, replace)

#define REQUIRE_STRING(obj, cstr)                                        \
    {                                                                    \
        REQUIRE(obj.hasOutputAt(0));                                     \
        REQUIRE(t.outputAtomAt(0).isA<DataTypeString>());                \
        REQUIRE(t.outputAtomAt(0).asD<DataTypeString>()->str() == cstr); \
        t.clearAll();                                                    \
    }

TEST_CASE("string.replace", "[external]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            TestExtStringReplace t("string.replace");
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @mode, "all");
            REQUIRE_PROPERTY(t, @all, 1);
            REQUIRE_PROPERTY(t, @first, 0.f);
            REQUIRE_PROPERTY(t, @last, 0.f);
            REQUIRE_PROPERTY(t, @from, "");
            REQUIRE_PROPERTY(t, @to, "");

            t.sendSymbol(SYM("abc"));
            REQUIRE_STRING(t, "abc");

            t.sendList(StringAtom("def"));
            REQUIRE_STRING(t, "def");

            t.sendFloat(123);
            REQUIRE(!t.hasOutput());

            t.sendList(IntA(1000));
            REQUIRE(!t.hasOutput());

            t.sendBang();
            REQUIRE(!t.hasOutput());
        }

        SECTION("args")
        {
            TestExtStringReplace t("string.replace", LA("X"));
            REQUIRE_PROPERTY(t, @from, "X");
            REQUIRE_PROPERTY(t, @to, "");

            t.sendSymbol(SYM("XXL"));
            REQUIRE_STRING(t, "L");

            t.sendList(StringAtom("XYZ"));
            REQUIRE_STRING(t, "YZ");
        }

        SECTION("args")
        {
            TestExtStringReplace t("string.replace", LA("X", "Y"));
            REQUIRE_PROPERTY(t, @from, "X");
            REQUIRE_PROPERTY(t, @to, "Y");

            t.sendSymbol(SYM("XXL"));
            REQUIRE_STRING(t, "YYL");
        }
    }

    SECTION("space")
    {
        TestExtStringReplace t("string.replace", LA("\" \"", "Y"));
        REQUIRE_PROPERTY(t, @from, " ");
        REQUIRE_PROPERTY(t, @to, "Y");
    }

    SECTION("space")
    {
        TestExtStringReplace t("string.replace", LA("\"A\"", "\"\""));
        REQUIRE_PROPERTY(t, @from, "A");
        REQUIRE_PROPERTY(t, @to, "");
    }
}
