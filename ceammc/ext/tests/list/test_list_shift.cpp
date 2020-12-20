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
#include "list_shift.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListShift, list, shift)

using TExt = TestExtListShift;
using TObj = TestListShift;

TEST_CASE("list.shift", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("list.shift");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @shift, 0);
        }

        SECTION("args")
        {
            TObj t("list.shift", LF(20));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @shift, 20);
        }

        SECTION("props")
        {
            TObj t("list.shift", LA("@shift", -10));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @shift, -10);
        }
    }

    SECTION("do")
    {
        TExt t("list.shift", LF(0.4));

        t << L();
        REQUIRE(t.outputListAt(0) == L());

        t << LA("a", "b");
        REQUIRE(t.outputListAt(0) == LX(0, 0));

        t << LF(1);
        REQUIRE(t.outputListAt(0) == LX(1));

        t << LF(1, 2);
        REQUIRE(t.outputListAt(0) == LX(1.4, 2));

        t.send(MLA());
        REQUIRE(t.outputAtomAt(0) == MLA());

        t.send(MLA(1, 2));
        REQUIRE(t.outputAtomAt(0) == MLA(1.4, 2));
    }
}
