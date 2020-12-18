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
#include "flow_pipe.h"
#include "test_flow_base.h"
#include "test_seq_base.h"

PD_COMPLETE_TEST_SETUP(FlowPipe, flow, pipe)

TEST_CASE("flow.pipe", "[externals]")
{
    pd_test_init();
    setTestSampleRate(64000);

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("flow.pipe");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @delay, 0.);
            REQUIRE_PROPERTY(t, @size, 0.);
            REQUIRE_PROPERTY(t, @empty, 1);
        }

        SECTION("arg")
        {
            TObj t("flow.pipe", LF(2));
            REQUIRE_PROPERTY(t, @delay, 2);
        }
    }

    SECTION("run")
    {
        TExt t("flow.pipe", "@delay", 5);

        const Message bang = Message::makeBang();

        t.bang();
        REQUIRE_PROPERTY(t, @size, 1);
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(5_wd);
        REQUIRE(t.messagesAt(0) == MessageList({ bang }));
        REQUIRE_PROPERTY(t, @size, 0);
        t.clearAll();

        // consecutive bangs
        t.bang();
        t.bang();
        REQUIRE_PROPERTY(t, @size, 2);
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(5_wd);
        REQUIRE(t.messagesAt(0) == MessageList({ bang, bang }));
        REQUIRE_PROPERTY(t, @size, 0);
        t.clearAll();

        // delayed bangs
        t.bang();
        t.schedTicks(3);
        t.bang();
        REQUIRE_PROPERTY(t, @size, 2);
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(4);
        REQUIRE(t.messagesAt(0) == MessageList({ bang }));
        REQUIRE_PROPERTY(t, @size, 1);
        t.schedTicks(4);
        REQUIRE(t.messagesAt(0) == MessageList({ bang, bang }));
        REQUIRE_PROPERTY(t, @size, 0);
        t.clearAll();

        const Atom f0(100.5);
        const Atom s0(SYM("ABC"));
        const IntA i0(1000);
        const AtomList l0(1, 2, 3);
        const Message m0(SYM("@msg"), LF(1));

        t << f0;
        REQUIRE_PROPERTY(t, @size, 1);
        t.schedTicks(1);
        t << s0;
        REQUIRE_PROPERTY(t, @size, 2);
        t.schedTicks(1);
        t << i0;
        REQUIRE_PROPERTY(t, @size, 3);
        t.schedTicks(1);
        t << l0;
        REQUIRE_PROPERTY(t, @size, 4);
        t.schedTicks(1);
        t.sendMessage(m0);
        REQUIRE_PROPERTY(t, @size, 5);
        t.schedTicks(1_wd);
        REQUIRE_PROPERTY(t, @size, 4);
        REQUIRE(t.messagesAt(0) == MessageList({ f0 }));
        t.schedTicks(1);
        REQUIRE_PROPERTY(t, @size, 3);
        REQUIRE(t.messagesAt(0) == MessageList({ f0, s0 }));
        t.schedTicks(1);
        REQUIRE_PROPERTY(t, @size, 2);
        REQUIRE(t.messagesAt(0) == MessageList({ f0, s0, i0 }));
        t.schedTicks(1);
        REQUIRE_PROPERTY(t, @size, 1);
        REQUIRE(t.messagesAt(0) == MessageList({ f0, s0, i0, l0 }));
        t.schedTicks(1);
        REQUIRE_PROPERTY(t, @size, 0);
        REQUIRE(t.messagesAt(0) == MessageList({ f0, s0, i0, l0, m0 }));
    }

    SECTION("second inlet")
    {
        SECTION("delay")
        {
            TExt t("flow.pipe", "@delay", 5);
            REQUIRE_PROPERTY(t, @delay, 5);
            t.sendFloatTo(10, 1);
            REQUIRE_PROPERTY(t, @delay, 10);
            t.sendFloatTo(1.5, 1);
            REQUIRE_PROPERTY(t, @delay, 1.5);
            t.sendFloatTo(0, 1);
            REQUIRE_PROPERTY(t, @delay, 0);
            t.sendFloatTo(-1, 1);
            REQUIRE_PROPERTY(t, @delay, 0);
        }

        SECTION("bang")
        {
            TExt t("flow.pipe", "@delay", 10);
            t.bang();
            t.bang();
            REQUIRE_PROPERTY(t, @size, 2);
            t.sendBangTo(1);
            REQUIRE_PROPERTY(t, @size, 0.);
            REQUIRE(t.messagesAt(0).empty());
        }

        SECTION("reset")
        {
            TExt t("flow.pipe", "@delay", 10);
            t.bang();
            t.bang();
            REQUIRE_PROPERTY(t, @size, 2);
            t.sendMessageTo(Message(SYM("reset"), L()), 1);
            REQUIRE_PROPERTY(t, @size, 0.);
            REQUIRE(t.messagesAt(0).empty());
        }

        SECTION("flush")
        {
            const Message bang = Message::makeBang();

            TExt t("flow.pipe", "@delay", 10);
            t << 100;
            t << 200;
            t << 300;
            REQUIRE_PROPERTY(t, @size, 3);
            t.sendMessageTo(Message(SYM("flush"), L()), 1);
            REQUIRE_PROPERTY(t, @size, 3);
            REQUIRE(t.messagesAt(0) == MessageList({ Message(100), Message(200), Message(300) }));
            t.schedTicks(1);
            REQUIRE_PROPERTY(t, @size, 0.);
        }
    }
}
