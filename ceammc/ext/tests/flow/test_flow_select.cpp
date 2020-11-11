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
#include "flow_select.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowSelect, flow, select)

TEST_CASE("flow.select", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("flow.select");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @keep_value, 0.);
        }

        SECTION("arg")
        {
            TExt t("flow.select", LF(2));
            REQUIRE(t.numOutlets() == 2);
        }

        SECTION("arg2")
        {
            TExt t("flow.select", LA("2|3|4", "3..4", "0~0.125", ">125"));
            REQUIRE(t.numOutlets() == 5);
        }

        SECTION("alias")
        {
            TExt t("flow.sel", LF(2));
            REQUIRE(t.numOutlets() == 2);
        }
    }

    SECTION("process")
    {
        SECTION("noargs")
        {
            TExt t("flow.sel");

            t << 123;
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 123);

            t << "ABC";
            REQUIRE(t.isOutputSymbolAt(0));
            REQUIRE(t.outputSymbolAt(0) == SYM("ABC"));
        }

        SECTION("simple args")
        {
            TExt t("flow.sel", LA("A", "ABC", 100, "ab100", "10ms.1"));

            t << "A";
            REQUIRE(t.isOutputBangAt(0));
            t << "ABC";
            REQUIRE(t.isOutputBangAt(1));
            t << 100;
            REQUIRE(t.isOutputBangAt(2));
            t << "ab100";
            REQUIRE(t.isOutputBangAt(3));
            t << "10ms.1";
            REQUIRE(t.isOutputBangAt(4));
            t << "????";
            REQUIRE(t.isOutputSymbolAt(5));
            REQUIRE(t.outputSymbolAt(5) == SYM("????"));
            t << 100.0001;
            REQUIRE(t.isOutputFloatAt(5));
            REQUIRE(t.outputFloatAt(5) == Approx(100.0001));
        }

        SECTION(".. range")
        {
            TExt t("flow.sel", LA("1..2", "3..4"));

            REQUIRE(t.numOutlets() == 3);

            t << 1;
            REQUIRE(t.isOutputBangAt(0));
            t << 1.5;
            REQUIRE(t.isOutputBangAt(0));
            t << 2;
            REQUIRE(t.isOutputBangAt(0));

            t << 2.125;
            REQUIRE(t.outputFloatAt(2) == 2.125);
            t << 2.75;
            REQUIRE(t.outputFloatAt(2) == 2.75);

            t << 3;
            REQUIRE(t.isOutputBangAt(1));
            t << 3.5;
            REQUIRE(t.isOutputBangAt(1));
            t << 4;
            REQUIRE(t.isOutputBangAt(1));

            t << 4.125;
            REQUIRE(t.outputFloatAt(2) == 4.125);
        }

        SECTION("[..] range")
        {
            TExt t("flow.sel", LA("[1..2]", "[3..4]"));

            REQUIRE(t.numOutlets() == 3);

            t << 1;
            REQUIRE(t.isOutputBangAt(0));
            t << 1.5;
            REQUIRE(t.isOutputBangAt(0));
            t << 2;
            REQUIRE(t.isOutputBangAt(0));

            t << 2.125;
            REQUIRE(t.outputFloatAt(2) == 2.125);
            t << 2.75;
            REQUIRE(t.outputFloatAt(2) == 2.75);

            t << 3;
            REQUIRE(t.isOutputBangAt(1));
            t << 3.5;
            REQUIRE(t.isOutputBangAt(1));
            t << 4;
            REQUIRE(t.isOutputBangAt(1));

            t << 4.125;
            REQUIRE(t.outputFloatAt(2) == 4.125);
        }

        SECTION("[..) range")
        {
            TExt t("flow.sel", LA("[1..2)", "(3..4]", "(-1..0)"));

            REQUIRE(t.numOutlets() == 4);

            t << 1;
            REQUIRE(t.isOutputBangAt(0));
            t << 1.5;
            REQUIRE(t.isOutputBangAt(0));
            t << 2;
            REQUIRE_FALSE(t.hasOutputAt(0));
            REQUIRE(t.outputFloatAt(3) == 2);

            t << 3;
            REQUIRE_FALSE(t.hasOutputAt(1));
            REQUIRE(t.outputFloatAt(3) == 3);
            t << 3.5;
            REQUIRE(t.isOutputBangAt(1));
            t << 4;
            REQUIRE(t.isOutputBangAt(1));

            t << -1;
            REQUIRE_FALSE(t.hasOutputAt(2));
            REQUIRE(t.outputFloatAt(3) == -1);

            t << -0.999;
            REQUIRE(t.isOutputBangAt(2));
            t << -0.00001;
            REQUIRE(t.isOutputBangAt(2));
            t << 0.;
            REQUIRE_FALSE(t.hasOutputAt(2));
            REQUIRE(t.outputFloatAt(3) == 0);
        }

        SECTION("cmp")
        {
            TExt t("flow.sel", LA(">1", ">=2", "<10", "<=18"));

            t << 0.;
            REQUIRE(t.isOutputBangAt(2));
            REQUIRE(t.isOutputBangAt(3));

            t << 1;
            REQUIRE(t.isOutputBangAt(2));
            REQUIRE(t.isOutputBangAt(3));

            t << 1.0001;
            REQUIRE(t.isOutputBangAt(0));
            REQUIRE(t.isOutputBangAt(2));
            REQUIRE(t.isOutputBangAt(3));

            t << 2;
            REQUIRE(t.isOutputBangAt(0));
            REQUIRE(t.isOutputBangAt(1));
            REQUIRE(t.isOutputBangAt(2));
            REQUIRE(t.isOutputBangAt(3));

            t << 4;
            REQUIRE(t.isOutputBangAt(0));
            REQUIRE(t.isOutputBangAt(1));
            REQUIRE(t.isOutputBangAt(2));
            REQUIRE(t.isOutputBangAt(3));

            t << 10;
            REQUIRE(t.isOutputBangAt(0));
            REQUIRE(t.isOutputBangAt(1));
            REQUIRE(!t.hasOutputAt(2));
            REQUIRE(t.isOutputBangAt(3));

            t << 18;
            REQUIRE(t.isOutputBangAt(0));
            REQUIRE(t.isOutputBangAt(1));
            REQUIRE(!t.hasOutputAt(2));
            REQUIRE(t.isOutputBangAt(3));

            t << 18.0001;
            REQUIRE(t.isOutputBangAt(0));
            REQUIRE(t.isOutputBangAt(1));
            REQUIRE(!t.hasOutputAt(2));
            REQUIRE(!t.hasOutputAt(3));
        }
    }
}
