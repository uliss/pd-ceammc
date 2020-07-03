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
#include "list_prepend.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListPrepend, list, prepend)

using TExt = TestExtListPrepend;
using TObj = TestListPrepend;

TEST_CASE("list.prepend", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TObj t("list.prepend");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("empty")
    {
        TObj t("list.prepend");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_THAT(t, outputFloat(&t, 10));

        WHEN_SEND_SYMBOL_TO(0, t, "ABC");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "ABC");

        WHEN_SEND_DATA_TO(0, t, IntData(12));
        REQUIRE_DATA_AT_OUTLET(0, t, IntA(12));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2));
    }

    SECTION("args")
    {
        TObj t("list.prepend", LA("@prop", 1));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LA("@prop", 1));

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_LIST_AT_OUTLET(0, t, LA("@prop", 1, 10));

        WHEN_SEND_SYMBOL_TO(0, t, "ABC");
        REQUIRE_LIST_AT_OUTLET(0, t, LA("@prop", 1, "ABC"));

        WHEN_SEND_DATA_TO(0, t, IntData(12));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("@prop", 1, IntA(12)));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, LA("@prop", 1));

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("@prop", 1, 1, 2));

        WHEN_SEND_LIST_TO(1, t, LF(100));
        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(100, 1, 2));
    }

    SECTION("data")
    {
        IntA d1(10);
        IntA d2(20);
        IntA d3(30);

        TExt t("list.prepend");

        t->onInlet(1, LA(d1, d2));
        t.send(d3);
        REQUIRE(t.outputListAt(0) == LA(d1, d2, d3));

        t->onInlet(1, LA("a", "b"));
        t.send(MLA());
        REQUIRE(t.isOutputDataAt(0));
        REQUIRE(t.outputAtomAt(0) == MLA("a", "b"));

        t.send(MLA(1, 2, 3, MLA()));
        REQUIRE(t.outputAtomAt(0) == MLA("a", "b", 1, 2, 3, MLA()));
    }
}
