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
#include "../flow/flow_change.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<FlowChange> FlowChangeTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("flow.change", "[externals]")
{
    SECTION("init")
    {
        FlowChangeTest t("flow.change");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("process")
    {
        FlowChangeTest t("flow.change");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 2.1f);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2.1);

        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "a");

        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_SYMBOL_TO(0, t, "b");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "b");

        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 2));

        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, L2(1, 3));
        REQUIRE_LIST_AT_OUTLET(0, t, L2(1, 3));

        WHEN_SEND_ANY_TO(t, "any", L2(3, 4));
        REQUIRE_ANY_AT_OUTLET(0, t, L3("any", 3, 4));
    }

    SECTION("reset")
    {
        FlowChangeTest t("flow.change");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_CALL(t, reset);
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);
    }

    SECTION("set")
    {
        FlowChangeTest t("flow.change");

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_CALL_1(t, set, 1);
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);

        WHEN_CALL_3(t, set, 1, 2, 3);
        WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, L4(1, 2, 3, 4));
        REQUIRE_LIST_AT_OUTLET(0, t, L4(1, 2, 3, 4));
    }
}
