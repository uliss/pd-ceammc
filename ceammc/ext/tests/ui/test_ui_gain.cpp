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
#include "ui_gain.h"

#include "test_ui_base.h"

UI_COMPLETE_TEST_SETUP(Gain)

TEST_CASE("ui.gain~", "[ui.gain~]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestGain t("ui.gain~");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "min", -60);
        REQUIRE_UI_FLOAT_PROPERTY(t, "db", -60);
        REQUIRE_UI_FLOAT_PROPERTY(t, "amp", 0.f);
        REQUIRE_UI_FLOAT_PROPERTY(t, "output_value", 0);
        REQUIRE(t->presetId() == gensym("ui.gain~.0"));

        SECTION("props")
        {
            TestGain t("ui.gain~", LA("@min", -90, "@max", 6));
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 6);
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", -90);
        }
    }

    SECTION("db")
    {
        TestGain t("ui.gain~");
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "min", -60);
        t->setDbValue(-20);
        REQUIRE(t->dbValue() == Approx(-20));
        t->setDbValue(-60);
        REQUIRE(t->dbValue() == Approx(-60));
        t->setDbValue(-61);
        REQUIRE(t->dbValue() == Approx(-60));
        t->setDbValue(0);
        REQUIRE(t->dbValue() == Approx(0));
        t->setDbValue(1);
        REQUIRE(t->dbValue() == Approx(0));
    }

    SECTION("amp")
    {
        TestGain t("ui.gain~", LA("@max", 6, "@min", -70));
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", 6);
        REQUIRE_UI_FLOAT_PROPERTY(t, "min", -70);
        t->setDbValue(-6.0206f);
        REQUIRE(t->ampValue() == Approx(0.5));
        t->setDbValue(-12.041);
        REQUIRE(t->ampValue() == Approx(0.25001f).epsilon(0.01));
        t->setAmpValue(1);
        REQUIRE(t->dbValue() == Approx(0.0).margin(0.00001));
        t->setAmpValue(0.5);
        REQUIRE(t->dbValue() == Approx(-6.0206));
        t->setAmpValue(0);
        REQUIRE(t->dbValue() == Approx(-70));
    }

    SECTION("presets")
    {
        TestGain t("ui.gain~");

        t->setDbValue(-20);
        t->storePreset(0);
        t->setDbValue(-30);
        t->storePreset(1);
        t->setDbValue(0);
        t->storePreset(2);

        t->loadPreset(3);
        REQUIRE(t->dbValue() == Approx(-60));
        t->loadPreset(2);
        REQUIRE(t->dbValue() == Approx(0));
        t->loadPreset(1);
        REQUIRE(t->dbValue() == Approx(-30));
        t->loadPreset(0);
        REQUIRE(t->dbValue() == Approx(-20));

        t->loadPreset(6);
        REQUIRE(t->dbValue() == Approx(-60));
    }

    SECTION("external")
    {
        // just create test
        TestExtGain t("ui.gain~");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 1);

        SECTION("bang")
        {
            t.bang();
            REQUIRE_OUTPUT_ANY(t, 0, LA("@db", -60));

            t.call("@db", -20);
            REQUIRE_NO_OUTPUT(t);

            t.bang();
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputAnyAt(0));
            REQUIRE(t.outputAnyAt(0).slice(1) == LX(-20));
            t.clearAt(0);
        }
    }

    SECTION("methods")
    {
        TestExtGain t("ui.gain~");
        t.call("++");
        REQUIRE(t->dbValue() == Approx(-59));
        t.call("+", 40);
        REQUIRE(t->dbValue() == Approx(-19));
        t.call("+", 40);
        REQUIRE(t->dbValue() == Approx(0));
        t.call("--");
        REQUIRE(t->dbValue() == Approx(-1));
        t.call("-", 50);
        REQUIRE(t->dbValue() == Approx(-51));
        t.call("-", 50);
        REQUIRE(t->dbValue() == Approx(-60));

        t.call("set", -30);
        REQUIRE(t->dbValue() == Approx(-30));
    }

    SECTION("send")
    {
        TestExtGain t("ui.gain~", LA("@send", "r1"));
        t.addListener("r1");

        t << BANG;
        REQUIRE_FLOAT_WAS_SEND(t, "r1", -60);

        t.mouseDown(5, 20);
        REQUIRE_NONE_WAS_SEND(t, "r1");
        t.mouseDrag(4, 18);
        REQUIRE_FLOAT_WAS_SEND(t, "r1", -59);

        t->setProperty(gensym("relative"), LF(0));
        t.mouseDown(5, 10);
        REQUIRE_FLOAT_WAS_SEND(t, "r1", -5);
    }
}
