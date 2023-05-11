/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "data_bimap.h"
#include "test_base.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(DataBiMap, data, bimap)

TEST_CASE("data.bimap", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("construct")
    {
        TExt t("data.bimap");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("bimap")
    {
        TExt t("data.bimap", LA("A", 1, "B", 2, "C", 3));
        REQUIRE_PROPERTY_LIST(t, @v, LA("A", 1, "B", 2, "C", 3));

        t << "A";
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        t << "B";
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);
        t << "C";
        REQUIRE_FLOAT_AT_OUTLET(0, t, 3);
        t << "D";
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t << 1;
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t << LA("A", "C", "B");
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 3, 2));

        t.sendFloatTo(1, 1);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "A");
        t.sendFloatTo(2, 1);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "B");
        t.sendFloatTo(3, 1);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "C");
        t.sendFloatTo(3.001, 1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        t.sendListTo(LF(3, 2, 1), 1);
        REQUIRE_LIST_AT_OUTLET(1, t, LA("C", "B", "A"));
    }
}
