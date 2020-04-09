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
#include "midi_ctl2str.h"
#include "test_midi_base.h"

PD_COMPLETE_TEST_SETUP(MidiCtl2Str, midi, ctl2str)

TEST_CASE("midi.ctl->str", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("midi.ctl->str", LA("@symbol"));

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
        TExt t("midi.ctl->str");

        t << 64;
        REQUIRE(dataAt(t) == StringAtom("Hold Pedal"));
    }
}
