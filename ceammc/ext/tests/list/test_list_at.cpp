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
#include "ceammc_data.h"
#include "list_at.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListAt, list, at)

using TObj = TestListAt;
using TExt = TestExtListAt;
using MA = MListAtom;

TEST_CASE("list.at", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            TObj t("list.at");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_THAT(t, hasProperty(&t, "@index", 0.f));
        }

        SECTION("properties")
        {
            TObj t("list.at", LA("@index", 2));
            REQUIRE_THAT(t, hasProperty(&t, "@index", 2.f));
        }

        SECTION("positional arguments")
        {
            TObj t("list.at", LF(100));
            REQUIRE_THAT(t, hasProperty(&t, "@index", 100));
        }

        SECTION("positional arguments and props mixed")
        {
            TObj t("list.at", LA(100, "@index", 10.9));
            REQUIRE_THAT(t, hasProperty(&t, "@index"));
        }

        SECTION("invalid args")
        {
            SECTION("props")
            {
                TObj t("list.at", LA("@index", "none"));
                REQUIRE_THAT(t, hasProperty(&t, "@index"));
            }

            SECTION("positional")
            {
                TObj t("list.at", LA("none"));
                REQUIRE_THAT(t, hasProperty(&t, "@index"));

                t << LF(1,2,3);
                REQUIRE(listAt(t) == L());
            }
        }

        SECTION("floats")
        {
            TObj t("list.at", LF(1, 2.5, 5));
            REQUIRE_PROPERTY(t, @index, 1, 5);
        }
    }

    SECTION("do")
    {
        SECTION("single")
        {
            TObj t("list.at");

            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_THAT(t, outputFloat(&t, 1));

            t.setProperty("@index", LF(2));
            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_THAT(t, outputFloat(&t, 3));

            WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c", "d"));
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "c");
        }

        SECTION("many")
        {
            TObj t("list.at", LA(1, 1));
            REQUIRE_THAT(t, hasProperty(&t, "@index", 1, 1));

            WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c", "d"));
            REQUIRE_LIST_AT_OUTLET(0, t, LA("b", "b"));

            t.setProperty("@index", LF(-1, 1, 2));
            WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c", "d"));
            REQUIRE_LIST_AT_OUTLET(0, t, LA("d", "b", "c"));
        }
    }

    SECTION("external")
    {
        TExt t("list.at", LF(-1));
        REQUIRE(t->property("@index")->get() == LF(-1));

        t.sendList(LA(1, 2, 3, 4, 5));
        REQUIRE_THAT(t, outputFloat(&t, 5));

        t.sendList(MA(1, 2, 3, -10));
        REQUIRE_THAT(t, outputFloat(&t, -10));
    }

    SECTION("@*?")
    {
        TExt t("list.at");
        t.call("dump");

        // @*? test
        t.call("@*?");
        REQUIRE_THAT(t, hasOutput(&t));
        auto props = t.outputAnyAt(0);
        props.sort();
        REQUIRE(props == LA("@*", "@clip", "@default", "@fold", "@index", "@method", "@rel", "@wrap"));

        // single test
        t.call("@clip?");
        REQUIRE_THAT(t, hasOutput(&t));
        REQUIRE(t.outputAnyAt(0) == LA("@clip", 0.f));

        // multiple test
        t.call("@clip?", LA("@rel?", "@wrap?"));
        REQUIRE_THAT(t, hasOutput(&t));
        REQUIRE(t.outputAnyAt(0) == LA("@clip", 0.f, "@rel", 1, "@wrap", 0.f));

        // multiple test with invalid
        t.call("@clip?", LA("", "@norequest", "non-prop", "@rel?", "@xxx?", 100));
        REQUIRE_THAT(t, hasOutput(&t));
        REQUIRE(t.outputAnyAt(0) == LA("@clip", 0.f, "@rel", 1));
    }

    SECTION("@default")
    {
        TExt t("list.at", LA(2, "@default", "\"???\""));
        REQUIRE_THAT(t, hasProperty(&t, "@index", 2));

        t.sendList(LF(1, 2, 3));
        REQUIRE_THAT(t, hasOutput(&t));
        REQUIRE_THAT(t, outputFloat(&t, 3));

        t.sendList(LF(1, 2));
        REQUIRE_THAT(t, hasOutput(&t));
        REQUIRE(t.outputSymbolAt(0) == S("???"));
    }
}
