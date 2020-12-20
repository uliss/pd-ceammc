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
#include "plot_logspace_tilde.h"
#include "test_array_base.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(PlotLogTilde, plot, logspace_tilde)

using TSig = TestSignal<0, 1>;
using TDsp = PlotLogTildeDSP<0, 1>;

TEST_CASE("plot.logspace~", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            TExt t("plot.logspace~");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);

            REQUIRE_PROPERTY(t, @start, 0);
            REQUIRE_PROPERTY(t, @stop, 2);
            REQUIRE_PROPERTY(t, @n, 100);
            REQUIRE_PROPERTY(t, @base, 10);
            REQUIRE_PROPERTY(t, @endpoint, 1);
        }

        SECTION("args")
        {
            TExt t("plot.logspace~", LA(-2, -4, 5, 2));

            REQUIRE_PROPERTY(t, @start, -2);
            REQUIRE_PROPERTY(t, @stop, -4);
            REQUIRE_PROPERTY(t, @n, 5);
            REQUIRE_PROPERTY(t, @base, 2);
            REQUIRE_PROPERTY(t, @endpoint, 1);
        }

        SECTION("args")
        {
            TExt t("plot.logspace~", LA(-2, -4, 5, "e"));

            REQUIRE_PROPERTY(t, @start, -2);
            REQUIRE_PROPERTY(t, @stop, -4);
            REQUIRE_PROPERTY(t, @n, 5);
            REQUIRE_PROPERTY(t, @base, "e");
        }
    }

    SECTION("ctrl")
    {
        TExt t("plot.logspace~");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(listAt(t, 1_out) == LA(100, 1, 100, 10));
    }

    SECTION("log10")
    {
        TExt t("plot.linspace~", LF(0, 3, 4));
        TSig s0;
        TDsp dsp(s0, t);
        dsp.processBlock();

        SECTION("bang")
        {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE(listAt(t, 1_out) == LF(4, 1, 1000, 10));

            dsp.processBlock(1);
            for (size_t i = 0; i < 4; i++) {
                REQUIRE(s0.out[0][i] == Approx(std::pow(10, i)));
            }

            for (size_t i = 4; i < t.blockSize(); i++) {
                REQUIRE(s0.out[0][i] == 1000);
            }
        }

        SECTION("31")
        {
            WHEN_SEND_FLOAT_TO(0, t, 31);
            REQUIRE_PROPERTY(t, @n, 31);
            REQUIRE(listAt(t, 1_out) == LF(31, 1, 1000, 10));

            dsp.processBlock(1);
            for (size_t i = 0; i < 31; i++) {
                REQUIRE(s0.out[0][i] == Approx(std::pow(10, i / 10.f)));
            }

            for (size_t i = 31; i < t.blockSize(); i++) {
                REQUIRE(s0.out[0][i] == 1000);
            }
        }
    }

    SECTION("log2")
    {
        TExt t("plot.linspace~", LA(-2, 2, 5, "@2"));
        TSig s0;
        TDsp dsp(s0, t);
        dsp.processBlock();

        SECTION("bang")
        {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE(listAt(t, 1_out) == LF(5, 0.25, 4, 2));

            dsp.processBlock(1);
            REQUIRE(s0.out[0][0] == Approx(0.25));
            REQUIRE(s0.out[0][1] == Approx(0.5));
            REQUIRE(s0.out[0][2] == Approx(1));
            REQUIRE(s0.out[0][3] == Approx(2));
            REQUIRE(s0.out[0][4] == Approx(4));

            for (size_t i = 5; i < t.blockSize(); i++) {
                REQUIRE(s0.out[0][i] == 4);
            }
        }
    }

    SECTION("ln")
    {
        TExt t("plot.linspace~", LA(1, 2, 5, "e"));
        TSig s0;
        TDsp dsp(s0, t);
        dsp.processBlock();

        SECTION("bang")
        {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE(listAt(t, 1_out) == LAX(5, std::exp(1), std::exp(2), "e"));

            dsp.processBlock(1);
            REQUIRE(s0.out[0][0] == Approx(std::exp(1)));
            REQUIRE(s0.out[0][1] == Approx(std::exp(1.25)));
            REQUIRE(s0.out[0][2] == Approx(std::exp(1.5)));
            REQUIRE(s0.out[0][3] == Approx(std::exp(1.75)));
            REQUIRE(s0.out[0][4] == Approx(std::exp(2)));

            for (size_t i = 5; i < t.blockSize(); i++) {
                REQUIRE(s0.out[0][i] == Approx(std::exp(2)));
            }
        }
    }

    SECTION("endpoint log2")
    {
        TExt t("plot.linspace~", LA(0., 4, 4, "@2", "@endpoint", "false"));
        TSig s0;
        TDsp dsp(s0, t);
        dsp.processBlock();
        REQUIRE_PROPERTY(t, @endpoint, 0);

        SECTION("bang")
        {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE(listAt(t, 1_out) == LF(4, 1, 16, 2));

            dsp.processBlock(1);
            REQUIRE(s0.out[0][0] == Approx(1));
            REQUIRE(s0.out[0][1] == Approx(2));
            REQUIRE(s0.out[0][2] == Approx(4));
            REQUIRE(s0.out[0][3] == Approx(8));
            REQUIRE(s0.out[0][4] == Approx(8));

            for (size_t i = 5; i < t.blockSize(); i++) {
                REQUIRE(s0.out[0][i] == 8);
            }
        }
    }
}
