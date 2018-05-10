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
#include "../list/list_prepend.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListPrepend> ListPrependTest;

TEST_CASE("list.prepend", "[externals]")
{
    SECTION("create")
    {
        ListPrependTest t("list.prepend");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("empty")
    {
        ListPrependTest t("list.prepend");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_LIST_AT_OUTLET(0, t, L1(10));

        WHEN_SEND_SYMBOL_TO(0, t, "ABC");
        REQUIRE_LIST_AT_OUTLET(0, t, L1("ABC"));

        DataPtr d0(new IntData(12));
        WHEN_SEND_DATA_TO(0, t, d0);
        REQUIRE_LIST_AT_OUTLET(0, t, L1(d0));

        WHEN_SEND_LIST_TO(0, t, AtomList());
        REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 2));
    }

    SECTION("args")
    {
        ListPrependTest t("list.prepend", L2("@prop", 1));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, L2("@prop", 1));

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_LIST_AT_OUTLET(0, t, L3("@prop", 1, 10));

        WHEN_SEND_SYMBOL_TO(0, t, "ABC");
        REQUIRE_LIST_AT_OUTLET(0, t, L3("@prop", 1, "ABC"));

        DataPtr d0(new IntData(12));
        WHEN_SEND_DATA_TO(0, t, d0);
        REQUIRE_LIST_AT_OUTLET(0, t, L3("@prop", 1, d0));

        WHEN_SEND_LIST_TO(0, t, AtomList());
        REQUIRE_LIST_AT_OUTLET(0, t, L2("@prop", 1));

        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, L4("@prop", 1, 1, 2));

        WHEN_SEND_LIST_TO(1, t, L1(100));
        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, L3(100, 1, 2));
    }

    SECTION("data")
    {
        DataPtr d1(new IntData(10));
        DataPtr d2(new IntData(20));
        DataPtr d3(new IntData(30));

        ListPrependTest t("list.prepend");

        WHEN_SEND_LIST_TO(1, t, L2(d1, d2));
        WHEN_SEND_DATA_TO(0, t, d3);
        REQUIRE_LIST_AT_OUTLET(0, t, L3(d1, d2, d3));
    }
}
