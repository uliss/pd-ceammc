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
#include "ui_colorpanel.h"
#include "test_ui.h"

UI_COMPLETE_TEST_SETUP(ColorPanel)

TEST_CASE("ui.colorpanel", "[ui.colorpanel]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestColorPanel t("ui.colorpanel");
        REQUIRE(t->numOutlets() == 1);
        REQUIRE(t->numInlets() == 1);
        REQUIRE_UI_LIST_PROPERTY(t, "size", LF(193, 105));
        REQUIRE_UI_LIST_PROPERTY(t, "matrix", LF(24, 13));
        REQUIRE_UI_FLOAT_PROPERTY(t, "saturation", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "hue", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "lightness", 1);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LA(-1, -1));
        REQUIRE_UI_FLOAT_PROPERTY(t, "pd", 0);
        REQUIRE_UI_LIST_PROPERTY(t, "hex", LA("#FFFFFF"));
        REQUIRE_UI_LIST_PROPERTY(t, "hsl", LF(1, 1, 1));
        REQUIRE_UI_LIST_PROPERTY(t, "rgb", LF(1, 1, 1));
        REQUIRE_UI_FLOAT_PROPERTY(t, "reverse", 0);
    }

    SECTION("external")
    {
        // just create test
        TestExtColorPanel t("ui.colorpanel");
    }

    SECTION("mouse")
    {
        TestExtColorPanel t("ui.colorpanel");

        t.mouseDown(5, 5);
        REQUIRE_OUTPUT_ANY(t, 0, LA("@hex", "#FFFFFF"));

        t.mouseDown(5, 15);
        REQUIRE_OUTPUT_ANY(t, 0, LA("@hex", "#FFD7D7"));

        t.mouseDown(5, 50);
        REQUIRE_OUTPUT_ANY(t, 0, LA("@hex", "#FF1313"));

        t.mouseDown(5, 90);
        REQUIRE_OUTPUT_ANY(t, 0, LA("@hex", "#4E0000"));
    }

    SECTION("set")
    {
        TestExtColorPanel t("ui.colorpanel");

        t <<= LA("set", 3, 4);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(3, 4));

        // invalid
        t <<= LA("set");
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(3, 4));

        t <<= LA("set", 1);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(3, 4));

        t <<= LA("set", "?", 1);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(3, 4));

        t <<= LA("set", "?", "?");
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(3, 4));

        t <<= LA("set", 1, "?");
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(3, 4));

        t <<= LA("set", -1, 1);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(3, 4));

        t <<= LA("set", 1, -1);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(3, 4));

        t <<= LA("set", 100, 1);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(3, 4));

        t <<= LA("set", 1, 100);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(3, 4));

        t <<= LA("set", 1000, 100);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(3, 4));
    }

    SECTION("@index")
    {
        TestExtColorPanel t("ui.colorpanel");

        t << $2("@index", 1, 2);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(1, 2));

        t << $2("@index", 1, 10);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(1, 10));
    }

    SECTION("send")
    {
        TestExtColorPanel t("ui.colorpanel", LA("@send", "r1"));
        t.addListener("r1");

        t.mouseDown(30, 40);
        REQUIRE_ANY_WAS_SEND(t, "r1", LA("@hex", "#FFD762"));
    }

    SECTION("presets")
    {
        TestExtColorPanel t("ui.colorpanel");
        REQUIRE_UI_LIST_PROPERTY(t, "presetname", LA("ui.colorpanel.0"));

        t->storePreset(0);
        t <<= LA("set", 2, 3);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(2, 3));
        t->storePreset(1);
        t <<= LA("set", 4, 5);
        t->storePreset(2);

        REQUIRE_NO_OUTPUT(t);

        t->loadPreset(0);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(4, 5));
        REQUIRE_OUTPUT_ANY(t, 0, LA("@hex", "#FFFE3A"));

        t->loadPreset(1);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(2, 3));
        REQUIRE_OUTPUT_ANY(t, 0, LA("@hex", "#FFC489"));

        t->loadPreset(2);
        REQUIRE_UI_LIST_PROPERTY(t, "index", LF(4, 5));
    }

    SECTION("@pd?")
    {
        TestExtColorPanel t("ui.colorpanel");

        REQUIRE_UI_FLOAT_PROPERTY(t, "pd", 0);
        t.mouseDown(4, 5);
        REQUIRE_UI_FLOAT_PROPERTY(t, "pd", -16777216.f);
        t.mouseDown(24, 35);
        REQUIRE_UI_FLOAT_PROPERTY(t, "pd", -16757091.f);
    }

    SECTION("@rgb?")
    {
        TestExtColorPanel t("ui.colorpanel");

        REQUIRE_UI_LIST_PROPERTY(t, "rgb", LX(1, 1, 1));
        t.mouseDown(4, 5);
        REQUIRE_UI_LIST_PROPERTY(t, "rgb", LX(1, 1, 1));
        t.mouseDown(24, 35);
        REQUIRE_UI_LIST_PROPERTY(t, "rgb", LX(1, 0.692307, 0.384615));
    }

    SECTION("@hsl?")
    {
        TestExtColorPanel t("ui.colorpanel");

        REQUIRE_UI_LIST_PROPERTY(t, "hsl", LF(1, 1, 1));
        t.mouseDown(1, 2);
        REQUIRE_UI_LIST_PROPERTY(t, "hsl", LF(0.f, 1, 1));
        t.mouseDown(24, 35);
        REQUIRE_UI_LIST_PROPERTY(t, "hsl", LX(0.0833333, 1, 0.692307));
    }

    SECTION("@hex?")
    {
        TestExtColorPanel t("ui.colorpanel");

        REQUIRE_UI_LIST_PROPERTY(t, "hex", LA("#FFFFFF"));

        t.mouseDown(1, 2);
        REQUIRE_UI_LIST_PROPERTY(t, "hex", LA("#FFFFFF"));

        t.mouseDown(24, 50);
        REQUIRE_UI_LIST_PROPERTY(t, "hex", LA("#FF8913"));
    }

    SECTION("bang")
    {
        TestExtColorPanel t("ui.colorpanel");

        t << BANG;
        REQUIRE_NO_OUTPUT(t);

        t.mouseDown(24, 35);
        t << BANG;
        REQUIRE_OUTPUT_ANY(t, 0, LA("@hex", "#FFB062"));
    }
}
