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
#include "list_search.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListSearch, list, search)

using TExt = TestExtListSearch;
using TObj = TestListSearch;

TEST_CASE("list.search", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TObj t("list.search");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("search")
    {
        SECTION("empty")
        {
            TObj t("list.search");
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_THAT(t, !hasOutput(&t));

            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_THAT(t, !hasOutput(&t));

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_THAT(t, !hasOutput(&t));

            WHEN_SEND_DATA_TO(0, t, IntData(100));
            REQUIRE_THAT(t, !hasOutput(&t));

            WHEN_SEND_LIST_TO(0, t, L());
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            WHEN_SEND_LIST_TO(0, t, LF(123));
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
            REQUIRE_LIST_AT_OUTLET(0, t, L());
        }

        SECTION("simple")
        {
            TObj t("list.search", LA(1, 2, 3, "A"));

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
            REQUIRE_THAT(t, outputFloat(&t, 3));

            WHEN_SEND_LIST_TO(0, t, LA(5, 8, 7, 8, 9));
            REQUIRE_THAT(t, outputFloat(&t, 1));
        }

        SECTION("data")
        {
            TObj t("list.search");

            IntA d0(100);
            IntA d1(200);
            IntA d2(300);
            IntA d3(400);
            StrA d4("str1");
            StrA d5("str2");

            WHEN_SEND_LIST_TO(0, t, LA(d0, d1, d2, d3, d4, d5));
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            WHEN_SEND_LIST_TO(1, t, LA(d1, d5));
            WHEN_SEND_LIST_TO(0, t, LA(d0, d1, d2, d3, d4, d5));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 5));
        }
    }

    SECTION("external")
    {
        TExt t("list.search", LA("@a", 100, "B"));

        t << LA(1, 2, 3, "@a");
        REQUIRE(t.outputListAt(0) == LF(3, -1, -1));

        t << LF(10, 100);
        REQUIRE(t.outputListAt(0) == LF(-1, 1, -1));

        t << LA(10, 100, "B");
        REQUIRE(t.outputListAt(0) == LF(-1, 1, 2));

        t.send(MLA());
        REQUIRE(t.outputListAt(0) == LF(-1, -1, -1));

        t.send(MLA(1, 2, 3, 4, 5, 6, 7, 8, 9, 100, "A", "B", "C", "D", "E", "@a", "@b"));
        REQUIRE(t.outputListAt(0) == LF(15, 9, 11));
    }
}
