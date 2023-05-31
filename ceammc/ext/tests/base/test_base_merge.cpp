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
#include "base_merge_tilde.h"
#include "test_base.h"
#include "test_external.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(BaseMergeTilde, base, merge_tilde)

TEST_CASE("merge~", "[externals]")
{
    SECTION("construct")
    {
        SECTION("default")
        {
            TExt t("merge~", L(), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numInputChannels() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.numOutputChannels() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @n, 2);
        }

        SECTION("args")
        {
            TExt t("merge~", LF(4), true);
            REQUIRE(t.numInlets() == 4);
            REQUIRE(t.numInputChannels() == 4);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.numOutputChannels() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @n, 4);
        }

        SECTION("props")
        {
            TExt t("merge~", LA("@n", 4), true);
            REQUIRE(t.numInlets() == 4);
            REQUIRE(t.numInputChannels() == 4);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.numOutputChannels() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @n, 4);
        }
    }

    SECTION("process 2 :> 1")
    {
        using DspSignal = TestSignal<2, 1>;
        using DspChain = BaseMergeTildeDSP<2, 1>;

        TExt t("merge~", LF(2), true);
        DspSignal sig;
        sig.fillInputN(0, 0.5);
        sig.fillInputN(1, 1.5);

        DspChain dsp(sig, t);
        dsp.processBlock();

        for (size_t i = 0; i < t.blockSize(); i++) {
            REQUIRE(dsp.out(0, i) == 2);
        }

        sig.fillInputN(0, 0.5);
        sig.fillInputN(1, -1.5);
        dsp.processBlock();

        for (size_t i = 0; i < t.blockSize(); i++) {
            REQUIRE(dsp.out(0, i) == -1);
        }
    }

    SECTION("process 32 :> 1")
    {
        using DspSignal = TestSignal<32, 1>;
        using DspChain = BaseMergeTildeDSP<32, 1>;

        TExt t("merge~", LF(32), true);
        DspSignal sig;
        for (int i = 0; i < 32; i++)
            sig.fillInputN(i, i);

        DspChain dsp(sig, t);
        dsp.processBlock();

        for (size_t i = 0; i < t.blockSize(); i++) {
            REQUIRE(dsp.out(0, i) == 31 * 16);
        }
    }
}
