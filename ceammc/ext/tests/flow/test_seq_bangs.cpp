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
#include "seq_bangs.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(SeqBangs, seq, bangs)

TEST_CASE("seq.bangs", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("seq.bangs");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 3);
            REQUIRE_PROPERTY(t, @bpm, 60);
            REQUIRE_PROPERTY(t, @div, 4);
            REQUIRE_PROPERTY(t, @pattern, L());
        }

        SECTION("args")
        {
            TObj t("seq.bangs", LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @pattern, LF(1, 2, 3));
        }

        SECTION("args invalid")
        {
            TObj t("seq.bangs", LA("A", 2, 3, "[a: 123]"));
            REQUIRE_PROPERTY(t, @pattern, LF(2, 3));
        }

        SECTION("ext")
        {
            TExt t("seq.bangs");
        }

        SECTION("alias")
        {
            TExt t("seq.b");
        }
    }

    SECTION("process")
    {
        SECTION("empty")
        {
            TExt t("seq.b");

            t.sendBang();
            REQUIRE(t.messagesAt(0).empty());
            REQUIRE(t.messagesAt(1).empty());
            REQUIRE(t.messagesAt(2).empty());
        }

        SECTION("single")
        {
            TExt t("seq.b", LA(1, "@bpm", 600, "@div", 16));

            t.sendBang();
            REQUIRE(t.messagesAt(0)[0].isBang());
            REQUIRE(t.messagesAt(1)[0].atomValue().asFloat() == 25);
            REQUIRE(t.messagesAt(2).empty());

            t.schedTicks(25);
            REQUIRE(t.messagesAt(2)[0].isBang());
        }

        SECTION("many")
        {
            TExt t("seq.b", LA(1, 2, 0.5, "@bpm", 600, "@div", 40));

            t.sendBang();
            REQUIRE(t.messagesAt(0)[0].isBang());
            REQUIRE(t.messagesAt(1)[0].atomValue().asFloat() == 10);
            REQUIRE(t.messagesAt(2).empty());

            t.schedTicks(10);

            REQUIRE(t.messagesAt(0)[1].isBang());
            REQUIRE(t.messagesAt(1)[1].atomValue().asFloat() == 20);
            REQUIRE(t.messagesAt(2).empty());

            t.schedTicks(20);

            REQUIRE(t.messagesAt(0)[2].isBang());
            REQUIRE(t.messagesAt(1)[2].atomValue().asFloat() == 5);
            REQUIRE(t.messagesAt(2).empty());

            t.schedTicks(5);
            REQUIRE(t.messagesAt(0).size() == 3);
            REQUIRE(t.messagesAt(1).size() == 3);
            REQUIRE(t.messagesAt(2).size() == 1);
            REQUIRE(t.messagesAt(2)[0].isBang());
        }
    }
}
