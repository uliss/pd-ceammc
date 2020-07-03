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
#include "../fx/fx_looper.h"
#include "test_base.h"
#include "test_external.h"
#include "test_sound.h"

#include <algorithm>
#include <cstdlib>
#include <initializer_list>
#include <numeric>
#include <stdio.h>

PD_COMPLETE_SND_TEST_SETUP(FxLooper, fx, looper)

//typedef std::vector<t_sample> Signal;

class Signal : public std::vector<t_sample> {
public:
    Signal() {}

    Signal(size_t n, t_sample v)
        : std::vector<t_sample>(n, v)
    {
    }

    Signal(std::initializer_list<t_sample> l)
        : std::vector<t_sample>(l)
    {
    }
};

Signal operator+(const Signal& s0, const Signal& s1)
{
    Signal res(s0);
    std::copy(s1.begin(), s1.end(), std::back_inserter(res));
    return res;
}

static Signal sigLin(size_t len, t_sample from = 0, t_sample to = 1)
{
    assert(len > 1);

    Signal res(len, 0);
    res[0] = from;

    t_sample inc = (to - from) / (len - 1);
    for (size_t i = 1; i < len - 1; i++)
        res[i] = res[i - 1] + inc;

    res[len - 1] = to;
    return res;
}

class FxLooperTest : public TestSoundExternal<FxLooper> {
    static const size_t BS = 8;
    static const size_t SR = 512;

public:
    typedef TestSignal<1, 1> Vec;
    Signal output;

public:
    FxLooperTest(const char* name, const AtomList& args = L())
        : TestSoundExternal<FxLooper>(name, args, true)
    {
        FxLooper::setBlockSize(BS);
        FxLooper::setSamplerate(SR);
        setTestSampleRate(SR);
        FxLooper::calcXFades();
    }

    void operator<<(const Signal& v)
    {
        const size_t n = v.size() / BS;
        output.clear();

        auto it = v.begin();
        for (size_t i = 0; i < n; i++) {
            Vec sig;

            for (size_t j = 0; j < BS; j++)
                sig.setInput(j, v[i * BS + j]);

            processBlock(sig.in, sig.out);

            for (size_t j = 0; j < BS; j++)
                output.push_back(sig.out[0][j]);
        }
    }

    void stop()
    {
        m_stop(&s_, L());
    }

    void play()
    {
        m_play(&s_, L());
    }

    void record()
    {
        m_record(&s_, L());
    }

    void overdub()
    {
        m_overdub(&s_, L());
    }

    void clear()
    {
        m_clear(&s_, L());
    }

    template <class P>
    P* prop(const char* name)
    {
        return static_cast<P*>(property(name));
    }
};

template <class F>
static void fill_block(t_sample* b, F f)
{
    std::generate(b, b + 64, f);
}

#define REQUIRE_EQ(v0, v1)                       \
    {                                            \
        REQUIRE(v0.size() == v1.size());         \
        for (size_t i = 0; i < v0.size(); i++) { \
            REQUIRE(v0[i] == Approx(v1[i]));     \
        }                                        \
    }

