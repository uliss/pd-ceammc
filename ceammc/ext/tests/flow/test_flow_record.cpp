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
            REQUIRE_PROPERTY_FLOAT(t, @speed, 1);
            REQUIRE_PROPERTY_FLOAT(t, @max, 256);
            // clang-format off
            REQUIRE_PROPERTY_FLOAT(t, @auto, 0);
            // clang-format on
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

    SECTION("quant")
    {
        TExt t("flow.record");

        t.sendMessageTo(Message(SYM("rec"), AtomListView()), 1);

        const auto rec_start = t->recStartMs();

        t.schedTicks(4); // 4
        t << 1;
        t.schedTicks(4); // 8
        t << 2;
        t.schedTicks(4); // 12
        t << 3;
        t.schedTicks(4); // 16
        t << 4;
        t.schedTicks(4); // 20
        t << 5;
        t.schedTicks(4); // 24
        t << 6;

        t.sendMessageTo(Message(SYM("stop"), AtomListView()), 1);

#define REQUIRE_TICKS(t, n, v)                                                \
    {                                                                         \
        REQUIRE(t->events()[n].t_ms == Approx(v + rec_start).margin(0.0001)); \
    }

        REQUIRE_TICKS(t, 0, 4);
        REQUIRE_TICKS(t, 1, 8);
        REQUIRE_TICKS(t, 2, 12);
        REQUIRE_TICKS(t, 3, 16);
        REQUIRE_TICKS(t, 4, 20);
        REQUIRE_TICKS(t, 5, 24);

        t.sendMessageTo(Message(SYM("quant"), AtomListView(5)), 1);

        REQUIRE_TICKS(t, 0, 5);
        REQUIRE_TICKS(t, 1, 10);
        REQUIRE_TICKS(t, 2, 10);
        REQUIRE_TICKS(t, 3, 15);
        REQUIRE_TICKS(t, 4, 20);
        REQUIRE_TICKS(t, 5, 25);
    }

    SECTION("qlist")
    {
        TExt t("flow.record");

        t.sendMessageTo(Message(SYM("rec"), AtomListView()), 1);

        t.schedTicks(5); // 4
        t << 1;
        t.schedTicks(6); // 8
        t << 2;
        t.schedTicks(7); // 12
        t << 3;

        t.sendMessageTo(Message(SYM("qlist"), AtomListView()), 1);
        REQUIRE(t.messagesAt(0) == ML {
                    M(SYM("add"), LF(5, 1)),
                    M(SYM("add"), LF(6, 2)),
                    M(SYM("add"), LF(7, 3)),
                });
    }

    SECTION("sync")
    {
        TExt t("flow.record", "@sync", 1);

        t.schedTicks(100);
        t.sendBangTo(1);
        t.schedTicks(100);
        t.sendBangTo(1);

        t.schedTicks(20);
        t.sendMessageTo(Message(SYM("rec"), AtomListView()), 1);
        auto rs = t->recStartMs();

        t.schedTicks(20);
        t << "A";
        t.schedTicks(60);
        t.sendBangTo(1);

        t.schedTicks(70);
        t << "B";
        t.schedTicks(20);
        t.sendMessageTo(Message(SYM("stop"), AtomListView()), 1);
        t.schedTicks(10);
        t.sendBangTo(1);
        t.schedTicks(100);
        t.sendBangTo(1);

        t->dump();

        REQUIRE(t->recLengthMs() == Approx(200).margin(0.00001));
    }

    SECTION("@max")
    {
        TExt t("flow.record", "@max", 4);

        t.sendMessageTo(Message(SYM("rec"), AtomListView()), 1);
        t << 1;
        t << 2;
        t << 3;
        t << 4;

        REQUIRE(t->events().size() == 4);

        t << 5;
        REQUIRE(t->events().size() == 4);
    }

    SECTION("any")
    {
        TExt t("flow.record", "@max", 4);

        t.sendMessageTo(Message(SYM("rec"), AtomListView()), 1);
        t.sendMessage(SYM("@any"), LF(1, 2, 3));
        REQUIRE(t->events().size() == 1);
        REQUIRE(t->events()[0].msg->view() == LA("@any", 1, 2, 3));
    }
}
