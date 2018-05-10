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
#include "../list/list_search.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListSearch> ListSearchTest;

TEST_CASE("list.search", "[externals]")
{
    SECTION("create")
    {
        ListSearchTest t("list.search");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("search")
    {
        SECTION("empty")
        {
            ListSearchTest t("list.search");
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_NO_MSG(t);

            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_NO_MSG(t);

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_NO_MSG(t);

            WHEN_SEND_DATA_TO(0, t, DataPtr(new IntData(100)));
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, AtomList());
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

            WHEN_SEND_LIST_TO(0, t, L1(123));
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

            WHEN_SEND_LIST_TO(0, t, L4(1, 2, 3, 4));
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());
        }

        SECTION("simple")
        {
            ListSearchTest t("list.search", L4(1, 2, 3, "A"));

            WHEN_SEND_LIST_TO(0, t, AtomList());
            REQUIRE_LIST_AT_OUTLET(0, t, L4(-1, -1, -1, -1));

            WHEN_SEND_LIST_TO(0, t, L1(100));
            REQUIRE_LIST_AT_OUTLET(0, t, L4(-1, -1, -1, -1));

            WHEN_SEND_LIST_TO(0, t, L1(1));
            REQUIRE_LIST_AT_OUTLET(0, t, L4(0.f, -1, -1, -1));

            WHEN_SEND_LIST_TO(0, t, L1(2));
            REQUIRE_LIST_AT_OUTLET(0, t, L4(-1, 0.f, -1, -1));

            WHEN_SEND_LIST_TO(0, t, L1(3));
            REQUIRE_LIST_AT_OUTLET(0, t, L4(-1, -1, 0.f, -1));

            WHEN_SEND_LIST_TO(0, t, L1("A"));
            REQUIRE_LIST_AT_OUTLET(0, t, L4(-1, -1, -1, 0.f));

            WHEN_SEND_LIST_TO(0, t, L2(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, L4(0.f, 1, -1, -1));

            WHEN_SEND_LIST_TO(0, t, L2(3, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, L4(-1, 1, 0.f, -1));

            WHEN_SEND_LIST_TO(0, t, L5("D", "E", "C", "B", "A"));
            REQUIRE_LIST_AT_OUTLET(0, t, L4(-1, -1, -1, 4));

            WHEN_SEND_LIST_TO(1, t, L1(8));
            WHEN_SEND_LIST_TO(0, t, L5(5, 6, 7, 8, 9));
            REQUIRE_LIST_AT_OUTLET(0, t, L1(3));

            WHEN_SEND_LIST_TO(0, t, L5(5, 8, 7, 8, 9));
            REQUIRE_LIST_AT_OUTLET(0, t, L1(1));
        }

        SECTION("data")
        {
            ListSearchTest t("list.search");

            DataPtr d0(new IntData(100));
            DataPtr d1(new IntData(200));
            DataPtr d2(new IntData(300));
            DataPtr d3(new IntData(400));
            DataPtr d4(new StrData("str1"));
            DataPtr d5(new StrData("str2"));

            WHEN_SEND_LIST_TO(0, t, L6(d0, d1, d2, d3, d4, d5));
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

            WHEN_SEND_LIST_TO(1, t, L2(d1, d5));
            WHEN_SEND_LIST_TO(0, t, L6(d0, d1, d2, d3, d4, d5));
            REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 5));
        }
    }
}
