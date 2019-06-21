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
#include "test_sound.h"

PD_COMPLETE_TEST_SETUP(HoaProcess, spat, hoa_process)

TEST_CASE("hoa.process~", "[externals]")
{
    pd_test_init([]() { ceammc_spat_setup(); });
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("empty args")
        {
            TestExtHoaProcess t("hoa.process~");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 0);
            REQUIRE_PROPERTY(t, @domain, S("harmonics"));
        }

        SECTION("no patch")
        {
            TestExtHoaProcess t("hoa.process~", LA(5));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 0);
            REQUIRE_PROPERTY(t, @domain, S("harmonics"));
        }

        SECTION("invalid patch name")
        {
            TestExtHoaProcess t("hoa.process~", LA(5, "not-exists"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 0);
            REQUIRE_PROPERTY(t, @domain, S("harmonics"));
        }

        SECTION("patch name 01")
        {
            TestExtHoaProcess t("hoa.process~", LA(5, TEST_DATA_DIR "/hoa_test_01"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @target, -1);
            REQUIRE_PROPERTY(t, @domain, S("harmonics"));
        }

        SECTION("patch name 02")
        {
            TestExtHoaProcess t("hoa.process~", LA(5, TEST_DATA_DIR "/hoa_test_02"));
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 4);
            REQUIRE_PROPERTY_FLOAT(t, @target, -1);
            REQUIRE_PROPERTY(t, @domain, S("harmonics"));
        }

        SECTION("invalid patch name planewaves")
        {
            TestExtHoaProcess t("hoa.process~", LA(11, "not-exists", "planewaves"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 0);
            REQUIRE_PROPERTY_FLOAT(t, @target, -1);
            REQUIRE_PROPERTY(t, @domain, S("planewaves"));
        }

        SECTION("patch name planewaves")
        {
            TestExtHoaProcess t("hoa.process~", LA(15, TEST_DATA_DIR "/hoa_test_10", "planewaves"));
            REQUIRE_PROPERTY_FLOAT(t, @nwaves, 15);
            REQUIRE_PROPERTY(t, @domain, S("planewaves"));
            REQUIRE(t.numInlets() == 15);
            REQUIRE(t.numOutlets() == 15);

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

    SECTION("control 03")
    {
        TestExtHoaProcess t("hoa.process~", LA(2, TEST_DATA_DIR "/hoa_test_03"));
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("control 04")
    {
        TestExtHoaProcess t("hoa.process~", LA(1, TEST_DATA_DIR "/hoa_test_04"));
        t->setProperty("@target", LF(1));
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 4);

        t.sendFloatTo(2.0, 1);
        REQUIRE(t.messagesAt(0) == messageList(2));
        REQUIRE(t.messagesAt(1) == messageList(4));
        REQUIRE(t.messagesAt(2) == messageList(6));
        REQUIRE(t.messagesAt(3) == messageList(8));
    }

    SECTION("control 05")
    {
        TestExtHoaProcess t("hoa.process~", LA(1, TEST_DATA_DIR "/hoa_test_05"));
        t->setProperty("@target", LF(1));
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 4);

        t.sendFloatTo(2.0, 1);
        REQUIRE(t.messagesAt(3) == messageList(2));
        REQUIRE(t.messagesAt(2) == messageList(4));
        REQUIRE(t.messagesAt(1) == messageList(6));
        REQUIRE(t.messagesAt(0) == messageList(8));
    }

    SECTION("audio 10")
    {
        TestExtHoaProcess t("hoa.process~", LA(3, TEST_DATA_DIR "/hoa_test_10"));
        REQUIRE(t.numInlets() == 7);
        REQUIRE(t.numOutlets() == 7);
    }

    SECTION("audio 10 plain")
    {
        TestExtHoaProcess t("hoa.process~", LA(3, TEST_DATA_DIR "/hoa_test_10", "planewaves"));
        REQUIRE(t.numInlets() == 3);
        REQUIRE(t.numOutlets() == 3);
    }

    SECTION("audio 11")
    {
        TestExtHoaProcess t("hoa.process~", LA(4, TEST_DATA_DIR "/hoa_test_11"));
        REQUIRE(t.numInlets() == 9);
        REQUIRE(t.numOutlets() == 9);
    }

    SECTION("audio 11 plane")
    {
        TestExtHoaProcess t("hoa.process~", LA(4, TEST_DATA_DIR "/hoa_test_11", "planewaves"));
        REQUIRE(t.numInlets() == 4);
        REQUIRE(t.numOutlets() == 4);
    }

    SECTION("audio 12")
    {
        TestExtHoaProcess t("hoa.process~", LA(5, TEST_DATA_DIR "/hoa_test_12"));
        REQUIRE(t.numInlets() == 11);
        REQUIRE(t.numOutlets() == 12);
    }

    SECTION("audio 12 plane")
    {
        TestExtHoaProcess t("hoa.process~", LA(5, TEST_DATA_DIR "/hoa_test_12", "planewaves"));
        REQUIRE(t.numInlets() == 5);
        REQUIRE(t.numOutlets() == 6);
    }

    SECTION("audio 13")
    {
        TestExtHoaProcess t("hoa.process~", LA(1, TEST_DATA_DIR "/hoa_test_10"));
        REQUIRE(t.numInlets() == 3);
        REQUIRE(t.numOutlets() == 3);
        REQUIRE(t->numInputChannels() == 3);
        REQUIRE(t->numOutputChannels() == 3);

        pd::External sig1("sig~", LF(1));
        REQUIRE(sig1.connectTo(0, t, 0));
        pd::External sig2("sig~", LF(-1));
        REQUIRE(sig2.connectTo(0, t, 1));
        pd::External sig3("sig~", LF(2.5));
        REQUIRE(sig3.connectTo(0, t, 2));

        cnv->addExternal(sig1);
        cnv->addExternal(sig2);
        cnv->addExternal(sig3);
        cnv->addExternal(t);

        REQUIRE(t->inputBuffer().size() == 0);
        REQUIRE(t->outputBuffer().size() == 0);

        canvas_resume_dsp(1);
        t.schedTicks(1);
        canvas_suspend_dsp();

        REQUIRE(t->inputBuffer().size() != 0);
        REQUIRE(t->outputBuffer().size() != 0);

        for (size_t i = 0; i < t->outputBuffer().size(); i++) {
            auto samp = t->outputBuffer()[i];
            if (i / t->blockSize() == 0)
                REQUIRE(samp == 2);
            else if (i / t->blockSize() == 1)
                REQUIRE(samp == -2);
            else if (i / t->blockSize() == 2)
                REQUIRE(samp == 5);
        }
    }

    SECTION("audio 13 plane")
    {
        TestExtHoaProcess t("hoa.process~", LA(3, TEST_DATA_DIR "/hoa_test_10", "planewaves"));
        REQUIRE(t.numInlets() == 3);
        REQUIRE(t.numOutlets() == 3);
        REQUIRE(t->numInputChannels() == 3);
        REQUIRE(t->numOutputChannels() == 3);

        pd::External sig1("sig~", LF(1));
        REQUIRE(sig1.connectTo(0, t, 0));
        pd::External sig2("sig~", LF(-1));
        REQUIRE(sig2.connectTo(0, t, 1));
        pd::External sig3("sig~", LF(2.5));
        REQUIRE(sig3.connectTo(0, t, 2));

        cnv->addExternal(sig1);
        cnv->addExternal(sig2);
        cnv->addExternal(sig3);
        cnv->addExternal(t);

        REQUIRE(t->inputBuffer().size() == 0);
        REQUIRE(t->outputBuffer().size() == 0);

        canvas_resume_dsp(1);
        t.schedTicks(1);
        canvas_suspend_dsp();

        REQUIRE(t->inputBuffer().size() != 0);
        REQUIRE(t->outputBuffer().size() != 0);

        for (size_t i = 0; i < t->outputBuffer().size(); i++) {
            auto samp = t->outputBuffer()[i];
            if (i / t->blockSize() == 0)
                REQUIRE(samp == 2);
            else if (i / t->blockSize() == 1)
                REQUIRE(samp == -2);
            else if (i / t->blockSize() == 2)
                REQUIRE(samp == 5);
        }
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
