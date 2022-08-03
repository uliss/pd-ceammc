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
#include "test_external.h"
#include "test_sound.h"
#include "xfade_tilde.h"

typedef TestSoundExternal<XFadeTilde> XFadeTildeTest;

PD_COMPLETE_TEST_SETUP(XFadeTilde, base, xfade_tilde)

TEST_CASE("xfade~", "[externals]")
{
    pd_test_mod_init_base_xfade_tilde();

    SECTION("construct")
    {
        SECTION("default")
        {
            XFadeTildeTest t("xfade~", L(), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numInputChannels() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.numOutputChannels() == 1);
            REQUIRE_PROPERTY_LIST(t, @smooth, LX(20));

            t.setProperty("@smooth", LF(10));
            REQUIRE_PROPERTY_FLOAT(t, @smooth, 10);

            t.setProperty("@smooth", LF(10000));
            REQUIRE_PROPERTY_FLOAT(t, @smooth, 10000);

            t.setProperty("@smooth", LF(0.f));
            REQUIRE_PROPERTY_FLOAT(t, @smooth, 10000);

            t.setProperty("@smooth", LF(-10));
            REQUIRE_PROPERTY_FLOAT(t, @smooth, 10000);

            t.setProperty("@smooth", LA("NONE"));
            REQUIRE_PROPERTY_FLOAT(t, @smooth, 10000);
        }

        SECTION("args")
        {
            XFadeTildeTest t("xfade~", LF(4), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 5);
            REQUIRE(t.numInputChannels() == 4);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.numOutputChannels() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @x, 0);
        }

        SECTION("args x")
        {
            TExt t("xfade~", LF(4, 0.125));
            REQUIRE_PROPERTY_FLOAT(t, @x, 0.125);
        }
    }

    SECTION("process all")
    {
        XFadeTildeTest t("gain~", LA(4, "@lin"), true);
        TestSignal<4, 1> s0;
        s0.fillInputN(0, 1);
        s0.fillInputN(1, 2);
        s0.fillInputN(2, -2);
        s0.fillInputN(3, -1);

        DSP<TestSignal<4, 1>, XFadeTildeTest> dsp(s0, t);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(1));
        }

        // 1-2 mix
        t.sendFloat(0.5, 4);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(1.5));
        }

        // 2 only
        t.sendFloat(1, 4);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(2));
        }

        // 2-3 mix (2*0.75 + -2*0.25 == 2*0.5 == 1)
        t.sendFloat(1.25, 4);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(1));
        }

        // 3 only
        t.sendFloat(2, 4);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(-2));
        }

        // 3-4 mix
        t.sendFloat(2.5, 4);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(-1.5));
        }

        // 4 only
        t.sendFloat(3, 4);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(-1));
        }

        // invalid
        t.sendFloat(4, 4);
        REQUIRE_PROPERTY_FLOAT(t, @x, 3);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(-1));
        }

        // invalid
        t.sendFloat(-1, 4);
        REQUIRE_PROPERTY_FLOAT(t, @x, 3);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(-1));
        }
    }

    SECTION("inlet")
    {
        TExt t("xfade~", LF(2, 0.125));
        REQUIRE_PROPERTY_FLOAT(t, @x, 0.125);

        t.sendFloatTo(0.5, 2);
        REQUIRE_PROPERTY_FLOAT(t, @x, 0.5);
    }
}
