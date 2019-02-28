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
#include "datatype_mlist.h"
#include "../list/list_equal.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListEqual, list, equal);

TEST_CASE("list.equal", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("default")
        {
            TestListEqual t("list.equal");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @pattern, L());
        }

        SECTION("args")
        {
            TestListEqual t("list.equal", LA("a", "b", "@c"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @pattern, LA("a", "b", "@c"));
        }

        SECTION("args + prop")
        {
            TestListEqual t("list.equal", LA("a", "b", "@pattern", 1, 2));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @pattern, LF(1, 2));
        }

        SECTION("prop")
        {
            TestListEqual t("list.equal", LA("@pattern", 1, 2));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @pattern, LF(1, 2));
        }
    }

    SECTION("do")
    {
        TestListEqual t("list.equal");

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(1, t, LF(1, 2));
        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
    }

    SECTION("mlist")
    {
        TestExtListEqual t("list.equal", LF(1, 2));

        t.send(DataTypeMList("()"));
        REQUIRE(t.outputFloatAt(0) == 0);

        t.send(DataTypeMList("(a b)"));
        REQUIRE(t.outputFloatAt(0) == 0);

        t.send(DataTypeMList("(a 2)"));
        REQUIRE(t.outputFloatAt(0) == 0);

        t.send(DataTypeMList("(1 2)"));
        REQUIRE(t.outputFloatAt(0) == 1);

        t.send(DataTypeMList("(1 2 ())"));
        REQUIRE(t.outputFloatAt(0) == 0);

        t.send(DataTypeMList("((1 2))"));
        REQUIRE(t.outputFloatAt(0) == 0);
    }
}
