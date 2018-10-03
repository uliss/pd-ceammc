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
#include "../math/math_cmul_tilde.h"
#include "test_base.h"
#include "test_external.h"
#include "test_sound.h"

#include <algorithm>
#include <cstdlib>
#include <stdio.h>

#define REQUIRE_OUTPUT_EQUAL(dsp, val, ival)        \
    {                                               \
        dsp.processBlock(1);                        \
        for (size_t i = 0; i < 64; i++)             \
            REQUIRE(dsp.out(0, i) == Approx(val));  \
        for (size_t i = 0; i < 64; i++)             \
            REQUIRE(dsp.out(1, i) == Approx(ival)); \
    }

PD_COMPLETE_SND_TEST_SETUP(MathComplexMulTilde, math, cmul_tilde)

typedef TestSignal<4, 2> CMulSignal;
typedef DSP<CMulSignal, TestExtMathComplexMulTilde> CMulDSP;

TEST_CASE("math.cmul~", "[externals]")
{
    pd_test_init();

    SECTION("process")
    {
        TestExtMathComplexMulTilde t("math.cmul~");
        REQUIRE(t.blockSize() == 64);
        REQUIRE(t.numInlets() == 4);
        REQUIRE(t.numOutlets() == 2);
        //        REQUIRE(t.numInputChannels() == 4);
        REQUIRE(t.numOutputChannels() == 2);

        CMulSignal sig;
        CMulDSP dsp(sig, t);
        REQUIRE_OUTPUT_EQUAL(dsp, 0, 0);

        sig.fillInputN(0, 1);
        sig.fillInputN(2, 1);
        REQUIRE_OUTPUT_EQUAL(dsp, 1, 0);

        sig.fillInput(0.f);
        sig.fillInputN(1, 1);
        sig.fillInputN(3, 1);
        REQUIRE_OUTPUT_EQUAL(dsp, -1, 0);
    }

    SECTION("alias")
    {
        TestPdExternal<MathComplexMulTilde> t("cmul~");
        REQUIRE(t.object());
    }
}
