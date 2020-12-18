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
#include "seq_arp.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(SeqArp, seq, arp)

TEST_CASE("seq.arp", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("seq.arp");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
        }

        SECTION("arg")
        {
            TExt t("seq.arp", LF(2, 3));
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @chord, LF(2, 3));
        }
    }

    SECTION("process")
    {
        SECTION("empty")
        {
            TExt t("seq.arp");
            t.call("on");
            REQUIRE(!t.hasOutput());
        }

        SECTION("single")
        {
            TExt t("seq.arp", LF(60));

            t.call("on");
            REQUIRE(t.outputListAt(0) == LF(60, 127));
            REQUIRE(t.outputFloatAt(1) == 1);

            t.call("on");
            REQUIRE(!t.hasOutput());

            t.call("reset");
            REQUIRE(!t.hasOutput());

            t.call("on", LA("f", 64.5));
            REQUIRE(t.outputListAt(0) == LF(60, 64.5));
            REQUIRE(t.outputFloatAt(1) == 1);

            t.call("reset");
            t.call("on", LA("f", 34.5, 10));
            REQUIRE(t.outputListAt(0) == LF(60, 34.5));
            REQUIRE(t.outputFloatAt(1) == 1);

            t.call("reset");
            t.call("on", LA("f", 34.5, -1));
            REQUIRE(!t.hasOutput());

            t.call("on", LA("f", 12));
            REQUIRE(t.outputFloatAt(1) == 1);
            t.call("off", LA("first"));
            REQUIRE(t.outputFloatAt(1) == 0);
        }

        SECTION("first")
        {
            TExt t("seq.arp", LF(60, 70, 75));

            t.call("on");
            REQUIRE(t.outputListAt(0) == LF(60, 127));

            t.call("on");
            REQUIRE(t.outputListAt(0) == LF(70, 127));

            t.call("on");
            REQUIRE(t.outputListAt(0) == LF(75, 127));

            t.call("on");
            REQUIRE(!t.hasOutput());

            t.call("reset");
            REQUIRE(!t.hasOutput());

            t.call("on", LA("first", 64, 2));
            REQUIRE(t.messagesAt(0)[0] == Message(LF(60, 64)));
            REQUIRE(t.messagesAt(0)[1] == Message(LF(70, 64)));
        }

        SECTION("last")
        {
            TExt t("seq.arp", LF(60, 70, 75));

            t.call("on", LA("l"));
            REQUIRE(t.outputListAt(0) == LF(75, 127));

            t.call("on", LA("l"));
            REQUIRE(t.outputListAt(0) == LF(70, 127));

            t.call("on", LA("last"));
            REQUIRE(t.outputListAt(0) == LF(60, 127));

            t.call("on", LA("last"));
            REQUIRE(!t.hasOutput());

            t.call("reset");
            REQUIRE(!t.hasOutput());

            t.call("on", LA("last", 64, 2));
            REQUIRE(t.messagesAt(0)[0] == Message(LF(75, 64)));
            REQUIRE(t.messagesAt(0)[1] == Message(LF(70, 64)));

            t.call("off", LA("last"));
            REQUIRE(t.outputListAt(0) == LF(75, 0));
            t.call("off", LA("last"));
            REQUIRE(t.outputListAt(0) == LF(70, 0));

            t.call("on", LA("all", 11));
            REQUIRE(t.messagesAt(0).size() == 3);
            REQUIRE(t.messagesAt(0)[0] == Message(LF(60, 11)));
            REQUIRE(t.messagesAt(0)[1] == Message(LF(70, 11)));
            REQUIRE(t.messagesAt(0)[2] == Message(LF(75, 11)));
            REQUIRE(t.messagesAt(1)[0] == Message(1));
            REQUIRE(t.messagesAt(1)[1] == Message(2));
            REQUIRE(t.messagesAt(1)[2] == Message(3));

            t.call("off", LA("last", 2));
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0] == Message(LF(75, 0)));
            REQUIRE(t.messagesAt(0)[1] == Message(LF(70, 0)));

            t.call("off", LA("last", 2));
            REQUIRE(t.messagesAt(0).size() == 1);
            REQUIRE(t.messagesAt(0)[0] == Message(LF(60, 0)));
        }

        SECTION("all")
        {
            TExt t("seq.arp", LF(60, 70, 75));

            t.call("on", LA("all", 11));
            REQUIRE(t.messagesAt(0)[0] == Message(LF(60, 11)));
            REQUIRE(t.messagesAt(0)[1] == Message(LF(70, 11)));
            REQUIRE(t.messagesAt(0)[2] == Message(LF(75, 11)));
            REQUIRE(t.messagesAt(1)[0] == Message(1));
            REQUIRE(t.messagesAt(1)[1] == Message(2));
            REQUIRE(t.messagesAt(1)[2] == Message(3));

            t.call("off", LA("all"));
            REQUIRE(t.messagesAt(0)[0] == Message(LF(60, 0)));
            REQUIRE(t.messagesAt(0)[1] == Message(LF(70, 0)));
            REQUIRE(t.messagesAt(0)[2] == Message(LF(75, 0)));
            REQUIRE(t.messagesAt(1)[0] == Message(2));
            REQUIRE(t.messagesAt(1)[1] == Message(1));
            REQUIRE(t.messagesAt(1)[2] == Message(0.));
        }
    }
}
