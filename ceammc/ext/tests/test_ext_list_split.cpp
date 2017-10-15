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
#include "../list/list_split.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListSplit> ListSplitTest;

TEST_CASE("list.split", "[externals]")
{
    SECTION("init")
    {
        ListSplitTest t("list.split");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE_PROPERTY(t, @at, 0.f);
    }

    SECTION("do")
    {
        ListSplitTest t("list.split", L1(2));
        REQUIRE_PROPERTY(t, @at, 2);

        WHEN_SEND_LIST_TO(0, t, L4(1, 2, 3, 4));
        REQUIRE_LIST_AT_OUTLET(1, t, L2(3, 4));
        REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 2));

        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_LIST_AT_OUTLET(1, t, AtomList());
        REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 2));

        WHEN_SEND_LIST_TO(0, t, L1(1));
        REQUIRE_LIST_AT_OUTLET(1, t, AtomList());
        REQUIRE_LIST_AT_OUTLET(0, t, L1(1));

        WHEN_SEND_LIST_TO(0, t, AtomList());
        REQUIRE_LIST_AT_OUTLET(1, t, AtomList());
        REQUIRE_LIST_AT_OUTLET(0, t, AtomList());
    }
}
