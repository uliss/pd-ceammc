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
#include "array_plot_tilde.h"
#include "test_array_base.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(ArrayPlotTilde, array, plot_tilde)

using TSig = TestSignal<1, 0>;
using TDsp = ArrayPlotTildeDSP<1, 0>;

TEST_CASE("array.plot~", "[externals]")
{
    pd_test_init();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("init")
    {
        SECTION("empty")
        {
            TExt t("array.plot~");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @array, "");
            REQUIRE_PROPERTY(t, @ymin, -1);
            REQUIRE_PROPERTY(t, @ymax, 1);
            REQUIRE_PROPERTY(t, @yauto, 0);
        }

        SECTION("args")
        {

            TExt t("array.plot~", LA("array1", -2, 3.5));

            REQUIRE_PROPERTY(t, @array, "array1");
            REQUIRE_PROPERTY(t, @ymin, -2);
            REQUIRE_PROPERTY(t, @ymax, 3.5);
            REQUIRE_PROPERTY(t, @yauto, 0);
        }
    }

    SECTION("dsp")
    {
        TExt t("array.plot~", LA("array_plot~0"));
        TSig s0;
        TDsp dsp(s0, t);
        dsp.processBlock();

        SECTION("bang")
        {
            ArrayPtr aptr = cnv->createArray("array_plot~0", 30);
            s0.fillInputN(0, [](size_t i) { return (2 * i - 50.0) / 50.0; });

            t.sendBang(1);
            dsp.processBlock(2);

            REQUIRE(aptr->size() == 30);

            for (size_t i = 0; i < 30; i++) {
                REQUIRE(aptr->at(i) == Approx((2 * i - 50.0) / 50.0));
            }
        }

        SECTION("bang yauto")
        {
            ArrayPtr aptr = cnv->createArray("array_plot~1", 30);

            t.setProperty("@yauto", { 1 });
            t.setProperty("@array", LA("array_plot~1"));

            s0.fillInputN(0, [](size_t i) { return 2 * i; });

            t.sendBang(1);
            dsp.processBlock(2);

            REQUIRE(aptr->size() == 30);

            for (size_t i = 0; i < 30; i++) {
                REQUIRE(aptr->at(i) == 2 * i);
            }
        }

        SECTION("bang clip")
        {
            ArrayPtr aptr = cnv->createArray("array_plot~2", 20);
            t.setProperty("@array", LA("array_plot~2"));

            s0.fillInputN(0, [](size_t i) { return 0.5 * i; });

            t.sendBang(1);
            dsp.processBlock(2);

            REQUIRE(aptr->size() == 20);

            REQUIRE(aptr->at(0) == 0);
            REQUIRE(aptr->at(1) == Approx(0.5));
            REQUIRE(aptr->at(2) == Approx(1));

            for (size_t i = 3; i < 20; i++) {
                REQUIRE(aptr->at(i) == 1);
            }
        }

        SECTION("float")
        {
            ArrayPtr aptr = cnv->createArray("array_plot~3", 20);
            t.setProperty("@yauto", { 1 });
            t.setProperty("@array", LA("array_plot~3"));

            s0.fillInputN(0, [](size_t i) { return 2 * i; });

            t.sendFloat(10, 1);
            dsp.processBlock(2);
            aptr->update();
            REQUIRE(aptr->size() == 10);

            for (size_t i = 0; i < 10; i++) {
                REQUIRE(aptr->at(i) == i * 2);
            }
        }
    }
}
