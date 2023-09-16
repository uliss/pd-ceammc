/*****************************************************************************
 * Copyright 2023 Serge Poltavsky. All rights reserved.
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
#include "ceammc_random.h"
#include "test_macro.h"

#include "catch.hpp"

using namespace ceammc;
using namespace ceammc::random;

TEST_CASE("random", "[core]")
{
    SECTION("RandomGen")
    {
        RandomGen gen;
        auto gen_ptr = &gen.get();

        gen.setSeed(0);
        REQUIRE(&gen.get() != gen_ptr);
        auto gen_ptr1 = &gen.get();

        gen.setSeed(1);
        REQUIRE(&gen.get() == gen_ptr1);

#ifdef __APPLE__
        gen.setSeed(1);
        REQUIRE(gen.gen_uniform_uint(0, 2) == 1);
        REQUIRE(gen.gen_uniform_uint(0, 2) == 0);
        REQUIRE(gen.gen_uniform_uint(0, 2) == 0);

        gen.setSeed(1);
        REQUIRE(gen.gen_uniform_uint(0, 2) == 1);
        REQUIRE(gen.gen_uniform_uint(0, 2) == 0);
        REQUIRE(gen.gen_uniform_uint(0, 2) == 0);

        REQUIRE(gen.gen_uniform_float(0, 100) == Approx(0.01144).margin(0.001));
#endif
    }

    SECTION("SeedProperty")
    {
        RandomGen gen;
        auto gen_ptr = &gen.get();

        SeedProperty p(gen);
        REQUIRE(p.name() == gensym("@seed"));
        REQUIRE(p.isReadWrite());
        REQUIRE(p.defaultValue() == 0);

#ifdef __APPLE__
        p.set(LF(1));
        REQUIRE(gen.gen_uniform_uint(0, 2) == 1);
        REQUIRE(gen.gen_uniform_uint(0, 2) == 0);
        REQUIRE(gen.gen_uniform_uint(0, 2) == 0);
        REQUIRE(gen.gen_uniform_uint(0, 2) == 1);

        p.set(LF(1));
        REQUIRE(gen.gen_uniform_uint(0, 2) == 1);
        REQUIRE(gen.gen_uniform_uint(0, 2) == 0);
        REQUIRE(gen.gen_uniform_uint(0, 2) == 0);
#endif
    }
}
