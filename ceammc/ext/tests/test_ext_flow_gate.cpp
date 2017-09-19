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
#include "../flow/flow_gate.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<FlowGate> FlowGateTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("flow.gate", "[externals]")
{
    SECTION("init")
    {
        SECTION("default")
        {
            FlowGateTest t("flow.gate");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @state, 0.f);
        }

        SECTION("closed")
        {
            FlowGateTest t("flow.gate", L1(0.f));
            REQUIRE_PROPERTY(t, @state, 0.f);
        }

        SECTION("open")
        {
            FlowGateTest t("flow.gate", L1(1));
            REQUIRE_PROPERTY(t, @state, 1);
        }
    }

    SECTION("process")
    {
        FlowGateTest t("flow.gate");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_FLOAT_TO(0, t, 100);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_NO_MSG(t);
        WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
        REQUIRE_NO_MSG(t);
        WHEN_SEND_ANY_TO(t, "test", L2(1, 2));
        REQUIRE_NO_MSG(t);
        WHEN_SEND_ANY_TO(t, "@test", L2(1, 2));
        REQUIRE_NO_MSG(t);
        DataPtr dp(new IntData(123));
        WHEN_SEND_DATA_TO(0, t, dp);
        REQUIRE_NO_MSG(t);

        t.setProperty("@state", L1(1));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_BANG_AT_OUTLET(0, t);
        WHEN_SEND_FLOAT_TO(0, t, 100);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 100);
        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");
        WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(0, t, L3(1, 2, 3));
        WHEN_SEND_ANY_TO(t, "test", L2(1, 2));
        REQUIRE_ANY_AT_OUTLET(0, t, L3("test", 1, 2));
        DataPtr dp2(new IntData(123));
        WHEN_SEND_DATA_TO(0, t, dp2);
        REQUIRE_DATA_AT_OUTLET(0, t, dp2.asAtom());
    }
}
