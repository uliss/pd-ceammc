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
#include "metro_seq.h"
#include "prop_declare.h"
#include "test_external.h"
#include "test_external_log_output.h"

void setup_prop_get();
void setup_metro_seq();

struct Dummy {
};

TEST_CASE("prop.get", "[externals]")
{
    setup_prop_get();
    setup_metro_seq();
    setup_prop_declare();
    test::pdPrintToStdError();

    SECTION("single")
    {
        pd::External t("prop.get", LA("@interval"));
        REQUIRE(t.object());
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);

        LogExternalOutput l0;
        REQUIRE(t.connectTo(1, l0, 0));

        TestPdExternal<MetroSeq> mseq("metro.seq", LA(90, 1, 1, 1));
        REQUIRE(mseq.object());

        t.sendBang();
        REQUIRE(l0.msg().isNone());

        REQUIRE(t.connectTo(0, mseq, 0));

        // get property
        t.sendBang();
        REQUIRE(l0.msg().isFloat());
        REQUIRE(l0.msg().atomValue().asFloat() == 90);

        mseq->setProperty("@interval", LF(25.5));

        t.sendBang();
        REQUIRE(l0.msg().isFloat());
        REQUIRE(l0.msg().atomValue().asFloat() == 25.5);
    }

    SECTION("multiple")
    {
        pd::External t("prop.get", LA("@interval", "@current"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 3);

        LogExternalOutput l0;
        LogExternalOutput l1;

        TestPdExternal<MetroSeq> mseq("metro.seq", LA(90, 1, 1, 1));
        REQUIRE(mseq.object());

        REQUIRE(t.connectTo(1, l0, 0));
        REQUIRE(t.connectTo(2, l1, 0));
        REQUIRE(t.connectTo(0, mseq, 0));

        // get properties
        t.sendBang();
        REQUIRE(l0.msg() == Message(90));
        REQUIRE(l1.msg() == Message(0.));

        mseq->setProperty("@interval", LF(25.5));
        mseq->setProperty("@current", LF(2));

        t.sendBang();
        REQUIRE(l0.msg() == Message(25.5));
        REQUIRE(l1.msg() == Message(2));
    }

    SECTION("dynamic")
    {
        pd::External t("prop.get", L());
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);

        LogExternalOutput log;

        TestPdExternal<MetroSeq> mseq("metro.seq", LA(90, 1, 1, 1));
        REQUIRE(mseq.object());

        REQUIRE(t.connectTo(1, log, 0));
        REQUIRE(t.connectTo(0, mseq, 0));

        t.sendMessage("@interval");
        REQUIRE(log.msg() == Message(90));

        t.sendMessage("@pattern");
        REQUIRE(log.msg() == Message(LF(1, 1, 1)));

        t.sendMessage("@interval?");
        REQUIRE(log.msg() == Message(90));

        t.sendMessage("@pattern?");
        REQUIRE(log.msg() == Message(LF(1, 1, 1)));

        log.reset();
        t.sendMessage("@????");
        REQUIRE(log.msgList().empty());

        t.sendMessage("not a property");
        REQUIRE(log.msgList().empty());

        t.sendBang();
        REQUIRE(log.msgList().empty());
    }

    SECTION("no props")
    {
        pd::External t("prop.get", L());
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);

        LogExternalOutput log;

        pd::External f("float", L());
        REQUIRE(f.object());

        REQUIRE(t.connectTo(1, log, 0));
        REQUIRE(t.connectTo(0, f, 0));

        t.sendMessage("@interval");
        t.sendBang();
        REQUIRE(log.msgList().empty());
    }

    SECTION("abstractions")
    {
        pd::External t("prop.get", L());
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);

        LogExternalOutput log;

        pd::External abs0(TEST_DATA_DIR "/base/prop_get_abs0", L());
        REQUIRE(abs0.object());

        REQUIRE(t.connectTo(1, log, 0));
        REQUIRE(t.connectTo(0, abs0, 0));

        t.sendMessage("@unknown");
        t.sendBang();
        REQUIRE(log.msgList().empty());

        t.sendMessage("@pget0");
        REQUIRE(log.msg() == Message(-100));

        t.sendMessage("@pget1");
        REQUIRE(log.msg() == Message(S("ABC")));

        t.sendMessage("@pget2");
        REQUIRE(log.msg() == Message({ 1, 2, 3 }));
    }

    SECTION("alias")
    {
        pd::External t("p.get");
        REQUIRE(t.object());
    }
}
