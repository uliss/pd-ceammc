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
#include "../base/function.h"
#include "flow_change.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowChange, flow, change)

typedef TestExternal<Function> FunctionTest;

TEST_CASE("flow.change", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("flow.change");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @onrepeat, "");
    }

    SECTION("process")
    {
        TObj t("flow.change");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 2.1f);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2.1);

        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "a");

        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_SYMBOL_TO(0, t, "b");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "b");

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2));

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 3));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 3));

        WHEN_SEND_ANY_TO(t, "any", LF(3, 4));
        REQUIRE_ANY_AT_OUTLET(0, t, LA("any", 3, 4));
    }

    SECTION("reset")
    {
        TObj t("flow.change");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        t.m_reset({});
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);
    }

    SECTION("set")
    {
        TObj t("flow.change");

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        t.m_set(LF(1));
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);

        t.m_set(LF(1, 2, 3));
        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3, 4));
    }

    SECTION("@onrepeat")
    {
        FunctionTest cb1("function", LA("test_callback1"));
        FunctionTest cb2("function", LA("test_callback2"));

#define STORE()                     \
    {                               \
        cb1.storeAllMessageCount(); \
        cb2.storeAllMessageCount(); \
    }

        REQUIRE(Function::exists(gensym("test_callback1")));
        REQUIRE(Function::exists(gensym("test_callback2")));

        TObj t("flow.change", LA("@onrepeat", "test_callback1"));
        REQUIRE_PROPERTY(t, @onrepeat, gensym("test_callback1"));

        SECTION("float")
        {
            // float
            STORE();
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 100);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);
            // repeat
            STORE();
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            REQUIRE_BANG_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);

            // repeat again
            STORE();
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            REQUIRE_BANG_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);

            // new float
            STORE();
            WHEN_SEND_FLOAT_TO(0, t, 200);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 200);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);
        }

        SECTION("symbol")
        {
            STORE();
            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "ABC");
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);
            // repeat
            STORE();
            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            REQUIRE_BANG_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);

            // repeat again
            STORE();
            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            REQUIRE_BANG_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);

            // new symbol
            STORE();
            WHEN_SEND_SYMBOL_TO(0, t, "ABCD");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "ABCD");
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);
        }

        SECTION("list")
        {
            STORE();
            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3));
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);
            // change callback
            t.setProperty("@onrepeat", LA("test_callback2"));
            REQUIRE_PROPERTY(t, @onrepeat, gensym("test_callback2"));
            // repeat
            STORE();
            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb1);
            REQUIRE_BANG_AT_OUTLET(1, cb2);

            // repeat again
            STORE();
            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb1);
            REQUIRE_BANG_AT_OUTLET(1, cb2);

            // new list
            STORE();
            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3, 4));
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);
        }

        SECTION("any")
        {
            STORE();
            WHEN_SEND_ANY_TO(t, "any1", LF(1));
            REQUIRE_ANY_AT_OUTLET(0, t, LA("any1", 1));
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);

            // repeat
            STORE();
            WHEN_SEND_ANY_TO(t, "any1", LF(1));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            REQUIRE_BANG_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);

            // repeat again
            STORE();
            WHEN_SEND_ANY_TO(t, "any1", LF(1));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            REQUIRE_BANG_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);

            // new list
            STORE();
            WHEN_SEND_ANY_TO(t, "any2", LF(1));
            REQUIRE_ANY_AT_OUTLET(0, t, LA("any2", 1));
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, cb2);
        }

        SECTION("invalid callback")
        {
            TObj t("flow.change", LA("@onrepeat", "test_callback????"));
            REQUIRE_PROPERTY(t, @onrepeat, gensym("test_callback????"));

            STORE();
            WHEN_SEND_FLOAT_TO(0, t, 100);
            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }
    }
}
