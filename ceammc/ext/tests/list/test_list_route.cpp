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
#include "list_route.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListRoute, list, route)

using TExt = TestExtListRoute;
using TObj = TestListRoute;

typedef TestExternal<ListRoute> ListRouteTest;

TEST_CASE("list.route", "[externals]")
{
    pd_test_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            ListRouteTest t("list.route", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_THAT(t, hasProperty(&t, "@trim", 0.f)); 
            REQUIRE_THAT(t, hasProperty(&t, "@as_any", 0.f)); 
            REQUIRE_THAT(t, hasProperty(&t, "@simplify", 1)); 

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_SEND_FLOAT_TO(0, t, 200);
            REQUIRE_THAT(t, outputFloat(&t, 200));

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "ABC");

            WHEN_SEND_LIST_TO(0, t, LF(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2));
        }

        SECTION("patterns")
        {
            ListRouteTest t("list.route", LF(1, 2, 3));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 4);

            REQUIRE_THAT(t, hasProperty(&t, "@trim", 0.f)); 
        }

        SECTION("patterns")
        {
            ListRouteTest t("list.route", LA(1, 2, 3, "@trim"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 4);

            REQUIRE_THAT(t, hasProperty(&t, "@trim", 1)); 
        }
    }

    SECTION("floats")
    {
        SECTION("default")
        {
            ListRouteTest t("list.route", LF(200, 300));

            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

            WHEN_SEND_FLOAT_TO(0, t, 200);
            REQUIRE_THAT(t, outputFloat(&t, 200));

            WHEN_SEND_FLOAT_TO(0, t, 300);
            REQUIRE_FLOAT_AT_OUTLET(1, t, 300);
        }

        SECTION("trim")
        {
            ListRouteTest t("list.route", LA(200, 300, "@trim"));

            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

            WHEN_SEND_FLOAT_TO(0, t, 200);
            REQUIRE_BANG_AT_OUTLET(0, t);

            WHEN_SEND_FLOAT_TO(0, t, 300);
            REQUIRE_BANG_AT_OUTLET(1, t);
        }
    }

    SECTION("symbols")
    {
        SECTION("default")
        {
            ListRouteTest t("list.route", LA("A", "B"));

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");

            WHEN_SEND_SYMBOL_TO(0, t, "B");
            REQUIRE_SYMBOL_AT_OUTLET(1, t, "B");

            WHEN_SEND_SYMBOL_TO(0, t, "C");
            REQUIRE_SYMBOL_AT_OUTLET(2, t, "C");
        }

        SECTION("trim")
        {
            ListRouteTest t("list.route", LA("A", "B", "@trim"));

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_BANG_AT_OUTLET(0, t);

            WHEN_SEND_SYMBOL_TO(0, t, "B");
            REQUIRE_BANG_AT_OUTLET(1, t);

            WHEN_SEND_SYMBOL_TO(0, t, "C");
            REQUIRE_SYMBOL_AT_OUTLET(2, t, "C");
        }
    }

    SECTION("lists")
    {
        SECTION("default")
        {
            ListRouteTest t("list.route", LA("A", "B"));

            WHEN_SEND_LIST_TO(0, t, LA("A", "B", "C"));
            REQUIRE_LIST_AT_OUTLET(0, t, LA("A", "B", "C"));

            WHEN_SEND_LIST_TO(0, t, LA("B", "C", "D"));
            REQUIRE_LIST_AT_OUTLET(1, t, LA("B", "C", "D"));

            WHEN_SEND_LIST_TO(0, t, LA("C", "B", "A"));
            REQUIRE_LIST_AT_OUTLET(2, t, LA("C", "B", "A"));

            // short lists
            WHEN_SEND_LIST_TO(0, t, LA("A"));
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");

            WHEN_SEND_LIST_TO(0, t, LA("C"));
            REQUIRE_SYMBOL_AT_OUTLET(2, t, "C");
        }

        SECTION("as any")
        {
            ListRouteTest t("list.route", LA("A", "B", 100, "@as_any"));
            REQUIRE_THAT(t, hasProperty(&t, "@as_any", 1)); 

            // no action on floats
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

            // only symbols and lists
            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_ANY_AT_OUTLET(0, t, LA("A"));

            WHEN_SEND_LIST_TO(0, t, LA("A", "B", "C"));
            REQUIRE_ANY_AT_OUTLET(0, t, LA("A", "B", "C"));

            WHEN_SEND_LIST_TO(0, t, LA("B", "C"));
            REQUIRE_ANY_AT_OUTLET(1, t, LA("B", "C"));
        }

        SECTION("as any + trim")
        {
            ListRouteTest t("list.route", LA("A", "B", 100, "@as_any", "@trim"));
            REQUIRE_THAT(t, hasProperty(&t, "@as_any", 1)); 

            // no action on floats and symbols
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_BANG_AT_OUTLET(2, t);

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_BANG_AT_OUTLET(0, t);

            // only lists
            WHEN_SEND_LIST_TO(0, t, LA("A", "B", "C"));
            REQUIRE_ANY_AT_OUTLET(0, t, LA("B", "C"));

            WHEN_SEND_LIST_TO(0, t, LA("B", "C"));
            REQUIRE_ANY_AT_OUTLET(1, t, LA("C"));

            // with @simplify
            t.setProperty("@simplify", LF(1));

            // no action on floats and symbols
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_BANG_AT_OUTLET(2, t);

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_BANG_AT_OUTLET(0, t);

            // only lists
            WHEN_SEND_LIST_TO(0, t, LA("A", "B", "C"));
            REQUIRE_ANY_AT_OUTLET(0, t, LA("B", "C"));

            WHEN_SEND_LIST_TO(0, t, LA("B", "C"));
            REQUIRE_ANY_AT_OUTLET(1, t, LA("C"));
        }

        SECTION("simplify")
        {
            ListRouteTest t("list.route", LA("A", "B", 100, "@simplify", 1));
            REQUIRE_THAT(t, hasProperty(&t, "@simplify", 1)); 
            REQUIRE_THAT(t, hasProperty(&t, "@trim", 0.f)); 

            // no action on floats and symbols
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");

            WHEN_SEND_LIST_TO(0, t, LA("A", "B", "C"));
            REQUIRE_LIST_AT_OUTLET(0, t, LA("A", "B", "C"));

            // only one element lists
            WHEN_SEND_LIST_TO(0, t, LF(100));
            REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

            WHEN_SEND_LIST_TO(0, t, LA("B"));
            REQUIRE_SYMBOL_AT_OUTLET(1, t, "B");
        }

        SECTION("simplify + as_any")
        {
            ListRouteTest t("list.route", LA("A", "B", 100, "@simplify", 1, "@as_any"));
            REQUIRE_THAT(t, hasProperty(&t, "@simplify", 1)); 
            REQUIRE_THAT(t, hasProperty(&t, "@trim", 0.f)); 

            // no action on floats
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

            WHEN_SEND_SYMBOL_TO(0, t, "B");
            REQUIRE_ANY_AT_OUTLET(1, t, LA("B"));

            WHEN_SEND_LIST_TO(0, t, LA("A", "B", "C"));
            REQUIRE_ANY_AT_OUTLET(0, t, LA("A", "B", "C"));

            // only one element lists
            WHEN_SEND_LIST_TO(0, t, LF(100));
            REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

            WHEN_SEND_LIST_TO(0, t, LA("B"));
            REQUIRE_ANY_AT_OUTLET(1, t, LA("B"));
        }
    }

    SECTION("mlist")
    {
        ListRouteTest t("list.route", LA("A", "B", 1000, "@trim"));

        WHEN_SEND_DATA_TO(0, t, MLD());
        REQUIRE_THAT(t, !hasOutput(&t));

        WHEN_SEND_TDATA_TO(0, t, MLD("A", 1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3));

        WHEN_SEND_TDATA_TO(0, t, MLD("B", 3, 4, 5));
        REQUIRE_LIST_AT_OUTLET(1, t, LF(3, 4, 5));

        WHEN_SEND_TDATA_TO(0, t, MLD(1000, 1001, 1002));
        REQUIRE_LIST_AT_OUTLET(2, t, LF(1001, 1002));

        WHEN_SEND_TDATA_TO(0, t, MLD(1, 2));
        REQUIRE_LIST_AT_OUTLET(3, t, LF(1, 2));
    }
}
