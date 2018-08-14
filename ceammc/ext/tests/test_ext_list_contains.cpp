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
#include "../list/list_contains.h"
#include "catch.hpp"
#include "test_base.h"

#include <stdio.h>

typedef TestExternal<ListContains> TestListContains;

TEST_CASE("list.contains", "[externals]")
{
    SECTION("create")
    {
        SECTION("empty")
        {
            TestListContains t("list.contains");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
        }
    }

    SECTION("empty")
    {
        TestListContains t("list.contains");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_TDATA_TO(0, t, DataTypeMList("(a b c)"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
    }

    SECTION("empty")
    {
        TestListContains t("list.contains", LF(11));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_NO_MSG(t);

        // symbol
        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_NO_MSG(t);

        // list
        WHEN_SEND_LIST_TO(0, t, LF(1, 11));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, LF(1));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, LF(2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, LF(3));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        // data
        WHEN_SEND_TDATA_TO(0, t, DataTypeMList("(a b c)"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_TDATA_TO(0, t, DataTypeMList("(2 3 4 11 2 3)"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
    }

    SECTION("inlet 2")
    {
        TestListContains t("list.contains", LF(2));
    }
}
