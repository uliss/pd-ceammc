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
#include "../midi/midi_event.h"
#include "test_base.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExternal<MidiEventToNote> MidiEventNoteTest;

TEST_CASE("midievent.note", "[externals]")
{
    SECTION("init")
    {
        MidiEventNoteTest t("midi.ev->note");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 3);
    }

    SECTION("ignored events")
    {
        MidiEventNoteTest t("midi.ev->note");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 11);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_ANY_TO(t, "any", LF(1, 2, 3));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("midi")
    {
        MidiEventNoteTest t("midi.ev->note");

        WHEN_SEND_ANY_TO(t, "MidiEvent", LF(1, 1, 1));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        // non note
        WHEN_SEND_ANY_TO(t, "MidiEvent", LA(100, 4, 1, 0x60, 0x48, 0x40));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        // note event
        WHEN_SEND_ANY_TO(t, "MidiEvent", LA(100, 4, 200, 0x90, 0x48, 0x40));
        REQUIRE_FLOAT_AT_OUTLET(2, t, 4);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 200);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(72, 64));
    }
}
