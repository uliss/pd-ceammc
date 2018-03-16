/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "../ui/ui_bang.h"
#include "ceammc_format.h"
#include "ui_external_test.h"

UI_COMPLETE_TEST_SETUP(Bang)

TEST_CASE("ui.bang", "[ui.bang]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestBang t("ui.bang");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 1);
        REQUIRE_UI_LIST_PROPERTY(t, "send", L1("(null)"));
        REQUIRE_UI_LIST_PROPERTY(t, "receive", L1("(null)"));
    }

    SECTION("external")
    {
        TestExtBang t("ui.bang");

        t.bang();
        REQUIRE_OUTPUT_BANG(t, 0);
        t.send(10);
        REQUIRE_OUTPUT_BANG(t, 0);
        t.send(gensym("ABC"));
        REQUIRE_OUTPUT_BANG(t, 0);
        t.send(AtomList());
        REQUIRE_OUTPUT_BANG(t, 0);
        t.send(L1(1));
        REQUIRE_OUTPUT_BANG(t, 0);
        t.send(L3(1, 2, 3));
        REQUIRE_OUTPUT_BANG(t, 0);
        t.call("a");
        REQUIRE_OUTPUT_BANG(t, 0);

        t.mouseDown(5, 5);
        REQUIRE_OUTPUT_BANG(t, 0);
        t.mouseUp(5, 5);
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("send test")
    {
        TestExtBang t("ui.bang", L2("@send", "r1"));
        t.addListener("r1");

        t << BANG;
        REQUIRE_BANG_WAS_SEND(t, "r1");
        REQUIRE_NONE_WAS_SEND(t, "r1");

        t << 20;
        REQUIRE_BANG_WAS_SEND(t, "r1");

        t << "ABC";
        REQUIRE_BANG_WAS_SEND(t, "r1");

        t << AtomList();
        REQUIRE_BANG_WAS_SEND(t, "r1");

        t << L1(100);
        REQUIRE_BANG_WAS_SEND(t, "r1");

        t << L5(1, 2, 3, 4, 5);
        REQUIRE_BANG_WAS_SEND(t, "r1");

        t.call("any");
        REQUIRE_BANG_WAS_SEND(t, "r1");

        t.mouseDown(5, 5);
        REQUIRE_BANG_WAS_SEND(t, "r1");
    }
}
