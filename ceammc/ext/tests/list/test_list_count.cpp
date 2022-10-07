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
#include "list_count.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListCount, list, count)

using TExt = TestExtListCount;
using TObj = TestListCount;

TEST_CASE("list.count", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            TObj t("list.count");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_NONE(t, @pattern);
        }

        SECTION("positional symbol")
        {
            TObj t("list.count", LA("c"));
            REQUIRE_THAT(t, hasProperty(&t, "@pattern", "c"));
        }

        SECTION("positional float")
        {
            TObj t("list.count", LF(2));
            REQUIRE_THAT(t, hasProperty(&t, "@pattern", 2));
        }

        SECTION("positional list")
        {
            TObj t("list.count", LA(1, 2, 3, 4, 5));
            REQUIRE_THAT(t, hasProperty(&t, "@pattern", 1));
        }

        SECTION("properties")
        {
            TObj t("list.count", LA("@pattern", 111));
            REQUIRE_THAT(t, hasProperty(&t, "@pattern", 111));
        }
    }

    SECTION("do")
    {
        TObj t("list.count", LF(2));

        WHEN_SEND_LIST_TO(0, t, LF(1, 3, 4, 5));
        REQUIRE_THAT(t, outputFloat(&t, 0));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_THAT(t, outputFloat(&t, 0));

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_THAT(t, outputFloat(&t, 1));

        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 4, 5, 2));
        REQUIRE_THAT(t, outputFloat(&t, 2));
    }

    SECTION("inlet 2")
    {
        TObj t("list.count", LF(2));
        REQUIRE_THAT(t, hasProperty(&t, "@pattern", 2));

        WHEN_SEND_FLOAT_TO(1, t, 122);
        REQUIRE_THAT(t, hasProperty(&t, "@pattern", 122));

        WHEN_SEND_SYMBOL_TO(1, t, "A");
        REQUIRE_THAT(t, hasProperty(&t, "@pattern", "A"));

        WHEN_SEND_LIST_TO(1, t, LA("C", "B", "A"));
        REQUIRE_THAT(t, hasProperty(&t, "@pattern", "C"));

        WHEN_SEND_LIST_TO(1, t, L());
        REQUIRE_THAT(t, hasProperty(&t, "@pattern", "C"));
    }

    SECTION("external simple")
    {
        TExt t("list.count", LF(10));

        t.sendList(LF(1, 2, 3, 4));
        REQUIRE_THAT(t, outputFloat(&t, 0));

        t.sendList(LF(1, 2, 10, 4));
        REQUIRE_THAT(t, outputFloat(&t, 1));

        t.sendList(LF(10, 2, 10, 4));
        REQUIRE_THAT(t, outputFloat(&t, 2));
    }

    SECTION("external data")
    {
        TExt t("list.count", LP("(1 2)"));

        t.send(1, 2);
        REQUIRE(floatAt(t) == 0);

        t.send(1, 2, 3, MLA());
        REQUIRE(floatAt(t) == 0);

        t.send(1, 2, 3, MLA(1));
        REQUIRE(floatAt(t) == 0);

        t.send(1, 2, 3, MLA(1, 2));
        REQUIRE(floatAt(t) == 1);

        t.send(1, 2, 3, MLA(1, 2), 4, MLA(1, 2));
        REQUIRE(floatAt(t) == 2);

        t.send(MLA(1, MLA(1, 2)));
        REQUIRE(floatAt(t) == 1);
    }
}
