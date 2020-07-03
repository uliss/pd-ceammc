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
#include "list_slice.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListSlice, list, slice)

using TExt = TestExtListSlice;
using TObj = TestListSlice;

TEST_CASE("list.slice", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("list.slice");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        REQUIRE_THAT(t, hasProperty(&t, "@from", 0.f)); 
        REQUIRE_PROPERTY_FLOAT(t, @to, -1);
        REQUIRE_PROPERTY_FLOAT(t, @step, 1);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3));

        SECTION("positional arguments")
        {
            SECTION("single")
            {
                TObj t("list.slice", LF(10));
                REQUIRE_THAT(t, hasProperty(&t, "@from", 10)); 
                REQUIRE_PROPERTY_FLOAT(t, @to, -1);
                REQUIRE_PROPERTY_FLOAT(t, @step, 1);
            }

            SECTION("begin end")
            {
                TObj t("list.slice", LF(10, 20));
                REQUIRE_THAT(t, hasProperty(&t, "@from", 10)); 
                REQUIRE_THAT(t, hasProperty(&t, "@to", 20)); 
                REQUIRE_PROPERTY_FLOAT(t, @step, 1);
            }

            SECTION("begin step")
            {
                TObj t("list.slice", LF(10, 20, 2));
                REQUIRE_THAT(t, hasProperty(&t, "@from", 10)); 
                REQUIRE_THAT(t, hasProperty(&t, "@to", 20)); 
                REQUIRE_THAT(t, hasProperty(&t, "@step", 2)); 
            }
        }

        SECTION("props")
        {
            TObj t("list.slice", LA("@from", 3));
            REQUIRE_THAT(t, hasProperty(&t, "@from", 3)); 
            REQUIRE_PROPERTY_FLOAT(t, @to, -1);
            REQUIRE_PROPERTY_FLOAT(t, @step, 1);
        }

        SECTION("props2")
        {
            TObj t("list.slice", LA("@from", 3, "@to", 5));
            REQUIRE_THAT(t, hasProperty(&t, "@from", 3)); 
            REQUIRE_THAT(t, hasProperty(&t, "@to", 5)); 
            REQUIRE_PROPERTY_FLOAT(t, @step, 1);
        }

        SECTION("props3")
        {
            TObj t("list.slice", LA("@from", 3, "@step", 4, "@to", 5));
            REQUIRE_THAT(t, hasProperty(&t, "@from", 3)); 
            REQUIRE_THAT(t, hasProperty(&t, "@to", 5)); 
            REQUIRE_THAT(t, hasProperty(&t, "@step", 4)); 
        }
    }

    SECTION("split")
    {
        TObj t("list.slice");

        // no params
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(1, 2, 3, 4, 5, 6));

        // from
        t.setProperty("@from", LF(3));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(4, 5, 6));

        // negative from
        t.setProperty("@from", LF(-2));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(5, 6));

        // invalid from
        t.setProperty("@from", LF(40));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        t.setProperty("@from", LF(2));
        t.setProperty("@to", LF(4));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(3, 4, 5));

        // negative @to
        t.setProperty("@from", LF(2));
        t.setProperty("@to", LF(-1));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(3, 4, 5, 6));

        // negative @to
        t.setProperty("@from", LF(2));
        t.setProperty("@to", LF(-1));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(3, 4, 5, 6));

        // negative @from and @to
        t.setProperty("@from", LF(-4));
        t.setProperty("@to", LF(-1));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(3, 4, 5, 6));

        // @step
        t.setProperty("@from", LF(1));
        t.setProperty("@to", LF(4));
        t.setProperty("@step", LF(1));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(2, 3, 4, 5));

        // @step
        t.setProperty("@from", LF(0.f));
        t.setProperty("@to", LF(4));
        t.setProperty("@step", LF(2));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 3, 5));

        // @step
        t.setProperty("@from", LF(1));
        t.setProperty("@to", LF(6));
        t.setProperty("@step", LF(3));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(2, 5));

        // invalid @step
        t.setProperty("@from", LF(1));
        t.setProperty("@to", LF(4));
        t.setProperty("@step", LF(0.f));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        // invalid @step
        t.setProperty("@from", LF(3));
        t.setProperty("@to", LF(5));
        t.setProperty("@step", LF(-2));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        // invalid @step
        t.setProperty("@from", LF(3));
        t.setProperty("@to", Atom());
        t.setProperty("@step", Atom());
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L());
    }
}
