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
#include "../ui/ui_tab.h"
#include "ui_external_test.h"

UI_COMPLETE_TEST_SETUP(Tab)

TEST_CASE("ui.tab", "[ui.tab]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestTab t("ui.tab");
        REQUIRE(t->numInlets());
        REQUIRE(t->numOutlets() == 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "orientation", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "toggle", 0);

        SECTION("raw items")
        {
            TestTab t("ui.tab", L3("a", "b", 10));
            REQUIRE_UI_LIST_PROPERTY(t, "items", L3("a", "b", 10));
        }

        SECTION("@items property")
        {
            TestTab t("ui.tab", L4("@items", "a", "b", 10));
            REQUIRE_UI_LIST_PROPERTY(t, "items", L3("a", "b", 10));
        }
    }

    SECTION("external")
    {
        SECTION("empty")
        {
            TestExtTab t("ui.tab");
            t << BANG;
            REQUIRE_NO_OUTPUT(t);
        }

        SECTION("items")
        {
            TestExtTab t("ui.tab", L2(1, 2));
            t->setProperty(gensym("send"), L1("r1"));
            t.addListener("r1");

            t << BANG;
            REQUIRE_NO_OUTPUT(t);
            REQUIRE_UI_LIST_PROPERTY(t, "size", L2(100, 15));
            REQUIRE_NONE_WAS_SEND(t, "r1");

            t.mouseDown(25, 10);
            REQUIRE_OUTPUT_FLOAT(t, 0, 0);
            REQUIRE_FLOAT_WAS_SEND(t, "r1", 0);

            t.mouseDown(75, 10);
            REQUIRE_OUTPUT_FLOAT(t, 0, 1);
            REQUIRE_FLOAT_WAS_SEND(t, "r1", 1);

            // invalid
            t << -1;
            REQUIRE_NO_OUTPUT(t);
            t << 100;
            REQUIRE_NO_OUTPUT(t);
            t << 2;
            REQUIRE_NO_OUTPUT(t);

            t << 1;
            REQUIRE_OUTPUT_FLOAT(t, 0, 1);
            REQUIRE_FLOAT_WAS_SEND(t, "r1", 1);

            t << 0.f;
            REQUIRE_OUTPUT_FLOAT(t, 0, 0);
            REQUIRE_FLOAT_WAS_SEND(t, "r1", 0);
        }
    }

    SECTION("onBang")
    {
        SECTION("single")
        {
            TestExtTab t("ui.tab", L2("A", "B"));
            t << BANG;
            REQUIRE_NO_OUTPUT(t);

            t.mouseDown(25, 8);
            t << BANG;
            REQUIRE_OUTPUT_FLOAT(t, 0, 0);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(1, 0.f));

            t.mouseDown(75, 8);
            t << BANG;
            REQUIRE_OUTPUT_FLOAT(t, 0, 1);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(0.f, 1));

            t <<= L2("select", 0.f);
            REQUIRE_NO_OUTPUT(t);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(1, 0.f));

            t <<= L2("select", 1);
            REQUIRE_NO_OUTPUT(t);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(0.f, 1));

            t <<= L2("select", -1);
            REQUIRE_NO_OUTPUT(t);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(0.f, 1));

            t <<= L2("select", 2);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(0.f, 1));

            t <<= L2("select", "A");
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(1, 0.f));

            t <<= L2("select", "B");
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(0.f, 1.0));
        }

        SECTION("toggle")
        {
            TestExtTab t("ui.tab", L5("@toggle", 1, "@items", "A", "B"));
            t << BANG;
            REQUIRE_OUTPUT_ANY(t, 0, L3("@selected", 0.f, 0.f));

            t.mouseDown(25, 8);
            t << BANG;
            REQUIRE_OUTPUT_LIST(t, 0, L2(0.f, 1));
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(1, 0.f));

            t.mouseDown(75, 8);
            t << BANG;
            REQUIRE_OUTPUT_LIST(t, 0, L2(1, 1));
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(1, 1));

            t.mouseDown(25, 8);
            t << BANG;
            REQUIRE_OUTPUT_LIST(t, 0, L2(0.f, 0.f));
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(0.f, 1));

            t.mouseDown(75, 8);
            t << BANG;
            REQUIRE_OUTPUT_LIST(t, 0, L2(1, 0.f));
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(0.f, 0.f));

            REQUIRE_UI_LIST_PROPERTY(t, "items", L2("A", "B"));
            t <<= L1("clear");
            REQUIRE(!t->hasProperty(gensym("@items")));
        }
    }

    SECTION("presets")
    {
        SECTION("single")
        {
            TestExtTab t("ui.tab", L3("@items", "A", "B"));
            t << 1;
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(0.f, 1));

            t->storePreset(0);
            t << 0.f;
            t->storePreset(1);

            t->loadPreset(0);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(0.f, 1));
            t->loadPreset(1);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(1, 0.f));
            t->loadPreset(2);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L2(1, 0.f));
        }
    }

    SECTION("append")
    {
        TestExtTab t("ui.tab");
        t <<= L3("append", "A", "B");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", L2("A", "B"));

        t <<= L3("append", "C", "D");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", L4("A", "B", "C", "D"));
    }

    SECTION("set_item")
    {
        TestExtTab t("ui.tab");
        t <<= L3("append", "A", "B");

        t <<= L3("set_item", -1, "D");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", L2("A", "B"));

        t <<= L3("set_item", 2, "D");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", L2("A", "B"));

        t <<= L3("set_item", "INVALID_INDEX", "D");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", L2("A", "B"));

        t <<= L3("set_item", 1, "D");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", L2("A", "D"));

        t <<= L3("set_item", 0.f, "E");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", L2("E", "D"));
    }

    SECTION("delete")
    {
        TestExtTab t("ui.tab");
        t <<= L2("delete", 0.f);
        REQUIRE_NO_OUTPUT(t);

        t <<= L3("append", "A", "B");
        t <<= L2("delete", 0.f);
        REQUIRE_UI_LIST_PROPERTY(t, "items", L1("B"));

        t <<= L2("delete", -1);
        REQUIRE_UI_LIST_PROPERTY(t, "items", L1("B"));
        t <<= L2("delete", 1);
        REQUIRE_UI_LIST_PROPERTY(t, "items", L1("B"));

        t <<= L2("delete", 0.f);
        REQUIRE(t->hasProperty(gensym("items")));
    }

    SECTION("insert")
    {
        TestExtTab t("ui.tab");

        // invalid
        t <<= L1("insert");
        t <<= L2("insert", -1);
        t <<= L3("insert", -1, "A");
        REQUIRE_UI_FLOAT_PROPERTY(t, "count", 0);

        t <<= L3("insert", 0.f, "A");
        REQUIRE_UI_LIST_PROPERTY(t, "items", L1("A"));
        REQUIRE_UI_FLOAT_PROPERTY(t, "count", 1);

        t <<= L3("insert", 0.f, "B");
        REQUIRE_UI_LIST_PROPERTY(t, "items", L2("B", "A"));
    }

    SECTION("select")
    {
        SECTION("single")
        {
            TestExtTab t("ui.tab");
            t << PropertySetter("@items", L4(1, 2, "A", "B"));
            REQUIRE_UI_LIST_PROPERTY(t, "items", L4(1, 2, "A", "B"));

            // index
            t <<= L2("select", 0.f);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 0);
            t <<= L2("select", 1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 1);
            t <<= L2("select", -1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 1);
            t <<= L2("select", 2);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 2);
            t <<= L2("select", 3);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 3);
            t <<= L2("select", 4);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 3);

            t <<= L2("select", "1");
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 0);
            t <<= L2("select", "2");
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 1);
            t <<= L2("select", "A");
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 2);
            t <<= L2("select", "B");
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 3);
            t <<= L2("select", "???");
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 3);
        }

        SECTION("toggle")
        {
        }
    }

    SECTION("")
    {
        SECTION("single")
        {
            TestExtTab t("ui.tab");
            t << "A";
            REQUIRE_NO_OUTPUT(t);

            t <<= L5("@items", "A", "B", "C", "D");

            t << "A";
            REQUIRE_OUTPUT_FLOAT(t, 0, 0);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 0);
            t << "D";
            REQUIRE_OUTPUT_FLOAT(t, 0, 3);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 3);
            t << "?";
            REQUIRE_NO_OUTPUT(t);
        }

        SECTION("toggle")
        {
            TestExtTab t("ui.tab", L2("@toggle", 1));
            t << "A";
            REQUIRE_NO_OUTPUT(t);

            t <<= L5("@items", "A", "B", "C", "D");

            t << "A";
            REQUIRE_OUTPUT_LIST(t, 0, L2(0.f, 1));
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", -1);

            t << "A";
            REQUIRE_OUTPUT_LIST(t, 0, L2(0.f, 0.f));
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", -1);

            t << "B";
            REQUIRE_OUTPUT_LIST(t, 0, L2(1, 1));
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", -1);

            t << "D";
            REQUIRE_OUTPUT_LIST(t, 0, L2(3, 1));
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L4(0.f, 1, 0.f, 1));
        }
    }
}
