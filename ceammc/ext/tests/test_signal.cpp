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
#include "ceammc.h"
#include "ceammc_convert.h"
#include "ceammc_signal.h"

using namespace ceammc;

typedef SmoothLinT<float> SmoothTest;

TEST_CASE("convert", "[PureData]")
{
    SECTION("smooth")
    {
        SmoothTest t(0, 0, 10);

        REQUIRE(t.step() == Approx(0));
        REQUIRE(t.current() == Approx(0));
        REQUIRE(t.target() == 0);

        for (size_t i = 0; i < 100; i++) {
            REQUIRE(t() == Approx(0));
        }

        t.setTargetValue(10);
        REQUIRE(t.current() == Approx(0));
        REQUIRE(t.step() == Approx(1));
        REQUIRE(t.target() == Approx(10));

        REQUIRE(t() == Approx(1));
        REQUIRE(t() == Approx(2));
        REQUIRE(t() == Approx(3));
        REQUIRE(t() == Approx(4));
        REQUIRE(t() == Approx(5));
        REQUIRE(t() == Approx(6));
        REQUIRE(t() == Approx(7));
        REQUIRE(t() == Approx(8));
        REQUIRE(t() == Approx(9));
        REQUIRE(t() == Approx(10));
        REQUIRE(t() == Approx(10));

        t.setTargetValue(5);
        REQUIRE(t.target() == Approx(5));
        REQUIRE(t.current() == Approx(10));
        REQUIRE(t.step() == Approx(-0.5));
        REQUIRE(t() == Approx(9.5));
        REQUIRE(t() == Approx(9));
        REQUIRE(t() == Approx(8.5));
        REQUIRE(t() == Approx(8));
        REQUIRE(t() == Approx(7.5));
        REQUIRE(t() == Approx(7));
        REQUIRE(t() == Approx(6.5));
        REQUIRE(t() == Approx(6));
        REQUIRE(t() == Approx(5.5));
        REQUIRE(t() == Approx(5));
        REQUIRE(t() == Approx(5));

        t.setTargetValue(5.1);
        REQUIRE(t() == Approx(5.01));
        REQUIRE(t() == Approx(5.02));
        REQUIRE(t() == Approx(5.03));
        REQUIRE(t() == Approx(5.04));
        REQUIRE(t() == Approx(5.05));
        REQUIRE(t() == Approx(5.06));
        REQUIRE(t() == Approx(5.07));
        REQUIRE(t() == Approx(5.08));
        REQUIRE(t() == Approx(5.09));
        REQUIRE(t() == Approx(5.1));
        REQUIRE(t() == Approx(5.1));

        t.setTargetValue(-10);
        REQUIRE(t.target() == Approx(-10));
        REQUIRE(t.current() == Approx(5.1));
        REQUIRE(t.step() == Approx(-1.51));

        REQUIRE(t() == Approx(3.59f));
        REQUIRE(t() == Approx(2.08f));
        REQUIRE(t() == Approx(0.57f));
        REQUIRE(t() == Approx(-0.94f));
        REQUIRE(t() == Approx(-2.45));

        for (size_t i = 0; i < 10; i++)
            t();

        REQUIRE(t() == Approx(-10));
    }
}
