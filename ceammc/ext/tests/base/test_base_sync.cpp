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
#include "base_sync.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(BaseSync, base, sync)

TEST_CASE("sync", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        SECTION("default")
        {
            TExt t("sync");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY_FLOAT(t, @n, 2);
        }

        SECTION("args")
        {
            TExt t("sync", LF(4));
            REQUIRE(t.numInlets() == 4);
            REQUIRE(t.numOutlets() == 4);
            REQUIRE_PROPERTY_FLOAT(t, @n, 4);
        }

        SECTION("props")
        {
            TExt t("sync", LA("@n", 5));
            REQUIRE(t.numInlets() == 5);
            REQUIRE(t.numOutlets() == 5);
            REQUIRE_PROPERTY_FLOAT(t, @n, 5);
        }
    }

    SECTION("process")
    {
        TExt t("sync", LA("@n", 3));

        t.sendFloatTo(100, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 100);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_FLOAT_AT_OUTLET(2, t, 100);

        t.sendFloatTo(-20, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, -20);
        REQUIRE_FLOAT_AT_OUTLET(1, t, -20);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);

        t.call("_:01", 10);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 10);
        REQUIRE_FLOAT_AT_OUTLET(2, t, 10);
    }
}
