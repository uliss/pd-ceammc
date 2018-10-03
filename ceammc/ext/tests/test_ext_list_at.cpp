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
#include "../list/list_at.h"
#include "catch.hpp"
#include "datatype_mlist.h"
#include "test_base.h"
#include "test_external.h"

#include <stdio.h>

PD_COMPLETE_TEST_SETUP(ListAt, list, at);

typedef TestExternal<ListAt> ListAtTest;

TEST_CASE("list.at", "[externals]")
{
    pd_test_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            ListAtTest t("list.at");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @index, 0.f);
        }

        SECTION("properties")
        {
            ListAtTest t("list.at", LA("@index", 2));
            REQUIRE_PROPERTY(t, @index, 2.f);
        }

        SECTION("positional arguments")
        {
            ListAtTest t("list.at", LF(100));
            REQUIRE_PROPERTY(t, @index, 100);
        }

        SECTION("positional arguments and props mixed")
        {
            ListAtTest t("list.at", LA(100, "@index", 10));
            REQUIRE_PROPERTY(t, @index, 10);
        }

        SECTION("invalid args")
        {
            SECTION("props")
            {
                ListAtTest t("list.at", LA("@index", "none"));
                REQUIRE_PROPERTY_LIST(t, @index, LA("none"));
            }

            SECTION("positional")
            {
                ListAtTest t("list.at", LA("none"));
                REQUIRE_PROPERTY(t, @index, 0.f);
            }
        }
    }

    SECTION("do")
    {
        SECTION("single")
        {
            ListAtTest t("list.at");

            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            t.setProperty("@index", LF(2));
            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_FLOAT_AT_OUTLET(0, t, 3);

            WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c", "d"));
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "c");
        }

        SECTION("many")
        {
            ListAtTest t("list.at", LA(1, 1, ""));

            WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c", "d"));
            REQUIRE_LIST_AT_OUTLET(0, t, LA("b", "b"));

            t.setProperty("@index", LF(-1, 1, 2));
            WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c", "d"));
            REQUIRE_LIST_AT_OUTLET(0, t, LA("d", "b", "c"));
        }
    }

    SECTION("external")
    {
        TestExtListAt t("list.at", LF(-1));
        REQUIRE(t->property("@index")->get() == LF(-1));

        t.sendList(LA(1, 2, 3, 4, 5));
        REQUIRE(t.outputFloatAt(0) == Approx(5));

        t.send(DataTypeMList("(1 2 3 -10)"));
        REQUIRE(t.outputFloatAt(0) == Approx(-10));
    }

    SECTION("@*?")
    {
        test::pdPrintToStdError(true);

        TestExtListAt t("list.at");
        t.call("dump");

        // @*? test
        t.call("@*?");
        REQUIRE(t.hasOutput());
        auto props = t.outputAnyAt(0);
        props.sort();
        REQUIRE(props == LA("@*", "@clip", "@fold", "@index", "@method", "@rel", "@wrap"));

        // single test
        t.call("@clip?");
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputAnyAt(0) == LA("@clip", 0.f));

        // multiple test
        t.call("@clip?", LA("@rel?", "@wrap?"));
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputAnyAt(0) == LA("@clip", 0.f, "@rel", 1, "@wrap", 0.f));

        // multiple test with invalid
        t.call("@clip?", LA("", "@norequest", "non-prop", "@rel?", "@xxx?", 100));
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputAnyAt(0) == LA("@clip", 0.f, "@rel", 1));
    }
}
