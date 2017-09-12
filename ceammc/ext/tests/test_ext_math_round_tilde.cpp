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
#include "../math/math_round_tilde.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <algorithm>
#include <cstdlib>
#include <stdio.h>

typedef TestSoundExtension<MathRoundTilde> MathRoundTildeTest;

struct ConstGen {
    double v;
    ConstGen(double val)
        : v(val)
    {
    }
    t_sample operator()(size_t) const { return t_sample(v); }
};

struct FRandGen {
    int f, t;
    FRandGen(int from, int to)
        : f(from)
        , t(to)
    {
    }
    t_sample operator()(size_t) const { return t_sample(rand() % (t - f) + f); }
};

template <size_t IN, size_t OUT>
struct SigVec {
    t_sample buf_in[IN][64];
    t_sample buf_out[OUT][64];
    const t_sample* in_ptr;
    t_sample* out_ptr;
    const t_sample** in;
    t_sample** out;

    SigVec()
        : in_ptr(buf_in[0])
        , out_ptr(buf_out[0])
        , in(&in_ptr)
        , out(&out_ptr)
    {
    }

    void fill(float v)
    {
        for (size_t c = 0; c < IN; c++)
            for (size_t s = 0; s < 64; s++)
                buf_in[c][s] = v;
    }

    template <class Gen>
    void fill(Gen fn)
    {
        for (size_t c = 0; c < IN; c++)
            for (size_t s = 0; s < 64; s++)
                buf_in[c][s] = fn(s);
    }
};

template <class F>
static void fill_block(t_sample* b, F f)
{
    std::generate(b, b + 64, f);
}

TEST_CASE("math.round~", "[externals]")
{

    SECTION("process")
    {
        MathRoundTildeTest t("math.round~");
        REQUIRE(t.blockSize() == 64);
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        SigVec<1, 1> sig;
        sig.fill(1.1f);

        t.processBlock(sig.in, sig.out);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == 1.f);
        }

        sig.fill(1.9f);

        t.processBlock(sig.in, sig.out);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == 2.f);
        }

        sig.fill(-0.1f);

        t.processBlock(sig.in, sig.out);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == 0.f);
        }

        sig.fill(-0.6f);

        t.processBlock(sig.in, sig.out);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == -1.f);
        }

        sig.fill(FRandGen(-100, 100));
        t.processBlock(sig.in, sig.out);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(sig.out[0][i] == roundf(sig.in[0][i]));
        }
    }
}
