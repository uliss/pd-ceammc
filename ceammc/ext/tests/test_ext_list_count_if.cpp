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
#include "../list/list_count_if.h"
#include "test_base.h"
#include "test_external.h"

#include <stdio.h>

PD_COMPLETE_TEST_SETUP(ListCountIf, list, count_if)

TEST_CASE("list.count_if", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TestListCountIf t("list.count_if");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("external simple")
    {
        TestExtListCountIf t("list.count_if");
        pd::External pred(">", LF(2));

        // [list.count_if] X [> 2]

        t.connectTo(1, pred, 0);
        t.connectFrom(0, pred, 1);

        t.sendList(L());
        REQUIRE(t.outputFloatAt(0) == 0);

        t.sendList(LF(1, 2));
        REQUIRE(t.outputFloatAt(0) == 0);

        t.sendList(LF(1, 2, 3));
        REQUIRE(t.outputFloatAt(0) == 1);

        t.sendList(LF(1, 2, 3, 4));
        REQUIRE(t.outputFloatAt(0) == 2);
    }

    SECTION("external data")
    {
        TestExtListCountIf t("list.count_if");
        pd::External pred("<", LF(2));

        t.connectTo(1, pred, 0);
        t.connectFrom(0, pred, 1);

        DataPtr d100(new IntData(100));
        DataPtr d1(new IntData(1));

        t.sendList(LA(d100));
        REQUIRE(t.outputFloatAt(0) == 0);

        t.sendList(LA(1, d1, -1, 2));
        REQUIRE(t.outputFloatAt(0) == 2);
    }
}
