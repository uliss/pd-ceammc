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
#include "../spat/pan_cosine.h"
#include "ceammc_pd.h"
#include "sound_external_test.h"

#include "catch.hpp"

typedef TestSoundExtension<PanCosine> PanCosTest;

using namespace ceammc;

TEST_CASE("pan.cos~", "[externals]")
{
    SECTION("init")
    {
        PanCosTest t("pan.cos~", AtomList(), true);
        REQUIRE_PROPERTY(t, @pos, 0.f);
        REQUIRE_PROPERTY(t, @smooth, 20);
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE(t.numInputChannels() == 1);
        REQUIRE(t.numOutputChannels() == 2);
    }

    SECTION("process")
    {
        PanCosTest t("pan.cos~", L2("@smooth", 0.f), true);
        REQUIRE(t.blockSize() == 64);
        REQUIRE(t.numInputChannels() == 1);
        REQUIRE(t.numOutputChannels() == 2);

        REQUIRE_PROPERTY(t, @pos, 0.f);
        REQUIRE_PROPERTY(t, @smooth, 0.f);

        TestSignal<1, 2> sig;
        sig.fillInput(1.f);
        sig.fillOutput(0);

        t.setProperty("@pos", L1(-1));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == Approx(1.f));
            REQUIRE(sig.out[1][i] == Approx(0.f));
        }

        t.setProperty("@pos", L1(1));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == Approx(0.f));
            REQUIRE(sig.out[1][i] == Approx(1.f));
        }

        t.setProperty("@pos", L1(0.1f));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] * sig.out[0][i] + sig.out[1][i] * sig.out[1][i] == Approx(1));
        }

        t.setProperty("@pos", L1(-0.4f));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] * sig.out[0][i] + sig.out[1][i] * sig.out[1][i] == Approx(1));
        }

        t.setProperty("@pos", L1(0.4f));
        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] * sig.out[0][i] + sig.out[1][i] * sig.out[1][i] == Approx(1));
        }

        t.setProperty("@pos", L1(0.f));
        t.processBlock(sig.in, sig.out);

        const t_float f = sqrtf(0.5);
        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == Approx(f));
            REQUIRE(sig.out[1][i] == Approx(f));
            REQUIRE(sig.out[0][i] * sig.out[0][i] + sig.out[1][i] * sig.out[1][i] == Approx(1));
        }
    }
}
