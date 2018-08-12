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
#include "../list/list_search.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListSearch, list, search);

TEST_CASE("list.search", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestListSearch t("list.search");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("search")
    {
        SECTION("empty")
        {
            TestListSearch t("list.search");
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_NO_MSG(t);

            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_NO_MSG(t);

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_NO_MSG(t);

            WHEN_SEND_DATA_TO(0, t, DataPtr(new IntData(100)));
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, L());
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            WHEN_SEND_LIST_TO(0, t, LF(123));
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
            REQUIRE_LIST_AT_OUTLET(0, t, L());
        }

        SECTION("simple")
        {
            TestListSearch t("list.search", LA(1, 2, 3, "A"));

            WHEN_SEND_LIST_TO(0, t, L());
            REQUIRE_LIST_AT_OUTLET(0, t, LF(-1, -1, -1, -1));

            WHEN_SEND_LIST_TO(0, t, LF(100));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(-1, -1, -1, -1));

            WHEN_SEND_LIST_TO(0, t, LF(1));
            REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, -1, -1, -1));

            WHEN_SEND_LIST_TO(0, t, LF(2));
            REQUIRE_LIST_AT_OUTLET(0, t, LA(-1, 0.f, -1, -1));

            WHEN_SEND_LIST_TO(0, t, LF(3));
            REQUIRE_LIST_AT_OUTLET(0, t, LA(-1, -1, 0.f, -1));

            WHEN_SEND_LIST_TO(0, t, LA("A"));
            REQUIRE_LIST_AT_OUTLET(0, t, LA(-1, -1, -1, 0.f));

            WHEN_SEND_LIST_TO(0, t, LF(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, 1, -1, -1));

            WHEN_SEND_LIST_TO(0, t, LF(3, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, LA(-1, 1, 0.f, -1));

            WHEN_SEND_LIST_TO(0, t, LA("D", "E", "C", "B", "A"));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(-1, -1, -1, 4));

            WHEN_SEND_LIST_TO(1, t, LF(8));
            WHEN_SEND_LIST_TO(0, t, LA(5, 6, 7, 8, 9));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(3));

            WHEN_SEND_LIST_TO(0, t, LA(5, 8, 7, 8, 9));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1));
        }

        SECTION("data")
        {
            TestListSearch t("list.search");

            DataPtr d0(new IntData(100));
            DataPtr d1(new IntData(200));
            DataPtr d2(new IntData(300));
            DataPtr d3(new IntData(400));
            DataPtr d4(new StrData("str1"));
            DataPtr d5(new StrData("str2"));

            WHEN_SEND_LIST_TO(0, t, LA(d0, d1, d2, d3, d4, d5));
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            WHEN_SEND_LIST_TO(1, t, LA(d1, d5));
            WHEN_SEND_LIST_TO(0, t, LA(d0, d1, d2, d3, d4, d5));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 5));
        }
    }

    SECTION("external")
    {
        TestExtListSearch t("list.search", LA("@a", 100, "B"));

        t << LA(1, 2, 3, "@a");
        REQUIRE(t.outputListAt(0) == LF(3, -1, -1));

        t << LF(10, 100);
        REQUIRE(t.outputListAt(0) == LF(-1, 1, -1));

        t << LA(10, 100, "B");
        REQUIRE(t.outputListAt(0) == LF(-1, 1, 2));

        t.send(DataTypeMList());
        REQUIRE(t.outputListAt(0) == LF(-1, -1, -1));

        t.send(DataTypeMList("(1 2 3 4 5 6 7 8 9 100 A B C D E @a @b)"));
        REQUIRE(t.outputListAt(0) == LF(15, 9, 11));
    }
}
