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
#include "midi_vrand.h"
#include "test_midi_base.h"

PD_COMPLETE_TEST_SETUP(MidiVRrand, midi, vrand)

#define VEL_RANGE(lst, note, a, b) \
    {                              \
        REQUIRE(lst.size() >= 2);  \
        REQUIRE(lst[0] == note);   \
        REQUIRE(lst[1] >= a);      \
        REQUIRE(lst[1] <= b);      \
    }

TEST_CASE("midi.vrand", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("midi.vrand");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @min, 0);
            REQUIRE_PROPERTY(t, @max, 127);
            REQUIRE_PROPERTY(t, @seed, 0);
            REQUIRE_PROPERTY(t, @dist, "uniform");
            REQUIRE_PROPERTY(t, @dev, 0);
        }

        SECTION("args")
        {
            TExt t("midi.vrand", LF(20, 40));
            REQUIRE_PROPERTY(t, @min, 20);
            REQUIRE_PROPERTY(t, @max, 40);
        }
    }

    SECTION("process @dev 0")
    {
        TExt t("midi.vrand", 60, 80, "@dist", "uniform");

        for (int i = 0; i < 100; i++) {
            t << 10;
            VEL_RANGE(t.outputListAt(0), 10, 60, 80);
        }

        for (int i = 0; i < 100; i++) {
            t << 127;
            VEL_RANGE(t.outputListAt(0), 127, 60, 80);
        }

        t << -1;
        REQUIRE_FALSE(t.hasOutputAt(0));

        t << 128;
        REQUIRE_FALSE(t.hasOutputAt(0));

        t << LF(100, 2);
        VEL_RANGE(t.outputListAt(0), 100, 60, 80);

        t << LF(100, 2);
        VEL_RANGE(t.outputListAt(0), 100, 60, 80);

        t->setProperty("@dist", LA("cmin"));
        t << 40;
        REQUIRE(t.outputListAt(0) == LF(40, 60));
        t << LF(1, 2, 3);
        REQUIRE(t.outputListAt(0) == LF(1, 60, 3));

        t->setProperty("@dist", LA("cmax"));
        t << 50;
        REQUIRE(t.outputListAt(0) == LF(50, 80));
        t << LF(1, 2, 3);
        REQUIRE(t.outputListAt(0) == LF(1, 80, 3));

        t->setProperty("@dist", LA("cmean"));
        t << 51;
        REQUIRE(t.outputListAt(0) == LF(51, 70));
        t << LF(1, 2, 3);
        REQUIRE(t.outputListAt(0) == LF(1, 70, 3));

        t->setProperty("@dist", LA("linup"));
        for (int i = 0; i < 100; i++) {
            t << 10;
            VEL_RANGE(t.outputListAt(0), 10, 60, 80);
        }

        t->setProperty("@dist", LA("lindown"));
        for (int i = 0; i < 100; i++) {
            t << 10;
            VEL_RANGE(t.outputListAt(0), 10, 60, 80);
        }

        t->setProperty("@dist", LA("gauss"));
        for (int i = 0; i < 100; i++) {
            t << 10;
            VEL_RANGE(t.outputListAt(0), 10, 40, 100);
        }
    }

    SECTION("process @dev 1")
    {
        TExt t("midi.vrand", -10, 10, "@dist", "uniform", "@dev", 1);
        REQUIRE_PROPERTY(t, @min, -10);

        for (int i = 0; i < 100; i++) {
            t << 100;
            VEL_RANGE(t.outputListAt(0), 100, 54, 74);
        }

        for (int i = 0; i < 100; i++) {
            t << LF(120, 40);
            VEL_RANGE(t.outputListAt(0), 120, 30, 50);
        }
    }
}
