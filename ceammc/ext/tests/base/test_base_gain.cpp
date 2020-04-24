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
#include "gain.h"
#include "test_base.h"
#include "catch.hpp"
#include "ceammc_pd.h"
#include "test_sound.h"

#include <algorithm>
#include <cstdlib>
#include <stdio.h>

typedef TestSoundExternal<Gain> GainTest;

TEST_CASE("gain~", "[externals]")
{
    SECTION("construct")
    {
        SECTION("default")
        {
            GainTest t("gain~", L(), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numInputChannels() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.numOutputChannels() == 1);
            REQUIRE_PROPERTY_LIST(t, @db, LX(-144));
            REQUIRE_PROPERTY_LIST(t, @value, LX(0));
        }

        SECTION("args")
        {
            GainTest t("gain~", LF(4), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 5);
            REQUIRE(t.numInputChannels() == 4);
            REQUIRE(t.numOutlets() == 4);
            REQUIRE(t.numOutputChannels() == 4);
            REQUIRE_PROPERTY_LIST(t, @db, LX(-144, -144, -144, -144));
            REQUIRE_PROPERTY_LIST(t, @value, LX(0, 0, 0, 0));
        }

        SECTION("props")
        {
            GainTest t("gain~", LA(3, "@db", -6, -6), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 4);
            REQUIRE(t.numInputChannels() == 3);
            REQUIRE(t.numOutlets() == 3);
            REQUIRE(t.numOutputChannels() == 3);
            REQUIRE_PROPERTY_LIST(t, @db, LX(-6, -6, -144));
            REQUIRE_PROPERTY_LIST(t, @value, LX(0.501187, 0.501187, 0));
        }
    }

    SECTION("process all")
    {
        GainTest t("gain~", LF(2), true);
        TestSignal<2, 2> s0;
        s0.fillInput(10);

        DSP<TestSignal<2, 2>, GainTest> dsp(s0, t);
        dsp.processBlock();

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
            REQUIRE(dsp.out(1, i) == Approx(0));
        }

        t.onInlet(2, LF(0.5));
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.5, 0.5));

        dsp.processBlock();
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) > 0);
            REQUIRE(dsp.out(1, i) > 0);
            REQUIRE(dsp.out(0, i) == dsp.out(1, i));
        }

        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(5));
            REQUIRE(dsp.out(1, i) == Approx(5));
            REQUIRE(dsp.out(0, i) == dsp.out(1, i));
        }
    }

    SECTION("process all2")
    {
        GainTest t("gain~", LF(2), true);
        TestSignal<2, 2> s0;
        s0.fillInputN(0, 10);
        s0.fillInputN(1, 6);

        DSP<TestSignal<2, 2>, GainTest> dsp(s0, t);
        dsp.processBlock();

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
            REQUIRE(dsp.out(1, i) == Approx(0));
        }

        t.onInlet(2, LF(0.5));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(5));
            REQUIRE(dsp.out(1, i) == Approx(3));
        }
    }

    SECTION("process separate")
    {
        GainTest t("gain~", LF(2), true);
        TestSignal<2, 2> s0;
        s0.fillInputN(0, 10);
        s0.fillInputN(1, 6);

        DSP<TestSignal<2, 2>, GainTest> dsp(s0, t);
        dsp.processBlock();

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
            REQUIRE(dsp.out(1, i) == Approx(0));
        }

        t.onList(LA(0.5, 2));
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(5));
            REQUIRE(dsp.out(1, i) == Approx(12));
        }
    }

    SECTION("props")
    {
        GainTest t("gain~", LF(2), true);
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
        GainTest t("gain~", LA(2, "@value", 1, 1), true);
        REQUIRE_PROPERTY_LIST(t, @value, LX(1, 1));

        WHEN_CALL(t, plus);
        REQUIRE_PROPERTY_LIST(t, @value, LX(1, 1));

        WHEN_CALL(t, minus);
        REQUIRE_PROPERTY_LIST(t, @value, LX(1, 1));

        WHEN_CALL_N(t, plus, 0.1);
        REQUIRE_PROPERTY_LIST(t, @value, LX(1.1, 1));

        WHEN_CALL_N(t, plus, 0.1, 0.5);
        REQUIRE_PROPERTY_LIST(t, @value, LX(1.2, 1.5));

        WHEN_CALL_N(t, plus, -0.1, -0.5);
        REQUIRE_PROPERTY_LIST(t, @value, LX(1.1, 1));

        WHEN_CALL_N(t, minus, -0.1, -0.5);
        REQUIRE_PROPERTY_LIST(t, @value, LX(1.2, 1.5));

        WHEN_CALL_N(t, minus, 3, 2);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0, 0));

        WHEN_CALL_N(t, plus, -1, -2);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0, 0));

        WHEN_CALL_N(t, minus, -1);
        REQUIRE_PROPERTY_LIST(t, @value, LX(1, 0));

        t.setProperty("@db", LF(0.f, 0.f));
        WHEN_CALL_N(t, plusDb, 6.0206, -6.0206);
        REQUIRE_PROPERTY_LIST(t, @value, LX(2, 0.5));

        WHEN_CALL_N(t, plusDb, 6.0206);
        REQUIRE_PROPERTY_LIST(t, @value, LX(4, 0.5));

        WHEN_CALL_N(t, minusDb, 6.0206);
        REQUIRE_PROPERTY_LIST(t, @value, LX(2, 0.5));

        WHEN_CALL_N(t, plusDb, -6.0206, 6.0206);
        REQUIRE_PROPERTY_LIST(t, @value, LX(1, 1));

        WHEN_CALL_N(t, minusDb, 6.0206, -6.0206);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.5, 2));

        WHEN_CALL_N(t, plusDb, -144, -144);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0, 0));

        WHEN_CALL_N(t, plusAll, 0.5);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.5, 0.5));

        WHEN_CALL_N(t, plusAll, -0.1);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.4, 0.4));

        WHEN_CALL_N(t, minusAll, 0.1);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.3, 0.3));

        WHEN_CALL_N(t, minusAll, -0.3);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.6, 0.6));

        WHEN_CALL(t, minusAll);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.6, 0.6));
        WHEN_CALL(t, plusAll);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.6, 0.6));
        WHEN_CALL(t, plus);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.6, 0.6));
        WHEN_CALL(t, minus);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.6, 0.6));
        WHEN_CALL(t, plusDb);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.6, 0.6));
        WHEN_CALL(t, minusDb);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.6, 0.6));

        WHEN_CALL_N(t, set, 0.5);
        REQUIRE_PROPERTY_LIST(t, @value, LX(0.5, 0.5));

        WHEN_CALL_N(t, setDb, 6.0206);
        REQUIRE_PROPERTY_LIST(t, @value, LX(2, 2));
    }
}
