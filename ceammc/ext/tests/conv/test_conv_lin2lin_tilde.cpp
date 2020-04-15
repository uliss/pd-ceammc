/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "conv_lin2lin_tilde.h"
#include "test_conv_base.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(Lin2LinTilde, conv, lin2lin_tilde)

TEST_CASE("conv.lin2lin~", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TestExtLin2LinTilde t("conv.lin2lin~", L(), true);
        REQUIRE(t.blockSize() == 64);
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numInputChannels() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE(t.numOutputChannels() == 1);
        REQUIRE_PROPERTY_LIST(t, @in_from, LX(-1));
        REQUIRE_PROPERTY_LIST(t, @in_to, LX(1));
        REQUIRE_PROPERTY_LIST(t, @out_from, LX(-1));
        REQUIRE_PROPERTY_LIST(t, @out_to, LX(1));
        REQUIRE_PROPERTY_LIST(t, @clip, LA("minmax"));
    }

    SECTION("process all")
    {
        typedef TestSignal<1, 1> LinSignal;

        TestExtLin2LinTilde t("lin->lin~", L(), true);

        LinSignal s0;
        s0.fillInputN(0, 1);

        DSP<LinSignal, TestExtLin2LinTilde> dsp(s0, t);
        dsp.processBlock();

        for (size_t i = 0; i < dsp.BS; i++) {
            REQUIRE(dsp.out(0, i) == Approx(1));
        }

        s0.fillInputN(0, -1);
        dsp.processBlock();
        for (size_t i = 0; i < dsp.BS; i++) {
            REQUIRE(dsp.out(0, i) == Approx(-1));
        }

        s0.fillInputN(0, 0);
        dsp.processBlock();
        for (size_t i = 0; i < dsp.BS; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
        }

        // clip min
        s0.fillInputN(0, -2);
        dsp.processBlock();
        for (size_t i = 0; i < dsp.BS; i++) {
            REQUIRE(dsp.out(0, i) == Approx(-1));
        }

        // clip max
        s0.fillInputN(0, 1.1);
        dsp.processBlock();
        for (size_t i = 0; i < dsp.BS; i++) {
            REQUIRE(dsp.out(0, i) == Approx(1));
        }

        auto gen_fn = [](size_t n) { return n / 10.f; };

        t.setProperty("@clip", LA("noclip"));
        s0.fillInput(gen_fn);
        dsp.processBlock();
        for (size_t i = 0; i < dsp.BS; i++) {
            REQUIRE(dsp.out(0, i) == Approx(gen_fn(i)));
        }
    }
}
