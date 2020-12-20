/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "local_list.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(LocalList, local, list)

TEST_CASE("local.list", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        TestLocalList t("local.list");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numInlets() == 1);
    }

    SECTION("do")
    {
        TestExtLocalList t1("local.list");
        REQUIRE(t1.object());

        TestExtLocalList t2("local.list");
        REQUIRE(t2.object());

        t1 << BANG;
        REQUIRE(t1.outputListAt(0) == L());

        t2 << BANG;
        REQUIRE(t2.outputListAt(0) == L());

        t1 << LA("a", "b", "c");
        REQUIRE(t1.outputListAt(0) == LA("a", "b", "c"));

        t2 << BANG;
        REQUIRE(t2.outputListAt(0) == LA("a", "b", "c"));

        t2.send(MLA(1, 2, 3, 4, 5));
        REQUIRE(t2.outputListAt(0) == LX(1, 2, 3, 4, 5));

        t1 << BANG;
        REQUIRE(t1.outputListAt(0) == LX(1, 2, 3, 4, 5));

        t1 << "SYMBOL";
        REQUIRE(t1.outputListAt(0) == LA("SYMBOL"));

        t1 << 1000.f;
        REQUIRE_FALSE(t1.hasOutput());
    }

    SECTION("methods")
    {
        test::pdPrintToStdError(true);

        TestExtLocalList t("local.list", LA("lst", 1, 2));
        REQUIRE(t.object());

        t << BANG;
        REQUIRE(t.outputListAt(0) == L());

        t <<= LA("set", 1, 2, 3, 4, 5, 6, 7, 8, 9);
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3, 4, 5, 6, 7, 8, 9));

        t <<= LA("reverse");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(9, 8, 7, 6, 5, 4, 3, 2, 1));

        t <<= LA("shuffle");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) != LF(9, 8, 7, 6, 5, 4, 3, 2, 1));

        t <<= LA("sort");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3, 4, 5, 6, 7, 8, 9));

        t <<= LA("pop");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3, 4, 5, 6, 7, 8));

        // no remove position
        t <<= LA("remove");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3, 4, 5, 6, 7, 8));

        // invalid remove position
        t <<= LA("remove", -100);
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3, 4, 5, 6, 7, 8));
        t <<= LA("remove", 9);
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3, 4, 5, 6, 7, 8));
        t <<= LA("remove", 8);
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3, 4, 5, 6, 7, 8));

        // valid position
        t <<= LA("remove", 0.f);
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(2, 3, 4, 5, 6, 7, 8));

        // valid position
        t <<= LA("remove", 6);
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(2, 3, 4, 5, 6, 7));

        // negative offset
        t <<= LA("remove", -1);
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(2, 3, 4, 5, 6));

        // fill
        t <<= LA("fill", 20);
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(20, 20, 20, 20, 20));

        t <<= LA("clear");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == L());

        // invalid
        t <<= LA("pop");
        REQUIRE_FALSE(t.hasOutput());
    }
}
