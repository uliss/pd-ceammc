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
#include "../fx/fx_looper.h"
#include "base_extension_test.h"
#include "catch.hpp"

extern "C" {
#include "s_stuff.h"
}

#include <algorithm>
#include <cstdlib>
#include <stdio.h>

typedef TestSoundExtension<FxLooper> FxLooperTest;

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

typedef SigVec<1, 1> Vec;

#define REQUIRE_OUT_VEC(v, f)                  \
    {                                          \
        for (size_t i = 0; i < 64; i++) {      \
            REQUIRE(v.out[0][i] == Approx(f)); \
        }                                      \
    }

TEST_CASE("fx.looper~", "[externals]")
{
    sys_dacsr = 44100;

    SECTION("init")
    {
        FxLooperTest t("fx.looper~");
        REQUIRE(t.blockSize() == 64);
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @capacity, 5);
        REQUIRE_PROPERTY(t, @state, "init");

        Vec v;
        v.fill(1.2f);

        // init state
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 0);

        // RECORD
        t.m_record(&s_, AtomList());
        REQUIRE_PROPERTY(t, @state, "record");
        t.processBlock(v.in, v.out);
        v.fill(1.3f);
        t.processBlock(v.in, v.out);
        v.fill(-1.f);
        t.processBlock(v.in, v.out);

        // PLAY
        t.m_play(&s_, AtomList());
        REQUIRE_PROPERTY(t, @state, "play");
        REQUIRE_PROPERTY(t, @length, (3 * 64) / 44100.0f);
        // 1.2
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 1.2f);
        // 1.3
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 1.3f);
        // -1
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, -1.f);
        // 1.2
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 1.2f);
        // 1.3
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 1.3f);
        // -1
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, -1.f);

        // OVERDUB
        t.m_overdub(&s_, AtomList());
        REQUIRE_PROPERTY(t, @state, "overdub");
        v.fill(0.1f);

        // the same
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 1.2f);
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 1.3f);
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, -1.f);

        // +0.1
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 1.3f);
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 1.4f);
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, -0.9f);

        // +0.1
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 1.4f);
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 1.5f);
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, -0.8f);

        t.m_stop(&s_, AtomList());
        REQUIRE_PROPERTY(t, @state, "stop");
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 0);

        t.m_play(&s_, AtomList());
        REQUIRE_PROPERTY(t, @state, "play");
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 1.5f);
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, 1.6f);
        t.processBlock(v.in, v.out);
        REQUIRE_OUT_VEC(v, -0.7f);
    }
}
