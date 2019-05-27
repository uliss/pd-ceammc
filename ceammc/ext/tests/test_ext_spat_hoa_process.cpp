/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "../src/spat/hoa_process.h"
#include "../src/spat/mod_spat.h"
#include "catch.hpp"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(HoaProcess, spat, hoa_process);

TEST_CASE("hoa.process~", "[externals]")
{
    pd_test_init([]() { ceammc_spat_setup(); });
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("empty args")
        {
            TestHoaProcess t("hoa.process~");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 0);
        }

        SECTION("no patch")
        {
            TestHoaProcess t("hoa.process~", LA(5));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 0);
        }

        SECTION("invalid patch name")
        {
            TestHoaProcess t("hoa.process~", LA(5, "not-exists"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 0);
        }

        SECTION("patch name 01")
        {
            TestHoaProcess t("hoa.process~", LA(5, TEST_DATA_DIR "/hoa_test_01"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @target, -1);
        }

        SECTION("patch name 02")
        {
            TestHoaProcess t("hoa.process~", LA(5, TEST_DATA_DIR "/hoa_test_02"));
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 4);
            REQUIRE_PROPERTY_FLOAT(t, @target, -1);
        }
    }

    SECTION("control 01")
    {
        TestExtHoaProcess t("hoa.process~", LA(2, TEST_DATA_DIR "/hoa_test_01"));
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);

        // bang
        t.sendBangTo(1);
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.isOutputBangAt(0));
        REQUIRE(t.messagesAt(0) == messageList(&s_bang, &s_bang, &s_bang, &s_bang, &s_bang));

        // float
        t.sendFloatTo(10, 1);
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.isOutputFloatAt(0));
        REQUIRE(t.outputFloatAt(0) == Approx(10));
        REQUIRE(t.messagesAt(0) == messageList(10, 10, 10, 10, 10));

        // symbol
        t.sendSymbolTo(gensym("ABCD"), 1);
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.isOutputSymbolAt(0));
        REQUIRE(t.outputSymbolAt(0) == gensym("ABCD"));
        REQUIRE(t.messagesAt(0) == messageList(gensym("ABCD"), gensym("ABCD"), gensym("ABCD"), gensym("ABCD"), gensym("ABCD")));

        // list
        t.sendListTo(LF(1, 2, 3), 1);
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.isOutputListAt(0));
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3));
        REQUIRE(t.messagesAt(0) == messageList(LF(1, 2, 3), LF(1, 2, 3), LF(1, 2, 3), LF(1, 2, 3), LF(1, 2, 3)));

        // any
        pd::External a("t", LA("a"));
        a.connectTo(0, t, 1);
        a.sendMessage(gensym("MSG"), LF(1, 2));

        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.isOutputAnyAt(0));
        REQUIRE(t.outputAnyAt(0) == LA("MSG", 1, 2));
    }

    SECTION("@target")
    {
        TestExtHoaProcess t("hoa.process~", LA(2, TEST_DATA_DIR "/hoa_test_01"));
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);

        t->setProperty("@target", LF(1));

        // bang
        t.sendBangTo(1);
        REQUIRE(t.messagesAt(0) == messageList(&s_bang));
        t.sendFloatTo(-10, 1);
        REQUIRE(t.messagesAt(0) == messageList(-10));
        t.sendSymbolTo("ABC", 1);
        REQUIRE(t.messagesAt(0) == messageList(gensym("ABC")));
        t.sendListTo(LF(1, 2, 3), 1);
        REQUIRE(t.messagesAt(0) == messageList(LF(1, 2, 3)));
    }
}
