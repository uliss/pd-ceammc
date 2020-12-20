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
#include "list_remove.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListRemove, list, remove)

using TExt = TestExtListRemove;
using TObj = TestListRemove;

TEST_CASE("list.remove", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("list.remove");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_LIST(t, @indexes, L());
        }

        SECTION("multiple")
        {
            TExt t("list.remove", LF(1, 2, 2, 3, 4, 4, 1));
            REQUIRE_PROPERTY_LIST(t, @indexes, LF(1, 2, 3, 4));
        }
    }

    SECTION("do")
    {
        SECTION("empty")
        {
            TExt t("list.remove");

            t << L();
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1);
            REQUIRE(t.outputListAt(0) == LF(1));

            t << LF(1, 2, 3);
            REQUIRE(t.outputListAt(0) == LF(1, 2, 3));
        }

        SECTION("positive index")
        {
            TExt t("list.remove", LF(1));

            REQUIRE_PROPERTY_LIST(t, @indexes, LF(1));

            t << L();
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1);
            REQUIRE(t.outputListAt(0) == LF(1));

            t << LF(1, 2);
            REQUIRE(t.outputListAt(0) == LF(1));

            t << LF(1, 2, 3);
            REQUIRE(t.outputListAt(0) == LF(1, 3));
        }

        SECTION("negative index")
        {
            TExt t("list.remove", LF(-1));

            REQUIRE_PROPERTY_LIST(t, @indexes, LF(-1));

            t << L();
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1);
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1, 2);
            REQUIRE(t.outputListAt(0) == LF(1));

            t << LF(1, 2, 3);
            REQUIRE(t.outputListAt(0) == LF(1, 2));
        }

        SECTION("negative index")
        {
            TExt t("list.remove", LF(-1));

            t << L();
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1);
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1, 2);
            REQUIRE(t.outputListAt(0) == LF(1));

            t << LF(1, 2, 3);
            REQUIRE(t.outputListAt(0) == LF(1, 2));
        }

        SECTION("list index")
        {
            TExt t("list.remove", LF(0, 2, -1));

            REQUIRE_PROPERTY_LIST(t, @indexes, LF(-1, 0, 2));

            t << L();
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1);
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1, 2);
            REQUIRE(t.outputListAt(0) == L());

            t << LF(1, 2, 3);
            REQUIRE(t.outputListAt(0) == LF(2));

            t << LF(1, 2, 3, 4);
            REQUIRE(t.outputListAt(0) == LF(2));

            t << LF(1, 2, 3, 4, 5);
            REQUIRE(t.outputListAt(0) == LF(2, 4));

            t << LF(1, 2, 3, 4, 5, 6);
            REQUIRE(t.outputListAt(0) == LF(2, 4, 5));
        }
    }

    SECTION("mlist")
    {
        TExt t("list.remove", LF(1));

        t.send(MLA());
        REQUIRE(t.outputAtomAt(0) == MLA());

        t.send(MLA(1));
        REQUIRE(t.outputAtomAt(0) == MLA(1));

        t.send(MLA(1, 2, 3));
        REQUIRE(t.outputAtomAt(0) == MLA(1, 3));

        t.send(MLA(1, 2, 3, 4, 5));
        REQUIRE(t.outputAtomAt(0) == MLA(1, 3, 4, 5));
    }
}
