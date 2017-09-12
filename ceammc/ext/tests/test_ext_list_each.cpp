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
#include "../list/list_each.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListEach> ListEachTest;

TEST_CASE("list.each", "[externals]")
{
    obj_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            ListEachTest t("list.each", AtomList());
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);

            REQUIRE_PROPERTY(t, @step, 1);

            WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());
            REQUIRE(t.messageCount(1) == 3);
            REQUIRE(t.messageAt(0, 1).atomValue().asFloat() == 1);
            REQUIRE(t.messageAt(1, 1).atomValue().asFloat() == 2);
            REQUIRE(t.messageAt(2, 1).atomValue().asFloat() == 3);
        }

        SECTION("properties")
        {
            ListEachTest t("list.each", L2("@step", 2));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);

            REQUIRE_PROPERTY(t, @step, 2);

            WHEN_SEND_LIST_TO(0, t, L4(1, 2, 3, 4));
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());
            REQUIRE(t.messageCount(1) == 2);
            REQUIRE(t.messageAt(0, 1).listValue() == L2(1, 2));
            REQUIRE(t.messageAt(1, 1).listValue() == L2(3, 4));

            t.cleanAllMessages();

            WHEN_SEND_LIST_TO(0, t, L5(1, 2, 3, 4, 5));
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());
            REQUIRE(t.messageCount(1) == 3);
            REQUIRE(t.messageAt(0, 1).listValue() == L2(1, 2));
            REQUIRE(t.messageAt(1, 1).listValue() == L2(3, 4));
            REQUIRE(t.messageAt(2, 1).listValue() == L1(5));
        }

        SECTION("positional arguments")
        {
            ListEachTest t("list.each", L1(12));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);

            REQUIRE_PROPERTY(t, @step, 12);
        }

        SECTION("positional arguments and props mixed")
        {
            ListEachTest t("list.each", L3(12, "@step", 4));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);

            // property arguments have priority
            REQUIRE_PROPERTY(t, @step, 4);
        }

        SECTION("invalid args")
        {
            SECTION("props")
            {
                ListEachTest t("list.each", L2("@prop", -100));
                REQUIRE(t.numInlets() == 2);
                REQUIRE(t.numOutlets() == 2);

                // default value
                REQUIRE_PROPERTY(t, @step, 1);
            }

            SECTION("positional")
            {
                ListEachTest t("list.each", L1(-100));
                REQUIRE(t.numInlets() == 2);
                REQUIRE(t.numOutlets() == 2);

                // default value
                REQUIRE_PROPERTY(t, @step, 1);
            }
        }
    }
}
