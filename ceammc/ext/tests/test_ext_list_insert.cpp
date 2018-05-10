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
#include "../list/list_insert.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListInsert> ListInsertTest;

TEST_CASE("list.insert", "[externals]")
{
    SECTION("create")
    {
        ListInsertTest t("list.insert");
        REQUIRE(t.numInlets() == 3);
        REQUIRE(t.numOutlets() == 1);

        REQUIRE_PROPERTY_FLOAT(t, @index, 0);

        WHEN_SEND_LIST_TO(2, t, L1(1000));
        REQUIRE_PROPERTY_FLOAT(t, @index, 1000);
    }

    SECTION("empty")
    {
        ListInsertTest t("list.insert");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_SYMBOL_TO(0, t, "ABC");
        REQUIRE_NO_MSG(t);

        DataPtr d0(new IntData(12));
        WHEN_SEND_DATA_TO(0, t, d0);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, AtomList());
        REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 2));
    }

    SECTION("single")
    {
        ListInsertTest t("list.insert", L3(1000, "@index", 1));

        WHEN_SEND_LIST_TO(0, t, AtomList());
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, L1("A"));
        REQUIRE_LIST_AT_OUTLET(0, t, L2("A", 1000));

        WHEN_SEND_LIST_TO(0, t, L2("A", "B"));
        REQUIRE_LIST_AT_OUTLET(0, t, L3("A", 1000, "B"));

        WHEN_SEND_LIST_TO(1, t, L2(-1, -2));
        WHEN_SEND_LIST_TO(0, t, L2("A", "B"));
        REQUIRE_LIST_AT_OUTLET(0, t, L4("A", -1, -2, "B"));

        t.setProperty("@index", L1(2));
        WHEN_SEND_LIST_TO(0, t, L2("A", "B"));
        REQUIRE_LIST_AT_OUTLET(0, t, L4("A", "B", -1, -2));

        t.setProperty("@index", L1(0.f));
        WHEN_SEND_LIST_TO(0, t, L2("A", "B"));
        REQUIRE_LIST_AT_OUTLET(0, t, L4(-1, -2, "A", "B"));

        t.setProperty("@index", L1(-1));
        WHEN_SEND_LIST_TO(0, t, L2("A", "B"));
        REQUIRE_LIST_AT_OUTLET(0, t, L4(-1, -2, "A", "B"));
    }

    SECTION("data")
    {
        DataPtr d1(new IntData(10));
        DataPtr d2(new IntData(20));
        DataPtr d3(new IntData(30));

        ListInsertTest t("list.insert", L2("@index", 1));

        WHEN_SEND_LIST_TO(1, t, L1(d3));
        WHEN_SEND_LIST_TO(0, t, L2(d1, d2));
        REQUIRE_LIST_AT_OUTLET(0, t, L3(d1, d3, d2));
    }
}
