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
#include "math_cdiv_tilde.h"
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

PD_COMPLETE_SND_TEST_SETUP(MathComplexDivTilde, math, cdiv_tilde)

struct CDivSignal : public TestSignal<4, 2> {
    typedef std::pair<std::array<t_sample, 64>, std::array<t_sample, 64>> ComplexBuf;

    void clear()
    {
        fillInput(0.f);
    }

    void setComplex0(float r, float i)
    {
        fillInputN(0, r);
        fillInputN(1, i);
    }

    void setComplex1(float r, float i)
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

typedef DSP<CDivSignal, TestExtMathComplexDivTilde> CDivDSP;

TEST_CASE("math.cdiv~", "[externals]")
{
    pd_test_init();

    SECTION("process")
    {
        TestExtMathComplexDivTilde t("math.cdiv~");
        REQUIRE(t.blockSize() == 64);
        REQUIRE(t.numInlets() == 4);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE(t.numOutputChannels() == 2);

        CDivSignal sig;
        CDivDSP dsp(sig, t);
        REQUIRE_OUTPUT_EQUAL(dsp, 0, 0);

        sig.setComplex0(144, 2);
        sig.setComplex1(3, 2);
        REQUIRE_OUTPUT_EQUAL(dsp, ((144 * 3 + 2 * 2) / 13.0), ((2 * 3 - 144 * 2) / 13.0));

        // division by zero
        sig.setComplex1(0, 0);
        REQUIRE_OUTPUT_EQUAL(dsp, 0, 0);
    }

    SECTION("alias")
    {
        TestPdExternal<MathComplexDivTilde> t("cdiv~");
        REQUIRE(t.object());
    }
}
