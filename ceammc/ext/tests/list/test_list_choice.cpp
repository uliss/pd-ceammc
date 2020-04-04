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
#include "list_choice.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListChoice, list, choice)

using TObj = TestListChoice;
using TExt = TestExtListChoice;

TEST_CASE("list.choice", "[externals]")
{
    pd_test_init();

    SECTION("test create with:")
    {
        TObj t("list.choice");
        REQUIRE_THAT(t, hasProperty(&t, "@norepeat", 0));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_THAT(t, !hasOutput(&t));

        WHEN_SEND_FLOAT_TO(0, t, 123);
        REQUIRE_THAT(t, !hasOutput(&t));

        WHEN_SEND_SYMBOL_TO(0, t, "123");
        REQUIRE_THAT(t, !hasOutput(&t));

        SECTION("@norepeat")
        {
            TObj t("list.choice", LA("@norepeat", 1));
            REQUIRE_THAT(t, hasProperty(&t, "@norepeat", 1));

            TObj t1("list.choice", LA("@norepeat", 0.f));
            REQUIRE_THAT(t1, hasProperty(&t1, "@norepeat", 0.f));
        }
    }

    SECTION("choice")
    {
        TObj t("list.choice");

        // empty list
        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_THAT(t, !hasOutput(&t));

        // single value
        int N = 10;
        while (N-- > 0) {
            WHEN_SEND_LIST_TO(0, t, LF(11));
            REQUIRE_THAT(t, outputFloat(&t, 11));
        }

        // multiple values
        N = 10;
        while (N-- > 0) {
            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
            REQUIRE_THAT(t, hasOutput(&t));
            float v = t.lastMessage(0).atomValue().asFloat();
            bool ok = (v == 1 || v == 2 || v == 3 || v == 4);
            REQUIRE(ok);
        }
    }

    SECTION("no-repeat")
    {
        TObj t("list.choice", LA("@norepeat", 1));

        // empty list
        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_THAT(t, !hasOutput(&t));

        // single value
        // first time is ok
        WHEN_SEND_LIST_TO(0, t, LF(11));
        REQUIRE_THAT(t, outputFloat(&t, 11));

        // second is not output
        WHEN_SEND_LIST_TO(0, t, LF(11));
        REQUIRE_THAT(t, !hasOutput(&t));

        // two values
        WHEN_SEND_LIST_TO(0, t, LF(10, 20));
        REQUIRE_THAT(t, outputFloat(&t, 20));
        WHEN_SEND_LIST_TO(0, t, LF(10, 20));
        REQUIRE_THAT(t, outputFloat(&t, 10));
        WHEN_SEND_LIST_TO(0, t, LF(10, 20));
        REQUIRE_THAT(t, outputFloat(&t, 20));
        WHEN_SEND_LIST_TO(0, t, LF(10, 20));
        REQUIRE_THAT(t, outputFloat(&t, 10));
        WHEN_SEND_LIST_TO(0, t, LF(10, 10));
        REQUIRE_THAT(t, outputFloat(&t, 10));
        WHEN_SEND_LIST_TO(0, t, LF(10, 10));
        REQUIRE_THAT(t, outputFloat(&t, 10));

        // several values
        int n = 1000;
        float prev = 0;
        while (n-- > 0) {
            WHEN_SEND_LIST_TO(0, t, LF(10, 20, 30, 40, 50, 60, 70));
            REQUIRE_THAT(t, hasOutput(&t));
            float v = t.lastMessage(0).atomValue().asFloat();
            REQUIRE(v != Approx(prev));
            prev = v;
        }
    }
}
