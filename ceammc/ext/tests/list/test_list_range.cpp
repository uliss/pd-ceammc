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
#include "list_range.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListRange, list, range)

using TExt = TestExtListRange;
using TObj = TestListRange;

TEST_CASE("list.range", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TObj t("list.range");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        TExt t("list.range");

        t << L();
        REQUIRE_THAT(t, !hasOutput(&t));;
        t << LF(2);
        REQUIRE(t.outputListAt(0) == LX(2, 2));
    }

    SECTION("mlist")
    {
        TExt t("list.range");

        t.send(MLA());
        REQUIRE_THAT(t, !hasOutput(&t));;

        t.send(MLA(200));
        REQUIRE(t.outputListAt(0) == LF(200, 200));

        t.send(MLA(MLA(), 20));
        REQUIRE(t.outputListAt(0) == LF(20, 20));

        t.send(MLA(MLA(), 20, 30, 40));
        REQUIRE(t.outputListAt(0) == LF(20, 40));
    }

    SECTION("aliases")
    {
        TExt t1("list.range");
        TExt t2("list.minmax");

        REQUIRE(t1.object());
        REQUIRE(t2.object());
    }
}
