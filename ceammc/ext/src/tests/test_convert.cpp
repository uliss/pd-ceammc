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
    }
}
