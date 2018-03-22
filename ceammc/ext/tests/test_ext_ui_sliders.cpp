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
#include "../ui/ui_sliders.h"
#include "ui_external_test.h"

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
        REQUIRE_UI_FLOAT_PROPERTY(t, "auto_range", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "show_range", 1);
        REQUIRE_UI_LIST_PROPERTY(t, "size", L2(150, 100));
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
            TestSliders t("ui.sliders", L6("@count", 16, "@min", -10, "@max", 84));
            REQUIRE(t->numOutlets() == 1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "count", 16);
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", -10);
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 84);
        }
    }

    SECTION("@count")
    {
#define SET_COUNT(t, n) t->setProperty(gensym("count"), L1(float(n)))
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
            t->onList(L5(0.1, 0.2, 0.3, 0.4, 2));
            REQUIRE(t->realValues() == L8(0.1, 0.2, 0.3, 0.4, 1, 0.f, 0.f, 0.f));
            t->onList(AtomList());
            REQUIRE(t->realValues() == L8(0.1, 0.2, 0.3, 0.4, 1, 0.f, 0.f, 0.f));
            t->onList(L5(0.5, "A", 1, 1, 1));
            REQUIRE(t->realValues() == L8(0.5, 0.f, 1, 1, 1, 0.f, 0.f, 0.f));
        }

        SECTION("auto range")
        {
            TestSliders t("ui.sliders", L2("@auto_range", 1));
            REQUIRE(t->realValues() == AtomList::zeroes(8));
            t->onList(L5(1, 2, 3, 4, 5));
            REQUIRE(t->realValues() == L5(1, 2, 3, 4, 5));
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", 1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 5);
            t->onList(AtomList());
            REQUIRE(t->realValues() == L5(1, 2, 3, 4, 5));
            t->onList(L1(1));
            REQUIRE(t->realValues() == L5(1, 2, 3, 4, 5));
            t->onList(L2(-1, 2));
            REQUIRE(t->realValues() == L2(-1, 2));
            t->onList(L2(2, 2));
            REQUIRE(t->realValues() == L2(-1, 2));
        }
    }

    SECTION("set")
    {
        SECTION("no auto range")
        {
            TestSliders t("ui.sliders");
            REQUIRE(t->realValues() == AtomList::zeroes(8));
            t->m_set(L5(0.1, 0.2, 0.3, 0.4, 2));
            REQUIRE(t->realValues() == L8(0.1, 0.2, 0.3, 0.4, 1, 0.f, 0.f, 0.f));
            t->m_set(AtomList());
            REQUIRE(t->realValues() == L8(0.1, 0.2, 0.3, 0.4, 1, 0.f, 0.f, 0.f));
            t->m_set(L5(0.5, "A", 1, 1, 1));
            REQUIRE(t->realValues() == L8(0.5, 0.f, 1, 1, 1, 0.f, 0.f, 0.f));
        }

        SECTION("auto range")
        {
            TestSliders t("ui.sliders", L2("@auto_range", 1));
            REQUIRE(t->realValues() == AtomList::zeroes(8));
            t->m_set(L5(1, 2, 3, 4, 5));
            REQUIRE(t->realValues() == L5(1, 2, 3, 4, 5));
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", 1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 5);
            t->m_set(AtomList());
            REQUIRE(t->realValues() == L5(1, 2, 3, 4, 5));
            t->m_set(L1(1));
            REQUIRE(t->realValues() == L5(1, 2, 3, 4, 5));
            t->m_set(L2(-1, 2));
            REQUIRE(t->realValues() == L2(-1, 2));
            t->m_set(L2(2, 2));
            REQUIRE(t->realValues() == L2(-1, 2));
        }
    }

    SECTION("presets")
    {
        TestSliders t("ui.sliders", L4("@min", 1, "@max", 12));
        t->m_set(L8(2, 3, 4, 5, 6, 7, 8, 9));
        t.call("store", L1(0.f));
        t->m_set(L8(12, 11, 10, 9, 8, 7, 6, 5));
        t.call("store", L1(1));
        t->m_set(AtomList::ones(8));
        t.call("store", L1(2));

        t.call("load", L1(0.f));
        REQUIRE(t->realValues() == L8(2, 3, 4, 5, 6, 7, 8, 9));
        t.call("load", L1(1));
        REQUIRE(t->realValues() == L8(12, 11, 10, 9, 8, 7, 6, 5));
        t.call("load", L1(2));
        REQUIRE(t->realValues() == AtomList::ones(8));
        t.call("load", L1(100));
        REQUIRE(t->realValues() == AtomList::ones(8));
        t.call("load");
        REQUIRE(t->realValues() == L8(2, 3, 4, 5, 6, 7, 8, 9));
    }

    SECTION("normalize values")
    {
        TestSliders t("ui.sliders", L4("@min", 1, "@max", 12));
        REQUIRE(t->realValues() == AtomList::ones(8));

        t->normalize();

        REQUIRE(t->realValues() == AtomList::ones(8));
        REQUIRE_UI_FLOAT_PROPERTY(t, "min", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", 12);

        t->m_set(L5(-1, 2, 3, 4, 5));
        REQUIRE(t->realValues() == L8(1, 2, 3, 4, 5, 1, 1, 1));
        t->normalize();
        REQUIRE(t->realValues() == L8(1, 2, 3, 4, 5, 1, 1, 1));
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

            t->m_set(L3(1, 2, 3));
            t.bang();
            REQUIRE_OUTPUT_LIST(t, 0, L8(1, 1, 1, 0.f, 0.f, 0.f, 0.f, 0.f));
        }

        SECTION("onList")
        {
            TestExtSliders t("ui.sliders");
            t.send(AtomList());
            REQUIRE_NO_OUTPUT(t);

            t.send(L1(100));
            REQUIRE_OUTPUT_LIST(t, 0, L1(1) + AtomList::zeroes(7));

            t.send(L2(100, 100));
            REQUIRE_OUTPUT_LIST(t, 0, L2(1, 1) + AtomList::zeroes(6));

            t.send(L3(0.1, 0.2, 0.3));
            REQUIRE_OUTPUT_LIST(t, 0, L3(0.1, 0.2, 0.3) + AtomList::zeroes(5));
        }

        SECTION("onList auto_range")
        {
            TestExtSliders t("ui.sliders", L2("@auto_range", 1));

            t.send(AtomList());
            REQUIRE_NO_OUTPUT(t);
            REQUIRE(t->realValues() == AtomList::zeroes(8));

            t.send(L1(100));
            REQUIRE_NO_OUTPUT(t);
            REQUIRE(t->realValues() == AtomList::zeroes(8));

            t.send(L2(100, 100));
            REQUIRE_NO_OUTPUT(t);
            REQUIRE(t->realValues() == AtomList::zeroes(8));

            t.send(L2(1, 2));
            REQUIRE_OUTPUT_LIST(t, 0, L2(1, 2));
            REQUIRE_UI_FLOAT_PROPERTY(t, "count", 2);
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", 1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 2);
        }
    }

    SECTION("mouseDown")
    {
        TestExtSliders t("ui.sliders");
        REQUIRE_UI_LIST_PROPERTY(t, "size", L2(150, 100));

#define CLICK(t, x0, y0)         \
    {                            \
        t_pt p;                  \
        p.x = x0;                \
        p.y = y0;                \
        t->onMouseDown(0, p, 0); \
    }

        CLICK(t, 10, 50);
        REQUIRE_OUTPUT_LIST(t, 0, L1(0.5) + AtomList::zeroes(7));

        CLICK(t, 20, 75);
        REQUIRE_OUTPUT_LIST(t, 0, L2(0.5f, 0.25f) + AtomList::zeroes(6));

        TestExtSliders t_vert("ui.sliders");
        t_vert->resize(100, 160);
        REQUIRE_UI_LIST_PROPERTY(t_vert, "size", L2(100, 160));

        CLICK(t_vert, 50, 10);
        REQUIRE_OUTPUT_LIST(t_vert, 0, L1(0.5) + AtomList::zeroes(7));

        CLICK(t_vert, 10, 150);
        REQUIRE_OUTPUT_LIST(t_vert, 0, L1(0.5) + AtomList::zeroes(6) + L1(0.1f));
    }

    SECTION("set sliders")
    {
        TestExtSliders t("ui.sliders");

        t.call("set", L1("slider"));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::zeroes(8));

        t.call("set", L2("slider", 6));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::zeroes(8));

        t.call("set", L3("slider", -1, 0.3));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::zeroes(8));

        t.call("set", L3("slider", 1, 0.3));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "value", L2(0.f, 0.3) + AtomList::zeroes(6));
    }

    SECTION("operator")
    {
        TestExtSliders t("ui.sliders");

        t.call("+", 0.1);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::filled(0.1, 8));
        t.call("+", 2);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::ones(8));

        t.call("-", 0.1);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::filled(0.9f, 8));
        t.call("-", 10);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::zeroes(8));

        t.call("*", 10);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::zeroes(8));
        t.call("+", 0.15);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::filled(0.15f, 8));

        t.call("*", 2);
        REQUIRE_UI_LIST_PROPERTY(t, "value", AtomList::filled(0.3f, 8));
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

        t.call("get", AtomList());
        REQUIRE_NO_OUTPUT(t);

        t.call("get", L1("slider"));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", L2("slider", -2));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", L2("slider", 2));
        REQUIRE_OUTPUT_ANY(t, 0, L3("slider", 2, 0.f));

        t.call("set", L3("slider", 4, 0.6f));
        t.call("get", L2("slider", 4));
        REQUIRE_OUTPUT_ANY(t, 0, L3("slider", 4, 0.6f));
    }

    SECTION("send test")
    {
        TestExtSliders t("ui.sliders", L2("@send", "r1"));
        t.addListener("r1");

        t << BANG;
        REQUIRE_LIST_WAS_SEND(t, "r1", AtomList::zeroes(8));

        t << L8(0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8);
        REQUIRE_LIST_WAS_SEND(t, "r1", L8(0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8));

        t.call("fill", 0.5);
        t << BANG;
        REQUIRE_LIST_WAS_SEND(t, "r1", AtomList::filled(0.5f, 8));

        t.mouseDown(10, 30);
        REQUIRE_LIST_WAS_SEND(t, "r1", L1(0.7) + AtomList::filled(0.5f, 7));
    }
}
