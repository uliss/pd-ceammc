/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "hoa_connections.h"
#include "test_spat_base.h"

PD_COMPLETE_TEST_SETUP(HoaIn, spat, hoa_connections)

TEST_CASE("hoa.in", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TestHoaIn t("hoa.in");
            REQUIRE(t.numInlets() == 0);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.extra() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @index, 1);
        }

        SECTION("invalid")
        {
            TestHoaIn t("hoa.in", LF(0));
            REQUIRE(t.numInlets() == 0);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.extra() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @index, 1);
        }

        SECTION("explicit")
        {
            TestHoaIn t("hoa.in", LF(1));
            REQUIRE(t.numInlets() == 0);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.extra() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @index, 1);
        }

        SECTION("explicit2")
        {
            TestHoaIn t("hoa.in", LF(2));
            REQUIRE(t.numInlets() == 0);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.extra() == 2);
            REQUIRE_PROPERTY_FLOAT(t, @index, 2);
        }
    }
}
