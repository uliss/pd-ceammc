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
#include "flow_prepend.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowPrepend, flow, prepend)

TEST_CASE("flow.prepend", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("flow.prepend");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @msg, L());
        }

        SECTION("args")
        {
            TExt t("flow.prepend", LA(1, "\"@prop\""));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @msg, LA(1, "@prop"));
        }

        SECTION("props")
        {
            TExt t("flow.prepend", LA("@msg", 1, "\"@msg\""));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @msg, LA(1, "@msg"));
        }
    }

    SECTION("process")
    {
        SECTION("default")
        {
            TExt t("flow.prepend");
            t.sendFloat(100);

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].isBang());
            REQUIRE(t.messagesAt(0)[1].atomValue() == A(100));
        }

        SECTION("float")
        {
            TExt t("flow.prepend", 200);
            t.sendFloat(100);

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].atomValue() == A(200));
            REQUIRE(t.messagesAt(0)[1].atomValue() == A(100));
        }

        SECTION("float explicit")
        {
            TExt t("flow.prepend", LA("float", 200));
            t.sendFloat(100);

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].atomValue() == A(200));
            REQUIRE(t.messagesAt(0)[1].atomValue() == A(100));
        }

        SECTION("symbol")
        {
            TExt t("flow.prepend", LA("symbol", "A"));
            t.sendFloat(100);

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].atomValue() == "A");
            REQUIRE(t.messagesAt(0)[1].atomValue() == A(100));
        }

        SECTION("symbol prop")
        {
            TExt t("flow.prepend", LA("symbol", "\"@A\""));
            t.sendFloat(100);

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].atomValue() == A("@A"));
            REQUIRE(t.messagesAt(0)[1].atomValue() == A(100));
        }

        SECTION("list implicit")
        {
            TExt t("flow.prepend", LF(1, 2, 3));
            t.sendFloat(100);

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].listValue() == LF(1, 2, 3));
            REQUIRE(t.messagesAt(0)[1].atomValue() == A(100));
        }

        SECTION("list explicit")
        {
            TExt t("flow.append", LA("list", 1, 2, 3));
            t.sendFloat(100);

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].atomValue() == A(100));
            REQUIRE(t.messagesAt(0)[1].listValue() == LF(1, 2, 3));
        }

        SECTION("any")
        {
            TExt t("flow.append", LA("any"));
            t.sendFloat(100);

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].atomValue() == A(100));
            REQUIRE(t.messagesAt(0)[1].isAny());
            REQUIRE(t.messagesAt(0)[1].anyValue() == LA("any"));
        }

        SECTION("any2")
        {
            TExt t("flow.append", LA("any", 1, 2, 3));
            t.sendFloat(100);

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].atomValue() == A(100));
            REQUIRE(t.messagesAt(0)[1].isAny());
            REQUIRE(t.messagesAt(0)[1].anyValue() == LA("any", 1, 2, 3));
        }

        SECTION("any prop")
        {
            TExt t("flow.append", LA("\"@any\"", 1, 2, 3));
            t.sendFloat(100);

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].atomValue() == A(100));
            REQUIRE(t.messagesAt(0)[1].isAny());
            REQUIRE(t.messagesAt(0)[1].anyValue() == LA("@any", 1, 2, 3));
            t.clearAll();

            t.sendMessageTo(Message(SYM("@any2"), LF(3, 4, 5)), 1);
            t.sendFloat(200);

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].atomValue() == A(200));
            REQUIRE(t.messagesAt(0)[1].anyValue() == LA("@any2", 3, 4, 5));
        }

        SECTION("all")
        {
            TExt t("flow.append", LF(1, 2, 3));

            t.clearAll();
            t.sendMessageTo(Message::makeBang(), 1);
            t.sendFloat(100);

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].atomValue() == A(100));
            REQUIRE(t.messagesAt(0)[1].isBang());

            t.clearAll();
            t.sendMessageTo(Message(-50), 1);
            t.sendBang();

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].isBang());
            REQUIRE(t.messagesAt(0)[1].atomValue() == A(-50));

            t.clearAll();
            t.sendMessageTo(Message(SYM("ABC")), 1);
            t.sendList(LF(1, 2, 3));

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].listValue() == LF(1, 2, 3));
            REQUIRE(t.messagesAt(0)[1].atomValue() == A("ABC"));

            t.clearAll();
            t.sendMessageTo(Message(LF(20, 30)), 1);
            t.sendSymbol("DEF");

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].atomValue() == A("DEF"));
            REQUIRE(t.messagesAt(0)[1].listValue() == LF(20, 30));

            t.clearAll();
            t.sendMessageTo(Message(SYM("any"), LF(20, 30)), 1);
            t.sendSymbol("DEF");

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].atomValue() == A("DEF"));
            REQUIRE(t.messagesAt(0)[1].anyValue() == LA("any", 20, 30));

            t.clearAll();
            t.sendMessageTo(Message(SYM("@prop"), LF(20, 30)), 1);
            t.sendSymbol("DEF");

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].atomValue() == A("DEF"));
            REQUIRE(t.messagesAt(0)[1].anyValue() == LA("@prop", 20, 30));

            t.clearAll();
            t.sendMessageTo(Message(SYM("@gate"), LF(0)), 1);
            t.sendMessage(Message(SYM("@gate"), LF(1)));

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].anyValue() == LA("@gate", 1));
            REQUIRE(t.messagesAt(0)[1].anyValue() == LA("@gate", 0.));

            t.clearAll();
            t.sendMessageTo(Message(SYM("@gate"), LF(0)), 1);
            t.sendMessage(Message(SYM("@delay"), LF(1000)));

            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0)[0].anyValue() == LA("@delay", 1000));
            REQUIRE(t.messagesAt(0)[1].anyValue() == LA("@gate", 0.));
        }
    }
}
