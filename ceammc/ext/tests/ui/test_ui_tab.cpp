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
#include "ui_tab.h"

#include "test_ui.h"

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
            TestTab t("ui.tab", LA("a", "b", 10));
            REQUIRE_UI_LIST_PROPERTY(t, "items", LA("a", "b", 10));
        }

        SECTION("mixed items")
        {
            TestTab t("ui.tab", LA("a", "b", 10, "@size", 40, 20));
            REQUIRE_UI_LIST_PROPERTY(t, "items", LA("a", "b", 10));
            REQUIRE_UI_LIST_PROPERTY(t, "size", LF(40, 20));
        }

        SECTION("@items property")
        {
            TestTab t("ui.tab", LA("@items", "a", "b", 10));
            REQUIRE_UI_LIST_PROPERTY(t, "items", LA("a", "b", 10));
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
            TestExtTab t("ui.tab", LF(1, 2));
            t->setProperty(gensym("send"), LA("r1"));
            t.addListener("r1");

            t << BANG;
            REQUIRE_NO_OUTPUT(t);
            REQUIRE_UI_LIST_PROPERTY(t, "size", LF(100, 15));
            REQUIRE_NONE_WAS_SEND(t, "r1");

            t.mouseDown(25, 10);
            REQUIRE_OUTPUT_LIST(t, 0, LF(0, 1));
            REQUIRE_LIST_WAS_SEND(t, "r1", LF(0, 1));

            t.mouseDown(75, 10);
            REQUIRE_OUTPUT_LIST(t, 0, LF(1, 2));
            REQUIRE_LIST_WAS_SEND(t, "r1", LF(1, 2));

            // invalid
            t << -1;
            REQUIRE_NO_OUTPUT(t);
            t << 100;
            REQUIRE_NO_OUTPUT(t);
            t << 2;
            REQUIRE_NO_OUTPUT(t);

            t << 1;
            REQUIRE_OUTPUT_LIST(t, 0, LF(1, 2));
            REQUIRE_LIST_WAS_SEND(t, "r1", LF(1, 2));

            t << 0.f;
            REQUIRE_OUTPUT_LIST(t, 0, LF(0, 1));
            REQUIRE_LIST_WAS_SEND(t, "r1", LF(0, 1));
        }
    }

    SECTION("onBang")
    {
        SECTION("single")
        {
            TestExtTab t("ui.tab", LA("A", "B"));
            t << BANG;
            REQUIRE_NO_OUTPUT(t);

            t.mouseDown(25, 8);
            t << BANG;
            REQUIRE_OUTPUT_LIST(t, 0, LA(0.f, "A"));
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(1, 0.f));

            t.mouseDown(75, 8);
            t << BANG;
            REQUIRE_OUTPUT_LIST(t, 0, LA(1, "B"));
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(0.f, 1));

            t <<= LA("select", 0.f);
            REQUIRE_NO_OUTPUT(t);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(1, 0.f));

            t <<= LA("select", 1);
            REQUIRE_NO_OUTPUT(t);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(0.f, 1));

            t <<= LA("select", -1);
            REQUIRE_NO_OUTPUT(t);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(0.f, 1));

            t <<= LA("select", 2);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(0.f, 1));

            t <<= LA("select", "A");
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(1, 0.f));

            t <<= LA("select", "B");
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(0.f, 1.0));

            t <<= LA("erase");
            REQUIRE_UI_LIST_PROPERTY(t, "selected", L());
        }

        SECTION("toggle")
        {
            TestExtTab t("ui.tab", LA("@toggle", 1, "@items", "A", "B"));
            t << BANG;
            REQUIRE_OUTPUT_ANY(t, 0, LA("@selected", 0.f, 0.f));

            t.mouseDown(25, 8);
            t << BANG;
            REQUIRE_OUTPUT_LIST(t, 0, LA(0.f, 1));
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(1, 0.f));

            t.mouseDown(75, 8);
            t << BANG;
            REQUIRE_OUTPUT_LIST(t, 0, LF(1, 1));
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LF(1, 1));

            t.mouseDown(25, 8);
            t << BANG;
            REQUIRE_OUTPUT_LIST(t, 0, LF(0.f, 0.f));
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(0.f, 1));

            t.mouseDown(75, 8);
            t << BANG;
            REQUIRE_OUTPUT_LIST(t, 0, LA(1, 0.f));
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LF(0.f, 0.f));

            REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "B"));
            t <<= LA("clear"); // this is preset method
            REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "B"));

            t <<= LA("erase");
            REQUIRE(t->hasProperty(gensym("items")));
            REQUIRE_UI_LIST_PROPERTY(t, "items", L());
        }
    }

    SECTION("presets")
    {
        SECTION("single")
        {
            TestExtTab t("ui.tab", LA("@items", "A", "B"));
            t << 1;
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(0.f, 1));

            t->storePreset(0);
            t << 0.f;
            t->storePreset(1);

            t->loadPreset(0);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(0.f, 1));
            t->loadPreset(1);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(1, 0.f));
            t->loadPreset(2);
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(1, 0.f));
        }
    }

    SECTION("append")
    {
        TestExtTab t("ui.tab");
        t <<= LA("append", "A", "B");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "B"));

        t <<= LA("append", "C", "D");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "B", "C", "D"));
    }

    SECTION("set_item")
    {
        TestExtTab t("ui.tab");
        t <<= LA("append", "A", "B");

        t <<= LA("set_item", -1, "D");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "B"));

        t <<= LA("set_item", 2, "D");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "B"));

        t <<= LA("set_item", "INVALID_INDEX", "D");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "B"));

        t <<= LA("set_item", 1, "D");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A", "D"));

        t <<= LA("set_item", 0.f, "E");
        REQUIRE_NO_OUTPUT(t);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("E", "D"));
    }

    SECTION("delete")
    {
        TestExtTab t("ui.tab");
        t <<= LA("delete", 0.f);
        REQUIRE_NO_OUTPUT(t);

        t <<= LA("append", "A", "B");
        t <<= LA("delete", 0.f);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("B"));

        t <<= LA("delete", -1);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("B"));
        t <<= LA("delete", 1);
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("B"));

        t <<= LA("delete", 0.f);
        REQUIRE(t->hasProperty(gensym("items")));
    }

    SECTION("insert")
    {
        TestExtTab t("ui.tab");

        // invalid
        t <<= LA("insert");
        t <<= LA("insert", -1);
        t <<= LA("insert", -1, "A");
        REQUIRE_UI_FLOAT_PROPERTY(t, "count", 0);

        t <<= LA("insert", 0.f, "A");
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("A"));
        REQUIRE_UI_FLOAT_PROPERTY(t, "count", 1);

        t <<= LA("insert", 0.f, "B");
        REQUIRE_UI_LIST_PROPERTY(t, "items", LA("B", "A"));
    }

    SECTION("select")
    {
        SECTION("single")
        {
            TestExtTab t("ui.tab");
            t << PropertySetter("@items", LA(1, 2, "A", "B"));
            REQUIRE_UI_LIST_PROPERTY(t, "items", LA(1, 2, "A", "B"));

            // index
            t <<= LA("select", 0.f);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 0);
            t <<= LA("select", 1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 1);
            t <<= LA("select", -1);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 1);
            t <<= LA("select", 2);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 2);
            t <<= LA("select", 3);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 3);
            t <<= LA("select", 4);
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 3);

            t <<= LA("select", "1");
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 0);
            t <<= LA("select", "2");
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 1);
            t <<= LA("select", "A");
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 2);
            t <<= LA("select", "B");
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 3);
            t <<= LA("select", "???");
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

            t <<= LA("@items", "A", "B", "C", "D");

            t << "A";
            REQUIRE_OUTPUT_LIST(t, 0, LA(0.f, "A"));
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 0);
            t << "D";
            REQUIRE_OUTPUT_LIST(t, 0, LA(3, "D"));
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", 3);
            t << "?";
            REQUIRE_NO_OUTPUT(t);
        }

        SECTION("toggle")
        {
            TestExtTab t("ui.tab", LA("@toggle", 1));
            t << "A";
            REQUIRE_NO_OUTPUT(t);

            t <<= LA("@items", "A", "B", "C", "D");

            t << "A";
            REQUIRE_OUTPUT_LIST(t, 0, LA(0.f, 1));
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", -1);

            t << "A";
            REQUIRE_OUTPUT_LIST(t, 0, LF(0.f, 0.f));
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", -1);

            t << "B";
            REQUIRE_OUTPUT_LIST(t, 0, LF(1, 1));
            REQUIRE_UI_FLOAT_PROPERTY(t, "current", -1);

            t << "D";
            REQUIRE_OUTPUT_LIST(t, 0, LF(3, 1));
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(0.f, 1, 0.f, 1));
        }
    }

    SECTION("+ - next prev")
    {
        SECTION("single")
        {
            TestExtTab t("ui.tab");
            t <<= LA("+", 1);

            REQUIRE(t->propCurrent() == -1);
            t <<= LA("@items", "A");
            t << "A";
            t <<= LA("+", 1);
            REQUIRE(t->propCurrent() == 0);
            t <<= LA("-", 1);
            REQUIRE(t->propCurrent() == 0);
            t <<= LA("next");
            REQUIRE(t->propCurrent() == 0);
            t <<= LA("prev");
            REQUIRE(t->propCurrent() == 0);
            t <<= LA("random");
            REQUIRE(t->propCurrent() == 0);

            t <<= LA("@items", "A", "B", "C");
            // +
            t <<= LA("+", 1);
            REQUIRE(t->propCurrent() == 1);
            REQUIRE_OUTPUT_LIST(t, 0, LA(1, "B"));
            t <<= LA("+", 1);
            REQUIRE(t->propCurrent() == 2);
            REQUIRE_OUTPUT_LIST(t, 0, LA(2, "C"));
            t <<= LA("+", 1);
            REQUIRE(t->propCurrent() == 0);
            REQUIRE_OUTPUT_LIST(t, 0, LA(0.0, "A"));
            t <<= LA("+", 1);
            REQUIRE(t->propCurrent() == 1);
            REQUIRE_OUTPUT_LIST(t, 0, LA(1, "B"));
            t <<= LA("+", 0.0);
            REQUIRE(t->propCurrent() == 1);
            REQUIRE_OUTPUT_LIST(t, 0, LA(1, "B"));
            t <<= LA("+", -1);
            REQUIRE(t->propCurrent() == 0);
            REQUIRE_OUTPUT_LIST(t, 0, LA(0.0, "A"));
            t <<= LA("+", -1);
            REQUIRE(t->propCurrent() == 2);
            t <<= LA("+", -2);
            REQUIRE(t->propCurrent() == 0);
            t <<= LA("+", 3);
            REQUIRE(t->propCurrent() == 0);
            t <<= LA("+", 4);
            REQUIRE(t->propCurrent() == 1);
            // -
            t <<= LA("-", 1);
            REQUIRE(t->propCurrent() == 0);
            REQUIRE_OUTPUT_LIST(t, 0, LA(0.0, "A"));
            t <<= LA("-", 1);
            REQUIRE(t->propCurrent() == 2);
            REQUIRE_OUTPUT_LIST(t, 0, LA(2, "C"));
            t <<= LA("-", 1);
            REQUIRE(t->propCurrent() == 1);
            REQUIRE_OUTPUT_LIST(t, 0, LA(1, "B"));
            t <<= LA("-", 3);
            REQUIRE(t->propCurrent() == 1);
            REQUIRE_OUTPUT_LIST(t, 0, LA(1, "B"));
            t <<= LA("-", 6);
            REQUIRE(t->propCurrent() == 1);
            t <<= LA("-", 5);
            REQUIRE(t->propCurrent() == 2);
            // next
            t <<= LA("next");
            REQUIRE(t->propCurrent() == 2);
            REQUIRE_NO_OUTPUT(t);
            t <<= LA("next");
            REQUIRE(t->propCurrent() == 2);
            REQUIRE_NO_OUTPUT(t);
            t << "A";
            t <<= LA("next");
            REQUIRE(t->propCurrent() == 1);
            REQUIRE_OUTPUT_LIST(t, 0, LA(1, "B"));
            t <<= LA("next");
            REQUIRE(t->propCurrent() == 2);
            REQUIRE_OUTPUT_LIST(t, 0, LA(2, "C"));
            // prev
            t <<= LA("prev");
            REQUIRE(t->propCurrent() == 1);
            REQUIRE_OUTPUT_LIST(t, 0, LA(1, "B"));
            t <<= LA("prev");
            REQUIRE(t->propCurrent() == 0);
            REQUIRE_OUTPUT_LIST(t, 0, LA(0.0, "A"));
            t <<= LA("prev");
            REQUIRE(t->propCurrent() == 0);
            REQUIRE_NO_OUTPUT(t);
        }

        SECTION("toggle")
        {
            TestExtTab t("ui.tab", LA("A", "B", "@toggle", 1));
            t <<= LA("+", 1);
            t <<= LA("-", 1);
            t <<= LA("next");
            t <<= LA("prev");
        }
    }

    SECTION("random")
    {
        SECTION("single")
        {
            TestExtTab t("ui.tab");
            t <<= LA("random");
            REQUIRE_NO_OUTPUT(t);

            t <<= LA("@items", "A");
            t <<= LA("random");
            REQUIRE_OUTPUT_LIST(t, 0, LA(0.0, "A"));
            t <<= LA("random");
            REQUIRE_OUTPUT_LIST(t, 0, LA(0.0, "A"));
        }

        SECTION("toggle")
        {
            TestExtTab t("ui.tab", LA("@toggle", 1));
            t <<= LA("random");
            REQUIRE_OUTPUT_ANY(t, 0, LA("@selected"));
        }
    }

    SECTION("flip")
    {
        SECTION("single")
        {
            TestExtTab t("ui.tab", LA("A", "B"));
            t <<= LA("flip");
            REQUIRE_NO_OUTPUT(t);
        }

        SECTION("toggle")
        {
            TestExtTab t("ui.tab", LA("A", "B", "C", "@toggle", 1));
            t->m_flip();
            REQUIRE_OUTPUT_ANY(t, 0, LA("@selected", 1, 1, 1));

            t << "A";
            t << "B";
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(0., 0., 1));
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0).back() == LF(1, 0));
            REQUIRE(t.messagesAt(0).front().anyValue() == LA("@selected", 0., 0., 1));

            t.clearAll();
            t->m_flip();
            REQUIRE_UI_LIST_PROPERTY(t, "selected", LA(1, 1, 0.));
        }
    }
}
