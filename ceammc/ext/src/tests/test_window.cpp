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
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_window.h"

using namespace ceammc;

TEST_CASE("Window", "[ceammc::ceammc_loader_coreaudio]")
{
    SECTION("fill")
    {
        float data[5] = { 1, 1, 1, 1, 1 };

        REQUIRE_FALSE(window::fill(data, data, window::triangle<float>));
        REQUIRE(window::fill(data, data + 1, window::triangle<float>));
        REQUIRE(data[0] == 0.f);
        REQUIRE(data[1] == 1.f);
        REQUIRE(window::fill(data, data + 2, window::triangle<float>));
        REQUIRE(data[0] == 0.f);
        REQUIRE(data[1] == 0.f);
        REQUIRE(window::fill(data, data + 3, window::triangle<float>));
        REQUIRE(data[0] == 0.f);
        REQUIRE(data[1] == 1.f);
        REQUIRE(data[0] == 0.f);
    }

    SECTION("triangle")
    {
        // empty
        REQUIRE(window::triangle<float>(0, 0) == 0.0f);

        // single
        REQUIRE(window::triangle<float>(0, 1) == 0.0f);

        // 2 elements
        REQUIRE(window::triangle<float>(0, 2) == 0.0f);
        REQUIRE(window::triangle<float>(1, 2) == 0.0f);

        // 3 elements
        REQUIRE(window::triangle<float>(0, 3) == 0.0f);
        REQUIRE(window::triangle<float>(1, 3) == 1.0f);
        REQUIRE(window::triangle<float>(2, 3) == 0.0f);

        // 4 elements
        REQUIRE(window::triangle<float>(0, 4) == 0.0f);
        REQUIRE(window::triangle<float>(1, 4) == Approx(0.66667));
        REQUIRE(window::triangle<float>(2, 4) == Approx(0.66667));
        REQUIRE(window::triangle<float>(3, 4) == 0.0f);

        // 5 elements
        REQUIRE(window::triangle<float>(0, 5) == 0.0f);
        REQUIRE(window::triangle<float>(1, 5) == 0.5f);
        REQUIRE(window::triangle<float>(2, 5) == 1.0f);
        REQUIRE(window::triangle<float>(3, 5) == 0.5f);
        REQUIRE(window::triangle<float>(4, 5) == 0.0f);
    }
}
