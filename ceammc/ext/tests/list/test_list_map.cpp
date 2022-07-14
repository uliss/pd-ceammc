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
#include "datatype_dict.h"
#include "list_map.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListMap, list, map)

using DA = DictAtom;

TEST_CASE("list.map", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("list.map");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @dict, DA());
        }

        SECTION("args")
        {
            TObj t("list.map", LP("[a: 1 2 3 b: A B C d: e: (1 2 3)]"));
            REQUIRE_PROPERTY_LIST(t, @dict, DA("[a: 1 2 3 b: A B C d: e: (1 2 3)]"));
        }

        SECTION("@dict")
        {
            TObj t("list.map", LP("@dict [1: abc 2: def]"));
            REQUIRE_PROPERTY_LIST(t, @dict, DA("[1: abc 2: def]"));
        }
    }

    SECTION("float")
    {
        TExt t("list.map");

        // empty dict
        t << 1;
        REQUIRE(!t.hasOutput());
        t << 2;
        REQUIRE(!t.hasOutput());
        t << 3;
        REQUIRE(!t.hasOutput());

        t.sendListTo(LP("[2: 1 2 3: 1 2 3]"), 1);
        REQUIRE_PROPERTY(t, @dict, DA("[2: 1 2 3: 1 2 3]"));

        t << 1;
        REQUIRE(!t.hasOutput());
        t << 2;
        REQUIRE(t.outputListAt(0) == LF(1, 2));
        t << 3;
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3));

        t << 2.1;
        REQUIRE(!t.hasOutput());
    }

    SECTION("symbol")
    {
        TExt t("list.map");

        // empty dict
        t << "A";
        REQUIRE(!t.hasOutput());
        t << "B";
        REQUIRE(!t.hasOutput());
        t << "C";
        REQUIRE(!t.hasOutput());

        t.sendMessage(SYM("@dict"), LP("[A: 1 2 B: \"with spaces\"]"));
        REQUIRE_PROPERTY(t, @dict, DA("[A: 1 2 B: \"with spaces\"]"));

        t << "A";
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LF(1, 2));
        t << "B";
        REQUIRE(t.outputListAt(0) == LA("with spaces"));
        t << "C";
        REQUIRE(!t.hasOutput());
    }

    SECTION("list")
    {
        TExt t("list.map");

        // empty dict
        t << LF(1, 2, 3, 4, 5);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == L());

        t.sendListTo(LP("[1: one 2: two 3: three 4: four 5: five]"), 1);

        t << L();
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == L());

        t << LF(1);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA("one"));

        t << LF(0);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == L());

        t << LF(3, 2, 1, 0, 1, 2, 3);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA("three", "two", "one", "one", "two", "three"));
    }

    SECTION("mlist")
    {
        TExt t("list.map", LP("[1: one 2: two 3: three 4: four 5: five]"));

        t << MLA();
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == MLA());

        t << MLA(1);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == MLA("one"));

        t << MLA(0);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == MLA());

        t << MLA(3, 2, 1, 0, 1, 2, 3);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == MLA("three", "two", "one", "one", "two", "three"));
    }
}
