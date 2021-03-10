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
#include "conv_bits2int.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(ConvBits2Int, conv, bits2int)

TEST_CASE("conv.bits2int", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("conv.bits2int");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @rev, 0);
        }

        SECTION("ext")
        {
            TExt t("conv.bits2int");
        }

        SECTION("alias")
        {
            TExt t("bits->int");
        }
    }

    SECTION("do")
    {
        TExt t("bits->int", 8);

        t << 0.;
        REQUIRE(t.outputFloatAt(0) == 0);
        t << 1;
        REQUIRE(t.outputFloatAt(0) == 1);
        t << LF(1, 1, 1);
        REQUIRE(t.outputFloatAt(0) == 7);
        t << LF(0, 0, 0, 0, 1, 0, 1);
        REQUIRE(t.outputFloatAt(0) == 5);
        t << LF(0, 0, 0, 0, 0, 0, 0);
        REQUIRE(t.outputFloatAt(0) == 0);

        t << LF(0, 0, 0, 0, -1, 100, 0);
        REQUIRE(t.outputFloatAt(0) == 0);
    }

    SECTION("do @rev")
    {
        TExt t("bits->int", LA(8, "@rev", 1));

        t << 0.;
        REQUIRE(t.outputFloatAt(0) == 0);
        t << 1;
        REQUIRE(t.outputFloatAt(0) == 1);
        t << LF(1, 1, 1);
        REQUIRE(t.outputFloatAt(0) == 7);
        t << LF(1, 0, 1, 0, 0, 0, 0, 0);
        REQUIRE(t.outputFloatAt(0) == 5);
        t << LA("A", 0., "B", 0., 0., 0., 0., 0.);
        REQUIRE(t.outputFloatAt(0) == 0);
    }
}
