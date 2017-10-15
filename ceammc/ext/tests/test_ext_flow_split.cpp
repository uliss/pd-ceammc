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
#include "../flow/flow_split.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<FlowSplit> FlowSplitTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("flow.split", "[externals]")
{
    SECTION("init")
    {
        FlowSplitTest t("flow.split");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 3);
    }

    SECTION("bang")
    {
        FlowSplitTest t("flow.split");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_BANG_AT_OUTLET(2, t);
        REQUIRE_BANG_AT_OUTLET(1, t);
    }

    SECTION("float")
    {
        FlowSplitTest t("flow.split");
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(2, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 2);
    }

    SECTION("symbol")
    {
        FlowSplitTest t("flow.split");
        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_SYMBOL_AT_OUTLET(2, t, "a");
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "a");
    }

    SECTION("list")
    {
        FlowSplitTest t("flow.split");
        WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(2, t, L3(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(1, t, L3(1, 2, 3));
    }

    SECTION("any")
    {
        FlowSplitTest t("flow.split");
        WHEN_SEND_ANY_TO(t, "go", L2(1, 2));
        REQUIRE_ANY_AT_OUTLET(2, t, L3("go", 1, 2));
        REQUIRE_ANY_AT_OUTLET(1, t, L3("go", 1, 2));
    }
}
