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
#include "../list/list_contains.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListContains> TestListContains;

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
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_DATA_TO(0, t, DataPtr(new IntData(100)));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
    }

    SECTION("empty")
    {
        TestListContains t("list.contains", L4(1, 2, 3, "A"));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        // symbol
        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        // list
        WHEN_SEND_LIST_TO(0, t, L2(1, 10));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, L1(1));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, L1(2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, L1(3));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, L2(2, 3));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, L2(1, 3));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        // data
        WHEN_SEND_DATA_TO(0, t, DataPtr(new IntData(100)));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        DataAtom d0(new IntData(1000));
        DataAtom d1(new StrData("ABC"));
        AtomList l0(d0.toAtom(), d1.toAtom());

        WHEN_SEND_LIST_TO(1, t, l0);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_DATA_TO(0, t, DataPtr(new IntData(1000)));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        WHEN_SEND_DATA_TO(0, t, DataPtr(new IntData(999)));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        WHEN_SEND_DATA_TO(0, t, DataPtr(new StrData("abc")));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        WHEN_SEND_DATA_TO(0, t, DataPtr(new StrData("ABC")));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
    }

    SECTION("inlet 2")
    {
        TestListContains t("list.contains", L1(2));
    }
}
