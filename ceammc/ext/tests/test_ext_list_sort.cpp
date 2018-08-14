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
#include "../list/list_sort.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListSort, list, sort);

TEST_CASE("list.sort", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TestListSort t("list.sort");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        TestExtListSort t("list.sort");

        t << L();
        REQUIRE(t.outputListAt(0) == L());

        t << LF(8, 6, 4, 2, 1, 5, 3, 7);
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3, 4, 5, 6, 7, 8));

        t << LA("z", "f", "d", "1", "11", 100, "sdf", "a");
        REQUIRE(t.outputListAt(0) == LA(100, "1", "11", "a", "d", "f", "sdf", "z"));

        t << LA("в", "а", "аб");
        REQUIRE(t.outputListAt(0) == LA("а", "аб", "в"));
    }

    SECTION("mlist")
    {
        TestExtListSort t("list.sort");

        t.send(DataTypeMList());
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList));

        t.send(DataTypeMList("(4 3 2 1)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(1 2 3 4)")));
    }
}
