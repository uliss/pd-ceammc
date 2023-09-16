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
#include "ui_bang.h"

#include "test_ui.h"
#include <cstdarg>

UI_COMPLETE_TEST_SETUP(Bang)

extern "C" int canvas_getdollarzero(void);
static char BUFFER[MAXPDSTRING];
t_symbol* print_sym(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(BUFFER, sizeof(BUFFER), fmt, ap);
    va_end(ap);
    return gensym(BUFFER);
}

TEST_CASE("ui.bang", "[ui.bang]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestBang t("ui.bang");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 1);
        REQUIRE_UI_LIST_PROPERTY(t, "send", LA("(null)"));
        REQUIRE_UI_LIST_PROPERTY(t, "receive", LA("(null)"));

        REQUIRE_UI_LIST_PROPERTY(t, "background_color", LX(0.93, 0.93, 0.93, 1));
    }

    SECTION("label")
    {
        TestExtBang t("ui.bang");
        t.call("@label", LA("ABC"));
        REQUIRE_UI_LIST_PROPERTY(t, "label", LA("ABC"));

        t.call("@label", LA("A B"));
        REQUIRE_UI_LIST_PROPERTY(t, "label", LA("A B"));

        t.call("@label", LA("A", "B", "C"));
        REQUIRE_UI_LIST_PROPERTY(t, "label", LA("A B C"));

        t.call("@label", LA("A", 1.5, 2));
        REQUIRE_UI_LIST_PROPERTY(t, "label", LA("A 1.5 2"));
    }

    SECTION("external")
    {
        TestExtBang t("ui.bang");

        t.bang();
        REQUIRE_OUTPUT_BANG(t, 0);
        t.send(10);
        REQUIRE_OUTPUT_BANG(t, 0);
        t.send(gensym("ABC"));
        REQUIRE_OUTPUT_BANG(t, 0);
        t.send(L());
        REQUIRE_OUTPUT_BANG(t, 0);
        t.send(LF(1));
        REQUIRE_OUTPUT_BANG(t, 0);
        t.send(LF(1, 2, 3));
        REQUIRE_OUTPUT_BANG(t, 0);
        t.call("a");
        REQUIRE_OUTPUT_BANG(t, 0);

        t.mouseDown(5, 5);
        REQUIRE_OUTPUT_BANG(t, 0);
        t.mouseUp(5, 5);
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("send test")
    {
        TestExtBang t("ui.bang", LA("@send", "r1"));
        t.addListener("r1");

        t << BANG;
        REQUIRE_BANG_WAS_SEND(t, "r1");
        REQUIRE_NONE_WAS_SEND(t, "r1");

        t << 20;
        REQUIRE_BANG_WAS_SEND(t, "r1");

        t << "ABC";
        REQUIRE_BANG_WAS_SEND(t, "r1");

        t << L();
        REQUIRE_BANG_WAS_SEND(t, "r1");

        t << LF(100);
        REQUIRE_BANG_WAS_SEND(t, "r1");

        t << LA(1, 2, 3, 4, 5);
        REQUIRE_BANG_WAS_SEND(t, "r1");

        t.call("any");
        REQUIRE_BANG_WAS_SEND(t, "r1");

        t.mouseDown(5, 5);
        REQUIRE_BANG_WAS_SEND(t, "r1");
    }

    SECTION("send raute test")
    {
        TestExtBang t("ui.bang", LA("@send", "r1-#0"));

        auto s = canvas_realizedollar(t->canvas(), gensym("r1-$0"));
        t.addListener(s->s_name);

        t << BANG;
        REQUIRE_BANG_WAS_SEND(t, s->s_name);
    }

    SECTION("receive test")
    {
        TestExtBang t("ui.bang", LA("@receive", "r2"));

        pd_bang(gensym("r2")->s_thing);
        REQUIRE_OUTPUT_BANG(t, 0);
    }

    SECTION("receive raute test")
    {
        TestExtBang t("ui.bang", LA("@receive", "r2-#0"));

        pd_bang(canvas_realizedollar(t->canvas(), gensym("r2-$0"))->s_thing);
        REQUIRE_OUTPUT_BANG(t, 0);
    }

    SECTION("send/receive loop test")
    {
        TestExtBang t("ui.bang", LA("@receive", "r3", "@send", "r3"));

        t.bang();
        REQUIRE_OUTPUT_BANG(t, 0);

        pd_bang(gensym("r3")->s_thing);
        REQUIRE_OUTPUT_BANG(t, 0);
    }

    SECTION("$0 test")
    {
        SECTION("receive")
        {
            TestExtBang t("ui.bang", LA("@receive", "#0-r", "@send", "#0-s"));
            REQUIRE_UI_LIST_PROPERTY(t, "receive", LA("#0-r"));
            REQUIRE_UI_LIST_PROPERTY(t, "send", LA("#0-s"));

            REQUIRE(gensym("#0-r")->s_thing == nullptr);
            REQUIRE(print_sym("%d-r", canvas_getdollarzero())->s_thing == t->asPd());

            auto send_sym = print_sym("%d-s", canvas_getdollarzero());
            REQUIRE(send_sym->s_thing == nullptr);
            t.addListener(print_sym("%d-s", canvas_getdollarzero()));
            REQUIRE(send_sym->s_thing == ebox_getsender(t->asEBox()));
        }

        auto send_sym = print_sym("%d-s", canvas_getdollarzero());
        REQUIRE(send_sym->s_thing == nullptr);

        SECTION("@label")
        {
            TestExtBang t("ui.bang", LA("@label", "label: #0"));
            REQUIRE_UI_LIST_PROPERTY(t, "label", LA("label: #0"));
        }
    }
}
