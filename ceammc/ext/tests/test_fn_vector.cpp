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
#include "ceammc_fn_vector.h"

using namespace ceammc;

TEST_CASE("vector functions", "[ceammc::vector]")
{
    SECTION("magnitude")
    {
        AtomList l;
        REQUIRE(vector::magnitude(l) == 0.0);
        l.append(gensym("a"));
        REQUIRE(vector::magnitude(l) == 0.0);

        l.append(-3.f);
        l.append(4.f);
        l.append(gensym("b"));
        REQUIRE(vector::magnitude(l) == 5.0);
    }

    SECTION("dotProduct")
    {
        AtomList a;
        AtomList b;

        REQUIRE(vector::dotProduct(a, b) == 0.0);
        REQUIRE(vector::dotProduct(b, a) == 0.0);

        a.append(1.0);
        REQUIRE(vector::dotProduct(a, b) == 0.0);
        REQUIRE(vector::dotProduct(b, a) == 0.0);

        b.append(3.0);
        REQUIRE(vector::dotProduct(a, b) == 3.0);
        REQUIRE(vector::dotProduct(b, a) == 3.0);

        a = AtomList::values(4, 1.0, 2.0, 3.0, 4.0);
        b = AtomList::values(4, 5.0, 6.0, 7.0, 8.0);
        REQUIRE(vector::dotProduct(a, b) == 70.0);
        REQUIRE(vector::dotProduct(b, a) == 70.0);

        b.clear();
        REQUIRE(vector::dotProduct(a, b) == 0.0);
        b = AtomList::values(2, 5.0, 6.0);
        REQUIRE(vector::dotProduct(a, b) == 17.0);

        a.clear();
        b.clear();

        a.append(gensym("a"));
        a.append(gensym("b"));
        b.append(gensym("c"));
        b.append(gensym("d"));
        REQUIRE(vector::dotProduct(a, b) == 0.0);
        a.append(10.f);
        b.append(20.f);
        REQUIRE(vector::dotProduct(a, b) == 200.);
    }

    SECTION("distance")
    {
        AtomList a;
        AtomList b;

        REQUIRE(vector::distance(a, b) == 0.0);
        REQUIRE(vector::distance(b, a) == 0.0);

        a.append(1.f);
        REQUIRE(vector::distance(a, b) == 0.f);
        REQUIRE(vector::distance(b, a) == 0.f);

        a.clear();
        b.clear();
        a.append(1.f);
        b.append(0.f);
        REQUIRE(vector::distance(a, b) == 1.f);
        REQUIRE(vector::distance(b, a) == 1.f);

        a.clear();
        b.clear();
        a.append(1.f);
        b.append(1.f);
        REQUIRE(vector::distance(a, b) == 0.f);
        REQUIRE(vector::distance(b, a) == 0.f);

        a.clear();
        b.clear();
        a.append(1.f);
        a.append(10.f);
        b.append(1.f);
        b.append(12.f);
        REQUIRE(vector::distance(a, b) == 2.f);
        REQUIRE(vector::distance(b, a) == 2.f);

        a.clear();
        b.clear();
        a.append(10.f);
        b.append(20.f);
        REQUIRE(vector::distance(a, b) > 0);
        REQUIRE(vector::distance(b, a) > 0);

        a.clear();
        b.clear();
        a.append(3.f);
        a.append(8.f);
        b.append(6.f);
        b.append(12.f);
        REQUIRE(vector::distance(a, b) == 5.0);
        REQUIRE(vector::distance(b, a) == 5.0);

        REQUIRE(vector::distance(a, a) == 0.0);

        a.clear();
        b.clear();
        a.append(-10.f);
        b.append(-20.f);
        REQUIRE(vector::distance(a, b) > 0);
        REQUIRE(vector::distance(b, a) > 0);
    }
}
