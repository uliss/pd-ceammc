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

using DspVector = std::vector<t_sample>;

static DspVector make_seq(const DspVector& pattern, const DspVector& pre = {}, size_t n = 64)
{
    DspVector res(pre);
    auto t = n / pattern.size();
    for (int i = 0; i <= t; i++)
        res.insert(res.end(), pattern.begin(), pattern.end());

    res.resize(n);
    return res;
}

static DspVector make_seq(int from, int to, const DspVector& pre = {}, size_t n = 64)
{
    DspVector res(pre);
    auto t = n / std::abs(from - to + 1);
    for (int i = 0; i <= t; i++) {
        for (int k = from; k < to; k++)
            res.push_back(k);

        for (int k = from; k > to; k--)
            res.push_back(k);
    }

    res.resize(n);
    return res;
}

PD_COMPLETE_SND_TEST_SETUP(ArrayGrainer, array, grainer)

TEST_CASE("array.grainer", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("GrainPool")
    {
        GrainCloud c;
        auto g = c.appendGrain(20, 40, 3);
        REQUIRE(g);
        REQUIRE(g->arrayPosInSamples() == 20);
        REQUIRE(g->lengthInSamples() == 40);
        REQUIRE(g->timeBefore() == 3);
    }

    SECTION("Grain")
    {
        ArrayPtr aptr = cnv->createArray("array_g0", 20);
        aptr->fillWith([](size_t i) -> t_float { return i; });
        REQUIRE(aptr->at(0) == 0);
        REQUIRE(aptr->at(1) == 1);
        REQUIRE(aptr->at(2) == 2);
        REQUIRE(aptr->at(3) == 3);

        GrainCloud c;
        Grain* g = c.appendGrain(3, 7, 6);
        // 0 1 2 3 4 5 6 7 8 9 10...
        // . . . 3 4 5 6 7 8 9 10 0
        REQUIRE(g);
        REQUIRE(g->speed() == 1);
        REQUIRE(g->durationInSamples() == 7);
        REQUIRE(g->endInSamples() == 13);
        REQUIRE(g->timeBefore() == 6);
        g->setTimeAfter(1);
        REQUIRE(g->timeAfter() == 1);
        REQUIRE(g->amplitude() == 1);
        REQUIRE(g->pan() == 0.5);
        REQUIRE(g->panOverflow() == GRAIN_PROP_OVERFLOW_CLIP);
        REQUIRE(g->winType() == GRAIN_WIN_RECT);
        g->setPan(-1);
        REQUIRE(g->pan() == 0);

        constexpr size_t BS = 4;
        constexpr size_t SR = 48000;
        t_sample buf0[BS] = { 0 };
        t_sample buf1[BS] = { 0 };
        t_sample* buf[] = { buf0, buf1 };

        // invalid grain state
        REQUIRE(g->playStatus() == GRAIN_FINISHED);
        g->process(aptr->begin(), aptr->size(), buf, BS, SR);
        REQUIRE(std::all_of(std::begin(buf0), std::end(buf0), [](t_sample s) { return s == 0; }));
        REQUIRE(g->playStatus() == GRAIN_FINISHED);
        g->resetFirstTime();

        // out of range play position
        g->start(aptr->size());
        g->process(aptr->begin(), aptr->size(), buf, BS, SR);
        REQUIRE(std::all_of(std::begin(buf0), std::end(buf0), [](t_sample s) { return s == 0; }));
        REQUIRE(g->playStatus() == GRAIN_FINISHED);
        g->resetFirstTime();

        // play out of grain block
        g->start(0);
        g->process(aptr->begin(), aptr->size(), buf, BS, SR);
        REQUIRE(buf0[0] == 0);
        REQUIRE(buf0[1] == 0);
        REQUIRE(buf0[2] == 0);
        REQUIRE(buf0[3] == 0);
        REQUIRE(g->playStatus() == GRAIN_PLAYING);

        // play 0, 0, 3, 4
        g->start(BS);
        g->process(aptr->begin(), aptr->size(), buf, BS, SR);
        REQUIRE(buf0[0] == 0);
        REQUIRE(buf0[1] == 0);
        REQUIRE(buf0[2] == 3);
        REQUIRE(buf0[3] == 4);
        REQUIRE(g->playStatus() == GRAIN_PLAYING);

        // play again - 0, 0, 3, 4: add to buffer values
        g->start(BS);
        g->process(aptr->begin(), aptr->size(), buf, BS, SR);
        REQUIRE(buf0[0] == 0);
        REQUIRE(buf0[1] == 0);
        REQUIRE(buf0[2] == 6);
        REQUIRE(buf0[3] == 8);
        REQUIRE(g->playStatus() == GRAIN_PLAYING);

        std::fill(std::begin(buf0), std::end(buf0), 0);
        g->process(aptr->begin(), aptr->size(), buf, BS, SR);
        REQUIRE(buf0[0] == 5);
        REQUIRE(buf0[1] == 6);
        REQUIRE(buf0[2] == 7);
        REQUIRE(buf0[3] == 8);
        REQUIRE(g->playStatus() == GRAIN_PLAYING);

        std::fill(std::begin(buf0), std::end(buf0), 0);
        g->process(aptr->begin(), aptr->size(), buf, BS, SR);
        REQUIRE(buf0[0] == 9);
        REQUIRE(buf0[1] == 0);
        REQUIRE(buf0[2] == 0);
        REQUIRE(buf0[3] == 0);
        REQUIRE(g->playStatus() == GRAIN_FINISHED);

        std::fill(std::begin(buf0), std::end(buf0), 0);
        g->process(aptr->begin(), aptr->size(), buf, BS, SR);
        REQUIRE(buf0[0] == 0);
        REQUIRE(buf0[1] == 0);
        REQUIRE(buf0[2] == 0);
        REQUIRE(buf0[3] == 0);
        REQUIRE(g->playStatus() == GRAIN_FINISHED);
    }

    SECTION("Grain window")
    {
        ArrayPtr aptr = cnv->createArray("array_g01", 20);
        aptr->fillWith(1);

        GrainCloud c;
        Grain* g = c.appendGrain(0, 9, 0);

        REQUIRE(g);
        REQUIRE(g->speed() == 1);
        REQUIRE(g->durationInSamples() == 9);
        REQUIRE(g->endInSamples() == 9);
        REQUIRE(g->pan() == 0.5);
        REQUIRE(g->panOverflow() == GRAIN_PROP_OVERFLOW_CLIP);
        REQUIRE(g->winType() == GRAIN_WIN_RECT);
        g->setPan(-1);
        g->setWinType(GRAIN_WIN_TRI);
        REQUIRE(g->pan() == 0);
        REQUIRE(g->winType() == GRAIN_WIN_TRI);

        constexpr size_t BS = 20;
        constexpr size_t SR = 48000;
        t_sample buf0[BS] = { 0 };
        t_sample buf1[BS] = { 0 };
        t_sample* buf[] = { buf0, buf1 };

        g->start(0);
        g->process(aptr->begin(), aptr->size(), buf, BS, SR);

        REQUIRE(buf0[0] == 0);
        REQUIRE(buf0[1] == 0.25);
        REQUIRE(buf0[2] == 0.5);
        REQUIRE(buf0[3] == 0.75);
        REQUIRE(buf0[4] == 1);
        REQUIRE(buf0[5] == 0.75);
        REQUIRE(buf0[6] == 0.5);
        REQUIRE(buf0[7] == 0.25);
        REQUIRE(buf0[8] == 0);

        g->setWinType(GRAIN_WIN_TRPZ);
        REQUIRE(g->winType() == GRAIN_WIN_TRPZ);
        std::fill_n(buf0, BS, 0);
        std::fill_n(buf1, BS, 0);

        g->start(0);
        g->process(aptr->begin(), aptr->size(), buf, BS, SR);

        REQUIRE(buf0[0] == 0);
        REQUIRE(buf0[1] == Approx(0.44444));
        REQUIRE(buf0[2] == Approx(0.88889));
        REQUIRE(buf0[3] == 1);
        REQUIRE(buf0[4] == 1);
        REQUIRE(buf0[5] == 1);
        REQUIRE(buf0[6] == Approx(0.88889));
        REQUIRE(buf0[7] == Approx(0.44444));
        REQUIRE(buf0[8] == 0);

        g->setWinType(GRAIN_WIN_LINUP);
        std::fill_n(buf0, BS, 0);
        std::fill_n(buf1, BS, 0);

        g->start(0);
        g->process(aptr->begin(), aptr->size(), buf, BS, SR);

        const auto k = 8 - 9 / 8.0;

        REQUIRE(buf0[0] == 0);
        REQUIRE(buf0[1] == Approx(1 / k));
        REQUIRE(buf0[2] == Approx(2 / k));
        REQUIRE(buf0[3] == Approx(3 / k));
        REQUIRE(buf0[4] == Approx(4 / k));
        REQUIRE(buf0[5] == Approx(5 / k));
        REQUIRE(buf0[6] == Approx(6 / k));
        REQUIRE(buf0[7] == Approx(8 / 9.0));
        REQUIRE(buf0[8] == 0);
    }

    SECTION("GrainCloud")
    {
        GrainCloud gc;

        auto g0 = gc.appendGrain(100, 10, 11);
        REQUIRE(g0->timeBefore() == 11);
        REQUIRE(g0->endInSamples() == 21);
        REQUIRE(gc.size() == 1);
        REQUIRE(gc.grains()[0]->timeBefore() == 11);

        auto g1 = gc.appendGrain(100, 10, 100);
        REQUIRE(gc.grains()[0]->timeBefore() == 11);
        REQUIRE(gc.grains()[1]->timeBefore() == 100);
        REQUIRE(gc.size() == 2);

        auto g2 = gc.appendGrain(100, 10, 60);
        REQUIRE(gc.grains()[0]->timeBefore() == 11);
        REQUIRE(gc.grains()[1]->timeBefore() == 100);
        REQUIRE(gc.grains()[2]->timeBefore() == 60);
        REQUIRE(gc.size() == 3);
    }

    SECTION("do single grain")
    {
        ArrayPtr aptr = cnv->createArray("array_g1", 128);
        aptr->fillWith([](size_t i) -> t_float { return i; });

        TExt t("array.grainer~", LA("array_g1"));
        REQUIRE_PROPERTY(t, @array, "array_g1");

        TestSignal<0, 2> s0;
        DspVector block;

        REQUIRE(t.cloud().empty());
        t.m_grain(&s_, LA("@at", 4, "@l", 10, "@speed", 1, "@pan", 0.0, "@ta", 1, "@tb", 3));
        REQUIRE(t.cloud().size() == 1);
        t.dump();

        DSP<TestSignal<0, 2>, TExt> dsp(s0, t);
        dsp.processBlock();

        block.assign(s0.out[0], s0.out[0] + 64);
        REQUIRE(block == make_seq({ 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0 }, { 0, 0, 0 }));

        dsp.processBlock();
        block.assign(s0.out[0], s0.out[0] + 64);
        REQUIRE(block == make_seq({ 10, 11, 12, 13, 0, 4, 5, 6, 7, 8, 9 }));
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

#define REQUIRE_SPEED(t, v)                               \
    {                                                     \
        REQUIRE(t.cloud().grains().size() > 0);           \
        REQUIRE(t.cloud().grains().back()->speed() == v); \
    }

#define GRAIN_DONE(t)                                    \
    {                                                    \
        REQUIRE(t.cloud().grains().size() > 0);          \
        REQUIRE(t.cloud().grains().back()->done() == 0); \
    }

#define CLEAR(t) t.m_clear(&s_, {})

        t.m_grain(&s_, AtomList::parseString("@speed 2.5"));
        REQUIRE_SPEED(t, 2.5);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed -4"));
        REQUIRE_SPEED(t, -4);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed -10"));
        REQUIRE_SPEED(t, -10);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed 10"));
        REQUIRE_SPEED(t, 10);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed 10.1"));
        REQUIRE_SPEED(t, 10);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed -10.1"));
        REQUIRE_SPEED(t, -10);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed =1.5"));
        REQUIRE_SPEED(t, 1.5);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed +=2.5"));
        REQUIRE_SPEED(t, 3.5);
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed $pi"));
        REQUIRE_SPEED(t, Approx(std::acos(-1)));
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed -2*$pi"));
        REQUIRE_SPEED(t, Approx(-2 * std::acos(-1)));
        t.m_clear(&s_, {});

        t.m_grain(&s_, AtomList::parseString("@speed rand(1\\,2)"));
        REQUIRE(t.cloud().grains().size() > 0);
        speed = t.cloud().grains().back()->speed();
        REQUIRE(speed >= 1);
        REQUIRE(speed <= 2);
        t.m_clear(&s_, {});

        MSG_GRAIN(t, "@speed 5 @speed ondone +=0.5");
        REQUIRE_SPEED(t, 5);
        GRAIN_DONE(t);
        REQUIRE_SPEED(t, 5.5);
        GRAIN_DONE(t);
        REQUIRE_SPEED(t, 6);
        CLEAR(t);

        MSG_GRAIN(t, "@speed 5 @speed ondone +=40");
        REQUIRE_SPEED(t, 5);
        GRAIN_DONE(t);
        REQUIRE_SPEED(t, 10);
        CLEAR(t);

        MSG_GRAIN(t, "@speed 5 @speed ondone -=0.5");
        REQUIRE_SPEED(t, 5);
        GRAIN_DONE(t);
        REQUIRE_SPEED(t, 4.5);
        CLEAR(t);

        MSG_GRAIN(t, "@speed 5 @speed ondone -2");
        REQUIRE_SPEED(t, 5);
        GRAIN_DONE(t);
        REQUIRE_SPEED(t, -2);
        GRAIN_DONE(t);
        REQUIRE_SPEED(t, -2);
        CLEAR(t);

        GrainRandom::seed(0);
        MSG_GRAIN(t, "@speed 5 @speed ondone rand(2\\,5)");
        REQUIRE_SPEED(t, 5);
        GRAIN_DONE(t);
        REQUIRE_SPEED(t, Approx(3.64644));
        GRAIN_DONE(t);
        REQUIRE_SPEED(t, Approx(3.77853));
        CLEAR(t);
    }

    SECTION("Grain @speed expr")
    {
        TExt t("array.grainer~", LA("array_g1"));
        MSG_GRAIN(t, "@speed 2^3");
        REQUIRE_SPEED(t, 8);

        MSG_GRAIN(t, "@speed $sr/10000");
        REQUIRE_SPEED(t, Approx(4.8));
    }

    SECTION("Grain @speed -1")
    {
        TExt t("array.grainer~", LA("array_g1"));
        MSG_GRAIN(t, "@at 2 @l 20 @speed -1 @pan mode none @tb 3 @ta 2");
        REQUIRE_SPEED(t, -1);

        TestSignal<0, 2> s0;
        DspVector block;

        DSP<TestSignal<0, 2>, TExt> dsp(s0, t);

        dsp.processBlock();
        block.assign(s0.out[0], s0.out[0] + 64);
        REQUIRE(block == make_seq({ 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 0, 0 }, { 0, 0, 0 }));

        dsp.processBlock();
        block.assign(s0.out[0], s0.out[0] + 64);
        REQUIRE(block == make_seq({ 4, 3, 2, 0, 0, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5 }, {}));
    }

    SECTION("Grain @amp")
    {
#define GET_AMP(t) t.cloud().grains().back()->amplitude()

        TExt t("array.grainer~", LA("array_g1"));
        MSG_GRAIN(t, "@amp 0.5");
        REQUIRE(GET_AMP(t) == 0.5);
    }

    SECTION("Grain @amp ondone set")
    {
        GrainRandom::seed(0);

        TExt t("array.grainer~", LA("array_g1"));
        MSG_GRAIN(t, "@amp ondone 0.5");
        REQUIRE(GET_AMP(t) == 1);
        t.cloud().grains().back()->done();
        REQUIRE(GET_AMP(t) == 0.5);
    }

    SECTION("Grain @amp ondone random")
    {
        GrainRandom::seed(0);

        TExt t("array.grainer~", LA("array_g1"));
        MSG_GRAIN(t, "@amp ondone rand(0.5\\,0.75)");
        REQUIRE(GET_AMP(t) == 1);
        t.cloud().grains().back()->done();
        REQUIRE(GET_AMP(t) == Approx(0.6372));
        t.cloud().grains().back()->done();
        REQUIRE(GET_AMP(t) == Approx(0.64821));
    }

    SECTION("Grain @amp ondone add expr")
    {
        TExt t("array.grainer~", LA("array_g1"));
        MSG_GRAIN(t, "@amp ondone +=-1/4");
        REQUIRE(GET_AMP(t) == 1);
        t.cloud().grains().back()->done();
        REQUIRE(GET_AMP(t) == Approx(0.75));
        t.cloud().grains().back()->done();
        REQUIRE(GET_AMP(t) == Approx(0.5));
    }

    SECTION("Grain @amp ondone expr")
    {
        TExt t("array.grainer~", LA("array_g1"));
        MSG_GRAIN(t, "@amp ondone =$n/4");
        REQUIRE(GET_AMP(t) == 1);
        t.cloud().grains().back()->done();
        REQUIRE(GET_AMP(t) == Approx(0.25));
        t.cloud().grains().back()->done();
        REQUIRE(GET_AMP(t) == Approx(0.5));
        t.cloud().grains().back()->done();
        REQUIRE(GET_AMP(t) == Approx(0.75));
    }

    SECTION("Grain @amp ondone")
    {
        TExt t("array.grainer~", LA("array_g1"));

        t.m_fill(&s_, AtomList::parseString("2 @at 3 @speed 2 @l $A @tb 20 @amp ondone *=0.5 @tag abc"));
        REQUIRE(t.cloud().grains().size() == 2);
        REQUIRE(t.cloud().grains().back()->arrayPosInSamples() == 3);
        REQUIRE(t.cloud().grains().back()->lengthInSamples() == 128);
        REQUIRE(t.cloud().grains().back()->durationInSamples() == 64);
        REQUIRE(t.cloud().grains().back()->timeBefore() == 20);
        REQUIRE(t.cloud().grains().back()->speed() == 2);
        REQUIRE(t.cloud().grains().back()->amplitude() == 1);

        t.cloud().grains().back()->done();
        REQUIRE(t.cloud().grains().back()->amplitude() == 0.5);
        t.cloud().grains().back()->done();
        REQUIRE(t.cloud().grains().back()->amplitude() == 0.25);
    }

    SECTION("Grain @time")
    {
#define GET_TIME(t) t.cloud().grains().back()->timeBefore()

        TExt t("array.grainer~", LA("array_g1"));

        MSG_GRAIN(t, "@at 1 @len 5 @tb 16");
        REQUIRE(t.cloud().size() == 1);
        REQUIRE(t.cloud().grains().back()->speed() == 1);
        REQUIRE(t.cloud().grains().back()->arrayPosInSamples() == 1);
        REQUIRE(t.cloud().grains().back()->lengthInSamples() == 5);
        REQUIRE(t.cloud().grains().back()->timeBefore() == 16);
        REQUIRE(GET_TIME(t) == 16);

        MSG_GRAIN(t, "@at 2 @amp $gi*0.75 @tb $A/2");
        REQUIRE(t.cloud().size() == 2);
        REQUIRE(GET_TIME(t) == 64);
        REQUIRE(GET_AMP(t) == 0.75);

        MSG_GRAIN(t, "@at 4 @len 16 @tb 1024 @speed ondone $gi+1");
        REQUIRE(t.cloud().size() == 3);
        t.dump();

        t.cloud().grains().back()->done();
        REQUIRE(t.cloud().grains().back()->speed() == 3);
    }

    SECTION("grain tags")
    {
        TExt t("array.grainer~", LA("array_g1"));

        MSG_GRAIN(t, "@len 5 @tag t1");
        MSG_GRAIN(t, "@len 6 @tag t1");
        MSG_GRAIN(t, "@len 7 @tag t1");

        MSG_GRAIN(t, "@len 5 @tag t2 @amp 0.1");
        MSG_GRAIN(t, "@len 6 @tag t2 @amp 0.2");
        MSG_GRAIN(t, "@len 7 @tag t2 @amp 0.3");

        REQUIRE(t.cloud().size() == 6);

#define MSG_SET(t, str) t.m_set(gensym("set"), AtomList::parseString(str));

        MSG_SET(t, "t2 @len 16");

        for (auto g : t.cloud().grains()) {
            if (g->tag() == SYM("t2")) {
                REQUIRE(g->lengthInSamples() == 16);
            } else {
                REQUIRE(g->lengthInSamples() != 16);
            }
        }

        MSG_SET(t, "* @len 25");
        for (auto g : t.cloud().grains()) {
            REQUIRE(g->lengthInSamples() == 25);
        }
    }

    SECTION("grain ID")
    {
        TExt t("array.grainer~", LA("array_g1"));

        MSG_GRAIN(t, "@len 5");
        MSG_GRAIN(t, "@len 6");
        MSG_GRAIN(t, "@len 7");

        MSG_GRAIN(t, "@len 5 @amp 0.1");
        MSG_GRAIN(t, "@len 6 @amp 0.2");
        MSG_GRAIN(t, "@len 7 @amp 0.3");

        REQUIRE(t.cloud().size() == 6);

#define MSG_SET(t, str) t.m_set(gensym("set"), AtomList::parseString(str));
#define MSG_CLEAR(t, str) t.m_clear(gensym("clear"), AtomList::parseString(str));

        MSG_SET(t, "-1 @len 16");
        MSG_SET(t, "0 @len 10");
        MSG_SET(t, "1 @len 20");
        MSG_SET(t, "2 @len 30");
        MSG_SET(t, "3 @len 40");
        MSG_SET(t, "4 @len 50");
        MSG_SET(t, "5 @len 60");
        MSG_SET(t, "6 @len 70");

#define GET_LEN_AT(t, i) t.cloud().grains().at(i)->lengthInSamples()

        REQUIRE(GET_LEN_AT(t, 0) == 10);
        REQUIRE(GET_LEN_AT(t, 1) == 20);
        REQUIRE(GET_LEN_AT(t, 2) == 30);
        REQUIRE(GET_LEN_AT(t, 3) == 40);
        REQUIRE(GET_LEN_AT(t, 4) == 50);
        REQUIRE(GET_LEN_AT(t, 5) == 60);

        t.dump();

        MSG_CLEAR(t, "2");
        REQUIRE(t.cloud().size() == 5);

        REQUIRE(GET_LEN_AT(t, 0) == 10);
        REQUIRE(GET_LEN_AT(t, 1) == 20);
        REQUIRE(GET_LEN_AT(t, 2) == 40);
        REQUIRE(GET_LEN_AT(t, 3) == 50);
        REQUIRE(GET_LEN_AT(t, 4) == 60);
    }

    SECTION("mod")
    {
        TExt t("array.grainer~", LA("array_g1"));
        MSG_GRAIN(t, "@amp mod 0..1 sin(100)");
        MSG_GRAIN(t, "@speed mod -1..1 saw(100)");
        MSG_GRAIN(t, "@speed mod -1..1 tri(100)");
        MSG_GRAIN(t, "@speed mod -1..1 sqr(100)");
        MSG_GRAIN(t, "@speed mod none");

        REQUIRE(t.cloud().size() == 5);
    }

    SECTION("slice")
    {
        TExt t("array.grainer~", LA("array_g1"));

        t.m_slice(&s_, AtomList::parseString("1 @at 4 @tag g0 @l 10samp @p 1 @amp 0.5"));
        REQUIRE(t.cloud().size() == 1);
        REQUIRE(t.cloud().grains().at(0)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(0)->timeAfter() == 0);
        REQUIRE(t.cloud().grains().at(0)->lengthInSamples() == 128);
        REQUIRE(t.cloud().grains().at(0)->tag() == SYM("g0"));
        REQUIRE(t.cloud().grains().at(0)->pan() == 1);
        REQUIRE(t.cloud().grains().at(0)->amplitude() == 0.5);

        t.m_clear(&s_, {});
        t.m_slice(&s_, AtomList::parseString("4 @at 4 @tag g0 @l 10samp @p 1 @amp 0.5"));
        REQUIRE(t.cloud().size() == 4);
        REQUIRE(t.cloud().grains().at(0)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(0)->timeAfter() == 96);
        REQUIRE(t.cloud().grains().at(0)->lengthInSamples() == 32);
        REQUIRE(t.cloud().grains().at(0)->tag() == SYM("g0"));
        REQUIRE(t.cloud().grains().at(0)->pan() == 1);
        REQUIRE(t.cloud().grains().at(0)->amplitude() == 0.5);
        REQUIRE(t.cloud().grains().at(1)->timeBefore() == 32);
        REQUIRE(t.cloud().grains().at(1)->timeAfter() == 96);
        REQUIRE(t.cloud().grains().at(1)->lengthInSamples() == 32);
        REQUIRE(t.cloud().grains().at(1)->tag() == SYM("g0"));
        REQUIRE(t.cloud().grains().at(1)->pan() == 1);
        REQUIRE(t.cloud().grains().at(1)->amplitude() == 0.5);
        REQUIRE(t.cloud().grains().at(2)->timeBefore() == 64);
        REQUIRE(t.cloud().grains().at(2)->timeAfter() == 96);
        REQUIRE(t.cloud().grains().at(2)->lengthInSamples() == 32);
        REQUIRE(t.cloud().grains().at(2)->tag() == SYM("g0"));
        REQUIRE(t.cloud().grains().at(2)->pan() == 1);
        REQUIRE(t.cloud().grains().at(2)->amplitude() == 0.5);
        REQUIRE(t.cloud().grains().at(3)->timeBefore() == 96);
        REQUIRE(t.cloud().grains().at(3)->timeAfter() == 96);
        REQUIRE(t.cloud().grains().at(3)->lengthInSamples() == 32);
        REQUIRE(t.cloud().grains().at(3)->tag() == SYM("g0"));
        REQUIRE(t.cloud().grains().at(3)->pan() == 1);
        REQUIRE(t.cloud().grains().at(3)->amplitude() == 0.5);

        t.m_clear(&s_, {});
        t.m_slice(&s_, AtomList::parseString("0 @at 4 @tag g0 @l 10samp @p 1 @amp 0.5"));
        REQUIRE(t.cloud().size() == 0);

        t.m_slice(&s_, AtomList::parseString("2 @s 2"));
        REQUIRE(t.cloud().size() == 2);
        REQUIRE(t.cloud().grains().at(0)->durationInSamples() == 32);
        REQUIRE(t.cloud().grains().at(0)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(0)->timeAfter() == 96);
        REQUIRE(t.cloud().grains().at(0)->speed() == 2);
        REQUIRE(t.cloud().grains().at(1)->durationInSamples() == 32);
        REQUIRE(t.cloud().grains().at(1)->timeBefore() == 64);
        REQUIRE(t.cloud().grains().at(1)->timeAfter() == 96);
        REQUIRE(t.cloud().grains().at(1)->speed() == 2);

        t.m_clear(&s_, {});
        t.m_slice(&s_, AtomList::parseString("2 @s 0.5"));
        REQUIRE(t.cloud().size() == 2);
        REQUIRE(t.cloud().grains().at(0)->durationInSamples() == 128);
        REQUIRE(t.cloud().grains().at(0)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(0)->timeAfter() == 0);
        REQUIRE(t.cloud().grains().at(0)->speed() == 0.5);
        REQUIRE(t.cloud().grains().at(1)->durationInSamples() == 128);
        REQUIRE(t.cloud().grains().at(1)->timeBefore() == 64);
        REQUIRE(t.cloud().grains().at(1)->timeAfter() == 0);
        REQUIRE(t.cloud().grains().at(1)->speed() == 0.5);

        t.m_clear(&s_, {});
        t.m_slice(&s_, AtomList::parseString("2 32samp"));
        REQUIRE(t.cloud().size() == 2);
        REQUIRE(t.cloud().grains().at(0)->durationInSamples() == 16);
        REQUIRE(t.cloud().grains().at(0)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(0)->timeAfter() == 16);
        REQUIRE(t.cloud().grains().at(1)->durationInSamples() == 16);
        REQUIRE(t.cloud().grains().at(1)->timeBefore() == 16);
        REQUIRE(t.cloud().grains().at(1)->timeAfter() == 16);
    }

    SECTION("spread")
    {
        TExt t("array.grainer~", LA("array_g1"));
        t.m_append(&s_, AtomList::parseString("1 @at 4 @tag g0 @l 10samp"));
        t.m_append(&s_, AtomList::parseString("1 @at 3 @tag g0 @l 10samp"));
        t.m_append(&s_, AtomList::parseString("1 @at 2 @tag g1 @l 10samp"));
        t.m_append(&s_, AtomList::parseString("1 @at 1 @tag g1 @l 10samp"));
        REQUIRE(t.cloud().size() == 4);

        REQUIRE(t.cloud().grains().at(0)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(1)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(2)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(3)->timeBefore() == 0);

        t.m_spread(&s_, LA("100samp", "g0"));
        REQUIRE(t.cloud().grains().at(0)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(0)->timeAfter() == 90);
        REQUIRE(t.cloud().grains().at(1)->timeBefore() == 50);
        REQUIRE(t.cloud().grains().at(1)->timeAfter() == 90);
        REQUIRE(t.cloud().grains().at(2)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(3)->timeBefore() == 0);

        t.m_spread(&s_, LA("100samp"));
        REQUIRE(t.cloud().grains().at(0)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(0)->timeAfter() == 90);
        REQUIRE(t.cloud().grains().at(1)->timeBefore() == 25);
        REQUIRE(t.cloud().grains().at(1)->timeAfter() == 90);
        REQUIRE(t.cloud().grains().at(2)->timeBefore() == 50);
        REQUIRE(t.cloud().grains().at(2)->timeAfter() == 90);
        REQUIRE(t.cloud().grains().at(3)->timeBefore() == 75);
        REQUIRE(t.cloud().grains().at(3)->timeAfter() == 90);

        t.m_spread(&s_, L());
        REQUIRE(t.cloud().grains().at(0)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(0)->timeAfter() == 118);
        REQUIRE(t.cloud().grains().at(1)->timeBefore() == 32);
        REQUIRE(t.cloud().grains().at(1)->timeAfter() == 118);
        REQUIRE(t.cloud().grains().at(2)->timeBefore() == 64);
        REQUIRE(t.cloud().grains().at(2)->timeAfter() == 118);
        REQUIRE(t.cloud().grains().at(3)->timeBefore() == 96);
        REQUIRE(t.cloud().grains().at(3)->timeAfter() == 118);

        t.m_spread(&s_, LA("*"));
        REQUIRE(t.cloud().grains().at(0)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(0)->timeAfter() == 118);
        REQUIRE(t.cloud().grains().at(1)->timeBefore() == 32);
        REQUIRE(t.cloud().grains().at(1)->timeAfter() == 118);
        REQUIRE(t.cloud().grains().at(2)->timeBefore() == 64);
        REQUIRE(t.cloud().grains().at(2)->timeAfter() == 118);
        REQUIRE(t.cloud().grains().at(3)->timeBefore() == 96);
        REQUIRE(t.cloud().grains().at(3)->timeAfter() == 118);

        t.m_spread(&s_, LA(&s_));
        REQUIRE(t.cloud().grains().at(0)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(0)->timeAfter() == 118);
        REQUIRE(t.cloud().grains().at(1)->timeBefore() == 32);
        REQUIRE(t.cloud().grains().at(1)->timeAfter() == 118);
        REQUIRE(t.cloud().grains().at(2)->timeBefore() == 64);
        REQUIRE(t.cloud().grains().at(2)->timeAfter() == 118);
        REQUIRE(t.cloud().grains().at(3)->timeBefore() == 96);
        REQUIRE(t.cloud().grains().at(3)->timeAfter() == 118);

        t.m_set(&s_, LA("g0", "@ta", 0., "@tb", 0.));
        t.m_spread(&s_, LA("g0"));
        REQUIRE(t.cloud().grains().at(0)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(0)->timeAfter() == 118);
        REQUIRE(t.cloud().grains().at(1)->timeBefore() == 64);
        REQUIRE(t.cloud().grains().at(1)->timeAfter() == 118);
        REQUIRE(t.cloud().grains().at(2)->timeBefore() == 64);
        REQUIRE(t.cloud().grains().at(2)->timeAfter() == 118);
        REQUIRE(t.cloud().grains().at(3)->timeBefore() == 96);
        REQUIRE(t.cloud().grains().at(3)->timeAfter() == 118);

        t.m_set(&s_, LA("g0", "@ta", 0., "@tb", 0.));
        t.m_spread(&s_, LA("g1"));
        REQUIRE(t.cloud().grains().at(0)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(0)->timeAfter() == 0);
        REQUIRE(t.cloud().grains().at(1)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(1)->timeAfter() == 0);
        REQUIRE(t.cloud().grains().at(2)->timeBefore() == 0);
        REQUIRE(t.cloud().grains().at(2)->timeAfter() == 118);
        REQUIRE(t.cloud().grains().at(3)->timeBefore() == 64);
        REQUIRE(t.cloud().grains().at(3)->timeAfter() == 118);
    }
}
