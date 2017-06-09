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
#include "../data/set_union.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<SetUnion> SetUnionTest;

#define REQUIRE_SET_OUTPUT(t, set)                          \
    {                                                       \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);                   \
        DataTypeSet* s = t.typedLastDataAt<DataTypeSet>(0); \
        REQUIRE(s != 0);                                    \
        REQUIRE(*s == set);                                 \
    }

#define DSET(l) Data(new DataTypeSet(l))
#define DINT(v) Data(new IntData(v))
#define DSTR(v) Data(new StrData(v))

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("set.intersection", "[externals]")
{
    setup_set0x2eunion();

    SECTION("create")
    {
        SECTION("empty")
        {
            SetUnionTest t("set.union");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            WHEN_SEND_TDATA_TO(0, t, DataTypeSet());
            REQUIRE_SET_OUTPUT(t, DataTypeSet());
        }

        SECTION("args")
        {
            SetUnionTest t("set.union", L3(1, 2, 3));
            WHEN_SEND_TDATA_TO(0, t, DataTypeSet());
            REQUIRE_SET_OUTPUT(t, DataTypeSet(L3(1, 2, 3)));
        }
    }

    SECTION("do")
    {
        SetUnionTest t("set.union");

        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L3(3, 4, 5)));
        REQUIRE_SET_OUTPUT(t, DataTypeSet(L3(3, 4, 5)));

        WHEN_SEND_TDATA_TO(0, t, DataTypeSet());
        REQUIRE_SET_OUTPUT(t, DataTypeSet());

        WHEN_SEND_LIST_TO(1, t, L2(1, 4));

        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L2(3, 5)));
        REQUIRE_SET_OUTPUT(t, DataTypeSet(L4(1, 3, 4, 5)));
    }

    SECTION("do data")
    {
        SetUnionTest t("set.union");
        WHEN_SEND_LIST_TO(1, t, L2(DINT(1), DINT(3)));

        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L2(DINT(2), DINT(4))));
        REQUIRE_SET_OUTPUT(t, DataTypeSet(L4(DINT(1), DINT(2), DINT(3), DINT(4))));

        WHEN_SEND_LIST_TO(1, t, L1(DSET(L2(DINT(1), DINT(3))).toAtom()));
        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L2(DINT(2), DINT(4))));
        REQUIRE_SET_OUTPUT(t, DataTypeSet(L4(DINT(1), DINT(2), DINT(3), DINT(4))));

        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L4(DINT(2), DINT(4), DINT(3), DINT(1))));
        REQUIRE_SET_OUTPUT(t, DataTypeSet(L4(DINT(1), DINT(2), DINT(3), DINT(4))));
    }

    SECTION("do list")
    {
        SetUnionTest t("set.union");

        WHEN_SEND_LIST_TO(1, t, L2(1, 2));
        WHEN_SEND_LIST_TO(0, t, L2(1, 3));

        REQUIRE_SET_OUTPUT(t, DataTypeSet(L3(1, 2, 3)));

        WHEN_SEND_LIST_TO(1, t, AtomList());
        WHEN_SEND_LIST_TO(0, t, AtomList());

        REQUIRE_SET_OUTPUT(t, DataTypeSet());
    }
}
