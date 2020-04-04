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
#include "list_enumerate.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListEnumerate, list, enumerate)

using TExt = TestExtListEnumerate;
using TObj = TestListEnumerate;

TEST_CASE("list.enumerate", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("list.enumerate");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @from, 0);
        }

        SECTION("args")
        {
            TObj t("list.enumerate", LF(100));
            REQUIRE_PROPERTY_FLOAT(t, @from, 100);
        }

        SECTION("props")
        {
            TObj t("list.enumerate", LA("@from", 10));
            REQUIRE_PROPERTY_FLOAT(t, @from, 10);
        }
    }

    SECTION("do")
    {
        TObj t("list.enumerate");

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, "a", 1, "b", 2, "c"));
    }

    SECTION("@from")
    {
        TObj t("list.enumerate", LA("@from", 1));

        WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(1, "a", 2, "b", 3, "c"));

        t.setProperty("@from", LF(-10));
        WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(-10, "a", -9, "b", -8, "c"));
    }

    SECTION("@append")
    {
        TObj t("list.enumerate", LA("@append", "@from", 1));

        WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("a", 1, "b", 2, "c", 3));
    }

    SECTION("mlist")
    {
        TExt t("list.enumerate", LF(1));

        t.send(MLA());
        REQUIRE(t.outputListAt(0) == L());

        t.send(MLA("a", "b"));
        REQUIRE(t.outputListAt(0) == LA(1, "a", 2, "b"));
    }
}
