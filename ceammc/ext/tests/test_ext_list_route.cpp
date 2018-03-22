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
#include "../list/list_route.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListRoute> ListRouteTest;

TEST_CASE("list.route", "[externals]")
{
    obj_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            ListRouteTest t("list.route", AtomList());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @trim, 0.f);
            REQUIRE_PROPERTY(t, @as_any, 0.f);
            REQUIRE_PROPERTY(t, @simplify, 1);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_SEND_FLOAT_TO(0, t, 200);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 200);

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "ABC");

            WHEN_SEND_LIST_TO(0, t, L2(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 2));
        }

        SECTION("patterns")
        {
            ListRouteTest t("list.route", L3(1, 2, 3));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 4);

            REQUIRE_PROPERTY(t, @trim, 0.f);
        }

        SECTION("patterns")
        {
            ListRouteTest t("list.route", L4(1, 2, 3, "@trim"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 4);

            REQUIRE_PROPERTY(t, @trim, 1);
        }
    }

    SECTION("floats")
    {
        SECTION("default")
        {
            ListRouteTest t("list.route", L2(200, 300));

            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

            WHEN_SEND_FLOAT_TO(0, t, 200);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 200);

            WHEN_SEND_FLOAT_TO(0, t, 300);
            REQUIRE_FLOAT_AT_OUTLET(1, t, 300);
        }

        SECTION("trim")
        {
            ListRouteTest t("list.route", L3(200, 300, "@trim"));

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
            ListRouteTest t("list.route", L2("A", "B"));

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");

            WHEN_SEND_SYMBOL_TO(0, t, "B");
            REQUIRE_SYMBOL_AT_OUTLET(1, t, "B");

            WHEN_SEND_SYMBOL_TO(0, t, "C");
            REQUIRE_SYMBOL_AT_OUTLET(2, t, "C");
        }

        SECTION("trim")
        {
            ListRouteTest t("list.route", L3("A", "B", "@trim"));

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
            ListRouteTest t("list.route", L2("A", "B"));

            WHEN_SEND_LIST_TO(0, t, L3("A", "B", "C"));
            REQUIRE_LIST_AT_OUTLET(0, t, L3("A", "B", "C"));

            WHEN_SEND_LIST_TO(0, t, L3("B", "C", "D"));
            REQUIRE_LIST_AT_OUTLET(1, t, L3("B", "C", "D"));

            WHEN_SEND_LIST_TO(0, t, L3("C", "B", "A"));
            REQUIRE_LIST_AT_OUTLET(2, t, L3("C", "B", "A"));

            // short lists
            WHEN_SEND_LIST_TO(0, t, L1("A"));
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");

            WHEN_SEND_LIST_TO(0, t, L1("C"));
            REQUIRE_LIST_AT_OUTLET(2, t, L1("C"));
        }

        SECTION("as any")
        {
            ListRouteTest t("list.route", L4("A", "B", 100, "@as_any"));
            REQUIRE_PROPERTY(t, @as_any, 1);

            // no action on floats
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

            // only symbols and lists
            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_ANY_AT_OUTLET(0, t, L1("A"));

            WHEN_SEND_LIST_TO(0, t, L3("A", "B", "C"));
            REQUIRE_ANY_AT_OUTLET(0, t, L3("A", "B", "C"));

            WHEN_SEND_LIST_TO(0, t, L2("B", "C"));
            REQUIRE_ANY_AT_OUTLET(1, t, L2("B", "C"));
        }

        SECTION("as any + trim")
        {
            ListRouteTest t("list.route", L5("A", "B", 100, "@as_any", "@trim"));
            REQUIRE_PROPERTY(t, @as_any, 1);

            // no action on floats and symbols
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_BANG_AT_OUTLET(2, t);

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_BANG_AT_OUTLET(0, t);

            // only lists
            WHEN_SEND_LIST_TO(0, t, L3("A", "B", "C"));
            REQUIRE_ANY_AT_OUTLET(0, t, L2("B", "C"));

            WHEN_SEND_LIST_TO(0, t, L2("B", "C"));
            REQUIRE_ANY_AT_OUTLET(1, t, L1("C"));

            // with @simplify
            t.setProperty("@simplify", L1(1));

            // no action on floats and symbols
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_BANG_AT_OUTLET(2, t);

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_BANG_AT_OUTLET(0, t);

            // only lists
            WHEN_SEND_LIST_TO(0, t, L3("A", "B", "C"));
            REQUIRE_ANY_AT_OUTLET(0, t, L2("B", "C"));

            WHEN_SEND_LIST_TO(0, t, L2("B", "C"));
            REQUIRE_ANY_AT_OUTLET(1, t, L1("C"));
        }

        SECTION("simplify")
        {
            ListRouteTest t("list.route", L5("A", "B", 100, "@simplify", 1));
            REQUIRE_PROPERTY(t, @simplify, 1);
            REQUIRE_PROPERTY(t, @trim, 0.f);

            // no action on floats and symbols
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");

            WHEN_SEND_LIST_TO(0, t, L3("A", "B", "C"));
            REQUIRE_LIST_AT_OUTLET(0, t, L3("A", "B", "C"));

            // only one element lists
            WHEN_SEND_LIST_TO(0, t, L1(100));
            REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

            WHEN_SEND_LIST_TO(0, t, L1("B"));
            REQUIRE_SYMBOL_AT_OUTLET(1, t, "B");
        }

        SECTION("simplify + as_any")
        {
            ListRouteTest t("list.route", L6("A", "B", 100, "@simplify", 1, "@as_any"));
            REQUIRE_PROPERTY(t, @simplify, 1);
            REQUIRE_PROPERTY(t, @trim, 0.f);

            // no action on floats
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

            WHEN_SEND_SYMBOL_TO(0, t, "B");
            REQUIRE_ANY_AT_OUTLET(1, t, L1("B"));

            WHEN_SEND_LIST_TO(0, t, L3("A", "B", "C"));
            REQUIRE_ANY_AT_OUTLET(0, t, L3("A", "B", "C"));

            // only one element lists
            WHEN_SEND_LIST_TO(0, t, L1(100));
            REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

            WHEN_SEND_LIST_TO(0, t, L1("B"));
            REQUIRE_ANY_AT_OUTLET(1, t, L1("B"));
        }
    }
}
