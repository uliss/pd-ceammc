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
#include "test_seq_base.h"

PD_COMPLETE_TEST_SETUP(SeqSequencer, seq, sequencer)

TEST_CASE("seq.sequencer", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    using M = Message;
    using ML = std::vector<M>;

    auto ri = [](int i, int n) { return M(SYM("ri"), LF(t_float(i), t_float(n))); };
    auto i = [](int i, int n) { return M(SYM("i"), LF(t_float(i), t_float(n))); };
    auto done = M(SYM("done"), AtomList {});

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
            TObj t("seq", LA(1, 2, 3, "@t", "10ms"));
            REQUIRE_PROPERTY(t, @v, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @t, 10);
        }

        SECTION("args bpm")
        {
            TObj t("seq", LA(1, 2, 3, "@t", "120bpm"));
            REQUIRE_PROPERTY(t, @v, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @t, 500);
        }

        SECTION("args @once")
        {
            TObj t("seq", LA(1, "@once", "@t", "120bpm"));
            REQUIRE_PROPERTY(t, @r, 1);
            REQUIRE_PROPERTY(t, @t, 500);
        }

        SECTION("args @inf")
        {
            TObj t("seq", LA(2, "@inf", "@t", "120bpm"));
            REQUIRE_PROPERTY(t, @r, "inf");
        }

        SECTION("args @dur")
        {
            TObj t("seq", LA(1, 2, 3, 4, "@dur", 500));
            REQUIRE_PROPERTY(t, @dur, 500);
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
            TExt t("seq", LA(100, 200, "@r", 0., "@t", "2ms"));
            REQUIRE_PROPERTY(t, @r, 0);

            t.bang();
            t.schedTicks(2);
            REQUIRE_FALSE(t.hasOutput());
        }

        SECTION("simple @r 1")
        {
            TExt t("seq", LA(100, 200, "@r", 1, "@t", "2ms"));
            REQUIRE_PROPERTY(t, @r, 1);

            t.bang();
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 2) });
            REQUIRE(t.messagesAt(0) == ML { 100 });
            t.schedTicks(2_wd);
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 2), i(1, 2) });
            REQUIRE(t.messagesAt(0) == ML { 100, 200 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 2), i(1, 2), done });
            REQUIRE(t.messagesAt(0) == ML { 100, 200 });
        }

        SECTION("simple @r2")
        {
            TExt t("seq", LA(100, 200, 300, "@r", 2, "@t", "2ms"));
            REQUIRE_PROPERTY(t, @r, 2);

            t.bang();
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3) });
            REQUIRE(t.messagesAt(0) == ML { 100 });
            t.schedTicks(2_wd);
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), i(1, 3) });
            REQUIRE(t.messagesAt(0) == ML { 100, 200 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), i(1, 3), i(2, 3) });
            REQUIRE(t.messagesAt(0) == ML { 100, 200, 300 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), i(1, 3), i(2, 3), ri(1, 2), i(0, 3) });
            REQUIRE(t.messagesAt(0) == ML { 100, 200, 300, 100 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), i(1, 3), i(2, 3), ri(1, 2), i(0, 3), i(1, 3) });
            REQUIRE(t.messagesAt(0) == ML { 100, 200, 300, 100, 200 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), i(1, 3), i(2, 3), ri(1, 2), i(0, 3), i(1, 3), i(2, 3) });
            REQUIRE(t.messagesAt(0) == ML { 100, 200, 300, 100, 200, 300 });
            t.schedTicks(2);
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), i(1, 3), i(2, 3), ri(1, 2), i(0, 3), i(1, 3), i(2, 3), done });
            REQUIRE(t.messagesAt(0) == ML { 100, 200, 300, 100, 200, 300 });
            t.schedTicks(20);
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), i(1, 3), i(2, 3), ri(1, 2), i(0, 3), i(1, 3), i(2, 3), done });
            REQUIRE(t.messagesAt(0) == ML { 100, 200, 300, 100, 200, 300 });
        }

        SECTION("simple empty sequence")
        {
            TExt t("seq", LA("@r", 2, "@t", "2ms"));

            t.bang();
            t.schedTicks(2);
            REQUIRE_FALSE(t.hasOutput());
        }

        SECTION("mlist @r 1")
        {
            TExt t("seq", LA("(100", "127)", "@r", 1, "@t", "2ms"));
            REQUIRE_PROPERTY(t, @r, 1);

            t.bang();
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 1) });
            REQUIRE(t.messagesAt(0) == ML { LF(100, 127) });
            t.schedTicks(2_wd);
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 1), done });
            REQUIRE(t.messagesAt(0) == ML { LF(100, 127) });
        }
    }

    SECTION("time")
    {
        SECTION("float")
        {
            TExt t("seq", LA(100, 200, 300, "@t", 11));
            REQUIRE_PROPERTY(t, @t, 11);
        }

        SECTION("ms")
        {
            TExt t("seq", LA(100, 200, 300, "@t", "50ms"));
            REQUIRE_PROPERTY(t, @t, 50);
        }

        SECTION("bpm")
        {
            TExt t("seq", LA(100, 200, 300, "@t", "120bpm"));
            REQUIRE_PROPERTY(t, @t, 500);
        }

        SECTION("dur")
        {
            TExt t("seq", LA(100, 200, 300, 400, "@dur", 1000));
            REQUIRE_PROPERTY(t, @t, 250);
        }
    }

    SECTION("manual")
    {
        TExt t("seq", LA(100, 200, "@r", 2, "@t", "2ms"));

        t->m_tick(&s_, {});
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 2) });
        REQUIRE(t.messagesAt(0) == ML { 100 });
        t->m_tick(&s_, {});
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 2), i(1, 2) });
        REQUIRE(t.messagesAt(0) == ML { 100, 200 });
        t->m_tick(&s_, {});
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 2), i(1, 2), ri(1, 2), i(0, 2) });
        REQUIRE(t.messagesAt(0) == ML { 100, 200, 100 });
        t->m_tick(&s_, {});
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 2), i(1, 2), ri(1, 2), i(0, 2), i(1, 2) });
        REQUIRE(t.messagesAt(0) == ML { 100, 200, 100, 200 });
        t->m_tick(&s_, {});
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 2), i(1, 2), ri(1, 2), i(0, 2), i(1, 2), done });
        REQUIRE(t.messagesAt(0) == ML { 100, 200, 100, 200 });
        t->m_tick(&s_, {});
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 2), i(1, 2), ri(1, 2), i(0, 2), i(1, 2), done, done });
        REQUIRE(t.messagesAt(0) == ML { 100, 200, 100, 200 });
    }

    SECTION("dec")
    {
        TExt t("seq", LA(1, 2, 3, "@r", 2, "@t", 2, "@mode", "dec"));

        t.call("start");
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(2, 3) });
        REQUIRE(t.messagesAt(0) == ML { 3 });

        t.schedTicks(2_wd);
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(2, 3), i(1, 3) });
        REQUIRE(t.messagesAt(0) == ML { 3, 2 });

        t.schedTicks(2);
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(2, 3), i(1, 3), i(0, 3) });
        REQUIRE(t.messagesAt(0) == ML { 3, 2, 1 });

        t.schedTicks(2);
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(2, 3), i(1, 3), i(0, 3), ri(1, 2), i(2, 3) });
        REQUIRE(t.messagesAt(0) == ML { 3, 2, 1, 3 });

        t.schedTicks(2);
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(2, 3), i(1, 3), i(0, 3), ri(1, 2), i(2, 3), i(1, 3) });
        REQUIRE(t.messagesAt(0) == ML { 3, 2, 1, 3, 2 });

        t.schedTicks(2);
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(2, 3), i(1, 3), i(0, 3), ri(1, 2), i(2, 3), i(1, 3), i(0, 3) });
        REQUIRE(t.messagesAt(0) == ML { 3, 2, 1, 3, 2, 1 });

        t.schedTicks(2);
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(2, 3), i(1, 3), i(0, 3), ri(1, 2), i(2, 3), i(1, 3), i(0, 3), done });
        REQUIRE(t.messagesAt(0) == ML { 3, 2, 1, 3, 2, 1 });
    }

    SECTION("tri")
    {
        TExt t("seq", LA(1, 2, 3, "@r", 2, "@t", "2ms", "@mode", "tri"));

        t.call("start");
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3) });
        REQUIRE(t.messagesAt(0) == ML { 1 });

        t.schedTicks(2_wd);
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), i(1, 3) });
        REQUIRE(t.messagesAt(0) == ML { 1, 2 });

        t.schedTicks(2);
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), i(1, 3), i(2, 3) });
        REQUIRE(t.messagesAt(0) == ML { 1, 2, 3 });

        t.schedTicks(2);
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), i(1, 3), i(2, 3), i(1, 3) });
        REQUIRE(t.messagesAt(0) == ML { 1, 2, 3, 2 });

        t.schedTicks(2);
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), i(1, 3), i(2, 3), i(1, 3), ri(1, 2), i(0, 3) });
        REQUIRE(t.messagesAt(0) == ML { 1, 2, 3, 2, 1 });

        t.schedTicks(2);
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), i(1, 3), i(2, 3), i(1, 3), ri(1, 2), i(0, 3), i(1, 3) });
        REQUIRE(t.messagesAt(0) == ML { 1, 2, 3, 2, 1, 2 });

        t.schedTicks(2);
        REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), i(1, 3), i(2, 3), i(1, 3), ri(1, 2), i(0, 3), i(1, 3), i(2, 3) });
        REQUIRE(t.messagesAt(0) == ML { 1, 2, 3, 2, 1, 2, 3 });
    }
}
