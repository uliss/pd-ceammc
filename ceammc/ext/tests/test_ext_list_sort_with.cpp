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
#include "../list/list_sort_with.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListSortWith, list, sort_with);

TEST_CASE("list.sort_with", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TestExtListSortWith t("list.sort_with");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("sort <")
    {
        TestExtListSortWith t("list.sort_with");
        External cmp("<");

        t.connectTo(1, cmp, 0);
        t.connectFrom(0, cmp, 1);

        t << L();
        REQUIRE(t.outputListAt(0) == L());

        t << LF(100);
        REQUIRE(t.outputFloatAt(0) == 100);

        t << LF(5, 2, 4);
        REQUIRE(t.outputListAt(0) == LF(2, 4, 5));
    }

    SECTION("sort >")
    {
        TestExtListSortWith t("list.sort_with");
        External cmp(">");

        t.connectTo(1, cmp, 0);
        t.connectFrom(0, cmp, 1);

        t << L();
        REQUIRE(t.outputListAt(0) == L());

        t << LF(100);
        REQUIRE(t.outputFloatAt(0) == 100);

        t << LF(5, 2, 4);
        REQUIRE(t.outputListAt(0) == LF(5, 4, 2));

        t.send(DataTypeMList());
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList()));

        t.send(DataTypeMList("(1 2 3 4 5)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(5 4 3 2 1)")));
    }
}
