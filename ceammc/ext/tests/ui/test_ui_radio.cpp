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
#include "ceammc_preset.h"
#include "ui_radio.h"

#include "test_ui_base.h"

UI_COMPLETE_TEST_SETUP(Radio)

TEST_CASE("ui.radio", "[ui.radio]")
{
    ui_test_init();
    PresetStorage::instance().clearAll();

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

        SECTION("aliases")
        {
            SECTION("ui.hrd")
            {
                TestExtRadio t("ui.hrd");
                REQUIRE(t.object());
                REQUIRE(t->width() == 127);
                REQUIRE(t->height() == 15);
                REQUIRE(t->p_mode() == 0);
            }

            SECTION("ui.hrd*")
            {
                TestExtRadio t("ui.hrd*");
                REQUIRE(t.object());
                REQUIRE(t->width() == 127);
                REQUIRE(t->height() == 15);
                REQUIRE(t->p_mode() == 1);
            }

            SECTION("ui.vrd")
            {
                TestExtRadio t("ui.vrd");
                REQUIRE(t.object());
                REQUIRE(t->width() == 15);
                REQUIRE(t->height() == 127);
                REQUIRE(t->p_mode() == 0);
            }

            SECTION("ui.vrd*")
            {
                TestExtRadio t("ui.vrd*");
                REQUIRE(t.object());
                REQUIRE(t->width() == 15);
                REQUIRE(t->height() == 127);
                REQUIRE(t->p_mode() == 1);
            }
        }
    }

    SECTION("nitems")
    {
        TestRadio t("ui.radio");
        REQUIRE(t->p_numItems() == 8);
        REQUIRE(t->width() == 127);
        REQUIRE(t->height() == 15);
        REQUIRE_UI_FLOAT_PROPERTY(t, "nitems", 8);

        t->setProperty(gensym("nitems"), LF(-1));

        REQUIRE(t->p_numItems() == 2);
        REQUIRE(t->width() == 127);
        REQUIRE(t->height() == 63);

        t->setProperty(gensym("nitems"), LF(0.f));
        REQUIRE(t->p_numItems() == 2);
        t->setProperty(gensym("nitems"), LF(2));
        REQUIRE(t->p_numItems() == 2);

        t->setProperty(gensym("nitems"), LF(129));

        REQUIRE(t->p_numItems() == 128);
        REQUIRE(t->width() == 1151);
        REQUIRE(t->height() == 8);

        t->setProperty(gensym("nitems"), LF(128));
        REQUIRE(t->p_numItems() == 128);
        t->setProperty(gensym("nitems"), LF(127));
        REQUIRE(t->p_numItems() == 127);

        t->setProperty(gensym("nitems"), LF(8.5));
        REQUIRE(t->p_numItems() == 8);
    }

    SECTION("m_plus")
    {
        TestRadio t("ui.radio", LA("@nitems", 3));
        REQUIRE(t->p_numItems() == 3);

        REQUIRE(t->singleValue() == 0);
        t.call("+", LF(1));
        REQUIRE(t->singleValue() == 1);
        t.call("+", LF(0.f));
        REQUIRE(t->singleValue() == 1);
        t.call("+", LF(1));
        REQUIRE(t->singleValue() == 2);
        t.call("+", LF(1));
        REQUIRE(t->singleValue() == 0);
        t.call("+", LF(2));
        REQUIRE(t->singleValue() == 2);
        t.call("+", LF(2));
        REQUIRE(t->singleValue() == 1);
        t.call("+", LF(-1));
        REQUIRE(t->singleValue() == 0);
        t.call("+", LF(-1));
        REQUIRE(t->singleValue() == 2);
        t.call("+", LF(-300));
        REQUIRE(t->singleValue() == 2);
        t.call("+", LF(-301));
        REQUIRE(t->singleValue() == 1);
    }

    SECTION("m_minus")
    {
        TestRadio t("ui.radio", LA("@nitems", 3));
        REQUIRE(t->p_numItems() == 3);

        REQUIRE(t->singleValue() == 0);
        t.call("-", LF(1));
        REQUIRE(t->singleValue() == 2);
        t.call("-", LF(0.f));
        REQUIRE(t->singleValue() == 2);
        t.call("-", LF(1));
        REQUIRE(t->singleValue() == 1);
        t.call("-", LF(1));
        REQUIRE(t->singleValue() == 0);
        t.call("-", LF(2));
        REQUIRE(t->singleValue() == 1);
        t.call("-", LF(2));
        REQUIRE(t->singleValue() == 2);
        t.call("-", LF(-1));
        REQUIRE(t->singleValue() == 0);
        t.call("-", LF(-1));
        REQUIRE(t->singleValue() == 1);
        t.call("-", LF(-300));
        REQUIRE(t->singleValue() == 1);
        t.call("-", LF(-301));
        REQUIRE(t->singleValue() == 2);
    }

    SECTION("next/prev")
    {
        TestRadio t("ui.radio", LA("@nitems", 3));
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
        t->setProperty(gensym("mode"), LF(1));
        REQUIRE(t->singleValue() == 0);

        REQUIRE(t->singleValue() == 0);
        t.call("next");
        REQUIRE(t->singleValue() == 0);
        t.call("prev");
        REQUIRE(t->singleValue() == 0);
    }

    SECTION("flip")
    {
        TestRadio t("ui.radio", LA("@nitems", 3, "@mode", 1));
        REQUIRE(t->p_numItems() == 3);

        t.call("flip");
        REQUIRE(t->listValue() == LF(1, 1, 1));
        REQUIRE_UI_LIST_PROPERTY(t, "value", LF(1, 1, 1));

        t.call("flip");
        REQUIRE(t->listValue() == LF(0.f, 0.f, 0.f));

        // only in check list mode
        t.call("@mode", LF(0.f));
        t.call("flip");
        REQUIRE(t->listValue() == LF(0.f, 0.f, 0.f));
    }

    SECTION("PD")
    {
        TestExtRadio t("ui.radio", LA("@nitems", 4));
        REQUIRE(t->p_numItems() == 4);

        t.call("@value?");
        REQUIRE_OUTPUT_ANY(t, 0, LA("@value", 0.f));
        t.bang();
        REQUIRE_OUTPUT_FLOAT(t, 0, 0);
        t.call("next");
        REQUIRE_OUTPUT_FLOAT(t, 0, 1);
        t.call("@value?");
        REQUIRE_OUTPUT_ANY(t, 0, LA("@value", 1));
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
        t.call("@value", LF(2));
        REQUIRE_NO_OUTPUT(t);
        t.call("reset");
        REQUIRE_NO_OUTPUT(t);

        // presets
        t.call("store", LF(1));
        REQUIRE_NO_OUTPUT(t);
        t.call("@value", LF(1));
        REQUIRE_NO_OUTPUT(t);
        t.call("store", LF(2));
        REQUIRE_NO_OUTPUT(t);
        t.call("load", LF(1));
        REQUIRE_OUTPUT_FLOAT(t, 0, 2);
        t.call("load", LF(2));
        REQUIRE_OUTPUT_FLOAT(t, 0, 1);

        t.call("@mode", 1.f);
        REQUIRE_NO_OUTPUT(t);
        t.call("flip");
        REQUIRE_OUTPUT_LIST(t, 0, LF(1, 1, 1, 1));
        t.call("@value?");
        REQUIRE_OUTPUT_ANY(t, 0, LA("@value", 1, 1, 1, 1));
        t.bang();
        REQUIRE_OUTPUT_LIST(t, 0, LF(1, 1, 1, 1));

        t.send(LF(0.f, 1));
        REQUIRE_OUTPUT_LIST(t, 0, LA(0.f, 1, 1, 1));
        t.send(LA(0.f, 1, 0.f, 0.f, 1));
        REQUIRE_OUTPUT_LIST(t, 0, LA(0.f, 1, 0.f, 0.f));

        t.call("@value", LA(1, 0.f, 1, 0.f));
        REQUIRE_NO_OUTPUT(t);
        t.call("store", LF(1));
        REQUIRE_NO_OUTPUT(t);
        t.call("flip");
        t.call("store", LF(2));

        t.call("load", LF(1));
        REQUIRE_OUTPUT_LIST(t, 0, LA(1, 0.f, 1, 0.f));
        t.call("load", LF(2));
        REQUIRE_OUTPUT_LIST(t, 0, LA(0.f, 1, 0.f, 1));
        t.call("load", LF(1));
        REQUIRE_OUTPUT_LIST(t, 0, LA(1, 0.f, 1, 0.f));

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
            TestExtRadio t("ui.radio", LA("@send", "r2"));
            t.addListener("r2");

            t << BANG;
            REQUIRE_FLOAT_WAS_SEND(t, "r2", 0);

            t << 2;
            REQUIRE_FLOAT_WAS_SEND(t, "r2", 2);

            t << BANG;
            REQUIRE_FLOAT_WAS_SEND(t, "r2", 2);

            t << LF(1, 2);
            REQUIRE_NONE_WAS_SEND(t, "r2");

            t.call("any", LF(1, 2));
            REQUIRE_NONE_WAS_SEND(t, "r2");

            t << "ABC";
            REQUIRE_NONE_WAS_SEND(t, "r2");

            t.mouseDown(55, 5);
            REQUIRE_FLOAT_WAS_SEND(t, "r2", 3);
        }

        SECTION("checkbox")
        {
            TestExtRadio t("ui.radio", LA("@mode", 1, "@send", "r2"));
            t.addListener("r2");

            t << BANG;
            REQUIRE_LIST_WAS_SEND(t, "r2", AtomList::zeroes(8));

            t << 2;
            REQUIRE_NONE_WAS_SEND(t, "r2");

            t << LF(1, 1, 1, 1);
            REQUIRE_LIST_WAS_SEND(t, "r2", LF(1, 1, 1, 1) + AtomList::zeroes(4));

            t.call("any", LF(1, 2));
            REQUIRE_NONE_WAS_SEND(t, "r2");

            t.call("reset");
            t.mouseDown(55, 5);
            REQUIRE_LIST_WAS_SEND(t, "r2", LX(0, 0, 0, 1, 0, 0, 0, 0));
        }
    }

    PresetStorage::instance().clearAll();
}
