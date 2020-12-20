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
#include "pan_cosine.h"
#include "test_spat_base.h"

PD_COMPLETE_SND_TEST_SETUP(PanCosine, pan, cosine)

typedef TestSoundExternal<PanCosine> PanCosTest;

TEST_CASE("pan.cos~", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        PanCosTest t("pan.cos~", L(), true);
        REQUIRE_PROPERTY(t, @pos, 0.f);
        REQUIRE_PROPERTY(t, @smooth, 20);
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE(t.numInputChannels() == 1);
        REQUIRE(t.numOutputChannels() == 2);
    }

    SECTION("process")
    {
        PanCosTest t("pan.cos~", LA("@smooth", 0.f), true);
        REQUIRE(t.blockSize() == 64);
        REQUIRE(t.numInputChannels() == 1);
        REQUIRE(t.numOutputChannels() == 2);

        REQUIRE_PROPERTY(t, @pos, 0.f);
        REQUIRE_PROPERTY(t, @smooth, 0.f);

        TestSignal<1, 2> sig;
        sig.fillInput(1.f);
        sig.fillOutput(0);

        t.setProperty("@pos", LF(-1));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == Approx(1.f));
            REQUIRE(sig.out[1][i] == Approx(0.f));
        }

        t.setProperty("@pos", LF(1));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == Approx(0).margin(0.00001));
            REQUIRE(sig.out[1][i] == Approx(1).margin(0.00001));
        }

        t.setProperty("@pos", LA(0.1f));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] * sig.out[0][i] + sig.out[1][i] * sig.out[1][i] == Approx(1));
        }

        t.setProperty("@pos", LA(-0.4f));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] * sig.out[0][i] + sig.out[1][i] * sig.out[1][i] == Approx(1));
        }

        t.setProperty("@pos", LA(0.4f));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] * sig.out[0][i] + sig.out[1][i] * sig.out[1][i] == Approx(1));
        }

        t.setProperty("@pos", LF(0.f));
        t.processBlock(sig.in, sig.out);

        const t_float f = sqrtf(0.5);
        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == Approx(f));
            REQUIRE(sig.out[1][i] == Approx(f));
            REQUIRE(sig.out[0][i] * sig.out[0][i] + sig.out[1][i] * sig.out[1][i] == Approx(1));
        }
    }
}
