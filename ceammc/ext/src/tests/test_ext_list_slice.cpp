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
#include "../list/list_slice.h"
#include "base_extension_test.h"
#include "catch.hpp"

typedef TestExtension<ListSlice> ListSliceTest;

TEST_CASE("list.slice", "[externals]")
{
    obj_init();

    SECTION("init")
    {
        ListSliceTest t("list.slice");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        REQUIRE_PROPERTY(t, @from, 0.f);
        REQUIRE_PROPERTY_NONE(t, @to);
        REQUIRE_PROPERTY_NONE(t, @step);

        WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(0, t, L3(1, 2, 3));

        SECTION("positional arguments")
        {
            SECTION("single")
            {
                ListSliceTest t("list.slice", L1(10));
                REQUIRE_PROPERTY(t, @from, 10);
                REQUIRE_PROPERTY_NONE(t, @to);
                REQUIRE_PROPERTY_NONE(t, @step);
            }

            SECTION("begin end")
            {
                ListSliceTest t("list.slice", L2(10, 20));
                REQUIRE_PROPERTY(t, @from, 10);
                REQUIRE_PROPERTY(t, @to, 20);
                REQUIRE_PROPERTY_NONE(t, @step);
            }

            SECTION("begin step")
            {
                ListSliceTest t("list.slice", L3(10, 20, 2));
                REQUIRE_PROPERTY(t, @from, 10);
                REQUIRE_PROPERTY(t, @to, 20);
                REQUIRE_PROPERTY(t, @step, 2);
            }
        }

        SECTION("props")
        {
            ListSliceTest t("list.slice", L2("@from", 3));
            REQUIRE_PROPERTY(t, @from, 3);
            REQUIRE_PROPERTY_NONE(t, @to);
            REQUIRE_PROPERTY_NONE(t, @step);
        }

        SECTION("props2")
        {
            ListSliceTest t("list.slice", L4("@from", 3, "@to", 5));
            REQUIRE_PROPERTY(t, @from, 3);
            REQUIRE_PROPERTY(t, @to, 5);
            REQUIRE_PROPERTY_NONE(t, @step);
        }

        SECTION("props3")
        {
            ListSliceTest t("list.slice", L6("@from", 3, "@step", 4, "@to", 5));
            REQUIRE_PROPERTY(t, @from, 3);
            REQUIRE_PROPERTY(t, @to, 5);
            REQUIRE_PROPERTY(t, @step, 4);
        }
    }

    SECTION("split")
    {
        ListSliceTest t("list.slice");

        // no params
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L6(1, 2, 3, 4, 5, 6));

        // from
        t.setProperty("@from", L1(3));
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L3(4, 5, 6));

        // negative from
        t.setProperty("@from", L1(-2));
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L2(5, 6));

        // invalid from
        t.setProperty("@from", L1(40));
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

        t.setProperty("@from", L1(2));
        t.setProperty("@to", L1(4));
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L2(3, 4));

        // negative @to
        t.setProperty("@from", L1(2));
        t.setProperty("@to", L1(-1));
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L3(3, 4, 5));

        // negative @to
        t.setProperty("@from", L1(2));
        t.setProperty("@to", L1(-1));
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L3(3, 4, 5));

        // negative @from and @to
        t.setProperty("@from", L1(-4));
        t.setProperty("@to", L1(-1));
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L3(3, 4, 5));

        // @step
        t.setProperty("@from", L1(1));
        t.setProperty("@to", L1(4));
        t.setProperty("@step", L1(1));
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L3(2, 3, 4));

        // @step
        t.setProperty("@from", L1(0.f));
        t.setProperty("@to", L1(4));
        t.setProperty("@step", L1(2));
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 3));

        // @step
        t.setProperty("@from", L1(1));
        t.setProperty("@to", L1(6));
        t.setProperty("@step", L1(3));
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L2(2, 5));

        // invalid @step
        t.setProperty("@from", L1(1));
        t.setProperty("@to", L1(4));
        t.setProperty("@step", L1(0.f));
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L3(2, 3, 4));

        // invalid @step
        t.setProperty("@from", L1(3));
        t.setProperty("@to", L1(5));
        t.setProperty("@step", L1(-2));
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));
        REQUIRE_LIST_AT_OUTLET(0, t, L2(4, 5));
    }
}
