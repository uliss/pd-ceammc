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
#include "conv_bits2pos.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(ConvBits2Pos, conv, bits2pos)

TEST_CASE("conv.bits2pos", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("conv.bits2pos");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @rev, 0);
        }

        SECTION("ext")
        {
            TExt t("conv.bits2pos");
        }

        SECTION("alias")
        {
            TExt t("bits->pos");
        }
    }

    SECTION("do")
    {
        TExt t("bits->pos");

        t << 0.;
        REQUIRE(!t.hasOutputAt(0));
        t << 1;
        REQUIRE(t.outputFloatAt(0) == 0);
        t << LF(0, 0, 0, 0, 0, 0, 1, 0);
        REQUIRE(t.outputListAt(0) == LF(1));
        t << LF(1, 0);
        REQUIRE(t.outputListAt(0) == LF(1));
        t << LF(0, 0, 0, 0, 0, 1, 0, 1);
        REQUIRE(t.outputListAt(0) == LF(0, 2));
        t << LF(0, 0, 0, 0, 1, 0, 1, 1);
        REQUIRE(t.outputListAt(0) == LF(0, 1, 3));
        t << LF(0, 0, 0, 1, 0, 1, 1, 0);
        REQUIRE(t.outputListAt(0) == LF(1, 2, 4));
        t << LF(1, 0, 1, 1, 0, 0, 0, 0, 0, 0);
        REQUIRE(t.outputListAt(0) == LF(6, 7, 9));
        t << LF(1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1);
        REQUIRE(t.outputListAt(0) == LF(0, 7, 8, 10));
    }

    SECTION("do @rev 1")
    {
        TExt t("bits->pos", LA("@rev", 1));

        t << 0.;
        REQUIRE(!t.hasOutputAt(0));
        t << 1;
        REQUIRE(t.outputFloatAt(0) == 0);
        t << LF(1, 0);
        REQUIRE(t.outputListAt(0) == LF(0));
        t << LF(1, 0, 1);
        REQUIRE(t.outputListAt(0) == LF(0, 2));
        t << LF(1, 0, 1, 1);
        REQUIRE(t.outputListAt(0) == LF(0, 2, 3));
        t << LF(1, 0, 1, 1, 0);
        REQUIRE(t.outputListAt(0) == LF(0, 2, 3));
        t << LF(1, 0, 1, 1, 0, 0, 0, 0, 0, 0);
        REQUIRE(t.outputListAt(0) == LF(0, 2, 3));
        t << LF(1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1);
        REQUIRE(t.outputListAt(0) == LF(0, 2, 3, 10));
    }
}
