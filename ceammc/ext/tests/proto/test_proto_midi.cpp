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
#include "proto_midi.h"
#include "test_proto_base.h"

PD_COMPLETE_TEST_SETUP(ProtoMidi, proto, midi)

template <class... Args>
static Message M(const char* m, Args... args)
{
    return Message(gensym(m), AtomList(args...));
}

using ML = std::vector<Message>;

TEST_CASE("proto.midi", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("create")
    {
        SECTION("default")
        {
            TExt t("proto.midi");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }
    }

    SECTION("input")
    {
        TExt t("proto.midi");

        // NoteOn
        t << LF(0x91, 0x40, 0x11);
        REQUIRE(t.messagesAt(0) == ML { M("noteon", 1, 0x40, 0x11) });

        // NoteOn with timeclock
        t << LF(0x91, 0xF8, 0x41, 0x12);
        REQUIRE(t.messagesAt(0) == ML { M("clock"), M("noteon", 1, 0x41, 0x12) });

        // NoteOn with timeclock
        t << LF(0x91, 0x42, 0xF9, 0x13);
        REQUIRE(t.messagesAt(0) == ML { M("tick"), M("noteon", 1, 0x42, 0x13) });

        // NoteOff
        t << LF(0x80, 10, 0);
        REQUIRE(t.messagesAt(0) == ML { M("noteoff", 0, 10, 0) });
    }
}
