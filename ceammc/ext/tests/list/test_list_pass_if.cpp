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
#include "list_pass_if.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListPassIf, list, pass_if)

using TExt = TestExtListPassIf;
using TObj = TestListPassIf;

TEST_CASE("list.pass_if", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TObj t("list.pass_if");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("do")
    {
        TExt t("list.pass_if");

        External less("<", LF(20));
        t.connectTo(1, less, 0);
        t.connectFrom(0, less, 1);

        t << L();
        REQUIRE(t.outputListAt(0) == L());

        t << LF(10, 12);
        REQUIRE(t.outputListAt(0) == LF(10, 12));

        t << LF(10, 19, 20, 21);
        REQUIRE(t.outputListAt(0) == LF(10, 19));

        t << LA("A", "B", "C");
        REQUIRE(t.outputListAt(0) == L());

        t.send(MLA());
        REQUIRE(t.outputAtomAt(0) == MLA());

        t.send(MLA(18, 19, 20, 21, MLA(22), MLA(-1)));
        REQUIRE(t.outputAtomAt(0) == MLA(18, 19));
    }

    SECTION("alias")
    {
        TExt t1("list.pass_if");
        TExt t2("list.filter");

        REQUIRE(t1.object());
        REQUIRE(t2.object());
    }
}
