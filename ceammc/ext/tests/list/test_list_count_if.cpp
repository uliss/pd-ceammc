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
#include "list_count_if.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListCountIf, list, count_if)

using TExt = TestExtListCountIf;
using TObj = TestListCountIf;

TEST_CASE("list.count_if", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("list.count_if");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("external simple")
    {
        TExt t("list.count_if");
        pd::External pred(">", 2);

        // [list.count_if] X [> 2]

        t.connectTo(1, pred, 0);
        t.connectFrom(0, pred, 1);

        t.sendList(L());
        REQUIRE_THAT(t, outputFloat(&t, 0));

        t.sendList(LF(1, 2));
        REQUIRE_THAT(t, outputFloat(&t, 0));

        t.sendList(LF(1, 2, 3));
        REQUIRE_THAT(t, outputFloat(&t, 1));

        t.sendList(LF(1, 2, 3, 4));
        REQUIRE_THAT(t, outputFloat(&t, 2));
    }

    SECTION("external data")
    {
        TExt t("list.count_if");
        pd::External pred("<", 2);

        t.connectTo(1, pred, 0);
        t.connectFrom(0, pred, 1);

        t.sendList(LA(MLA(100)));
        REQUIRE_THAT(t, outputFloat(&t, 0));

        t.sendList(LA(1, MLA(1), -1, 2));
        REQUIRE_THAT(t, outputFloat(&t, 2));
    }
}
