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
#include "../data/datatype_mlist.h"
#include "../list/list_remove.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListRemove, list, remove);

TEST_CASE("list.remove", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestListRemove t("list.remove");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        SECTION("empty")
        {
            TestExtListRemove t("list.remove");

            t << L();
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1);
            REQUIRE(t.outputFloatAt(0) == 1);

            t << LF(1, 2, 3);
            REQUIRE(t.outputListAt(0) == LF(1, 2, 3));
        }

        SECTION("positive index")
        {
            TestExtListRemove t("list.remove", LF(1));

            t << L();
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1);
            REQUIRE(t.outputFloatAt(0) == 1);

            t << LF(1, 2);
            REQUIRE(t.outputFloatAt(0) == 1);

            t << LF(1, 2, 3);
            REQUIRE(t.outputListAt(0) == LF(1, 3));
        }

        SECTION("negative index")
        {
            TestExtListRemove t("list.remove", LF(-1));

            t << L();
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1);
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1, 2);
            REQUIRE(t.outputFloatAt(0) == 1);

            t << LF(1, 2, 3);
            REQUIRE(t.outputListAt(0) == LF(1, 2));
        }

        SECTION("negative index")
        {
            TestExtListRemove t("list.remove", LF(-1));

            t << L();
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1);
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1, 2);
            REQUIRE(t.outputFloatAt(0) == 1);

            t << LF(1, 2, 3);
            REQUIRE(t.outputListAt(0) == LF(1, 2));
        }

        SECTION("list index")
        {
            TestExtListRemove t("list.remove", LF(0, 2, -1));

            t << L();
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1);
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1, 2);
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1, 2, 3);
            REQUIRE(t.outputFloatAt(0) == 2);

            t << LF(1, 2, 3, 4);
            REQUIRE(t.outputFloatAt(0) == 2);

            t << LF(1, 2, 3, 4, 5);
            REQUIRE(t.outputListAt(0) == LF(2, 4));

            t << LF(1, 2, 3, 4, 5, 6);
            REQUIRE(t.outputListAt(0) == LF(2, 4, 5));
        }
    }

    SECTION("mlist")
    {
        TestExtListRemove t("list.remove", LF(1));

        t.send(DataTypeMList());
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList()));

        t.send(DataTypeMList("(1)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(1)")));

        t.send(DataTypeMList("(1 2 3)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(1 3)")));

        t.send(DataTypeMList("(1 2 3 4 5)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(1 3 4 5)")));
    }
}
