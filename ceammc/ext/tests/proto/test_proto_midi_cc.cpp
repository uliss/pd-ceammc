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
#include "proto_midi_cc.h"
#include "test_proto_base.h"

PD_COMPLETE_TEST_SETUP(ProtoMidiCC, proto, midi_cc)

template <class... Args>
static Message M(const char* m, Args... args)
{
    return Message(gensym(m), AtomList(args...));
}

using ML = std::vector<Message>;

TEST_CASE("proto.midi.cc", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("create")
    {
        SECTION("default")
        {
            TExt t("proto.midi.cc");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }
    }

    SECTION("input")
    {
        TExt t("proto.midi.cc", "@list");

        t.call("bendsens", LF(0.0));
        REQUIRE(t.outputListAt(0) == LF(0xB0, 101, 0, 0xB0, 100, 0, 0xB0, 0x6, 0, 0xB0, 0x26, 0));

        t.call("bendsens", LF(1));
        REQUIRE(t.outputListAt(0) == LF(0xB0, 101, 0, 0xB0, 100, 0, 0xB0, 0x6, 1, 0xB0, 0x26, 0));

        t.call("bendsens", LF(2.5));
        REQUIRE(t.outputListAt(0) == LF(0xB0, 101, 0, 0xB0, 100, 0, 0xB0, 0x6, 2, 0xB0, 0x26, 50));
    }

    SECTION("in/out")
    {
        TExt t0("proto.midi.cc", "@list");
        TExt t1("proto.midi.cc");

        t0.connectTo(0, t1, 0);
        t0.call("bendsens");
        t0.call("bendsens", -1.5);
        REQUIRE_FALSE(t1.hasOutput());

        t0.call("bendsens", 0.0);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 0, 0) });
        t1.clearAll();

        t0.call("bendsens", 1);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 0, 0x1 << 7) });
        t1.clearAll();

        t0.call("bendsens", 1.5);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 0, (0x1 << 7) | 50) });
        t1.clearAll();

        t0.call("bendsens", 1.75);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 0, (0x1 << 7) | 75) });
        t1.clearAll();

        t0.call("tunebank", 1);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 4, 1) });
        t1.clearAll();

        t0.call("tunebank", 35);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 4, 35) });
        t1.clearAll();

        t0.call("tuneprog", 35);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 3, 35) });
        t1.clearAll();

        t0.call("tunefine", 0);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 1, 0x2000) });
        t1.clearAll();

        t0.call("tunefine", -50);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 1, 0x1000) });
        t1.clearAll();

        t0.call("tunefine", -100);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 1, 0) });
        t1.clearAll();

        t0.call("tunefine", 100);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 1, 0x3FFF) });
        t1.clearAll();

        t0.call("tunefine", 50);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 1, 0x2FFF) });
        t1.clearAll();

        t0.call("tunefine", -15);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 1, 0x2000 - 1229) });
        t1.clearAll();

        t0.call("tunecoarse", 0);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 2, 64 << 7) });
        t1.clearAll();

        t0.call("tunecoarse", 5);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 2, 69 << 7) });
        t1.clearAll();

        t0.call("tunesemi", 1.5);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 2, 65 << 7), M("rpn", 0, 1, 0x2FFF) });
        t1.clearAll();

        t0.call("tunesemi", -2.5);
        REQUIRE(t1.messagesAt(0) == ML { M("rpn", 0, 2, 62 << 7), M("rpn", 0, 1, 0x1000) });
        t1.clearAll();

        t0.call("tunesemi", -200);
        REQUIRE_FALSE(t1.hasOutput());

        t0.call("tunesemi", -10, 16);
        REQUIRE_FALSE(t1.hasOutput());

        t0.call("pan:i", 2, 0x2000);
        REQUIRE(t1.messagesAt(0) == ML {
                    M("pan~", 2, 0x40),
                    M("pan:i", 2, 0x2000),
                    M("pan:f", 2, 0),
                    M("pan.", 2, 0),
                    M("pan:i", 2, 0x2000),
                    M("pan:f", 2, 0),
                });
        t1.clearAll();

        t0.call("pan:f", 2, 1);
        REQUIRE(t1.messagesAt(0) == ML {
                    M("pan~", 2, 0x7F),
                    M("pan:i", 2, 0x7f << 7),
                    M("pan:f", 2, t0->bit14ToPan(0x7F, 0)),
                    M("pan.", 2, 0x7F),
                    M("pan:i", 2, 0x3fff),
                    M("pan:f", 2, 1),
                });
        t1.clearAll();

        t0.call("pan:f", 2, -1);
        REQUIRE(t1.messagesAt(0) == ML {
                    M("pan~", 2, 0),
                    M("pan:i", 2, 0x7F),
                    M("pan:f", 2, t0->bit14ToPan(0, 0x7F)),
                    M("pan.", 2, 0),
                    M("pan:i", 2, 0),
                    M("pan:f", 2, -1),
                });
        t1.clearAll();

        t0.call("hold", 0, 0);
        REQUIRE(t1.messagesAt(0) == ML { M("hold", 0, 0) });
        t1.clearAll();

        t0.call("hold", 2, 1);
        REQUIRE(t1.messagesAt(0) == ML { M("hold", 2, 1) });
        t1.clearAll();

        t0.call("sostenuto", 3, 1);
        REQUIRE(t1.messagesAt(0) == ML { M("sostenuto", 3, 1) });
        t1.clearAll();
    }

    SECTION("pan")
    {
        TExt t("proto.midi.cc");
        t << LF(0xB1, CC_PAN_POSITION_COARSE, 0x40);
        REQUIRE(t.messagesAt(0) == ML { M("pan~", 1, 0x40), M("pan:i", 1, 0x2000), M("pan:f", 1, 0) });
        t << LF(0xB1, CC_PAN_POSITION_COARSE, 0x0);
        REQUIRE(t.messagesAt(0) == ML { M("pan~", 1, 0x0), M("pan:i", 1, 0), M("pan:f", 1, -1) });
        t << LF(0xB1, CC_PAN_POSITION_COARSE, 0x7F);
        t << LF(0xB1, CC_PAN_POSITION_FINE, 0x7F);
        REQUIRE(t.messagesAt(0) == ML { M("pan.", 1, 0x7F), M("pan:i", 1, 0x3FFF), M("pan:f", 1, 1) });
    }
}
