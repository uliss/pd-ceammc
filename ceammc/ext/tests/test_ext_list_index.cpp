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
#include "../list/list_index.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListIndex> Test;

TEST_CASE("list.index", "[externals]")
{
    SECTION("create")
    {
        SECTION("empty")
        {
            Test t("list.index");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @start, 0);
            REQUIRE_PROPERTY_FLOAT(t, @end, -1);
        }

        SECTION("props")
        {
            Test t("list.index", L4("@start", 1, "@end", 100));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @start, 1);
            REQUIRE_PROPERTY_FLOAT(t, @end, 100);
        }
    }

    SECTION("search")
    {
        Test t("list.index", L8(1, 2, 3, 4, 3, 2, 1, 2));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 10000);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);

        WHEN_SEND_FLOAT_TO(0, t, 10000);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);
        WHEN_SEND_LIST_TO(0, t, L2(2, 3));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        t.setProperty("@start", L1(3));
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 6);
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 5);
        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 6);
        WHEN_SEND_LIST_TO(0, t, L2(4, 3));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 3);
        WHEN_SEND_LIST_TO(0, t, L2(2, 3));
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);

        WHEN_SEND_LIST_TO(0, t, AtomList());
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);

        // out of range start
        t.setProperty("@start", L1(100));
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);
        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);

        // last element start
        t.setProperty("@start", L1(7));
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 7);
        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);

        // out of range @end
        t.setProperty("@start", L1(1));
        t.setProperty("@end", L1(100));
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 6);
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 6);

        // @start == @end
        t.setProperty("@start", L1(2));
        t.setProperty("@end", L1(2));
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);
        WHEN_SEND_FLOAT_TO(0, t, 4);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);
        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);

        // @start > @end
        t.setProperty("@start", L1(3));
        t.setProperty("@end", L1(2));
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);
        WHEN_SEND_FLOAT_TO(0, t, 4);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);
        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, -1);
    }
}
