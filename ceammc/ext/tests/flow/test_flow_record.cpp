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
#include "flow_record.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowRecord, flow, record)

using ML = std::vector<Message>;
using M = Message;

TEST_CASE("flow.record", "[externals]")
{
    pd_test_init();
    setTestSampleRate(64000);
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("flow.record");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @r, 1);
        }
    }

    SECTION("simple")
    {
        TExt t("flow.record");
        t.schedTicks(10);

        const Message bang = Message::makeBang();

        // bang
        t.bang();
        REQUIRE_FALSE(t.hasOutput());

        t.sendMessageTo(Message(SYM("rec"), AtomListView()), 1);

        t << 1;
        t << 2;

        t.sendMessageTo(Message(SYM("stop"), AtomListView()), 1);

        t->dump();
        t.schedTicks(100);
        REQUIRE_FALSE(t.hasOutput());

        t.sendMessageTo(Message(SYM("play"), AtomListView()), 1);
        t.schedTicks(1);

        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.messagesAt(0) == ML { M(1), M(2) });

        t.clearAll();
        t.schedTicks(1);
        REQUIRE_FALSE(t.hasOutput());

        t.sendMessageTo(Message(SYM("play"), AtomListView()), 1);
        t.schedTicks(1);
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.messagesAt(0) == ML { M(1), M(2) });
    }

    SECTION("many")
    {
        TExt t("flow.record");

        t.sendMessageTo(Message(SYM("rec"), AtomListView()), 1);

        t.schedTicks(10_ticks);
        t << 5;
        t.schedTicks(20_ticks);
        t << 10;

        t.sendMessageTo(Message(SYM("stop"), AtomListView()), 1);
        t->dump();
        t.clearAll();

        t.sendMessageTo(Message(SYM("play"), AtomListView()), 1);
        REQUIRE_FALSE(t.hasOutput());

        t.schedTicks(5_ticks); // no output yet
        REQUIRE_FALSE(t.hasOutput());

        t.schedTicks(6_ticks); // first output: 5
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.messagesAt(0) == ML { M(5) });

        t.schedTicks(20); // next output: 10
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.messagesAt(0) == ML { M(5), M(10) });
        t.clearAll();

        // single play: no output
        t.schedTicks(10);
        REQUIRE_FALSE(t.hasOutput());

        // play again
        t.sendMessageTo(Message(SYM("play"), AtomListView(1)), 1);
        REQUIRE_FALSE(t.hasOutput());

        t.schedTicks(11_ticks); // first output: 5
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.messagesAt(0) == ML { M(5) });
    }

    SECTION("loop")
    {
        TExt t("flow.record", "@r", 2);

        t.sendMessageTo(Message(SYM("rec"), AtomListView()), 1);

        t.schedTicks(10_ticks);
        t << 5;
        t.schedTicks(20_ticks);
        t << 10;

        t.sendMessageTo(Message(SYM("stop"), AtomListView()), 1);
        t->dump();
        t.clearAll();

        t.schedTicks(5_ticks);
        t.sendMessageTo(Message(SYM("play"), AtomListView()), 1);
        REQUIRE_FALSE(t.hasOutput());

        // [10] 5 [20] 10 [15] 5 [20] 10

        t.schedTicks(11_ticks); // output: 5
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.messagesAt(0) == ML { M(5) });

        t.schedTicks(20); // next output: 10
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.messagesAt(0) == ML { M(5), M(10) });
        t.clearAll();

        t.schedTicks(15_ticks); // output: 5
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.messagesAt(0) == ML { M(5) });

        t.schedTicks(20); // next output: 10
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.messagesAt(0) == ML { M(5), M(10) });
        t.clearAll();

        t.schedTicks(100);
        REQUIRE_FALSE(t.hasOutput());
    }
}
