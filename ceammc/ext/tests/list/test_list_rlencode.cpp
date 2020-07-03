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
#include "list_rlencode.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListRLEncode, list, rlencode)

using TExt = TestExtListRLEncode;
using TObj = TestListRLEncode;

TEST_CASE("list.rlencode", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestListRLEncode t("list.rlencode");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("do")
    {
        TestListRLEncode t("list.rlencode");

#define REQUIRE_ENCODE(t, in, out1, out2)           \
    {                                               \
        WHEN_SEND_LIST_TO(0, t, in);                \
        REQUIRE_THAT(t, hasOutput(&t));               \
        REQUIRE(t.lastMessage(0) == Message(out1)); \
        REQUIRE(t.hasNewMessages(1));               \
        REQUIRE(t.lastMessage(1) == Message(out2)); \
    }

        REQUIRE_ENCODE(t, L(), L(), L());
        REQUIRE_ENCODE(t, LA(1), LA(1), LA(1));
        REQUIRE_ENCODE(t, LA(1, 1), LA(1), LA(2));
        REQUIRE_ENCODE(t, LA(1, 1, 1), LA(1), LA(3));
        REQUIRE_ENCODE(t, LA(1, 1, 1, 1), LA(1), LA(4));
        REQUIRE_ENCODE(t, LA("a", "b", "c", "c"), LA("a", "b", "c"), LF(1, 1, 2));
    }
}
