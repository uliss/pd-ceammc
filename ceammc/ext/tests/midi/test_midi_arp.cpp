/*****************************************************************************
 * Copyright 2023 Serge Poltavsky. All rights reserved.
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
#include "midi_arp.h"
#include "test_midi_base.h"

PD_COMPLETE_TEST_SETUP(MidiArp, midi, arp)

TEST_CASE("midi.arp", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("midi.arp");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @t, 100);
            REQUIRE_PROPERTY(t, @mode, "up");
            REQUIRE_PROPERTY(t, @external, 0);
            REQUIRE_PROPERTY(t, @pass, 0);
            REQUIRE_PROPERTY(t, @min_notes, 1);
        }

        SECTION("args")
        {
            TExt t("midi.arp", 200);
            REQUIRE_PROPERTY(t, @t, 200);
        }
    }

    SECTION("up")
    {
        TExt t("midi.arp", LA("@mode", "up", "@external", 1));

        // single note
        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));

        t << LF(60, 0);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 0));
        t.sendBangTo(1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        // single note velocity
        t << LF(60, 60);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t << LF(60, 0);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 0));
        t.sendBangTo(1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        // two notes
        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t << LF(62, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));

        t << LF(60, 0);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));

        t << LF(62, 0);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 0));
    }

    SECTION("up1")
    {
        TExt t("midi.arp", LA("@mode", "up1", "@external", 1));

        // single note
        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        // note off
        t << LF(60, 0);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 0));
        t.sendBangTo(1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        // 3 notes
        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t << LF(62, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t << LF(64, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 127));
    }

    SECTION("down")
    {
        TExt t("midi.arp", LA("@mode", "down", "@external", 1));

        // single note
        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        // note off
        t << LF(60, 0);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 0));
        t.sendBangTo(1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        // 3 notes
        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t << LF(62, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t << LF(64, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 127));
    }

    SECTION("down1")
    {
        TExt t("midi.arp", LA("@mode", "down1", "@external", 1));

        // single note
        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        // note off
        t << LF(60, 0);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 0));
        t.sendBangTo(1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        // 3 notes
        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t << LF(62, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t << LF(64, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 127));
    }

    SECTION("tri")
    {
        TExt t("midi.arp", LA("@mode", "tri", "@external", 1));

        // single note
        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        // note off
        t << LF(60, 0);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 0));
        t.sendBangTo(1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        // 3 notes
        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t << LF(62, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t << LF(64, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
    }

    SECTION("tri1")
    {
        TExt t("midi.arp", LA("@mode", "tri1", "@external", 1));

        // single note
        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        // note off
        t << LF(60, 0);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 0));
        t.sendBangTo(1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        // 3 notes
        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t << LF(62, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t << LF(64, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
    }

    SECTION("random")
    {
        TExt t("midi.arp", LA("@mode", "random", "@external", 1, "@seed", 1));

        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        // note off
        t << LF(60, 0);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 0));
        t.sendBangTo(1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

#ifdef __APPLE__
        t << LF(60, 127);
        t << LF(62, 127);
        t << LF(63, 127);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(63, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
#endif
    }

    SECTION("random")
    {
        TExt t("midi.arp", LA("@mode", "random1", "@external", 1));
        REQUIRE_PROPERTY_FLOAT(t, @seed, 0.);

        t << LF(60, 127);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        // note off
        t << LF(60, 0);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 0));
        t.sendBangTo(1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

#ifdef __APPLE__
        t << LF(60, 127);
        t << LF(62, 127);
        t << LF(63, 127);
        t.sendMessage("@seed", 2);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(63, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(63, 127));

        LIB_ERR << "####";
        t->setProperty("@seed", LF(2));
        REQUIRE_PROPERTY_FLOAT(t, @seed, 2);
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(63, 127));
        t.sendBangTo(1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(63, 127));
#endif
    }
}
