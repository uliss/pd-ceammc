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
#include "midi_sustain.h"
#include "test_midi_base.h"

PD_COMPLETE_TEST_SETUP(MidiSustain, midi, sustain)

TEST_CASE("midi.sustain", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    using ML = std::vector<Message>;

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("midi.sustain");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @on, 0);
            REQUIRE_PROPERTY(t, @ctlin, 0);
        }
    }

    SECTION("do")
    {
        TExt t("midi.sustain");

        t << LF(1, 2);
        REQUIRE(t.outputListAt(0) == LF(1, 2));
        t << LF(1, 0);
        REQUIRE(t.outputListAt(0) == LF(1, 0));

        t << LF(10, 20);
        REQUIRE(t.outputListAt(0) == LF(10, 20));
        t->setProperty("@on", LF(1));
        t << LF(10, 0);
        REQUIRE_FALSE(t.hasOutput());
        t << LF(11, 20);
        REQUIRE(t.outputListAt(0) == LF(11, 20));
        t << LF(11, 0);
        REQUIRE_FALSE(t.hasOutput());
        t << LF(11, 0);
        REQUIRE_FALSE(t.hasOutput());
        t->setProperty("@on", LF(0));
        REQUIRE(t.messagesAt(0) == ML { LF(10, 0), LF(11, 0) });

        t << LF(10, 20);
        REQUIRE(t.outputListAt(0) == LF(10, 20));
        t << LF(10, 20);
        REQUIRE(t.outputListAt(0) == LF(10, 20));
        t.clearAll();

        t->setProperty("@ctlin", LF(1));
        Atom args[2] = { 64, 10 };
        pd_typedmess(gensym("#ctlin")->s_thing, &s_list, 2, (t_atom*)args);
        REQUIRE(t.messagesAt(0) == ML { LF(10, 0), LF(11, 0) });
    }
}
