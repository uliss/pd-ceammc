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
#include "plot_geomspace_tilde.h"
#include "test_array_base.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(PlotGeomSpaceTilde, plot, geomspace_tilde)

using TSig = TestSignal<0, 1>;
using TDsp = PlotGeomSpaceTildeDSP<0, 1>;

TEST_CASE("plot.geomspace~", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            TExt t("plot.geomspace~");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);

            REQUIRE_PROPERTY(t, @start, 0.1);
            REQUIRE_PROPERTY(t, @stop, 100);
            REQUIRE_PROPERTY(t, @n, 100);
            REQUIRE_PROPERTY(t, @base, 10);
            REQUIRE_PROPERTY(t, @endpoint, 1);
        }

        SECTION("args")
        {
            TExt t("plot.geomspace~", LA(1, 1000, 5, 2));

            REQUIRE_PROPERTY(t, @start, 1);
            REQUIRE_PROPERTY(t, @stop, 1000);
            REQUIRE_PROPERTY(t, @n, 5);
            REQUIRE_PROPERTY(t, @base, 2);
            REQUIRE_PROPERTY(t, @endpoint, 1);
        }

        SECTION("args")
        {
            TExt t("plot.geomspace~", LA(2, 6, 5, "e"));

            REQUIRE_PROPERTY(t, @start, 2);
            REQUIRE_PROPERTY(t, @stop, 6);
            REQUIRE_PROPERTY(t, @n, 5);
            REQUIRE_PROPERTY(t, @base, "e");
            REQUIRE_PROPERTY(t, @endpoint, 1);
        }
    }

    SECTION("ctrl")
    {
        TExt t("plot.geomspace~");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(listAt(t, 1_out) == LX(100, 0.1, 100, 10));
    }

    SECTION("dsp")
    {
        TExt t("plot.geomspace~", LF(0.1, 1000, 5));
        TSig s0;
        TDsp dsp(s0, t);
        dsp.processBlock();

        SECTION("log10")
        {
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE(listAt(t, 1_out) == LF(5, 0.1, 1000, 10));

            dsp.processBlock(1);

            REQUIRE(s0.out[0][0] == Approx(0.1));
            REQUIRE(s0.out[0][1] == Approx(1));
            REQUIRE(s0.out[0][2] == Approx(10));
            REQUIRE(s0.out[0][3] == Approx(100));
            REQUIRE(s0.out[0][4] == Approx(1000));

            for (size_t i = 5; i < t.blockSize(); i++) {
                REQUIRE(s0.out[0][i] == 1000);
            }

            WHEN_SEND_FLOAT_TO(0, t, 10);
            REQUIRE_PROPERTY(t, @n, 10);
            REQUIRE(listAt(t, 1_out) == LF(10, 0.1, 1000, 10));
        }

        SECTION("log2")
        {
            t.setProperty("@base", { 2 });

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE(listAt(t, 1_out) == LF(5, 0.1, 1000, 2));

            dsp.processBlock(1);

            REQUIRE(s0.out[0][0] == Approx(0.1));
            REQUIRE(s0.out[0][1] == Approx(1));
            REQUIRE(s0.out[0][2] == Approx(10));
            REQUIRE(s0.out[0][3] == Approx(100));
            REQUIRE(s0.out[0][4] == Approx(1000));

            for (size_t i = 5; i < t.blockSize(); i++) {
                REQUIRE(s0.out[0][i] == 1000);
            }
        }

        SECTION("ln")
        {
            t.setProperty("@base", LA("e"));

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE(listAt(t, 1_out) == LAX(5, 0.1, 1000, "e"));

            dsp.processBlock(1);

            REQUIRE(s0.out[0][0] == Approx(0.1));
            REQUIRE(s0.out[0][1] == Approx(1));
            REQUIRE(s0.out[0][2] == Approx(10));
            REQUIRE(s0.out[0][3] == Approx(100));
            REQUIRE(s0.out[0][4] == Approx(1000));

            for (size_t i = 5; i < t.blockSize(); i++) {
                REQUIRE(s0.out[0][i] == 1000);
            }
        }
    }
}
