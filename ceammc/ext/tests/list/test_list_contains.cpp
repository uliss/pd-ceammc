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
#include "list_contains.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListContains, list, contains)

using TObj = TestListContains;
using TExt = TestExtListContains;

TEST_CASE("list.contains", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TObj t("list.contains");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
        }
    }

    SECTION("empty")
    {
        TObj t("list.contains");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_THAT(t, outputFloat(&t, 1));

        WHEN_SEND_TDATA_TO(0, t, MLD("a", "b", "c"));
        REQUIRE_THAT(t, outputFloat(&t, 1));
    }

    SECTION("empty")
    {
        TObj t("list.contains", LF(11));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_NO_MSG(t);

        // symbol
        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_NO_MSG(t);

        // list
        WHEN_SEND_LIST_TO(0, t, LF(1, 11));
        REQUIRE_THAT(t, outputTrue(&t));

        WHEN_SEND_LIST_TO(0, t, LF(1));
        REQUIRE_THAT(t, outputFalse(&t));

        WHEN_SEND_LIST_TO(0, t, LF(2));
        REQUIRE_THAT(t, outputFalse(&t));

        WHEN_SEND_LIST_TO(0, t, LF(3, 4, 5, 6, 7, 8, 9, 10));
        REQUIRE_THAT(t, outputFalse(&t));

        // data
        WHEN_SEND_TDATA_TO(0, t, MLD("a", "b", "c"));
        REQUIRE_THAT(t, outputFalse(&t));

        WHEN_SEND_TDATA_TO(0, t, MLD(2, 3, 4, 11, 2, 3));
        REQUIRE_THAT(t, outputTrue(&t));
    }

    SECTION("inlet 2")
    {
        TObj t("list.contains", LF(2));
    }
}
