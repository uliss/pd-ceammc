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
#include "flt_freqz.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(FltFreqZ, flt, freqz)

TEST_CASE("flt.freqz", "[externals]")
{
    pd_test_init();
    setTestSampleRate(10000);

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("flt.freqz");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 3);
            t->dump();
            REQUIRE_PROPERTY_LIST(t, @b, LF(1));
            REQUIRE_PROPERTY_LIST(t, @a, L());
        }
    }

    SECTION("calc")
    {
        SECTION("a")
        {
            TExt t("flt.freqz", 8, "@b", 0.5, 0.5);
            t.bang();

            // generated with freqz.py
            const double ampz[] = {
                1.0,
                0.9807852804032304,
                0.9238795325112867,
                0.8314696123025452,
                0.7071067811865476,
                0.5555702330196022,
                0.38268343236508984,
                0.19509032201612828,
            };

            const double phasez[] = {
                0.0,
                -0.1963495408493621,
                -0.3926990816987242,
                -0.5890486225480862,
                -0.7853981633974483,
                -0.9817477042468105,
                -1.1780972450961726,
                -1.3744467859455345,
            };

            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.isOutputListAt(1));
            REQUIRE(t.isOutputFloatAt(2));
            REQUIRE(t.outputFloatAt(2) == 8);

            auto a0 = t.outputListAt(0);
            for (int i = 0; i < 8; i++) {
                REQUIRE(a0[i].asFloat() == Approx(ampz[i]));
            }

            auto p0 = t.outputListAt(1);
            for (int i = 0; i < 8; i++) {
                REQUIRE(p0[i].asFloat() == Approx(phasez[i]));
            }
        }
    }

    SECTION("m_biquad")
    {
        TExt t("flt.freqz", 8, "@b", 0.5, 0.5);
        REQUIRE_PROPERTY(t, @b, LF(0.5, 0.5));
        REQUIRE_PROPERTY(t, @a, L());

        t.call("biquad", LF(1, 2, 3, 4, 5));
        REQUIRE_PROPERTY(t, @b, LF(1, 2, 3));
        REQUIRE_PROPERTY(t, @a, LF(4, 5));

        t.call("biquad", LF(1111, 2, 3, 4, 5, 6));
        REQUIRE_PROPERTY(t, @b, LF(1, 2, 3));
        REQUIRE_PROPERTY(t, @a, LF(4, 5));

        t.call("biquad", LF(1, 2, 3, 7));
        REQUIRE_PROPERTY(t, @b, LF(1, 2, 3));
        REQUIRE_PROPERTY(t, @a, LF(4, 5));
    }
}
