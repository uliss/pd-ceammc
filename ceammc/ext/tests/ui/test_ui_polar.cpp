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
#include "ceammc_preset.h"
#include "ui_polar.h"

#include "test_ui_base.h"

#include <cmath>

UI_COMPLETE_TEST_SETUP(Polar)

TEST_CASE("ui.polar", "[ui.polar]")
{
    ui_test_init();

    SECTION("construct")
    {
        SECTION("default")
        {
            TestPolar t("ui.polar");
            REQUIRE(t->numInlets() == 1);
            REQUIRE(t->numOutlets() == 1);
            REQUIRE_UI_LIST_PROPERTY(t, "size", LF(100, 100));
            REQUIRE_UI_FLOAT_PROPERTY(t, "angle", 0);
            REQUIRE_UI_FLOAT_PROPERTY(t, "radius", 0);
            REQUIRE_UI_FLOAT_PROPERTY(t, "clockwise", 1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "radians", 0);
            REQUIRE_UI_FLOAT_PROPERTY(t, "positive", 0);
            REQUIRE_UI_LIST_PROPERTY(t, "direction", LA("N"));
            REQUIRE(t->presetId() == gensym("ui.polar.0"));
            REQUIRE(t->realValue() == LX(0, 0));
            REQUIRE(t->realAngle() == Approx(0));
        }

        SECTION("direction N")
        {
            TestPolar t("ui.polar", LA("@direction", "N"));
            REQUIRE_UI_LIST_PROPERTY(t, "direction", LA("N"));
            REQUIRE_UI_FLOAT_PROPERTY(t, "angle", 0);
            REQUIRE_UI_FLOAT_PROPERTY(t, "radius", 0);
            REQUIRE(t->realValue() == LX(0, 0));
            REQUIRE(t->realAngle() == Approx(0));
        }

        SECTION("direction E")
        {
            TestPolar t("ui.polar", LA("@direction", "E"));
            REQUIRE_UI_LIST_PROPERTY(t, "direction", LA("E"));
            REQUIRE_UI_FLOAT_PROPERTY(t, "angle", Approx(0));
            REQUIRE_UI_FLOAT_PROPERTY(t, "radius", 0);
            REQUIRE(t->realValue() == LX(0, 0));
            REQUIRE(t->realAngle() == Approx(0));
        }

        SECTION("direction W")
        {
            TestPolar t("ui.polar", LA("@direction", "W"));
            REQUIRE_UI_LIST_PROPERTY(t, "direction", LA("W"));
            REQUIRE_UI_FLOAT_PROPERTY(t, "angle", 0);
            REQUIRE_UI_FLOAT_PROPERTY(t, "radius", 0);
            REQUIRE(t->realValue() == LX(0, 0));
            REQUIRE(t->realAngle() == Approx(0));
        }

        SECTION("direction S")
        {
            TestPolar t("ui.polar", LA("@direction", "S"));
            REQUIRE_UI_LIST_PROPERTY(t, "direction", LA("S"));
            REQUIRE_UI_FLOAT_PROPERTY(t, "angle", 0);
            REQUIRE_UI_FLOAT_PROPERTY(t, "radius", 0);
            REQUIRE(t->realValue() == LX(0, 0));
            REQUIRE(t->realAngle() == Approx(0));
        }

        SECTION("@radians")
        {
            TestPolar t("ui.polar", LA("@radians", 1));
            REQUIRE_UI_FLOAT_PROPERTY(t, "radians", 1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "angle", 0.f);
            REQUIRE_UI_FLOAT_PROPERTY(t, "radius", 0.f);
            REQUIRE(t->realValue() == LAX(0.f, 0.f));
            REQUIRE(t->realAngle() == Approx(0));
        }
    }

    SECTION("presets")
    {
        TestPolar t("ui.polar");
        t->setRealValue(LF(1, 1));
        t->storePreset(0);
        t->setRealValue(LA(0.5, -1));
        t->storePreset(1);
        t->setRealValue(LA(1, 2));
        t->storePreset(2);
        t->setRealValue(LF(1, 0.5));
        t->storePreset(3);

        t->loadPreset(4);
        REQUIRE(t->realValue() == LX(0, 0));
        t->loadPreset(0);
        REQUIRE(t->realValue() == LX(1, 1));
        t->loadPreset(1);
        REQUIRE(t->realValue() == LX(0.5, -1));
        t->loadPreset(2);
        REQUIRE(t->realValue() == LX(1, 2));
        t->loadPreset(3);
        REQUIRE(t->realValue() == LX(1, 0.5));

        t->clearPreset(2);
        t->loadPreset(2);
        REQUIRE(t->realValue() == LX(0, 0));
    }

    SECTION("setValue")
    {
        SECTION("radians")
        {
            SECTION("default")
            {
                TestPolar t("ui.polar", LA("@radians", 1));
                REQUIRE(t->realValue() == LX(0, 0));

                // invalid
                REQUIRE_FALSE(t->setRealValue(L()));
                REQUIRE_FALSE(t->setRealValue(LF(100)));
                REQUIRE_FALSE(t->setRealValue(LA("A", 1)));
                REQUIRE_FALSE(t->setRealValue(LA(1, "B")));
                REQUIRE_FALSE(t->setRealValue(LA("A", "B")));

                // clip radius
                REQUIRE(t->setRealValue(LF(1.2, 1)));
                REQUIRE(t->realValue() == LX(1, 1));
                REQUIRE(t->setRealValue(LF(-0.1, 1)));
                REQUIRE(t->realValue() == LX(0, 1));

                // ok
                t->setRealValue(LF(1, 0.5));
                REQUIRE(t->realValue() == LX(1, 0.5));
                REQUIRE_UI_FLOAT_PROPERTY(t, "radius", 1);
                REQUIRE_UI_FLOAT_PROPERTY(t, "angle", 0.5);

                t->setRealValue(LF(1, -0.5));
                REQUIRE(t->realValue() == LX(1, -0.5));
                REQUIRE_UI_FLOAT_PROPERTY(t, "radius", 1);
                REQUIRE_UI_FLOAT_PROPERTY(t, "angle", -0.5);

                // angle wrap
                t->setRealValue(LF(1, 3.11));
                REQUIRE(t->realValue() == LX(1, 3.11));

                t->setRealValue(LF(1, 3.15));
                REQUIRE(t->realValue() == LX(1, -3.13319));

                t->setRealValue(LF(1, 6.27));
                REQUIRE(t->realValue() == LX(1, -0.013185).margin(0.00001));

                t->setRealValue(LF(1, 6.3));
                REQUIRE(t->realValue() == LX(1, 0.0168157).margin(0.00001));
            }

            SECTION("clockwise")
            {
                TestPolar t("ui.polar", LA("@clockwise", 1));
                REQUIRE(t->realValue() == LX(0, 0));

                // clip radius
                REQUIRE(t->setRealValue(LF(1.2, 1)));
                REQUIRE(t->realValue() == LX(1, 1));
                REQUIRE(t->setRealValue(LF(-0.1, 1)));
                REQUIRE(t->realValue() == LX(0, 1));

                // ok
                t->setRealValue(LF(1, 0.5));
                REQUIRE(t->realValue() == LX(1, 0.5));
                REQUIRE_UI_FLOAT_PROPERTY(t, "radius", 1);
                REQUIRE_UI_FLOAT_PROPERTY(t, "angle", 0.5);
            }

            SECTION("positive")
            {
                TestPolar t("ui.polar", LA("@positive", 1, "@radians", 1));
                REQUIRE(t->realValue() == LX(0, 0));

                // clip radius
                REQUIRE(t->setRealValue(LF(1.2, 1)));
                REQUIRE(t->realValue() == LX(1, 1));
                REQUIRE(t->setRealValue(LF(-0.1, 1)));
                REQUIRE(t->realValue() == LX(0, 1));

                // ok
                t->setRealValue(LF(1, 0.5));
                REQUIRE(t->realValue() == LX(1, 0.5));
                REQUIRE_UI_FLOAT_PROPERTY(t, "radius", 1);
                REQUIRE_UI_FLOAT_PROPERTY(t, "angle", 0.5);

                // angle wrap
                t->setRealValue(LF(1, 3.11));
                REQUIRE(t->realValue() == LX(1, 3.11));

                t->setRealValue(LF(1, 3.15));
                REQUIRE(t->realValue() == LX(1, 3.15));

                t->setRealValue(LF(1, 6.27));
                REQUIRE(t->realValue() == LX(1, 6.27));

                t->setRealValue(LF(1, 6.3));
                REQUIRE(t->realValue() == LX(1, 0.0168152).margin(0.000001));
            }
        }

        SECTION("degrees")
        {
            SECTION("default")
            {
                TestPolar t("ui.polar");
                REQUIRE(t->realValue() == LX(0, 0));

                // invalid
                REQUIRE_FALSE(t->setRealValue(L()));
                REQUIRE_FALSE(t->setRealValue(LF(100)));
                REQUIRE_FALSE(t->setRealValue(LA("A", 1)));
                REQUIRE_FALSE(t->setRealValue(LA(1, "B")));
                REQUIRE_FALSE(t->setRealValue(LA("A", "B")));

                // clip radius
                REQUIRE(t->setRealValue(LF(1.2, 1)));
                REQUIRE(t->realValue() == LX(1, 1));
                REQUIRE(t->setRealValue(LF(-0.1, 1)));
                REQUIRE(t->realValue() == LX(0, 1));

                // ok
                t->setRealValue(LF(1, 50));
                REQUIRE(t->realValue() == LX(1, 50));
                REQUIRE_UI_FLOAT_PROPERTY(t, "radius", 1);
                REQUIRE_UI_FLOAT_PROPERTY(t, "angle", 50);

                t->setRealValue(LF(1, -50));
                REQUIRE(t->realValue() == LX(1, -50));
                REQUIRE_UI_FLOAT_PROPERTY(t, "radius", 1);
                REQUIRE_UI_FLOAT_PROPERTY(t, "angle", -50);

                // angle wrap
                t->setRealValue(LF(1, 179));
                REQUIRE(t->realValue() == LX(1, 179));

                t->setRealValue(LF(1, 180));
                REQUIRE(t->realValue() == LX(1, -180));

                t->setRealValue(LF(1, 181));
                REQUIRE(t->realValue() == LX(1, -179));

                t->setRealValue(LF(1, 359));
                REQUIRE(t->realValue() == LX(1, -1));

                t->setRealValue(LF(1, 360));
                REQUIRE(t->realValue() == LX(1, 0));

                t->setRealValue(LF(1, 361));
                REQUIRE(t->realValue() == LX(1, 1));
            }

            SECTION("positive")
            {
                TestPolar t("ui.polar", LA("@positive", 1));
                REQUIRE(t->realValue() == LX(0, 0));

                // clip radius
                REQUIRE(t->setRealValue(LF(1.2, 1)));
                REQUIRE(t->realValue() == LX(1, 1));
                REQUIRE(t->setRealValue(LF(-0.1, 1)));
                REQUIRE(t->realValue() == LX(0, 1));

                // ok
                t->setRealValue(LF(1, 0.5));
                REQUIRE(t->realValue() == LX(1, 0.5));
                REQUIRE_UI_FLOAT_PROPERTY(t, "radius", 1);
                REQUIRE_UI_FLOAT_PROPERTY(t, "angle", 0.5);

                // angle wrap
                t->setRealValue(LF(1, 179));
                REQUIRE(t->realValue() == LX(1, 179));

                t->setRealValue(LF(1, 180));
                REQUIRE(t->realValue() == LX(1, 180));

                t->setRealValue(LF(1, 181));
                REQUIRE(t->realValue() == LX(1, 181));

                t->setRealValue(LF(1, 359));
                REQUIRE(t->realValue() == LX(1, 359));

                t->setRealValue(LF(1, 360));
                REQUIRE(t->realValue() == LX(1, 0));

                t->setRealValue(LF(1, 361));
                REQUIRE(t->realValue() == LX(1, 1));
            }
        }
    }

    SECTION("@radius")
    {
        TestExtPolar t("ui.polar");

        REQUIRE(t->realRadius() == Approx(0));

        // invalid
        t <<= LA("@radius");
        t <<= LA("@radius", "ABC");
        REQUIRE(t->realRadius() == Approx(0));

        // ok
        t <<= LA("@radius", 0.5);
        REQUIRE(t->realRadius() == Approx(0.5));

        // clip
        t <<= LA("@radius", 2);
        REQUIRE(t->realRadius() == Approx(1));

        t <<= LA("@radius", -99);
        REQUIRE(t->realRadius() == Approx(0));
    }

    SECTION("@angle")
    {
        SECTION("@degree")
        {
            TestExtPolar t("ui.polar");

            REQUIRE(t->realAngle() == Approx(0));

            // invalid
            t <<= LA("@angle");
            t <<= LA("@angle", "ABC");
            REQUIRE(t->realAngle() == Approx(0));

            // ok
            t <<= LA("@angle", 45);
            REQUIRE(t->realAngle() == Approx(45));

            // clip
            t <<= LA("@angle", 361);
            REQUIRE(t->realAngle() == Approx(1));

            t <<= LA("@angle", 270);
            REQUIRE(t->realAngle() == Approx(-90));
        }
    }

    SECTION("methods")
    {
        SECTION("set")
        {
            SECTION("default")
            {
                TestPolar t("ui.polar", LA("@direction", "E"));
                REQUIRE(t->realValue() == LX(0, 0));

                // invalid
                t.call("set", L());
                t.call("set", LF(1));
                t.call("set", LA("A"));
                t.call("set", LA("A", "B"));
                t.call("set", LA(1, "B"));
                REQUIRE(t->realValue() == LX(0, 0));

                // ok
                t.call("set", LA(1, 2));
                REQUIRE(t->realValue() == LX(1, 2));
            }

            SECTION("radians")
            {
                TestPolar t("ui.polar", LA("@radians", 1, "@direction", "E"));
                REQUIRE(t->realValue() == LX(0, 0));

                // invalid
                t.call("set", L());
                t.call("set", LF(1));
                t.call("set", LA("A"));
                t.call("set", LA("A", "B"));
                t.call("set", LA(1, "B"));
                REQUIRE(t->realValue() == LX(0, 0));

                // ok
                t.call("set", LA(1, 2));
                REQUIRE(t->realValue() == LX(1, 2));
            }

            SECTION("degrees")
            {
                TestPolar t("ui.polar", LA("@direction", "E"));
                REQUIRE(t->realValue() == LX(0, 0));

                // invalid
                t.call("set", L());
                t.call("set", LF(1));
                t.call("set", LA("A"));
                t.call("set", LA("A", "B"));
                t.call("set", LA(1, "B"));
                REQUIRE(t->realValue() == LX(0, 0));

                // ok
                t.call("set", LA(1, 90));
                REQUIRE(t->realValue() == LX(1, 90));
            }
        }

        SECTION("polar")
        {
            SECTION("degrees")
            {
                TestPolar t("ui.polar", LA("@direction", "E"));
                REQUIRE(t->realValue() == LX(0, 0));

                // invalid
                t.call("polar", L());
                t.call("polar", LF(1));
                t.call("polar", LA("A"));
                t.call("polar", LA("A", "B"));
                t.call("polar", LA(1, "B"));
                REQUIRE(t->realValue() == LX(0, 0));

                // ok
                t.call("polar", LA(1, 90));
                REQUIRE(t->realValue() == LX(1, 90));
            }
        }

        SECTION("cartesian")
        {
            SECTION("degrees")
            {
                TestPolar t("ui.polar", LA("@direction", "E", "@clockwise", 0.f));
                REQUIRE(t->realValue() == LX(0, 0));

                // invalid
                t.call("cartesian", L());
                t.call("cartesian", LF(1));
                t.call("cartesian", LA("A"));
                t.call("cartesian", LA("A", "B"));
                t.call("cartesian", LA(1, "B"));
                REQUIRE(t->realValue() == LX(0, 0));

                // ok
                t.call("cartesian", LF(0, 0));
                REQUIRE(t->realValue() == LX(0, 0));
                t.call("cartesian", LF(0, 1));
                REQUIRE(t->realValue() == LX(1, 90));
                t.call("cartesian", LF(0, -1));
                REQUIRE(t->realValue() == LX(1, -90));
                t.call("cartesian", LF(1, 0));
                REQUIRE(t->realValue() == LX(1, 0));
                t.call("cartesian", LF(-1, 0));
                REQUIRE(t->realValue() == LX(1, -180));

                // clip x-value
                t.call("cartesian", LF(1.1, 0));
                REQUIRE(t->realValue() == LX(1, 0));
                t.call("cartesian", LF(-2, 0));
                REQUIRE(t->realValue() == LX(1, -180));

                // clip y-value
                t.call("cartesian", LF(0, 1.1));
                REQUIRE(t->realValue() == LX(1, 90));
                t.call("cartesian", LF(0, -1.1));
                REQUIRE(t->realValue() == LX(1, -90));
            }
        }

        SECTION("rotate")
        {
            SECTION("radian")
            {
                TestPolar t("ui.polar", LA("@radians", 1));
                REQUIRE(t->realValue() == LAX(0., 0.));

                t.call("rotate", 0);
                REQUIRE(t->realValue() == LAX(0., 0.));
                t.call("rotate", 1);
                REQUIRE(t->realValue() == LAX(0., 1));
                t.call("rotate", 1);
                REQUIRE(t->realValue() == LAX(0., 2));
                t.call("rotate", 1);
                REQUIRE(t->realValue() == LAX(0., 3));
                t.call("rotate", 1);
                REQUIRE(t->realValue() == LAX(0., -2.28319));
            }

            SECTION("degree")
            {
                TestPolar t("ui.polar");
                REQUIRE(t->realValue() == LX(0, 0));

                t.call("rotate", 0);
                REQUIRE(t->realValue() == LX(0, 0));
                t.call("rotate", 60);
                REQUIRE(t->realValue() == LX(0, 60));
                t.call("rotate", 60);
                REQUIRE(t->realValue() == LX(0, 120));
                t.call("rotate", 60);
                REQUIRE(t->realValue() == LX(0, -180));
                t.call("rotate", 60);
                REQUIRE(t->realValue() == LX(0, -120));
                t.call("rotate", 60);
                REQUIRE(t->realValue() == LX(0, -60));
                t.call("rotate", 60);
                REQUIRE(t->realValue() == LX(0, 0));
                t.call("rotate", -60);
                REQUIRE(t->realValue() == LX(0, -60));
            }
        }
    }

    SECTION("mouse")
    {
        SECTION("radians")
        {
            TestExtPolar t("ui.polar", LA("@radians", 1));
            t.mouseDown(50, 0);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0).margin(0.00001));
            t.mouseDown(0, 50);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -M_PI_2));
            t.mouseDown(50, 50);
            REQUIRE_OUTPUT_LIST(t, 0, LX(0, M_PI_2));
            t.mouseDown(100, 50);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, M_PI_2));
            t.mouseDown(50, 100);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -M_PI));

            t.mouseUp(0, 50);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -M_PI_2));

            t.mouseDrag(50, 100);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -M_PI));
            t.mouseDrag(5000, 100000);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 3.09211));
        }

        SECTION("degrees")
        {
            TestExtPolar t("ui.polar");
            t.mouseDown(0, 0);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -45));
            t.mouseDown(50, 0);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
            t.mouseDown(100, 0);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 45));
            t.mouseDown(0, 50);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
            t.mouseDown(50, 50);
            REQUIRE_OUTPUT_LIST(t, 0, LX(0, 90));
            t.mouseDown(100, 50);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
            t.mouseDown(0, 100);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -135));
            t.mouseDown(50, 100);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
            t.mouseDown(100, 100);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 135));

            t.mouseUp(0, 50);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));

            t.mouseDrag(50, 100);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
            t.mouseDrag(5000, 100000);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 177.165));
        }

        SECTION("clockwise")
        {

            SECTION("direction")
            {
                SECTION("N")
                {
                    TestExtPolar t("ui.polar", LA("@direction", "N", "@clockwise", 1));
                    t.mouseDown(0, 0);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -45));
                    t.mouseDown(50, 0);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
                    t.mouseDown(100, 0);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 45));
                    t.mouseDown(0, 50);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
                    t.mouseDown(50, 50);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(0, 90));
                    t.mouseDown(100, 50);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
                    t.mouseDown(0, 100);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -135));
                    t.mouseDown(50, 100);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
                    t.mouseDown(100, 100);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 135));
                }

                SECTION("E")
                {
                    TestExtPolar t("ui.polar", LA("@direction", "E", "@clockwise", 1));
                    t.mouseDown(0, 0);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -135));
                    t.mouseDown(50, 0);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
                    t.mouseDown(100, 0);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -45));
                    t.mouseDown(0, 50);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
                    t.mouseDown(50, 50);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(0, 0));
                    t.mouseDown(100, 50);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
                    t.mouseDown(0, 100);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 135));
                    t.mouseDown(50, 100);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
                    t.mouseDown(100, 100);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 45));
                }

                SECTION("S")
                {
                    TestExtPolar t("ui.polar", LA("@direction", "S", "@clockwise", 1));
                    t.mouseDown(0, 0);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 135));
                    t.mouseDown(50, 0);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
                    t.mouseDown(100, 0);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -135));
                    t.mouseDown(0, 50);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
                    t.mouseDown(50, 50);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(0, -90));
                    t.mouseDown(100, 50);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
                    t.mouseDown(0, 100);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 45));
                    t.mouseDown(50, 100);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
                    t.mouseDown(100, 100);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -45));
                }

                SECTION("W")
                {
                    TestExtPolar t("ui.polar", LA("@direction", "W", "@clockwise", 1));
                    t.mouseDown(0, 0);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 45));
                    t.mouseDown(50, 0);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
                    t.mouseDown(100, 0);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 135));
                    t.mouseDown(0, 50);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
                    t.mouseDown(50, 50);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(0, -180));
                    t.mouseDown(100, 50);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
                    t.mouseDown(0, 100);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -45));
                    t.mouseDown(50, 100);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
                    t.mouseDown(100, 100);
                    REQUIRE_OUTPUT_LIST(t, 0, LX(1, -135));
                }
            }
        }
    }

    SECTION("counter clockwise")
    {
        SECTION("direction")
        {
            SECTION("N")
            {
                TestExtPolar t("ui.polar", LA("@direction", "N", "@clockwise", 0.f));
                t.mouseDown(0, 0);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 45));
                t.mouseDown(50, 0);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
                t.mouseDown(100, 0);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -45));
                t.mouseDown(0, 50);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
                t.mouseDown(50, 50);
                REQUIRE_OUTPUT_LIST(t, 0, LX(0, -90));
                t.mouseDown(100, 50);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
                t.mouseDown(0, 100);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 135));
                t.mouseDown(50, 100);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
                t.mouseDown(100, 100);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -135));
            }

            SECTION("E")
            {
                TestExtPolar t("ui.polar", LA("@direction", "E", "@clockwise", 0.f));
                t.mouseDown(0, 0);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 135));
                t.mouseDown(50, 0);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
                t.mouseDown(100, 0);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 45));
                t.mouseDown(0, 50);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
                t.mouseDown(50, 50);
                REQUIRE_OUTPUT_LIST(t, 0, LX(0, 0));
                t.mouseDown(100, 50);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
                t.mouseDown(0, 100);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -135));
                t.mouseDown(50, 100);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
                t.mouseDown(100, 100);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -45));
            }

            SECTION("S")
            {
                TestExtPolar t("ui.polar", LA("@direction", "S", "@clockwise", 0.f));
                t.mouseDown(0, 0);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -135));
                t.mouseDown(50, 0);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
                t.mouseDown(100, 0);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 135));
                t.mouseDown(0, 50);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
                t.mouseDown(50, 50);
                REQUIRE_OUTPUT_LIST(t, 0, LX(0, 90));
                t.mouseDown(100, 50);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
                t.mouseDown(0, 100);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -45));
                t.mouseDown(50, 100);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
                t.mouseDown(100, 100);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 45));
            }

            SECTION("W")
            {
                TestExtPolar t("ui.polar", LA("@direction", "W", "@clockwise", 0.f));
                t.mouseDown(0, 0);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -45));
                t.mouseDown(50, 0);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
                t.mouseDown(100, 0);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -135));
                t.mouseDown(0, 50);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
                t.mouseDown(50, 50);
                REQUIRE_OUTPUT_LIST(t, 0, LX(0, -180));
                t.mouseDown(100, 50);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
                t.mouseDown(0, 100);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 45));
                t.mouseDown(50, 100);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
                t.mouseDown(100, 100);
                REQUIRE_OUTPUT_LIST(t, 0, LX(1, 135));
            }
        }
    }

    SECTION("popup")
    {
        SECTION("N clockwise")
        {
            TestExtPolar t("ui.polar");
            t.mouseUp(0, 50);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));

            t->onPopup(SYM("unknown"), 1, {});
            REQUIRE_NO_OUTPUT(t);

            t->onPopup(SYM("polar"), 0, {});
            REQUIRE_OUTPUT_LIST(t, 0, LX(0, 0));
            t->onPopup(SYM("polar"), 1, {});
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
            t->onPopup(SYM("polar"), 2, {});
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
            t->onPopup(SYM("polar"), 3, {});
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
            t->onPopup(SYM("polar"), 4, {});
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));

            // invalid indexes
            t->onPopup(SYM("polar"), -1, {});
            REQUIRE_NO_OUTPUT(t);
            t->onPopup(SYM("polar"), 5, {});
            REQUIRE_NO_OUTPUT(t);
        }

        SECTION("N")
        {
            TestExtPolar t("ui.polar", LA("@direction", "N", "@clockwise", 0.f));

            t->onPopup(SYM("polar"), 0, {}); // center
            REQUIRE_OUTPUT_LIST(t, 0, LX(0, 0));
            t->onPopup(SYM("polar"), 1, {}); // left
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
            t->onPopup(SYM("polar"), 2, {}); // right
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
            t->onPopup(SYM("polar"), 3, {}); // top
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
            t->onPopup(SYM("polar"), 4, {}); // bottom
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
        }

        SECTION("E")
        {
            TestExtPolar t("ui.polar", LA("@direction", "E", "@clockwise", 0.f));

            t->onPopup(SYM("polar"), 0, {}); // center
            REQUIRE_OUTPUT_LIST(t, 0, LX(0, 0));
            t->onPopup(SYM("polar"), 1, {}); // left
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
            t->onPopup(SYM("polar"), 2, {}); // right
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
            t->onPopup(SYM("polar"), 3, {}); // top
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
            t->onPopup(SYM("polar"), 4, {}); // bottom
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
        }

        SECTION("E clockwise")
        {
            TestExtPolar t("ui.polar", LA("@direction", "E", "@clockwise", 1));

            t->onPopup(SYM("polar"), 0, {}); // center
            REQUIRE_OUTPUT_LIST(t, 0, LX(0, 0));
            t->onPopup(SYM("polar"), 1, {}); // left
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
            t->onPopup(SYM("polar"), 2, {}); // right
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
            t->onPopup(SYM("polar"), 3, {}); // top
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
            t->onPopup(SYM("polar"), 4, {}); // bottom
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
        }

        SECTION("S")
        {
            TestExtPolar t("ui.polar", LA("@direction", "S", "@clockwise", 0.f));

            t->onPopup(SYM("polar"), 0, {}); // center
            REQUIRE_OUTPUT_LIST(t, 0, LX(0, 0));
            t->onPopup(SYM("polar"), 1, {}); // left
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
            t->onPopup(SYM("polar"), 2, {}); // right
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
            t->onPopup(SYM("polar"), 3, {}); // top
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
            t->onPopup(SYM("polar"), 4, {}); // bottom
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
        }

        SECTION("S clockwise")
        {
            TestExtPolar t("ui.polar", LA("@direction", "S", "@clockwise", 1));

            t->onPopup(SYM("polar"), 0, {}); // center
            REQUIRE_OUTPUT_LIST(t, 0, LX(0, 0));
            t->onPopup(SYM("polar"), 1, {}); // left
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
            t->onPopup(SYM("polar"), 2, {}); // right
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
            t->onPopup(SYM("polar"), 3, {}); // top
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
            t->onPopup(SYM("polar"), 4, {}); // bottom
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
        }

        SECTION("W")
        {
            TestExtPolar t("ui.polar", LA("@direction", "W", "@clockwise", 0.f));

            t->onPopup(SYM("polar"), 0, {}); // center
            REQUIRE_OUTPUT_LIST(t, 0, LX(0, 0));
            t->onPopup(SYM("polar"), 1, {}); // left
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
            t->onPopup(SYM("polar"), 2, {}); // right
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
            t->onPopup(SYM("polar"), 3, {}); // top
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
            t->onPopup(SYM("polar"), 4, {}); // bottom
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
        }

        SECTION("W clockwise")
        {
            TestExtPolar t("ui.polar", LA("@direction", "W", "@clockwise", 1));

            t->onPopup(SYM("polar"), 0, {}); // center
            REQUIRE_OUTPUT_LIST(t, 0, LX(0, 0));
            t->onPopup(SYM("polar"), 1, {}); // left
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0));
            t->onPopup(SYM("polar"), 2, {}); // right
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -180));
            t->onPopup(SYM("polar"), 3, {}); // top
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
            t->onPopup(SYM("polar"), 4, {}); // bottom
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, -90));
        }
    }

    SECTION("paint")
    {
        TestExtPolar t("ui.polar");
        REQUIRE(t->asEBox());
        REQUIRE(t->asEObj());
        REQUIRE(t->asGObj());
        REQUIRE(t->asPd());
        REQUIRE(t->asPdObject());
        REQUIRE(t->canvas());
        t.show();
    }

    SECTION("list")
    {
        SECTION("radian")
        {
            TestExtPolar t("ui.polar", LA("@radians", 1));
            REQUIRE(t->realValue() == LX(0, 0));

            // invalid list
            t << L();
            REQUIRE_NO_OUTPUT(t);
            t << LF(1);
            REQUIRE_NO_OUTPUT(t);
            t << LA("A");
            REQUIRE_NO_OUTPUT(t);
            t << LA("A", 1);
            REQUIRE_NO_OUTPUT(t);
            t << LA(2, "A");
            REQUIRE_NO_OUTPUT(t);
            t << LF(1, 2, 3);
            REQUIRE_NO_OUTPUT(t);

            t << LF(1, M_PI * 2);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0.));
        }

        SECTION("degree")
        {
            TestExtPolar t("ui.polar");
            REQUIRE(t->realValue() == LX(0, 0));

            // invalid list
            t << L();
            REQUIRE_NO_OUTPUT(t);
            t << LF(1);
            REQUIRE_NO_OUTPUT(t);
            t << LA("A");
            REQUIRE_NO_OUTPUT(t);
            t << LA("A", 1);
            REQUIRE_NO_OUTPUT(t);
            t << LA(2, "A");
            REQUIRE_NO_OUTPUT(t);
            t << LF(1, 2, 3);
            REQUIRE_NO_OUTPUT(t);

            t << LF(1, 90);
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 90));
        }
    }

    SECTION("send")
    {
        TestExtPolar t("ui.polar", LA("@send", "r1", "@radians", 1));
        t.addListener("r1");

        t << BANG;
        REQUIRE_LIST_WAS_SEND(t, "r1", LAX(0., 0.));

        t << LA(0.4, -0.6);
        REQUIRE_LIST_WAS_SEND(t, "r1", LAX(0.4, -0.6));

        t.mouseDown(30, 70);
        REQUIRE_LIST_WAS_SEND(t, "r1", LAX(0.565685, -2.35619));
    }

    PresetStorage::instance().clearAll();
}
