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
#include "midi_note_pass.h"
#include "test_midi_base.h"

PD_COMPLETE_TEST_SETUP(MidiNotePass, midi, note_pass)

TEST_CASE("midi.note.pass", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("midi.note.pass");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @expr, L());
        }

        SECTION("alias")
        {
            TExt t("note.pass");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
        }
    }

    SECTION("basic")
    {
        SECTION(">")
        {
            TExt t("note.pass", LA("$N>60"));
            t << LF(60, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(61, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(61, 127));
            t << LF(62, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
        }
    }

    SECTION("scale")
    {
        SECTION("major")
        {
            TExt t("note.pass", LA("scale(C, major)"));
            t << LF(60, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
            t << LF(61, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(62, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
            t << LF(63, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(64, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 127));
            t << LF(65, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(65, 127));
            t << LF(66, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(67, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(67, 127));
            t << LF(68, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(69, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(69, 127));
            t << LF(70, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(71, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(71, 127));
        }

        SECTION("major")
        {
            TExt t("note.pass", LA("scale(E, major)"));
            t << LF(64, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 127));
            t << LF(65, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(66, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(66, 127));
            t << LF(67, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(68, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(68, 127));
            t << LF(69, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(69, 127));
            t << LF(70, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(71, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(71, 127));
            t << LF(72, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(73, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(73, 127));
            t << LF(74, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(75, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(75, 127));
        }
    }

    SECTION("chords")
    {
        SECTION("sus4")
        {
            TExt t("note.pass", LA("chord(\"Dsus2\")"));
            t << LF(60, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(61, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(62, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
            t << LF(63, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(64, 127);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 127));
            t << LF(65, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(66, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(67, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(68, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(69, 120);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(69, 120));
            t << LF(70, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            t << LF(71, 127);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }
    }
}
