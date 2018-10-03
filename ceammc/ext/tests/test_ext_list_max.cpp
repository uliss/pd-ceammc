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
#include "datatype_mlist.h"
#include "../list/list_max.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListMax, list, max);

TEST_CASE("list.max", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestListMax t("list.max");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY_LIST(t, @type, LA("float"));
    }

    SECTION("float")
    {
        TestExtListMax t("list.max");

        t << L();
        REQUIRE_FALSE(t.hasOutput());

        t << LA("a", "b", "c");
        REQUIRE_FALSE(t.hasOutput());

        t << LF(100);
        REQUIRE(t.outputFloatAt(0) == 100);

        t << LF(100, 200);
        REQUIRE(t.outputFloatAt(0) == 200);

        t << LF(100, 200, 50);
        REQUIRE(t.outputFloatAt(0) == 200);

        t << LA("a", 200, 50);
        REQUIRE(t.outputFloatAt(0) == 200);

        t.send(DataTypeMList());
        REQUIRE_FALSE(t.hasOutput());

        t.send(DataTypeMList("(())"));
        REQUIRE_FALSE(t.hasOutput());

        t.send(DataTypeMList("(1 2 3)"));
        REQUIRE(t.outputFloatAt(0) == 3);

        t.send(DataTypeMList("(1 2 () a 3)"));
        REQUIRE(t.outputFloatAt(0) == 3);
    }

    SECTION("symbol")
    {
        TestExtListMax t("list.max", LA("@symbol"));
        REQUIRE_PROPERTY_LIST(t, @type, LA("symbol"));

        t << L();
        REQUIRE_FALSE(t.hasOutput());

        t << LA("test");
        REQUIRE(t.outputSymbolAt(0) == A("test"));

        t << LA("a", "b", "c");
        REQUIRE(t.outputSymbolAt(0) == A("c"));

        t << LA("a", "b", 100);
        REQUIRE(t.outputSymbolAt(0) == A("b"));
    }

    SECTION("any")
    {
        TestExtListMax t("list.max", LA("@any"));
        REQUIRE_PROPERTY_LIST(t, @type, LA("any"));

        t << L();
        REQUIRE_FALSE(t.hasOutput());

        t << LA("a");
        REQUIRE(t.outputSymbolAt(0) == A("a"));

        t << LF(100);
        REQUIRE(t.outputFloatAt(0) == 100);

        t << LA(100, "a");
        REQUIRE(t.isOutputSymbolAt(0));
        REQUIRE(t.outputSymbolAt(0) == A("a"));

        t << LA(100, "a", "b", 200);
        REQUIRE(t.outputSymbolAt(0) == A("b"));
    }

    SECTION("mlist")
    {
        TestExtListMax t("list.max", LA("@any"));

        t.send(DataTypeMList());
        REQUIRE_FALSE(t.hasOutput());

        t.send(DataTypeMList("(3 2 1)"));
        REQUIRE(t.outputFloatAt(0) == 3);

        t.send(DataTypeMList("(3 2 ABC)"));
        REQUIRE(t.outputSymbolAt(0) == A("ABC"));

        t.send(DataTypeMList("(()()(()))"));
        REQUIRE_FALSE(t.hasOutput());
    }
}
