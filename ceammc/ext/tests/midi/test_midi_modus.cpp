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
#include "midi_modus.h"
#include "test_midi_base.h"

PD_COMPLETE_TEST_SETUP(MidiModus, midi, modus)

TEST_CASE("midi.modus", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("midi.modus");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @root, "C");
            REQUIRE_PROPERTY(t, @scale, "chromatic");
        }

        SECTION("args")
        {
            TExt t0("midi.modus", 1);
            REQUIRE_PROPERTY(t0, @root, 1);

            TExt t1("midi.modus", "C##");
            REQUIRE_PROPERTY(t1, @root, "C##");
        }
    }

    SECTION("snap")
    {
        SECTION("C chromatic")
        {
            TExt t("midi.modus", "C", "chromatic", "@mode", "snap");

            for (int i = 0; i < 127; i++) {
                t << i;
                REQUIRE(t.outputFloatAt(0) == i);
            }

            for (int i = 0; i < 127; i++) {
                t << LF(float(i), 64);
                REQUIRE(t.outputListAt(0) == LF(float(i), 64));
            }

            for (int i = 0; i < 127; i++) {
                t << LF(float(i), 64, 1000);
                REQUIRE(t.outputListAt(0) == LF(float(i), 64, 1000));
            }
        }

        SECTION("F chromatic")
        {
            TExt t("midi.modus", "F", "chromatic", "@mode", "snap");

            for (int i = 0; i < 127; i++) {
                t << i;
                REQUIRE(t.outputFloatAt(0) == i);
            }
        }

        SECTION("C whole")
        {
            TExt t("midi.modus", "C", "whole", "@mode", "snap");

            t << 60;
            REQUIRE(t.outputFloatAt(0) == 60);
            t << 61;
            REQUIRE(t.outputFloatAt(0) == 62);
            t << 62;
            REQUIRE(t.outputFloatAt(0) == 62);
            t << 63;
            REQUIRE(t.outputFloatAt(0) == 64);
            t << 64;
            REQUIRE(t.outputFloatAt(0) == 64);
            t << 65;
            REQUIRE(t.outputFloatAt(0) == 66);
            t << 66;
            REQUIRE(t.outputFloatAt(0) == 66);
            t << 67;
            REQUIRE(t.outputFloatAt(0) == 68);
            t << 68;
            REQUIRE(t.outputFloatAt(0) == 68);
            t << 69;
            REQUIRE(t.outputFloatAt(0) == 70);
            t << 70;
            REQUIRE(t.outputFloatAt(0) == 70);
            t << 71;
            REQUIRE(t.outputFloatAt(0) == 70);
            t << 72;
            REQUIRE(t.outputFloatAt(0) == 72);
        }

        SECTION("D whole")
        {
            TExt t("midi.modus", "D", "whole", "@mode", "snap");

            t << 60;
            REQUIRE(t.outputFloatAt(0) == 60);
            t << 61;
            REQUIRE(t.outputFloatAt(0) == 60);
            t << 62;
            REQUIRE(t.outputFloatAt(0) == 62);
            t << 63;
            REQUIRE(t.outputFloatAt(0) == 64);
            t << 64;
            REQUIRE(t.outputFloatAt(0) == 64);
            t << 65;
            REQUIRE(t.outputFloatAt(0) == 66);
            t << 66;
            REQUIRE(t.outputFloatAt(0) == 66);
            t << 67;
            REQUIRE(t.outputFloatAt(0) == 68);
            t << 68;
            REQUIRE(t.outputFloatAt(0) == 68);
            t << 69;
            REQUIRE(t.outputFloatAt(0) == 70);
            t << 70;
            REQUIRE(t.outputFloatAt(0) == 70);
            t << 71;
            REQUIRE(t.outputFloatAt(0) == 72);
            t << 72;
            REQUIRE(t.outputFloatAt(0) == 72);
        }

        SECTION("C ritusen")
        {
            // 0, 2, 5, 7, 9
            TExt t("midi.modus", "C", "ritusen", "@mode", "snap");

            t << 60;
            REQUIRE(t.outputFloatAt(0) == 60);
            t << 61;
            REQUIRE(t.outputFloatAt(0) == 62);
            t << 62;
            REQUIRE(t.outputFloatAt(0) == 62);
            t << 63;
            REQUIRE(t.outputFloatAt(0) == 62);
            t << 64;
            REQUIRE(t.outputFloatAt(0) == 65);
            t << 65;
            REQUIRE(t.outputFloatAt(0) == 65);
            t << 66;
            REQUIRE(t.outputFloatAt(0) == 67);
            t << 67;
            REQUIRE(t.outputFloatAt(0) == 67);
            t << 68;
            REQUIRE(t.outputFloatAt(0) == 69);
            t << 69;
            REQUIRE(t.outputFloatAt(0) == 69);
            t << 70;
            REQUIRE(t.outputFloatAt(0) == 69);
            t << 71;
            REQUIRE(t.outputFloatAt(0) == 72);
            t << 72;
            REQUIRE(t.outputFloatAt(0) == 72);
        }

        SECTION("D ritusen")
        {
            // 0, 2, 5, 7, 9
            // 2, 4, 7, 9, 11
            TExt t("midi.modus", "D", "ritusen", "@mode", "snap");

            t << 60;
            REQUIRE(t.outputFloatAt(0) == 59);
            t << 61;
            REQUIRE(t.outputFloatAt(0) == 62);
            t << 62;
            REQUIRE(t.outputFloatAt(0) == 62);
            t << 63;
            REQUIRE(t.outputFloatAt(0) == 64);
            t << 64;
            REQUIRE(t.outputFloatAt(0) == 64);
            t << 65;
            REQUIRE(t.outputFloatAt(0) == 64);
            t << 66;
            REQUIRE(t.outputFloatAt(0) == 67);
            t << 67;
            REQUIRE(t.outputFloatAt(0) == 67);
            t << 68;
            REQUIRE(t.outputFloatAt(0) == 69);
            t << 69;
            REQUIRE(t.outputFloatAt(0) == 69);
            t << 70;
            REQUIRE(t.outputFloatAt(0) == 71);
            t << 71;
            REQUIRE(t.outputFloatAt(0) == 71);
            t << 72;
            REQUIRE(t.outputFloatAt(0) == 71);
        }
    }
}
