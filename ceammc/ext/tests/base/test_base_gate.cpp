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
#include "base_gate_tilde.h"
#include "ceammc_convert.h"
#include "test_base.h"
#include "test_external.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(BaseGateTilde, base, gate_tilde)

TEST_CASE("gate~", "[externals]")
{
    SECTION("construct")
    {
        SECTION("default")
        {
            TExt t("gate~", L(), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numInputChannels() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.numOutputChannels() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @n, 1);
            REQUIRE_PROPERTY_FLOAT(t, @init, 1);
            REQUIRE_PROPERTY_FLOAT(t, @smooth, 50);
        }

        SECTION("args")
        {
            TExt t("gate~", LF(4), true);
            REQUIRE(t.numInlets() == 5);
            REQUIRE(t.numInputChannels() == 4);
            REQUIRE(t.numOutlets() == 4);
            REQUIRE(t.numOutputChannels() == 4);
            REQUIRE_PROPERTY_FLOAT(t, @n, 4);
            REQUIRE_PROPERTY_FLOAT(t, @init, 1);
        }

        SECTION("props")
        {
            TExt t("gate~", LA("@n", 4, "@init", 0.), true);
            REQUIRE(t.numInlets() == 5);
            REQUIRE(t.numInputChannels() == 4);
            REQUIRE(t.numOutlets() == 4);
            REQUIRE(t.numOutputChannels() == 4);
            REQUIRE_PROPERTY_FLOAT(t, @n, 4);
            REQUIRE_PROPERTY_FLOAT(t, @init, 0);
        }
    }

    SECTION("process 1 |-> 1")
    {
        using DspSignal = TestSignal<1, 1>;
        using DspChain = BaseGateTildeDSP<1, 1>;

        TExt t("gate~", LA(1, "@init", 0., "@smooth", 0.), true);
        REQUIRE_PROPERTY_FLOAT(t, @smooth, 0);
        t.setDspParams(1024, 4);

        REQUIRE_PROPERTY_FLOAT(t, @init, 0);

        DspSignal sig;
        sig.fillInput(1);

        DspChain dsp(sig, t);
        dsp.processBlock();

        for (size_t i = 0; i < t.blockSize(); i++)
            REQUIRE(dsp.out(0, i) == 0);

        sig.fillInput(1);
        dsp.processBlock();

        for (size_t i = 0; i < t.blockSize(); i++)
            REQUIRE(dsp.out(0, i) == 0);

        // turn on
        t.sendFloat(1, 1);
        dsp.processBlock();
        for (size_t i = 0; i < t.blockSize(); i++)
            REQUIRE(dsp.out(0, i) == 1);

        sig.fillInput(10000);
        dsp.processBlock();
        for (size_t i = 0; i < t.blockSize(); i++)
            REQUIRE(dsp.out(0, i) == 10000);

        // turn off
        t.sendFloat(0, 1);
        dsp.processBlock();
        for (size_t i = 0; i < t.blockSize(); i++)
            REQUIRE(dsp.out(0, i) == 0);

        t.setProperty("@smooth", A(convert::tau2msec(convert::pole2tau<double>(0.5, 1024))));

        sig.fillInput(1);
        t.sendFloat(1, 1);
        dsp.processBlock();
        REQUIRE(dsp.out(0, 0) == 0.5);
        REQUIRE(dsp.out(0, 1) == 0.75);
        REQUIRE(dsp.out(0, 2) == 0.875);
        REQUIRE(dsp.out(0, 3) == 0.9375);

        dsp.processBlock(100);
        REQUIRE(dsp.out(0, 0) == 1);
    }

    SECTION("process 4 |-> 4")
    {
        using DspSignal = TestSignal<4, 4>;
        using DspChain = BaseGateTildeDSP<4, 4>;

        TExt t("gate~", LA(4, "@init", 1, "@smooth", 0.), true);
        t.setDspParams(1024, 4);

        DspSignal sig;
        sig.fillInputN(0, 1);
        sig.fillInputN(1, 2);
        sig.fillInputN(2, 3);
        sig.fillInputN(3, 4);

        DspChain dsp(sig, t);
        dsp.processBlock();

        for (size_t i = 0; i < t.blockSize(); i++) {
            REQUIRE(dsp.out(0, i) == 1);
            REQUIRE(dsp.out(1, i) == 2);
            REQUIRE(dsp.out(2, i) == 3);
            REQUIRE(dsp.out(3, i) == 4);
        }

        sig.fillInputN(0, -1);
        sig.fillInputN(1, -2);
        sig.fillInputN(2, -3);
        sig.fillInputN(3, -4);
        dsp.processBlock();

        for (size_t i = 0; i < t.blockSize(); i++) {
            REQUIRE(dsp.out(0, i) == -1);
            REQUIRE(dsp.out(1, i) == -2);
            REQUIRE(dsp.out(2, i) == -3);
            REQUIRE(dsp.out(3, i) == -4);
        }
    }
}
