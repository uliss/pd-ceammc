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
#include "conv_list2props.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(ConvList2Props, list, to_props)

TEST_CASE("conv.list2props", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestConvList2Props t("list->props");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("list")
    {
        TestConvList2Props t("list->props");

        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5));
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LA("a", "b", "@c"));
        REQUIRE(t.messageCount() == 1);
        REQUIRE(t.messageAt(0, 0).anyValue() == LA("@c"));
        t.cleanAllMessages();

        WHEN_SEND_LIST_TO(0, t, LA("a", "b", "@c", 100));
        REQUIRE(t.messageCount() == 1);
        REQUIRE(t.messageAt(0, 0).anyValue() == LA("@c", 100));
        t.cleanAllMessages();

        WHEN_SEND_LIST_TO(0, t, LA("a", "b", "@d", 100, 200));
        REQUIRE(t.messageCount() == 1);
        REQUIRE(t.messageAt(0, 0).anyValue() == LA("@d", 100, 200));
        t.cleanAllMessages();

        WHEN_SEND_LIST_TO(0, t, LA("a", "@b", "@d", 100, 200));
        REQUIRE(t.messageCount() == 2);
        REQUIRE(t.messageAt(0, 0).anyValue() == LA("@b"));
        REQUIRE(t.messageAt(1, 0).anyValue() == LA("@d", 100, 200));
        t.cleanAllMessages();

        WHEN_SEND_LIST_TO(0, t, LA("@a", "A", "@b", "@d", 100, 200));
        REQUIRE(t.messageCount() == 3);
        REQUIRE(t.messageAt(0, 0).anyValue() == LA("@a", "A"));
        REQUIRE(t.messageAt(1, 0).anyValue() == LA("@b"));
        REQUIRE(t.messageAt(2, 0).anyValue() == LA("@d", 100, 200));
        t.cleanAllMessages();
    }

    SECTION("any")
    {
        TestConvList2Props t("any->props");

        WHEN_SEND_ANY_TO(t, LA("a", "b", "@c"));
        REQUIRE(t.messageCount() == 1);
        REQUIRE(t.messageAt(0, 0).anyValue() == LA("@c"));
        t.cleanAllMessages();

        WHEN_SEND_ANY_TO(t, LA("a", "b", "@c", 100));
        REQUIRE(t.messageCount() == 1);
        REQUIRE(t.messageAt(0, 0).anyValue() == LA("@c", 100));
        t.cleanAllMessages();

        WHEN_SEND_ANY_TO(t, LA("a", "b", "@d", 100, 200));
        REQUIRE(t.messageCount() == 1);
        REQUIRE(t.messageAt(0, 0).anyValue() == LA("@d", 100, 200));
        t.cleanAllMessages();

        WHEN_SEND_ANY_TO(t, LA("a", "@b", "@d", 100, 200));
        REQUIRE(t.messageCount() == 2);
        REQUIRE(t.messageAt(0, 0).anyValue() == LA("@b"));
        REQUIRE(t.messageAt(1, 0).anyValue() == LA("@d", 100, 200));
        t.cleanAllMessages();

        WHEN_SEND_ANY_TO(t, LA("@a", "A", "@b", "@d", 100, 200));
        REQUIRE(t.messageCount() == 3);
        REQUIRE(t.messageAt(0, 0).anyValue() == LA("@a", "A"));
        REQUIRE(t.messageAt(1, 0).anyValue() == LA("@b"));
        REQUIRE(t.messageAt(2, 0).anyValue() == LA("@d", 100, 200));
        t.cleanAllMessages();
    }

    SECTION("aliases")
    {
        TestExtConvList2Props t1("conv.list2props");
        REQUIRE(t1.object());

        TestExtConvList2Props t2("list->props");
        REQUIRE(t2.object());

        TestExtConvList2Props t3("any->props");
        REQUIRE(t3.object());
    }
}
