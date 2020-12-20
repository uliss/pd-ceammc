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
#include "ceammc_timeline.h"
#include "test_external.h"

struct Dummy {
};

typedef TestPdExternal<Dummy> TestExt;


TEST_CASE("tl.transport", "[externals]")
{
    SECTION("init")
    {
        test::pdPrintToStdError();
        auto cnv = PureData::instance().createTopCanvas("test");

        TestExt t("tl.transport");
        REQUIRE(t.object());

        TestExt cue0("tl.cue");
        REQUIRE(cue0.object());
        cue0.call("pos", LF(100, 10));
        TestExt cue1("tl.cue");
        cue1.call("pos", LF(200, 10));

        REQUIRE(cue0.xPos() == 100);
        REQUIRE(cue1.xPos() == 200);

        TestExt b0("tl.bang");
        REQUIRE(b0.object());
        TestExt b1("tl.bang");
        TestExt b2("tl.bang");

        t.call("cue", 1);
    }
}
