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
#include "msg_unpack.h"
#include "test_base.h"
#include "test_catch2.hpp"

PD_COMPLETE_TEST_SETUP(MsgUnpack, base, msg_unpack)

TEST_CASE("msg.unpack", "[extension]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TestExtMsgUnpack t("msg.unpack");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @n, 1);
        }

        SECTION("args: 1")
        {
            TestExtMsgUnpack t("msg.unpack", 1);
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @n, 1);
        }

        SECTION("args: 2")
        {
            TestExtMsgUnpack t("msg.unpack", 2);
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY_FLOAT(t, @n, 2);
        }

        SECTION("@n 3")
        {
            TestExtMsgUnpack t("msg.unpack", LA("@n", 3));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 3);
            REQUIRE_PROPERTY_FLOAT(t, @n, 3);
        }
    }

    SECTION("unpack")
    {
        SECTION("1")
        {
            TestExtMsgUnpack t("msg.unpack");

            t.bang();
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "bang");

            t << 100;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "float");

            t << gensym("test");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "symbol");

            t << LF(1, 2, 3);
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "list");

            t.sendMessage("msg", LA(1, 2, 3));
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "msg");
        }

        SECTION("3")
        {
            TestExtMsgUnpack t("msg.unpack", 3);

            t.bang();
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "bang");
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
            REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);

            t << 100;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "float");
            REQUIRE_FLOAT_AT_OUTLET(1, t, 100);
            REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);

            t << gensym("test");
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "symbol");
            REQUIRE_SYMBOL_AT_OUTLET(1, t, "test");
            REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
            t.clearAll();

            t << L();
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "list");
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
            REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);

            t << LF(1);
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "list");
            REQUIRE_FLOAT_AT_OUTLET(1, t, 1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);

            t << LF(1, 2);
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "list");
            REQUIRE_FLOAT_AT_OUTLET(1, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 2);

            t << LF(1, 2, 3);
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "list");
            REQUIRE_FLOAT_AT_OUTLET(1, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 2);

            t << LF(1, 2, 3, 4);
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "list");
            REQUIRE_FLOAT_AT_OUTLET(1, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 2);
            t.clearAll();

            t.sendMessage("msg", L());
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "msg");
            REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
            REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);

            t.sendMessage("msg", LA(1));
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "msg");
            REQUIRE_FLOAT_AT_OUTLET(1, t, 1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);

            t.sendMessage("msg", LA(1, 2));
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "msg");
            REQUIRE_FLOAT_AT_OUTLET(1, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 2);

            t.sendMessage("msg", LA(1, 2, 3));
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "msg");
            REQUIRE_FLOAT_AT_OUTLET(1, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 2);

            t.sendMessage("msg", LA(1, 2, 3, 4, 5));
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "msg");
            REQUIRE_FLOAT_AT_OUTLET(1, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 2);
        }
    }
}
