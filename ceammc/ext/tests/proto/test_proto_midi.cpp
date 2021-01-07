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

        // NoteOn with timetick
        t << LF(0x91, 0x42, 0xF9, 0x13);
        REQUIRE(t.messagesAt(0) == ML { M("tick"), M("noteon", 1, 0x42, 0x13) });

        // NoteOff
        t << LF(0x80, 10, 0);
        REQUIRE(t.messagesAt(0) == ML { M("noteoff", 0, 10, 0) });

        // NoteOff with start
        t << LF(0x83, 0xFA, 12, 1);
        REQUIRE(t.messagesAt(0) == ML { M("start"), M("noteoff", 3, 12, 1) });

        // NoteOff with stop
        t << LF(0x83, 12, 0xFC, 1);
        REQUIRE(t.messagesAt(0) == ML { M("stop"), M("noteoff", 3, 12, 1) });

        // NoteOff with continue
        t << LF(0x83, 12, 1, 0xFB);
        REQUIRE(t.messagesAt(0) == ML { M("noteoff", 3, 12, 1), M("continue") });

        // NoteOn with activesense
        t << LF(0x91, 0x42, 0xFE, 0x13);
        REQUIRE(t.messagesAt(0) == ML { M("activesense"), M("noteon", 1, 0x42, 0x13) });

        // NoteOn with system reset
        t << LF(0x91, 0x42, 0xFF, 0x13);
        REQUIRE(t.messagesAt(0) == ML { M("sysreset"), M("noteon", 1, 0x42, 0x13) });

        // aftertouch
        t << LF(0xA2, 0x40, 0x15);
    }

    SECTION("output")
    {
        TExt t0("proto.midi");
        TExt t1("proto.midi");

        t0.connectTo(0, t1, 0);
        t0.call("note", LF(0, 60, 127));
        REQUIRE(t1.messagesAt(0) == ML { M("noteon", 0, 60, 127) });
        t1.clearAll();

        // errors
        t0.call("note", LF(60, 128));
        REQUIRE(t1.messagesAt(0) == ML {});
        t0.call("note", LF(60, -1));
        REQUIRE(t1.messagesAt(0) == ML {});
        t0.call("note", LF(-1, 77));
        REQUIRE(t1.messagesAt(0) == ML {});
        t0.call("note", LF(128, 77));
        REQUIRE(t1.messagesAt(0) == ML {});
        t0.call("note", LA(60, "A"));
        REQUIRE(t1.messagesAt(0) == ML {});
        t0.call("note", LA("A", 60));
        REQUIRE(t1.messagesAt(0) == ML {});
        t0.call("note", LA("A", "B"));
        REQUIRE(t1.messagesAt(0) == ML {});
        t0.call("note", LA(60, 61, "C"));
        REQUIRE(t1.messagesAt(0) == ML {});
        t0.call("note", LA(60, 61, -1));
        REQUIRE(t1.messagesAt(0) == ML {});
        t0.call("note", LA(60, 61, 16));
        REQUIRE(t1.messagesAt(0) == ML {});
        t0.call("note");
        REQUIRE(t1.messagesAt(0) == ML {});
        t0.call("note", LA(60, 61, 15, 16));
        REQUIRE(t1.messagesAt(0) == ML {});

        // ok
        t0.call("note", LF(3, 60, 61));
        REQUIRE(t1.messagesAt(0) == ML { M("noteon", 3, 60, 61) });
        t1.clearAll();

        t0.call("noteon", LF(0, 127, 64));
        REQUIRE(t1.messagesAt(0) == ML { M("noteon", 0, 127, 64) });
        t1.clearAll();

        t0.call("noteoff", LF(0, 60, 0));
        REQUIRE(t1.messagesAt(0) == ML { M("noteoff", 0, 60, 0) });
        t1.clearAll();

        t0.call("aftertouch", LF(0, 60));
        REQUIRE(t1.messagesAt(0) == ML { M("aftertouch", 0, 60) });
        t1.clearAll();

        t0.call("polytouch", LF(0, 60, 43));
        REQUIRE(t1.messagesAt(0) == ML { M("polytouch", 0, 60, 43) });
        t1.clearAll();

        t0.call("cc", LF(0, 79, 18));
        REQUIRE(t1.messagesAt(0) == ML { M("cc", 0, 79, 18) });
        t1.clearAll();

        t0.call("program", LF(1, 79));
        REQUIRE(t1.messagesAt(0) == ML { M("program", 1, 79) });
        t1.clearAll();

        t0.call("pitchwheel", LF(1, 40, 40));
        REQUIRE(t1.messagesAt(0) == ML { M("pitchwheel", 1, 40, 40) });
        t1.clearAll();
    }

    SECTION("float")
    {
        TExt t0("proto.midi", LA("@raw", "false"));
        TExt t1("proto.midi", LA("@raw", "false"));
        t0.connectTo(0, t1, 0);

        t0.call("pitchwheel", LF(1, 1.1));
        REQUIRE(t1.messagesAt(0) == ML {});

        t0.call("pitchwheel", LF(1, -1.1));
        REQUIRE(t1.messagesAt(0) == ML {});

        t0.call("pitchwheel", LF(1));
        REQUIRE(t1.messagesAt(0) == ML {});

        t0.call("pitchwheel", LA("?"));
        REQUIRE(t1.messagesAt(0) == ML {});

        t0.call("pitchwheel", LA("?", "?"));
        REQUIRE(t1.messagesAt(0) == ML {});

        t0.call("pitchwheel", LA(2, "V"));
        REQUIRE(t1.messagesAt(0) == ML {});

        t0.call("pitchwheel", LA(2, 1));
        REQUIRE(t1.messagesAt(0) == ML { M("pitchwheel", 2, 1) });
        t1.clearAll();

        t0.call("pitchwheel", LA(2, 0.f));
        REQUIRE(t1.messagesAt(0) == ML { M("pitchwheel", 2, 0) });
        t1.clearAll();

        t0.call("pitchwheel", LA(2, -1));
        REQUIRE(t1.messagesAt(0) == ML { M("pitchwheel", 2, -1) });
        t1.clearAll();
    }
}
