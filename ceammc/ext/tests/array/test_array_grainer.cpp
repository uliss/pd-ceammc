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
#include "array_grainer.h"
#include "grain_random.h"
#include "test_array_base.h"
#include "test_sound.h"

#include <cmath>
#include <random>

PD_COMPLETE_SND_TEST_SETUP(ArrayGrainer, array, grainer)

TEST_CASE("array.grainer", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("GrainPool")
    {
        auto& gp = GrainPool::instance();
        REQUIRE(gp.capacity() == 2048);

        Grain* g0 = gp.allocate();
        REQUIRE(g0);
        REQUIRE(gp.capacity() == 2047);
        REQUIRE(g0->startInSamples() == 0);
        REQUIRE(g0->endInSamples() == 0);
        REQUIRE(g0->lengthInSamples() == 0);

        Grain* g1 = gp.allocate(10, 20, 40);
        REQUIRE(g1);
        REQUIRE(gp.capacity() == 2046);
        REQUIRE(g1->arrayPosInSamples() == 10);
        REQUIRE(g1->lengthInSamples() == 20);
        REQUIRE(g1->startInSamples() == 40);
        REQUIRE(g1->endInSamples() == 60);

        g1->setSpeed(2);
        REQUIRE(g1->lengthInSamples() == 10);
        g1->setSpeed(0.5);
        REQUIRE(g1->lengthInSamples() == 40);

        gp.free(g0);
        REQUIRE(gp.capacity() == 2047);
    }

    SECTION("Grain")
    {
        ArrayPtr aptr = cnv->createArray("array_g0", 20);
        aptr->fillWith([](size_t i) -> t_float { return i; });
        REQUIRE(aptr->at(0) == 0);
        REQUIRE(aptr->at(1) == 1);
        REQUIRE(aptr->at(2) == 2);
        REQUIRE(aptr->at(3) == 3);

        Grain* g = GrainPool::instance().allocate(3, 7, 6);
        // 0 1 2 3 4 5 6 7 8 9 10...
        // . . . 3 4 5 6 7 8 9 10 0
        REQUIRE(g);
        REQUIRE(g->speed() == 1);
        REQUIRE(g->lengthInSamples() == 7);
        REQUIRE(g->endInSamples() == 13);
        REQUIRE(g->pan() == 0);
        REQUIRE(g->panNorm() == 0.5);
        REQUIRE(g->panOverflow() == Grain::PAN_OVERFLOW_CLIP);
        g->setPan(-1);
        REQUIRE(g->pan() == -1);
        REQUIRE(g->panNorm() == 0);

        constexpr size_t BS = 4;
        t_sample buf0[BS] = { 0 };
        t_sample buf1[BS] = { 0 };
        t_sample* buf[] = { buf0, buf1 };

        // invalid grain state
        REQUIRE(g->playStatus() == Grain::FINISHED);
        g->process(aptr->begin(), aptr->size(), buf, BS);
        REQUIRE(std::all_of(std::begin(buf0), std::end(buf0), [](t_sample s) { return s == 0; }));
        REQUIRE(g->playStatus() == Grain::FINISHED);

        // out of range play position
        g->start(aptr->size());
        g->process(aptr->begin(), aptr->size(), buf, BS);
        REQUIRE(std::all_of(std::begin(buf0), std::end(buf0), [](t_sample s) { return s == 0; }));
        REQUIRE(g->playStatus() == Grain::FINISHED);

        // play out of block
        g->start(0);
        g->process(aptr->begin(), aptr->size(), buf, BS);
        REQUIRE(std::all_of(std::begin(buf0), std::end(buf0), [](t_sample s) { return s == 0; }));
        REQUIRE(g->playStatus() == Grain::PLAYING);

        // play 0, 0, 3, 4
        g->start(BS);
        g->process(aptr->begin(), aptr->size(), buf, BS);
        REQUIRE(buf0[0] == 0);
        REQUIRE(buf0[1] == 0);
        REQUIRE(buf0[2] == 3);
        REQUIRE(buf0[3] == 4);
        REQUIRE(g->playStatus() == Grain::PLAYING);

        // play again - 0, 0, 3, 4
        g->start(BS);
        g->process(aptr->begin(), aptr->size(), buf, BS);
        REQUIRE(buf0[0] == 0);
        REQUIRE(buf0[1] == 0);
        REQUIRE(buf0[2] == 6);
        REQUIRE(buf0[3] == 8);
        REQUIRE(g->playStatus() == Grain::PLAYING);

        std::fill(std::begin(buf0), std::end(buf0), 0);
        g->process(aptr->begin(), aptr->size(), buf, BS);
        REQUIRE(buf0[0] == 5);
        REQUIRE(buf0[1] == 6);
        REQUIRE(buf0[2] == 7);
        REQUIRE(buf0[3] == 8);
        REQUIRE(g->playStatus() == Grain::PLAYING);

        std::fill(std::begin(buf0), std::end(buf0), 0);
        g->process(aptr->begin(), aptr->size(), buf, BS);
        REQUIRE(buf0[0] == 9);
        REQUIRE(buf0[1] == 0);
        REQUIRE(buf0[2] == 0);
        REQUIRE(buf0[3] == 0);
        REQUIRE(g->playStatus() == Grain::FINISHED);
    }

    SECTION("GrainCloud")
    {
        GrainCloud gc;

        auto g0 = GrainPool::instance().allocate(100, 10, 11);
        gc.addGrain(g0);
        REQUIRE(g0->startInSamples() == 11);
        REQUIRE(g0->endInSamples() == 21);
        REQUIRE(gc.size() == 1);
        REQUIRE(gc.grains()[0].time_samp == 11);
        REQUIRE(gc.offset() == 11);
        REQUIRE(gc.length() == 10);

        auto g1 = GrainPool::instance().allocate(100, 10, 100);
        gc.addGrain(g1);
        REQUIRE(gc.grains()[0].time_samp == 11);
        REQUIRE(gc.grains()[1].time_samp == 100);
        REQUIRE(gc.size() == 2);
        REQUIRE(gc.offset() == 11);
        REQUIRE(gc.length() == 99);

        auto g2 = GrainPool::instance().allocate(100, 10, 60);
        gc.addGrain(g2);
        REQUIRE(gc.grains()[0].time_samp == 11);
        REQUIRE(gc.grains()[1].time_samp == 60);
        REQUIRE(gc.grains()[2].time_samp == 100);
        REQUIRE(gc.size() == 3);
        REQUIRE(gc.offset() == 11);
        REQUIRE(gc.length() == 99);
    }

    SECTION("do single grain")
    {
        ArrayPtr aptr = cnv->createArray("array_g1", 128);
        aptr->fillWith([](size_t i) -> t_float { return i; });

        TExt t("array.grainer~", LA("array_g1"));
        REQUIRE_PROPERTY(t, @array, "array_g1");

        TestSignal<0, 2> s0;

        REQUIRE(t.cloud().empty());
        t.m_grain(&s_, LA("@at", 4, "@l", 10, "@speed", 1, "@pan", -1));
        REQUIRE(t.cloud().size() == 1);
        t.dump();

        DSP<TestSignal<0, 2>, TExt> dsp(s0, t);
        dsp.processBlock();

        for (size_t i = 0; i < 10; i++) {
            REQUIRE(s0.out[0][i] == i + 4);
        }

        for (size_t i = 10; i < 64; i++) {
            REQUIRE(s0.out[0][i] == 0);
        }

        s0.fillOutput(0);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(s0.out[0][i] == 0);
        }

        dsp.processBlock();
        for (size_t i = 0; i < 10; i++) {
            REQUIRE(s0.out[0][i] == i + 4);
        }

        for (size_t i = 10; i < 64; i++) {
            REQUIRE(s0.out[0][i] == 0);
        }
    }

    SECTION("do empty")
    {
        ArrayPtr aptr = cnv->createArray("array_g2", 20);
        TExt t("array.grainer~", LA("array_g2"));
        REQUIRE_PROPERTY(t, @array, "array_g2");

        TestSignal<0, 2> s0;
        s0.fillOutput(10);
        DSP<TestSignal<0, 2>, TExt> dsp(s0, t);
        dsp.processBlock();

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(s0.out[0][i] == 0);
        }
    }

    SECTION("Grain @speed ondone")
    {
        TExt t("array.grainer~", LA("array_g1"));
        float speed = 0;

#define MSG_GRAIN(t, str) t.m_grain(gensym("grain"), AtomList::parseString(str));
#define GET_SPEED(t) t.cloud().grains().back().grain->speed()
#define GRAIN_DONE(t) t.cloud().grains().back().grain->done()
#define CLEAR(t) t.m_clear(&s_, {})

        t.m_grain(&s_, AtomList::parseString("@speed 2.5"));
        REQUIRE(GET_SPEED(t) == 2.5);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed -4"));
        REQUIRE(GET_SPEED(t) == -4);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed -10"));
        REQUIRE(GET_SPEED(t) == -10);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed 10"));
        REQUIRE(GET_SPEED(t) == 10);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed 10.1"));
        REQUIRE(GET_SPEED(t) == 10);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed -10.1"));
        REQUIRE(GET_SPEED(t) == -10);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed random 1 2"));
        speed = GET_SPEED(t);
        REQUIRE(speed >= 1);
        REQUIRE(speed <= 2);
        t.m_clear(&s_, {});

        MSG_GRAIN(t, "@speed 5 @speed ondone add 0.5");
        REQUIRE(GET_SPEED(t) == 5);
        GRAIN_DONE(t);
        REQUIRE(GET_SPEED(t) == 5.5);
        GRAIN_DONE(t);
        REQUIRE(GET_SPEED(t) == 6);
        CLEAR(t);

        MSG_GRAIN(t, "@speed 5 @speed ondone add 40");
        REQUIRE(GET_SPEED(t) == 5);
        GRAIN_DONE(t);
        REQUIRE(GET_SPEED(t) == 10);
        CLEAR(t);

        MSG_GRAIN(t, "@speed 5 @speed ondone add -0.5");
        REQUIRE(GET_SPEED(t) == 5);
        GRAIN_DONE(t);
        REQUIRE(GET_SPEED(t) == 4.5);
        CLEAR(t);

        MSG_GRAIN(t, "@speed 5 @speed ondone set -2");
        REQUIRE(GET_SPEED(t) == 5);
        GRAIN_DONE(t);
        REQUIRE(GET_SPEED(t) == -2);
        GRAIN_DONE(t);
        REQUIRE(GET_SPEED(t) == -2);
        CLEAR(t);

        GrainRandom::seed(0);
        MSG_GRAIN(t, "@speed 5 @speed ondone set random 2 5");
        REQUIRE(GET_SPEED(t) == 5);
        GRAIN_DONE(t);
        REQUIRE(GET_SPEED(t) == Approx(3.64644));
        GRAIN_DONE(t);
        REQUIRE(GET_SPEED(t) == Approx(3.77853));
        CLEAR(t);
    }

    SECTION("Grain @speed ondone")
    {
        TExt t("array.grainer~", LA("array_g1"));
        MSG_GRAIN(t, "@speed range -1 1");
        REQUIRE(t.cloud().grains().back().grain->speedRange() == std::pair<float, float>(-1, 1));

        MSG_GRAIN(t, "@speed range 3 1");
        REQUIRE(t.cloud().grains().back().grain->speedRange() == std::pair<float, float>(1, 3));
    }

    SECTION("Grain @speed set expr")
    {
        TExt t("array.grainer~", LA("array_g1"));
        MSG_GRAIN(t, "@speed expr 2^3");
        REQUIRE(GET_SPEED(t) == 8);

        MSG_GRAIN(t, "@speed expr $sr/10000");
        REQUIRE(GET_SPEED(t) == Approx(4.8));
    }

    SECTION("Grain @speed -1")
    {
        TExt t("array.grainer~", LA("array_g1"));
        MSG_GRAIN(t, "@at 0 @l 20 @speed -1 @pan mode none");
        REQUIRE(GET_SPEED(t) == -1);

        TestSignal<0, 2> s0;

        DSP<TestSignal<0, 2>, TExt> dsp(s0, t);
        dsp.processBlock();

        for (size_t i = 0; i < 20; i++) {
            REQUIRE(s0.out[0][i] == (19 - i));
        }

        for (size_t i = 20; i < 64; i++) {
            REQUIRE(s0.out[0][i] == 0);
        }
    }

    SECTION("Grain @amp")
    {
        TExt t("array.grainer~", LA("array_g1"));
        MSG_GRAIN(t, "@amp 0.5");
        REQUIRE(t.cloud().grains().back().grain->amplitude() == 0.5);
    }

    SECTION("Grain @amp random")
    {
        GrainRandom::seed(0);

        TExt t("array.grainer~", LA("array_g1"));
        MSG_GRAIN(t, "@amp range 1 20 @amp random 10 20");
        REQUIRE(t.cloud().grains().back().grain->amplitude() == Approx(15.48814));
        REQUIRE(t.cloud().grains().back().grain->amplitudeRange() == std::pair<float, float>(1, 20));
    }
}
