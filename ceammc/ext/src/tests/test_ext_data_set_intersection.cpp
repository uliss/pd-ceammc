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
#include "../data/set_intersection.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<SetIntersection> SetIntersectionTest;

#define REQUIRE_SET_OUTPUT(t, set)                          \
    {                                                       \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);                   \
        DataTypeSet* s = t.typedLastDataAt<DataTypeSet>(0); \
        REQUIRE(s != 0);                                    \
        REQUIRE(*s == set);                                 \
    }

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("set.intersection", "[externals]")
{
    setup_set0x2eintersection();

    SECTION("create")
    {
        SECTION("empty")
        {
            SetIntersectionTest t("set.intersection");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L2(1, 2)));
            REQUIRE_SET_OUTPUT(t, DataTypeSet());
        }

        SECTION("args")
        {
            SetIntersectionTest t("set.intersection", L3(1, 2, 3));
            WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L3(2, 3, 4)));
            REQUIRE_SET_OUTPUT(t, DataTypeSet(L2(2, 3)));
        }
    }

    SECTION("do")
    {
        SetIntersectionTest t("set.intersecion");

        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L3(3, 4, 5)));
        REQUIRE_SET_OUTPUT(t, DataTypeSet());

        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L3(3, 4, 5)));
        REQUIRE_SET_OUTPUT(t, DataTypeSet());

        WHEN_SEND_LIST_TO(1, t, L4(1, 2, 3, 4));

        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L3(3, 4, 5)));
        REQUIRE_SET_OUTPUT(t, DataTypeSet(L2(3, 4)));
    }

    SECTION("do data")
    {
        SetIntersectionTest t("set.intersecion");
        Data set1(new DataTypeSet(L2(1, 3)));
        WHEN_SEND_LIST_TO(1, t, set1.toAtom());

        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L3(3, 4, 5)));
        REQUIRE_SET_OUTPUT(t, DataTypeSet(Atom(3)));
    }

    SECTION("do list")
    {
        SetIntersectionTest t("set.intersecion");
        WHEN_SEND_LIST_TO(1, t, L3(1, 3, 5));
        WHEN_SEND_LIST_TO(0, t, L6(1, 2, 3, 4, 5, 6));

        REQUIRE_SET_OUTPUT(t, DataTypeSet(L3(1, 3, 5)));
    }
}
