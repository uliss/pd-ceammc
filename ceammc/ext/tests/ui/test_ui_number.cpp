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
#include "ui_number.h"
#include "test_ui.h"

UI_COMPLETE_TEST_SETUP(Number)

TEST_CASE("ui.number", "[ui.number]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestNumber t("ui.number");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "min", -std::numeric_limits<float>::max());
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", std::numeric_limits<float>::max());
        REQUIRE_UI_LIST_PROPERTY(t, "size", LF(53, 15));
        REQUIRE_UI_FLOAT_PROPERTY(t, "value", 0.f);
        REQUIRE(t->value() == 0.f);
    }

    SECTION("presets")
    {
        TestExtNumber t("ui.number");
        REQUIRE(t->presetId()->s_name == std::string("ui.number.0"));

        t->setValue(100);
        REQUIRE_NO_OUTPUT(t);
        t->storePreset(0);
        t->setValue(200);
        t->storePreset(1);
        t->setValue(-1200);
        t->storePreset(2);

        t <<= LA("load");
        REQUIRE_OUTPUT_FLOAT(t, 0, 100.f);
        t <<= LA("load", 1);
        REQUIRE_OUTPUT_FLOAT(t, 0, 200.f);
        t <<= LA("load", 0.f);
        REQUIRE_OUTPUT_FLOAT(t, 0, 100.f);
        t <<= LA("load", 2);
        REQUIRE_OUTPUT_FLOAT(t, 0, -1200.f);
        t <<= LA("load", 4);
        REQUIRE_OUTPUT_FLOAT(t, 0, 0.f);
    }

    SECTION("external")
    {
        TestExtNumber t("ui.number");

        t << BANG;
        REQUIRE_OUTPUT_FLOAT(t, 0, 0);
        t << 100.f;
        REQUIRE_OUTPUT_FLOAT(t, 0, 100.f);
        t << -0.11122f;
        REQUIRE_OUTPUT_FLOAT(t, 0, -0.11122f);

        t << "ABC";
        REQUIRE_NO_OUTPUT(t);
        t << LF(1, 2, 3);
        REQUIRE_OUTPUT_FLOAT(t, 0, 1.f);
        t.call("any", 1);
        REQUIRE_NO_OUTPUT(t);

        t << $1("@min", -10);
        t << -100;
        REQUIRE_OUTPUT_FLOAT(t, 0, -10.f);
        t << -11;
        REQUIRE_OUTPUT_FLOAT(t, 0, -10.f);
        t << -10;
        REQUIRE_OUTPUT_FLOAT(t, 0, -10.f);
        t << -9.9999f;
        REQUIRE_OUTPUT_FLOAT(t, 0, -9.9999f);

        t << $1("@max", 100);
        t << 1000;
        REQUIRE_OUTPUT_FLOAT(t, 0, 100.f);
        t << 101;
        REQUIRE_OUTPUT_FLOAT(t, 0, 100.f);
        t << 100;
        REQUIRE_OUTPUT_FLOAT(t, 0, 100.f);
        t << 99.999f;
        REQUIRE_OUTPUT_FLOAT(t, 0, 99.999f);

        REQUIRE((t >> "@max") == LF(100));
        REQUIRE((t >> "@min") == LF(-10));

        t << 10;
        REQUIRE((t >> "@value") == LF(10));

        t <<= LA("++");
        REQUIRE(t->value() == 11.f);
        REQUIRE_NO_OUTPUT(t);
        t << 99 <<= LA("++");
        REQUIRE(t->value() == 100.f);
        t <<= LA("++");
        REQUIRE(t->value() == 100.f);
        t <<= LA("--");
        REQUIRE(t->value() == 99.f);
        t <<= LA("--");
        REQUIRE(t->value() == 98.f);
        t <<= LA("/", 2);
        REQUIRE(t->value() == 49.f);
        t <<= LA("-", 29);
        t <<= LA("*", 2);
        REQUIRE(t->value() == 40.f);
        t <<= LA("+", 10);
        REQUIRE(t->value() == 50.f);
        t <<= LA("/", 0.f);
        REQUIRE(t->value() == 50.f);

        t <<= LA("set", -5);
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->value() == -5.f);
    }

    SECTION("keys")
    {
        TestExtNumber t("ui.number");

        t->onKey('z', 0);
        REQUIRE_NO_OUTPUT(t);

        t->onKey('-', 0);
        REQUIRE_OUTPUT_FLOAT(t, 0, -1);
        t->onKey('-', 0);
        REQUIRE_OUTPUT_FLOAT(t, 0, -2);
        t->onKey('+', 0);
        REQUIRE_OUTPUT_FLOAT(t, 0, -1);
        t->onKey('+', 0);
        REQUIRE_OUTPUT_FLOAT(t, 0, 0);
    }

    SECTION("mouse")
    {
        TestExtNumber t("ui.number");

        t.mouseDown(5, 5);
        REQUIRE_NO_OUTPUT(t);

        t.mouseDrag(10, 3);
        REQUIRE_OUTPUT_FLOAT(t, 0, 2.f);
        t.mouseDrag(10, 30);
        REQUIRE_OUTPUT_FLOAT(t, 0, -25.f);
        t.mouseDrag(10, 20);
        REQUIRE_OUTPUT_FLOAT(t, 0, -15.f);
        t.mouseDrag(10, 10);
        REQUIRE_OUTPUT_FLOAT(t, 0, -5.f);
        t.mouseDrag(10, 6);
        REQUIRE_OUTPUT_FLOAT(t, 0, -1.f);
        t.mouseDrag(10, 5);
        REQUIRE_OUTPUT_FLOAT(t, 0, 0.f);

        t.doubleClick(5, 6);
        REQUIRE_NO_OUTPUT(t);
        t->onKey('1', 0);
        REQUIRE_NO_OUTPUT(t);
        t->onKey('2', 0);
        t->onKey('.', 0);
        t->onKey('9', 0);
        t->onKeyFilter(EKEY_ENTER, 0);
        REQUIRE_OUTPUT_FLOAT(t, 0, 12.9f);

        t.doubleClick(5, 6);
        t->onKey('-', 0);
        t->onKey('2', 0);
        t->onKey('.', 0);
        t->onKey('9', 0);
        t->onKeyFilter(EKEY_ENTER, 0);
        REQUIRE_OUTPUT_FLOAT(t, 0, -2.9f);

        t.doubleClick(5, 6);
        t->onKey('.', 0);
        t->onKey('9', 0);
        t->onKeyFilter(EKEY_ENTER, 0);
        REQUIRE_OUTPUT_FLOAT(t, 0, 0.9f);

        t.doubleClick(5, 6);
        t->onKey('2', 0);
        t->onKeyFilter(EKEY_ESC, 0);
        REQUIRE_NO_OUTPUT(t);

        t.doubleClick(5, 6);
        t->onKey('2', 0);
        t->onKeyFilter(EKEY_DEL, 0);
        t->onKey('5', 0);
        t->onKeyFilter(EKEY_ENTER, 0);
        REQUIRE_OUTPUT_FLOAT(t, 0, 5.f);
    }

    SECTION("send")
    {
        TestExtNumber t("ui.number", LA("@send", "r1"));
        t.addListener("r1");

        t << 4;
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 4);
        t << 10;
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 10);
        t << -10;
        REQUIRE_FLOAT_WAS_SEND(t, "r1", -10);
    }

    SECTION("alias")
    {
        TestExtNumber t("ui.n");
    }
}
