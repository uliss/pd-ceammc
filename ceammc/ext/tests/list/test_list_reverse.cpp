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
#include "list_reverse.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListReverse, list, reverse)

using TExt = TestExtListReverse;
using TObj = TestListReverse;

TEST_CASE("list.reverse", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TObj t("list.reverse");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        TExt t("list.reverse");

        t << L();
        REQUIRE(t.outputListAt(0) == L());

        t << LF(1);
        REQUIRE(t.outputListAt(0) == LF(1));

        t << LF(1, 2);
        REQUIRE(t.outputListAt(0) == LF(2, 1));

        t << LA(1, 2, "A", "B");
        REQUIRE(t.outputListAt(0) == LA("B", "A", 2, 1));

        t.send(MLA());
        REQUIRE(t.outputAtomAt(0) == MLA());

        t.send(MLA(1, 2, 3));
        REQUIRE(t.outputAtomAt(0) == MLA(3, 2, 1));

        t.send(MLA(MLA(), MLA(), MLA()));
        REQUIRE(t.outputAtomAt(0) == MLA(MLA(), MLA(), MLA()));

        t.send(MLA(MLA("A", "B", "C"), MLA(1, 2, 3), MLA("\"with spaces\"")));
        REQUIRE(t.outputAtomAt(0) == MLA(MLA("\"with spaces\""), MLA(1, 2, 3), MLA("A", "B", "C")));
    }
}
