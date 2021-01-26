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
#include "flt_common.h"
#include "flt_lpf2biquad.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(FltLpf2Biquad, flt, lpf2biquad)

TEST_CASE("flt.c_lpf", "[externals]")
{
    pd_test_init();
    setTestSampleRate(10000);

    SECTION("flt_common")
    {
        using namespace ceammc::flt;
        REQUIRE(hz2w(0, 1000) == 0);
        REQUIRE(hz2w(500, 1000) == Approx(m_pi));

        for (float i = 0.1; i < 10; i++) {
            REQUIRE(bandwidth2q(q2bandwidth(i, 0), 0) == i);
            REQUIRE(q2bandwidth(bandwidth2q(i, 0), 0) == i);
        }

        for (float i = 0.1; i < 10; i++) {
            REQUIRE(bandwidth2q(q2bandwidth(i, m_pi), m_pi) == Approx(i));
            REQUIRE(q2bandwidth(bandwidth2q(i, 1), 1) == Approx(i));
        }

        for (float i = 0.1; i < 10; i++) {
            REQUIRE(bandwidth2q(q2bandwidth(i, m_pi / 2), m_pi / 2) == Approx(i));
            REQUIRE(q2bandwidth(bandwidth2q(i, m_pi / 2), m_pi / 2) == Approx(i));
        }
    }

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("flt.c_lpf");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @freq, 20);
            REQUIRE_PROPERTY_FLOAT(t, @norm, 1);
            REQUIRE_PROPERTY_FLOAT(t, @q, 1 / std::sqrt(2));
        }

        SECTION("arg")
        {
            TExt t("flt.c_lpf", LF(1000));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @freq, 1000);
            REQUIRE_PROPERTY_FLOAT(t, @norm, 1);
        }

        SECTION("alias")
        {
            TExt t("lpf->biquad");
        }
    }

    SECTION("calc")
    {
        SECTION("a")
        {
            TExt t("flt.c_lpf", 2500, "@q", 1);

            t->dump();

            t.bang();
            REQUIRE(t.outputListAt(0) == LX(1.0 / 3, 2.0 / 3, 1.0 / 3, 0, 1.0 / 3).margin(0.0000001));

            t << 5000;
            REQUIRE(t.outputListAt(0) == LF(1, 2, 1, 2, 1));
        }
    }
}
