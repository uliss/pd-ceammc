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
#include "list_rotate.h"
#include "test_list_base.h"

#include <stdio.h>

PD_COMPLETE_TEST_SETUP(ListRotate, list, rotate)

using TExt = TestExtListRotate;
using TObj = TestListRotate;

typedef TestExternal<ListRotate> ListRotateTest;

TEST_CASE("list.rotate", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            ListRotateTest t("list.rotate");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_THAT(t, hasProperty(&t, "@step", 1)); 
        }

        SECTION("int")
        {
            ListRotateTest t("list.rotate", LF(12));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_THAT(t, hasProperty(&t, "@step", 12)); 
        }

        SECTION("wrong")
        {
            ListRotateTest t("list.rotate", LA("ABC"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_THAT(t, hasProperty(&t, "@step", 1)); 
        }
    }

    SECTION("rotate")
    {
        ListRotateTest t("list.rotate", LF(3));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(4, 5, 1, 2, 3));

        t.setProperty("@step", LF(-1));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(5, 1, 2, 3, 4));
    }

    SECTION("external")
    {
        SECTION("rotate")
        {
            TExt t("list.rotate");
            REQUIRE(t->property("@step")->get() == LF(1));

            t << LF(1, 2, 3, 4);
            REQUIRE(t.outputListAt(0) == LF(2, 3, 4, 1));

            t->setProperty("@step", LF(-1));
            t << LF(1, 2, 3, 4);
            REQUIRE(t.outputListAt(0) == LF(4, 1, 2, 3));
        }

        SECTION("<<")
        {
            TExt t("list.<<");
            REQUIRE(t->property("@step")->get() == LF(1));

            t << LF(1, 2, 3, 4);
            REQUIRE(t.outputListAt(0) == LF(2, 3, 4, 1));

            t->setProperty("@step", LF(-1));
            t << LF(1, 2, 3, 4);
            REQUIRE(t.outputListAt(0) == LF(4, 1, 2, 3));
        }

        SECTION(">>")
        {
            TExt t("list.>>");
            REQUIRE(t->property("@step")->get() == LF(1));

            t << LF(1, 2, 3, 4);
            REQUIRE(t.outputListAt(0) == LF(4, 1, 2, 3));

            t->setProperty("@step", LF(-1));
            t << LF(1, 2, 3, 4);
            REQUIRE(t.outputListAt(0) == LF(2, 3, 4, 1));
        }
    }
}
