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
#include "../list/list_count.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListCount> ListCountTest;

TEST_CASE("list.count", "[externals]")
{
    setup_list0x2ecount();

    SECTION("init")
    {
        SECTION("empty")
        {
            ListCountTest t("list.count");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_NONE(t, @pattern);
        }

        SECTION("positional symbol")
        {
            ListCountTest t("list.count", L1("c"));
            REQUIRE_PROPERTY(t, @pattern, "c");
        }

        SECTION("positional float")
        {
            ListCountTest t("list.count", L1(2));
            REQUIRE_PROPERTY(t, @pattern, 2);
        }

        SECTION("positional list")
        {
            ListCountTest t("list.count", L5(1, 2, 3, 4, 5));
            REQUIRE_PROPERTY(t, @pattern, 1);
        }

        SECTION("properties")
        {
            ListCountTest t("list.count", L2("@pattern", 111));
            REQUIRE_PROPERTY(t, @pattern, 111);
        }
    }

    SECTION("do")
    {
        ListCountTest t("list.count", L1(2));

        WHEN_SEND_LIST_TO(0, t, L4(1, 3, 4, 5));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, AtomList());
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, L5(1, 2, 4, 5, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);
    }

    SECTION("inlet 2")
    {
        ListCountTest t("list.count", L1(2));
        REQUIRE_PROPERTY(t, @pattern, 2);

        WHEN_SEND_FLOAT_TO(1, t, 122);
        REQUIRE_PROPERTY(t, @pattern, 122);

        WHEN_SEND_SYMBOL_TO(1, t, "A");
        REQUIRE_PROPERTY(t, @pattern, "A");

        WHEN_SEND_LIST_TO(1, t, L3("C", "B", "A"));
        REQUIRE_PROPERTY(t, @pattern, "C");

        WHEN_SEND_LIST_TO(1, t, AtomList());
        REQUIRE_PROPERTY(t, @pattern, "C");
    }
}
