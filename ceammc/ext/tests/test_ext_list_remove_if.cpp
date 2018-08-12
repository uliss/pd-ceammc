/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "../list/list_remove_if.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListRemoveIf, list, remove_if);

TEST_CASE("list.remove_if", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TestListRemoveIf t("list.remove_if");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("do")
    {
        TestExtListRemoveIf t("list.remove_if");
        External pred("<", LF(3));

        t.connectTo(1, pred, 0);
        t.connectFrom(0, pred, 1);

        t << L();
        REQUIRE(t.outputListAt(0) == L());

        t << LF(1);
        REQUIRE(t.outputListAt(0) == L());

        t << LF(1, 2);
        REQUIRE(t.outputListAt(0) == L());

        t << LF(1, 2, 3);
        REQUIRE(t.outputFloatAt(0) == 3);

        t << LF(1, 2, 3, 4);
        REQUIRE(t.outputListAt(0) == LF(3, 4));

        t << LF(45, 2, 3, 4);
        REQUIRE(t.outputListAt(0) == LF(45, 3, 4));

        t.send(DataTypeMList());
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList()));

        t.send(DataTypeMList("(1)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList()));

        t.send(DataTypeMList("(2 1)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList()));

        t.send(DataTypeMList("(3 2 1)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(3)")));

        t.send(DataTypeMList("(3 2 1 8)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(3 8)")));
    }
}
