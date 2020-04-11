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
#include "flow_multiplex_tilde.h"
#include "test_flow_base.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(MultiplexTilde, flow, multiplex_tilde)

using TExt = TestExtMultiplexTilde;

template <size_t N>
using SigN = TestSignal<N, 1>;

template <size_t N>
using DspN = DSP<SigN<N>, TExt>;

TEST_CASE("flow.mux~", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        SECTION("default")
        {
            TExt t("mux~", L(), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numInputChannels() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.numOutputChannels() == 1);
            REQUIRE_PROPERTY_LIST(t, @value, LX(1, 0));

            t <<= { 1_in, 1 };
            REQUIRE_PROPERTY_LIST(t, @value, LF(0, 1));
            t <<= { 1_in, 0 };
            REQUIRE_PROPERTY_LIST(t, @value, LF(1, 0));
        }

        SECTION("args")
        {
            TExt t("mux~", LF(4), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 5);
            REQUIRE(t.numInputChannels() == 4);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.numOutputChannels() == 1);
            REQUIRE_PROPERTY_LIST(t, @value, LX(1, 0, 0, 0));

            t <<= { 1_in, 1 };
            REQUIRE_PROPERTY_LIST(t, @value, LF(0, 1, 0, 0));
            t <<= { 1_in, 2 };
            REQUIRE_PROPERTY_LIST(t, @value, LF(0, 0, 1, 0));
            t <<= { 1_in, 3 };
            REQUIRE_PROPERTY_LIST(t, @value, LF(0, 0, 0, 1));
        }
    }

    SECTION("index")
    {
        TExt t("mux~", LF(3), true);
        REQUIRE(t.numInlets() == 4);
        REQUIRE_PROPERTY(t, @value, 1, 0, 0);

        SigN<3> s0;
        s0.fillInputN(0, 10);
        s0.fillInputN(1, 20);
        s0.fillInputN(2, 30);

        DspN<3> dsp(s0, t);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(10));
        }

        t <<= { 3_in, 0 };
        REQUIRE_PROPERTY(t, @value, 1, 0, 0);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(10));
        }

        t <<= { 3_in, 1 };
        REQUIRE_PROPERTY(t, @value, 0, 1, 0);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(20));
        }

        t <<= { 3_in, 2 };
        REQUIRE_PROPERTY(t, @value, 0, 0, 1);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(30));
        }

        t <<= { 3_in, 0 };
        REQUIRE_PROPERTY(t, @value, 1, 0, 0);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(10));
        }

        // invalid inlet
        t <<= { 3_in, -1 };
        REQUIRE_PROPERTY(t, @value, 1, 0, 0);

        // invalid inlet
        t <<= { 3_in, 3 };
        REQUIRE_PROPERTY(t, @value, 1, 0, 0);
    }

    SECTION("index")
    {
        TExt t("mux~", LF(3), true);

        SigN<3> s0;
        s0.fillInputN(0, 10);
        s0.fillInputN(1, 20);
        s0.fillInputN(2, 40);

        DspN<3> dsp(s0, t);

        t << LF(1, 1, 1);
        REQUIRE_PROPERTY(t, @value, 1, 1, 1);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(70));
        }

        t << LF(1, 0, 1);
        REQUIRE_PROPERTY(t, @value, 1, 0, 1);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(50));
        }

        t << LF(1, 0.5, 0.25);
        REQUIRE_PROPERTY(t, @value, 1, 0.5, 0.25);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(30));
        }
    }
}
