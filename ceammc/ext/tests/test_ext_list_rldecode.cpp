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
#include "../data/datatype_mlist.h"
#include "../list/list_rldecode.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListRLDecode, list, rldecode);

TEST_CASE("list.rldecode", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestListRLDecode t("list.rldecode");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        TestListRLDecode t("list.rldecode");

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        WHEN_SEND_LIST_TO(1, t, LF(1, 1));
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2));
        WHEN_SEND_LIST_TO(0, t, LA("A", "B"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("A", "B"));

        WHEN_SEND_LIST_TO(1, t, LF(1, 3));
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 2, 2));
        WHEN_SEND_LIST_TO(0, t, LA("A", "B"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("A", "B", "B", "B"));
    }
}
