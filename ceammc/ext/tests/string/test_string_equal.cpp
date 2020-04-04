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
#include "string_equal.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(StringEqual, string, equal)

TEST_CASE("string.equal", "[external]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("empty")
        {
            TestExtStringEqual t("string.equal");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendSymbol(SYM("test"));
            REQUIRE(t.outputFloatAt(0) == 0);
            t.sendSymbol(SYM(""));
            REQUIRE(t.outputFloatAt(0) == 1);

            t.sendList(Atom(new DataTypeString("abc")));
            REQUIRE(t.outputFloatAt(0) == 0);
            t.sendList(Atom(new DataTypeString("")));
            REQUIRE(t.outputFloatAt(0) == 1);
        }

        SECTION("args")
        {
            TestExtStringEqual t("string.equal", LA("синица"));
            t->dump();
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendSymbol(SYM("test"));
            REQUIRE(t.outputFloatAt(0) == 0);
            t.sendSymbol(SYM("синица"));
            REQUIRE(t.outputFloatAt(0) == 1);

            t.sendList(Atom(new DataTypeString("abc")));
            REQUIRE(t.outputFloatAt(0) == 0);
            t.sendList(Atom(new DataTypeString("синица")));
            REQUIRE(t.outputFloatAt(0) == 1);
        }
    }

    SECTION("change pattern")
    {
        TestExtStringEqual t("string.equal", LA("sound"));

        t.sendSymbol(SYM("noise"));
        REQUIRE(t.outputFloatAt(0) == 0);
        t.sendSymbol(SYM("sound"));
        REQUIRE(t.outputFloatAt(0) == 1);

        t.sendListTo(LA("звук"), 1);

        t.sendSymbol(SYM("звук"));
        REQUIRE(t.outputFloatAt(0) == 1);
        t.sendSymbol(SYM("sound"));
        REQUIRE(t.outputFloatAt(0) == 0);

        // space
        t.sendListTo(LA("\"", "\""), 1);

        t.sendSymbol(SYM("a"));
        REQUIRE(t.outputFloatAt(0) == 0);
        t.sendSymbol(SYM(" "));
        REQUIRE(t.outputFloatAt(0) == 1);
    }
}
