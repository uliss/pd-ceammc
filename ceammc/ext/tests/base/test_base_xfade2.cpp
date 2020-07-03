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
#include "xfade2_tilde.h"
#include "test_sound.h"
#include "test_external.h"

typedef TestSoundExternal<XFade2Tilde> XFade2TildeTest;

PD_COMPLETE_TEST_SETUP(XFade2Tilde, base, xfade2_tilde)

TEST_CASE("xfade2~", "[externals]")
{
    setup_base_xfade_tilde();
    pd_test_mod_init_base_xfade2_tilde();

    SECTION("construct")
    {
        SECTION("default")
        {
            XFade2TildeTest t("xfade2~", L(), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 5);
            REQUIRE(t.numInputChannels() == 4);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE(t.numOutputChannels() == 2);
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
            XFade2TildeTest t("xfade2~", LF(4), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 9);
            REQUIRE(t.numInputChannels() == 8);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE(t.numOutputChannels() == 2);
        }
    }

    SECTION("process all")
    {
        XFade2TildeTest t("xfade2~", LA(3, "@lin"), true);
        TestSignal<6, 2> s0;
        s0.fillInputN(0, 1);
        s0.fillInputN(1, 10);
        s0.fillInputN(2, 2);
        s0.fillInputN(3, 20);
        s0.fillInputN(4, -1);
        s0.fillInputN(5, -10);

        DSP<TestSignal<6, 2>, XFade2TildeTest> dsp(s0, t);
        REQUIRE(t.gains() == std::vector<float>({ 1, 0, 0 }));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(1));
            REQUIRE(dsp.out(1, i) == Approx(10));
        }

        // 1-2 mix
        t.sendFloat(0.5, 7);
        REQUIRE(t.gains() == std::vector<float>({ 0.5f, 0.5f, 0 }));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(1.5));
            REQUIRE(dsp.out(1, i) == Approx(15));
        }

        // 2 only
        t.sendFloat(1, 7);
        REQUIRE(t.gains() == std::vector<float>({ 0, 1, 0 }));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(2));
            REQUIRE(dsp.out(1, i) == Approx(20));
        }

        //  2-3 mix: 0*1 + 0.5*2 + 0.5*-1 = 0.5
        t.sendFloat(1.5, 7);
        REQUIRE(t.gains() == std::vector<float>({ 0, 0.5f, 0.5f }));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            //            REQUIRE(dsp.out(0, i) == Approx(0.5));
            //            REQUIRE(dsp.out(1, i) == Approx(5));
        }

        // 3 only
        t.sendFloat(2, 7);
        REQUIRE(t.gains() == std::vector<float>({ 0, 0, 1 }));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(-1));
            REQUIRE(dsp.out(1, i) == Approx(-10));
        }

        // invalid
        t.sendFloat(3, 7);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(-1));
            REQUIRE(dsp.out(1, i) == Approx(-10));
        }
    }
}
