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
#include "math_cmul_tilde.h"
#include "test_math_base.h"
#include "test_external.h"
#include "test_sound.h"

#include <algorithm>
#include <cstdlib>

#define REQUIRE_OUTPUT_EQUAL(dsp, val, ival)        \
    {                                               \
        dsp.processBlock(1);                        \
        for (size_t i = 0; i < 64; i++)             \
            REQUIRE(dsp.out(0, i) == Approx(val));  \
        for (size_t i = 0; i < 64; i++)             \
            REQUIRE(dsp.out(1, i) == Approx(ival)); \
    }

PD_COMPLETE_SND_TEST_SETUP(MathComplexMulTilde, math, cmul_tilde)

struct CMulSignal : public TestSignal<4, 2> {
    typedef std::pair<std::array<t_sample, 64>, std::array<t_sample, 64>> ComplexBuf;

    void clear()
    {
        fillInput(0);
    }

    void setComplex0(t_float r, t_float i)
    {
        fillInputN(0, r);
        fillInputN(1, i);
    }

    void setComplex1(t_float r, t_float i)
    {
        fillInputN(2, r);
        fillInputN(3, i);
    }

    ComplexBuf result() const
    {
        ComplexBuf buf;
        std::copy(std::begin(buf_out[0]), std::end(buf_out[0]), buf.first.begin());
        std::copy(std::begin(buf_out[1]), std::end(buf_out[1]), buf.second.begin());
        return buf;
    }
};

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

        sig.setComplex0(1, 0);
        sig.setComplex1(1, 0);
        REQUIRE_OUTPUT_EQUAL(dsp, 1, 0);

        sig.setComplex0(0, 1);
        sig.setComplex1(0, 1);
        REQUIRE_OUTPUT_EQUAL(dsp, -1, 0);
    }

    SECTION("commut")
    {
        TestExtMathComplexMulTilde t("math.cmul~");

        CMulSignal sig;
        CMulDSP dsp(sig, t);

        sig.setComplex0(123, -2);
        sig.setComplex1(-3, 1);
        dsp.processBlock();

        auto res0 = sig.result();

        sig.setComplex1(123, -2);
        sig.setComplex0(-3, 1);
        dsp.processBlock();

        auto res1 = sig.result();
        REQUIRE(res0 == res1);

        REQUIRE_OUTPUT_EQUAL(dsp, (123 * -3 - (-2 * 1)), (-2 * -3 + 123 * 1));
    }

    SECTION("alias")
    {
        TestPdExternal<MathComplexMulTilde> t("cmul~");
        REQUIRE(t.object());
    }
}
