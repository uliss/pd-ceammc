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
#include "ui_menu.h"
#include "test_ui.h"

UI_COMPLETE_TEST_SETUP(Menu)

TEST_CASE("ui.menu", "[ui.menu]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestMenu t("ui.menu");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 1);

#ifdef __APPLE__
        REQUIRE_UI_LIST_PROPERTY(t, "size", LF(100, 16));
#endif

        REQUIRE_UI_FLOAT_PROPERTY(t, "index", -1);
        REQUIRE_UI_LIST_PROPERTY(t, "value", LA(""));
        REQUIRE_UI_FLOAT_PROPERTY(t, "open", 0);

        SECTION("raw")
        {
            TestMenu t("ui.menu", LA("A", "C", "D"));
            REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "C", "D"));
        }
    }

    SECTION("@index")
    {
        TestExtMenu t("ui.menu", LA("A", "B"));
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 0);
        t <<= LA("@index", -1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 0);
        t <<= LA("@index", 2);
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 0);
        t <<= LA("@index", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 1);
    }

    SECTION("@value")
    {
        TestExtMenu t("ui.menu", LA("A", "B"));
        REQUIRE_UI_LIST_PROPERTY(t, "value", LA("A"));

        t <<= LA("@value", "Z");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "value", LA("A"));

        t <<= LA("@value", "B");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "value", LA("B"));
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 1);

        t <<= LA("@value", "A");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "value", LA("A"));
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 0);

        t <<= LA("@value");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "value", LA("A"));
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 0);
    }

    SECTION("set")
    {
        TestExtMenu t("ui.menu");
        REQUIRE(t->hasProperty(gensym("items")));

        t << $2("@items", "A", "B");
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "B"));
        REQUIRE_NO_OUTPUT(t);

        t <<= LA("set", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 1);
        REQUIRE_UI_LIST_PROPERTY(t, "value", LA("B"));
        REQUIRE_NO_OUTPUT(t);

        t <<= LA("set", "A");
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 0.f);
        REQUIRE_UI_LIST_PROPERTY(t, "value", LA("A"));
        REQUIRE_NO_OUTPUT(t);

        t << $2("@items", 1, 100);
        t <<= LA("set", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 1);
        REQUIRE_UI_LIST_PROPERTY(t, "value", LF(100));
        t <<= LA("set", "1");
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 0);
        REQUIRE_UI_LIST_PROPERTY(t, "value", LF(1));

        t <<= LA("set");
        REQUIRE_UI_LIST_PROPERTY(t, "value", LF(1));
    }

    SECTION("preset")
    {
        TestExtMenu t("ui.menu", LA("A", "B", "C"));
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "B", "C"));

        t << 0.f;
        REQUIRE_OUTPUT_LIST(t, 0, LA(0.f, "A"));
        t->storePreset(0);

        t << 1;
        REQUIRE_OUTPUT_LIST(t, 0, LA(1, "B"));
        t->storePreset(1);

        t << 2;
        REQUIRE_OUTPUT_LIST(t, 0, LA(2, "C"));
        t->storePreset(2);

        t->loadPreset(0);
        REQUIRE_OUTPUT_LIST(t, 0, LA(0.f, "A"));

        t->loadPreset(1);
        REQUIRE_OUTPUT_LIST(t, 0, LA(1, "B"));

        t->loadPreset(2);
        REQUIRE_OUTPUT_LIST(t, 0, LA(2, "C"));

        t->loadPreset(3);
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("clear")
    {
        TestExtMenu t("ui.menu");

        t <<= LA("erase");
        REQUIRE_NO_OUTPUT(t);

        t <<= LA("@items", "A", "Z");
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "Z"));
        t << 1;
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 1);

        t <<= LA("clear");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "Z"));
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 1);
        REQUIRE(t->hasProperty(gensym("items")));

        t <<= LA("erase"); // test for fixed clear/erase name conflict
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", L());
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", -1);
        REQUIRE(t->hasProperty(gensym("items")));
    }

    SECTION("delete")
    {
        TestExtMenu t("ui.menu", LF(1, 2, 3));
        t << 1;
        REQUIRE_OUTPUT_LIST(t, 0, LA(1, 2));
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 1);

        // fail
        t <<= LA("delete", -1);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LF(1, 2, 3));
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 1);

        t <<= LA("delete", 3);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LF(1, 2, 3));
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 1);

        // ok
        t <<= LA("delete", 2);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LF(1, 2));

        t <<= LA("delete", 0.f);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LF(2));

        t <<= LA("delete", 0.f);
    }

    SECTION("symbol | any")
    {
        TestExtMenu t("ui.menu", LA("A", 2, 3));

        t << "NONE";
        REQUIRE_NO_OUTPUT(t);

        t << "A";
        REQUIRE_OUTPUT_LIST(t, 0, LA(0.f, "A"));
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 0);
        REQUIRE_UI_LIST_PROPERTY(t, "value", LA("A"));

        t << "2";
        REQUIRE_OUTPUT_LIST(t, 0, LA(1, 2));

        t <<= LA("A");
        REQUIRE_OUTPUT_LIST(t, 0, LA(0.f, "A"));
    }

    SECTION("append")
    {
        TestExtMenu t("ui.menu");

        t <<= LA("append", "C", "D");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("C", "D"));

        t <<= LA("append", "E", "F");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("C", "D", "E", "F"));

        t <<= LA("append");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("C", "D", "E", "F"));
    }

    SECTION("insert")
    {
        TestExtMenu t("ui.menu");
        t <<= LA("insert", -1, "A");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_FLOAT_PROPERTY(t, "count", 0);

        t <<= LA("insert", 0.f, "A");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_FLOAT_PROPERTY(t, "count", 1);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A"));

        t <<= LA("insert", 1, "B");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_FLOAT_PROPERTY(t, "count", 2);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "B"));

        t <<= LA("insert", 3, "B");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_FLOAT_PROPERTY(t, "count", 2);

        t <<= LA("insert", "A", "B");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_FLOAT_PROPERTY(t, "count", 2);

        t <<= LA("insert", 0.f, "C");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("C", "A", "B"));

        t << 2;
        REQUIRE_UI_LIST_PROPERTY(t, "value", LA("B"));
    }

    SECTION("set_item")
    {
        TestExtMenu t("ui.menu");

        t <<= LA("set_item", 0.f, "A");
        REQUIRE_NO_OUTPUT(t);

        t <<= LA("append", "A", "B", "C", "D");
        t <<= LA("set_item", 1, "b");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "b", "C", "D"));

        t <<= LA("set_item", 3, "d");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "b", "C", "d"));

        t << 3;
        REQUIRE_UI_LIST_PROPERTY(t, "value", LA("d"));

        t <<= LA("set_item", "INVALID", "d");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "b", "C", "d"));
    }

    SECTION("bang")
    {
        TestExtMenu t("ui.menu");

        t << BANG;
        REQUIRE_NO_OUTPUT(t);

        t <<= LA("@items", "A", "B");
        t << BANG;
        REQUIRE_NO_OUTPUT(t);

        t << 1;
        REQUIRE_OUTPUT_LIST(t, 0, LA(1, "B"));

        t << 0.0;
        REQUIRE_OUTPUT_LIST(t, 0, LA(0.0, "A"));
    }

    SECTION("float")
    {
        TestExtMenu t("ui.menu");

        t << 1;
        REQUIRE_NO_OUTPUT(t);

        t <<= LA("@items", "A", "B");
        t << -1;
        REQUIRE_NO_OUTPUT(t);

        t << 2;
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("mouse")
    {
        TestExtMenu t("ui.menu");

        t.mouseDown(5, 5);
        REQUIRE_UI_FLOAT_PROPERTY(t, "open", 1);
        REQUIRE_NO_OUTPUT(t);

        t <<= LA("@open", 0.0);
        t <<= LA("@items", "A", "B", "C");
        t.mouseDown(5, 5);
        REQUIRE_UI_FLOAT_PROPERTY(t, "open", 1);

#ifdef __APPLE__
        t.mouseDown(10, 40);
        REQUIRE_UI_FLOAT_PROPERTY(t, "open", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "index", 1);
        REQUIRE_UI_LIST_PROPERTY(t, "value", LA("B"));
#endif
    }

    SECTION("file_glob")
    {
        TestExtMenu t("ui.menu");
        REQUIRE_UI_FLOAT_PROPERTY(t, "count", 0);

        t <<= LA("file_glob", "*.wav", TEST_DATA_DIR);
        t_float c = 0;
        t->getProperty(gensym("count"), c);
        REQUIRE(c > 0);
    }

    SECTION("max_length")
    {
        TestExtMenu t("ui.menu");
        REQUIRE_UI_FLOAT_PROPERTY(t, "max_length", 100);

        t <<= LA("@max_length", 10);
        REQUIRE_UI_FLOAT_PROPERTY(t, "max_length", 10);

        t <<= LA("@items", "12345678901234567890", "ABCDEFGHIJK", "АБВГДЕЖЗ", "/очень/длинное/имя/файла");

        t << 0.0;
        REQUIRE_UI_LIST_PROPERTY(t, "value", LA("12345678901234567890"));
        REQUIRE_UI_LIST_PROPERTY(t, "title", LA("123...67890"));

        t << 1;
        REQUIRE_UI_LIST_PROPERTY(t, "title", LA("ABC...GHIJK"));

        t << 2;
        REQUIRE_UI_LIST_PROPERTY(t, "title", LA("АБВГДЕЖЗ"));

        t << 3;
        REQUIRE_UI_LIST_PROPERTY(t, "title", LA("/оч...файла"));
    }

    SECTION("send")
    {
        TestExtMenu t("ui.menu", LA("@send", "r1"));
        t.addListener("r1");

        t <<= LA("@items", "A", "B", "C");
        t << 2;
        REQUIRE_LIST_WAS_SEND(t, "r1", LA(2, "C"));
    }
}
