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
#include "array_circular_tilde.h"
#include "test_array_base.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(ArrayCircularTilde, array, circular_tilde)

using TSig = TestSignal<2, 1>;
using TDsp = ArrayCircularTildeDSP<2, 1>;

TEST_CASE("array.circular~", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("empty")
    {
        TExt t("array.circular~");
        REQUIRE(t.numInlets() == 3);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @array, &s_);
        REQUIRE_PROPERTY(t, @on, 1);
        REQUIRE_PROPERTY(t, @interp, 0.);
    }

    SECTION("invalid")
    {
        TExt t("array.circular~", LA("non-exists"));
        REQUIRE(t.numInlets() == 3);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @array, "non-exists");
    }

    SECTION("do")
    {
        ArrayPtr aptr = cnv->createArray("array_circ0", 32);
        aptr->fillWith(0.);

        TExt t("array.circular~", LA("array_circ0"));

        constexpr size_t BS = 64;
        TSig s0;
        TDsp dsp(s0, t);

        s0.fillOutput(-1);
        dsp.processBlock();

        REQUIRE(s0.blocksize() == BS);

        for (size_t i = 0; i < BS; i++) {
            REQUIRE(s0.out[0][i] == 0);
        }

        s0.fillInputN(0, [](size_t i) { return i; });
        s0.fillInputN(1, 0);
        dsp.processBlock();

        for (size_t i = 0; i < BS; i++) {
            REQUIRE(s0.out[0][i] == i);
        }

        s0.fillInputN(1, 1);
        dsp.processBlock();

        REQUIRE(s0.out[0][0] == 63);
        REQUIRE(s0.out[0][1] == 0);
        REQUIRE(s0.out[0][63] == 62);

        s0.fillInputN(1, 2);
        dsp.processBlock();

        REQUIRE(s0.out[0][0] == 62);
        REQUIRE(s0.out[0][1] == 63);
        REQUIRE(s0.out[0][2] == 0);
        REQUIRE(s0.out[0][3] == 1);
        REQUIRE(s0.out[0][63] == 61);

        s0.fillInputN(1, 31);
        dsp.processBlock();

        REQUIRE(s0.out[0][0] == 33);
        REQUIRE(s0.out[0][1] == 34);
        REQUIRE(s0.out[0][2] == 35);
        REQUIRE(s0.out[0][63] == 32);

        s0.fillInputN(1, 32);
        dsp.processBlock();

        REQUIRE(s0.out[0][0] == 0);
        REQUIRE(s0.out[0][1] == 1);
        REQUIRE(s0.out[0][2] == 2);
        REQUIRE(s0.out[0][63] == 63);

        t.setProperty("@on", LF(0));
        dsp.processBlock();

        for (size_t i = 0; i < BS; i++) {
            REQUIRE(s0.out[0][i] == 0);
        }

        t.setProperty("@on", LF(1));
        t.setProperty("@array", LA("non-exists"));
        s0.fillOutput(-1);
        dsp.processBlock();

        for (size_t i = 0; i < BS; i++) {
            REQUIRE(s0.out[0][i] == 0);
        }
    }
}
