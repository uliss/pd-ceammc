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
#include "../ui/mod_ui.h"
#include "../ui/ui_slider2d.h"
#include "ui_external_test.h"

UI_COMPLETE_TEST_SETUP(Slider2D)

TEST_CASE("ui.slider2d", "[ui.slider2d]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestSlider2D t("ui.slider2d");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 1);
        REQUIRE_UI_LIST_PROPERTY(t, "size", L2(100, 100));
        REQUIRE_UI_FLOAT_PROPERTY(t, "x_range", 2);
        REQUIRE_UI_FLOAT_PROPERTY(t, "y_range", 2);
        REQUIRE_UI_FLOAT_PROPERTY(t, "x_value", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "y_value", 0);
        REQUIRE(t->presetId() == gensym("ui.slider2d.0"));
        REQUIRE(t->realValue() == L2(0.f, 0.f));
        REQUIRE(t->realXValue() == 0);
        REQUIRE(t->realYValue() == 0);
        REQUIRE(t->xRange() == 2);
        REQUIRE(t->yRange() == 2);
    }

    SECTION("presets")
    {
        TestSlider2D t("ui.slider2d");
        t->setRealValue(L2(1, 1));
        t->storePreset(0);
        t->setRealValue(L2(-1, -1));
        t->storePreset(1);
        t->setRealValue(L2(-1, 1));
        t->storePreset(2);
        t->setRealValue(L2(1, -1));
        t->storePreset(3);

        t->loadPreset(4);
        REQUIRE(t->realValue() == L2(0.f, 0.f));
        t->loadPreset(0);
        REQUIRE(t->realValue() == L2(1, 1));
        t->loadPreset(1);
        REQUIRE(t->realValue() == L2(-1, -1));
        t->loadPreset(2);
        REQUIRE(t->realValue() == L2(-1, 1));
        t->loadPreset(3);
        REQUIRE(t->realValue() == L2(1, -1));

        t->clearPreset(2);
        t->loadPreset(2);
        REQUIRE(t->realValue() == L2(0.f, 0.f));
    }

    SECTION("setValue")
    {
        TestSlider2D t("ui.slider2d");
        REQUIRE(t->realValue() == L2(0.f, 0.f));

        // invalid
        REQUIRE_FALSE(t->setRealValue(AtomList()));
        REQUIRE_FALSE(t->setRealValue(L1(100)));
        REQUIRE_FALSE(t->setRealValue(L2("A", 1)));
        REQUIRE_FALSE(t->setRealValue(L2(1, "B")));
        REQUIRE_FALSE(t->setRealValue(L2("A", "B")));

        REQUIRE(t->setRealValue(L2(10, 20)));
        REQUIRE(t->realValue() == L2(1, 1));
        REQUIRE(t->setRealValue(L2(-10, -20)));
        REQUIRE(t->realValue() == L2(-1, -1));

        t->setRealValue(L2(0.2, 0.4));
        REQUIRE(t->realValue() == ListApprox(0.2, 0.4));

        // zero range
        t->setProperty(gensym("x_min"), L1(2));
        t->setProperty(gensym("x_max"), L1(2));
        REQUIRE(t->realValue() == ListApprox(2, 0.4));

        REQUIRE_FALSE(t->setRealValue(L2(1, 1)));
        REQUIRE(t->realValue() == ListApprox(2, 0.4));

        t->setProperty(gensym("y_min"), L1(10));
        t->setProperty(gensym("y_max"), L1(10));
        REQUIRE(t->realValue() == L2(2, 10));

        REQUIRE_FALSE(t->setRealValue(L2(1, 1)));
        REQUIRE(t->realValue() == L2(2, 10));
    }

    SECTION("@x/y_value")
    {
        TestSlider2D t("ui.slider2d");

        t->setProperty(gensym("x_value"), AtomList());
        REQUIRE(t->realXValue() == Approx(0));
        t->setProperty(gensym("x_value"), L1("A"));
        REQUIRE(t->realXValue() == Approx(0));
        t->setProperty(gensym("x_value"), L1(0.1));
        REQUIRE(t->realXValue() == Approx(0.1));
        t->setProperty(gensym("x_value"), L1(20));
        REQUIRE(t->realXValue() == Approx(1));
        t->setProperty(gensym("x_value"), L1(-20));
        REQUIRE(t->realXValue() == Approx(-1));

        t->setProperty(gensym("y_value"), AtomList());
        REQUIRE(t->realYValue() == Approx(0));
        t->setProperty(gensym("y_value"), L1("A"));
        REQUIRE(t->realYValue() == Approx(0));
        t->setProperty(gensym("y_value"), L1(0.1));
        REQUIRE(t->realYValue() == Approx(0.1));
        t->setProperty(gensym("y_value"), L1(20));
        REQUIRE(t->realYValue() == Approx(1));
        t->setProperty(gensym("y_value"), L1(-20));
        REQUIRE(t->realYValue() == Approx(-1));

        t->setProperty(gensym("x_min"), L1(1));
        t->setProperty(gensym("x_max"), L1(1));
        t->setProperty(gensym("y_min"), L1(10));
        t->setProperty(gensym("y_max"), L1(10));

        t->setProperty(gensym("x_value"), L1(1));
        REQUIRE(t->realXValue() == Approx(1));
        t->setProperty(gensym("y_value"), L1(10));
        REQUIRE(t->realYValue() == Approx(10));
    }

    SECTION("move")
    {
        TestSlider2D t("ui.slider2d");

        t->m_move(AtomList());
        REQUIRE(t->realValue() == L2(0.f, 0.f));
        t->m_move(L1(1));
        REQUIRE(t->realValue() == L2(0.f, 0.f));
        t->m_move(L2("A", "B"));
        REQUIRE(t->realValue() == L2(0.f, 0.f));
        t->m_move(L2(1, "B"));
        REQUIRE(t->realValue() == L2(0.f, 0.f));
        t->m_move(L2("A", 1));
        REQUIRE(t->realValue() == L2(0.f, 0.f));
        t->m_move(L2(0.1, -0.2));
        REQUIRE(t->realValue() == ListApprox(0.1, -0.2));
        t->m_move(L2(0.1, -0.2));
        REQUIRE(t->realValue() == ListApprox(0.2, -0.4));
        t->m_move(L2(0.1, -0.2));
        REQUIRE(t->realValue() == ListApprox(0.3, -0.6));
        t->m_move(L2(0.4, -0.3));
        REQUIRE(t->realValue() == ListApprox(0.7, -0.9));
        t->m_move(L2(0.4, -0.3));
        REQUIRE(t->realValue() == ListApprox(1, -1));
    }

    SECTION("mouse")
    {
        TestExtSlider2D t("ui.slider2d");
        t.mouseDown(0, 0);
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(-1, -1));
        t.mouseDown(50, 0);
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(0, -1));
        t.mouseDown(100, 0);
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(1, -1));
        t.mouseDown(0, 50);
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(-1, 0));
        t.mouseDown(50, 50);
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(0, 0));
        t.mouseDown(100, 50);
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(1, 0));
        t.mouseDown(0, 100);
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(-1, 1));
        t.mouseDown(50, 100);
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(0, 1));
        t.mouseDown(100, 100);
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(1, 1));

        t.mouseUp(0, 50);
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(-1, 0));

        t.mouseDrag(50, 100);
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(0, 1));
        t.mouseDrag(5000, 100000);
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(1, 1));
    }

    SECTION("external")
    {
        TestExtSlider2D t("ui.slider2d");

        t.send(10);
        REQUIRE_NO_OUTPUT(t);

        t.send(gensym("A"));
        REQUIRE_NO_OUTPUT(t);

        // invalid
        t.send(L1(200));
        REQUIRE_NO_OUTPUT(t);
        t.send(L1("ABC"));
        REQUIRE_NO_OUTPUT(t);
        t.send(L2("A", "B"));
        REQUIRE_NO_OUTPUT(t);
        t.send(L2("A", 1));
        REQUIRE_NO_OUTPUT(t);
        t.send(L2(1, "B"));
        REQUIRE_NO_OUTPUT(t);

        // ok
        t.send(L2(0.1, 0.2));
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(0.1, 0.2));

        t.bang();
        REQUIRE_OUTPUT_LIST(t, 0, ListApprox(0.1, 0.2));

        t.call("set", AtomList());
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->realValue() == ListApprox(0.1, 0.2));

        t.call("set", L1(1));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->realValue() == ListApprox(0.1, 0.2));

        t.call("set", L2("A", "B"));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->realValue() == ListApprox(0.1, 0.2));

        t.call("set", L2("A", 1));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->realValue() == ListApprox(0.1, 0.2));

        t.call("set", L2(1, "B"));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->realValue() == ListApprox(0.1, 0.2));

        t.call("set", L2(-0.5, 0.7));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->realValue() == ListApprox(-0.5, 0.7));
    }

    SECTION("send")
    {
        TestExtSlider2D t("ui.slider2d", L2("@send", "r1"));
        t.addListener("r1");

        t << BANG;
        REQUIRE_LIST_WAS_SEND(t, "r1", L2(0.f, 0.f));

        t << L2(0.4, -0.6);
        REQUIRE_LIST_WAS_SEND(t, "r1", ListApprox(0.4, -0.6));

        t.mouseDown(30, 70);
        REQUIRE_LIST_WAS_SEND(t, "r1", ListApprox(-0.4, 0.4));
    }
}
