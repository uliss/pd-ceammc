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
#include "catch.hpp"

#include "base_extension_test.h"
#include "ceammc_fn_list.h"

using namespace ceammc;

TEST_CASE("list functions", "[ceammc::list]")
{
    SECTION("histogram")
    {
        REQUIRE(list::histogram(AtomList(), 0) == AtomList());
        REQUIRE(list::histogram(AtomList(), 10) == AtomList());
        REQUIRE(list::histogram(L1(1), 0) == AtomList());
        REQUIRE(list::histogram(L2(1, 2), 0) == AtomList());

        REQUIRE(list::histogram(L1(1), 1) == L1(1));
        REQUIRE(list::histogram(L2(1, 2), 1) == L1(1));

        REQUIRE(list::histogram(L4(1, 2, 3, 4), 4) == L4(0.25, 0.25, 0.25, 0.25));
        REQUIRE(list::histogram(L4(2, 2, 2, 2), 2) == L2(1, 0.f));
        REQUIRE(list::histogram(L4(1, 2, 2, 2), 2) == L2(0.25, 0.75));
        REQUIRE(list::histogram(L4(1, 1, 2, 2), 2) == L2(0.5, 0.5));
        REQUIRE(list::histogram(L4(1, 1, 1, 2), 2) == L2(0.75, 0.25));
        REQUIRE(list::histogram(L4(1, 1, 1, 1), 2) == L2(1, 0.0));

        REQUIRE(list::histogram(L4(1, 1.1f, 2, 2), 2) == L2(0.5, 0.5));
        REQUIRE(list::histogram(L4(1, 1.1f, 1.9f, 2), 3) == L3(0.5, 0.25, 0.25));
    }
}
