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
#include "flow_demultiplex.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowDemultiplex, flow, demultiplex)

TEST_CASE("flow.demultiplex", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("flow.demultiplex");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE_PROPERTY(t, @index, 0.f);

        // invalid
        SECTION("number")
        {
            TObj t("flow.demultiplex", LF(0.f));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @index, 0.f);
        }

        // invalid
        SECTION("number")
        {
            TObj t("flow.demultiplex", LF(-1));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @index, 0.f);
        }

        // invalid
        SECTION("number")
        {
            TObj t("flow.demultiplex", LF(1000));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 24);
            REQUIRE_PROPERTY(t, @index, 0.f);
        }

        // invalid
        SECTION("number")
        {
            TObj t("flow.demultiplex", LF(1));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @index, 0.f);
        }

        // valid
        SECTION("number")
        {
            TObj t("flow.demultiplex", LF(8));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 8);
            REQUIRE_PROPERTY(t, @index, 0.f);
        }
    }

    SECTION("process")
    {
        TObj t("flow.demultiplex", LF(3));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_BANG_AT_OUTLET(0, t);
        WHEN_SEND_FLOAT_TO(0, t, 100);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 100);
        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "A");
        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3));
        WHEN_SEND_ANY_TO(t, "test", LF(1, 2));
        REQUIRE_ANY_AT_OUTLET(0, t, LA("test", 1, 2));
        WHEN_SEND_DATA_TO(0, t, IntData(123));
        REQUIRE_DATA_AT_OUTLET(0, t, IntA(123));

        t.setProperty("@index", LF(1));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_BANG_AT_OUTLET(1, t);
        WHEN_SEND_FLOAT_TO(0, t, 100);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 100);
        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "A");
        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(1, t, LF(1, 2, 3));
        WHEN_SEND_ANY_TO(t, "test", LF(1, 2));
        REQUIRE_ANY_AT_OUTLET(1, t, LA("test", 1, 2));
        WHEN_SEND_DATA_TO(0, t, IntData(123));
        REQUIRE_DATA_AT_OUTLET(1, t, IntA(123));

        t.setProperty("@index", LF(2));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_BANG_AT_OUTLET(2, t);
        WHEN_SEND_FLOAT_TO(0, t, 100);
        REQUIRE_FLOAT_AT_OUTLET(2, t, 100);
        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_SYMBOL_AT_OUTLET(2, t, "A");
        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(2, t, LF(1, 2, 3));
        WHEN_SEND_ANY_TO(t, "test", LF(1, 2));
        REQUIRE_ANY_AT_OUTLET(2, t, LA("test", 1, 2));
        WHEN_SEND_DATA_TO(0, t, IntData(123));
        REQUIRE_DATA_AT_OUTLET(2, t, IntA(123));

        t.setProperty("@index", LF(3));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        t.setProperty("@index", LF(-1));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);
    }

    SECTION("inlet external")
    {
        test::pdPrintToStdError();

        TExt t("flow.demultiplex");
        REQUIRE(t.object());

        TExt t2("flow.demux");
        REQUIRE(t2.object());

        pd::External f("f");
        f.connectTo(0, t, 1);

        t << 100;
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(!t.hasOutputAt(1));
        REQUIRE(t.outputFloatAt(0) == Approx(100));

        f.sendFloat(1);
        REQUIRE(t->property("@index")->get() == LF(1));
        t << 200;
        REQUIRE(!t.hasOutputAt(0));
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.outputFloatAt(1) == Approx(200));

        f.sendFloat(2);
        REQUIRE(t->property("@index")->get() == LF(2));
        t << 300;
        REQUIRE(!t.hasOutputAt(0));
        REQUIRE(!t.hasOutputAt(1));

        f.sendFloat(0);
        REQUIRE(t->property("@index")->get() == LF(0));
        t << 400;
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(!t.hasOutputAt(1));
        REQUIRE(t.outputFloatAt(0) == Approx(400));

        t.call("@size?");
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(!t.hasOutputAt(1));
        REQUIRE(t.outputAnyAt(0) == LA("@size?"));

        t.call("@index?");
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(!t.hasOutputAt(1));
        REQUIRE(t.outputAnyAt(0) == LA("@index?"));

        t.call("@index", LF(1));
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(!t.hasOutputAt(1));
        REQUIRE(t.outputAnyAt(0) == LA("@index", 1));
        REQUIRE(t->property("@index")->get() == LF(0));
    }
}
