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
#include "list_mean.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListMean, list, mean)

TEST_CASE("list.mean", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestListMean t("list.mean");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        TestExtListMean t("list.mean");

        t << L();
        REQUIRE_FALSE(t.hasOutput());
        t << LA("a", "b");
        REQUIRE_FALSE(t.hasOutput());
        t << "symbol";
        REQUIRE_FALSE(t.hasOutput());

        t << 100;
        REQUIRE(t.outputFloatAt(0) == 100);

        t << LF(200);
        REQUIRE(t.outputFloatAt(0) == 200);
        t << LF(200, 100);
        REQUIRE(t.outputFloatAt(0) == Approx(150));
        t << LF(200, -200);
        REQUIRE(t.outputFloatAt(0) == Approx(0));
        t << LA(200, -200, "symbol");
        REQUIRE(t.outputFloatAt(0) == Approx(0));
    }

    SECTION("mlist")
    {
        TestExtListMean t("list.mean");

        t.send(MLA());
        REQUIRE_FALSE(t.hasOutput());

        t.send(MLA(123));
        REQUIRE_THAT(t, outputFloat(&t, 123));

        t.send(MLA(MLA(), 1, 11));
        REQUIRE_THAT(t, outputFloat(&t, 6));
    }

    SECTION("alias")
    {
        TestExtListMean t1("list.mean");
        TestExtListMean t2("list.average");

        REQUIRE(t1.object());
        REQUIRE(t2.object());
    }
}