TEST_CASE("fx.looper~", "[externals]")
{
    setTestSampleRate(512);
    test::pdPrintToStdError();

    SECTION("init")
    {
        FxLooperTest t("fx.looper~", LF(0.125));

        REQUIRE(t.blockSize() == 8);
        REQUIRE(t.samplerate() == 512);
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE(t.numInputChannels() == 1);
        REQUIRE(t.numOutputChannels() == 1);
        REQUIRE(t.maxSamples() == 64);
        REQUIRE_PROPERTY(t, @capacity, 0.125f);
        REQUIRE_PROPERTY(t, @loop_bang, 0.f);
        REQUIRE_PROPERTY(t, @length, 0.f);
        REQUIRE_PROPERTY(t, @play_to_stop_time, 10);
        REQUIRE_PROPERTY(t, @stop_to_play_time, 10);
        REQUIRE_PROPERTY(t, @rec_to_play_time, 30);
        REQUIRE_PROPERTY(t, @dub_to_play_time, 20);

        SECTION("wrong max length")
        {
            FxLooperTest t("fx.looper~", LF(0.f));
            REQUIRE(t.maxSamples() == 5 * 512);
        }

        SECTION("wrong max length")
        {
            FxLooperTest t("fx.looper~", LF(-100));
            REQUIRE(t.maxSamples() == 5 * 512);
        }
    }

    SECTION("pause")
    {
        FxLooperTest t("fx.looper~", LF(0.125));
        REQUIRE(t.maxSamples() == 64);

        REQUIRE(t.state() == STATE_INIT);
        t.m_pause(&s_, L());
        REQUIRE(t.state() == STATE_INIT);

        // record loop
        t.m_record(&s_, L());
        REQUIRE(t.state() == STATE_REC);
        t << Signal({ 1, 1, 1, 1, 1, 1, 1, 1 });
        REQUIRE(t.output == Signal(8, 0));

        t.m_pause(&s_, L());
        REQUIRE(t.state() == STATE_PAUSE);
        REQUIRE(t.loopLengthInSamples() == 8);
        t << Signal(16, -1);
        REQUIRE(t.output == Signal(16, 0));

        t.m_play(&s_, L());
        REQUIRE(t.state() == STATE_PLAY);
        t << Signal(16, -1);
        REQUIRE(t.output == Signal(16, 1));

        t.m_pause(&s_, L());
        REQUIRE(t.state() == STATE_PAUSE);
        t << Signal(16, -1);
        REQUIRE(t.output == Signal(16, 0));

        t.m_play(&s_, L());
        REQUIRE(t.state() == STATE_PLAY);
        t << Signal(16, -1);
        REQUIRE(t.output == Signal(16, 1));
    }

    SECTION("record->stop")
    {
        FxLooperTest t("fx.looper~", LF(0.125));
        REQUIRE(t.maxSamples() == 64);
        REQUIRE_PROPERTY(t, @play_phase, 0.f);
        REQUIRE_PROPERTY(t, @play_pos, 0.f);
        REQUIRE_PROPERTY(t, @length, 0.f);

        t.setProperty("@loop_smooth", LF(0.f));
        t.setProperty("@rec_to_stop_time", LF(0.f));
        REQUIRE(t.state() == STATE_INIT);
        REQUIRE(t.prop<FloatProperty>("@loop_smooth")->value() == 0);

        // record loop
        t.record();
        t << sigLin(16, 0, 1);
        // no output while recording
        REQUIRE(t.output == Signal(16, 0));
        REQUIRE(t.state() == STATE_REC);
        REQUIRE_PROPERTY(t, @state, "record");
        REQUIRE(t.loopLengthInSamples() == 0);

        // stop record
        t.stop();
        REQUIRE(t.state() == STATE_REC_XFADE_STOP);
        REQUIRE(t.loopLengthInSamples() == 16);
        REQUIRE_EQ(t.loop(), sigLin(16, 0, 1));
        REQUIRE_PROPERTY(t, @state, "rec->stop");

        // go to stop
        // no output while stop
        t << Signal(24, 1);
        REQUIRE(t.output == Signal(24, 0));
        REQUIRE(t.state() == STATE_STOP);
        REQUIRE_EQ(t.loop(), sigLin(16, 0, 1));
        REQUIRE_PROPERTY(t, @play_phase, 0.f);
        REQUIRE_PROPERTY(t, @length, 16 / 512.f);

        // clear
        t.clear();
        REQUIRE(t.loopLengthInSamples() == 0);
        t << Signal(24, 1);
        REQUIRE_EQ(t.output, Signal(24, 0));

        // apply fadein/out
        t.setProperty("@loop_smooth", LA(1000 * 8 / 512.0));

        // record again
        t.record();
        t << Signal(24, 10);
        // no output while recording
        REQUIRE_EQ(t.output, Signal(24, 0));
        REQUIRE(t.state() == STATE_REC);
        REQUIRE_PROPERTY(t, @state, "record");
        REQUIRE_PROPERTY(t, @length, 0.f);

        // stop record
        t.stop();
        REQUIRE(t.state() == STATE_REC_XFADE_STOP);
        REQUIRE(t.loopLengthInSamples() == 24);
        REQUIRE_EQ(t.loop(), Signal(24, 10));
        REQUIRE_PROPERTY(t, @state, "rec->stop");

        t << Signal(24, 1);
        const Signal out = sigLin(8, 0, 8.75) + Signal(8, 10) + sigLin(8, 8.75, 0);
        REQUIRE_EQ(t.loop(), out);
        REQUIRE(t.state() == STATE_STOP);
    }

    SECTION("record->play")
    {
        FxLooperTest t("fx.looper~", LF(0.125));
        REQUIRE(t.maxSamples() == 64);
        REQUIRE_PROPERTY(t, @play_phase, 0.f);
        REQUIRE_PROPERTY(t, @play_pos, 0.f);
        REQUIRE_PROPERTY(t, @length, 0.f);

        t.setProperty("@rec_to_play_time", LF(0.f));
        REQUIRE(t.prop<LinFadeinProperty>("@rec_to_play_time")->value() == 0);
        REQUIRE(t.prop<LinFadeinProperty>("@rec_to_play_time")->samples() == 0);

        // record loop
        t.record();
        t << sigLin(16, 0, 1);
        // no output while recording
        REQUIRE_EQ(t.output, Signal(16, 0));
        REQUIRE(t.state() == STATE_REC);
        REQUIRE_PROPERTY(t, @state, "record");
        REQUIRE(t.loopLengthInSamples() == 0);

        // play loop
        t.play();
        REQUIRE(t.state() == STATE_REC_XFADE_PLAY);
        REQUIRE(t.loopLengthInSamples() == 16);
        REQUIRE_EQ(t.loop(), sigLin(16, 0, 1));
        REQUIRE_PROPERTY(t, @state, "rec->play");

        t << Signal(32, 100);
        REQUIRE(t.state() == STATE_PLAY);
        REQUIRE_PROPERTY(t, @state, "play");
        const Signal v = sigLin(16, 0, 1) + sigLin(16, 0, 1);
        REQUIRE_EQ(t.output, v);

        // clear
        t.clear();
        t.setProperty("@rec_to_play_time", LF(20));
        t.record();
        t << Signal(24, 10);

        // no output while recording
        REQUIRE_EQ(t.output, Signal(24, 0));
        REQUIRE(t.state() == STATE_REC);
        REQUIRE_PROPERTY(t, @state, "record");
        REQUIRE(t.loopLengthInSamples() == 0);

        // play loop
        t.play();
        REQUIRE(t.state() == STATE_REC_XFADE_PLAY);
        REQUIRE(t.loopLengthInSamples() == 24);
        REQUIRE_EQ(t.loop(), Signal(24, 10));
        REQUIRE_PROPERTY(t, @state, "rec->play");

        t << Signal(24, 10);
        REQUIRE_PROPERTY(t, @state, "play");
        const Signal ramp({ 10.0f, 12.1875f, 13.75f, 14.6875f, 15.0f, 14.6875f, 13.75f, 12.1875f });
        const Signal out = ramp + Signal(16, 10);
        REQUIRE_EQ(t.loop(), out);

        t << Signal(48, 10);
        auto out2 = out + out;
        REQUIRE_EQ(t.output, out2);
    }

    SECTION("record max loop length")
    {
        FxLooperTest t("fx.looper~", LF(0.125));
        REQUIRE(t.maxSamples() == 64);
        REQUIRE_PROPERTY(t, @play_phase, 0.f);
        REQUIRE_PROPERTY(t, @play_pos, 0.f);
        REQUIRE_PROPERTY(t, @length, 0.f);

        t.setProperty("@loop_smooth", LF(0.f));

        // record loop
        t.record();
        t << sigLin(64, 0, 1);
        // no output while recording
        REQUIRE(t.output == Signal(64, 0));
        REQUIRE(t.state() == STATE_STOP);
        REQUIRE_PROPERTY(t, @state, "stop");
        REQUIRE(t.loopLengthInSamples() == 64);

        // loop length overflow -> stop state
        t << Signal(16, -10);
        REQUIRE(t.state() == STATE_STOP);
        REQUIRE(t.loopLengthInSamples() == 64);
        REQUIRE(t.loop() == sigLin(64, 0, 1));
        REQUIRE(t.buffer() == sigLin(64, 0, 1));

        // no output
        REQUIRE_EQ(t.output, Signal(16, 0));

        // clear
        t.clear();
        t.setProperty("@loop_smooth", LA(1000 * 8 / 512.0));

        // record loop
        t.record();
        t << Signal(128, 10);
        // no output while recording
        REQUIRE_EQ(t.output, Signal(128, 0));
        REQUIRE(t.state() == STATE_STOP);
        REQUIRE_PROPERTY(t, @state, "stop");
        REQUIRE(t.loopLengthInSamples() == 64);
        const Signal reference = sigLin(9, 0, 10) + Signal(46, 10) + sigLin(9, 10, 0);
        REQUIRE_EQ(t.loop(), reference);
        REQUIRE_EQ(t.buffer(), reference);
    }

    SECTION("rec -> dub")
    {
        FxLooperTest t("fx.looper~", LF(0.125));
        REQUIRE(t.maxSamples() == 64);

        t.setProperty("@rec_to_play_time", LF(0.f));

        // record loop
        t.record();
        t << sigLin(16, 0, 1);
        t.play();
        t << Signal(32, 1);
        REQUIRE_EQ(t.loop(), sigLin(16, 0, 1));

        // overdub
        t.overdub();
        REQUIRE(t.state() == STATE_PLAY_XFADE_DUB);
        REQUIRE_PROPERTY(t, @state, "play->dub");
        t << sigLin(16, 0, 1);

        REQUIRE(t.state() == STATE_DUB);
        REQUIRE_PROPERTY(t, @state, "overdub");

        REQUIRE_EQ(t.output, sigLin(16, 0, 1));
        REQUIRE_EQ(t.loop(), sigLin(16, 0, 2));

        // continue overdub
        t << sigLin(16, 0, 1);

        REQUIRE_EQ(t.output, sigLin(16, 0, 2));
        REQUIRE_EQ(t.loop(), sigLin(16, 0, 3));

        t.setProperty("@dub_to_play_time", LF(0.f));

        // play
        t.play();
        REQUIRE(t.state() == STATE_DUB_XFADE_PLAY);
        REQUIRE_PROPERTY(t, @state, "dub->play");

        t << Signal(16, 1);
        REQUIRE_EQ(t.output, sigLin(16, 0, 3));
        t << Signal(16, 1);
        REQUIRE_EQ(t.output, sigLin(16, 0, 3));
    }

    SECTION("bang")
    {
        FxLooperTest t("fx.looper~", LA(0.25, "@loop_smooth", 0.f, "@rec_to_play_time", 0.f));
        REQUIRE(t.maxSamples() == 128);

        t.record();
        t << sigLin(64, 0, 63);
        t.play();
        t << Signal(16, 1);
        REQUIRE(t.property("@play_pos")->get().floatAt(0, -1) == Approx(16 / 512.0));
        REQUIRE_EQ(t.output, sigLin(16, 0, 15));

        // bang
        t.onBang();
        REQUIRE(t.property("@play_pos")->get().floatAt(0, -1) == 0.f);
        t << Signal(16, 1);
        REQUIRE_EQ(t.output, sigLin(16, 0, 15));
    }

    SECTION("stop -> play")
    {
        FxLooperTest t("fx.looper~", LA(0.125, "@loop_smooth", 0.f, "@rec_to_play_time", 0.f));
        REQUIRE(t.maxSamples() == 64);

        t.record();
        t << Signal(64, 1);
        REQUIRE_EQ(t.loop(), Signal(64, 1));
        REQUIRE(t.state() == STATE_STOP);

        t.setProperty("@stop_to_play_time", LF(0.f));
        t.play();
        t << Signal(256, 0);
        REQUIRE_EQ(t.output, Signal(256, 1));

        return;

        // play with fadein
        t.stop();
        t << Signal(256, 0);
        REQUIRE_EQ(t.output, Signal(256, 0));
        t.setProperty("@stop_to_play_time", LF(20));
        t.play();
        t << Signal(256, 0);
        const Signal ref = sigLin(9, 0, 1) + Signal(247, 1);
        REQUIRE_EQ(t.output, ref);
    }

    SECTION("LinFadeinProperty")
    {
        LinFadeinProperty p("@p");
        REQUIRE_FALSE(p.isRunning());
        REQUIRE(p.phase() == 0);
        REQUIRE(p.name() == SYM("@p"));
        REQUIRE(p.samples() == 0);
        REQUIRE(p.value() == 0);
        REQUIRE_FALSE(p.isReadOnly());
        REQUIRE(p.isPublic());

        static const size_t SR = 1000;
        static const size_t BS = 10;

        p.calc(SR, BS);
        REQUIRE(p.phase() == 0);
        REQUIRE(p.samples() == BS * 0);

        p.setValue(100);
        REQUIRE(p.value() == 100);
        p.calc(SR, BS);

        REQUIRE(p.phase() == 0);
        REQUIRE(p.samples() == 100);

        p.setValue(9);
        REQUIRE(p.value() == 9);
        p.calc(SR, BS);

        REQUIRE(p.phase() == 0);
        REQUIRE(p.samples() == 0);

        p.setValue(10);
        REQUIRE(p.value() == 10);
        p.calc(SR, BS);

        REQUIRE(p.phase() == 0);
        REQUIRE(p.samples() == 10);

        p.setValue(90);
        REQUIRE(p.value() == 90);
        p.calc(SR, BS);

        REQUIRE(p.phase() == 0);
        REQUIRE(p.samples() == 90);
    }

    SECTION("clear")
    {
        FxLooperTest t("fx.looper~", LA(0.125, "@loop_smooth", 0.f, "@rec_to_play_time", 0.f));
        REQUIRE(t.maxSamples() == 64);

        t.record();
        t << Signal(32, 1);
        t.clear();
        REQUIRE(t.state() == STATE_STOP);
        REQUIRE(t.loopLengthInSamples() == 0);

        t.record();
        t << Signal(64, 1);
        t.play();
        t << Signal(256, 0);
        t.clear();
        REQUIRE(t.state() == STATE_STOP);
        t << Signal(256, 0);
    }
}
