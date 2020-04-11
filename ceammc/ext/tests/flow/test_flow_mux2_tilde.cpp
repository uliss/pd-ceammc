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
#include "flow_multiplex2_tilde.h"
#include "test_flow_base.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(Multiplex2Tilde, flow, multiplex2_tilde)

using TExt = TestExtMultiplex2Tilde;

template <size_t N>
using SigN = TestSignal<N, 2>;

template <size_t N>
using DspN = DSP<SigN<N>, TExt>;

TEST_CASE("flow.mux2~", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        SECTION("default")
        {
            TExt t("mux2~", L(), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 5);
            REQUIRE(t.numInputChannels() == 4);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE(t.numOutputChannels() == 2);
            REQUIRE_PROPERTY(t, @value, 1, 0);
        }

        SECTION("args")
        {
            TExt t("mux2~", LF(4), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 9);
            REQUIRE(t.numInputChannels() == 8);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE(t.numOutputChannels() == 2);
            REQUIRE_PROPERTY(t, @value, 1, 0, 0, 0);
        }

        SECTION("too many")
        {
            TExt t("mux2~", LF(17), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 33);
            REQUIRE(t.numInputChannels() == 32);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE(t.numOutputChannels() == 2);
            REQUIRE_PROPERTY(t, @value, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
    }

    SECTION("index")
    {
        TExt t("mux2~", LF(3), true);
        REQUIRE_PROPERTY(t, @value, 1, 0, 0);

        SigN<6> s0;
        s0.fillInputN(0, 1);
        s0.fillInputN(1, -1);
        s0.fillInputN(2, 2);
        s0.fillInputN(3, -2);
        s0.fillInputN(4, 4);
        s0.fillInputN(5, -4);

        DspN<6> dsp(s0, t);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(1));
            REQUIRE(dsp.out(1, i) == Approx(-1));
        }

        t <<= { 7_in, 0 };
        dsp.processBlock(20);
        REQUIRE_PROPERTY(t, @value, 1, 0, 0);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(1));
            REQUIRE(dsp.out(1, i) == Approx(-1));
        }

        t <<= { 7_in, 1 };
        dsp.processBlock(20);
        REQUIRE_PROPERTY(t, @value, 0, 1, 0);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(2));
            REQUIRE(dsp.out(1, i) == Approx(-2));
        }

        t <<= { 7_in, 2 };
        dsp.processBlock(20);
        REQUIRE_PROPERTY(t, @value, 0, 0, 1);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(4));
            REQUIRE(dsp.out(1, i) == Approx(-4));
        }

        t <<= { 7_in, 0 };
        dsp.processBlock(20);
        REQUIRE_PROPERTY(t, @value, 1, 0, 0);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(1));
            REQUIRE(dsp.out(1, i) == Approx(-1));
        }

        // invalid inlet
        t <<= { 7_in, -1 };
        REQUIRE_PROPERTY(t, @value, 1, 0, 0);

        t <<= { 7_in, 7 };
        REQUIRE_PROPERTY(t, @value, 1, 0, 0);
    }

    SECTION("list")
    {
        TExt t("mux2~", LF(3), true);

        SigN<6> s0;
        s0.fillInputN(0, 1);
        s0.fillInputN(1, -1);
        s0.fillInputN(2, 2);
        s0.fillInputN(3, -2);
        s0.fillInputN(4, 4);
        s0.fillInputN(5, -4);

        DspN<6> dsp(s0, t);

        t << LF(1, 0.5, 0.25);
        REQUIRE_PROPERTY(t, @value, 1, 0.5, 0.25);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(3));
            REQUIRE(dsp.out(1, i) == Approx(-3));
        }

        t << LF(0.5, 0.5, 0.5);
        REQUIRE_PROPERTY(t, @value, 0.5, 0.5, 0.5);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(3.5));
            REQUIRE(dsp.out(1, i) == Approx(-3.5));
        }
    }
}
