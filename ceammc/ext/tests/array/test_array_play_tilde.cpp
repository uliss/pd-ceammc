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
#include "array_play_tilde.h"
#include "test_array_base.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(ArrayPlayTilde, array, play_tilde)

using TSig = TestSignal<1, 1>;
using TDsp = ArrayPlayTildeDSP<1, 1>;

static void after_nticks(int n = 1)
{
    while (n-- > 0)
        sched_tick();
}

TEST_CASE("array.play~", "[externals]")
{
    pd_test_init();
    auto cnv = PureData::instance().findCanvas("test_canvas");
    auto eq0 = [](t_sample s) { return s == 0; };
    test::pdPrintToStdError();

    SECTION("test create")
    {
        TExt t("array.play~");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 3);
        REQUIRE_PROPERTY(t, @array, &s_);
        REQUIRE_PROPERTY(t, @speed, 1);
        REQUIRE_PROPERTY(t, @begin, 0);
        REQUIRE_PROPERTY(t, @end, -1);
        REQUIRE_PROPERTY(t, @cursor_ms, 0);
        REQUIRE_PROPERTY(t, @cursor_samp, 0);
        REQUIRE_PROPERTY(t, @cursor_sec, 0);
        REQUIRE_PROPERTY(t, @cursor_phase, 0);
        REQUIRE_PROPERTY(t, @state, 0);
        REQUIRE_PROPERTY(t, @interp, 1);
        REQUIRE_PROPERTY(t, @loop, 0.);
    }

    SECTION("states")
    {
        ArrayPtr aptr = cnv->createArray("array_play~0", 30);

#define M_PLAY(t) t.m_play(SYM("play"), AtomListView())
#define M_STOP(t) t.m_stop(SYM("stop"), AtomListView())
#define M_PAUSE(t) t.m_pause(SYM("pause"), AtomListView())
#define M_PAUSE_V(t, v) t.m_pause(SYM("pause"), LF(v))

        TExt t("array.play~", LA("array_play~0"));
        REQUIRE_PROPERTY(t, @state, 0);

        M_STOP(t);
        REQUIRE_PROPERTY(t, @state, STATE_STOPPED);
        M_PAUSE(t);
        REQUIRE_PROPERTY(t, @state, STATE_STOPPED);
        M_PLAY(t);
        REQUIRE_PROPERTY(t, @state, STATE_PLAYING);

        M_PLAY(t);
        REQUIRE_PROPERTY(t, @state, STATE_PLAYING);
        M_PAUSE(t);
        REQUIRE_PROPERTY(t, @state, STATE_PAUSED);
        M_PAUSE(t);
        REQUIRE_PROPERTY(t, @state, STATE_PAUSED);
        M_PAUSE_V(t, 1);
        REQUIRE_PROPERTY(t, @state, STATE_PAUSED);
        M_PLAY(t);
        REQUIRE_PROPERTY(t, @state, STATE_PLAYING);
        M_STOP(t);
        REQUIRE_PROPERTY(t, @state, STATE_STOPPED);
        M_PLAY(t);
        REQUIRE_PROPERTY(t, @state, STATE_PLAYING);

        M_PAUSE_V(t, 1);
        REQUIRE_PROPERTY(t, @state, STATE_PAUSED);
        M_PAUSE(t);
        REQUIRE_PROPERTY(t, @state, STATE_PAUSED);
        M_PAUSE_V(t, 0);
        REQUIRE_PROPERTY(t, @state, STATE_PLAYING);
        M_PAUSE(t);
        REQUIRE_PROPERTY(t, @state, STATE_PAUSED);
        M_PAUSE(t);
        REQUIRE_PROPERTY(t, @state, STATE_PAUSED);
        M_STOP(t);
        REQUIRE_PROPERTY(t, @state, STATE_STOPPED);
    }

    SECTION("process")
    {
        TSig s0;

        SECTION("none")
        {
            s0.fillOutput(2);

            TExt t("array.play~");
            TDsp dsp(s0, t);
            dsp.processBlock();

            REQUIRE(std::all_of(s0.beginOut(0), s0.endOut(0), eq0));
        }

        SECTION("small array")
        {
            constexpr size_t NA = 30;
            ArrayPtr aptr = cnv->createArray("array_play~1", NA);
            aptr->fillWith([](size_t i) -> t_float { return i; });

            TExt t("array.play~", LA("array_play~1"));
            REQUIRE_PROPERTY(t, @array, SYM("array_play~1"));

            // not playing
            s0.fillOutput(2);
            TDsp dsp(s0, t);
            dsp.processBlock();

            REQUIRE(t.messageCount(0) == 0);
            REQUIRE(t.messageCount(1) == 0);
            REQUIRE(t.messageCount(2) == 0);
            REQUIRE(std::all_of(s0.beginOut(0), s0.endOut(0), eq0));

            // playing
            M_PLAY(t);
            REQUIRE_PROPERTY(t, @state, STATE_PLAYING);
            dsp.processBlock();
            for (size_t i = 0; i < 30; i++)
                REQUIRE(s0.out[0][i] == i);

            for (size_t i = 30; i < s0.blocksize(); i++)
                REQUIRE(s0.out[0][i] == 0);

            after_nticks(1);
            REQUIRE(t.messageCount(2) == 1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE_PROPERTY(t, @state, STATE_STOPPED);
            REQUIRE(t.playPos() == 0);
            REQUIRE(t.position() == 0);

            // speed 2
            t.setProperty("@speed", LF(2));
            REQUIRE_PROPERTY(t, @speed, 2);

            M_PLAY(t);
            REQUIRE_PROPERTY(t, @state, STATE_PLAYING);
            dsp.processBlock();
            for (size_t i = 0; i < 15; i++)
                REQUIRE(s0.out[0][i] == 2 * i);

            for (size_t i = 15; i < s0.blocksize(); i++)
                REQUIRE(s0.out[0][i] == 0);

            REQUIRE(t.position() == 30);

            after_nticks(1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE_PROPERTY(t, @state, STATE_STOPPED);
            REQUIRE(t.position() == 0);

            // speed 0.5
            t.setProperty("@speed", LF(0.5));
            REQUIRE_PROPERTY(t, @speed, 0.5);
            size_t N05 = (NA - 1) / 0.5 + 1;

            M_PLAY(t);
            REQUIRE_PROPERTY(t, @state, STATE_PLAYING);
            dsp.processBlock();
            for (size_t i = 0; i < N05; i++)
                REQUIRE(s0.out[0][i] == Approx(0.5 * i));

            for (size_t i = N05; i < s0.blocksize(); i++)
                REQUIRE(s0.out[0][i] == 0);

            after_nticks(1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE_PROPERTY(t, @state, STATE_STOPPED);

            // speed 0.75
            t.setProperty("@speed", LF(0.75));
            REQUIRE_PROPERTY(t, @speed, 0.75);
            size_t N075 = (NA - 1) / 0.75 + 1;

            M_PLAY(t);
            REQUIRE_PROPERTY(t, @state, STATE_PLAYING);
            dsp.processBlock();
            for (size_t i = 0; i < N075; i++)
                REQUIRE(s0.out[0][i] == Approx(0.75 * i));

            for (size_t i = N075; i < s0.blocksize(); i++)
                REQUIRE(s0.out[0][i] == 0);

            after_nticks(1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE_PROPERTY(t, @state, STATE_STOPPED);

            // speed 0.25
            t.cleanAllMessages();
            t.setProperty("@speed", LF(0.25));
            REQUIRE_PROPERTY(t, @speed, 0.25);

            M_PLAY(t);
            REQUIRE_PROPERTY(t, @state, STATE_PLAYING);
            REQUIRE(t.playPos() == 0);
            after_nticks(1);

            for (size_t i = 0; i < 64; i++)
                REQUIRE(s0.out[0][i] == Approx(0.25 * i));

            REQUIRE_PROPERTY_FLOAT(t, @cursor_ms, 0.333333);
            REQUIRE_PROPERTY_FLOAT(t, @cursor_samp, 16);
            REQUIRE_PROPERTY_FLOAT(t, @cursor_sec, 0.000333333);
            REQUIRE_PROPERTY_FLOAT(t, @cursor_phase, 64 / 120.0);

            REQUIRE(t.messageCount(2) == 0);
            REQUIRE(t.playPos() == Approx(16));

            dsp.processBlock();
            size_t N025 = size_t((NA - 1) / 0.25 + 1) % 64;

            for (size_t i = 0; i < N025; i++)
                REQUIRE(s0.out[0][i] == Approx(16 + 0.25 * i));

            for (size_t i = N025; i < 64; i++)
                REQUIRE(s0.out[0][i] == 0);

            after_nticks(1);
            REQUIRE(t.messageCount(2) == 1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE_PROPERTY(t, @state, STATE_STOPPED);
        }

        SECTION("amp")
        {
            TExt t("array.play~", LA("array_play~1", "@amp", 0.5));
            TDsp dsp(s0, t);

            M_PLAY(t);
            dsp.processBlock();

            for (size_t i = 0; i < 30; i++)
                REQUIRE(s0.out[0][i] == i * 0.5);

            for (size_t i = 30; i < s0.blocksize(); i++)
                REQUIRE(s0.out[0][i] == 0);
        }

        SECTION("interpolation")
        {
            SECTION("none")
            {
                TExt t("array.play~", LA("array_play~1", "@interp", 0., "@speed", 0.5));
                REQUIRE_PROPERTY(t, @interp, 0.);

                TDsp dsp(s0, t);

                M_PLAY(t);
                dsp.processBlock();

                for (size_t i = 0; i < 59; i++)
                    REQUIRE(s0.out[0][i] == std::floor(i * 0.5));

                for (size_t i = 59; i < s0.blocksize(); i++)
                    REQUIRE(s0.out[0][i] == 0);
            }

            SECTION("linear")
            {
                TExt t("array.play~", LA("array_play~1", "@interp", 1, "@speed", 0.5));
                REQUIRE_PROPERTY(t, @interp, 1);

                TDsp dsp(s0, t);

                M_PLAY(t);
                dsp.processBlock();

                for (size_t i = 0; i < 59; i++)
                    REQUIRE(s0.out[0][i] == i * 0.5);

                for (size_t i = 59; i < s0.blocksize(); i++)
                    REQUIRE(s0.out[0][i] == 0);
            }

            SECTION("cubic")
            {
                TExt t("array.play~", LA("array_play~1", "@interp", 3, "@speed", 0.5));
                REQUIRE_PROPERTY(t, @interp, 3);

                TDsp dsp(s0, t);

                M_PLAY(t);
                dsp.processBlock();

                for (size_t i = 0; i < 59; i++)
                    REQUIRE(s0.out[0][i] == Approx(i * 0.5).margin(0.08));

                for (size_t i = 59; i < s0.blocksize(); i++)
                    REQUIRE(s0.out[0][i] == 0);
            }
        }

        SECTION("zero speed")
        {
            TExt t("array.play~", LA("array_play~1", "@interp", 1, "@speed", 0.));

            TDsp dsp(s0, t);

            M_PLAY(t);
            dsp.processBlock();

            for (size_t i = 0; i < 64; i++)
                REQUIRE(s0.out[0][i] == 0);
        }

        SECTION("negative speed")
        {
            SECTION("no interp")
            {
                TExt t("array.play~", LA("array_play~1", "@interp", 0., "@speed", -0.5));
                TDsp dsp(s0, t);

                t.sendBang();
                REQUIRE(t.playPos() == 29);
                dsp.processBlock();

                for (size_t i = 0; i < 59; i++)
                    REQUIRE(s0.out[0][i] == (29 - ((i + 1) / 2)));

                for (size_t i = 59; i < 64; i++)
                    REQUIRE(s0.out[0][i] == 0);

                after_nticks(1);
                REQUIRE(t.messageCount(2) == 1);
                REQUIRE(t.lastMessage(2) == Message::makeBang());
                REQUIRE_PROPERTY(t, @state, STATE_STOPPED);
            }

            SECTION("linear")
            {
                TExt t("array.play~", LA("array_play~1", "@interp", 1, "@speed", -0.5, "@amp", 4));
                TDsp dsp(s0, t);

                t.sendBang();
                REQUIRE(t.playPos() == 29);
                dsp.processBlock();

                for (size_t i = 0; i < 59; i++)
                    REQUIRE(s0.out[0][i] == (29 - i * 0.5) * 4);

                for (size_t i = 59; i < 64; i++)
                    REQUIRE(s0.out[0][i] == 0);

                after_nticks(1);
                REQUIRE(t.messageCount(2) == 1);
                REQUIRE(t.lastMessage(2) == Message::makeBang());
                REQUIRE_PROPERTY(t, @state, STATE_STOPPED);
                REQUIRE(t.playPos() == 29);
            }

            SECTION("cubic")
            {
                TExt t("array.play~", LA("array_play~1", "@interp", 3, "@speed", -0.5));
                TDsp dsp(s0, t);
                s0.fillOutput(2);

                t.sendBang();
                REQUIRE(t.playPos() == 29);
                dsp.processBlock();

                for (size_t i = 0; i < 59; i++)
                    REQUIRE(s0.out[0][i] == Approx(29 - i * 0.5).margin(0.5));

                for (size_t i = 59; i < 64; i++)
                    REQUIRE(s0.out[0][i] == 0);

                after_nticks(1);
                REQUIRE(t.messageCount(2) == 1);
                REQUIRE(t.lastMessage(2) == Message::makeBang());
                REQUIRE_PROPERTY(t, @state, STATE_STOPPED);
                REQUIRE(t.playPos() == 29);
            }
        }
    }

    SECTION("bang")
    {
        SECTION("positive speed")
        {
            TExt t("array.play~", LA("array_play~1", "@speed", 1));
            TSig s0;
            TDsp dsp(s0, t);

            t.sendBang();
            REQUIRE(t.playPos() == 0);

            dsp.processBlock();
            after_nticks(1);
            REQUIRE(t.messageCount(2) == 1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE_PROPERTY(t, @state, STATE_STOPPED);
            REQUIRE(t.playPos() == 0);
        }

        SECTION("zero speed")
        {
            TExt t("array.play~", LA("array_play~1", "@speed", 0.f));
            TSig s0;
            TDsp dsp(s0, t);

            t.sendBang();
            REQUIRE(t.playPos() == 0);

            dsp.processBlock(2);
            after_nticks(100);
            REQUIRE(t.messageCount(2) == 0);
            REQUIRE(t.playPos() == 0);
        }

        SECTION("negative speed")
        {
            TExt t("array.play~", LA("array_play~1", "@speed", -1));
            TSig s0;
            TDsp dsp(s0, t);

            t.sendBang();
            REQUIRE(t.playPos() == 29);

            dsp.processBlock(2);
            after_nticks(100);
            REQUIRE(t.messageCount(2) == 1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE(t.playPos() == 29);
        }
    }

    SECTION("range")
    {
        SECTION("begin 10")
        {
            TExt t("array.play~", LA("array_play~1", "@begin", 10, "@interp", 0.));
            REQUIRE_PROPERTY(t, @begin, 10);
            REQUIRE_PROPERTY(t, @end, -1);

            TSig s0;
            TDsp dsp(s0, t);

            t.sendBang();
            REQUIRE(t.playPos() == 10);

            dsp.processBlock();

            for (size_t i = 0; i < 20; i++)
                REQUIRE(s0.out[0][i] == i + 10);

            for (size_t i = 20; i < 64; i++)
                REQUIRE(s0.out[0][i] == 0);

            after_nticks(1);
            REQUIRE(t.messageCount(2) == 1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE(t.playPos() == 10);
        }

        SECTION("begin -15")
        {
            TExt t("array.play~", LA("array_play~1", "@begin", -15, "@interp", 0.));
            REQUIRE_PROPERTY(t, @begin, -15);
            REQUIRE_PROPERTY(t, @end, -1);

            TSig s0;
            TDsp dsp(s0, t);

            t.sendBang();
            REQUIRE(t.playPos() == 15);

            dsp.processBlock();

            for (size_t i = 0; i < 15; i++)
                REQUIRE(s0.out[0][i] == i + 15);

            for (size_t i = 15; i < 64; i++)
                REQUIRE(s0.out[0][i] == 0);

            after_nticks(1);
            REQUIRE(t.messageCount(2) == 1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE(t.playPos() == 15);
        }

        SECTION("begin 5 end 20")
        {
            TExt t("array.play~", LA("array_play~1", "@begin", 5, "@end", 20));
            REQUIRE_PROPERTY(t, @begin, 5);
            REQUIRE_PROPERTY(t, @end, 20);

            TSig s0;
            TDsp dsp(s0, t);

            t.sendBang();
            REQUIRE(t.playPos() == 5);

            dsp.processBlock();

            for (size_t i = 0; i <= 15; i++)
                REQUIRE(s0.out[0][i] == i + 5);

            for (size_t i = 16; i < 64; i++)
                REQUIRE(s0.out[0][i] == 0);

            after_nticks(1);
            REQUIRE(t.messageCount(2) == 1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE(t.playPos() == 5);
        }

        SECTION("begin -20 end -5")
        {
            TExt t("array.play~", LA("array_play~1", "@begin", -20, "@end", -5));
            REQUIRE_PROPERTY(t, @begin, -20);
            REQUIRE_PROPERTY(t, @end, -5);

            TSig s0;
            TDsp dsp(s0, t);

            t.sendBang();
            REQUIRE(t.playPos() == 10);

            dsp.processBlock();

            for (size_t i = 0; i <= 15; i++)
                REQUIRE(s0.out[0][i] == i + 10);

            for (size_t i = 16; i < 64; i++)
                REQUIRE(s0.out[0][i] == 0);

            after_nticks(1);
            REQUIRE(t.messageCount(2) == 1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE(t.playPos() == 10);
        }

        SECTION("back begin 10")
        {
            TExt t("array.play~", LA("array_play~1", "@begin", 10, "@speed", -1));
            REQUIRE_PROPERTY(t, @begin, 10);
            REQUIRE_PROPERTY(t, @end, -1);

            TSig s0;
            TDsp dsp(s0, t);

            t.sendBang();
            REQUIRE(t.playPos() == 29);

            dsp.processBlock();

            for (size_t i = 0; i < 20; i++)
                REQUIRE(s0.out[0][i] == (29 - i));

            for (size_t i = 20; i < 64; i++)
                REQUIRE(s0.out[0][i] == 0);

            after_nticks(1);
            REQUIRE(t.messageCount(2) == 1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE(t.playPos() == 29);
        }

        SECTION("back begin -10")
        {
            TExt t("array.play~", LA("array_play~1", "@begin", -10, "@speed", -1));
            REQUIRE_PROPERTY(t, @begin, -10);
            REQUIRE_PROPERTY(t, @end, -1);

            TSig s0;
            TDsp dsp(s0, t);

            t.sendBang();
            REQUIRE(t.playPos() == 29);

            dsp.processBlock();

            for (size_t i = 0; i < 10; i++)
                REQUIRE(s0.out[0][i] == (29 - i));

            for (size_t i = 10; i < 64; i++)
                REQUIRE(s0.out[0][i] == 0);

            after_nticks(1);
            REQUIRE(t.messageCount(2) == 1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE(t.playPos() == 29);
        }

        SECTION("back begin -10 end -5")
        {
            TExt t("array.play~", LA("array_play~1", "@begin", -10, "@end", -5, "@speed", -1));
            REQUIRE_PROPERTY(t, @begin, -10);
            REQUIRE_PROPERTY(t, @end, -5);

            TSig s0;
            TDsp dsp(s0, t);

            t.sendBang();
            REQUIRE(t.playPos() == 25);

            dsp.processBlock();

            for (size_t i = 0; i <= 5; i++)
                REQUIRE(s0.out[0][i] == (25 - i));

            for (size_t i = 6; i < 64; i++)
                REQUIRE(s0.out[0][i] == 0);

            after_nticks(1);
            REQUIRE(t.messageCount(2) == 1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE(t.playPos() == 25);
        }

        SECTION("@loop")
        {
            TExt t("array.play~", LA("array_play~1", "@loop", 1, "@speed", 1));
            REQUIRE_PROPERTY(t, @loop, 1);

            REQUIRE(t.blockSize() == 64);
            Array arr("array_play~1");
            REQUIRE(arr.size() == 30);

            TSig s0;
            TDsp dsp(s0, t);

            t.sendBang();
            REQUIRE(t.playPos() == 0);

            dsp.processBlock();

            for (size_t i = 0; i < 30; i++)
                REQUIRE(s0.out[0][i] == i);

            for (size_t i = 30; i < 60; i++)
                REQUIRE(s0.out[0][i] == (i - 30));

            for (size_t i = 60; i < 64; i++)
                REQUIRE(s0.out[0][i] == (i - 60));

            REQUIRE(t.playPos() == 4);
            dsp.processBlock();

            for (size_t i = 0; i < 64; i++)
                REQUIRE(s0.out[0][i] == (i + 4) % 30);

            sched_tick();

            REQUIRE(t.messageCount(2) == 1);
            REQUIRE(t.lastMessage(2) == Message::makeBang());
            REQUIRE(t.playPos() == 12);
        }
    }

    SECTION("range")
    {
        TExt t("array.play~", LA("array_play~1"));
        t.m_range(&s_, AtomList::parseString("1_samp 75%"));
        REQUIRE_PROPERTY(t, @begin, 1);
        REQUIRE_PROPERTY_FLOAT(t, @end, Approx(21.75));

        t.m_range(&s_, AtomList::parseString("25% 75%"));
        REQUIRE_PROPERTY_FLOAT(t, @begin, 7.25);
        REQUIRE_PROPERTY_FLOAT(t, @end, Approx(21.75));

        t.m_range(&s_, AtomList::parseString("0.25* 1*"));
        REQUIRE_PROPERTY_FLOAT(t, @begin, 7.25);
        REQUIRE_PROPERTY_FLOAT(t, @end, Approx(29));
    }

    SECTION("array resize set samples")
    {
        ArrayPtr aptr = cnv->createArray("array_play~10", 40);

        TExt t("array.play~", LA("array_play~10"));

        REQUIRE(t.setProperty("@cursor_samp", LF(30)));
        REQUIRE_PROPERTY_FLOAT(t, @cursor_samp, 30);

        REQUIRE_FALSE(t.setProperty("@cursor_samp", LF(50)));
        REQUIRE_PROPERTY_FLOAT(t, @cursor_samp, 30);

        REQUIRE(aptr->resize(100));
        REQUIRE(t.setProperty("@cursor_samp", LF(90)));
        REQUIRE_PROPERTY_FLOAT(t, @cursor_samp, 90);
    }
}
