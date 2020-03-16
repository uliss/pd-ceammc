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
#include "matrix.h"
#include "test_external.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(Matrix, base, matrix)

TEST_CASE("matrix~", "[externals]")
{
    pd_test_mod_init_base_matrix();

    SECTION("construct")
    {
        SECTION("default")
        {
            TestExtMatrix t("matrix~", L(), true);
            REQUIRE(t.blockSize() == 64);
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numInputChannels() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE(t.numOutputChannels() == 2);
            REQUIRE_PROPERTY_FLOAT(t, @inputs, 2);
            REQUIRE_PROPERTY_FLOAT(t, @outputs, 2);
        }

        SECTION("args")
        {
            TestExtMatrix t("matrix~", LF(4, 2), true);
            REQUIRE(t.numInlets() == 4);
            REQUIRE(t.numInputChannels() == 4);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE(t.numOutputChannels() == 2);
            REQUIRE_PROPERTY_FLOAT(t, @inputs, 4);
            REQUIRE_PROPERTY_FLOAT(t, @outputs, 2);
        }

        SECTION("limits")
        {
            TestExtMatrix t("matrix~", LF(1, 16), true);
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numInputChannels() == 2);
            REQUIRE(t.numOutlets() == 16);
            REQUIRE(t.numOutputChannels() == 16);
            REQUIRE_PROPERTY_FLOAT(t, @inputs, 2);
            REQUIRE_PROPERTY_FLOAT(t, @outputs, 16);
        }

        SECTION("limits")
        {
            TestExtMatrix t("matrix~", LF(0.f, 17), true);
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numInputChannels() == 2);
            REQUIRE(t.numOutlets() == 16);
            REQUIRE(t.numOutputChannels() == 16);
            REQUIRE_PROPERTY_FLOAT(t, @inputs, 2);
            REQUIRE_PROPERTY_FLOAT(t, @outputs, 16);
        }
    }

    SECTION("process all")
    {
        TestExtMatrix t("matrix~", LF(3, 2), true);
        TestSignal<3, 2> s0;
        s0.fillInputN(0, 0.1);
        s0.fillInputN(1, 0.2);
        s0.fillInputN(2, 0.4);

        MatrixDSP<3, 2> dsp(s0, t);
        dsp.processBlock(20);

        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
            REQUIRE(dsp.out(1, i) == Approx(0));
        }

        // X . .
        // . . .
        t.m_cell(gensym("cell"), LF(0.f, 0.f, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0.1));
            REQUIRE(dsp.out(1, i) == Approx(0));
        }

        // . X .
        // . . .
        t.m_cell(gensym("cell"), LF(0.f, 0.f, 0.f));
        t.m_cell(gensym("cell"), LF(0.f, 1, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0.2));
            REQUIRE(dsp.out(1, i) == Approx(0));
        }

        // X X .
        // . . .
        t.m_cell(gensym("cell"), LF(0.f, 0.f, 1));
        t.m_cell(gensym("cell"), LF(0.f, 1, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0.3));
            REQUIRE(dsp.out(1, i) == Approx(0));
        }

        // . . X
        // . . .
        t.m_cell(gensym("cell"), LF(0.f, 0.f, 0.f));
        t.m_cell(gensym("cell"), LF(0.f, 1, 0.f));
        t.m_cell(gensym("cell"), LF(0.f, 2, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0.4));
            REQUIRE(dsp.out(1, i) == Approx(0));
        }

        // X . X
        // . . .
        t.m_cell(gensym("cell"), LF(0.f, 0.f, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0.5));
            REQUIRE(dsp.out(1, i) == Approx(0));
        }

        // . X X
        // . . .
        t.m_cell(gensym("cell"), LF(0.f, 0.f, 0.f));
        t.m_cell(gensym("cell"), LF(0.f, 1, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0.6));
            REQUIRE(dsp.out(1, i) == Approx(0));
        }

        // X X X
        // . . .
        t.m_cell(gensym("cell"), LF(0.f, 0.f, 1));
        t.m_cell(gensym("cell"), LF(0.f, 1, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0.7));
            REQUIRE(dsp.out(1, i) == Approx(0));
        }

        // right
        // . . .
        // X . .
        t.m_cell(gensym("cell"), LF(0.f, 0.f, 0.f));
        t.m_cell(gensym("cell"), LF(0.f, 1, 0.f));
        t.m_cell(gensym("cell"), LF(0.f, 2, 0.f));
        t.m_cell(gensym("cell"), LF(1, 0.f, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
            REQUIRE(dsp.out(1, i) == Approx(0.1));
        }

        // . . .
        // . X .
        t.m_cell(gensym("cell"), LF(1, 0.f, 0.f));
        t.m_cell(gensym("cell"), LF(1, 1, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
            REQUIRE(dsp.out(1, i) == Approx(0.2));
        }

        // . . .
        // X X .
        t.m_cell(gensym("cell"), LF(1, 0.f, 1));
        t.m_cell(gensym("cell"), LF(1, 1, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
            REQUIRE(dsp.out(1, i) == Approx(0.3));
        }

        // . . .
        // . . X
        t.m_cell(gensym("cell"), LF(1, 0.f, 0.f));
        t.m_cell(gensym("cell"), LF(1, 1, 0.f));
        t.m_cell(gensym("cell"), LF(1, 2, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
            REQUIRE(dsp.out(1, i) == Approx(0.4));
        }

        // . . .
        // X . X
        t.m_cell(gensym("cell"), LF(1, 0.f, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
            REQUIRE(dsp.out(1, i) == Approx(0.5));
        }

        // . . .
        // . X X
        t.m_cell(gensym("cell"), LF(1, 0.f, 0.f));
        t.m_cell(gensym("cell"), LF(1, 1, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
            REQUIRE(dsp.out(1, i) == Approx(0.6));
        }

        // . . .
        // X X X
        t.m_cell(gensym("cell"), LF(1, 0.f, 1));
        t.m_cell(gensym("cell"), LF(1, 1, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0));
            REQUIRE(dsp.out(1, i) == Approx(0.7));
        }

        // . X .
        // X X X
        t.m_cell(gensym("cell"), LA(0.0, 1, 1));
        dsp.processBlock(20);
        for (size_t i = 0; i < 64; i++) {
            REQUIRE(dsp.out(0, i) == Approx(0.2));
            REQUIRE(dsp.out(1, i) == Approx(0.7));
        }
    }
}
