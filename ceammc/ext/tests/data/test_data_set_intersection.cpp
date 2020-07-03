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
#include "set_intersection.h"
#include "test_data_base.h"

#define REQUIRE_SET_OUTPUT(t, set)                   \
    {                                                \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);            \
        REQUIRE(t.lastMessage().atomValue() == set); \
    }

PD_COMPLETE_TEST_SETUP(SetIntersection, set, intersection)

using DSet = DataTypeSet;

TEST_CASE("set.intersection", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TObj t("set.intersection");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            WHEN_SEND_TDATA_TO(0, t, DSet(1, 2));
            REQUIRE_SET_OUTPUT(t, SetA());
        }

        SECTION("args")
        {
            TObj t("set.intersection", LF(1, 2, 3));
            WHEN_SEND_TDATA_TO(0, t, DSet(2, 3, 4));
            REQUIRE_SET_OUTPUT(t, SetA(2, 3));
        }
    }

    SECTION("do")
    {
        TObj t("set.intersecion");

        WHEN_SEND_TDATA_TO(0, t, DSet(3, 4, 5));
        REQUIRE_SET_OUTPUT(t, SetA());

        WHEN_SEND_TDATA_TO(0, t, DSet(3, 4, 5));
        REQUIRE_SET_OUTPUT(t, SetA());

        WHEN_SEND_LIST_TO(1, t, LF(1, 2, 3, 4));

        WHEN_SEND_TDATA_TO(0, t, DSet(3, 4, 5));
        REQUIRE_SET_OUTPUT(t, SetA(3, 4));
    }

    SECTION("do data")
    {
        TObj t("set.intersecion");

        WHEN_SEND_LIST_TO(1, t, SetA(1, 3));
        WHEN_SEND_TDATA_TO(0, t, DSet(3, 4, 5));
        REQUIRE_SET_OUTPUT(t, SetA(3));
    }

    SECTION("do list")
    {
        TObj t("set.intersecion");
        WHEN_SEND_LIST_TO(1, t, LF(1, 3, 5));
        WHEN_SEND_LIST_TO(0, t, LA(1, 2, 3, 4, 5, 6));

        REQUIRE_SET_OUTPUT(t, SetA(1, 3, 5));
    }
}
