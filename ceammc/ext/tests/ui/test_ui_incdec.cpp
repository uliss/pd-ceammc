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
#include "ui_incdec.h"
#include "test_ui.h"

UI_COMPLETE_TEST_SETUP(IncDec)

TEST_CASE("ui.incdec", "[ui.incdec]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestIncDec t("ui.incdec~");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 1);
        REQUIRE_UI_LIST_PROPERTY(t, "size", LF(15, 20));
        REQUIRE_UI_FLOAT_PROPERTY(t, "step", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 0);
    }

    SECTION("external")
    {
        TestExtIncDec t("ui.incdec", LA("@send", "r1"));
        t.addListener("r1");

        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 0);

        t << BANG;
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 0);
        REQUIRE_OUTPUT_FLOAT(t, 0, 0);

        t << 100;
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 100);
        REQUIRE_OUTPUT_FLOAT(t, 0, 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 100);

        t << -100;
        REQUIRE_FLOAT_WAS_SEND(t, "r1", -100);
        REQUIRE_OUTPUT_FLOAT(t, 0, -100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", -100);

        // nothing
        t <<= LA("set");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", -100);

        t <<= LA("set", 5);
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 5);

        t << $1("@value", 4);
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 4);

        t <<= LA("inc");
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 5);
        REQUIRE_OUTPUT_FLOAT(t, 0, 5);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 5);

        t <<= LA("inc");
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 6);
        REQUIRE_OUTPUT_FLOAT(t, 0, 6);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 6);

        t <<= LA("dec");
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 5);
        REQUIRE_OUTPUT_FLOAT(t, 0, 5);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 5);

        t <<= LA("dec");
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 4);
        REQUIRE_OUTPUT_FLOAT(t, 0, 4);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 4);

        t <<= LA("@step", 0.5f);
        t <<= LA("dec");
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 3.5f);
        REQUIRE_OUTPUT_FLOAT(t, 0, 3.5f);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 3.5f);

        t <<= LA("store", 0.f);
        t << 2;
        t <<= LA("store", 1);
        t << 3;
        t <<= LA("store", 2);

        t << 4;

        t <<= LA("load", 0.f);
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 3.5f);
        REQUIRE_OUTPUT_FLOAT(t, 0, 3.5f);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 3.5f);

        t <<= LA("load", 1);
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 2);
        REQUIRE_OUTPUT_FLOAT(t, 0, 2);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 2);

        t <<= LA("load", 2);
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 3);
        REQUIRE_OUTPUT_FLOAT(t, 0, 3);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 3);

        t <<= LA("load", 3);
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 0);
        REQUIRE_OUTPUT_FLOAT(t, 0, 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 0);
    }

    SECTION("mouse")
    {
        TestExtIncDec t("ui.incdec");

        t.mouseDown(5, 5);
        REQUIRE_OUTPUT_FLOAT(t, 0, 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 1);
        t.mouseUp(5, 5);

        t.mouseDown(5, 5);
        REQUIRE_OUTPUT_FLOAT(t, 0, 2);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 2);
        t.mouseUp(5, 5);

        t.mouseDown(5, 15);
        REQUIRE_OUTPUT_FLOAT(t, 0, 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 1);
        t.mouseUp(5, 15);

        t.mouseDown(5, 15);
        REQUIRE_OUTPUT_FLOAT(t, 0, 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 0);
        t.mouseUp(5, 15);
    }
}
