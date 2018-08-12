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
#include "../list/list_split.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListSplit, list, split)

TEST_CASE("list.split", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TestListSplit t("list.split");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE_PROPERTY(t, @at, 0.f);
    }

    SECTION("do")
    {
        TestListSplit t("list.split", LF(2));
        REQUIRE_PROPERTY(t, @at, 2);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
        REQUIRE_LIST_AT_OUTLET(1, t, LF(3, 4));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2));

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(1, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2));

        WHEN_SEND_LIST_TO(0, t, LF(1));
        REQUIRE_LIST_AT_OUTLET(1, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_LIST_AT_OUTLET(1, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, L());
    }

    SECTION("mlist")
    {
        TestExtListSplit t("list.split", LF(3));

        t.send(DataTypeMList());
        REQUIRE(t.outputDataAt(1) == DataPtr(new DataTypeMList));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList));

        t.send(DataTypeMList("(1)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(1)")));
        REQUIRE(t.outputDataAt(1) == DataPtr(new DataTypeMList));

        t.send(DataTypeMList("(1 2)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(1 2)")));
        REQUIRE(t.outputDataAt(1) == DataPtr(new DataTypeMList));

        t.send(DataTypeMList("(1 2 3)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(1 2 3)")));
        REQUIRE(t.outputDataAt(1) == DataPtr(new DataTypeMList));

        t.send(DataTypeMList("(1 2 3 4)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(1 2 3)")));
        REQUIRE(t.outputDataAt(1) == DataPtr(new DataTypeMList("(4)")));

        t.send(DataTypeMList("(1 2 3 4 5)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(1 2 3)")));
        REQUIRE(t.outputDataAt(1) == DataPtr(new DataTypeMList("(4 5)")));
    }
}
