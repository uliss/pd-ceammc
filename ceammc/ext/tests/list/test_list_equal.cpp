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
#include "list_equal.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListEqual, list, equal)

using TExt = TestExtListEqual;
using TObj = TestListEqual;

TEST_CASE("list.equal", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("list.equal");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @pattern, L());
        }

        SECTION("args")
        {
            TObj t("list.equal", LA("a", "b", "@c"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_THAT(t, hasProperty(&t, "@pattern", "a", "b"));
        }

        SECTION("args + prop")
        {
            TObj t("list.equal", LA("a", "b", "@pattern", 1, 2));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_THAT(t, hasProperty(&t, "@pattern", 1, 2));
        }

        SECTION("prop")
        {
            TObj t("list.equal", LA("@pattern", 1, 2));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_THAT(t, hasProperty(&t, "@pattern", 1, 2));
        }
    }

    SECTION("do")
    {
        TObj t("list.equal");

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_THAT(t, outputFloat(&t, 1));

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_THAT(t, outputFloat(&t, 0));

        WHEN_SEND_LIST_TO(1, t, LF(1, 2));
        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_THAT(t, outputFloat(&t, 1));
    }

    SECTION("mlist")
    {
        TExt t("list.equal", LF(1, 2));

        t.send(MLA());
        REQUIRE_THAT(t, outputFloat(&t, 0));

        t.send(MLA("a", "b"));
        REQUIRE_THAT(t, outputFloat(&t, 0));

        t.send(MLA("a", "2"));
        REQUIRE_THAT(t, outputFloat(&t, 0));

        t.send(MLA(1, 2));
        REQUIRE_THAT(t, outputFloat(&t, 1));

        t.send(MLA(1, 2, MLA()));
        REQUIRE_THAT(t, outputFloat(&t, 0));

        t.send(MLA(1, 2, MLA()));
        REQUIRE_THAT(t, outputFloat(&t, 0));
    }
}
