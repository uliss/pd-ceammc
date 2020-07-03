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
#include "list_each.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListEach, list, each)

using TExt = TestExtListEach;
using TObj = TestListEach;

typedef TestExternal<ListEach> TObj;

TEST_CASE("list.each", "[externals]")
{
    pd_test_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            TObj t("list.each", L());
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);

            REQUIRE_THAT(t, hasProperty(&t, "@step", 1));

            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, L());
            REQUIRE(t.messageCount(1) == 3);
            REQUIRE(t.messageAt(0, 1).atomValue().asFloat() == 1);
            REQUIRE(t.messageAt(1, 1).atomValue().asFloat() == 2);
            REQUIRE(t.messageAt(2, 1).atomValue().asFloat() == 3);
        }

        SECTION("properties")
        {
            TObj t("list.each", LA("@step", 2));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);

            REQUIRE_THAT(t, hasProperty(&t, "@step", 2));

            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
            REQUIRE_LIST_AT_OUTLET(0, t, L());
            REQUIRE(t.messageCount(1) == 2);
            REQUIRE(t.messageAt(0, 1).listValue() == LF(1, 2));
            REQUIRE(t.messageAt(1, 1).listValue() == LF(3, 4));

            t.cleanAllMessages();

            WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5));
            REQUIRE_LIST_AT_OUTLET(0, t, L());
            REQUIRE(t.messageCount(1) == 3);
            REQUIRE(t.messageAt(0, 1).listValue() == LF(1, 2));
            REQUIRE(t.messageAt(1, 1).listValue() == LF(3, 4));
            REQUIRE(t.messageAt(2, 1).listValue() == LF(5));
        }

        SECTION("positional arguments")
        {
            TObj t("list.each", LF(12));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);

            REQUIRE_THAT(t, hasProperty(&t, "@step", 12));
        }

        SECTION("positional arguments and props mixed")
        {
            TObj t("list.each", LA(12, "@step", 4));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);

            // property arguments have priority
            REQUIRE_THAT(t, hasProperty(&t, "@step", 4));
        }

        SECTION("invalid args")
        {
            SECTION("props")
            {
                TObj t("list.each", LA("@prop", -100));
                REQUIRE(t.numInlets() == 2);
                REQUIRE(t.numOutlets() == 2);

                // default value
                REQUIRE_THAT(t, hasProperty(&t, "@step", 1));
            }

            SECTION("positional")
            {
                TObj t("list.each", LF(-100));
                REQUIRE(t.numInlets() == 2);
                REQUIRE(t.numOutlets() == 2);

                // default value
                REQUIRE_THAT(t, hasProperty(&t, "@step", 1));
            }
        }
    }

    SECTION("step 2")
    {
        TExt t("list.each", 2);
        REQUIRE_PROPERTY(t, @step, 2);

        External mul("*");

        // [list.each] X [*]
        REQUIRE(t.object());
        REQUIRE(mul.object());
        REQUIRE(t.connectTo(1, mul, 0));
        REQUIRE(t.connectFrom(0, mul, 1));

        t << L();
        CHECK(t.outputListAt(0) == L());

        t << LF(10);
        CHECK(t.outputListAt(0) == LF(0));

        t << LF(10, 20);
        CHECK(t.outputListAt(0) == LF(200));

        t << LF(10, 20, 30);
        CHECK(t.outputListAt(0) == LF(200, 600));

        t << LF(10, 20, 30, 40);
        CHECK(t.outputListAt(0) == LF(200, 1200));
    }

    SECTION("test mlist")
    {
        TExt t("list.each");
        External mul("*", 2);

        // [list.each] X [* 2]
        REQUIRE(t.object());
        REQUIRE(mul.object());
        REQUIRE(t.connectTo(1, mul, 0));
        REQUIRE(t.connectFrom(0, mul, 1));

        t << L();
        CHECK(t.outputListAt(0) == L());

        t << LF(1, 2, 3);
        REQUIRE(t.outputListAt(0) == LF(2, 4, 6));

        t.send(MLA());
        REQUIRE(t.outputAtomAt(0) == MLA());

        t.send(MLA(1, 2, 3));
        REQUIRE(t.outputAtomAt(0) == MLA(2, 4, 6));

        t.send(MLA("a", "b", "c"));
        REQUIRE(t.outputAtomAt(0) == MLA());
    }
}
