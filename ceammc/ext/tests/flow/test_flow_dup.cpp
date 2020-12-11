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
#include "flow_dup.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowDup, flow, dup)

TEST_CASE("flow.dup", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("flow.dup");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @n, 2);
        }

        SECTION("args")
        {
            TObj t("flow.dup", LF(3));
            REQUIRE_PROPERTY(t, @n, 3);
        }

        SECTION("props")
        {
            TObj t("flow.dup", LA("@n", 1));
            REQUIRE_PROPERTY(t, @n, 1);
        }
    }

    SECTION("do")
    {
        const auto bang = Message::makeBang();
        const auto f0 = Message(10);
        const auto s0 = Message(SYM("ABC"));
        const auto l0 = Message(LF(1, 2, 3));
        const auto m0 = Message(SYM("@p"), LF(1, 2, 3));

        TExt t("flow.dup");

        t.bang();
        REQUIRE(t.messagesAt(0) == MessageList { bang, bang });
        t << 10;
        REQUIRE(t.messagesAt(0) == MessageList { f0, f0 });
        t << "ABC";
        REQUIRE(t.messagesAt(0) == MessageList { s0, s0 });
        t << LF(1, 2, 3);
        REQUIRE(t.messagesAt(0) == MessageList { l0, l0 });

        t.clearAll();
        t.sendMessage(m0);
        REQUIRE(t.messagesAt(0) == MessageList { m0, m0 });
    }

    SECTION("qwe")
    {
        TExt t("flow.dup", 4);
        REQUIRE_PROPERTY(t, @n, 4);

        t.sendFloatTo(2, 1);
        REQUIRE_PROPERTY(t, @n, 2);

        t.sendFloatTo(0, 1);
        REQUIRE_PROPERTY(t, @n, 0.f);

        t.sendFloatTo(-1, 1);
        REQUIRE_PROPERTY(t, @n, 0.f);

        t.sendFloatTo(1.1, 1);
        REQUIRE_PROPERTY(t, @n, 1);
    }
}
