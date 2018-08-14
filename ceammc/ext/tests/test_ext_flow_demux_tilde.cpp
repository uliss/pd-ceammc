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
#include "../flow/flow_demultiplex_tilde.h"
#include "ceammc_pd.h"
#include "test_sound.h"

#include <algorithm>
#include <cstdlib>
#include <stdio.h>

typedef TestSoundExternal<DemultiplexTilde> TestDemux;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

template <size_t N = 2>
class TestDemuxSignal : public TestSignal<1, N> {
public:
    TestDemuxSignal(t_sample in = 0, t_sample out = 0)
        : TestSignal<1, N>(in, out)
    {
    }
};

template <size_t N = 2>
class TestDemuxDSP : public DSP<TestDemuxSignal<N>, TestDemux> {
public:
    TestDemuxDSP(TestDemuxSignal<N>& signal, TestDemux& testObject)
        : DSP<TestDemuxSignal<N>, TestDemux>(signal, testObject)
    {
    }
};

TEST_CASE("flow.demux~", "[externals]")
{
    SECTION("construct")
    {
        SECTION("default")
        {
            TestDemux t("demux~", L(), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numInputChannels() == 1);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE(t.numOutputChannels() == 2);
            REQUIRE_PROPERTY_LIST(t, @value, LX(1, 0));
        }

        SECTION("args")
        {
            TestDemux t("demux~", LF(4), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numInputChannels() == 1);
            REQUIRE(t.numOutlets() == 4);
            REQUIRE(t.numOutputChannels() == 4);
            REQUIRE_PROPERTY_LIST(t, @value, LX(1, 0, 0, 0));
        }
    }

    SECTION("index")
    {
        TestDemux t("demux~", LF(3), true);

        TestDemuxSignal<3> s0;
        s0.fillInputN(0, 10);

        TestDemuxDSP<3> dsp(s0, t);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(10));
            REQUIRE(dsp.out(1, i) == Approx(0));
            REQUIRE(dsp.out(2, i) == Approx(0));
        }

        WHEN_SEND_FLOAT_TO(1, t, 0);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(10));
            REQUIRE(dsp.out(1, i) == Approx(0));
            REQUIRE(dsp.out(2, i) == Approx(0));
        }

        WHEN_SEND_FLOAT_TO(1, t, 1);
        REQUIRE_PROPERTY_LIST(t, @value, LF(0.f, 1, 0.f));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
            REQUIRE(dsp.out(1, i) == Approx(10));
            REQUIRE(dsp.out(2, i) == Approx(0));
        }

        WHEN_SEND_FLOAT_TO(1, t, 2);
        REQUIRE_PROPERTY_LIST(t, @value, LF(0.f, 0.f, 1));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
            REQUIRE(dsp.out(1, i) == Approx(0));
            REQUIRE(dsp.out(2, i) == Approx(10));
        }

        WHEN_SEND_FLOAT_TO(1, t, 0);
        REQUIRE_PROPERTY_LIST(t, @value, LF(1, 0.f, 0.f));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(10));
            REQUIRE(dsp.out(1, i) == Approx(0));
            REQUIRE(dsp.out(2, i) == Approx(0));
        }

        // invalid inlet
        WHEN_SEND_FLOAT_TO(1, t, 3);
        REQUIRE_PROPERTY_LIST(t, @value, LF(1, 0.f, 0.f));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(10));
            REQUIRE(dsp.out(1, i) == Approx(0));
            REQUIRE(dsp.out(2, i) == Approx(0));
        }

        // invalid inlet
        WHEN_SEND_FLOAT_TO(1, t, -1);
        REQUIRE_PROPERTY_LIST(t, @value, LF(1, 0.f, 0.f));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(10));
            REQUIRE(dsp.out(1, i) == Approx(0));
            REQUIRE(dsp.out(2, i) == Approx(0));
        }
    }

    SECTION("index")
    {
        TestDemux t("demux~", LF(3), true);

        TestDemuxSignal<3> s0;
        s0.fillInputN(0, 10);

        TestDemuxDSP<3> dsp(s0, t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 1, 1));
        REQUIRE_PROPERTY_LIST(t, @value, LF(1, 1, 1));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(10));
            REQUIRE(dsp.out(1, i) == Approx(10));
            REQUIRE(dsp.out(2, i) == Approx(10));
        }

        WHEN_SEND_LIST_TO(0, t, LA(0.5, 0.5, 0.5));
        REQUIRE_PROPERTY_LIST(t, @value, LA(0.5, 0.5, 0.5));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(5));
            REQUIRE(dsp.out(1, i) == Approx(5));
            REQUIRE(dsp.out(2, i) == Approx(5));
        }
    }
}
