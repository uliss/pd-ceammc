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
#include "../spat/pan_linear.h"
#include "ceammc_pd.h"
#include "sound_external_test.h"

#include "catch.hpp"

typedef TestSoundExtension<PanLinear> PanLinTest;

using namespace ceammc;

TEST_CASE("pan.lin~", "[externals]")
{
    SECTION("init")
    {
        PanLinTest t("pan.lin~", AtomList(), true);
        REQUIRE_PROPERTY(t, @pos, 0.f);
        REQUIRE_PROPERTY(t, @smooth, 20);
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE(t.numInputChannels() == 1);
        REQUIRE(t.numOutputChannels() == 2);
    }

    SECTION("smooth")
    {
        SmoothControlValue sm(0);

        for (int i = 0; i < 100; i++) {
            REQUIRE(sm.get(10) == 10.f);
        }

        sm.setSmoothTime(2, 1000, 1);
        REQUIRE(sm.get(0) == Approx(5));
        REQUIRE(sm.get(0) == Approx(2.5));
        REQUIRE(sm.get(0) == Approx(1.25));
        REQUIRE(sm.get(0) == Approx(0.625));
        REQUIRE(sm.get(0) == Approx(0.3125));
        REQUIRE(sm.get(0) == Approx(0.15625));
        REQUIRE(sm.get(0) == Approx(0.07812));
        REQUIRE(sm.get(0) == Approx(0.03906));
        REQUIRE(sm.get(0) == Approx(0.01953));
        REQUIRE(sm.get(0) == Approx(0.00977));
        REQUIRE(sm.get(0) == Approx(0.00488));
        REQUIRE(sm.get(0) == Approx(0.00244));
        REQUIRE(sm.get(0) == Approx(0.00122));
        REQUIRE(sm.get(0) == Approx(0.00061));
        REQUIRE(sm.get(0) == Approx(0.00030));
        REQUIRE(sm.get(0) == Approx(0.00015));
        REQUIRE(sm.get(0) == Approx(0.00007));
        REQUIRE(sm.get(0) == Approx(0.00003));
        REQUIRE(sm.get(0) == Approx(0.00001));
        REQUIRE(sm.get(0) == Approx(0.00000));
    }

    SECTION("process")
    {
        PanLinTest t("pan.lin~", L2("@smooth", 0.f), true);
        REQUIRE(t.blockSize() == 64);
        REQUIRE(t.numInputChannels() == 1);
        REQUIRE(t.numOutputChannels() == 2);

        REQUIRE_PROPERTY(t, @pos, 0.f);
        REQUIRE_PROPERTY(t, @smooth, 0.f);

        TestSignal<1, 2> sig;
        sig.fillInput(1.f);
        sig.fillOutput(0);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == 0.f);
        }

        REQUIRE_PROPERTY(t, @pos, 0.f);
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == 0.5f);
            REQUIRE(sig.out[1][i] == 0.5f);
        }

        t.setProperty("@pos", L1(-1));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == 1.f);
            REQUIRE(sig.out[1][i] == 0.f);
        }

        t.setProperty("@pos", L1(1));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == 0.f);
            REQUIRE(sig.out[1][i] == 1.f);
        }

        t.setProperty("@pos", L1(-0.6));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == Approx(0.8f));
            REQUIRE(sig.out[1][i] == Approx(0.2f));
        }

        t.setProperty("@pos", L1(0.4));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == Approx(0.3f));
            REQUIRE(sig.out[1][i] == Approx(0.7f));
        }
    }
}
