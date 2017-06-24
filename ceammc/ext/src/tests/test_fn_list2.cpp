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

    SECTION("normalizeByRange")
    {
        AtomList res;
        REQUIRE_FALSE(list::normalizeByRange(AtomList(), res));
        REQUIRE_FALSE(list::normalizeByRange(L1(0.f), res));
        REQUIRE_FALSE(list::normalizeByRange(L1(10), res));
        REQUIRE_FALSE(list::normalizeByRange(L1(-10), res));
        REQUIRE_FALSE(list::normalizeByRange(L4(1, 1, 1, 1), res));
        REQUIRE(res.empty());

#define REQUIRE_NORMALIZE(l1, dest)               \
    {                                             \
        AtomList res;                             \
        REQUIRE(list::normalizeByRange(l1, res)); \
        REQUIRE(res == dest);                     \
    }

        REQUIRE_NORMALIZE(L2(0.f, 1), L2(0.f, 1));
        REQUIRE_NORMALIZE(L2(0.f, 10), L2(0.f, 1));
        REQUIRE_NORMALIZE(L5(1, 2, 3, 4, 5), L5(0.f, 0.25, 0.5, 0.75, 1));
        REQUIRE_NORMALIZE(L5(-1, -2, -3, -4, -5), L5(1, 0.75, 0.5, 0.25, 0.f));
        REQUIRE_NORMALIZE(L3(-20, 0.0, 20), L3(0.0, 0.5, 1.0));
    }

    SECTION("enumerate")
    {
        REQUIRE(list::enumerate(AtomList()) == AtomList());
        REQUIRE(list::enumerate(L1(100)) == L2(0.0, 100));
        REQUIRE(list::enumerate(L1(100), 20) == L2(20, 100));
        REQUIRE(list::enumerate(L1(100), -20) == L2(-20, 100));

        REQUIRE(list::enumerate(L1(100), 20, list::PREPEND) == L2(20, 100));
        REQUIRE(list::enumerate(L1(100), -20, list::PREPEND) == L2(-20, 100));

        REQUIRE(list::enumerate(L1(100), 20, list::APPEND) == L2(100, 20));
        REQUIRE(list::enumerate(L1(100), -20, list::APPEND) == L2(100, -20));
    }

    SECTION("unique")
    {
        REQUIRE(list::unique(AtomList()) == AtomList());
        REQUIRE(list::unique(L1(100)) == L1(100));
        REQUIRE(list::unique(L2(1, 2)) == L2(1, 2));
        REQUIRE(list::unique(L6(1, 2, 3, 1, 3, 2)) == L3(1, 2, 3));
        REQUIRE(list::unique(L4(1, 1, 1, 1)) == L1(1));
    }
}
