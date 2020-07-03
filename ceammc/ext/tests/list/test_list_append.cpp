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
#include "list_append.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListAppend, list, append)

using TObj = TestListAppend;
using TExt = TestExtListAppend;
using IntA = DataAtom<IntData>;

TEST_CASE("list.append", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TObj t("list.append");
        CHECK(t.numInlets() == 2);
        CHECK(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @value);
    }

    SECTION("empty")
    {
        TExt t("list.append");

        t.bang();
        CHECK(listAt(t) == L());

        t << 10;
        CHECK(floatAt(t) == 10);

        t << "ABC";
        CHECK(symbolAt(t) == "ABC");

        t << IntA(12);
        CHECK(atomAt(t) == IntA(12));

        t << L();
        CHECK(listAt(t) == L());

        t << LF(1, 2);
        CHECK(listAt(t) == LF(1, 2));

        t << MLA(1, 2);
        CHECK(dataAt(t) == MLA(1, 2));
    }

    SECTION("atom")
    {
        TExt t("list.append", 0xBEEF);

        t.bang();
        CHECK(floatAt(t) == 0xBEEF);

        t << 10;
        CHECK(listAt(t) == LF(10, 0xBEEF));

        t << "ABC";
        CHECK(listAt(t) == LA("ABC", 0xBEEF));

        t << IntA(12);
        CHECK(listAt(t) == LA(IntA(12), 0xBEEF));

        t << L();
        CHECK(floatAt(t) == 0xBEEF);

        t << LF(1, 2);
        CHECK(listAt(t) == LF(1, 2, 0xBEEF));

        t.sendList(MLA(1, 2));
        CHECK(dataAt(t) == MLA(1, 2, 0xBEEF));
    }

    SECTION("args")
    {
        TObj t("list.append", LA("\"@prop\"", 1));
        REQUIRE_PROPERTY(t, @value, "@prop", 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LA("@prop", 1));

        WHEN_SEND_LIST_TO(0, t, LF(10));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(10, "@prop", 1));

        WHEN_SEND_LIST_TO(0, t, LA("ABC"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("ABC", "@prop", 1));

        WHEN_SEND_DATA_TO(0, t, IntData(12));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(IntA(12), "@prop", 1));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, LA("@prop", 1));

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(1, 2, "@prop", 1));

        WHEN_SEND_LIST_TO(1, t, LF(100));
        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 100));
    }

    SECTION("data")
    {
        TExt t("list.append");

        t.sendListTo(LA(IntA(10), IntA(20)), 1);
        REQUIRE_PROPERTY(t, @value, IntA(10), IntA(20));

        t.send(IntA(30));
        REQUIRE(t.outputListAt(0) == LA(IntA(30), IntA(10), IntA(20)));

        t.send("abc");
        REQUIRE(t.outputListAt(0) == LA("abc", IntA(10), IntA(20)));

        t.sendListTo(LA("a", "b"), 1);
        REQUIRE_PROPERTY(t, @value, "a", "b");

        t.send(MLA());
        REQUIRE(dataAt(t) == MLA("a", "b"));

        t << MLA(1, 2, 3, MLA());
        REQUIRE(dataAt(t) == MLA(1, 2, 3, MLA(), "a", "b"));
    }
}
