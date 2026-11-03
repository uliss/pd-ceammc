/*****************************************************************************
 * Copyright 2026 Serge Poltavsky. All rights reserved.
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
#include "midi_pitch_quantize.h"
#include "test_midi_base.h"

PD_COMPLETE_TEST_SETUP(MidiPitchQuantize, midi, pitch_quantize)

TEST_CASE("midi.pitch.quantize", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TExt t("midi.pitch.quantize");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @mode, "reject");
        REQUIRE_PROPERTY(t, @min, 0.0);
        REQUIRE_PROPERTY(t, @max, 127);
    }

    SECTION("alias")
    {
        TExt t("midi.pitch.q");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("reject")
    {
        TExt t("midi.pitch.q");

        SECTION("float")
        {
            t << 60;
            REQUIRE_FLOAT_AT_OUTLET(0, t, 60);
            t << 60.5;
            REQUIRE_FLOAT_AT_OUTLET(0, t, 61);
            t << 0.0;
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0.0);
            t << 127.0;
            REQUIRE_FLOAT_AT_OUTLET(0, t, 127.0);
            t << -0.5;
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << 128.0;
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }

        SECTION("list")
        {
            t << LF(60, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
            t << LF(60.5, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(61, 127));
            t << LF(62, -0.5);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(62, -0.5));
            t << LA(62, "A");
            REQUIRE_LIST_AT_OUTLET(0, t, LA(62, "A"));
            t << LA(62, "A", "B");
            REQUIRE_LIST_AT_OUTLET(0, t, LA(62, "A", "B"));
            t << LA(-1, "A");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LA(128, "A");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }
    }
}
