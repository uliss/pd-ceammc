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
#include "ceammc_data.h"
#include "datatype_mlist.h"
#include "list_append.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListAppend, list, append)

using TObj = TestListAppend;
using TExt = TestExtListAppend;
using MA = MListAtom;
using IntA = DataAtom<IntData>;

TEST_CASE("list.append", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TObj t("list.append");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("empty")
    {
        TObj t("list.append");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 10);

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
        TObj t("list.append", LA("@prop", 1));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LA("@prop", 1));

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_LIST_AT_OUTLET(0, t, LA(10, "@prop", 1));

        WHEN_SEND_SYMBOL_TO(0, t, "ABC");
        REQUIRE_LIST_AT_OUTLET(0, t, LA("ABC", "@prop", 1));

        WHEN_SEND_DATA_TO(0, t, IntData(12));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(IntA(12), "@prop", 1));

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
        TExt t("list.append");

        t.sendListTo({ IntA(10), IntA(20) }, 1);
        t.send(IntA(30));
        REQUIRE(t.outputListAt(0) == LA(IntA(30), IntA(10), IntA(20)));

        t.sendListTo(LA("a", "b"), 1);
        t.send(MA());
        REQUIRE(t.isOutputDataAt(0));
        REQUIRE(t.outputAtomAt(0).isData());
        REQUIRE(t.outputAtomAt(0) == MA("a", "b"));

        t.send(MA(1, 2, 3, MA()));
        REQUIRE(t.outputAtomAt(0) == MA(1, 2, 3, MA(), "a", "b"));
    }
}
