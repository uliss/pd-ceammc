/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "../ui/ui_radio.h"
#include "ui_external_test.h"

UI_COMPLETE_TEST_SETUP(Radio)

TEST_CASE("ui.radio", "[ui.radio]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestRadio t("ui.radio");
        REQUIRE(t->numOutlets() == 1);
        REQUIRE(t->width() == 127);
        REQUIRE(t->height() == 15);
        REQUIRE(t->singleValue() == 0);
        REQUIRE(t->p_numItems() == 8);
        REQUIRE(t->p_mode() == 0);
        REQUIRE_FALSE(t->isVertical());

        REQUIRE_UI_FLOAT_PROPERTY(t, "nitems", 8);
        HAS_UI_PROPERTY(t, "active_color");
        REQUIRE_UI_FLOAT_PROPERTY(t, "mode", 0);
        REQUIRE_PRESETS(t);
    }

    SECTION("nitems")
    {
        TestRadio t("ui.radio");
        REQUIRE(t->p_numItems() == 8);
        REQUIRE(t->width() == 127);
        REQUIRE(t->height() == 15);
        REQUIRE_UI_FLOAT_PROPERTY(t, "nitems", 8);

        t->setProperty(gensym("nitems"), L1(-1));

        REQUIRE(t->p_numItems() == 2);
        REQUIRE(t->width() == 127);
        REQUIRE(t->height() == 63);

        t->setProperty(gensym("nitems"), L1(0.f));
        REQUIRE(t->p_numItems() == 2);
        t->setProperty(gensym("nitems"), L1(2));
        REQUIRE(t->p_numItems() == 2);

        t->setProperty(gensym("nitems"), L1(129));

        REQUIRE(t->p_numItems() == 128);
        REQUIRE(t->width() == 1151);
        REQUIRE(t->height() == 8);

        t->setProperty(gensym("nitems"), L1(128));
        REQUIRE(t->p_numItems() == 128);
        t->setProperty(gensym("nitems"), L1(127));
        REQUIRE(t->p_numItems() == 127);

        t->setProperty(gensym("nitems"), L1(8.5));
        REQUIRE(t->p_numItems() == 8);
    }

    SECTION("m_plus")
    {
        TestRadio t("ui.radio", L2("@nitems", 3));
        REQUIRE(t->p_numItems() == 3);

        REQUIRE(t->singleValue() == 0);
        t.call("+", L1(1));
        REQUIRE(t->singleValue() == 1);
        t.call("+", L1(0.f));
        REQUIRE(t->singleValue() == 1);
        t.call("+", L1(1));
        REQUIRE(t->singleValue() == 2);
        t.call("+", L1(1));
        REQUIRE(t->singleValue() == 0);
        t.call("+", L1(2));
        REQUIRE(t->singleValue() == 2);
        t.call("+", L1(2));
        REQUIRE(t->singleValue() == 1);
        t.call("+", L1(-1));
        REQUIRE(t->singleValue() == 0);
        t.call("+", L1(-1));
        REQUIRE(t->singleValue() == 2);
        t.call("+", L1(-300));
        REQUIRE(t->singleValue() == 2);
        t.call("+", L1(-301));
        REQUIRE(t->singleValue() == 1);
    }

    SECTION("m_minus")
    {
        TestRadio t("ui.radio", L2("@nitems", 3));
        REQUIRE(t->p_numItems() == 3);

        REQUIRE(t->singleValue() == 0);
        t.call("-", L1(1));
        REQUIRE(t->singleValue() == 2);
        t.call("-", L1(0.f));
        REQUIRE(t->singleValue() == 2);
        t.call("-", L1(1));
        REQUIRE(t->singleValue() == 1);
        t.call("-", L1(1));
        REQUIRE(t->singleValue() == 0);
        t.call("-", L1(2));
        REQUIRE(t->singleValue() == 1);
        t.call("-", L1(2));
        REQUIRE(t->singleValue() == 2);
        t.call("-", L1(-1));
        REQUIRE(t->singleValue() == 0);
        t.call("-", L1(-1));
        REQUIRE(t->singleValue() == 1);
        t.call("-", L1(-300));
        REQUIRE(t->singleValue() == 1);
        t.call("-", L1(-301));
        REQUIRE(t->singleValue() == 2);
    }

    SECTION("next/prev")
    {
        TestRadio t("ui.radio", L2("@nitems", 3));
        REQUIRE(t->p_numItems() == 3);

        REQUIRE(t->singleValue() == 0);
        t.call("next");
        REQUIRE(t->singleValue() == 1);
        t.call("next");
        REQUIRE(t->singleValue() == 2);
        t.call("next");
        REQUIRE(t->singleValue() == 2);
        t.call("prev");
        REQUIRE(t->singleValue() == 1);
        t.call("prev");
        REQUIRE(t->singleValue() == 0);
        t.call("prev");
        REQUIRE(t->singleValue() == 0);

        // only in single mode
        t->setSingleValue(1);
        t->setProperty(gensym("mode"), L1(1));
        REQUIRE(t->singleValue() == 0);

        REQUIRE(t->singleValue() == 0);
        t.call("next");
        REQUIRE(t->singleValue() == 0);
        t.call("prev");
        REQUIRE(t->singleValue() == 0);
    }

    SECTION("flip")
    {
        TestRadio t("ui.radio", L4("@nitems", 3, "@mode", 1));
        REQUIRE(t->p_numItems() == 3);

        t.call("flip");
        REQUIRE(t->listValue() == L3(1, 1, 1));
        REQUIRE_UI_LIST_PROPERTY(t, "value", L3(1, 1, 1));

        t.call("flip");
        REQUIRE(t->listValue() == L3(0.f, 0.f, 0.f));

        // only in check list mode
        t.call("@mode", L1(0.f));
        t.call("flip");
        REQUIRE(t->listValue() == L3(0.f, 0.f, 0.f));
    }

    SECTION("PD")
    {
        TestPdExternal<UIRadio> t("ui.radio", L2("@nitems", 4));
        REQUIRE(t->p_numItems() == 4);

        t.call("@value?");
        REQUIRE_OUTPUT_ANY(t, 0, L2("@value", 0.f));
        t.bang();
        REQUIRE_OUTPUT_FLOAT(t, 0, 0);
        t.call("next");
        REQUIRE_OUTPUT_FLOAT(t, 0, 1);
        t.call("@value?");
        REQUIRE_OUTPUT_ANY(t, 0, L2("@value", 1));
        t.bang();
        REQUIRE_OUTPUT_FLOAT(t, 0, 1);
        t.call("flip");
        REQUIRE_NO_OUTPUT(t);

        t.call("set", 0);
        REQUIRE_NO_OUTPUT(t);
        t.bang();
        REQUIRE_OUTPUT_FLOAT(t, 0, 0);

        t.send(3);
        REQUIRE_OUTPUT_FLOAT(t, 0, 3);
        t.call("@value", L1(2));
        REQUIRE_NO_OUTPUT(t);
        t.call("reset");
        REQUIRE_NO_OUTPUT(t);

        // presets
        t.call("store", L1(1));
        REQUIRE_NO_OUTPUT(t);
        t.call("@value", L1(1));
        REQUIRE_NO_OUTPUT(t);
        t.call("store", L1(2));
        REQUIRE_NO_OUTPUT(t);
        t.call("load", L1(1));
        REQUIRE_OUTPUT_FLOAT(t, 0, 2);
        t.call("load", L1(2));
        REQUIRE_OUTPUT_FLOAT(t, 0, 1);

        t.call("@mode", 1.f);
        REQUIRE_NO_OUTPUT(t);
        t.call("flip");
        REQUIRE_OUTPUT_LIST(t, 0, L4(1, 1, 1, 1));
        t.call("@value?");
        REQUIRE_OUTPUT_ANY(t, 0, L5("@value", 1, 1, 1, 1));
        t.bang();
        REQUIRE_OUTPUT_LIST(t, 0, L4(1, 1, 1, 1));

        t.send(L2(0.f, 1));
        REQUIRE_OUTPUT_LIST(t, 0, L4(0.f, 1, 1, 1));
        t.send(L5(0.f, 1, 0.f, 0.f, 1));
        REQUIRE_OUTPUT_LIST(t, 0, L4(0.f, 1, 0.f, 0.f));

        t.call("@value", L4(1, 0.f, 1, 0.f));
        REQUIRE_NO_OUTPUT(t);
        t.call("store", L1(1));
        REQUIRE_NO_OUTPUT(t);
        t.call("flip");
        t.call("store", L1(2));

        t.call("load", L1(1));
        REQUIRE_OUTPUT_LIST(t, 0, L4(1, 0.f, 1, 0.f));
        t.call("load", L1(2));
        REQUIRE_OUTPUT_LIST(t, 0, L4(0.f, 1, 0.f, 1));
        t.call("load", L1(1));
        REQUIRE_OUTPUT_LIST(t, 0, L4(1, 0.f, 1, 0.f));

        t.call("+");
        REQUIRE_NO_OUTPUT(t);
        t.call("-");
        REQUIRE_NO_OUTPUT(t);
        t.call("+", 1);
        REQUIRE_NO_OUTPUT(t);
        t.call("-", 1);
        REQUIRE_NO_OUTPUT(t);

        t.call("reset");
        REQUIRE_OUTPUT_LIST(t, 0, AtomList::zeroes(4));
    }

    SECTION("send")
    {
        SECTION("plain")
        {
            TestExtRadio t("ui.radio", L2("@send", "r2"));
            t.addListener("r2");

            t << BANG;
            REQUIRE_FLOAT_WAS_SEND(t, "r2", 0);

            t << 2;
            REQUIRE_FLOAT_WAS_SEND(t, "r2", 2);

            t << BANG;
            REQUIRE_FLOAT_WAS_SEND(t, "r2", 2);

            t << L2(1, 2);
            REQUIRE_NONE_WAS_SEND(t, "r2");

            t.call("any", L2(1, 2));
            REQUIRE_NONE_WAS_SEND(t, "r2");

            t << "ABC";
            REQUIRE_NONE_WAS_SEND(t, "r2");

            t.mouseDown(55, 5);
            REQUIRE_FLOAT_WAS_SEND(t, "r2", 3);
        }

        SECTION("checkbox")
        {
            TestExtRadio t("ui.radio", L4("@mode", 1, "@send", "r2"));
            t.addListener("r2");

            t << BANG;
            REQUIRE_LIST_WAS_SEND(t, "r2", AtomList::zeroes(8));

            t << 2;
            REQUIRE_NONE_WAS_SEND(t, "r2");

            t << L4(1, 1, 1, 1);
            REQUIRE_LIST_WAS_SEND(t, "r2", L4(1, 1, 1, 1) + AtomList::zeroes(4));

            t.call("any", L2(1, 2));
            REQUIRE_NONE_WAS_SEND(t, "r2");

            t.call("reset");
            t.mouseDown(55, 5);
            REQUIRE_LIST_WAS_SEND(t, "r2", ListApprox(0, 0, 0, 1, 0, 0, 0, 0));
        }
    }
}
