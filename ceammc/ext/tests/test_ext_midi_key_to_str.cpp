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
#include "../midi/midi_common.h"
#include "../midi/midi_key2str.h"
#include "../string/datatype_string.h"
#include "base_extension_test.h"
#include "catch.hpp"

#define REQUIRE_STRING_OUTPUT(t, str_)                                  \
    {                                                                   \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);                               \
        const DataTypeString* s = t.typedLastDataAt<DataTypeString>(0); \
        REQUIRE(s != 0);                                                \
        REQUIRE(s->str() == str_);                                      \
    }

typedef TestExtension<MidiKey2Str> Key2StrTest;

TEST_CASE("midi.key->str", "[externals]")
{
    pd_init();

    SECTION("init")
    {
        setup_midi_key2str();

        Key2StrTest t("midi.key->str", L1("@symbol"));
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

        t.setProperty("@tonality", L1("F-moll"));
        REQUIRE_PROPERTY(t, @tonality, S("F minor"));

        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "Eb");
    }

    SECTION("str")
    {
        Key2StrTest t("midi.key->str");
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_STRING_OUTPUT(t, "C");
    }
}
