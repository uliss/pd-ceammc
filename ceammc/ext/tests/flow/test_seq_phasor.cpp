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
#include "test_seq_base.h"

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
            REQUIRE_PROPERTY(t, @invert, 0.);
            REQUIRE_PROPERTY(t, @open, 0.);
            REQUIRE_PROPERTY(t, @steps, 128);
        }

        SECTION("args")
        {
            TObj t("seq.phasor", LA(2.5));
            REQUIRE_PROPERTY(t, @freq, 2.5);
            REQUIRE_PROPERTY(t, @on, 0);
            REQUIRE_PROPERTY(t, @steps, 128);
        }

        SECTION("props")
        {
            TObj t("seq.phasor", LA(2.5, "@steps", "expr(2^7)"));
            REQUIRE_PROPERTY(t, @freq, 2.5);
            REQUIRE_PROPERTY(t, @steps, 128);
        }

        SECTION("ext")
        {
            TExt t("seq.phasor");
        }
    }

    SECTION("process")
    {
        SECTION("clock")
        {
        }

        SECTION("manual")
        {
            TExt t("seq.phasor", LA(1, "@steps", 4)); // 640
            REQUIRE_PROPERTY(t, @steps, 4);

            REQUIRE(t->value() == 0);
            t->tick();
            REQUIRE(t->value() == Approx(1 / 3.0));
            t->tick();
            REQUIRE(t->value() == Approx(2 / 3.0));
            t->tick();
            REQUIRE(t->value() == Approx(3 / 3.0));
            t->tick();
            REQUIRE(t->value() == Approx(0 / 3.0));
        }

        SECTION("manual @open")
        {
            TExt t("seq.phasor", LA(1, "@steps", 4, "@open", 1));
            REQUIRE_PROPERTY(t, @steps, 4);

            REQUIRE(t->value() == 0);
            t->tick();
            REQUIRE(t->value() == Approx(1 / 4.0));
            t->tick();
            REQUIRE(t->value() == Approx(2 / 4.0));
            t->tick();
            REQUIRE(t->value() == Approx(3 / 4.0));
            t->tick();
            REQUIRE(t->value() == Approx(0 / 4.0));
            t->tick();
            REQUIRE(t->value() == Approx(1 / 4.0));
        }

        SECTION("manual @invert")
        {
            TExt t("seq.phasor", LA(1, "@steps", 4, "@invert", 1)); // 640
            REQUIRE_PROPERTY(t, @steps, 4);

            t->reset();

            REQUIRE(t->value() == Approx(3 / 3.0));
            t->tick();
            REQUIRE(t->value() == Approx(2 / 3.0));
            t->tick();
            REQUIRE(t->value() == Approx(1 / 3.0));
            t->tick();
            REQUIRE(t->value() == Approx(0 / 3.0));
            t->tick();
            REQUIRE(t->value() == Approx(3 / 3.0));
        }

        SECTION("manual @invert @open")
        {
            TExt t("seq.phasor", LA(1, "@steps", 4, "@invert", 1, "@open", 1)); // 640
            REQUIRE_PROPERTY(t, @steps, 4);

            t->reset();

            REQUIRE(t->value() == Approx(3 / 4.0));
            t->tick();
            REQUIRE(t->value() == Approx(2 / 4.0));
            t->tick();
            REQUIRE(t->value() == Approx(1 / 4.0));
            t->tick();
            REQUIRE(t->value() == Approx(0 / 4.0));
            t->tick();
            REQUIRE(t->value() == Approx(3 / 4.0));
            t->tick();
            REQUIRE(t->value() == Approx(2 / 4.0));
        }

        SECTION("phase")
        {
            TExt t("seq.phasor", LA(1, "@steps", 5));
            REQUIRE_PROPERTY(t, @steps, 5);

            t->reset();
            REQUIRE(t->value() == Approx(0 / 4.0));

            t.call("phase", LF(0));
            REQUIRE(t->value() == Approx(0));

            t.call("phase", LF(0.25));
            REQUIRE(t->value() == Approx(1 / 4.0));

            t.call("phase", LF(0.5));
            REQUIRE(t->value() == Approx(2 / 4.0));

            t.call("phase", LF(0.75));
            REQUIRE(t->value() == Approx(3 / 4.0));

            t.call("phase", LF(1));
            REQUIRE(t->value() == Approx(1));
        }

        SECTION("phase @open")
        {
            TExt t("seq.phasor", LA(1, "@steps", 4, "@open", 1));
            REQUIRE_PROPERTY(t, @steps, 4);

            t->reset();
            REQUIRE(t->value() == Approx(0 / 4.0));

            t.call("phase", LF(0));
            REQUIRE(t->value() == Approx(0));

            t.call("phase", LF(0.25));
            REQUIRE(t->value() == Approx(1 / 4.0));

            t.call("phase", LF(0.5));
            REQUIRE(t->value() == Approx(2 / 4.0));

            t.call("phase", LF(0.75));
            REQUIRE(t->value() == Approx(3 / 4.0));

            t.call("phase", LF(1));
            REQUIRE(t->value() == Approx(0));
        }
    }
}
