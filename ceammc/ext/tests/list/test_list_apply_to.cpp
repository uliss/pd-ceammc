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
#include "list_apply_to.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListApplyTo, list, apply_to)

using TObj = TestListApplyTo;
using TExt = TestExtListApplyTo;
using IntA = DataAtom<IntData>;

TEST_CASE("list.apply_to", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TObj t("list.apply_to");
        REQUIRE(t.numInlets() == 3);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE_PROPERTY(t, @indexes);
    }

    SECTION("do")
    {
        TExt t("list.apply_to", LF(0, -1));
        REQUIRE_PROPERTY(t, @indexes, 0, -1);
        External plus("+", 10);

        REQUIRE(t.object());
        REQUIRE(plus.object());

        // [list.apply_to 0 -1] X [+ 10]
        t.connectTo(1, plus, 0);
        t.connectFrom(0, plus, 2);

        t << L();
        REQUIRE(t.outputListAt(0) == L());

        t << 100;
        REQUIRE(t.outputListAt(0) == LF(110));

        t << LF(1, 2, 3, 4);
        REQUIRE(t.outputListAt(0) == LF(11, 2, 3, 14));

        t << LF(1, 2);
        REQUIRE(t.outputListAt(0) == LF(11, 12));
    }

    SECTION("mlist")
    {
        TExt t("list.apply_to", LF(0, -1));
        External plus("*", LF(2));

        REQUIRE(t.object());
        REQUIRE(plus.object());

        // [list.apply_to 0 -1] X [* 2]
        t.connectTo(1, plus, 0);
        t.connectFrom(0, plus, 2);

        t.send(MLA(1));
        REQUIRE(t.outputAtomAt(0) == MLA(2));

        t.send(MLA(1, 3));
        REQUIRE(t.outputAtomAt(0) == MLA(2, 6));

        t.send(MLA(1, 3, 5));
        REQUIRE(t.outputAtomAt(0) == MLA(2, 3, 10));
    }
}
