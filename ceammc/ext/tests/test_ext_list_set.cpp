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
#include "../data/datatype_mlist.h"
#include "../list/list_set.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListSet, list, set);

TEST_CASE("list.set", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestListSet t("list.set");
        REQUIRE(t.numInlets() == 3);
        REQUIRE(t.numOutlets() == 1);

        REQUIRE(t.index() == 0);
        REQUIRE(!t.value().isValid());
    }

    SECTION("do")
    {
        SECTION("values")
        {
            TestListSet t("list.set");
        }

        SECTION("empty")
        {
            TestExtListSet t("list.set");

            t.sendList(L());
            REQUIRE_FALSE(t.hasOutput());
            t.sendList(LF(1));
            REQUIRE_FALSE(t.hasOutput());
            t.sendList(LF(1, 2, 3));
            REQUIRE_FALSE(t.hasOutput());
            t.send(DataTypeMList("(1 2 3)"));
            REQUIRE_FALSE(t.hasOutput());
        }

        SECTION("index0")
        {
            TestExtListSet t("list.set", LF(0, -10));
            REQUIRE(t->index() == 0);
            REQUIRE(t->value() == DataAtom(Atom(-10)));

            t.sendList(L());
            REQUIRE(t.outputListAt(0) == L());

            t.sendList(LF(1, 2));
            REQUIRE(t.outputListAt(0) == LF(-10, 2));
            t.sendList(LF(1, 2, 3));
            REQUIRE(t.outputListAt(0) == LF(-10, 2, 3));
            t.send(DataTypeMList("(1 2 3)"));
            REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(-10 2 3)")));
        }

        SECTION("index-1")
        {
            TestExtListSet t("list.set", LA(-1, "ABC"));
            REQUIRE(t->index() == -1);

            t.sendList(LF(1, 2));
            REQUIRE(t.outputListAt(0) == LA(1, "ABC"));
            t.sendList(LF(1, 2, 3));
            REQUIRE(t.outputListAt(0) == LA(1, 2, "ABC"));
        }
    }

    SECTION("mlist")
    {
        TestExtListSet t("list.set");
    }
}
