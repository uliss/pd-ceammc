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
    test::pdPrintToStdError();

    SECTION("init")
    {
        pd::External t("prop.get", LA("@interval"));
        REQUIRE(t.object());
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);

        LogExternalOutput l0;
        t.connectTo(1, l0, 0);

        TestPdExternal<MetroSeq> mseq("metro.seq", LA(1, 1, 1, "@interval", 90));
        REQUIRE(mseq.object());

        t.sendBang();
        REQUIRE(l0.msg().isNone());

        t.connectTo(0, mseq, 0);

        // get property
        t.sendBang();
        REQUIRE(l0.msg().isFloat());
        REQUIRE(l0.msg().atomValue().asFloat() == 90);

        mseq->setProperty("@interval", LF(25.5));

        t.sendBang();
        REQUIRE(l0.msg().isFloat());
        REQUIRE(l0.msg().atomValue().asFloat() == 25.5);
    }
}
