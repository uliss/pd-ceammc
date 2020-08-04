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
#include "plot_linspace_tilde.h"
#include "test_array_base.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(PlotLinTilde, plot, linspace_tilde)

using TSig = TestSignal<0, 1>;
using TDsp = PlotLinTildeDSP<0, 1>;

TEST_CASE("plot.linspace~", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            TExt t("plot.linspace~");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);

            REQUIRE_PROPERTY(t, @start, 0);
            REQUIRE_PROPERTY(t, @stop, 255);
            REQUIRE_PROPERTY(t, @n, 256);
            REQUIRE_PROPERTY(t, @endpoint, 1);
        }

        SECTION("args")
        {
            TExt t("plot.linspace~", LA(-2, -4, 5));

            REQUIRE_PROPERTY(t, @start, -2);
            REQUIRE_PROPERTY(t, @stop, -4);
            REQUIRE_PROPERTY(t, @n, 5);
            REQUIRE_PROPERTY(t, @endpoint, 1);
        }
    }

    SECTION("ctrl")
    {
        TExt t("plot.linspace~");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(listAt(t, 1_out) == LF(256, 0, 255, 0));
    }

    SECTION("dsp")
    {
        TExt t("plot.linspace~", LF(0, 16, 17));

        TSig s0;
        TDsp dsp(s0, t);
        dsp.processBlock();

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(listAt(t, 1_out) == LF(17, 0, 16, 0));

        dsp.processBlock(1);
        for (size_t i = 0; i < 16; i++) {
            REQUIRE(s0.out[0][i] == Approx(i));
        }

        for (size_t i = 16; i < t.blockSize(); i++) {
            REQUIRE(s0.out[0][i] == 16);
        }

        WHEN_SEND_FLOAT_TO(0, t, 9);
        REQUIRE_PROPERTY(t, @n, 9);
        REQUIRE(listAt(t, 1_out) == LF(9, 0, 16, 0));

        dsp.processBlock(1);
        for (size_t i = 0; i < 9; i++) {
            REQUIRE(s0.out[0][i] == Approx(2 * i));
        }

        for (size_t i = 9; i < t.blockSize(); i++) {
            REQUIRE(s0.out[0][i] == 16);
        }

        WHEN_SEND_FLOAT_TO(0, t, 33);
        REQUIRE_PROPERTY(t, @n, 33);
        REQUIRE(listAt(t, 1_out) == LF(33, 0, 16, 0));

        dsp.processBlock(1);
        for (size_t i = 0; i < 33; i++) {
            REQUIRE(s0.out[0][i] == Approx(0.5 * i));
        }

        for (size_t i = 33; i < t.blockSize(); i++) {
            REQUIRE(s0.out[0][i] == 16);
        }

        t.setProperty("@endpoint", LF(0));
        WHEN_SEND_FLOAT_TO(0, t, 8);
        REQUIRE_PROPERTY(t, @n, 8);
        REQUIRE(listAt(t, 1_out) == LF(8, 0, 16, 0));

        dsp.processBlock(1);
        for (size_t i = 0; i < 8; i++) {
            REQUIRE(s0.out[0][i] == Approx(2 * i));
        }

        for (size_t i = 8; i < t.blockSize(); i++) {
            REQUIRE(s0.out[0][i] == 14);
        }

        WHEN_SEND_FLOAT_TO(0, t, 32);
        REQUIRE_PROPERTY(t, @n, 32);
        REQUIRE(listAt(t, 1_out) == LF(32, 0, 16, 0));

        dsp.processBlock(1);
        for (size_t i = 0; i < 32; i++) {
            REQUIRE(s0.out[0][i] == Approx(0.5 * i));
        }

        for (size_t i = 32; i < t.blockSize(); i++) {
            REQUIRE(s0.out[0][i] == Approx(15.5));
        }
    }

    SECTION("dsp neg")
    {
        TExt t("plot.linspace~", LF(16, 8, 9));
        REQUIRE_PROPERTY(t, @start, 16);
        REQUIRE_PROPERTY(t, @stop, 8);
        REQUIRE_PROPERTY(t, @n, 9);
        REQUIRE_PROPERTY(t, @endpoint, 1);

        TSig s0;
        TDsp dsp(s0, t);
        dsp.processBlock();

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(listAt(t, 1_out) == LF(9, 16, 8, 0));

        dsp.processBlock(1);
        for (size_t i = 0; i < 9; i++) {
            REQUIRE(s0.out[0][i] == Approx(16 - i));
        }

        for (size_t i = 9; i < t.blockSize(); i++) {
            REQUIRE(s0.out[0][i] == 8);
        }
    }

    SECTION("dsp const")
    {
        TExt t("plot.linspace~", LF(8, 8, 9));
        REQUIRE_PROPERTY(t, @start, 8);
        REQUIRE_PROPERTY(t, @stop, 8);
        REQUIRE_PROPERTY(t, @n, 9);
        REQUIRE_PROPERTY(t, @endpoint, 1);

        TSig s0;
        TDsp dsp(s0, t);
        dsp.processBlock();

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(listAt(t, 1_out) == LF(9, 8, 8, 0));

        dsp.processBlock(1);
        for (size_t i = 0; i < t.blockSize(); i++) {
            REQUIRE(s0.out[0][i] == 8);
        }
    }
}
