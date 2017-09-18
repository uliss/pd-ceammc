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
#include "../midi/midi_ctl2str.h"
#include "../string/datatype_string.h"
#include "base_extension_test.h"
#include "catch.hpp"

typedef TestExtension<MidiCtl2Str> Ctl2StrTest;

#define REQUIRE_STRING_OUTPUT(t, str_)                                  \
    {                                                                   \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);                               \
        const DataTypeString* s = t.typedLastDataAt<DataTypeString>(0); \
        REQUIRE(s != 0);                                                \
        REQUIRE(s->str() == str_);                                      \
    }

TEST_CASE("midi.ctl->str", "[externals]")
{
    pd_init();

    SECTION("init")
    {
        setup_midi_ctl2str();

        Ctl2StrTest t("midi.ctl->str", L1("@symbol"));

        WHEN_SEND_FLOAT_TO(0, t, -1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 128);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 129);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 0.f);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "Bank Select");

        WHEN_SEND_FLOAT_TO(0, t, 15);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "");

        WHEN_SEND_FLOAT_TO(0, t, 16);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "General Purpose Slider 1");

        WHEN_SEND_FLOAT_TO(0, t, 64);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "Hold Pedal");

        WHEN_SEND_FLOAT_TO(0, t, 120);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "All Sound Off");

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "Pan");

        WHEN_SEND_FLOAT_TO(0, t, 85);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "");
        WHEN_SEND_FLOAT_TO(0, t, 90);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "");
        WHEN_SEND_FLOAT_TO(0, t, 102);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "");
        WHEN_SEND_FLOAT_TO(0, t, 119);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "");
    }

    SECTION("str")
    {
        Ctl2StrTest t("midi.ctl->str");
        WHEN_SEND_FLOAT_TO(0, t, 64);
        REQUIRE_STRING_OUTPUT(t, "Hold Pedal");
    }
}
