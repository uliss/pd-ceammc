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
#include "list_split.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListSplit, list, split)

using TExt = TestExtListSplit;
using TObj = TestListSplit;

TEST_CASE("list.split", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("list.split");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE_THAT(t, hasProperty(&t, "@at", 0.f)); 
    }

    SECTION("do")
    {
        TObj t("list.split", LF(2));
        REQUIRE_THAT(t, hasProperty(&t, "@at", 2)); 

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
        REQUIRE_LIST_AT_OUTLET(1, t, LF(3, 4));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2));

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(1, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2));

        WHEN_SEND_LIST_TO(0, t, LF(1));
        REQUIRE_LIST_AT_OUTLET(1, t, L());
        REQUIRE_THAT(t, outputFloat(&t, 1));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_LIST_AT_OUTLET(1, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, L());
    }

    SECTION("mlist")
    {
        TExt t("list.split", LF(3));

        t.send(MLA());
        REQUIRE(t.outputAtomAt(1) == MLA());
        REQUIRE(t.outputAtomAt(0) == MLA());

        t.send(MLA(1));
        REQUIRE(t.outputAtomAt(0) == MLA(1));
        REQUIRE(t.outputAtomAt(1) == MLA());

        t.send(MLA(1, 2));
        REQUIRE(t.outputAtomAt(0) == MLA(1, 2));
        REQUIRE(t.outputAtomAt(1) == MLA());

        t.send(MLA(1, 2, 3));
        REQUIRE(t.outputAtomAt(0) == MLA(1, 2, 3));
        REQUIRE(t.outputAtomAt(1) == MLA());

        t.send(MLA(1, 2, 3, 4));
        REQUIRE(t.outputAtomAt(0) == MLA(1, 2, 3));
        REQUIRE(t.outputAtomAt(1) == MLA(4));

        t.send(MLA(1, 2, 3, 4, 5));
        REQUIRE(t.outputAtomAt(0) == MLA(1, 2, 3));
        REQUIRE(t.outputAtomAt(1) == MLA(4, 5));
    }
}
