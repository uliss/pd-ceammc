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
#include "../list/list_rotate.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListRotate> ListRotateTest;

TEST_CASE("list.rotate", "[externals]")
{
    SECTION("init")
    {
        SECTION("empty")
        {
            ListRotateTest t("list.rotate");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @step, 1);
        }

        SECTION("int")
        {
            ListRotateTest t("list.rotate", L1(12));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @step, 12);
        }

        SECTION("wrong")
        {
            ListRotateTest t("list.rotate", L1("ABC"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @step, 1);
        }
    }

    SECTION("rotate")
    {
        ListRotateTest t("list.rotate", L1(3));
        WHEN_SEND_LIST_TO(0, t, L5(1, 2, 3, 4, 5));
        REQUIRE_LIST_AT_OUTLET(0, t, L5(4, 5, 1, 2, 3));

        t.setProperty("@step", L1(-1));
        WHEN_SEND_LIST_TO(0, t, L5(1, 2, 3, 4, 5));
        REQUIRE_LIST_AT_OUTLET(0, t, L5(5, 1, 2, 3, 4));
    }
}
