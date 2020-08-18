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
#include "flow_stack.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowStack, flow, stack)

TEST_CASE("flow.stack", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("flow.stack");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @max_size, 100);
            REQUIRE_PROPERTY(t, @size, 0);
            REQUIRE_PROPERTY(t, @empty, 1);
        }

        SECTION("args")
        {
            TObj t("flow.stack", LA(22));
            REQUIRE_PROPERTY(t, @max_size, 22);
            REQUIRE_PROPERTY(t, @size, 0);
            REQUIRE_PROPERTY(t, @empty, 1);
        }

        SECTION("args")
        {
            TObj t("flow.stack", LA("@max_size", 10));
            REQUIRE_PROPERTY(t, @max_size, 10);
            REQUIRE_PROPERTY(t, @size, 0);
            REQUIRE_PROPERTY(t, @empty, 1);
        }
    }

    SECTION("do")
    {
        TExt t("flow.stack", 6);

        const Message bang = Message::makeBang();
        const Message f0(11);
        const Message s0(SYM("ABC"));
        const AtomList l0 = LF(1, 2, 3);
        const Message m0(SYM("@test"), LF(3, 2, 1));

        t.bang();
        REQUIRE(t.messagesAt(0).empty());
        REQUIRE_PROPERTY(t, @empty, 0);
        REQUIRE_PROPERTY(t, @size, 1);
        t.sendMessageTo(Message(SYM("top"), L()), 1);
        REQUIRE(t.messagesAt(0) == MessageList({ bang }));

        t << 11;
        REQUIRE(t.messagesAt(0).empty());
        REQUIRE_PROPERTY(t, @size, 2);
        t.sendMessageTo(Message(SYM("top"), L()), 1);
        REQUIRE(t.messagesAt(0) == MessageList({ f0 }));

        t << "ABC";
        REQUIRE(t.messagesAt(0).empty());
        REQUIRE_PROPERTY(t, @size, 3);
        t.sendMessageTo(Message(SYM("top"), L()), 1);
        REQUIRE(t.messagesAt(0) == MessageList({ s0 }));

        t << l0;
        REQUIRE(t.messagesAt(0).empty());
        REQUIRE_PROPERTY(t, @size, 4);
        t.sendMessageTo(Message(SYM("top"), L()), 1);
        REQUIRE(t.messagesAt(0) == MessageList({ l0 }));

        t.clearAll();
        t.sendMessage(m0);
        REQUIRE(t.messagesAt(0).empty());
        REQUIRE_PROPERTY(t, @size, 5);
        t.sendMessageTo(Message(SYM("top"), L()), 1);
        REQUIRE(t.messagesAt(0) == MessageList({ m0 }));

        t << IntA(20);
        REQUIRE(t.messagesAt(0).empty());
        REQUIRE_PROPERTY(t, @size, 6);
        t.sendMessageTo(Message(SYM("top"), L()), 1);
        REQUIRE(t.messagesAt(0) == MessageList({ Message(IntA(20)) }));

        t.bang();
        REQUIRE(t.messagesAt(0).empty());
        REQUIRE_PROPERTY(t, @size, 6);
        t << 12;
        REQUIRE(t.messagesAt(0).empty());
        REQUIRE_PROPERTY(t, @size, 6);
        t << "CDE";
        REQUIRE(t.messagesAt(0).empty());
        REQUIRE_PROPERTY(t, @size, 6);
        t << LF(1, 2, 3);
        REQUIRE(t.messagesAt(0).empty());
        REQUIRE_PROPERTY(t, @size, 6);

        // do nothing
        t.call("pop");
        t.call("clear");
        t.call("flush");
        t.call("dump");
        REQUIRE_PROPERTY(t, @size, 6);

        // pop
        t.sendMessageTo(Message(SYM("pop"), L()), 1);
        REQUIRE(t.messagesAt(0).empty());
        REQUIRE_PROPERTY(t, @size, 5);

        // poptop
        t.sendBangTo(1);
        REQUIRE(t.messagesAt(0) == MessageList({ m0 }));
        REQUIRE_PROPERTY(t, @size, 4);

        // clear
        t.clearAll();
        t.sendMessageTo(Message(SYM("clear"), L()), 1);
        REQUIRE(t.messagesAt(0).empty());
        REQUIRE_PROPERTY(t, @size, 0);

        // more clear
        t.sendMessageTo(Message(SYM("clear"), L()), 1);
        t.sendMessageTo(Message(SYM("pop"), L()), 1);
        t.sendMessageTo(Message(SYM("top"), L()), 1);
        t.sendMessageTo(Message(SYM("flush"), L()), 1);

        // flush
        t.sendFloat(10);
        t.sendFloat(20);
        t.sendFloat(30);
        REQUIRE_PROPERTY(t, @size, 3);
        t.sendMessageTo(Message(SYM("flush"), L()), 1);
        REQUIRE(t.messagesAt(0).at(0).atomValue().asFloat() == 30);
        REQUIRE(t.messagesAt(0).at(1).atomValue().asFloat() == 20);
        REQUIRE(t.messagesAt(0).at(2).atomValue().asFloat() == 10);
        REQUIRE_PROPERTY(t, @size, 0);
    }

    SECTION("@on_empty")
    {
        TExt t("flow.stack", 2, "@on_empty", "e");
        ListenerExternal on_empty("e");

        t.bang();
        REQUIRE(on_empty.msg().isNone());
        t.bang();
        REQUIRE(on_empty.msg().isNone());

        t->m_pop({});
        REQUIRE(on_empty.msg().isNone());
        t->m_pop({});
        REQUIRE(on_empty.msg().isBang());
        on_empty.reset();

        t << 10.5;
        t->m_clear({});
        REQUIRE(on_empty.msg().isBang());
        on_empty.reset();

        t << 100;
        t->m_flush({});
        REQUIRE(on_empty.msg().isBang());
        on_empty.reset();

        t->m_flush({});
        REQUIRE(on_empty.msg().isNone());
        on_empty.reset();

        t << "ABC";
        t.sendBangTo(1);
        REQUIRE(on_empty.msg().isBang());
    }

    SECTION("@on_full")
    {
        TExt t("flow.stack", 2, "@on_full", "f");
        ListenerExternal on_full("f");

        t.bang();
        REQUIRE(on_full.msg().isNone());

        t.bang();
        REQUIRE(on_full.msg().isBang());
        on_full.reset();
        t.bang();
        REQUIRE(on_full.msg().isNone());
        t->m_pop({});

        t << 10.5;
        REQUIRE(on_full.msg().isBang());
        on_full.reset();
        t->m_pop({});

        t << "ABC";
        REQUIRE(on_full.msg().isBang());
        on_full.reset();
        t->m_pop({});

        t << LF(1, 2, 3);
        REQUIRE(on_full.msg().isBang());
        on_full.reset();
        t->m_pop({});

        t <<= LA("msg", 3);
        REQUIRE(on_full.msg().isBang());
        on_full.reset();
        t->m_pop({});
    }
}
