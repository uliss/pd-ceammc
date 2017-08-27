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
#include "../list/list_choice.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListChoice> ListChoiceTest;

TEST_CASE("list.choice", "[externals]")
{
    SECTION("test create with:")
    {
        ListChoiceTest t("list.choice");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 123);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_SYMBOL_TO(0, t, "123");
        REQUIRE_NO_MSG(t);
    }

    SECTION("choice")
    {
        ListChoiceTest t("list.choice");

        // empty list
        WHEN_SEND_LIST_TO(0, t, AtomList());
        REQUIRE_NO_MSG(t);

        // single value
        int N = 10;
        while (N-- > 0) {
            WHEN_SEND_LIST_TO(0, t, L1(11));
            REQUIRE_FLOAT_AT_OUTLET(0, t, 11);
        }

        // multiple values
        N = 10;
        while (N-- > 0) {
            WHEN_SEND_LIST_TO(0, t, L4(1, 2, 3, 4));
            REQUIRE(t.hasNewMessages(0));
            float v = t.lastMessage(0).atomValue().asFloat();
            bool ok = (v == 1 || v == 2 || v == 3 || v == 4);
            REQUIRE(ok);
        }
    }
}
