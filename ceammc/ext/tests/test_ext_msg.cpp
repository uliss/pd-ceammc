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
#include "../base/msg.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<Msg> MsgTest;

TEST_CASE("msg", "[extension]")
{
    obj_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            MsgTest t("msg", AtomList());
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_SEND_FLOAT_TO(0, t, 12);
            REQUIRE_LIST_AT_OUTLET(0, t, L1(12));

            WHEN_SEND_SYMBOL_TO(0, t, "abc");
            REQUIRE_ANY_AT_OUTLET(0, t, L1("abc"));

            WHEN_SEND_LIST_TO(0, t, L3("a", "b", 3));
            REQUIRE_ANY_AT_OUTLET(0, t, L3("a", "b", 3));

            WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, L3(1, 2, 3));

            WHEN_SEND_ANY_TO(t, "a", L2("b", "c"));
            REQUIRE_ANY_AT_OUTLET(0, t, L3("a", "b", "c"));

            // change prefix to symbol
            WHEN_SEND_SYMBOL_TO(1, t, "prepend");

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_ANY_AT_OUTLET(0, t, L1("prepend"));

            WHEN_SEND_FLOAT_TO(0, t, 11);
            REQUIRE_ANY_AT_OUTLET(0, t, L2("prepend", 11));

            WHEN_SEND_SYMBOL_TO(0, t, "b");
            REQUIRE_ANY_AT_OUTLET(0, t, L2("prepend", "b"));

            WHEN_SEND_SYMBOL_TO(0, t, "@b");
            REQUIRE_ANY_AT_OUTLET(0, t, L2("prepend", "@b"));

            WHEN_SEND_LIST_TO(0, t, L3("a", "b", "c"));
            REQUIRE_ANY_AT_OUTLET(0, t, L4("prepend", "a", "b", "c"));

            WHEN_SEND_ANY_TO(t, "a", L2("b", "c"));
            REQUIRE_ANY_AT_OUTLET(0, t, L4("prepend", "a", "b", "c"));

            // change prefix to float
            WHEN_SEND_FLOAT_TO(1, t, 123);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, L1(123));

            WHEN_SEND_FLOAT_TO(0, t, 124);
            REQUIRE_LIST_AT_OUTLET(0, t, L2(123, 124));

            WHEN_SEND_SYMBOL_TO(0, t, "a");
            REQUIRE_LIST_AT_OUTLET(0, t, L2(123, "a"));

            WHEN_SEND_SYMBOL_TO(0, t, "@b");
            REQUIRE_LIST_AT_OUTLET(0, t, L2(123, "@b"));

            WHEN_SEND_LIST_TO(0, t, L3("a", "b", "c"));
            REQUIRE_LIST_AT_OUTLET(0, t, L4(123, "a", "b", "c"));

            WHEN_SEND_ANY_TO(t, "a", L2("b", "c"));
            REQUIRE_LIST_AT_OUTLET(0, t, L4(123, "a", "b", "c"));

            // change prefix to list
            WHEN_SEND_LIST_TO(1, t, L2("a", "b"));

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_ANY_AT_OUTLET(0, t, L2("a", "b"));

            WHEN_SEND_FLOAT_TO(0, t, -100);
            REQUIRE_ANY_AT_OUTLET(0, t, L3("a", "b", -100));

            WHEN_SEND_SYMBOL_TO(0, t, "c");
            REQUIRE_ANY_AT_OUTLET(0, t, L3("a", "b", "c"));

            WHEN_SEND_SYMBOL_TO(0, t, "@c");
            REQUIRE_ANY_AT_OUTLET(0, t, L3("a", "b", "@c"));

            WHEN_SEND_LIST_TO(0, t, L3("c", "d", "e"));
            REQUIRE_ANY_AT_OUTLET(0, t, L5("a", "b", "c", "d", "e"));

            WHEN_SEND_ANY_TO(t, "c", L2("d", "e"));
            REQUIRE_ANY_AT_OUTLET(0, t, L5("a", "b", "c", "d", "e"));

            // change prefix to prop
            WHEN_SEND_LIST_TO(1, t, L1("@a"));

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_ANY_AT_OUTLET(0, t, L1("@a"));

            WHEN_SEND_FLOAT_TO(0, t, 11);
            REQUIRE_ANY_AT_OUTLET(0, t, L2("@a", 11));

            WHEN_SEND_SYMBOL_TO(0, t, "b");
            REQUIRE_ANY_AT_OUTLET(0, t, L2("@a", "b"));

            WHEN_SEND_SYMBOL_TO(0, t, "@b");
            REQUIRE_ANY_AT_OUTLET(0, t, L2("@a", "@b"));

            WHEN_SEND_LIST_TO(0, t, L3("a", "b", "c"));
            REQUIRE_ANY_AT_OUTLET(0, t, L4("@a", "a", "b", "c"));

            WHEN_SEND_ANY_TO(t, "a", L2("b", "c"));
            REQUIRE_ANY_AT_OUTLET(0, t, L4("@a", "a", "b", "c"));
        }

        SECTION("symbol arg")
        {
            MsgTest t("msg", L1("set"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_ANY_AT_OUTLET(0, t, L1("set"));

            WHEN_SEND_FLOAT_TO(0, t, 12);
            REQUIRE_ANY_AT_OUTLET(0, t, L2("set", 12));

            WHEN_SEND_SYMBOL_TO(0, t, "abc");
            REQUIRE_ANY_AT_OUTLET(0, t, L2("set", "abc"));

            WHEN_SEND_LIST_TO(0, t, L3("a", "b", 3));
            REQUIRE_ANY_AT_OUTLET(0, t, L4("set", "a", "b", 3));

            WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
            REQUIRE_ANY_AT_OUTLET(0, t, L4("set", 1, 2, 3));
        }
    }
}
