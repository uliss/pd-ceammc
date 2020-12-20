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
#include "conv_int2bits.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(ConvInt2Bits, conv, int2bits)

TEST_CASE("conv.int2bits", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("conv.int2bits");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @size, 32);
            REQUIRE_PROPERTY(t, @rev, 0);
        }

        SECTION("args")
        {
            TObj t("conv.int2bits", LF(16));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @size, 16);
            REQUIRE_PROPERTY(t, @rev, 0);
        }

        SECTION("ext")
        {
            TExt t("conv.int2bits");
        }

        SECTION("alias")
        {
            TExt t("int->bits");
        }
    }

    SECTION("do 8")
    {
        TExt t("int->bits", 8);

        t << 0.;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 0, 0, 0, 0, 0));
        t << 0b1;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 0, 0, 0, 0, 1));
        t << 0b10;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 0, 0, 0, 1, 0));
        t << 0b11;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 0, 0, 0, 1, 1));
        t << 0b100;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 0, 0, 1, 0, 0));
        t << 0xff;
        REQUIRE(t.outputListAt(0) == LF(1, 1, 1, 1, 1, 1, 1, 1));
        t << 0x100;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 0, 0, 0, 0, 0));
        t << -1;
        REQUIRE(t.outputListAt(0) == LF(1, 1, 1, 1, 1, 1, 1, 1));
        t << -2;
        REQUIRE(t.outputListAt(0) == LF(1, 1, 1, 1, 1, 1, 1, 0));
    }

    SECTION("do @rev")
    {
        TExt t("int->bits", LA(8, "@rev", 1));

        t << 0.;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 0, 0, 0, 0, 0));
        t << 0b1;
        REQUIRE(t.outputListAt(0) == LF(1, 0, 0, 0, 0, 0, 0, 0));
        t << 0b10;
        REQUIRE(t.outputListAt(0) == LF(0, 1, 0, 0, 0, 0, 0, 0));
        t << 0b11;
        REQUIRE(t.outputListAt(0) == LF(1, 1, 0, 0, 0, 0, 0, 0));
        t << 0b100;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 1, 0, 0, 0, 0, 0));
        t << 0b1000;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 1, 0, 0, 0, 0));
        t << 0b10000;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 0, 1, 0, 0, 0));
        t << 0b100000;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 0, 0, 1, 0, 0));
        t << 0b1000000;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 0, 0, 0, 1, 0));
        t << 0b10000000;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 0, 0, 0, 0, 1));
        t << 0b100000000;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 0, 0, 0, 0, 0));
        t << 0xff;
        REQUIRE(t.outputListAt(0) == LF(1, 1, 1, 1, 1, 1, 1, 1));
        t << 0x100;
        REQUIRE(t.outputListAt(0) == LF(0, 0, 0, 0, 0, 0, 0, 0));
        t << -1;
        REQUIRE(t.outputListAt(0) == LF(1, 1, 1, 1, 1, 1, 1, 1));
        t << -2;
        REQUIRE(t.outputListAt(0) == LF(0, 1, 1, 1, 1, 1, 1, 1));
    }
}
