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
#include "list_running_diff.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListRunningDiff, list, rundiff)

TEST_CASE("list.rundiff", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("list.rundiff", L());
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        TExt t("list.rundiff");

        t << L();
        REQUIRE(t.outputListAt(0) == L());

        t << LF(1);
        REQUIRE(t.outputListAt(0) == LF(1));

        t << LA("abc");
        REQUIRE(t.outputListAt(0) == LF(0));

        t << LF(1, 10);
        REQUIRE(t.outputListAt(0) == LF(1, 9));

        t << LA("S", 10);
        REQUIRE(t.outputListAt(0) == LF(0, 10));

        t << LA(10, "S");
        REQUIRE(t.outputListAt(0) == LF(10, -10));

        t << LA("S", 10, 21);
        REQUIRE(t.outputListAt(0) == LF(0, 10, 11));

        t << LA("S", "S");
        REQUIRE(t.outputListAt(0) == LF(0, 0));

        t << LF(1, 2, 3, 4, 5.5, -10.5);
        REQUIRE(t.outputListAt(0) == LF(1, 1, 1, 1, 1.5, -16));
    }
}
