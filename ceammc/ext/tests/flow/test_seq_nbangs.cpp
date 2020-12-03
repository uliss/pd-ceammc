/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "seq_nbangs.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(SeqNBangsT, seq, nbangs)

TEST_CASE("seq.nbangs", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("seq.nbangs");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @t, 0);
            REQUIRE_PROPERTY(t, @n, 0);
        }

        SECTION("args")
        {
            TObj t("seq.nbangs", LF(10, 20));
            REQUIRE_PROPERTY(t, @t, 20);
            REQUIRE_PROPERTY(t, @n, 10);
            REQUIRE_PROPERTY(t, @dur, 200);
        }

        SECTION("props")
        {
            TObj t("seq.nbangs", LA("@t", 10, "@n", 20));
            REQUIRE_PROPERTY(t, @t, 10);
            REQUIRE_PROPERTY(t, @n, 20);
        }

        SECTION("ext")
        {
            TExt t0("seq.nbangs");
            TExt t1("seq.nb");
        }
    }

    SECTION("process")
    {
        SECTION("bang")
        {
            TExt t("seq.nb", LF(4, 1));
            t.sendBang();
            REQUIRE(t.outputFloatAt(1) == 0);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.schedTicks(1);
            REQUIRE(t.outputFloatAt(1) == 1);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.schedTicks(1);
            REQUIRE(t.outputFloatAt(1) == 2);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.schedTicks(1);
            REQUIRE(t.outputFloatAt(1) == 3);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.schedTicks(1);
            REQUIRE(t.outputAnyAt(1) == LA("done"));
            REQUIRE_FALSE(t.hasOutputAt(0));
        }

        SECTION("float")
        {
            TExt t("seq.nb", LF(0, 2));
            t << 3;
            REQUIRE_PROPERTY(t, @n, 3);
            REQUIRE(t.outputFloatAt(1) == 0);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.schedTicks(2);
            REQUIRE(t.outputFloatAt(1) == 1);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.schedTicks(2);
            REQUIRE(t.outputFloatAt(1) == 2);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.schedTicks(2);
            REQUIRE(t.outputAnyAt(1) == LA("done"));
            REQUIRE_FALSE(t.hasOutputAt(0));
        }

        SECTION("list")
        {
            TExt t("seq.nb");
            t << LF(2, 3);
            REQUIRE_PROPERTY(t, @n, 2);
            REQUIRE_PROPERTY(t, @t, 3);
            REQUIRE(t.outputFloatAt(1) == 0);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.schedTicks(3);
            REQUIRE(t.outputFloatAt(1) == 1);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.schedTicks(3);
            REQUIRE(t.outputAnyAt(1) == LA("done"));
            REQUIRE_FALSE(t.hasOutputAt(0));
        }

        SECTION("start")
        {
            TExt t("seq.nb", LF(5, 5));
            t.call("start");

            REQUIRE(t.outputFloatAt(1) == 0);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.schedTicks(5);

            REQUIRE(t.outputFloatAt(1) == 1);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.call("start", LF(0));
            t.schedTicks(5);
            REQUIRE_FALSE(t.hasOutput());

            t.call("start", LF(1));
            REQUIRE(t.outputFloatAt(1) == 0);
            REQUIRE(t.isOutputBangAt(0));
        }

        SECTION("stop")
        {
            TExt t("seq.nb", LF(5, 5));
            t.call("start");

            REQUIRE(t.outputFloatAt(1) == 0);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.call("stop");
            t.schedTicks(5);
            REQUIRE_FALSE(t.hasOutput());

            t.call("stop", LF(0));
            REQUIRE(t.outputFloatAt(1) == 0);
            REQUIRE(t.isOutputBangAt(0));

            t.schedTicks(5);
            REQUIRE(t.outputFloatAt(1) == 1);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.call("stop", LF(1));
            t.schedTicks(5);
            REQUIRE_FALSE(t.hasOutput());
        }

        SECTION("reset")
        {
            TExt t("seq.nb", LF(5, 1));

            t.call("start");
            REQUIRE(t.outputFloatAt(1) == 0);
            REQUIRE(t.isOutputBangAt(0));

            t.schedTicks(1);
            REQUIRE(t.outputFloatAt(1) == 1);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.call("reset");
            t.schedTicks(100);
            REQUIRE_FALSE(t.hasOutput());

            t.call("start");
            REQUIRE(t.outputFloatAt(1) == 0);
            REQUIRE(t.isOutputBangAt(0));
            t.schedTicks(1);
            REQUIRE(t.outputFloatAt(1) == 1);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.sendBangTo(1);
            t.schedTicks(100);
            REQUIRE_FALSE(t.hasOutput());
        }

        SECTION("tick")
        {
            TExt t("seq.nb", LF(3, 1));
            t.call("tick");
            REQUIRE(t.outputFloatAt(1) == 0);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.schedTicks(100);
            t.call("tick");
            REQUIRE(t.outputFloatAt(1) == 1);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.schedTicks(100);
            t.call("tick");
            REQUIRE(t.outputFloatAt(1) == 2);
            REQUIRE(t.isOutputBangAt(0));

            t.clearAll();
            t.schedTicks(100);
            t.call("tick");
            REQUIRE(t.outputAnyAt(1) == LA("done"));
            REQUIRE_FALSE(t.hasOutputAt(0));
        }
    }

    SECTION("time")
    {
        SECTION("ms")
        {
            TObj t("seq.nbangs", LA(10, "20ms"));
            REQUIRE_PROPERTY(t, @t, 20);
            REQUIRE_PROPERTY(t, @n, 10);
        }

        SECTION("bpm")
        {
            TObj t("seq.nbangs", LA(10, "120bpm"));
            REQUIRE_PROPERTY(t, @t, 500);
            REQUIRE_PROPERTY(t, @n, 10);
        }

        SECTION("dur")
        {
            TObj t("seq.nbangs", LA(10, "@dur", 150));
            REQUIRE_PROPERTY(t, @t, 15);
            REQUIRE_PROPERTY(t, @n, 10);
        }
    }
}
