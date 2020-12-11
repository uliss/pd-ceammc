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
#include "pan_spread.h"
#include "test_spat_base.h"

PD_COMPLETE_SND_TEST_SETUP(PanSpread, pan, spread)

using namespace ceammc;

TEST_CASE("pan.spread~", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("pan.spread~", L(), true);
            REQUIRE_PROPERTY(t, @ch, 2);
            REQUIRE_PROPERTY(t, @spread, 1);
            REQUIRE_PROPERTY(t, @center, 0.f);
            REQUIRE_PROPERTY(t, @compensate, 0.f);

            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE(t.numInputChannels() == 2);
            REQUIRE(t.numOutputChannels() == 2);
        }

        SECTION("num")
        {
            TExt t("pan.spread~", LF(3), true);
            REQUIRE_PROPERTY(t, @ch, 3);

            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE(t.numInputChannels() == 3);
            REQUIRE(t.numOutputChannels() == 2);
        }

        SECTION("prop")
        {
            TExt t("pan.spread~", LA("@ch", 6), true);
            REQUIRE_PROPERTY(t, @ch, 6);

            REQUIRE(t.numInlets() == 6);
            REQUIRE(t.numInputChannels() == 6);
        }

        SECTION("spread")
        {
            TExt t("pan.spread~", LA("@spread", 0.25), true);
            REQUIRE_PROPERTY(t, @spread, 0.25);
        }

        SECTION("invalid")
        {
            SECTION("min")
            {
                TExt t("pan.spread~", LF(1), true);
                REQUIRE_PROPERTY(t, @ch, 2);
            }

            SECTION("max")
            {
                TExt t("pan.spread~", LF(32), true);
                REQUIRE_PROPERTY(t, @ch, 2);
            }

            SECTION("min prop")
            {
                TExt t("pan.spread~", LA("@ch", -6), true);
                REQUIRE_PROPERTY(t, @ch, 2);
            }

            SECTION("max prop")
            {
                TExt t("pan.spread~", LA("@ch", 32), true);
                REQUIRE_PROPERTY(t, @ch, 2);
            }

            SECTION("other")
            {
                TExt t("pan.spread~", LA("ABC"), true);
                REQUIRE_PROPERTY(t, @ch, 2);
            }

            SECTION("mixed")
            {
                TExt t("pan.spread~", LA(5, "@ch", 3), true);
                REQUIRE_PROPERTY(t, @ch, 3);
            }
        }
    }

    SECTION("process")
    {
        TExt t("pan.spread~", LF(3), true);
        REQUIRE(t.blockSize() == 64);
        REQUIRE(t.samplerate() == 48000);
        REQUIRE(t.numInputChannels() == 3);
        REQUIRE(t.numOutputChannels() == 2);

        TestSignal<3, 2> sig;
        sig.fillInput(0.f);
        sig.fillOutput(0);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == Approx(0));
            REQUIRE(sig.out[1][i] == Approx(0));
        }

        t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == Approx(0));
            REQUIRE(sig.out[1][i] == Approx(0));
        }

        sig.fillInputN(0, -1);
        for (size_t i = 0; i < 10000; i++)
            t.processBlock(sig.in, sig.out);

        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == Approx(-1));
            REQUIRE(sig.out[1][i] == Approx(0));
        }

        sig.fillInput(0.f);
        sig.fillInputN(1, 2);
        t.processBlock(sig.in, sig.out);
        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == Approx(sqrtf(2)));
            REQUIRE(sig.out[1][i] == Approx(sqrtf(2)));
        }

        sig.fillInput(0.f);
        sig.fillInputN(2, 10);
        t.processBlock(sig.in, sig.out);
        for (int i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == Approx(0));
            REQUIRE(sig.out[1][i] == Approx(10));
        }
    }
}
