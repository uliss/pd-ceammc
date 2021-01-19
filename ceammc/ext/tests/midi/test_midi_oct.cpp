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
#include "midi_octave.h"
#include "test_midi_base.h"

PD_COMPLETE_TEST_SETUP(MidiOctave, midi, octave)

TEST_CASE("midi.oct", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("midi.oct");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @oct, 0);
            REQUIRE_PROPERTY(t, @mode, "transpose");
            REQUIRE_PROPERTY(t, @set, 0);
            REQUIRE_PROPERTY(t, @transpose, 1);

            t.sendFloatTo(4, 1);
            REQUIRE_PROPERTY(t, @oct, 4);
        }

        SECTION("args")
        {
            TExt t("midi.oct", -5);
            REQUIRE_PROPERTY(t, @oct, -5);
        }
    }

    SECTION("@transpose")
    {
        TExt t("midi.oct");

        t << 0.;
        REQUIRE(t.outputFloatAt(0) == 0);
        t << 64.5;
        REQUIRE(t.outputFloatAt(0) == 64.5);
        t << 127;
        REQUIRE(t.outputFloatAt(0) == 127);

        // invalid
        t << 128;
        REQUIRE(t.outputFloatAt(0) == 127);
        t << -1;
        REQUIRE(t.outputFloatAt(0) == 0);

        // list
        t << LF(1, 2);
        REQUIRE(t.outputListAt(0) == LF(1, 2));
        t << LF(1, 2, 3);
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3));
        t << LF(1, 2, 3, 4);
        REQUIRE_FALSE(t.hasOutputAt(0));

        // @oct -2
        t->setProperty("@oct", LF(-2));
        t << 0.;
        REQUIRE(t.outputFloatAt(0) == 0);
        t << 1;
        REQUIRE(t.outputFloatAt(0) == 1);
        t << 12;
        REQUIRE(t.outputFloatAt(0) == 12);
        t << 23;
        REQUIRE(t.outputFloatAt(0) == 23);
        t << 24;
        REQUIRE(t.outputFloatAt(0) == 0);
        t << 25;
        REQUIRE(t.outputFloatAt(0) == 1);
        t << 124;
        REQUIRE(t.outputFloatAt(0) == 100);

        t << LF(1, 64);
        REQUIRE(t.outputListAt(0) == LF(1, 64));
        t << LF(23, 64);
        REQUIRE(t.outputListAt(0) == LF(23, 64));
        t << LF(24, 64);
        REQUIRE(t.outputListAt(0) == LF(0, 64));
        t << LF(124, 64);
        REQUIRE(t.outputListAt(0) == LF(100, 64));
        t << LF(124, 64, 100);
        REQUIRE(t.outputListAt(0) == LF(100, 64, 100));

        // @oct +5
        t->setProperty("@oct", LF(5));
        t << 0.;
        REQUIRE(t.outputFloatAt(0) == 60);
        t << 1;
        REQUIRE(t.outputFloatAt(0) == 61);
        t << 12;
        REQUIRE(t.outputFloatAt(0) == 72);
        t << 60;
        REQUIRE(t.outputFloatAt(0) == 120);
        t << 67;
        REQUIRE(t.outputFloatAt(0) == 127);
        t << 68;
        REQUIRE(t.outputFloatAt(0) == 68);
        t << 69;
        REQUIRE(t.outputFloatAt(0) == 69);

        t << LF(60, 64);
        REQUIRE(t.outputListAt(0) == LF(120, 64));
        t << LF(67, 64);
        REQUIRE(t.outputListAt(0) == LF(127, 64));
        t << LF(68, 64);
        REQUIRE(t.outputListAt(0) == LF(68, 64));
        t << LF(67, 64, 100);
        REQUIRE(t.outputListAt(0) == LF(127, 64, 100));
        t << LF(68, 64, 100);
        REQUIRE(t.outputListAt(0) == LF(68, 64, 100));
    }
}
