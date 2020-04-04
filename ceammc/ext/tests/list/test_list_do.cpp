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
#include "list_do.h"
#include "test_list_base.h"

#include <stdio.h>

PD_COMPLETE_TEST_SETUP(ListDo, list, do)

using TExt = TestExtListDo;
using TObj = TestListDo;

TEST_CASE("list.do", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("list.do");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("external simple")
    {
        TExt t("list.do");
        pd::External pack("pack", LA("f", "f", "f"));

        // [list.do] X [pack f f f]

        t.connectTo(1, pack, 0);
        t.connectFrom(0, pack, 1);

        t.sendList(L());
        REQUIRE(t.outputListAt(0) == L());
        t.clearAt(0);

        t << LF(10);
        REQUIRE(t.outputListAt(0) == LF(10, 0.f, 1));
        t.clearAt(0);

        t << LF(10, 20);
        REQUIRE(t.outputListAt(0) == LA(10, 0.f, 2, 20, 1, 2));
        t.clearAt(0);
    }
}
