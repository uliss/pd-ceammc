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
#include "list_max.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListMax, list, max)

using TExt = TestExtListMax;
using TObj = TestListMax;

TEST_CASE("list.max", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TObj t("list.max");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_THAT(t, hasProperty(&t, "@type", "float")); 
    }

    SECTION("float")
    {
        TExt t("list.max");

        t << L();
        REQUIRE_THAT(t, !hasOutput(&t));;

        t << LA("a", "b", "c");
        REQUIRE_THAT(t, !hasOutput(&t));;

        t << LF(100);
        REQUIRE_THAT(t, outputFloat(&t, 100));

        t << LF(100, 200);
        REQUIRE_THAT(t, outputFloat(&t, 200));

        t << LF(100, 200, 50);
        REQUIRE_THAT(t, outputFloat(&t, 200));

        t << LA("a", 200, 50);
        REQUIRE_THAT(t, outputFloat(&t, 200));

        t.send(MLA());
        REQUIRE_THAT(t, !hasOutput(&t));;

        t.send(MLA(1, 2, 3));
        REQUIRE_THAT(t, outputFloat(&t, 3));

        t.send(MLA(1, 2, MLA(), "a", 3));
        REQUIRE_THAT(t, outputFloat(&t, 3));
    }

    SECTION("symbol")
    {
        TExt t("list.max", LA("@symbol"));
        REQUIRE_THAT(t, hasProperty(&t, "@type", "symbol")); 

        t << L();
        REQUIRE_THAT(t, !hasOutput(&t));;

        t << LA("test");
        REQUIRE(t.outputSymbolAt(0) == A("test"));

        t << LA("a", "b", "c");
        REQUIRE(t.outputSymbolAt(0) == A("c"));

        t << LA("a", "b", 100);
        REQUIRE(t.outputSymbolAt(0) == A("b"));
    }

    SECTION("any")
    {
        TExt t("list.max", LA("@any"));
        REQUIRE_THAT(t, hasProperty(&t, "@type", "any")); 

        t << L();
        REQUIRE_THAT(t, !hasOutput(&t));;

        t << LA("a");
        REQUIRE(t.outputSymbolAt(0) == A("a"));

        t << LF(100);
        REQUIRE_THAT(t, outputFloat(&t, 100));

        t << LA(100, "a");
        REQUIRE(t.isOutputSymbolAt(0));
        REQUIRE(t.outputSymbolAt(0) == A("a"));

        t << LA(100, "a", "b", 200);
        REQUIRE(t.outputSymbolAt(0) == A("b"));
    }

    SECTION("mlist")
    {
        TExt t("list.max", LA("@any"));

        t.send(MLA());
        REQUIRE_THAT(t, !hasOutput(&t));;

        t.send(MLA(3, 2, 1));
        REQUIRE_THAT(t, outputFloat(&t, 3));

        t.send(MLA(3, 2, "ABC"));
        REQUIRE(t.outputSymbolAt(0) == A("ABC"));

        t.send(MLA(MLA(), MLA()));
        REQUIRE(t.outputAtomAt(0) == MLA());
    }
}
