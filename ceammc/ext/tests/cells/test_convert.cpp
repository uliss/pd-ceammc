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
#include "ceammc_convert.h"

#include <cmath>

using namespace ceammc::convert;

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

    SECTION("lin2lin_clip")
    {
#define L2L_CLIP(x, x0, x1, y0, y1, v) \
    REQUIRE(double(lin2lin_clip(float(x), float(x0), float(x1), float(y0), float(y1))) == Approx(v));

        L2L_CLIP(0, 0, 1, 0, 1, 0);
        L2L_CLIP(1, 0, 1, 0, 1, 1);

        L2L_CLIP(0, 0, 1, 1, 0, 1);
        L2L_CLIP(1, 0, 1, 1, 0, 0);

        // pos -> pos
        REQUIRE(lin2lin_clip<float>(0, 0, 1, -10, 10) == Approx(-10));
        REQUIRE(lin2lin_clip<float>(-0.01, 0, 1, -10, 10) == Approx(-10));
        REQUIRE(lin2lin_clip<float>(-1, 0, 1, -10, 10) == Approx(-10));
        REQUIRE(lin2lin_clip<float>(1.01, 0, 1, -10, 10) == Approx(10));
        REQUIRE(lin2lin_clip<float>(100, 0, 1, -10, 10) == Approx(10));

        // neg -> pos
        REQUIRE(lin2lin_clip<float>(1, 1, 0, -10, 10) == Approx(-10));
        REQUIRE(lin2lin_clip<float>(1.1, 1, 0, -10, 10) == Approx(-10));
        REQUIRE(lin2lin_clip<float>(0, 1, 0, -10, 10) == Approx(10));
        REQUIRE(lin2lin_clip<float>(-0.1, 1, 0, -10, 10) == Approx(10));

        // neg -> neg
        REQUIRE(lin2lin_clip<float>(0, 1, 0, 10, -10) == Approx(-10));
        REQUIRE(lin2lin_clip<float>(-0.1, 1, 0, 10, -10) == Approx(-10));
        REQUIRE(lin2lin_clip<float>(1, 1, 0, 10, -10) == Approx(10));
        REQUIRE(lin2lin_clip<float>(1.1, 1, 0, 10, -10) == Approx(10));

        // pos -> neg
        REQUIRE(lin2lin_clip<float>(0, 0, 1, 10, -10) == Approx(10));
        REQUIRE(lin2lin_clip<float>(-0.1, 0, 1, 10, -10) == Approx(10));
        REQUIRE(lin2lin_clip<float>(1, 0, 1, 10, -10) == Approx(-10));
        REQUIRE(lin2lin_clip<float>(1.1, 0, 1, 10, -10) == Approx(-10));
    }

    SECTION("clip_any")
    {
        using namespace ceammc;
        REQUIRE(clip_any<float>(1, 0, 10) == 1);
        REQUIRE(clip_any<float>(1, 10, 0) == 1);
        REQUIRE(clip_any<float>(-1, 0, 10) == 0);
        REQUIRE(clip_any<float>(-1, 10, 0) == 0);
        REQUIRE(clip_any<float>(10.1, 0, 10) == 10);
        REQUIRE(clip_any<float>(10.1, 10, 0) == 10);
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

    SECTION("lin2sin2")
    {
#define L2S2(x, x0, x1, y0, y1, v) REQUIRE(lin2sin2(x, x0, x1, y0, y1) == Approx(v))
        L2S2(0, 0, 1, 0, 1, 0);
        L2S2(0, 0, 1, 0, 2, 0);
        L2S2(0, 0, 2, 0, 2, 0);
        L2S2(0, 0, 2, -2, 2, -2);
        L2S2(0, 0, 2, 1, 2, 1);
        L2S2(1, 0, 2, 0, 2, 1);
        L2S2(1, 0, 2, 1, 11, 6);
        L2S2(2, 0, 2, 1, 11, 11);
        L2S2(2, 0, 2, -2, 1, 1);
        L2S2(3, 1, 5, 1, 2, 1.5);
        L2S2(0, -3, 3, -4, -2, -3);
    }

    SECTION("lin2sigmoid")
    {
#define L2S(x, x0, x1, y0, y1, v) REQUIRE(lin2sigmoid(x, x0, x1, y0, y1, 50) == Approx(v).margin(0.00001))
        L2S(0, 0, 1, 0, 1, 0);
        L2S(0, 0, 1, 0, 2, 0);
        L2S(0, 0, 2, 0, 2, 0);
        L2S(0, 0, 2, -2, 2, -2);
        L2S(0, 0, 2, 1, 2, 1);
        L2S(1, 0, 2, 0, 2, 1);
        L2S(1, 0, 2, 1, 11, 6);
        L2S(2, 0, 2, 1, 11, 11);
        L2S(2, 0, 2, -2, 1, 1);
        L2S(3, 1, 5, 1, 2, 1.5);
        L2S(0, -3, 3, -4, -2, -3);
    }

    SECTION("pitch")
    {
        SECTION("SPN")
        {
            SECTION("invalid")
            {
                REQUIRE(spn2midi("") == MIDI_NONE);
                REQUIRE(spn2midi("C") == 60);
                REQUIRE(spn2midi("C23") == MIDI_NONE);
                REQUIRE(spn2midi("C#") == 61);
                REQUIRE(spn2midi("C#b") == MIDI_NONE);
                REQUIRE(spn2midi("C?#b") == MIDI_NONE);
                REQUIRE(spn2midi("1") == MIDI_NONE);
                REQUIRE(spn2midi("R?") == MIDI_NONE);
            }

            REQUIRE(spn2midi("C0") == 12);
            REQUIRE(spn2midi("C1") == 24);
            REQUIRE(spn2midi("C4") == 60);
            REQUIRE(spn2midi("C") == 60);
            REQUIRE(spn2midi("Cb") == 59);
            REQUIRE(spn2midi("C#4") == 61);

            REQUIRE(spn2midi("A3") == 57);
            REQUIRE(spn2midi("A4") == 69);
            REQUIRE(spn2midi("Ab4") == 68);
            REQUIRE(spn2midi("Abb4") == 67);

            REQUIRE(spn2midi("E6") == 88);
            REQUIRE(spn2midi("E4") == 64);
            REQUIRE(spn2midi("Eb4") == 63);
            REQUIRE(spn2midi("Ebb4") == 62);

            REQUIRE(spn2midi("A3") == 57);
            REQUIRE(spn2midi("B3") == 59);
            REQUIRE(spn2midi("C3") == 48);
            REQUIRE(spn2midi("D3") == 50);
            REQUIRE(spn2midi("E3") == 52);
            REQUIRE(spn2midi("F3") == 53);
            REQUIRE(spn2midi("G3") == 55);
            REQUIRE(spn2midi("R") == MIDI_REST);
        }
    }

    SECTION("wrapInteger")
    {
        using namespace ceammc;
        REQUIRE(wrapInteger<int>(0, 3) == 0);
        REQUIRE(wrapInteger<int>(1, 3) == 1);
        REQUIRE(wrapInteger<int>(2, 3) == 2);
        REQUIRE(wrapInteger<int>(3, 3) == 0);
        REQUIRE(wrapInteger<int>(4, 3) == 1);
        REQUIRE(wrapInteger<int>(5, 3) == 2);
        REQUIRE(wrapInteger<int>(6, 3) == 0);
        REQUIRE(wrapInteger<int>(-1, 3) == 2);
        REQUIRE(wrapInteger<int>(-2, 3) == 1);
        REQUIRE(wrapInteger<int>(-3, 3) == 0);
        REQUIRE(wrapInteger<int>(-4, 3) == 2);
        REQUIRE(wrapInteger<int>(-5, 3) == 1);
        REQUIRE(wrapInteger<int>(-6, 3) == 0);
        REQUIRE(wrapInteger<int>(-7, 3) == 2);

        REQUIRE(wrapInteger<signed char>(0, 3) == 0);
        REQUIRE(wrapInteger<signed char>(1, 3) == 1);
        REQUIRE(wrapInteger<signed char>(2, 3) == 2);
        REQUIRE(wrapInteger<signed char>(3, 3) == 0);
        REQUIRE(wrapInteger<signed char>(4, 3) == 1);
        REQUIRE(wrapInteger<signed char>(5, 3) == 2);
        REQUIRE(wrapInteger<signed char>(6, 3) == 0);
        REQUIRE(wrapInteger<signed char>(-1, 3) == 2);
        REQUIRE(wrapInteger<signed char>(-2, 3) == 1);
        REQUIRE(wrapInteger<signed char>(-3, 3) == 0);
        REQUIRE(wrapInteger<signed char>(-4, 3) == 2);
        REQUIRE(wrapInteger<signed char>(-5, 3) == 1);
        REQUIRE(wrapInteger<signed char>(-6, 3) == 0);
        REQUIRE(wrapInteger<signed char>(-7, 3) == 2);

        REQUIRE(wrapInteger<long>(0, 3) == 0);
        REQUIRE(wrapInteger<long>(1, 3) == 1);
        REQUIRE(wrapInteger<long>(2, 3) == 2);
        REQUIRE(wrapInteger<long>(3, 3) == 0);
        REQUIRE(wrapInteger<long>(4, 3) == 1);
        REQUIRE(wrapInteger<long>(5, 3) == 2);
        REQUIRE(wrapInteger<long>(6, 3) == 0);
        REQUIRE(wrapInteger<long>(-1, 3) == 2);
        REQUIRE(wrapInteger<long>(-2, 3) == 1);
        REQUIRE(wrapInteger<long>(-3, 3) == 0);
        REQUIRE(wrapInteger<long>(-4, 3) == 2);
        REQUIRE(wrapInteger<long>(-5, 3) == 1);
        REQUIRE(wrapInteger<long>(-6, 3) == 0);
        REQUIRE(wrapInteger<long>(-7, 3) == 2);

        REQUIRE(wrapInteger<int>(0, 2) == 0);
        REQUIRE(wrapInteger<int>(1, 2) == 1);
        REQUIRE(wrapInteger<int>(2, 2) == 0);
        REQUIRE(wrapInteger<int>(3, 2) == 1);
        REQUIRE(wrapInteger<int>(4, 2) == 0);
        REQUIRE(wrapInteger<int>(5, 2) == 1);
        REQUIRE(wrapInteger<int>(6, 2) == 0);
        REQUIRE(wrapInteger<int>(-1, 2) == 1);
        REQUIRE(wrapInteger<int>(-2, 2) == 0);
        REQUIRE(wrapInteger<int>(-3, 2) == 1);
        REQUIRE(wrapInteger<int>(-4, 2) == 0);
        REQUIRE(wrapInteger<int>(-5, 2) == 1);
        REQUIRE(wrapInteger<int>(-6, 2) == 0);
        REQUIRE(wrapInteger<int>(-7, 2) == 1);

        REQUIRE(wrapInteger<int>(0, 1) == 0);
        REQUIRE(wrapInteger<int>(1, 1) == 0);
        REQUIRE(wrapInteger<int>(2, 1) == 0);
        REQUIRE(wrapInteger<int>(3, 1) == 0);
        REQUIRE(wrapInteger<int>(4, 1) == 0);
        REQUIRE(wrapInteger<int>(5, 1) == 0);
        REQUIRE(wrapInteger<int>(6, 1) == 0);
        REQUIRE(wrapInteger<int>(-1, 1) == 0);
        REQUIRE(wrapInteger<int>(-2, 1) == 0);
        REQUIRE(wrapInteger<int>(-3, 1) == 0);
        REQUIRE(wrapInteger<int>(-4, 1) == 0);
        REQUIRE(wrapInteger<int>(-5, 1) == 0);
        REQUIRE(wrapInteger<int>(-6, 1) == 0);
        REQUIRE(wrapInteger<int>(-7, 1) == 0);
    }

    SECTION("fold")
    {
        using namespace ceammc;
        REQUIRE(foldInteger<int>(0, 3) == 0);
        REQUIRE(foldInteger<int>(1, 3) == 1);
        REQUIRE(foldInteger<int>(2, 3) == 2);
        REQUIRE(foldInteger<int>(3, 3) == 1);
        REQUIRE(foldInteger<int>(4, 3) == 0);
        REQUIRE(foldInteger<int>(5, 3) == 1);
        REQUIRE(foldInteger<int>(6, 3) == 2);
        REQUIRE(foldInteger<int>(7, 3) == 1);
        REQUIRE(foldInteger<int>(8, 3) == 0);
        REQUIRE(foldInteger<int>(9, 3) == 1);
        REQUIRE(foldInteger<int>(10, 3) == 2);
        REQUIRE(foldInteger<int>(0, 3) == 0);

        REQUIRE(foldInteger<int>(-1, 3) == 1);
        REQUIRE(foldInteger<int>(-2, 3) == 2);
        REQUIRE(foldInteger<int>(-3, 3) == 1);
        REQUIRE(foldInteger<int>(-4, 3) == 0);
        REQUIRE(foldInteger<int>(-5, 3) == 1);
        REQUIRE(foldInteger<int>(-6, 3) == 2);
        REQUIRE(foldInteger<int>(-7, 3) == 1);
        REQUIRE(foldInteger<int>(-8, 3) == 0);
        REQUIRE(foldInteger<int>(-9, 3) == 1);

        REQUIRE(foldInteger<int>(0, 2) == 0);
        REQUIRE(foldInteger<int>(1, 2) == 1);
        REQUIRE(foldInteger<int>(2, 2) == 0);
        REQUIRE(foldInteger<int>(3, 2) == 1);
        REQUIRE(foldInteger<int>(4, 2) == 0);
        REQUIRE(foldInteger<int>(5, 2) == 1);
        REQUIRE(foldInteger<int>(6, 2) == 0);
        REQUIRE(foldInteger<int>(7, 2) == 1);
        REQUIRE(foldInteger<int>(8, 2) == 0);
        REQUIRE(foldInteger<int>(9, 2) == 1);

        REQUIRE(foldInteger<int>(0, 1) == 0);
        REQUIRE(foldInteger<int>(1, 1) == 0);
        REQUIRE(foldInteger<int>(2, 1) == 0);
        REQUIRE(foldInteger<int>(3, 1) == 0);
        REQUIRE(foldInteger<int>(4, 1) == 0);
        REQUIRE(foldInteger<int>(5, 1) == 0);
        REQUIRE(foldInteger<int>(6, 1) == 0);
        REQUIRE(foldInteger<int>(7, 1) == 0);
        REQUIRE(foldInteger<int>(8, 1) == 0);
        REQUIRE(foldInteger<int>(9, 1) == 0);
    }

    SECTION("relativeIndex")
    {
        using namespace ceammc;
        REQUIRE(relativeIndex<int>(0, 3) == 0);
        REQUIRE(relativeIndex<int>(1, 3) == 1);
        REQUIRE(relativeIndex<int>(2, 3) == 2);
        REQUIRE(relativeIndex<int>(3, 3) == -1);
        REQUIRE(relativeIndex<int>(4, 3) == -1);
        REQUIRE(relativeIndex<int>(5, 3) == -1);

        REQUIRE(relativeIndex<int>(-1, 3) == 2);
        REQUIRE(relativeIndex<int>(-2, 3) == 1);
        REQUIRE(relativeIndex<int>(-3, 3) == 0);
        REQUIRE(relativeIndex<int>(-4, 3) == -1);
        REQUIRE(relativeIndex<int>(-5, 3) == -1);
        REQUIRE(relativeIndex<int>(-6, 3) == -1);

        REQUIRE(relativeIndex<int>(0, 0) == -1);
    }

    SECTION("midi2freq")
    {
        REQUIRE(midi2freq(69.f) == Approx(440));
        REQUIRE(midi2freq(69.0) == Approx(440));
        REQUIRE(midi2freq(57.f) == Approx(220));
        REQUIRE(midi2freq(57.f) == Approx(220));
        REQUIRE(freq2midi(midi2freq(60.0)) == Approx(60));
        REQUIRE(freq2midi(midi2freq(60.0, 442), 442) == Approx(60));
        REQUIRE(freq2midi(midi2freq(60.f, 442), 442) == Approx(60));
        REQUIRE(freq2midi(midi2freq(60.f)) == Approx(60));
        REQUIRE(freq2midi(midi2freq(69.0)) == Approx(69));

        REQUIRE(midi2freq(69.f, 442) == Approx(442));
        REQUIRE(midi2freq(69.f, 415) == Approx(415));

        REQUIRE(midi2freq(-60.f) == Approx(0.25549f).epsilon(0.001));
        REQUIRE(midi2freq(127.f) == Approx(12543.85547f));
        REQUIRE(freq2midi(0.f) == Approx(-1));
        REQUIRE(freq2midi(-10.f) == Approx(-1));
    }

    SECTION("rad/phase/degrees")
    {
        REQUIRE(phase2rad<float>(0) == Approx(0));
        REQUIRE(phase2rad<float>(0.25) == Approx(M_PI_2));
        REQUIRE(phase2rad<float>(0.5) == Approx(M_PI));
        REQUIRE(phase2rad<float>(1) == Approx(0));
        REQUIRE(phase2rad<double>(0) == Approx(0));
        REQUIRE(phase2rad<double>(0.25) == Approx(M_PI_2));
        REQUIRE(phase2rad<double>(0.5) == Approx(M_PI));
        REQUIRE(phase2rad<double>(1) == Approx(0));
        REQUIRE(phase2rad<double>(1000) == Approx(0));
        REQUIRE(phase2rad<double>(1000.125) == Approx(M_PI_4));
        REQUIRE(phase2rad<double>(-1000) == Approx(0));

        REQUIRE(phase2degree<double>(0) == Approx(0));
        REQUIRE(phase2degree<double>(0.25) == Approx(90));
        REQUIRE(phase2degree<double>(5.5) == Approx(180));
        REQUIRE(phase2degree<double>(-0.25) == Approx(270));
        REQUIRE(degree2rad<double>(0) == Approx(0));
        REQUIRE(degree2rad<double>(90) == Approx(M_PI_2));
        REQUIRE(degree2rad<double>(-180) == Approx(M_PI));

        // degree->phase->degree
        REQUIRE(phase2degree<float>(degree2phase<float>(0)) == Approx(0));
        REQUIRE(phase2degree<float>(degree2phase<float>(90)) == Approx(90));
        REQUIRE(phase2degree<float>(degree2phase<float>(181)) == Approx(181));
        REQUIRE(phase2degree<float>(degree2phase<float>(272)) == Approx(272));
        REQUIRE(phase2degree<float>(degree2phase<float>(360)) == Approx(0));
        REQUIRE(phase2degree<float>(degree2phase<float>(361.1)) == Approx(1.1));
        REQUIRE(phase2degree<float>(degree2phase<float>(-90)) == Approx(270));
        REQUIRE(phase2degree<float>(degree2phase<float>(360010)) == Approx(10));

        // phase->degree->phase
        REQUIRE(degree2phase<float>(phase2degree<float>(0)) == Approx(0));
        REQUIRE(degree2phase<float>(phase2degree<float>(0.5)) == Approx(0.5));
        REQUIRE(degree2phase<float>(phase2degree<float>(2.1)) == Approx(0.1));
        REQUIRE(degree2phase<float>(phase2degree<float>(-0.4)) == Approx(0.6));

        // rad->phase->rad
        REQUIRE(phase2rad<float>(rad2phase<float>(0)) == Approx(0));
        REQUIRE(phase2rad<float>(rad2phase<float>(0.5)) == Approx(0.5));
        REQUIRE(phase2rad<float>(rad2phase<float>(M_PI)) == Approx(M_PI));
        REQUIRE(phase2rad<float>(rad2phase<float>(-M_PI)) == Approx(M_PI));
        REQUIRE(phase2rad<float>(rad2phase<float>(-M_PI_2)) == Approx(3 * M_PI_2));
        REQUIRE(phase2rad<float>(rad2phase<float>(2 * M_PI)) == Approx(0));

        // phase->rad->phase
        REQUIRE(rad2phase<float>(phase2rad<float>(0)) == Approx(0));
        REQUIRE(rad2phase<float>(phase2rad<float>(0.5)) == Approx(0.5));
        REQUIRE(rad2phase<float>(phase2rad<float>(1)) == Approx(0));
        REQUIRE(rad2phase<float>(phase2rad<float>(1.1)) == Approx(0.1));

        // rad->degree->rad
        REQUIRE(degree2rad<float>(rad2degree<float>(0)) == Approx(0));
        REQUIRE(degree2rad<float>(rad2degree<float>(M_PI)) == Approx(M_PI));
        REQUIRE(degree2rad<float>(rad2degree<float>(2 * M_PI)) == Approx(0));
        REQUIRE(degree2rad<float>(rad2degree<float>(-M_PI)) == Approx(M_PI));
        REQUIRE(degree2rad<float>(rad2degree<float>(-M_PI_2)) == Approx(3 * M_PI_2));

        // degree->rad->degree
        REQUIRE(rad2degree<float>(degree2rad<float>(0)) == Approx(0));
        REQUIRE(rad2degree<float>(degree2rad<float>(90)) == Approx(90));
        REQUIRE(rad2degree<float>(degree2rad<float>(359.9)) == Approx(359.9));
        REQUIRE(rad2degree<float>(degree2rad<float>(360)) == Approx(0));
        REQUIRE(rad2degree<float>(degree2rad<float>(360000)) == Approx(0));
        REQUIRE(rad2degree<float>(degree2rad<float>(-179)) == Approx(181));
    }

    SECTION("wrapeFloatMax")
    {
        using namespace ceammc;
        REQUIRE(wrapFloatMax<float>(0, 2.5) == Approx(0));
        REQUIRE(wrapFloatMax<float>(2, 2.5) == Approx(2));
        REQUIRE(wrapFloatMax<float>(2.49, 2.5) == Approx(2.49));
        REQUIRE(wrapFloatMax<float>(2.5, 2.5) == Approx(0));
        REQUIRE(wrapFloatMax<float>(2.51, 2.5) == Approx(0.01));
        REQUIRE(wrapFloatMax<float>(2.51, 2.5) == Approx(0.01));
        REQUIRE(wrapFloatMax<float>(-0.1, 2.5) == Approx(2.4));

        REQUIRE(wrapFloatMax<double>(0, 2.5) == Approx(0));
        REQUIRE(wrapFloatMax<double>(2, 2.5) == Approx(2));
        REQUIRE(wrapFloatMax<double>(2.49, 2.5) == Approx(2.49));
        REQUIRE(wrapFloatMax<double>(2.5, 2.5) == Approx(0));
        REQUIRE(wrapFloatMax<double>(2.51, 2.5) == Approx(0.01));
        REQUIRE(wrapFloatMax<double>(2.51, 2.5) == Approx(0.01));
        REQUIRE(wrapFloatMax<double>(-2500.01, 2.5) == Approx(2.49));
        REQUIRE(wrapFloatMax<double>(2500.01, 2.5) == Approx(0.01));
    }

    SECTION("wrapFloatMinMax")
    {
        using namespace ceammc;
        REQUIRE(wrapFloatMinMax<float>(1.1, 1.1, 3.2) == Approx(1.1));
        REQUIRE(wrapFloatMinMax<float>(1.2, 1.1, 3.2) == Approx(1.2));
        REQUIRE(wrapFloatMinMax<float>(3.1, 1.1, 3.2) == Approx(3.1));
        REQUIRE(wrapFloatMinMax<float>(3.2, 1.1, 3.2) == Approx(1.1));
        REQUIRE(wrapFloatMinMax<float>(1.0, 1.1, 3.2) == Approx(3.1));

        REQUIRE(wrapFloatMinMax<double>(-1, -1, 1) == Approx(-1));
        REQUIRE(wrapFloatMinMax<double>(0, -1, 1) == Approx(0));
        REQUIRE(wrapFloatMinMax<double>(0.9, -1, 1) == Approx(0.9));
        REQUIRE(wrapFloatMinMax<double>(1, -1, 1) == Approx(-1));
        REQUIRE(wrapFloatMinMax<double>(1.1, -1, 1) == Approx(-0.9));
        REQUIRE(wrapFloatMinMax<double>(-1.1, -1, 1) == Approx(0.9));
    }

    SECTION("clip")
    {
        using namespace ceammc;
        REQUIRE(clip<int>(-1, 0, 10) == 0);
        REQUIRE(clip<int>(0, 0, 10) == 0);
        REQUIRE(clip<int>(1, 0, 10) == 1);
        REQUIRE(clip<int>(10, 0, 10) == 10);
        REQUIRE(clip<int>(11, 0, 10) == 10);

        REQUIRE((clip<int, -1, 1>(-2)) == -1);
        REQUIRE((clip<int, -1, 1>(-1)) == -1);
        REQUIRE((clip<int, -1, 1>(0)) == 0);
        REQUIRE((clip<int, -1, 1>(1)) == 1);
        REQUIRE((clip<int, -1, 1>(2)) == 1);

        REQUIRE((clip<float, -1, 1>(-1.1)) == -1);
        REQUIRE((clip<float, -1, 1>(-1)) == -1);
        REQUIRE((clip<float, -1, 1>(0.1f)) == 0.1f);
        REQUIRE((clip<float, -1, 1>(1)) == 1);
        REQUIRE((clip<float, -1, 1>(1.1)) == 1);
    }

    SECTION("clip01")
    {
        using namespace ceammc;
        REQUIRE(clip01<float>(-1.1) == 0);
        REQUIRE(clip01<float>(-1) == 0);
        REQUIRE(clip01<float>(0) == 0);
        REQUIRE(clip01<float>(0.1f) == 0.1f);
        REQUIRE(clip01<float>(1) == 1);
        REQUIRE(clip01<float>(1.1) == 1);
    }

    SECTION("lin2lin template")
    {
        REQUIRE((lin2lin<float, 0, 1>(-0.5, 4, 2)) == Approx(5));
        REQUIRE((lin2lin<float, 0, 1>(0, 4, 2)) == Approx(4));
        REQUIRE((lin2lin<float, 0, 1>(0.5, 4, 2)) == Approx(3));
        REQUIRE((lin2lin<float, 0, 1>(1, 4, 2)) == Approx(2));
        REQUIRE((lin2lin<float, 0, 1>(1.5, 4, 2)) == Approx(1));

        REQUIRE((lin2lin_clip<float, 0, 1>(-0.5, 4, 2)) == Approx(4));
        REQUIRE((lin2lin_clip<float, 0, 1>(0, 4, 2)) == Approx(4));
        REQUIRE((lin2lin_clip<float, 0, 1>(0.5, 4, 2)) == Approx(3));
        REQUIRE((lin2lin_clip<float, 0, 1>(1, 4, 2)) == Approx(2));
        REQUIRE((lin2lin_clip<float, 0, 1>(1.5, 4, 2)) == Approx(2));
    }

    SECTION("normalizeIndex")
    {
        using namespace ceammc;
        REQUIRE(normalizeIndex(-6, 3) == 0);
        REQUIRE(normalizeIndex(-5, 3) == 1);
        REQUIRE(normalizeIndex(-4, 3) == 2);
        REQUIRE(normalizeIndex(-3, 3) == 0);
        REQUIRE(normalizeIndex(-2, 3) == 1);
        REQUIRE(normalizeIndex(-1, 3) == 2);
        REQUIRE(normalizeIndex(0, 3) == 0);
        REQUIRE(normalizeIndex(1, 3) == 1);
        REQUIRE(normalizeIndex(2, 3) == 2);
        REQUIRE(normalizeIndex(3, 3) == 0);
        REQUIRE(normalizeIndex(4, 3) == 1);
        REQUIRE(normalizeIndex(5, 3) == 2);
        REQUIRE(normalizeIndex(6, 3) == 0);
    }

    SECTION("color")
    {
        using namespace ceammc::convert::color;
        REQUIRE(rgb2pd(0, 0, 0) == -1);
        REQUIRE(rgb2pd(0, 0, 1) == -2);
        REQUIRE(rgb2pd(0, 0, 2) == -3);

        REQUIRE(rgb2pd(0, 1, 0) == -257);
        REQUIRE(rgb2pd(0, 2, 0) == -513);
    }
}
