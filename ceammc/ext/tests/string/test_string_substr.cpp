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
#include "datatype_string.h"
#include "string_substr.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(StringSubstr, string, substr)

using namespace ceammc;

#define REQUIRE_STRING(obj, cstr)                                        \
    {                                                                    \
        REQUIRE(obj.hasOutputAt(0));                                     \
        REQUIRE(t.outputAtomAt(0).isA<DataTypeString>());                \
        REQUIRE(t.outputAtomAt(0).asD<DataTypeString>()->str() == cstr); \
        t.clearAll();                                                    \
    }

TEST_CASE("string.substr", "[external]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("range")
        {
            TestExtStringSubstr t("string.substr", LF(1, 4));
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @from, 1);
            REQUIRE_PROPERTY(t, @len, 4);

            t.sendSymbol("");
            REQUIRE_STRING(t, "");

            t.sendSymbol("A");
            REQUIRE_STRING(t, "");

            t.sendSymbol("AB");
            REQUIRE_STRING(t, "B");

            t.sendSymbol("ABC");
            REQUIRE_STRING(t, "BC");

            t.sendSymbol("ABCDEF");
            REQUIRE_STRING(t, "BCDE");
        }

        SECTION("range")
        {
            TestExtStringSubstr t("string.substr", LF(-2));
            REQUIRE_PROPERTY(t, @from, -2);

            t.sendSymbol("");
            REQUIRE_STRING(t, "");

            t.sendSymbol("A");
            REQUIRE_STRING(t, "");

            t.sendSymbol("AB");
            REQUIRE_STRING(t, "AB");

            t.sendSymbol("ABC");
            REQUIRE_STRING(t, "BC");

            t.sendSymbol("ABCD");
            REQUIRE_STRING(t, "CD");
        }

        SECTION("utf-8")
        {
            TestExtStringSubstr t("str.substr", LF(1, 2));

            t.sendSymbol("");
            REQUIRE_STRING(t, "");

            t.sendSymbol("АБВГД");
            REQUIRE_STRING(t, "БВ");
        }
    }
}
