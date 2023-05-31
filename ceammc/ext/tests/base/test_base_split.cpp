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
#include "base_split_tilde.h"
#include "test_base.h"
#include "test_external.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(BaseSplitTilde, base, split_tilde)

TEST_CASE("split~", "[externals]")
{
    SECTION("construct")
    {
        SECTION("default")
        {
            TExt t("split~", L(), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numInputChannels() == 1);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE(t.numOutputChannels() == 2);
            REQUIRE_PROPERTY_FLOAT(t, @n, 2);
        }

        SECTION("args")
        {
            TExt t("split~", LF(4), true);
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numInputChannels() == 1);
            REQUIRE(t.numOutlets() == 4);
            REQUIRE(t.numOutputChannels() == 4);
            REQUIRE_PROPERTY_FLOAT(t, @n, 4);
        }

        SECTION("props")
        {
            TExt t("split~", LA("@n", 4), true);
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numInputChannels() == 1);
            REQUIRE(t.numOutlets() == 4);
            REQUIRE(t.numOutputChannels() == 4);
            REQUIRE_PROPERTY_FLOAT(t, @n, 4);
        }
    }

    SECTION("process 1 <: 3")
    {
        using DspSignal = TestSignal<1, 3, 4>;
        using DspChain = BaseSplitTildeDSP<1, 3, 4>;

        TExt t("split~", LF(3), true);
        t.setDspParams(44100, 4);
        DspSignal sig;
        sig.fillInputSeq(0, { 1, 2, 3, -1 });

        DspChain dsp(sig, t);
        dsp.processBlock();

        REQUIRE(dsp.out(0, 0) == 1);
        REQUIRE(dsp.out(0, 1) == 2);
        REQUIRE(dsp.out(0, 2) == 3);
        REQUIRE(dsp.out(0, 3) == -1);
        REQUIRE(dsp.out(1, 0) == 1);
        REQUIRE(dsp.out(1, 1) == 2);
        REQUIRE(dsp.out(1, 2) == 3);
        REQUIRE(dsp.out(1, 3) == -1);
        REQUIRE(dsp.out(2, 0) == 1);
        REQUIRE(dsp.out(2, 1) == 2);
        REQUIRE(dsp.out(2, 2) == 3);
        REQUIRE(dsp.out(2, 3) == -1);
    }
}
