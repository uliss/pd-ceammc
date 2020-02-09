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
#include "../list/list_append.h"
#include "datatype_mlist.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListAppend, list, append)

TEST_CASE("list.append", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestListAppend t("list.append");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("empty")
    {
        TestListAppend t("list.append");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 10);

        WHEN_SEND_SYMBOL_TO(0, t, "ABC");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "ABC");

        DataPtr d0(new IntData(12));
        WHEN_SEND_DATA_TO(0, t, d0);
        REQUIRE_DATA_AT_OUTLET(0, t, d0);

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2));
    }

    SECTION("args")
    {
        TestListAppend t("list.append", LA("@prop", 1));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LA("@prop", 1));

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_LIST_AT_OUTLET(0, t, LA(10, "@prop", 1));

        WHEN_SEND_SYMBOL_TO(0, t, "ABC");
        REQUIRE_LIST_AT_OUTLET(0, t, LA("ABC", "@prop", 1));

        DataPtr d0(new IntData(12));
        WHEN_SEND_DATA_TO(0, t, d0);
        REQUIRE_LIST_AT_OUTLET(0, t, LA(d0, "@prop", 1));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_LIST_AT_OUTLET(0, t, LA("@prop", 1));

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(1, 2, "@prop", 1));

        WHEN_SEND_LIST_TO(1, t, LF(100));
        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 100));
    }

    SECTION("data")
    {
        DataPtr d1(new IntData(10));
        DataPtr d2(new IntData(20));
        DataPtr d3(new IntData(30));

        TestExtListAppend t("list.append");

        t->onInlet(1, LA(d1, d2));
        t.send(d3);
        REQUIRE(t.outputListAt(0) == LA(d3, d1, d2));

        t->onInlet(1, LA("a", "b"));
        t.send(DataTypeMList());
        REQUIRE(t.isOutputDataAt(0));
        REQUIRE(t.outputDataAt(0).isValid());
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(a b)")));

        t.send(DataTypeMList("(1 2 3 ())"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(1 2 3 () a b)")));
    }
}
