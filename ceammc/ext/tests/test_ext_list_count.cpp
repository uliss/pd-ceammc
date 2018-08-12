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
#include "../list/list_count.h"
#include "test_base.h"
#include "test_external.h"

#include <stdio.h>

PD_COMPLETE_TEST_SETUP(ListCount, list, count)

TEST_CASE("list.count", "[externals]")
{
    pd_test_mod_init_list_count();

    SECTION("init")
    {
        SECTION("empty")
        {
            TestListCount t("list.count");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_NONE(t, @pattern);
        }

        SECTION("positional symbol")
        {
            TestListCount t("list.count", LA("c"));
            REQUIRE_PROPERTY(t, @pattern, "c");
        }

        SECTION("positional float")
        {
            TestListCount t("list.count", LF(2));
            REQUIRE_PROPERTY(t, @pattern, 2);
        }

        SECTION("positional list")
        {
            TestListCount t("list.count", LA(1, 2, 3, 4, 5));
            REQUIRE_PROPERTY(t, @pattern, 1);
        }

        SECTION("properties")
        {
            TestListCount t("list.count", LA("@pattern", 111));
            REQUIRE_PROPERTY(t, @pattern, 111);
        }
    }

    SECTION("do")
    {
        TestListCount t("list.count", LF(2));

        WHEN_SEND_LIST_TO(0, t, LF(1, 3, 4, 5));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 4, 5, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);
    }

    SECTION("inlet 2")
    {
        TestListCount t("list.count", LF(2));
        REQUIRE_PROPERTY(t, @pattern, 2);

        WHEN_SEND_FLOAT_TO(1, t, 122);
        REQUIRE_PROPERTY(t, @pattern, 122);

        WHEN_SEND_SYMBOL_TO(1, t, "A");
        REQUIRE_PROPERTY(t, @pattern, "A");

        WHEN_SEND_LIST_TO(1, t, LA("C", "B", "A"));
        REQUIRE_PROPERTY(t, @pattern, "C");

        WHEN_SEND_LIST_TO(1, t, L());
        REQUIRE_PROPERTY(t, @pattern, "C");
    }

    SECTION("external simple")
    {
        TestExtListCount t("list.count", LF(10));

        t.sendList(LF(1, 2, 3, 4));
        REQUIRE(t.outputFloatAt(0) == 0);

        t.sendList(LF(1, 2, 10, 4));
        REQUIRE(t.outputFloatAt(0) == 1);

        t.sendList(LF(10, 2, 10, 4));
        REQUIRE(t.outputFloatAt(0) == 2);
    }

    SECTION("external data")
    {
        DataPtr d100 = new IntData(100);
        DataPtr d200 = new IntData(200);
        DataPtr d100_new = new IntData(100);

        TestExtListCount t("list.count", LA(d100));

        t.sendList(LF(1, 2, 3, 100));
        REQUIRE(t.outputFloatAt(0) == 0);

        t.sendList(LA(1, 2, 100, d200));
        REQUIRE(t.outputFloatAt(0) == 0);

        t.sendList(LA(1, 2, 100, d100));
        REQUIRE(t.outputFloatAt(0) == 1);

        t.sendList(LA(1, 2, d100, d100_new));
        REQUIRE(t.outputFloatAt(0) == 2);
    }
}
