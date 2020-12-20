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
#include "flt_median.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(FltMedian, flt, median)

TEST_CASE("flt.median", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TestFltMedian t("flt.median");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @size, 9);
        }

        SECTION("@size")
        {
            TestFltMedian t("flt.median", LA("@size", 4));
            REQUIRE_PROPERTY_FLOAT(t, @size, 4);
        }

        SECTION("min")
        {
            TestFltMedian t("flt.median", LA("@size", 0.f));
            REQUIRE_PROPERTY_FLOAT(t, @size, 9);
        }

        SECTION("min2")
        {
            TestFltMedian t("flt.median", LA("@size", -1));
            REQUIRE_PROPERTY_FLOAT(t, @size, 9);
        }

        SECTION("max")
        {
            TestFltMedian t("flt.median", LA("@size", 128));
            REQUIRE_PROPERTY_FLOAT(t, @size, 128);
        }

        SECTION("max2")
        {
            TestFltMedian t("flt.median", LA("@size", 129));
            REQUIRE_PROPERTY_FLOAT(t, @size, 9);
        }
    }

    SECTION("odd")
    {
        TestFltMedian t("flt.median", LA("@size", 3));

        // [1]
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        // [1,2]
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(1.5));

        // [1,2,2]
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(2));

        // [2,2,10]
        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(2));

        // [2,10,2]
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(2));

        // [10,2,2]
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(2));

        // [2,2,2]
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(2));

        // [2,2,1]
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(2));

        // [2,1,1]
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(1));

        // SIZE 1
        t.setProperty("@size", LF(1));

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(1));
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(2));
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(3));

        // SIZE 2
        t.setProperty("@size", LF(2));

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(1));
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(1.5));
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(2.5));

        // SIZE 4
        t.setProperty("@size", LF(4));

        // [1]
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(1));
        // [1,2]
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(1.5));
        // [1,2,3]
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(2));
        // [1,2,3,4]
        WHEN_SEND_FLOAT_TO(0, t, 4);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(2.5));
        // [2,3,4,5]
        WHEN_SEND_FLOAT_TO(0, t, 5);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(3.5));
    }
}
