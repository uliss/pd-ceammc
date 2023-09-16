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
#include "conv_bpm2ms.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(BpmToMs, conv, bpm2ms)

using namespace ceammc;

TEST_CASE("conv.bpm2ms", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("conv.bpm2ms");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("alias")
        {
            TExt t("bpm->ms");
        }
    }

    SECTION("onFloat")
    {
        SECTION("default")
        {
            TExt t("conv.bpm2ms");
            t << 60;
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1000);
            t << 120;
            REQUIRE_FLOAT_AT_OUTLET(0, t, 500);
            t << 240;
            REQUIRE_FLOAT_AT_OUTLET(0, t, 250);
            t << 0.5;
            REQUIRE_FLOAT_AT_OUTLET(0, t, 120000);
            t << 0.0;
            REQUIRE_FLOAT_AT_OUTLET(0, t, std::numeric_limits<t_float>::max());
        }
    }
}
