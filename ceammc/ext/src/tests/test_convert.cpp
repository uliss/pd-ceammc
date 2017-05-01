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

#include "../base/debug_gensym.h"
#include "catch.hpp"
#include "ceammc.hpp"
#include "ceammc_convert.h"

using namespace ceammc::convert;

// strange link fix: do not remove!
t_symbol* t = gensym("test");

TEST_CASE("convert", "[PureData]")
{
    SECTION("time")
    {
        SECTION("sec2str")
        {
            using namespace time;
            REQUIRE(sec2str(0) == "00:00:00");
            REQUIRE(sec2str(1) == "00:00:01");
            REQUIRE(sec2str(59) == "00:00:59");
            REQUIRE(sec2str(60) == "00:01:00");
            REQUIRE(sec2str(61) == "00:01:01");
            REQUIRE(sec2str(601) == "00:10:01");
            REQUIRE(sec2str(3599) == "00:59:59");
            REQUIRE(sec2str(3600) == "01:00:00");
            REQUIRE(sec2str(3601) == "01:00:01");
            REQUIRE(sec2str(3661) == "01:01:01");
            REQUIRE(sec2str(3600 * 12) == "12:00:00");
            REQUIRE(sec2str(3600 * 24 + 60 * 3 + 1) == "00:03:01");
            REQUIRE(sec2str(-59) == "-00:00:59");

            REQUIRE(sec2str(0.5, true) == "00:00:00.500");
            REQUIRE(sec2str(-0.5, true) == "-00:00:00.500");
            REQUIRE(sec2str(0.999f, true) == "00:00:00.999");
            REQUIRE(sec2str(0.1111f, true) == "00:00:00.111");
            REQUIRE(sec2str(0, true) == "00:00:00.000");
        }

        SECTION("str2sec")
        {
            using namespace time;
            REQUIRE(str2sec("00:00:00") == 0.f);
            REQUIRE(str2sec("00:00") == 0.f);
            REQUIRE(str2sec("0") == 0.f);
            REQUIRE(str2sec("1") == 1.f);
            REQUIRE(str2sec("01") == 1.f);
            REQUIRE(str2sec("0:01") == 1.f);
            REQUIRE(str2sec("0:01") == 1.f);
            REQUIRE(str2sec("00:01") == 1.f);
            REQUIRE(str2sec("0:00:01") == 1.f);
            REQUIRE(str2sec("00:00:01") == 1.f);
            REQUIRE(str2sec("00:10:01") == 601.f);
            REQUIRE(str2sec("1:00:01") == 3601.f);

            REQUIRE(str2sec("-1") == -1.f);
            REQUIRE(str2sec("-0:01") == -1.f);
            REQUIRE(str2sec("-0:01") == -1.f);

            REQUIRE(str2sec("1.999") == Approx(1.999));
            REQUIRE(str2sec("01.999") == Approx(1.999));
            REQUIRE(str2sec("0:01.999") == Approx(1.999));
            REQUIRE(str2sec("00:01.999") == Approx(1.999));
            REQUIRE(str2sec("0:00:01.999") == Approx(1.999));
            REQUIRE(str2sec("00:00:01.999") == Approx(1.999));

            REQUIRE(str2sec("-01.999") == Approx(-1.999));
            REQUIRE(str2sec("-0:01.999") == Approx(-1.999));
            REQUIRE(str2sec("-00:01.999") == Approx(-1.999));
            REQUIRE(str2sec("-0:00:01.999") == Approx(-1.999));
            REQUIRE(str2sec("-00:00:01.999") == Approx(-1.999));

            for (int i = -100; i < 3600 * 24; i += 24) {
                REQUIRE(str2sec(sec2str(i)) == Approx(i));
            }

            SECTION("invalid")
            {
                float INV = -1.f;
                REQUIRE(str2sec("invalid", INV) == INV);
                REQUIRE(str2sec(".123", INV) == INV);
                REQUIRE(str2sec("111:111:111", INV) == INV);
                REQUIRE(str2sec("1111:999", INV) == INV);
                REQUIRE(str2sec("00:00.999999", INV) == INV);
                REQUIRE(str2sec("1:22:33:44", INV) == INV);
                REQUIRE(str2sec("11:22:33:44.555", INV) == INV);
            }
        }
    }

    SECTION("lin2lin")
    {
        REQUIRE(lin2lin<float>(0, 0, 1, 0, 127) == 0.f);
        REQUIRE(lin2lin<float>(0, 1, 0, 127, 0) == 0.f);
        REQUIRE(lin2lin<float>(1, 1, 0, 127, 0) == 127.f);
        REQUIRE(lin2lin<float>(1, 0, 1, 0, 127) == 127.f);
        REQUIRE(lin2lin<float>(0.5, 0, 1, 0, 127) == 63.5f);
        REQUIRE(lin2lin<float>(0.5, 1, 0, 0, 127) == 63.5f);
        REQUIRE(lin2lin<float>(0.5, 0, 1, 127, 0) == 63.5f);
        REQUIRE(lin2lin<float>(0.5, 1, 0, 127, 0) == 63.5f);

        REQUIRE(lin2lin<float>(0, 1, 0, 0, 127) == 127.f);
        REQUIRE(lin2lin<float>(1, 1, 0, 0, 127) == 0.f);
        REQUIRE(lin2lin<float>(-1, -1, 1, 0, 10) == 0.f);
        REQUIRE(lin2lin<float>(1, -1, 1, 0, 10) == 10.f);
        REQUIRE(lin2lin<float>(0, -1, 1, 0, 10) == 5.f);
        REQUIRE(lin2lin<float>(0, -1, 1, -10, 10) == 0.f);
        REQUIRE(lin2lin<float>(0, 0, 1, -10, 10) == -10.f);
        REQUIRE(lin2lin<float>(0, 1, -1, -10, 10) == 0.f);
        REQUIRE(lin2lin<float>(1, 1, -1, -10, 10) == -10.f);
        REQUIRE(lin2lin<float>(-1, 1, -1, -10, 10) == 10.f);
        REQUIRE(lin2lin<float>(-2, -1, -3, -2, -6) == -4.f);

        REQUIRE(lin2lin<float>(1, 10, 1, 20, 0) == 0.f);

        REQUIRE(lin2lin<float>(0.5, 0, 1, 1, 11) == 6.f);
        REQUIRE(lin2lin<float>(0.5, 1, 0, 1, 11) == 6.f);
        REQUIRE(lin2lin<float>(0.5, 0, 1, 11, 1) == 6.f);
        REQUIRE(lin2lin<float>(0.5, 1, 0, 11, 1) == 6.f);

        REQUIRE(lin2lin<float>(0.5, 0, 1, -1, -11) == -6.f);
        REQUIRE(lin2lin<float>(0.5, 1, 0, -1, -11) == -6.f);
        REQUIRE(lin2lin<float>(0.5, 0, 1, -11, -1) == -6.f);
        REQUIRE(lin2lin<float>(0.5, 1, 0, -11, -1) == -6.f);

        REQUIRE(lin2lin<float>(0.5, 0, 1, 0, -10) == -5.f);
        REQUIRE(lin2lin<float>(0.5, 1, 0, 0, -10) == -5.f);
        REQUIRE(lin2lin<float>(0.5, 0, 1, -10, 0) == -5.f);
        REQUIRE(lin2lin<float>(0.5, 1, 0, -10, 0) == -5.f);

        {
            REQUIRE(lin2lin<float>(0, 0, 1, 100, 200) == 100.f);
            REQUIRE(lin2lin<float>(1, 0, 1, 100, 200) == 200.f);
            REQUIRE(lin2lin<float>(0.3f, 0, 1, 100, 200) == 130.f);

            REQUIRE(lin2lin<float>(0, -1, 0, 100, 200) == 200.f);
            REQUIRE(lin2lin<float>(-1, -1, 0, 100, 200) == 100.f);
            REQUIRE(lin2lin<float>(-0.2f, -1, 0, 100, 200) == 180.f);

            REQUIRE(lin2lin<float>(-1, -1, 1, 100, 200) == 100.f);
            REQUIRE(lin2lin<float>(0, -1, 1, 100, 200) == 150.f);
            REQUIRE(lin2lin<float>(1, -1, 1, 100, 200) == 200.f);

            REQUIRE(lin2lin<float>(-1, -1, 1, 100, 200) == 100.f);
            REQUIRE(lin2lin<float>(0, -1, 1, 100, 200) == 150.f);
            REQUIRE(lin2lin<float>(1, -1, 1, 100, 200) == 200.f);

            REQUIRE(lin2lin<float>(-1, -2, -1, 100, 200) == 200.f);
            REQUIRE(lin2lin<float>(-1.7f, -2, -1, 100, 200) == 130.f);
            REQUIRE(lin2lin<float>(-2, -2, -1, 100, 200) == 100.f);

            REQUIRE(lin2lin<float>(0, 0, 1, 200, 100) == 200.f);
            REQUIRE(lin2lin<float>(1, 0, 1, 200, 100) == 100.f);
            REQUIRE(lin2lin<float>(0.3f, 0, 1, 200, 100) == 170.f);

            REQUIRE(lin2lin<float>(0, 1, 0, 200, 100) == 100.f);
            REQUIRE(lin2lin<float>(1, 1, 0, 200, 100) == 200.f);
            REQUIRE(lin2lin<float>(0.3f, 1, 0, 200, 100) == 130.f);
        }
    }

    SECTION("lin2exp")
    {
#define L2E(x, x0, x1, y0, y1, v) \
    REQUIRE(double(lin2exp(float(x), float(x0), float(x1), float(y0), float(y1))) == Approx(v));

        L2E(0, 0, 1, 0.01, 1, 0.01);
        L2E(1, 0, 1, 0.01, 1, 1);

        // all positive
        L2E(0, 0, 10, 0.01, 1, 0.01);
        L2E(1, 0, 10, 0.01, 1, 0.015848931924611);
        L2E(2, 0, 10, 0.01, 1, 0.025118864315096);
        L2E(3, 0, 10, 0.01, 1, 0.03981071705535);
        L2E(4, 0, 10, 0.01, 1, 0.063095734448019);
        L2E(5, 0, 10, 0.01, 1, 0.1);
        L2E(6, 0, 10, 0.01, 1, 0.15848931924611);
        L2E(7, 0, 10, 0.01, 1, 0.25118864315096);
        L2E(8, 0, 10, 0.01, 1, 0.3981071705535);
        L2E(9, 0, 10, 0.01, 1, 0.63095734448019);
        L2E(10, 0, 10, 0.01, 1, 1);

        // input mixed - output positive
        L2E(-5, -5, 5, 0.01, 1, 0.01);
        L2E(-4, -5, 5, 0.01, 1, 0.015848931924611);
        L2E(-3, -5, 5, 0.01, 1, 0.025118864315096);
        L2E(-2, -5, 5, 0.01, 1, 0.03981071705535);
        L2E(-1, -5, 5, 0.01, 1, 0.063095734448019);
        L2E(0, -5, 5, 0.01, 1, 0.1);
        L2E(1, -5, 5, 0.01, 1, 0.15848931924611);
        L2E(2, -5, 5, 0.01, 1, 0.25118864315096);
        L2E(3, -5, 5, 0.01, 1, 0.3981071705535);
        L2E(4, -5, 5, 0.01, 1, 0.63095734448019);
        L2E(5, -5, 5, 0.01, 1, 1);

        // input negative - output positive
        L2E(-5, -5, -1, 0.01, 1, 0.01);
        L2E(-4, -5, -1, 0.01, 1, 0.031622776601684);
        L2E(-3, -5, -1, 0.01, 1, 0.1);
        L2E(-2, -5, -1, 0.01, 1, 0.31622776601684);
        L2E(-1, -5, -1, 0.01, 1, 1);

        // input negative - output negative
        L2E(-5, -5, -1, -0.01, -1, -0.01);
        L2E(-4, -5, -1, -0.01, -1, -0.031622776601684);
        L2E(-3, -5, -1, -0.01, -1, -0.1);
        L2E(-2, -5, -1, -0.01, -1, -0.31622776601684);
        L2E(-1, -5, -1, -0.01, -1, -1);

        // input positive - output negative
        L2E(1, 1, 5, -0.01, -1, -0.01);
        L2E(2, 1, 5, -0.01, -1, -0.031622776601684);
        L2E(3, 1, 5, -0.01, -1, -0.1);
        L2E(4, 1, 5, -0.01, -1, -0.31622776601684);
        L2E(5, 1, 5, -0.01, -1, -1);
    }

    SECTION("exp2lin")
    {
#define E2L(x, x0, x1, y0, y1, v) REQUIRE(double(exp2lin(float(x), float(x0), float(x1), float(y0), float(y1))) == Approx(v));

        E2L(1, 1, 5, 0, 1, 0);
        E2L(2, 1, 5, 0, 1, 0.43067655807339);
        E2L(3, 1, 5, 0, 1, 0.68260619448599);
        E2L(4, 1, 5, 0, 1, 0.86135311614679);
        E2L(5, 1, 5, 0, 1, 1);

        // input positive - output negative
        E2L(1, 1, 5, 0, -1, -0);
        E2L(2, 1, 5, 0, -1, -0.43067655807339);
        E2L(3, 1, 5, 0, -1, -0.68260619448599);
        E2L(4, 1, 5, 0, -1, -0.86135311614679);
        E2L(5, 1, 5, 0, -1, -1);
    }

    SECTION("lin2curve")
    {
#define L2C(x, x0, x1, y0, y1, c, v) REQUIRE(double(lin2curve(float(x), float(x0), float(x1), float(y0), float(y1), float(c))) == Approx(v));

        L2C(1, 1, 5, 0, 1, -4, 0);
        L2C(2, 1, 5, 0, 1, -4, 0.64391425988797);
        L2C(3, 1, 5, 0, 1, -4, 0.88079707797788);
        L2C(4, 1, 5, 0, 1, -4, 0.96794139671992);
        L2C(5, 1, 5, 0, 1, -4, 1);

        L2C(0, 0, 4, 0, 1, -4, 0);
        L2C(1, 0, 4, 0, 1, -4, 0.64391425988797);
        L2C(2, 0, 4, 0, 1, -4, 0.88079707797788);
        L2C(3, 0, 4, 0, 1, -4, 0.96794139671992);
        L2C(4, 0, 4, 0, 1, -4, 1);

        L2C(-2, -2, 2, 0, 1, -4, 0);
        L2C(-1, -2, 2, 0, 1, -4, 0.64391425988797);
        L2C(0, -2, 2, 0, 1, -4, 0.88079707797788);
        L2C(1, -2, 2, 0, 1, -4, 0.96794139671992);
        L2C(2, -2, 2, 0, 1, -4, 1);

        L2C(-2, -2, 2, 0, 1, -3, 0);
        L2C(-1, -2, 2, 0, 1, -3, 0.5552791692202);
        L2C(+0, -2, 2, 0, 1, -3, 0.81757447619364);
        L2C(+1, -2, 2, 0, 1, -3, 0.94147400614883);
        L2C(+2, -2, 2, 0, 1, -3, 1);

        L2C(-2, -2, 2, 0, 1, -1, 0);
        L2C(-1, -2, 2, 0, 1, -1, 0.34993200875877);
        L2C(+0, -2, 2, 0, 1, -1, 0.62245933120185);
        L2C(+1, -2, 2, 0, 1, -1, 0.83470382332888);
        L2C(+2, -2, 2, 0, 1, -1, 1);

        L2C(-2, -2, 2, -1, 1, 0, -1);
        L2C(-1, -2, 2, -1, 1, 0, -0.5);
        L2C(+0, -2, 2, -1, 1, 0, 0);
        L2C(+1, -2, 2, -1, 1, 0, 0.5);
        L2C(+2, -2, 2, -1, 1, 0, 1);

        L2C(-2, -2, 2, -1, 1, 3, -1);
        L2C(-1, -2, 2, -1, 1, 3, -0.88294801229767);
        L2C(+0, -2, 2, -1, 1, 3, -0.63514895238729);
        L2C(+1, -2, 2, -1, 1, 3, -0.1105583384404);
        L2C(+2, -2, 2, -1, 1, 3, 1);
    }
}
