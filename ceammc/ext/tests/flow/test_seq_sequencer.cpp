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
#include "seq_sequencer.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(SeqSequencer, seq, sequencer)

TEST_CASE("seq.sequencer", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("seq");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @r, 1);
            REQUIRE_PROPERTY(t, @v, L());
        }

        SECTION("args")
        {
            TObj t("seq", LA("10ms", 1, 2, 3));
            REQUIRE_PROPERTY(t, @v, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @t, 10);
        }

        SECTION("args bpm")
        {
            TObj t("seq", LA("120bpm", 1, 2, 3));
            REQUIRE_PROPERTY(t, @v, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @t, 500);
        }

        SECTION("args @once")
        {
            TObj t("seq", LA("120bpm", 1, "@once"));
            REQUIRE_PROPERTY(t, @r, 1);
        }

        SECTION("args @inf")
        {
            TObj t("seq", LA("120bpm", 2, "@inf"));
            REQUIRE_PROPERTY(t, @r, "inf");
        }

        SECTION("ext")
        {
            TExt t("seq");
        }
    }

    SECTION("process")
    {
        SECTION("simple @r 0")
        {
            using M = Message;
            using ML = std::vector<M>;

            TExt t("seq", LA("2ms", 100, 200, "@r", 0.));
            REQUIRE_PROPERTY(t, @r, 0);

            t.bang();
            t.schedTicks(2);
            REQUIRE_FALSE(t.hasOutput());
        }

        SECTION("simple @r 1")
        {
            using M = Message;
            using ML = std::vector<M>;

            auto iota = [](int i) { return M(SYM("i"), LF(t_float(i))); };
            auto done = M(SYM("done"), AtomList {});

            TExt t("seq", LA("2ms", 100, 200, "@r", 1));
            REQUIRE_PROPERTY(t, @r, 1);

            t.bang();
            REQUIRE(t.messagesAt(1) == ML { iota(0), M(0.) });
            REQUIRE(t.messagesAt(0) == ML { 100 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { iota(0), M(0.), M(1) });
            REQUIRE(t.messagesAt(0) == ML { 100, 200 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { iota(0), M(0.), M(1), done });
            REQUIRE(t.messagesAt(0) == ML { 100, 200 });
        }

        SECTION("simple @r2")
        {
            using M = Message;
            using ML = std::vector<M>;

            auto iota = [](int i) { return M(SYM("i"), LF(t_float(i))); };
            auto done = M(SYM("done"), AtomList {});

            TExt t("seq", LA("2ms", 100, 200, 300, "@r", 2));
            REQUIRE_PROPERTY(t, @r, 2);

            t.bang();
            REQUIRE(t.messagesAt(1) == ML { iota(0), M(0.) });
            REQUIRE(t.messagesAt(0) == ML { 100 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { iota(0), M(0.), M(1) });
            REQUIRE(t.messagesAt(0) == ML { 100, 200 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { iota(0), M(0.), M(1), M(2) });
            REQUIRE(t.messagesAt(0) == ML { 100, 200, 300 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { iota(0), M(0.), M(1), M(2), iota(1), M(0.) });
            REQUIRE(t.messagesAt(0) == ML { 100, 200, 300, 100 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { iota(0), M(0.), M(1), M(2), iota(1), M(0.), M(1) });
            REQUIRE(t.messagesAt(0) == ML { 100, 200, 300, 100, 200 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { iota(0), M(0.), M(1), M(2), iota(1), M(0.), M(1), M(2) });
            REQUIRE(t.messagesAt(0) == ML { 100, 200, 300, 100, 200, 300 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { iota(0), M(0.), M(1), M(2), iota(1), M(0.), M(1), M(2), done });
            REQUIRE(t.messagesAt(0) == ML { 100, 200, 300, 100, 200, 300 });
        }

        SECTION("simple empty sequence")
        {
            using M = Message;
            using ML = std::vector<M>;

            TExt t("seq", LA("2ms", "@r", 2));

            t.bang();
            t.schedTicks(2);
            REQUIRE_FALSE(t.hasOutput());
        }

        SECTION("mlist @r 1")
        {
            using M = Message;
            using ML = std::vector<M>;

            auto iota = [](int i) { return M(SYM("i"), LF(t_float(i))); };
            auto done = M(SYM("done"), AtomList {});

            TExt t("seq", LA("2ms", "(100", "127)", "@r", 1));
            REQUIRE_PROPERTY(t, @r, 1);

            t.bang();
            REQUIRE(t.messagesAt(1) == ML { iota(0), M(0.) });
            REQUIRE(t.messagesAt(0) == ML { LF(100, 127) });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { iota(0), M(0.), done });
            REQUIRE(t.messagesAt(0) == ML { LF(100, 127) });
        }
    }

    SECTION("time")
    {
        SECTION("float")
        {
            TExt t("seq", LF(100, 100, 200, 300));
            REQUIRE_PROPERTY(t, @t, 100);
        }

        SECTION("ms")
        {
            TExt t("seq", LA("50ms", 100, 200, 300));
            REQUIRE_PROPERTY(t, @t, 50);
        }

        SECTION("bpm")
        {
            TExt t("seq", LA("120bpm", 100, 200, 300));
            REQUIRE_PROPERTY(t, @t, 500);
        }

        SECTION("hz")
        {
            TExt t("seq", LA("1hz", 100, 200, 300, 400));
            REQUIRE_PROPERTY(t, @t, 250);
        }
    }
}
