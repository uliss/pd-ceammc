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
#include "seq_phasor.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(SeqPhasor, seq, phasor)

TEST_CASE("seq.phasor", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("seq.phasor");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @freq, 0.);
            REQUIRE_PROPERTY(t, @on, 0.);
            REQUIRE_PROPERTY(t, @precision, 1 / 128.0);
        }

        SECTION("args")
        {
            TObj t("seq.phasor", LA(2.5, 1));
            REQUIRE_PROPERTY(t, @freq, 2.5);
            REQUIRE_PROPERTY(t, @on, 1);
            REQUIRE_PROPERTY(t, @precision, 1 / 128.0);
        }

        SECTION("props")
        {
            TObj t("seq.phasor", LA(2.5, "@precision", "expr(1/64)"));
            REQUIRE_PROPERTY(t, @freq, 2.5);
            REQUIRE_PROPERTY(t, @precision, 1 / 64.0);
        }

        SECTION("ext")
        {
            TExt t("seq.phasor");
        }
    }

    SECTION("process")
    {
        TExt t("seq.phasor", LA(1, "@precision", 0.1)); // 640

        t << 1;

        auto dt = std::round(1000 / 11.0);

        REQUIRE(t.isOutputFloatAt(0));
        REQUIRE(t.outputFloatAt(0) == 0);
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.1));
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.2));
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.3));
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.4));
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.5));
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.6));
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.7));
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.8));
        t << 0.0;
        t.schedTicks(10 * dt);
        REQUIRE(!t.hasOutputAt(0));
        t << 1; // right after
        REQUIRE(t.outputFloatAt(0) == Approx(0.9));
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(1));
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.));
        REQUIRE(t.isOutputBangAt(1));
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.1));
        t.schedTicks(dt * 2);
        REQUIRE(t.outputFloatAt(0) == Approx(0.3));
        t.sendBangTo(1);
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0));

        t->setProperty("@precision", LF(0.5));
        dt = std::round(1000 / 3.0) + 2;

        t.clearAll();
        t << 0.;
        t.sendBangTo(1);
        t << 1;
        REQUIRE(t.isOutputFloatAt(0));
        REQUIRE(t.outputFloatAt(0) == 0);
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.5));
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(1));
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0));
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.5));

        t.call("set", 0.);
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0));

        t.call("set", 0.49);
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0));

        t.call("set", 0.5);
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.5));

        t.call("set", 0.99);
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(0.5));

        t.call("set", 1);
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(1));

        t.call("set", 1.1);
        t.schedTicks(dt);
        REQUIRE(t.outputFloatAt(0) == Approx(1));
    }
}
