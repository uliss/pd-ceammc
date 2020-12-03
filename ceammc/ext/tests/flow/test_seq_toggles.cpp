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
#include "seq_toggles.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(SeqToggles, seq, toggles)

TEST_CASE("seq.toggles", "[externals]")
{
    pd_test_init();
    setup_seq_bangs();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    using M = Message;
    const M done(SYM("done"), L());
    const M m1(1);
    const M m0(0.);

    using ML = std::vector<M>;
    const M B = M::makeBang();
    auto iota = [](int i) { return M(SYM("i"), LF(t_float(i))); };
    auto dur = [](t_float f) { return M(SYM("dur"), LF(f)); };
    auto len = [](t_float f) { return M(SYM("len"), LF(f)); };

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("seq.toggles");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @t, 100);
            REQUIRE_PROPERTY(t, @p, L());
            REQUIRE_PROPERTY(t, @length, 0.75);
        }

        SECTION("args")
        {
            TObj t("seq.toggles", LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @p, LF(1, 2, 3));
        }

        SECTION("args invalid")
        {
            TObj t("seq.toggles", LA("A", 2, 3, "[a: 123]"));
            REQUIRE_PROPERTY(t, @p, LF(2, 3));
        }

        SECTION("ext")
        {
            TExt t("seq.toggles");
        }

        SECTION("alias")
        {
            TExt t("seq.t");
        }
    }

    SECTION("process")
    {
        SECTION("empty")
        {
            TExt t("seq.t");

            t.sendBang();
            REQUIRE(t.messagesAt(0).empty());
            REQUIRE(t.messagesAt(1).empty());
        }

        SECTION("single")
        {
            TExt t("seq.t", LA(1, "@t", 100));

            t.sendBang(); // on
            REQUIRE(t.messagesAt(0) == ML { m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(75) });

            t.schedTicks(75); // off
            REQUIRE(t.messagesAt(0) == ML { m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(75) });

            t.schedTicks(25); // next
            REQUIRE(t.messagesAt(0) == ML { m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(75), done });

            t.schedTicks(25); // none
            REQUIRE(t.messagesAt(0) == ML { m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(75), done });
        }

        SECTION("many")
        {
            TExt t("seq.t", LA(1, 2, 0.5, "@t", 8, "@length", 0.25));

            t.sendBang(); // on
            REQUIRE(t.messagesAt(0) == ML { m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(8), len(2) });

            t.schedTicks(2); // off
            REQUIRE(t.messagesAt(0) == ML { m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(8), len(2) });

            t.schedTicks(6); // on
            REQUIRE(t.messagesAt(0) == ML { m1, m0, m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(8), len(2), 1, dur(16), len(4) });

            t.schedTicks(4); // off
            REQUIRE(t.messagesAt(0) == ML { m1, m0, m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(8), len(2), 1, dur(16), len(4) });

            t.schedTicks(12); // on
            REQUIRE(t.messagesAt(0) == ML { m1, m0, m1, m0, m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(8), len(2), 1, dur(16), len(4), 2, dur(4), len(1) });

            t.schedTicks(1); // off
            REQUIRE(t.messagesAt(0) == ML { m1, m0, m1, m0, m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(8), len(2), 1, dur(16), len(4), 2, dur(4), len(1) });

            t.schedTicks(3); // done
            REQUIRE(t.messagesAt(0) == ML { m1, m0, m1, m0, m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(8), len(2), 1, dur(16), len(4), 2, dur(4), len(1), done });
        }

        SECTION("@length=0")
        {
            TExt t("seq.t", LA(2, 1, "@t", 10, "@length", 0.f));

            t.sendBang(); // on
            REQUIRE(t.messagesAt(0) == ML { m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(20), len(1) });

            t.schedTicks(1); // off
            REQUIRE(t.messagesAt(0) == ML { m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(20), len(1) });

            t.schedTicks(19); // on
            REQUIRE(t.messagesAt(0) == ML { m1, m0, m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(20), len(1), 1, dur(10), len(1) });

            t.schedTicks(1); // off
            REQUIRE(t.messagesAt(0) == ML { m1, m0, m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(20), len(1), 1, dur(10), len(1) });

            t.schedTicks(9); // done
            REQUIRE(t.messagesAt(0) == ML { m1, m0, m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(20), len(1), 1, dur(10), len(1), done });
        }

        SECTION("@length=1")
        {
            TExt t("seq.t", LA(2, 1, "@t", 10, "@length", 1));

            t.sendBang(); // on
            REQUIRE(t.messagesAt(0) == ML { m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(20), len(20) });

            t.schedTicks(19); // none
            REQUIRE(t.messagesAt(0) == ML { m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(20), len(20) });

            t.schedTicks(1); // off-on
            REQUIRE(t.messagesAt(0) == ML { m1, m0, m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(20), len(20), 1, dur(10), len(10) });

            t.schedTicks(9); // none
            REQUIRE(t.messagesAt(0) == ML { m1, m0, m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(20), len(20), 1, dur(10), len(10) });

            t.schedTicks(1); // off
            REQUIRE(t.messagesAt(0) == ML { m1, m0, m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(20), len(20), 1, dur(10), len(10), done });
        }
    }

    SECTION("stop")
    {
        SECTION("inactive")
        {
            TExt t("seq.t", LA(2, 1, "@t", 10));

            REQUIRE(!t.hasOutput());

            t.sendMessage("stop");
            REQUIRE(!t.hasOutput());

            t.schedTicks(40);
            REQUIRE(!t.hasOutput());
        }

        SECTION("while on")
        {
            TExt t("seq.t", LA(1, 2, "@t", 100, "@length", 0.75));

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(75) });

            t.schedTicks(50); // still on
            REQUIRE(t.messagesAt(0) == ML { m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(75) });

            t.sendMessage("stop");
            REQUIRE(t.messagesAt(0) == ML { m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(75) });

            // check no changes
            t.schedTicks(100);
            REQUIRE(t.messagesAt(0) == ML { m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(75) });
        }

        SECTION("after off")
        {
            TExt t("seq.t", LA(1, 2, "@t", 100, "@length", 0.5));

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(50) });

            t.schedTicks(75); // off
            REQUIRE(t.messagesAt(0) == ML { m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(50) });

            t.sendMessage("stop");
            REQUIRE(t.messagesAt(0) == ML { m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(50) });

            // check no changes
            t.schedTicks(100);
            REQUIRE(t.messagesAt(0) == ML { m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(50) });
        }
    }

    SECTION("reset")
    {
        SECTION("inactive")
        {
            TExt t("seq.t", LA(1, 2, "@t", 10, "@length", 0.75));

            t.sendMessage("reset");
            REQUIRE(!t.hasOutput());
        }

        SECTION("while on")
        {
            TExt t("seq.t", LA(1, 2, "@t", 100, "@length", 0.75));

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(75) });

            t.schedTicks(50); // still on
            REQUIRE(t.messagesAt(0) == ML { m1 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(75) });

            t.sendMessage("reset");
            REQUIRE(t.messagesAt(0) == ML { m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(75) });

            t.schedTicks(100); // no change
            REQUIRE(t.messagesAt(0) == ML { m1, m0 });
            REQUIRE(t.messagesAt(1) == ML { iota(0), 0., dur(100), len(75) });
        }
    }
}
