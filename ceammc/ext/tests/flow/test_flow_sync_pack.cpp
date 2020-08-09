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
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "flow_sync_pack.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(FlowPack, flow, sync_pack)

TEST_CASE("flow.sync_pack", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            TObj t("flow.sync_pack");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @n, 1);
            REQUIRE_PROPERTY_LIST(t, @init, LF(0));

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        }

        SECTION("wrong args")
        {
            TObj t("flow.sync_pack", LA("a"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @n, 1);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        }

        SECTION("args")
        {
            TObj t("flow.sync_pack", LX(0));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @n, 1);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        }

        SECTION("args")
        {
            TObj t("flow.sync_pack", LX(-1));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @n, 1);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        }

        SECTION("args")
        {
            TObj t("flow.sync_pack", LX(2));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @n, 2);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, LX(0, 0));
        }

        SECTION("args")
        {
            TObj t("flow.sync_pack", LX(5));
            REQUIRE(t.numInlets() == 5);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @n, 5);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, LX(0, 0, 0, 0, 0));
        }
    }

    SECTION("bang")
    {
        TExt t("flow.sync_pack", LA(3, "a", "b", "c"));

        t.sendBangTo(2);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA("a", "b", "c"));

        t.sendBangTo(1);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA("a", "b", "c"));

        t.sendBangTo(0);
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputListAt(0));
        REQUIRE(t.outputListAt(0) == LA("a", "b", "c"));
    }

    SECTION("float")
    {
        TExt t("flow.sync_pack", LA(3, "a", "b", "c"));

        t.sendFloatTo(100, 2);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA("a", "b", 100));

        t.sendFloatTo(-100, 1);
        REQUIRE(t.outputListAt(0) == LA("a", -100, 100));

        t.sendFloatTo(11, 0);
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputListAt(0));
        REQUIRE(t.outputListAt(0) == LA(11, -100, 100));

        t.sendFloat(12);
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputListAt(0));
        REQUIRE(t.outputListAt(0) == LA(12, -100, 100));
    }

    SECTION("symbol")
    {
        TExt t("flow.sync_pack", LA(3, "a", "b", "c"));

        t.sendBang();
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA("a", "b", "c"));

        t.sendSymbolTo("C", 2);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA("a", "b", "C"));

        t.sendSymbolTo("B", 1);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA("a", "B", "C"));

        t.sendSymbolTo("A", 0);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA("A", "B", "C"));
    }

    SECTION("list")
    {
        TExt t("flow.sync_pack", LA(4, "a", "b", "c", "d"));

        t.sendListTo(LX(1, 2, 3), 2);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA("a", "b", 1, 2));

        t.sendListTo(LX(100, 200), 1);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA("a", 100, 200, 2));

        t.sendListTo(LX(1000), 1);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA("a", 1000, 200, 2));

        t.sendListTo(L(), 1);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA("a", 1000, 200, 2));

        t.sendListTo(LA(-1, -2), 0);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LA(-1, -2, 200, 2));

        t.sendListTo(LA("A"), 0);
        REQUIRE(t.outputListAt(0) == LA("A", -2, 200, 2));

        t.sendListTo(LA("A", "B"), 0);
        REQUIRE(t.outputListAt(0) == LA("A", "B", 200, 2));

        t.sendListTo(LA("A", "B", "C"), 0);
        REQUIRE(t.outputListAt(0) == LA("A", "B", "C", 2));

        t.sendListTo(LA("A", "B", "C", "D"), 0);
        REQUIRE(t.outputListAt(0) == LA("A", "B", "C", "D"));

        t.sendListTo(LA("+", "B", "C", "D", "E"), 0);
        REQUIRE(t.outputListAt(0) == LA("+", "B", "C", "D"));
    }

    SECTION("any")
    {
        TExt t("flow.sync_pack", LA(3));

        t.bang();
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputListAt(0));
        REQUIRE(t.outputListAt(0) == LX(0, 0, 0));

        t.sendMessage(gensym("m"));
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputAnyAt(0));
        REQUIRE(t.outputAnyAt(0) == LA("m", 0.f, 0.f));

        t.sendMessage(gensym("m"), LF(-100));
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputAnyAt(0));
        REQUIRE(t.outputAnyAt(0) == LA("m", -100, 0.f));

        t.sendMessage(gensym("m"), LF(-100, 200));
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputAnyAt(0));
        REQUIRE(t.outputAnyAt(0) == LA("m", -100, 200));

        t.sendMessage(gensym("m"), LF(1, 2, 3));
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputAnyAt(0));
        REQUIRE(t.outputAnyAt(0) == LA("m", 1, 2));
    }

    SECTION("alias")
    {
        TExt t("flow.pack'");
    }
}
