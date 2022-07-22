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
#include "dict_pass.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(DictPass, dict, pass)

TEST_CASE("dict.pass", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("default")
        {
            TestDictPass t("dict.pass");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @keys);
        }

        SECTION("args")
        {
            TExt t("dict.pass", LP("a \"b a\" \"@c\""));
            REQUIRE_PROPERTY(t, @keys, "a", "b a", "@c");
        }

        SECTION("@keys")
        {
            TExt t("dict.pass", LP("@keys a \"a b\" \"@c\""));
            REQUIRE_PROPERTY(t, @keys, "a", "a b", "@c");
        }
    }

    SECTION("pass all")
    {
        TExt t("dict.pass");
        REQUIRE(t.object());
        REQUIRE_PROPERTY(t, @keys);

        // pass nothing
        t.send(DictA("[a: 1 b: 2 c: 3]"));
        REQUIRE(dataAt(t) == DictA());
    }

    SECTION("pass ")
    {
        TExt t("dict.pass", "a");
        REQUIRE(t.object());

        // pass a
        t.send(DictA("[a: b c: d e: f]"));
        REQUIRE(dataAt(t) == DictA("[a: b]"));

        // set new pass list
        t.sendListTo(LA("c", "e", "@p"), 1);
        REQUIRE_PROPERTY(t, @keys, "c", "e", "@p");

        t.send(DictA("[a: b c: d e: f]"));
        REQUIRE(dataAt(t) == DictA("[c: d e: f]"));
    }

    SECTION("any")
    {
        TExt t("dict.pass", "a");

        t <<= AtomList::parseString("[a: b c: d e: f]");
        REQUIRE(dataAt(t) == DictA("[a: b]"));
    }
}
