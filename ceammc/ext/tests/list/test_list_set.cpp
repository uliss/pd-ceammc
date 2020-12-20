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
#include "list_set.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListSet, list, set)

using TExt = TestExtListSet;
using TObj = TestListSet;

TEST_CASE("list.set", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TObj t("list.set");
        REQUIRE(t.numInlets() == 3);
        REQUIRE(t.numOutlets() == 1);

        REQUIRE_THAT(t, hasProperty(&t, "@index", 0));
        REQUIRE_THAT(t, hasProperty(&t, "@value", Atom()));
    }

    SECTION("do")
    {
        SECTION("values")
        {
            TObj t("list.set");
        }

        SECTION("empty")
        {
            TExt t("list.set");

            t.sendList(L());
            REQUIRE_THAT(t, !hasOutput(&t));;
            t.sendList(LF(1));
            REQUIRE_THAT(t, !hasOutput(&t));;
            t.sendList(LF(1, 2, 3));
            REQUIRE_THAT(t, !hasOutput(&t));;
            t.send(MLA(1, 2, 3));
            REQUIRE_THAT(t, !hasOutput(&t));;
        }

        SECTION("index0")
        {
            TExt t("list.set", LF(0, -10));
            REQUIRE_THAT(t, hasProperty(&t, "@index", 0.f)); 
            REQUIRE_THAT(t, hasProperty(&t, "@value", -10)); 

            t.sendList(L());
            REQUIRE(t.outputListAt(0) == L());

            t.send(1, 2);
            REQUIRE(t.outputListAt(0) == LF(-10, 2));
            t.send(1, 2, 3);
            REQUIRE(t.outputListAt(0) == LF(-10, 2, 3));
            t.send(MLA(1, 2, 3));
            REQUIRE(t.outputAtomAt(0) == MLA(-10, 2, 3));
        }

        SECTION("index-1")
        {
            TExt t("list.set", LA(-1, "ABC"));
            REQUIRE_THAT(t, hasProperty(&t, "@index", -1)); 
            REQUIRE_THAT(t, hasProperty(&t, "@value", "ABC")); 

            t.send(1, 2);
            REQUIRE(t.outputListAt(0) == LA(1, "ABC"));
            t.send(1, 2, 3);
            REQUIRE(t.outputListAt(0) == LA(1, 2, "ABC"));
        }
    }

    SECTION("mlist")
    {
        TExt t("list.set");
        REQUIRE_THAT(t, hasProperty(&t, "@index", 0));
        REQUIRE_THAT(t, hasProperty(&t, "@value", Atom()));
    }
}
