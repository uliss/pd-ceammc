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
#include "load_msg.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(LoadMsg, load, msg)

extern "C" {
EXTERN int canvas_getdollarzero(void);
}

TEST_CASE("loadmsg", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("loadmsg");
        }

        SECTION("alias")
        {
            TExt t("msg.onload");
        }
    }

    SECTION("do")
    {
        SECTION("empty")
        {
            TExt t("loadmsg");
            t->output();
            REQUIRE_BANG_AT_OUTLET(0, t);
        }

        SECTION("float")
        {
            TExt t("loadmsg", LF(-10.25));
            t->output();
            REQUIRE_FLOAT_AT_OUTLET(0, t, -10.25);
        }

        SECTION("list")
        {
            TExt t("loadmsg", LF(1, 2, 3));
            t->output();
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3));
        }

        SECTION("symbol")
        {
            TExt t("loadmsg", LA("symbol", "A"));
            t->output();
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");
        }

        SECTION("any")
        {
            TExt t("loadmsg", LA("A"));
            t->output();
            REQUIRE_ANY_AT_OUTLET(0, t, LA("A"));
        }

        SECTION("any")
        {
            TExt t("loadmsg", LA("A", 1, 2, 3));
            t->output();
            REQUIRE_ANY_AT_OUTLET(0, t, LA("A", 1, 2, 3));
        }

        SECTION("dollar $0")
        {
            TExt t("loadmsg", AtomList::parseString("$0"));
            t->output();
            REQUIRE_FLOAT_AT_OUTLET(0, t, canvas_getdollarzero());
        }

        SECTION("dollar $1")
        {
            TExt t("loadmsg", AtomList::parseString("$1"));
            t->output();
            REQUIRE_ANY_AT_OUTLET(0, t, LA("$1"));
        }

        SECTION("dollar symbol")
        {
            TExt t("loadmsg", AtomList::parseString("msg $0-args $1-args"));
            t->output();

            char buf[32];
            sprintf(buf, "%d-args", canvas_getdollarzero());
            auto arg0 = gensym(buf);

            REQUIRE_ANY_AT_OUTLET(0, t, LA("msg", arg0, "$1-args"));
        }

        SECTION("comma")
        {
            TExt t("loadmsg", AtomList::parseString("1, 2"));
            t->output();
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0] == 1);
            REQUIRE(t.messagesAt(0)[1] == 2);
        }

        SECTION("comma")
        {
            TExt t("loadmsg", AtomList::parseString("1, test 2"));
            t->output();
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0] == 1);
            REQUIRE(t.messagesAt(0)[1] == Message("test", 2));
        }

        SECTION("semicolon")
        {
            TExt t("loadmsg", AtomList::parseString("1; test 2, 1 2 3"));
            t->output();
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0] == 1);
            REQUIRE(t.messagesAt(0)[1] == Message(LF(1, 2, 3)));
        }
    }
}
