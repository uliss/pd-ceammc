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
#include "ceammc_pd.h"
#include "mix.h"
#include "test_sound.h"

#include <algorithm>
#include <cstdlib>
#include <stdio.h>

typedef TestSoundExternal<Mix> TestMix;

template <size_t N = 2>
class TestMixSignal : public TestSignal<N, 1> {
public:
    TestMixSignal(t_sample in = 0, t_sample out = 0)
        : TestSignal<N, 1>(in, out)
    {
    }
};

template <size_t N = 2>
class TestMixDSP : public DSP<TestMixSignal<N>, TestMix> {
public:
    TestMixDSP(TestMixSignal<N>& signal, TestMix& testObject)
        : DSP<TestMixSignal<N>, TestMix>(signal, testObject)
    {
    }
};

TEST_CASE("mix~", "[externals]")
{
    test::pdPrintToStdError();

    SECTION("construct")
    {
        SECTION("default")
        {
            TestMix t("mix~", L(), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numInputChannels() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.numOutputChannels() == 1);
            REQUIRE_PROPERTY_LIST(t, @value, LX(0, 0));
            REQUIRE_PROPERTY_LIST(t, @mute, LX(0, 0));
            REQUIRE_PROPERTY_LIST(t, @solo, LX(0, 0));
        }

        SECTION("args")
        {
            TestMix t("mix~", LF(4), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 4);
            REQUIRE(t.numInputChannels() == 4);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.numOutputChannels() == 1);
            REQUIRE_PROPERTY_LIST(t, @db, LX(-144, -144, -144, -144));
            REQUIRE_PROPERTY_LIST(t, @value, LX(0, 0, 0, 0));
            REQUIRE_PROPERTY_LIST(t, @mute, LX(0, 0, 0, 0));
            REQUIRE_PROPERTY_LIST(t, @solo, LX(0, 0, 0, 0));
        }
    }

    SECTION("process all")
    {
        TestMix t("mix~", LF(2), true);

        TestMixSignal<2> s0;
        s0.fillInputN(0, 10);
        s0.fillInputN(1, 6);

        TestMixDSP<2> dsp(s0, t);
        dsp.processBlock();

        // init mix state
        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) == Approx(0));

        WHEN_SEND_LIST_TO(0, t, LA(0.5, 2));
        REQUIRE_NO_MSG(t);

        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) == Approx(10 * 0.5 + 2 * 6));

        WHEN_SEND_LIST_TO(0, t, LA(0.1, 2));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) == Approx(1 + 2 * 6));

        t.setProperty("@mute", LF(1, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) == Approx(0));

        t.setProperty("@mute", LF(0.f, 1));
        REQUIRE_PROPERTY_LIST(t, @mute, LX(0, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) == Approx(1 + 0 * 6));

        t.setProperty("@mute", LF(1, 0.f));
        REQUIRE_PROPERTY_LIST(t, @mute, LX(1, 0));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) == Approx(1 * 0 + 2 * 6));

        t.setProperty("@mute", LF(0.f, 0.f));
        REQUIRE_PROPERTY_LIST(t, @mute, LX(0, 0));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) == Approx(1 + 2 * 6));

        t.setProperty("@solo", LF(1, 0.f));
        REQUIRE_PROPERTY_LIST(t, @solo, LX(1, 0));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) == Approx(1 + 0 * 6));

        t.setProperty("@solo", LF(0.f, 1));
        REQUIRE_PROPERTY_LIST(t, @solo, LX(0, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) == Approx(1 * 0 + 2 * 6));

        t.setProperty("@solo", LF(1, 1));
        REQUIRE_PROPERTY_LIST(t, @solo, LX(1, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) == Approx(1 + 2 * 6));

        t.setProperty("@solo", LF(0.f, 0.f));
        REQUIRE_PROPERTY_LIST(t, @solo, LX(0, 0));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) == Approx(1 + 2 * 6));

        WHEN_CALL_N(t, mute, 0.f, 1);
        REQUIRE_PROPERTY_LIST(t, @mute, LX(1, 0));
        WHEN_CALL_N(t, mute, 0.f, 0.f);
        REQUIRE_PROPERTY_LIST(t, @mute, LX(0, 0));
        WHEN_CALL_N(t, mute, 1, 1);
        REQUIRE_PROPERTY_LIST(t, @mute, LX(0, 1));
        WHEN_CALL_N(t, mute, 1, 0.f);
        REQUIRE_PROPERTY_LIST(t, @mute, LX(0, 0));

        WHEN_CALL_N(t, solo, 0.f, 1);
        REQUIRE_PROPERTY_LIST(t, @solo, LX(1, 0));
        WHEN_CALL_N(t, solo, 0.f, 0.f);
        REQUIRE_PROPERTY_LIST(t, @solo, LX(0, 0));
        WHEN_CALL_N(t, solo, 1, 1);
        REQUIRE_PROPERTY_LIST(t, @solo, LX(0, 1));
        WHEN_CALL_N(t, solo, 1, 0.f);
        REQUIRE_PROPERTY_LIST(t, @solo, LX(0, 0));
    }

    SECTION("props")
    {
        TestMix t("mix~", LF(2), true);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0, 0));
        REQUIRE_PROPERTY_LIST(t, @db, LX(-144, -144));

        WHEN_SEND_LIST_TO(0, t, LF(0.5, 0.5));
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.5, 0.5));
        REQUIRE_PROPERTY_LIST(t, @db, LX(-6.0206, -6.0206));

        WHEN_SEND_LIST_TO(0, t, LF(0.25, 0.5));
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.25, 0.5));
        REQUIRE_PROPERTY_LIST(t, @db, LX(-12.0412, -6.0206));

        WHEN_SEND_LIST_TO(0, t, LF(2));
        REQUIRE_PROPERTY_LIST(t, @value, LX(2, 0.5));
        REQUIRE_PROPERTY_LIST(t, @db, LX(6.0206, -6.0206));

        t.setProperty("@value", LF(1, 2));
        REQUIRE_PROPERTY_LIST(t, @value, LX(1, 2));

        t.setProperty("@db", LA(-4, -5));
        REQUIRE_PROPERTY_LIST(t, @db, LX(-4, -5));
    }

    SECTION("methods")
    {
        TestMix t("mix~", LA(2, "@value", 1, 1), true);
        REQUIRE_PROPERTY_LIST(t, @value, LX(1, 1));
    }

    SECTION("alias")
    {
        TestMix t("ceammc/mix~");
    }
}
