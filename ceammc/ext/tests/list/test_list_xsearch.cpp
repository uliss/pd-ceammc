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
#include "list_xsearch.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListXSearch, list, xsearch)

using TExt = TestExtListXSearch;
using TObj = TestListXSearch;

TEST_CASE("list.^search", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TestListXSearch t("list.^search");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @start, 0);
            REQUIRE_PROPERTY_FLOAT(t, @end, -1);
        }

        SECTION("props")
        {
            TestListXSearch t("list.^search", LA("@start", 1, "@end", 100));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @start, 1);
            REQUIRE_PROPERTY_FLOAT(t, @end, 100);
        }
    }

    SECTION("search")
    {
        TestListXSearch t("list.^search", LF(1, 2, 3, 4, 3, 2, 1, 2));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_THAT(t, !hasOutput(&t));

        WHEN_SEND_FLOAT_TO(0, t, 10000);
        REQUIRE_THAT(t, outputFloat(&t, -1));

        return;
        REQUIRE_THAT(t, outputFloat(&t, -1));
        return;

        WHEN_SEND_FLOAT_TO(0, t, 10000);
        REQUIRE_THAT(t, outputFloat(&t, -1));

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_THAT(t, outputFloat(&t, 0));
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_THAT(t, outputFloat(&t, 2));
        WHEN_SEND_LIST_TO(0, t, LF(2, 3));
        REQUIRE_THAT(t, outputFloat(&t, 1));

        t.setProperty("@start", LF(3));
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_THAT(t, outputFloat(&t, 6));
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_THAT(t, outputFloat(&t, 5));
        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_THAT(t, outputFloat(&t, 6));
        WHEN_SEND_LIST_TO(0, t, LF(4, 3));
        REQUIRE_THAT(t, outputFloat(&t, 3));
        WHEN_SEND_LIST_TO(0, t, LF(2, 3));
        REQUIRE_THAT(t, outputFloat(&t, -1));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_THAT(t, outputFloat(&t, -1));

        return;

        // out of range start
        t.setProperty("@start", LF(100));
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_THAT(t, outputFloat(&t, -1));
        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_THAT(t, outputFloat(&t, -1));

        // last element start
        t.setProperty("@start", LF(7));
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_THAT(t, outputFloat(&t, -1));
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_THAT(t, outputFloat(&t, 7));
        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_THAT(t, outputFloat(&t, -1));

        // out of range @end
        t.setProperty("@start", LF(1));
        t.setProperty("@end", LF(100));
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_THAT(t, outputFloat(&t, 6));
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_THAT(t, outputFloat(&t, 1));
        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_THAT(t, outputFloat(&t, 6));

        // @start == @end
        t.setProperty("@start", LF(2));
        t.setProperty("@end", LF(2));
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_THAT(t, outputFloat(&t, -1));
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_THAT(t, outputFloat(&t, -1));
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_THAT(t, outputFloat(&t, -1));
        WHEN_SEND_FLOAT_TO(0, t, 4);
        REQUIRE_THAT(t, outputFloat(&t, -1));
        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_THAT(t, outputFloat(&t, -1));

        // @start > @end
        t.setProperty("@start", LF(3));
        t.setProperty("@end", LF(2));
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_THAT(t, outputFloat(&t, -1));
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_THAT(t, outputFloat(&t, -1));
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_THAT(t, outputFloat(&t, -1));
        WHEN_SEND_FLOAT_TO(0, t, 4);
        REQUIRE_THAT(t, outputFloat(&t, -1));
        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_THAT(t, outputFloat(&t, -1));
    }
}
