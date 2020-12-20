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
#include "mod_ui.h"
#include "ui_slider2d.h"
#include "test_ui.h"

UI_COMPLETE_TEST_SETUP(Slider2D)

TEST_CASE("ui.slider2d", "[ui.slider2d]")
{
    ui_test_init();
    //    test::pdPrintToStdError(true);

    SECTION("construct")
    {
        TestSlider2D t("ui.slider2d");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 1);
        REQUIRE_UI_LIST_PROPERTY(t, "size", LF(100, 100));
        REQUIRE_UI_FLOAT_PROPERTY(t, "x_range", 2);
        REQUIRE_UI_FLOAT_PROPERTY(t, "y_range", 2);
        REQUIRE_UI_FLOAT_PROPERTY(t, "x_value", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "y_value", 0);
        REQUIRE(t->presetId() == gensym("ui.slider2d.0"));
        REQUIRE(t->realValue() == LF(0, 0));
        REQUIRE(t->realXValue() == 0);
        REQUIRE(t->realYValue() == 0);
        REQUIRE(t->xRange() == 2);
        REQUIRE(t->yRange() == 2);
    }

    SECTION("presets")
    {
        TestSlider2D t("ui.slider2d");
        t->setRealValue(LF(1, 1));
        t->storePreset(0);
        t->setRealValue(LA(-1, -1));
        t->storePreset(1);
        t->setRealValue(LA(-1, 1));
        t->storePreset(2);
        t->setRealValue(LF(1, -1));
        t->storePreset(3);

        t->loadPreset(4);
        REQUIRE(t->realValue() == LF(0.f, 0.f));
        t->loadPreset(0);
        REQUIRE(t->realValue() == LF(1, 1));
        t->loadPreset(1);
        REQUIRE(t->realValue() == LA(-1, -1));
        t->loadPreset(2);
        REQUIRE(t->realValue() == LA(-1, 1));
        t->loadPreset(3);
        REQUIRE(t->realValue() == LF(1, -1));

        t->clearPreset(2);
        t->loadPreset(2);
        REQUIRE(t->realValue() == LF(0.f, 0.f));
    }

    SECTION("setValue")
    {
        TestSlider2D t("ui.slider2d");
        REQUIRE(t->realValue() == LF(0.f, 0.f));

        // invalid
        REQUIRE_FALSE(t->setRealValue(L()));
        REQUIRE_FALSE(t->setRealValue(LF(100)));
        REQUIRE_FALSE(t->setRealValue(LA("A", 1)));
        REQUIRE_FALSE(t->setRealValue(LA(1, "B")));
        REQUIRE_FALSE(t->setRealValue(LA("A", "B")));

        REQUIRE(t->setRealValue(LF(10, 20)));
        REQUIRE(t->realValue() == LF(1, 1));
        REQUIRE(t->setRealValue(LA(-10, -20)));
        REQUIRE(t->realValue() == LA(-1, -1));

        t->setRealValue(LF(0.2, 0.4));
        REQUIRE(t->realValue() == LX(0.2, 0.4));

        // zero range
        t->setProperty(gensym("x_left"), LF(2));
        t->setProperty(gensym("x_right"), LF(2));
        REQUIRE(t->realValue() == LX(2, 0.4));

        REQUIRE_FALSE(t->setRealValue(LF(1, 1)));
        REQUIRE(t->realValue() == LX(2, 0.4));

        t->setProperty(gensym("y_top"), LF(10));
        t->setProperty(gensym("y_bottom"), LF(10));
        REQUIRE(t->realValue() == LF(2, 10));

        REQUIRE_FALSE(t->setRealValue(LF(1, 1)));
        REQUIRE(t->realValue() == LF(2, 10));
    }

    SECTION("@x/y_value")
    {
        TestSlider2D t("ui.slider2d");

        t->setProperty(gensym("x_value"), L());
        REQUIRE(t->realXValue() == Approx(0));
        t->setProperty(gensym("x_value"), LA("A"));
        REQUIRE(t->realXValue() == Approx(0));
        t->setProperty(gensym("x_value"), LF(0.1));
        REQUIRE(t->realXValue() == Approx(0.1));
        t->setProperty(gensym("x_value"), LF(20));
        REQUIRE(t->realXValue() == Approx(1));
        t->setProperty(gensym("x_value"), LF(-20));
        REQUIRE(t->realXValue() == Approx(-1));

        t->setProperty(gensym("y_value"), L());
        REQUIRE(t->realYValue() == Approx(0));
        t->setProperty(gensym("y_value"), LA("A"));
        REQUIRE(t->realYValue() == Approx(0));
        t->setProperty(gensym("y_value"), LF(0.1));
        REQUIRE(t->realYValue() == Approx(0.1));
        t->setProperty(gensym("y_value"), LF(20));
        REQUIRE(t->realYValue() == Approx(1));
        t->setProperty(gensym("y_value"), LF(-20));
        REQUIRE(t->realYValue() == Approx(-1));

        // set invalid ranges
        t->setProperty(gensym("x_left"), LF(1));
        t->setProperty(gensym("x_right"), LF(1));
        t->setProperty(gensym("y_top"), LF(10));
        t->setProperty(gensym("y_bottom"), LF(10));
        REQUIRE_UI_FLOAT_PROPERTY(t, "y_bottom", 10);

        t->setProperty(gensym("x_value"), LF(1));
        REQUIRE(t->realXValue() == Approx(1));
        t->setProperty(gensym("y_value"), LF(10));
        REQUIRE(t->realYValue() == Approx(10));
    }

    SECTION("move")
    {
        TestSlider2D t("ui.slider2d");

        t->m_move(L());
        REQUIRE(t->realValue() == LF(0.f, 0.f));
        t->m_move(LF(1));
        REQUIRE(t->realValue() == LF(0.f, 0.f));
        t->m_move(LA("A", "B"));
        REQUIRE(t->realValue() == LF(0.f, 0.f));
        t->m_move(LA(1, "B"));
        REQUIRE(t->realValue() == LF(0.f, 0.f));
        t->m_move(LA("A", 1));
        REQUIRE(t->realValue() == LF(0.f, 0.f));
        t->m_move(LA(0.1, -0.2));
        REQUIRE(t->realValue() == LX(0.1, -0.2));
        t->m_move(LA(0.1, -0.2));
        REQUIRE(t->realValue() == LX(0.2, -0.4));
        t->m_move(LA(0.1, -0.2));
        REQUIRE(t->realValue() == LX(0.3, -0.6));
        t->m_move(LA(0.4, -0.3));
        REQUIRE(t->realValue() == LX(0.7, -0.9));
        t->m_move(LA(0.4, -0.3));
        REQUIRE(t->realValue() == LX(1, -1));
    }

    SECTION("mouse")
    {
        TestExtSlider2D t("ui.slider2d");
        t.mouseDown(0, 0);
        REQUIRE_OUTPUT_LIST(t, 0, LX(-1, 1));
        t.mouseDown(50, 0);
        REQUIRE_OUTPUT_LIST(t, 0, LX(0, 1));
        t.mouseDown(100, 0);
        REQUIRE_OUTPUT_LIST(t, 0, LX(1, 1));
        t.mouseDown(0, 50);
        REQUIRE_OUTPUT_LIST(t, 0, LX(-1, 0));
        t.mouseDown(50, 50);
        REQUIRE_OUTPUT_LIST(t, 0, LX(0, 0));
        t.mouseDown(100, 50);
        REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
        t.mouseDown(0, 100);
        REQUIRE_OUTPUT_LIST(t, 0, LX(-1, -1));
        t.mouseDown(50, 100);
        REQUIRE_OUTPUT_LIST(t, 0, LX(0, -1));
        t.mouseDown(100, 100);
        REQUIRE_OUTPUT_LIST(t, 0, LX(1, -1));

        t.mouseUp(0, 50);
        REQUIRE_OUTPUT_LIST(t, 0, LX(-1, 0));

        t.mouseDrag(50, 100);
        REQUIRE_OUTPUT_LIST(t, 0, LX(0, -1));
        t.mouseDrag(5000, 100000);
        REQUIRE_OUTPUT_LIST(t, 0, LX(1, -1));
    }

    SECTION("external")
    {
        TestExtSlider2D t("ui.slider2d");

        t.send(10);
        REQUIRE_NO_OUTPUT(t);

        t.send(gensym("A"));
        REQUIRE_NO_OUTPUT(t);

        // invalid
        t.send(LF(200));
        REQUIRE_NO_OUTPUT(t);
        t.send(LA("ABC"));
        REQUIRE_NO_OUTPUT(t);
        t.send(LA("A", "B"));
        REQUIRE_NO_OUTPUT(t);
        t.send(LA("A", 1));
        REQUIRE_NO_OUTPUT(t);
        t.send(LA(1, "B"));
        REQUIRE_NO_OUTPUT(t);

        // ok
        t.send(LF(0.1, 0.2));
        REQUIRE_OUTPUT_LIST(t, 0, LX(0.1, 0.2));

        t.bang();
        REQUIRE_OUTPUT_LIST(t, 0, LX(0.1, 0.2));

        t.call("set", L());
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->realValue() == LX(0.1, 0.2));

        t.call("set", LF(1));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->realValue() == LX(0.1, 0.2));

        t.call("set", LA("A", "B"));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->realValue() == LX(0.1, 0.2));

        t.call("set", LA("A", 1));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->realValue() == LX(0.1, 0.2));

        t.call("set", LA(1, "B"));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->realValue() == LX(0.1, 0.2));

        t.call("set", LA(-0.5, 0.7));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->realValue() == LX(-0.5, 0.7));
    }

    SECTION("send")
    {
        TestExtSlider2D t("ui.slider2d", LA("@send", "r1"));
        t.addListener("r1");

        t << BANG;
        REQUIRE_LIST_WAS_SEND(t, "r1", LF(0.f, 0.f));

        t << LA(0.4, -0.6);
        REQUIRE_LIST_WAS_SEND(t, "r1", LX(0.4, -0.6));

        t.mouseDown(30, 70);
        REQUIRE_LIST_WAS_SEND(t, "r1", LX(-0.4, -0.4));
    }
}
