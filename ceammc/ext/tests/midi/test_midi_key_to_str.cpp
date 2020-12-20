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
#include "midi_common.h"
#include "midi_key2str.h"
#include "test_midi_base.h"

PD_COMPLETE_TEST_SETUP(MidiKey2Str, midi, key2str)

TEST_CASE("midi.key->str", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("midi.key->str", LA("@symbol"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @symbol, 1);
        REQUIRE_PROPERTY(t, @tonality, S("C major"));

        WHEN_SEND_FLOAT_TO(0, t, -1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 128);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 0.f);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "C");

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "C#");

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "Bb");

        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "D#");

        t.setProperty("@tonality", LA("F-moll"));
        REQUIRE_PROPERTY(t, @tonality, S("F minor"));

        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "Eb");
    }

    SECTION("str")
    {
        TExt t("midi.key->str");

        t << 0.0;
        REQUIRE(dataAt(t) == StringAtom("C"));
    }
}
