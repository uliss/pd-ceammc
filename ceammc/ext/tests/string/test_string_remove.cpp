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
#include "string_remove.h"
#include "test_string_base.h"

PD_COMPLETE_TEST_SETUP(StringRemove, string, remove)

#define REQUIRE_STRING(obj, cstr)               \
    {                                           \
        REQUIRE(obj.hasOutputAt(0));            \
        REQUIRE(t.outputAtomAt(0) == SA(cstr)); \
        t.clearAll();                           \
    }

TEST_CASE("string.remove", "[external]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            TestExtStringRemove t("string.remove");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @mode, "all");
            REQUIRE_PROPERTY(t, @all, 1);
            REQUIRE_PROPERTY(t, @first, 0.f);
            REQUIRE_PROPERTY(t, @last, 0.f);

            t.sendSymbol(SYM("abc"));
            REQUIRE_STRING(t, "abc");

            t.sendList(SA("def"));
            REQUIRE_STRING(t, "def");

            t.sendFloat(123);
            REQUIRE(!t.hasOutput());

            t.sendList(IntA(1000));
            REQUIRE(!t.hasOutput());

            t.sendBang();
            REQUIRE(!t.hasOutput());
        }

        SECTION("space")
        {
            TestExtStringRemove t("string.remove", LA("\"", "\""));
            t.sendSymbol(SYM("abc"));
            REQUIRE_STRING(t, "abc");
            t.sendSymbol(SYM("a b c"));
            REQUIRE_STRING(t, "abc");
        }
    }

    SECTION("@mode")
    {
        TestExtStringRemove t("string.remove", LA(".", "@all"));
        t.sendSymbol(SYM(".abc."));
        REQUIRE_STRING(t, "abc");

        t->setProperty("@first", L());
        t.sendSymbol(SYM(".abc."));
        REQUIRE_STRING(t, "abc.");

        t->setProperty("@last", L());
        t.sendSymbol(SYM(".abc."));
        REQUIRE_STRING(t, ".abc");

        t.sendListTo(LA("?"), 1);
        t.sendSymbol(SYM(".abc?"));
        REQUIRE_STRING(t, ".abc");

        t.sendListTo(LA("\"", "\""), 1);
        t.sendSymbol(SYM("a b c"));
        REQUIRE_STRING(t, "a bc");
    }
}
