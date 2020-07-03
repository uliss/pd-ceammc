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
#include "ui_icon.h"
#include "test_ui.h"

UI_COMPLETE_TEST_SETUP(Icon)

TEST_CASE("ui.icon", "[ui.icon]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestIcon t("ui.icon");
        REQUIRE(t->numOutlets() == 1);
        REQUIRE(t->numInlets() == 1);
        REQUIRE_UI_LIST_PROPERTY(t, "size", LF(24, 24));
        REQUIRE_UI_LIST_PROPERTY(t, "presetname", LA("ui.icon.0"));
        REQUIRE_UI_LIST_PROPERTY(t, "icon", LA("help"));
        REQUIRE_UI_LIST_PROPERTY(t, "mode", LA("button"));
        REQUIRE_UI_FLOAT_PROPERTY(t, "enabled", 1);
    }

    SECTION("external")
    {
        TestExtIcon t("ui.icon");

        t <<= LA("set", "play");
        REQUIRE_UI_LIST_PROPERTY(t, "icon", LA("play"));
    }

    SECTION("click")
    {
        SECTION("bang")
        {
            TestExtIcon t("ui.icon", LA("@send", "r1", "@mode", "bang"));
            t.addListener("r1");
            t.mouseDown(3, 3);
            REQUIRE_OUTPUT_BANG(t, 0);
            REQUIRE_BANG_WAS_SEND(t, "r1");

            t.mouseDown(3, 3, EMOD_ALT);
            REQUIRE_NO_OUTPUT(t);

            t.mouseUp(3, 3);
            REQUIRE_NO_OUTPUT(t);
        }

        SECTION("button")
        {
            TestExtIcon t("ui.icon", LA("@send", "r1", "@mode", "button"));
            t.addListener("r1");
            t.mouseDown(3, 3);
            REQUIRE_OUTPUT_FLOAT(t, 0, 1);
            REQUIRE_FLOAT_WAS_SEND(t, "r1", 1);

            t.mouseUp(3, 3);
            REQUIRE_OUTPUT_FLOAT(t, 0, 0);
            REQUIRE_FLOAT_WAS_SEND(t, "r1", 0);

            t.mouseUp(3, 3, EMOD_ALT);
            REQUIRE_NO_OUTPUT(t);
        }

        SECTION("toggle")
        {
            TestExtIcon t("ui.icon", LA("@send", "r1", "@mode", "toggle"));
            t.addListener("r1");
            t.mouseDown(3, 3);
            REQUIRE_OUTPUT_FLOAT(t, 0, 1);
            REQUIRE_FLOAT_WAS_SEND(t, "r1", 1);

            t.mouseUp(3, 3);
            REQUIRE_NO_OUTPUT(t);

            t.mouseDown(3, 3);
            REQUIRE_OUTPUT_FLOAT(t, 0, 0);
            REQUIRE_FLOAT_WAS_SEND(t, "r1", 0);

            t.mouseUp(3, 3);
            REQUIRE_NO_OUTPUT(t);

            t.mouseDown(3, 3);
            REQUIRE_OUTPUT_FLOAT(t, 0, 1);
            REQUIRE_FLOAT_WAS_SEND(t, "r1", 1);

            t.mouseUp(3, 3);
            REQUIRE_NO_OUTPUT(t);
        }
    }

    SECTION("enable")
    {
        TestExtIcon t("ui.icon");
        REQUIRE_UI_FLOAT_PROPERTY(t, "enabled", 1);

        t << $1("@enabled", 0.f);

        t << BANG;
        REQUIRE_NO_OUTPUT(t);

        t << 34;
        REQUIRE_NO_OUTPUT(t);

        t.mouseDown(3, 3);
        REQUIRE_NO_OUTPUT(t);

        t.mouseUp(3, 3);
        REQUIRE_NO_OUTPUT(t);

        t << $1("@enabled", 1);
        t << 34;
        REQUIRE_OUTPUT_FLOAT(t, 0, 1);
    }
}
