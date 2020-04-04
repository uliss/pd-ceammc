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
#include "list_product.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListProduct, list, product)

TEST_CASE("list.product", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestListProduct t("list.product");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        TestExtListProduct t("list.product");

        t << L();
        REQUIRE_FALSE(t.hasOutput());
        t << LA("a", "b");
        REQUIRE_FALSE(t.hasOutput());
        t << "symbol";
        REQUIRE_FALSE(t.hasOutput());

        t << 1;
        REQUIRE(t.outputFloatAt(0) == 1);

        t << LF(2);
        REQUIRE(t.outputFloatAt(0) == 2);
        t << LF(2, 3);
        REQUIRE(t.outputFloatAt(0) == Approx(6));
        t << LA(2, -2, "symbol");
        REQUIRE(t.outputFloatAt(0) == Approx(-4));
    }

    SECTION("mlist")
    {
        TestExtListProduct t("list.product");

        t.send(MLA());
        REQUIRE_THAT(t, !hasOutput(&t));

        t.send(MLA(123));
        REQUIRE_THAT(t, outputFloat(&t, 123));

        t.send(MLA(MLA(), 1, 11));
        REQUIRE_THAT(t, outputFloat(&t, 11));
    }
}
