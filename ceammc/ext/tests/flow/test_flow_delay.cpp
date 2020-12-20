/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "datatype_mlist.h"
#include "flow_delay.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowDelay, flow, delay)

TEST_CASE("flow.delay", "[externals]")
{
    pd_test_init();
    setTestSampleRate(44100);

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("flow.delay");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @delay, 0);
            REQUIRE_PROPERTY(t, @block, 0);
        }

        SECTION("args")
        {
            TObj t("flow.delay", LA(100));
            REQUIRE_PROPERTY(t, @delay, 100);
        }

        SECTION("props")
        {
            TObj t("flow.delay", LA("@delay", 200));
            REQUIRE_PROPERTY(t, @delay, 200);
        }
    }

    SECTION("delay time")
    {
        TExt t("flow.delay", 5_ticks);
        REQUIRE_PROPERTY_FLOAT(t, @delay, 5_ticks);

        t.sendFloatTo(10_ticks, 1);
        REQUIRE_PROPERTY_FLOAT(t, @delay, 10_ticks);
    }

    SECTION("do")
    {
        TExt t("flow.delay", 5_ticks);

        const Message bang = Message::makeBang();

        // bang
        t.bang();
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(6);
        REQUIRE(t.messagesAt(0) == MessageList({ bang }));
        t.clearAll();

        // float
        const Atom f0(100.5);
        t << f0;
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(6);
        REQUIRE(t.messagesAt(0) == MessageList({ f0 }));
        t.clearAll();

        // symbol
        const Atom s0(S("a symbol"));
        t << s0;
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(6);
        REQUIRE(t.messagesAt(0) == MessageList({ s0 }));
        t.clearAll();

        const IntA i0(1000);
        t << i0;
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(6);
        REQUIRE(t.messagesAt(0) == MessageList({ i0 }));
        t.clearAll();

        // list
        const AtomList l0 = LA(1, 2, 3, "A", "B", "C");
        t << l0;
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(6);
        REQUIRE(t.messagesAt(0) == MessageList({ l0 }));
        t.clearAll();

        // any
        const Message m0(gensym("any"), LF(1, 2, 3));
        t.sendMessage(m0);
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(6);
        REQUIRE(t.messagesAt(0) == MessageList { m0 });
        t.clearAll();

        // any props
        const Message m1(gensym("@delay"), LF(1000));
        REQUIRE_PROPERTY_LIST(t, @delay, LX(5_ticks));
        t.sendMessage(m1);
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(6);
        REQUIRE(t.messagesAt(0) == MessageList { m1 });
        t.clearAll();

        // reset
        t.sendMessage(m1);
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(3);
        REQUIRE(t.messagesAt(0).empty());
        t.sendMessageTo(Message(SYM("reset"), L()), 1);
        t.schedTicks(10);
        REQUIRE(t.messagesAt(0).empty());

        t.sendMessageTo(Message("error message to console:", 1, 2, 3), 1);
    }

    SECTION("@block")
    {
        TExt t("flow.delay", 5_ticks, "@block", 1);

        const Message bang = Message::makeBang();

        // bang
        t.bang();
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(2);
        REQUIRE(t.messagesAt(0).empty());
        t.sendFloat(1000);
        t.schedTicks(4);
        REQUIRE(t.messagesAt(0) == MessageList({ bang }));
        t.clearAll();

        // float
        const Atom f0(100.5);
        t << f0;
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(2);
        REQUIRE(t.messagesAt(0).empty());
        t.sendBang();
        t.schedTicks(4);
        REQUIRE(t.messagesAt(0) == MessageList({ f0 }));
        t.clearAll();

        // symbol
        const Atom s0(gensym("ABC"));
        t << s0;
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(2);
        REQUIRE(t.messagesAt(0).empty());
        t.sendBang();
        t.schedTicks(4);
        REQUIRE(t.messagesAt(0) == MessageList({ s0 }));
        t.clearAll();

        // list
        const AtomList l0 = LF(1, 2, 3);
        t << l0;
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(2);
        REQUIRE(t.messagesAt(0).empty());
        t.sendSymbol("DEF");
        t.schedTicks(4);
        REQUIRE(t.messagesAt(0) == MessageList({ l0 }));
        t.clearAll();

        // any
        const Message m0(gensym("any"), LF(1, 2, 3));
        t.sendMessage(m0);
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(4);
        REQUIRE(t.messagesAt(0).empty());
        t.sendFloat(200);
        t.schedTicks(4);
        REQUIRE(t.messagesAt(0) == MessageList { m0 });
        t.clearAll();

        // reset
        t.sendMessage(m0);
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(4);
        REQUIRE(t.messagesAt(0).empty());
        t.sendMessageTo(Message(SYM("reset"), L()), 1);
        t.sendMessage(m0);
        REQUIRE(t.messagesAt(0).empty());
        t.schedTicks(6);
        REQUIRE(t.messagesAt(0) == MessageList { m0 });
        t.clearAll();
    }
}
