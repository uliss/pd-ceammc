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
#include "ui_sliders.h"

#include "test_ui.h"

UI_COMPLETE_TEST_SETUP(Sliders)

TEST_CASE("ui.sliders", "[ui.sliders]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestSliders t("ui.sliders");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "count", 8);
        REQUIRE_UI_FLOAT_PROPERTY(t, "min", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "range", 1);
        REQUIRE_UI_LIST_PROPERTY(t, "auto_range_mode", LA("none"));
        REQUIRE_UI_FLOAT_PROPERTY(t, "auto_count", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "show_range", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "show_range", 0);
        REQUIRE_UI_LIST_PROPERTY(t, "size", LF(150, 100));
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::zeroes(8));
        REQUIRE(t->realValues() == AtomList::zeroes(8));

        HAS_PROPERTY(t, "min");
        HAS_PROPERTY(t, "max");
        HAS_PROPERTY(t, "size");
        HAS_PROPERTY(t, "presetname");
        HAS_PROPERTY(t, "background_color");
        HAS_PROPERTY(t, "border_color");
        HAS_PROPERTY(t, "slider_color");
        HAS_PROPERTY(t, "select_color");
        HAS_PROPERTY(t, "show_range");
        HAS_PROPERTY(t, "pinned");
        HAS_PROPERTY(t, "send");
        HAS_PROPERTY(t, "receive");

        SECTION("with props")
        {
            TestSliders t("ui.sliders", LA("@count", 16, "@min", -10, "@max", 84));
            REQUIRE(t->numOutlets() == 1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "count", 16);
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", -10);
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 84);
        }
    }

    SECTION("@count")
    {
#define SET_COUNT(t, n) t->setProperty(gensym("count"), LF(float(n)))
#define REQUIRE_COUNT(t, n) REQUIRE_UI_FLOAT_PROPERTY(t, "count", float(n))

        TestSliders t("ui.sliders");
        REQUIRE_COUNT(t, 8);
        SET_COUNT(t, 1);
        REQUIRE_COUNT(t, 1);
        SET_COUNT(t, 0);
        REQUIRE_COUNT(t, 1);
        SET_COUNT(t, -1);
        REQUIRE_COUNT(t, 1);
        SET_COUNT(t, 1023);
        REQUIRE_COUNT(t, 1023);
        SET_COUNT(t, 1024);
        REQUIRE_COUNT(t, 1024);
        SET_COUNT(t, 1025);
        REQUIRE_COUNT(t, 1024);

#undef SET_COUNT
#undef REQUIRE_COUNT
    }

    SECTION("onList")
    {
        SECTION("no auto range")
        {
            TestSliders t("ui.sliders");
            REQUIRE(t->realValues() == AtomList::zeroes(8));
            t->onList(LF(0.1, 0.2, 0.3, 0.4, 2));
            REQUIRE(t->realValues() == LF(0.1, 0.2, 0.3, 0.4, 1, 0, 0, 0));
            t->onList(L());
            REQUIRE(t->realValues() == LF(0.1, 0.2, 0.3, 0.4, 1, 0, 0, 0));
            t->onList(LA(0.5, "A", 1, 1, 1));
            REQUIRE(t->realValues() == LF(0.5, 0, 1, 1, 1, 0, 0, 0));
        }

        SECTION("auto range")
        {
            TestSliders t("ui.sliders", LA("@auto_range_mode", "both", "@auto_count", 1));
            REQUIRE(t->realValues() == AtomList::zeroes(8));
            t->onList(AtomList { 1, 2, 3, 4, 5 });
            REQUIRE(t->realValues() == LF(1, 2, 3, 4, 5));
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", 1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 5);
            t->onList(L());
            REQUIRE(t->realValues() == LF(1, 2, 3, 4, 5));
            t->onList(LF(1));
            REQUIRE(t->realValues() == LF(1, 2, 3, 4, 5));
            t->onList(LF(-1, 2));
            REQUIRE(t->realValues() == LF(-1, 2));
            t->onList(LF(2, 2));
            REQUIRE(t->realValues() == LF(-1, 2));
        }
    }

    SECTION("set")
    {
        SECTION("no auto range")
        {
            TestSliders t("ui.sliders");
            REQUIRE(t->realValues() == AtomList::zeroes(8));
            t->m_set(LF(0.1, 0.2, 0.3, 0.4, 2));
            REQUIRE(t->realValues() == LF(0.1, 0.2, 0.3, 0.4, 1, 0, 0, 0));
            t->m_set(L());
            REQUIRE(t->realValues() == LF(0.1, 0.2, 0.3, 0.4, 1, 0, 0, 0));
            t->m_set(LA(0.5, "A", 1, 1, 1));
            REQUIRE(t->realValues() == LF(0.5, 0, 1, 1, 1, 0, 0, 0));
        }

        SECTION("auto range")
        {
            TestSliders t("ui.sliders", LA("@auto_range_mode", "both", "@auto_count", 1));
            REQUIRE(t->realValues() == AtomList::zeroes(8));
            t->m_set(LF(1, 2, 3, 4, 5));
            REQUIRE(t->realValues() == LF(1, 2, 3, 4, 5));
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", 1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 5);
            t->m_set(L());
            REQUIRE(t->realValues() == LF(1, 2, 3, 4, 5));
            t->m_set(LF(1));
            REQUIRE(t->realValues() == LF(1, 2, 3, 4, 5));
            t->m_set(LF(-1, 2));
            REQUIRE(t->realValues() == LF(-1, 2));
            t->m_set(LF(2, 2));
            REQUIRE(t->realValues() == LF(-1, 2));
        }

        SECTION("auto count")
        {
            TestSliders t("ui.sliders", LA("@auto_count", 1));
            REQUIRE(t->realValues() == AtomList::zeroes(8));
            t->m_set(LF(1, 2, 3, 4, 5));
            REQUIRE(t->realValues() == LF(1, 1, 1, 1, 1));
            t->m_set(L());
            REQUIRE(t->realValues() == LF(1, 1, 1, 1, 1));
            t->m_set(LF(1));
            REQUIRE(t->realValues() == LF(1));
            t->m_set(LF(0.5, 0.25));
            REQUIRE(t->realValues() == LF(0.5, 0.25));
        }

        SECTION("auto range max")
        {
            TestSliders t("ui.sliders", LA("@auto_range_mode", "max", "@count", 3));
            REQUIRE(t->realValues() == AtomList::zeroes(3));
            t->m_set(LF(-1, 2, 3, 4));
            REQUIRE(t->realValues() == LF(0, 2, 3));
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", 0);
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 3);
            t->m_set(L());
            REQUIRE(t->realValues() == LF(0, 2, 3));
        }

        SECTION("auto range min")
        {
            TestSliders t("ui.sliders", LA("@auto_range_mode", "min", "@count", 3));
            REQUIRE(t->realValues() == AtomList::zeroes(3));
            t->m_set(LF(-1, 2, 3, 4));
            REQUIRE(t->realValues() == LF(-1, 1, 1));
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", -1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 1);
            t->m_set(L());
            REQUIRE(t->realValues() == LF(-1, 1, 1));
        }
    }

    SECTION("presets")
    {
        TestSliders t("ui.sliders", LA("@min", 1, "@max", 12));
        t->m_set(LF(2, 3, 4, 5, 6, 7, 8, 9));
        t.call("store", LF(0));
        t->m_set(LF(12, 11, 10, 9, 8, 7, 6, 5));
        t.call("store", LF(1));
        t->m_set(AtomList::ones(8));
        t.call("store", LF(2));

        t.call("load", LF(0));
        REQUIRE(t->realValues() == LF(2, 3, 4, 5, 6, 7, 8, 9));
        t.call("load", LF(1));
        REQUIRE(t->realValues() == LF(12, 11, 10, 9, 8, 7, 6, 5));
        t.call("load", LF(2));
        REQUIRE(t->realValues() == AtomList::ones(8));
        t.call("load", LF(100));
        REQUIRE(t->realValues() == AtomList::ones(8));
        t.call("load");
        REQUIRE(t->realValues() == LF(2, 3, 4, 5, 6, 7, 8, 9));
    }

    SECTION("normalize values")
    {
        TestSliders t("ui.sliders", LA("@min", 1, "@max", 12));
        REQUIRE(t->realValues() == AtomList::ones(8));

        t->normalize();

        REQUIRE(t->realValues() == AtomList::ones(8));
        REQUIRE_UI_FLOAT_PROPERTY(t, "min", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", 12);

        t->m_set(LF(-1, 2, 3, 4, 5));
        REQUIRE(t->realValues() == LX(1, 2, 3, 4, 5, 1, 1, 1));
        t->normalize();
        REQUIRE(t->realValues() == LX(1, 2, 3, 4, 5, 1, 1, 1));
        REQUIRE_UI_FLOAT_PROPERTY(t, "min", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", 5);
    }

    SECTION("external")
    {
        SECTION("onBang")
        {
            TestExtSliders t("ui.sliders");
            REQUIRE(t->presetId()->s_name == std::string("ui.sliders.0"));

            t.bang();
            REQUIRE_OUTPUT_LIST(t, 0, AtomList::zeroes(8));

            t->m_set(LF(1, 2, 3));
            t.bang();
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 1, 1, 0, 0, 0, 0, 0));
        }

        SECTION("onList")
        {
            TestExtSliders t("ui.sliders");
            t.send(L());
            REQUIRE_NO_OUTPUT(t);

            t.send(LF(100));
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 0, 0, 0, 0, 0, 0, 0));

            t.send(LF(100, 100));
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 1, 0, 0, 0, 0, 0, 0));

            t.send(LF(0.125, 0.25, 0.375));
            REQUIRE_OUTPUT_LIST(t, 0, LX(0.125, 0.25, 0.375, 0, 0, 0, 0, 0));
        }

        SECTION("onList auto_range")
        {
            TestExtSliders t("ui.sliders", LA("@auto_range_mode", "both", "@auto_count", 1));

            t.send(L());
            REQUIRE_NO_OUTPUT(t);
            REQUIRE(t->realValues() == AtomList::zeroes(8));

            t.send(LF(100));
            REQUIRE_NO_OUTPUT(t);
            REQUIRE(t->realValues() == AtomList::zeroes(8));

            t.send(LF(100, 100));
            REQUIRE_NO_OUTPUT(t);
            REQUIRE(t->realValues() == AtomList::zeroes(8));

            t.send(LF(1, 2));
            REQUIRE_OUTPUT_LIST(t, 0, LX(1, 2));
            REQUIRE_UI_FLOAT_PROPERTY(t, "count", 2);
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", 1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 2);
        }

        SECTION("onList auto_count")
        {
            TestExtSliders t("ui.sliders", LA("@auto_count", 1));

            t.send(L());
            REQUIRE_NO_OUTPUT(t);
            REQUIRE(t->realValues() == AtomList::zeroes(8));

            t.send(LF(100));
            REQUIRE_OUTPUT_FLOAT(t, 0, 1);
            REQUIRE(t->realValues() == LF(1));

            t.send(LF(100, 100));
            REQUIRE_OUTPUT_LIST(t, 0, LF(1, 1));
            REQUIRE(t->realValues() == LF(1, 1));

            t.send(LF(0, 0.25, 0.5, 1));
            REQUIRE_OUTPUT_LIST(t, 0, LF(0, 0.25, 0.5, 1));
            REQUIRE_UI_FLOAT_PROPERTY(t, "count", 4);
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", 0);
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 1);
        }
    }

    SECTION("mouseDown")
    {
        TestExtSliders t("ui.sliders");
        REQUIRE_UI_LIST_PROPERTY(t, "size", LF(150, 100));

#define CLICK(t, x0, y0) t.mouseDown(x0, y0)

        CLICK(t, 10, 50);
        REQUIRE_OUTPUT_LIST(t, 0, LF(0.5) + AtomList::zeroes(7));

        CLICK(t, 20, 75);
        REQUIRE_OUTPUT_LIST(t, 0, LF(0.5f, 0.25f) + AtomList::zeroes(6));

        TestExtSliders t_vert("ui.sliders");
        t_vert->resize(100, 160);
        REQUIRE_UI_LIST_PROPERTY(t_vert, "size", LF(100, 160));

        CLICK(t_vert, 50, 10);
        REQUIRE_OUTPUT_LIST(t_vert, 0, LF(0.5) + AtomList::zeroes(7));

        CLICK(t_vert, 10, 150);
        REQUIRE_OUTPUT_LIST(t_vert, 0, LF(0.5) + AtomList::zeroes(6) + LF(0.1f));
    }

    SECTION("set sliders")
    {
        TestExtSliders t("ui.sliders");

        t.call("set", LA("slider"));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::zeroes(8));

        t.call("set", LA("slider", 6));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::zeroes(8));

        t.call("set", LA("slider", -1, 0.375));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::zeroes(8));

        t.call("set", LA("slider", 1, 0.375));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "value", LF(0, 0.375) + AtomList::zeroes(6));
    }

    SECTION("operator")
    {
        TestExtSliders t("ui.sliders", LA("@size", 128, 64));

        t.call("+", 0.125);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::filled(0.125, 8));
        t.call("+", 2);
        REQUIRE_UI_LIST_PROPERTY(t, "value", LX(1, 1, 1, 1, 1, 1, 1, 1));

        t.call("-", 0.125);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::filled(0.875, 8));
        t.call("-", 10);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::zeroes(8));

        t.call("*", 10);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::zeroes(8));
        t.call("+", 0.25);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::filled(0.25, 8));

        t.call("*", 2);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::filled(0.5, 8));
        t.call("*", 4);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::ones(8));

        t.call("/", 0);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::ones(8));
        t.call("/", 4);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::filled(0.25f, 8));

        t.call("++");
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::ones(8));
        t.call("--");
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::zeroes(8));
    }

    SECTION("get")
    {
        TestExtSliders t("ui.sliders");

        t.call("get", L());
        REQUIRE_NO_OUTPUT(t);

        t.call("get", LA("slider"));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", LA("slider", -2));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", LA("slider", 2));
        REQUIRE_OUTPUT_ANY(t, 0, LA("slider", 2, 0.f));

        t.call("set", LA("slider", 4, 0.6f));
        t.call("get", LA("slider", 4));
        REQUIRE_OUTPUT_ANY(t, 0, LA("slider", 4, 0.6f));
    }

    SECTION("send test")
    {
        TestExtSliders t("ui.sliders", LA("@send", "r1"));
        t.addListener("r1");

        t << BANG;
        REQUIRE_LIST_WAS_SEND(t, "r1", AtomList::zeroes(8));

        t << LF(0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1);
        REQUIRE_LIST_WAS_SEND(t, "r1", LX(0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1));

        t.call("fill", 0.5);
        t << BANG;
        REQUIRE_LIST_WAS_SEND(t, "r1", AtomList::filled(0.5, 8));

        t.mouseDown(10, 25);
        REQUIRE_LIST_WAS_SEND(t, "r1", LX(0.75, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5));
    }

    SECTION("alias")
    {
        TestExtSliders t("ui.vsliders");
        REQUIRE_UI_LIST_PROPERTY(t, "size", LF(100, 150));
    }
}
