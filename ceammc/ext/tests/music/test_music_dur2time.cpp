/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "duration_to_time.h"
#include "test_base.h"
#include "test_external.h"

using namespace ceammc;

PD_COMPLETE_TEST_SETUP(Duration2Time, music, duration2time)

TEST_CASE("music.dur2time", "[ceammc::music]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("music.d->t");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @seq, 1);
        REQUIRE_PROPERTY(t, @bpm, LF(60, 0.25));
    }

    SECTION("alias")
    {
        TExt t0("music.dur2time");
        TExt t1("music.d->t");
    }

    SECTION("do")
    {
        TExt t("music.d->t", "@bpm", "120-4bpm");
        REQUIRE_PROPERTY(t, @bpm, LF(120, 0.25));

        t << LF(1, 2, 4, 8);
        REQUIRE(t.outputListAt(0) == LF(2000, 3000, 3500, 3750));

        t << LA("2/4");
        REQUIRE(t.outputListAt(0) == LF(1000));

        t << LA("4.", "8");
        REQUIRE(t.outputListAt(0) == LF(750, 1000));

        t << LA("1/4.", "1/8");
        REQUIRE(t.outputListAt(0) == LF(750, 1000));

        t << LA("4*4");
        REQUIRE(t.outputListAt(0) == LF(500, 1000, 1500, 2000));

        t << LA("2*4", "2*8", "1*16");
        REQUIRE(t.outputListAt(0) == LF(500, 1000, 1250, 1500, 1625));

        t << 4;
        REQUIRE(t.outputFloatAt(0) == 500);

        t << 2;
        REQUIRE(t.outputFloatAt(0) == 1000);

        t << "4";
        REQUIRE(t.outputListAt(0) == LF(500));

        t << "3/4";
        REQUIRE(t.outputListAt(0) == LF(1500));

        t << "2*2/4";
        REQUIRE(t.outputListAt(0) == LF(1000, 2000));
    }
}
