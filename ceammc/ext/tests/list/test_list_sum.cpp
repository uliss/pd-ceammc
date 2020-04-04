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
#include "list_sum.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListSum, list, sum)

using TExt = TestExtListSum;
using TObj = TestListSum;

TEST_CASE("list.sum", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("list.sum");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        TObj t("list.sum", LF(2));

        WHEN_SEND_LIST_TO(0, t, {});
        REQUIRE_THAT(t, !hasOutput(&t));

        WHEN_SEND_FLOAT_TO(0, t, 100);
        REQUIRE_THAT(t, outputFloat(&t, 100));

        WHEN_SEND_LIST_TO(0, t, { 1000 });
        REQUIRE_THAT(t, outputFloat(&t, 1000));

        WHEN_SEND_LIST_TO(0, t, { 1, 2, 3, 4 });
        REQUIRE_THAT(t, outputFloat(&t, 10));

        WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c"));
        REQUIRE_THAT(t, !hasOutput(&t));

        WHEN_SEND_LIST_TO(0, t, LA("a", "b", 1));
        REQUIRE_THAT(t, outputFloat(&t, 1));
    }

    SECTION("mlist")
    {
        TExt t("list.sum");

        t.send(MLA());
        REQUIRE_THAT(t, !hasOutput(&t));;

        t.send(MLA(1));
        REQUIRE_THAT(t, outputFloat(&t, 1));

        t.send(MLA(1, 2, 3));
        REQUIRE_THAT(t, outputFloat(&t, 6));

        t.send(MLA(1, 2, 3, MLA(), MLA("a", "b", "c"), 1));
        REQUIRE_THAT(t, outputFloat(&t, 7));

        t.send(MLA("a", "b", "c"));
        REQUIRE_THAT(t, !hasOutput(&t));;
    }
}
