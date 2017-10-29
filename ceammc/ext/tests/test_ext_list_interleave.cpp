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
#include "../list/list_interleave.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListInterleave> ListInterleaveTest;

TEST_CASE("list.interleave", "[externals]")
{
    SECTION("init")
    {
        SECTION("default") {
            ListInterleaveTest t("list.interleave");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("args") {
            ListInterleaveTest t("list.interleave", L1(4));
            REQUIRE(t.numInlets() == 4);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("args") {
            ListInterleaveTest t("list.interleave", L1(20));
            REQUIRE(t.numInlets() == 20);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("invalid") {
            SECTION("1") {
                ListInterleaveTest t("list.interleave", L1(1));
                REQUIRE(t.numInlets() == 2);
                REQUIRE(t.numOutlets() == 1);
            }

            SECTION("-10") {
                ListInterleaveTest t("list.interleave", L1(-10));
                REQUIRE(t.numInlets() == 2);
                REQUIRE(t.numOutlets() == 1);
            }

            SECTION("21") {
                ListInterleaveTest t("list.interleave", L1(20));
                REQUIRE(t.numInlets() == 20);
                REQUIRE(t.numOutlets() == 1);
            }
        }
    }

    SECTION("act") {
        SECTION("default") {
            ListInterleaveTest t("list.interleave");

            // default @min method
            WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());
            REQUIRE_PROPERTY_LIST(t, @l0, L3(1, 2, 3));

            // interleave with 4,5,6
            WHEN_SEND_LIST_TO(1, t, L3(4, 5, 6));
            REQUIRE_NO_MSG(t);
            REQUIRE_PROPERTY_LIST(t, @l1, L3(4, 5, 6));

            WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, L6(1, 4, 2, 5, 3, 6));

            WHEN_SEND_LIST_TO(0, t, L2(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, L4(1, 4, 2, 5));

            WHEN_SEND_LIST_TO(0, t, L1(1));
            REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 4));

            WHEN_SEND_LIST_TO(0, t, AtomList());
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

            // interleave with 4,5
            WHEN_SEND_LIST_TO(1, t, L2(4, 5));
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, L4(1, 4, 2, 5));

            WHEN_SEND_LIST_TO(0, t, L2(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, L4(1, 4, 2, 5));

            WHEN_SEND_LIST_TO(0, t, L1(1));
            REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 4));

            WHEN_SEND_LIST_TO(0, t, AtomList());
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

            // interleave with 4
            WHEN_SEND_LIST_TO(1, t, L1(4));
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 4));

            WHEN_SEND_LIST_TO(0, t, L2(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 4));

            WHEN_SEND_LIST_TO(0, t, L1(1));
            REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 4));

            WHEN_SEND_LIST_TO(0, t, AtomList());
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

            // interleave with ()
            WHEN_SEND_LIST_TO(1, t, AtomList());
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

            WHEN_SEND_LIST_TO(0, t, L2(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

            WHEN_SEND_LIST_TO(0, t, L1(1));
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

            WHEN_SEND_LIST_TO(0, t, AtomList());
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());
        }

        SECTION("multiple") {
            ListInterleaveTest t("list.interleave", L1(3));

            // default @min method
            WHEN_SEND_LIST_TO(0, t, L2(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());
            REQUIRE_PROPERTY_LIST(t, @l0, L2(1, 2));

            // interleave with 3,4 and 5,6
            WHEN_SEND_LIST_TO(1, t, L2(3, 4));
            REQUIRE_PROPERTY_LIST(t, @l1, L2(3, 4));
            REQUIRE_NO_MSG(t);
            WHEN_SEND_LIST_TO(2, t, L2(5, 6));
            REQUIRE_PROPERTY_LIST(t, @l2, L2(5, 6));
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, L2(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, L6(1, 3, 5, 2, 4, 6));
        }
    }
}
