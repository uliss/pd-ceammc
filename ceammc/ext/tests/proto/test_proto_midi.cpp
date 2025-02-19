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
            REQUIRE(t.numOutlets() == 2);
        }
    }

    SECTION("input")
    {
        TExt t("proto.midi");

        // NoteOn
        t << LF(0x91, 0x40, 0x11);
        REQUIRE(t.messagesAt(1) == ML { M("noteon", 1, 0x40, 0x11) });

        // NoteOn with timeclock
        t << LF(0x91, 0xF8, 0x41, 0x12);
        REQUIRE(t.messagesAt(1) == ML { M("clock"), M("noteon", 1, 0x41, 0x12) });

        // NoteOn with timetick
        t << LF(0x91, 0x42, 0xF9, 0x13);
        REQUIRE(t.messagesAt(1) == ML { M("tick"), M("noteon", 1, 0x42, 0x13) });

        // NoteOff
        t << LF(0x80, 10, 0);
        REQUIRE(t.messagesAt(1) == ML { M("noteoff", 0, 10, 0) });

        // NoteOff with start
        t << LF(0x83, 0xFA, 12, 1);
        REQUIRE(t.messagesAt(1) == ML { M("start"), M("noteoff", 3, 12, 1) });

        // NoteOff with stop
        t << LF(0x83, 12, 0xFC, 1);
        REQUIRE(t.messagesAt(1) == ML { M("stop"), M("noteoff", 3, 12, 1) });

        // NoteOff with continue
        t << LF(0x83, 12, 1, 0xFB);
        REQUIRE(t.messagesAt(1) == ML { M("noteoff", 3, 12, 1), M("continue") });

        // NoteOn with activesense
        t << LF(0x91, 0x42, 0xFE, 0x13);
        REQUIRE(t.messagesAt(1) == ML { M("activesense"), M("noteon", 1, 0x42, 0x13) });

        // NoteOn with system reset
        t << LF(0x91, 0x42, 0xFF, 0x13);
        REQUIRE(t.messagesAt(1) == ML { M("sysreset"), M("noteon", 1, 0x42, 0x13) });

        // aftertouch
        t << LF(0xA2, 0x40, 0x15);
        REQUIRE(t.messagesAt(1) == ML { M("polytouch", 2, 0x40, 0x15) });

        // song select
        t << LF(0xF3, 5);
        REQUIRE(t.messagesAt(1) == ML { M("songsel", 5) });
    }

    SECTION("output")
    {
        TExt t0("proto.midi");
        TExt t1("proto.midi");

        t0.connectTo(0, t1, 0);
        t0.call("note", LF(0, 60, 127));
        REQUIRE(t1.messagesAt(1) == ML { M("noteon", 0, 60, 127) });
        t1.clearAll();

        // errors
        t0.call("note", LF(60, 128));
        REQUIRE(t1.messagesAt(1) == ML {});
        t0.call("note", LF(60, -1));
        REQUIRE(t1.messagesAt(1) == ML {});
        t0.call("note", LF(-1, 77));
        REQUIRE(t1.messagesAt(1) == ML {});
        t0.call("note", LF(128, 77));
        REQUIRE(t1.messagesAt(1) == ML {});
        t0.call("note", LA(60, "A"));
        REQUIRE(t1.messagesAt(1) == ML {});
        t0.call("note", LA("A", 60));
        REQUIRE(t1.messagesAt(1) == ML {});
        t0.call("note", LA("A", "B"));
        REQUIRE(t1.messagesAt(1) == ML {});
        t0.call("note", LA(60, 61, "C"));
        REQUIRE(t1.messagesAt(1) == ML {});
        t0.call("note", LA(60, 61, -1));
        REQUIRE(t1.messagesAt(1) == ML {});
        t0.call("note", LA(60, 61, 16));
        REQUIRE(t1.messagesAt(1) == ML {});
        t0.call("note");
        REQUIRE(t1.messagesAt(1) == ML {});
        t0.call("note", LA(60, 61, 15, 16));
        REQUIRE(t1.messagesAt(1) == ML {});

        // ok
        t0.call("note", LF(3, 60, 61));
        REQUIRE(t1.messagesAt(1) == ML { M("noteon", 3, 60, 61) });
        t1.clearAll();

        t0.call("noteon", LF(0, 127, 64));
        REQUIRE(t1.messagesAt(1) == ML { M("noteon", 0, 127, 64) });
        t1.clearAll();

        t0.call("noteoff", LF(0, 60, 0));
        REQUIRE(t1.messagesAt(1) == ML { M("noteoff", 0, 60, 0) });
        t1.clearAll();

        t0.call("aftertouch", LF(0, 60));
        REQUIRE(t1.messagesAt(1) == ML { M("aftertouch", 0, 60) });
        t1.clearAll();

        t0.call("polytouch", LF(0, 60, 43));
        REQUIRE(t1.messagesAt(1) == ML { M("polytouch", 0, 60, 43) });
        t1.clearAll();

        t0.call("cc", LF(0, 79, 18));
        REQUIRE(t1.messagesAt(1) == ML { M("cc", 0, 79, 18) });
        t1.clearAll();

        t0.call("program", LF(1, 79));
        REQUIRE(t1.messagesAt(1) == ML { M("program", 1, 79) });
        t1.clearAll();

        t0.call("pitchwheel", LF(1, 0));
        REQUIRE(t1.messagesAt(1) == ML { M("pitchwheel", 1, 0) });
        t1.clearAll();

        t0.call("pitchwheel", LF(1, -0x2000));
        REQUIRE(t1.messagesAt(1) == ML { M("pitchwheel", 1, -0x2000) });
        t1.clearAll();

        t0.call("pitchwheel", LF(1, 0x1fff));
        REQUIRE(t1.messagesAt(1) == ML { M("pitchwheel", 1, 0x1fff) });
        t1.clearAll();

        t0.call("clock");
        REQUIRE(t1.messagesAt(1) == ML { M("clock") });
        t1.clearAll();

        t0.call("continue");
        REQUIRE(t1.messagesAt(1) == ML { M("continue") });
        t1.clearAll();

        t0.call("start");
        REQUIRE(t1.messagesAt(1) == ML { M("start") });
        t1.clearAll();

        t0.call("stop");
        REQUIRE(t1.messagesAt(1) == ML { M("stop") });
        t1.clearAll();

        t0.call("activesense");
        REQUIRE(t1.messagesAt(1) == ML { M("activesense") });
        t1.clearAll();

        t0.call("sysreset");
        REQUIRE(t1.messagesAt(1) == ML { M("sysreset") });
        t1.clearAll();

        t0.call("tunerequest");
        REQUIRE(t1.messagesAt(1) == ML { M("tunerequest") });
        t1.clearAll();

        t0.call("songsel", 16);
        REQUIRE(t1.messagesAt(1) == ML { M("songsel", 16) });
        t1.clearAll();

        t0.call("tick");
        REQUIRE(t1.messagesAt(1) == ML { M("tick") });
        t1.clearAll();

        t0.call("songpos");
        REQUIRE(t1.messagesAt(1) == ML {});
        t1.clearAll();

        t0.call("songpos", 255);
        REQUIRE(t1.messagesAt(1) == ML { M("songpos", 255) });
        t1.clearAll();

        t0.call("timecode", LA("19sec", 30));
        REQUIRE(t1.messagesAt(1) == ML { M("timecode", 0, 0, 19, 0, 30) });
        t1.clearAll();

        t0.call("timecode", LA("1.1min", 24));
        REQUIRE(t1.messagesAt(1) == ML { M("timecode", 0, 1, 6, 0, 24) });
        t1.clearAll();

        t0.call("timecode", LA("610sec", 25));
        REQUIRE(t1.messagesAt(1) == ML { M("timecode", 0, 10, 10, 0, 25) });
        t1.clearAll();

        t0.call("timecode", LA("1.1h", 29.97));
        REQUIRE(t1.messagesAt(1) == ML { M("timecode", 1, 6, 0, 0, 29.97) });
        t1.clearAll();

        t0.call("timecode", LA("15h", 24));
        REQUIRE(t1.messagesAt(1) == ML { M("timecode", 15, 0, 0, 0, 24) });
        t1.clearAll();

        t0.call("timecode", LA("17.1h", 24));
        REQUIRE(t1.messagesAt(1) == ML { M("timecode", 17, 6, 0, 0, 24) });
        t1.clearAll();

        t0.call("timecode", LA("0.5sec", 24));
        REQUIRE(t1.messagesAt(1) == ML { M("timecode", 0, 0, 0, 12, 24) });
        t1.clearAll();

        t0.call("timecode", LA("10.25sec", 24));
        REQUIRE(t1.messagesAt(1) == ML { M("timecode", 0, 0, 10, 6, 24) });
        t1.clearAll();

        t0.call("timecode", LA("01:23:45.12", 24));
        REQUIRE(t1.messagesAt(1) == ML { M("timecode", 1, 23, 45, 12, 24) });
        t1.clearAll();

        t0.call("sysex", LF(1, 2, 3));
        REQUIRE(t1.messagesAt(1) == ML { M("sysex", 240, 1, 2, 3, 247) });
        t1.clearAll();
    }

    SECTION("raw")
    {
        TExt t("proto.midi");
        t.call("raw", 1, 2, 3);
        REQUIRE(t.messagesAt(0).size() == 3);
        REQUIRE(t.messagesAt(0).at(0) == 1);
        REQUIRE(t.messagesAt(0).at(1) == 2);
        REQUIRE(t.messagesAt(0).at(2) == 3);

        t.call("raw", "0xBE", "0xEF", 100);
        REQUIRE(t.messagesAt(0).size() == 3);
        REQUIRE(t.messagesAt(0).at(0) == 0xbe);
        REQUIRE(t.messagesAt(0).at(1) == 0xef);
        REQUIRE(t.messagesAt(0).at(2) == 100);
    }
}
