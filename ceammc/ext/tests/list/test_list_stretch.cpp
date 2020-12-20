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
#include "list_stretch.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListStretch, list, stretch)

using TExt = TestExtListStretch;
using TObj = TestListStretch;

TEST_CASE("list.stretch", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("list.stretch");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @size, 1);
        }

        SECTION("args")
        {
            TObj t("list.stretch", { 4 });
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @size, 4);
        }

        SECTION("props")
        {
            TObj t("list.stretch", LA("@size", 3));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @size, 3);
        }

        SECTION("invalid")
        {
            TObj t("list.stretch", LA("@size", -2));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @size, 1);
        }

        SECTION("invalid")
        {
            TObj t("list.stretch", { 0 });
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @size, 1);
        }
    }

    SECTION("do")
    {
        TExt t("list.stretch", { 3 });

        t << L();
        REQUIRE_THAT(t, !hasOutput(&t));;
        t << 1;
        REQUIRE_THAT(t, !hasOutput(&t));;

        t << LF(1);
        REQUIRE(t.outputListAt(0) == LF(1, 1, 1));
        t << LF(1, 2);
        REQUIRE(t.outputListAt(0) == LX(1, 1.5, 2));
        t << LF(1, 2, 3);
        REQUIRE(t.outputListAt(0) == LX(1, 2, 3));
    }

    SECTION("mlist")
    {
        TExt t("list.stretch", { 3 });

        t.send(MLA());
        REQUIRE_THAT(t, !hasOutput(&t));;

        t.send(MLA(1));
        REQUIRE(t.outputAtomAt(0) == MLA(1, 1, 1));

        t.send(MLA(1, 2));
        REQUIRE(t.outputAtomAt(0) == MLA(1, 1.5, 2));
    }
}
