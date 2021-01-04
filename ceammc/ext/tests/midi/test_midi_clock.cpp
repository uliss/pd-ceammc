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
#include "midi_clock.h"
#include "test_midi_base.h"

PD_COMPLETE_TEST_SETUP(MidiClock, midi, clock)

static void sendClock()
{
    auto sym = gensym("#midirealtimein");
    if (sym->s_thing) {
        t_atom a[2];
        SETFLOAT(a, 1);
        SETFLOAT(a + 1, 0xF8);
        pd_typedmess(sym->s_thing, &s_list, 2, a);
    }
}

#define NSEND(n)          \
    {                     \
        int t = n;        \
        while (t-- > 0) { \
            sendClock();  \
        }                 \
    }

TEST_CASE("midi.clock", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("midi.clock");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);

        REQUIRE_PROPERTY_FLOAT(t, @div, 0);
    }

    SECTION("process")
    {
        using ML = std::vector<Message>;
        const auto b = Message::makeBang();

        TExt t("midi.clock");

        sendClock();
        REQUIRE(t.messagesAt(0) == ML { b });
        sendClock();
        REQUIRE(t.messagesAt(0) == ML { b, b });
        sendClock();
        REQUIRE(t.messagesAt(0) == ML { b, b, b });

        t.clearAll();
        t->setProperty("@div", LF(32));
        sendClock();
        REQUIRE(t.messagesAt(0) == ML { b });
        sendClock();
        REQUIRE(t.messagesAt(0) == ML { b });
        sendClock();
        REQUIRE(t.messagesAt(0) == ML { b });

        sendClock();
        REQUIRE(t.messagesAt(0) == ML { b, b });
        sendClock();
        REQUIRE(t.messagesAt(0) == ML { b, b });
        sendClock();
        REQUIRE(t.messagesAt(0) == ML { b, b });

        t.clearAll();
        t->setProperty("@div", LF(16));
        NSEND(6);
        REQUIRE(t.messagesAt(0) == ML { b });
        NSEND(6);
        REQUIRE(t.messagesAt(0) == ML { b, b });

        t.clearAll();
        t->setProperty("@div", LF(8));
        NSEND(12);
        REQUIRE(t.messagesAt(0) == ML { b });
        NSEND(12);
        REQUIRE(t.messagesAt(0) == ML { b, b });

        t.clearAll();
        t->setProperty("@div", LF(4));
        NSEND(24);
        REQUIRE(t.messagesAt(0) == ML { b });
        NSEND(24);
        REQUIRE(t.messagesAt(0) == ML { b, b });
        NSEND(24);
        REQUIRE(t.messagesAt(0) == ML { b, b, b });
    }
}
