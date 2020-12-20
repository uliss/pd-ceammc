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
#include "set_symdiff.h"
#include "test_data_base.h"

#define REQUIRE_SET_OUTPUT(t, set)                   \
    {                                                \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);            \
        REQUIRE(t.lastMessage().atomValue() == set); \
    }

PD_COMPLETE_TEST_SETUP(SetSymmetricDifference, set, symdiff)

using DSet = DataTypeSet;

TEST_CASE("set.symdiff", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TObj t("set.symdiff");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            // {1,2} \ {} = {1,2}
            WHEN_SEND_TDATA_TO(0, t, DSet(1, 2));
            REQUIRE_SET_OUTPUT(t, SetA(1, 2));
        }

        SECTION("args")
        {
            // {2,4} \ {1,2,3} = {1,3,4}
            TObj t("set.symdiff", LF(1, 2, 3));
            WHEN_SEND_TDATA_TO(0, t, DSet(2, 4));
            REQUIRE_SET_OUTPUT(t, SetA(1, 3, 4));

            // {1,2,3} \ {1,2,3} = {}
            WHEN_SEND_TDATA_TO(0, t, DSet(1, 2, 3));
            REQUIRE_SET_OUTPUT(t, SetA());

            // {} \ {1,2,3} = {1,2,3}
            WHEN_SEND_TDATA_TO(0, t, DSet());
            REQUIRE_SET_OUTPUT(t, SetA(1, 2, 3));
        }
    }

    SECTION("do")
    {
        TObj t("set.symdiff");

        // {3,4,5} \ {} = {3,4,5}
        WHEN_SEND_TDATA_TO(0, t, DSet(3, 4, 5));
        REQUIRE_SET_OUTPUT(t, SetA(3, 4, 5));

        // {} \ {} = {}
        WHEN_SEND_TDATA_TO(0, t, DataTypeSet());
        REQUIRE_SET_OUTPUT(t, SetA());

        // {3,5} \ {1,4} = {1,3,4,5}
        WHEN_SEND_LIST_TO(1, t, LF(1, 4));
        WHEN_SEND_TDATA_TO(0, t, DSet(3, 5));
        REQUIRE_SET_OUTPUT(t, SetA(3, 5, 1, 4));
    }

    SECTION("do data")
    {
        TObj t("set.symdiff");

        // {2,4} \ {1,3} = {2,4,1,3}
        WHEN_SEND_LIST_TO(1, t, LA(IntA(1), IntA(3)));
        WHEN_SEND_TDATA_TO(0, t, DSet(IntA(2), IntA(4)));
        REQUIRE_SET_OUTPUT(t, SetA(IntA(2), IntA(4), IntA(1), IntA(3)));

        // {2,4} \ {1,3} = {2,4}
        WHEN_SEND_LIST_TO(1, t, LA(SetA(IntA(1), IntA(3))));
        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(LA(IntA(2), IntA(4))));
        REQUIRE_SET_OUTPUT(t, SetA(IntA(2), IntA(4), IntA(1), IntA(3)));

        // {2,3,4,5} \ {1,3} = {1,2,4,5}
        WHEN_SEND_TDATA_TO(0, t, DSet(IntA(2), IntA(4), IntA(3), IntA(5)));
        REQUIRE_SET_OUTPUT(t, SetA(IntA(2), IntA(5), IntA(4), IntA(1)));
    }

    SECTION("do list")
    {
        TObj t("set.symdiff");

        // {1,3} \ {1,2} = {2,3}
        WHEN_SEND_LIST_TO(1, t, LF(1, 2));
        WHEN_SEND_LIST_TO(0, t, LF(1, 3));
        REQUIRE_SET_OUTPUT(t, SetA(2, 3));

        // {} \ {} = {}
        WHEN_SEND_LIST_TO(1, t, L());
        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_SET_OUTPUT(t, SetA());
    }
}
