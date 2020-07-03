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
#include "set_difference.h"
#include "test_data_base.h"

#define REQUIRE_SET_OUTPUT(t, set)                   \
    {                                                \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);            \
        REQUIRE(t.lastMessage().atomValue() == set); \
    }

PD_COMPLETE_TEST_SETUP(SetDifference, set, difference)

TEST_CASE("set.diff", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TObj t("set.diff");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            // {1,2} \ {} = {1,2}
            WHEN_SEND_TDATA_TO(0, t, DataTypeSet(1, 2));
            REQUIRE_SET_OUTPUT(t, SetA(1, 2));
        }

        SECTION("args")
        {
            // {2,4} \ {1,2,3} = {4}
            TObj t("set.diff", LF(1, 2, 3));
            WHEN_SEND_TDATA_TO(0, t, DataTypeSet(2, 4));
            REQUIRE_SET_OUTPUT(t, SetA(4));

            // {1,2,3} \ {1,2,3} = {}
            WHEN_SEND_TDATA_TO(0, t, DataTypeSet(1, 2, 3));
            REQUIRE_SET_OUTPUT(t, SetA());

            // {} \ {1,2,3} = {}
            WHEN_SEND_TDATA_TO(0, t, DataTypeSet());
            REQUIRE_SET_OUTPUT(t, SetA());
        }

        SECTION("args data")
        {
            TObj t("set.diff", AtomList::parseString("\"a b c\" pi()"));

            // {"a b c"} \ {"a b c" 3.1415} = {}
            WHEN_SEND_TDATA_TO(0, t, DataTypeSet("a b c"));
            REQUIRE_SET_OUTPUT(t, SetA());
        }
    }

    SECTION("do")
    {
        TObj t("set.diff");

        // {3,4,5} \ {} = {3,4,5}
        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(3, 4, 5));
        REQUIRE_SET_OUTPUT(t, SetA(3, 4, 5));

        // {} \ {} = {}
        WHEN_SEND_TDATA_TO(0, t, DataTypeSet());
        REQUIRE_SET_OUTPUT(t, SetA());

        // {3,5} \ {1,4} = {}
        WHEN_SEND_LIST_TO(1, t, LF(1, 4));
        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(3, 5));
        REQUIRE_SET_OUTPUT(t, SetA(3, 5));
    }

    SECTION("do data")
    {
        TObj t("set.diff");

        // {2,4} \ {1,3} = {2,4}
        WHEN_SEND_LIST_TO(1, t, LA(IntA(1), IntA(3)));
        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(IntA(2), IntA(4)));
        REQUIRE_SET_OUTPUT(t, SetA(IntA(2), IntA(4)));

        // {2,4} \ {1,3} = {2,4}
        WHEN_SEND_LIST_TO(1, t, LA(SetA(IntA(1), IntA(3))));
        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(IntA(2), IntA(4)));
        REQUIRE_SET_OUTPUT(t, SetA(IntA(2), IntA(4)));

        // {2,3,4,5} \ {1,3} = {2,4,5}
        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(IntA(2), IntA(4), IntA(3), IntA(5)));
        REQUIRE_SET_OUTPUT(t, SetA(IntA(2), IntA(5), IntA(4)));
    }

    SECTION("do list")
    {
        TObj t("set.diff");

        // {1,3} \ {1,2} = {3}
        WHEN_SEND_LIST_TO(1, t, LF(1, 2));
        WHEN_SEND_LIST_TO(0, t, LF(1, 3));
        REQUIRE_SET_OUTPUT(t, SetA(3));

        // {} \ {} = {}
        WHEN_SEND_LIST_TO(1, t, L());
        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_SET_OUTPUT(t, SetA());
    }
}
