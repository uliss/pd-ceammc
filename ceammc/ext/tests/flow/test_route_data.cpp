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
#include "route_data.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(RouteData, route, data)

TEST_CASE("route.data", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("route.data");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @type, &s_);
        }

        SECTION("alias")
        {
            TExt t("route.d");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @type, &s_);
        }

        SECTION("@type")
        {
            TExt t("route.d", LA("@type", "Color"));
            REQUIRE_PROPERTY(t, @type, S("Color"));
        }

        SECTION("args")
        {
            TExt t("route.d", LA("Color2"));
            REQUIRE_PROPERTY(t, @type, S("Color2"));
        }
    }

    SECTION("default")
    {
        TExt t("route.data");
        t.sendBang();
        REQUIRE_BANG_AT_OUTLET(1, t);

        t << 100.25;
        REQUIRE_FLOAT_AT_OUTLET(1, t, 100.25);

        t << "ABC";
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "ABC");

        t << LF(1, 2, 3);
        REQUIRE_LIST_AT_OUTLET(1, t, LF(1, 2, 3));

        t <<= LA("any", "message");
        REQUIRE_ANY_AT_OUTLET(1, t, LA("any", "message"));

        t << IntA(120);
        REQUIRE_DATA_AT_OUTLET(0, t, IntA(120));

        t << DataAtom<StrData>("test");
        REQUIRE_DATA_AT_OUTLET(0, t, DataAtom<StrData>("test"));
    }

    SECTION("typed")
    {
        IntData::staticType();
        StrData::staticType();

        SECTION("IntData")
        {
            TExt t("route.data", "IntData");

            t << IntA(120);
            REQUIRE_DATA_AT_OUTLET(0, t, IntA(120));

            t << DataAtom<StrData>("test");
            REQUIRE_DATA_AT_OUTLET(1, t, DataAtom<StrData>("test"));
        }

        SECTION("StrData")
        {
            TExt t("route.data", "StrData");

            t << IntA(120);
            REQUIRE_DATA_AT_OUTLET(1, t, IntA(120));

            t << DataAtom<StrData>("test");
            REQUIRE_DATA_AT_OUTLET(0, t, DataAtom<StrData>("test"));
        }
    }
}
