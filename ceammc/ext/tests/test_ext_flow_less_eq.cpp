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
#include "../flow/flow_less_eq.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<FlowLessEq> FlowLessEqTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("flow.less_eq", "[externals]")
{
    SECTION("init")
    {
        setup_flow_less_eq();

        SECTION("default")
        {
            FlowLessEqTest t("flow.less_eq");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            WHEN_SEND_FLOAT_TO(0, t, 123);
            REQUIRE_NO_MSG(t);
        }

        SECTION("single")
        {
            FlowLessEqTest t("flow.less_eq", L1(1));
            REQUIRE(t.numOutlets() == 2);

            WHEN_SEND_FLOAT_TO(0, t, -100);
            REQUIRE_FLOAT_AT_OUTLET(0, t, -100);

            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_FLOAT_TO(0, t, 1.0001);
            REQUIRE_FLOAT_AT_OUTLET(1, t, 1.0001);

            WHEN_SEND_FLOAT_TO(0, t, 100);
            REQUIRE_FLOAT_AT_OUTLET(1, t, 100);
        }

        SECTION("multiple")
        {
            FlowLessEqTest t("flow.less_eq", L3(-10, 1, 10)); // [. -10 . 1 . 10 .]
            REQUIRE(t.numOutlets() == 4);

            // 1st outlet
            WHEN_SEND_FLOAT_TO(0, t, -100);
            REQUIRE_FLOAT_AT_OUTLET(0, t, -100);

            WHEN_SEND_FLOAT_TO(0, t, -10);
            REQUIRE_FLOAT_AT_OUTLET(0, t, -10);

            // 2nd
            WHEN_SEND_FLOAT_TO(0, t, -9);
            REQUIRE_FLOAT_AT_OUTLET(1, t, -9);

            WHEN_SEND_FLOAT_TO(0, t, 0.f);
            REQUIRE_FLOAT_AT_OUTLET(1, t, 0.f);

            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_FLOAT_AT_OUTLET(1, t, 1);

            // 3rd
            WHEN_SEND_FLOAT_TO(0, t, 1.1);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 1.1);

            WHEN_SEND_FLOAT_TO(0, t, 2);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 2);

            WHEN_SEND_FLOAT_TO(0, t, 9);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 9);

            WHEN_SEND_FLOAT_TO(0, t, 10);
            REQUIRE_FLOAT_AT_OUTLET(2, t, 10);

            // 4th
            WHEN_SEND_FLOAT_TO(0, t, 10.1);
            REQUIRE_FLOAT_AT_OUTLET(3, t, 10.1);
        }
    }
}
