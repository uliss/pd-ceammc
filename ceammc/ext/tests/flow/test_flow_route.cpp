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
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "flow_route.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowRoute, flow, route)

TEST_CASE("flow.route", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            TObj t("flow.route");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("args")
        {
            TObj t("flow.route", LA("test"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
        }

        SECTION("args")
        {
            TObj t("flow.route", LA("a", "b", "c", "d"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 5);
        }

        SECTION("props")
        {
            TObj t("flow.route", LA("a", "@b", "c", "@d"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 5);
        }
    }

    SECTION("list")
    {
        SECTION("empty")
        {
            TObj t("flow.route");
            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3));
        }

        TObj t("flow.route", LA("a", "b", 500));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // no match
        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_LIST_AT_OUTLET(3, t, LF(1, 2, 3));

        // match first
        WHEN_SEND_LIST_TO(0, t, LA("a"));
        REQUIRE_LIST_AT_OUTLET(0, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_LIST_TO(0, t, LA("a", "b"));
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "b");
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c", "d"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("b", "c", "d"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match second
        WHEN_SEND_LIST_TO(0, t, LA("b"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST_AT_OUTLET(1, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_LIST_TO(0, t, LA("b", "c"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "c");
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_LIST_TO(0, t, LA("b", "c", "d"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST_AT_OUTLET(1, t, LA("c", "d"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match third
        WHEN_SEND_LIST_TO(0, t, LF(500));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_LIST_AT_OUTLET(2, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_LIST_TO(0, t, LA(500, "message"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_SYMBOL_AT_OUTLET(2, t, "message");
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_LIST_TO(0, t, LA(500, "a", 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_LIST_AT_OUTLET(2, t, LA("a", 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);
    }

    SECTION("any")
    {
        SECTION("empty")
        {
            TObj t("flow.route");
            WHEN_SEND_ANY_TO(t, LA("any"));
            REQUIRE_ANY_AT_OUTLET(0, t, LA("any"));
        }

        TObj t("flow.route", LA("a", "b"));

        // no match
        WHEN_SEND_ANY_TO(t, LA("empty"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_ANY_AT_OUTLET(2, t, LA("empty"));

        WHEN_SEND_ANY_TO(t, LA("msg", "a", "b", "c"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_ANY_AT_OUTLET(2, t, LA("msg", "a", "b", "c"));

        // match first
        WHEN_SEND_ANY_TO(t, LA("a"));
        REQUIRE_LIST_AT_OUTLET(0, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);

        WHEN_SEND_ANY_TO(t, LA("a", "b", "c"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("b", "c"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);

        // match second
        WHEN_SEND_ANY_TO(t, LA("b"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST_AT_OUTLET(1, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);

        WHEN_SEND_ANY_TO(t, LA("b", "c", "d"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST_AT_OUTLET(1, t, LA("c", "d"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
    }

    SECTION("data:mlist")
    {
        SECTION("empty")
        {
            TObj t("flow.route");

            WHEN_SEND_DATA_TO(0, t, MListD(1, 2, 3));
            REQUIRE_DATA_AT_OUTLET(0, t, MListA(1, 2, 3));
        }

        TObj t("flow.route", LA("a", "b", 100));

        // empty list
        WHEN_SEND_DATA_TO(0, t, MListD());
        REQUIRE_NO_MSG(t);

        // no match
        WHEN_SEND_DATA_TO(0, t, MListD(1, 2, 3));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_DATA_AT_OUTLET(3, t, MListA(1, 2, 3));

        // match first
        WHEN_SEND_DATA_TO(0, t, MListD("a"));
        REQUIRE_DATA_AT_OUTLET(0, t, MListA());
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_DATA_TO(0, t, MListD("a", "b", "c"));
        REQUIRE_DATA_AT_OUTLET(0, t, MListA("b", "c"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match second
        WHEN_SEND_DATA_TO(0, t, MListD("b"));
        REQUIRE_DATA_AT_OUTLET(1, t, MListA());
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_DATA_TO(0, t, MListD("b", "c", "d"));
        REQUIRE_DATA_AT_OUTLET(1, t, MListA("c", "d"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match third
        WHEN_SEND_DATA_TO(0, t, MListD(100));
        REQUIRE_DATA_AT_OUTLET(2, t, MListA());
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_DATA_TO(0, t, MListD(100, 200));
        REQUIRE_DATA_AT_OUTLET(2, t, MListA(200));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);
    }

    SECTION("data:dict")
    {
        SECTION("empty")
        {
            TObj t("flow.route");

            WHEN_SEND_DATA_TO(0, t, DictD("[a: b]"));
            REQUIRE_DATA_AT_OUTLET(0, t, DictA("[a: b]"));
        }

        TObj t("flow.route", LA("a", "b", 100));

        // no match
        WHEN_SEND_DATA_TO(0, t, DictD());
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_DATA_AT_OUTLET(3, t, DictA());

        WHEN_SEND_DATA_TO(0, t, DictD("[x: 1 y: 2]"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_DATA_AT_OUTLET(3, t, DictA("[x: 1 y: 2]"));

        // match 1
        WHEN_SEND_DATA_TO(0, t, DictD("[x: 1 b: 1000]"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 1000);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_DATA_TO(0, t, DictD("[x: 1 b: 1 2 3]"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST_AT_OUTLET(1, t, LF(1, 2, 3));
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match 1 data
        WHEN_SEND_DATA_TO(0, t, DictD("[x: 1 b: IntData(100)]"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_DATA_AT_OUTLET(1, t, IntA(100));
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match 0 and 2
        WHEN_SEND_DATA_TO(0, t, DictD("[a: 1000 b: 123]"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1000);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 123);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);
    }

    SECTION("other data")
    {
        TObj t("flow.route", LA("a", "b", 100));

        WHEN_SEND_DATA_TO(0, t, IntData(1000));
        REQUIRE_DATA_AT_OUTLET(3, t, IntA(1000));
    }

    SECTION("props")
    {
        TExt t("flow.route", LA("@a", "@b"));

        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 3);

        t << LA("@a", 1, 2);

        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LF(1, 2));
    }

    SECTION("*")
    {
        TExt t("flow.route", LA("a", "*b", "@c", "*@d", 100, "*200", "*"));

        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 8);

        t << LA("a");
        REQUIRE(t.messagesAt(0).back() == Message(L()));
        t << LA("a", "b");
        REQUIRE(t.messagesAt(0).back() == Message(LA("b")));
        t.sendMessage("a");
        REQUIRE(t.messagesAt(0).back() == Message(L()));
        t.sendMessage("a", LA("b"));
        REQUIRE(t.messagesAt(0).back() == Message(LA("b")));

        t << LA("b");
        REQUIRE(t.messagesAt(1).back() == Message(LA("b")));
        t << LA("b", "c");
        REQUIRE(t.messagesAt(1).back() == Message(LA("b", "c")));
        t.sendMessage("b");
        REQUIRE(t.messagesAt(1).back() == Message(SYM("b"), L()));
        t.sendMessage("b", LA("c"));
        REQUIRE(t.messagesAt(1).back() == Message(SYM("b"), LA("c")));

        t << LA("@c");
        REQUIRE(t.messagesAt(2).back() == Message(L()));
        t << LA("@c", "d");
        REQUIRE(t.messagesAt(2).back() == Message(LA("d")));
        t.sendMessage("@c");
        REQUIRE(t.messagesAt(2).back() == Message(L()));
        t.sendMessage("@c", LA("d"));
        REQUIRE(t.messagesAt(2).back() == Message(LA("d")));

        t << LA("@d");
        REQUIRE(t.messagesAt(3).back() == Message(LA("@d")));
        t << LA("@d", "e");
        REQUIRE(t.messagesAt(3).back() == Message(LA("@d", "e")));
        t.sendMessage("@d");
        REQUIRE(t.messagesAt(3).back() == Message(SYM("@d"), L()));
        t.sendMessage("@d", LA("e"));
        REQUIRE(t.messagesAt(3).back() == Message(SYM("@d"), LA("e")));

        t << LF(100);
        REQUIRE(t.messagesAt(4).back() == Message(L()));
        t << LF(100, 200);
        REQUIRE(t.messagesAt(4).back() == Message(LF(200)));

        t << LF(200);
        REQUIRE(t.messagesAt(5).back() == Message(LF(200)));
        t << LF(200, 300);
        REQUIRE(t.messagesAt(5).back() == Message(LF(200, 300)));

        t << LA("*");
        REQUIRE(t.messagesAt(6).back() == Message(L()));
        t.sendMessage("*");
        REQUIRE(t.messagesAt(6).back() == Message(L()));
    }

    SECTION("float")
    {
        TExt t("flow.route", LA(1, 20, "*30"));

        t << 1;
        REQUIRE(t.messagesAt(0).back() == Message(L()));
        t << 20;
        REQUIRE(t.messagesAt(1).back() == Message(L()));
        t << 30;
        REQUIRE(t.messagesAt(2).back() == Message(30));
        t << 40;
        REQUIRE(t.messagesAt(3).back() == Message(40));
    }
}
