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
#include "test_seq_base.h"

PD_COMPLETE_TEST_SETUP(SeqNBangsT, seq, nbangs)

TEST_CASE("seq.nbangs", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    using M = Message;
    using ML = std::vector<M>;
    const M B = M::makeBang();
    auto i = [](int i, int n) { return M(SYM("i"), LF(t_float(i), t_float(n))); };
    const M done(SYM("done"), AtomListView());

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("seq.nbangs");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @t, 0);
            REQUIRE_PROPERTY(t, @n, 0);
            REQUIRE_PROPERTY(t, @div, 1);
            REQUIRE_PROPERTY_FLOAT(t, @accel, 1);
            REQUIRE_PROPERTY_FLOAT(t, @curve, 1);
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
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 4) });

            t.schedTicks(1_wd);
            REQUIRE(t.messagesAt(0) == ML { B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 4), i(1, 4) });

            t.schedTicks(1);
            REQUIRE(t.messagesAt(0) == ML { B, B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 4), i(1, 4), i(2, 4) });

            t.schedTicks(1);
            REQUIRE(t.messagesAt(0) == ML { B, B, B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 4), i(1, 4), i(2, 4), i(3, 4) });

            t.schedTicks(1);
            REQUIRE(t.messagesAt(0) == ML { B, B, B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 4), i(1, 4), i(2, 4), i(3, 4), done });

            t.schedTicks(100);
            REQUIRE(t.messagesAt(0) == ML { B, B, B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 4), i(1, 4), i(2, 4), i(3, 4), done });
        }

        SECTION("float")
        {
            TExt t("seq.nb", LF(0, 2));
            t << 3;
            REQUIRE_PROPERTY(t, @n, 3);

            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 3) });

            t.schedTicks(2_wd);
            REQUIRE(t.messagesAt(0) == ML { B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 3), i(1, 3) });

            t.schedTicks(2);
            REQUIRE(t.messagesAt(0) == ML { B, B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 3), i(1, 3), i(2, 3) });

            t.schedTicks(2);
            REQUIRE(t.messagesAt(0) == ML { B, B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 3), i(1, 3), i(2, 3), done });
        }

        SECTION("list")
        {
            TExt t("seq.nb");
            t << LF(2, 3);
            REQUIRE_PROPERTY(t, @n, 2);
            REQUIRE_PROPERTY(t, @t, 3);

            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 2) });

            t.schedTicks(3_wd);
            REQUIRE(t.messagesAt(0) == ML { B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 2), i(1, 2) });

            t.schedTicks(3);
            REQUIRE(t.messagesAt(0) == ML { B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 2), i(1, 2), done });
        }

        SECTION("start/stop")
        {
            TExt t("seq.nb", LF(5, 5));
            t.call("start");

            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 5) });

            t.schedTicks(5_wd);
            REQUIRE(t.messagesAt(0) == ML { B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 5), i(1, 5) });

            t.sendMessage("stop");
            t.schedTicks(100);
            REQUIRE(t.messagesAt(0) == ML { B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 5), i(1, 5) });

            t.sendMessage("start");
            REQUIRE(t.messagesAt(0) == ML { B, B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 5), i(1, 5), i(2, 5) });
        }

        SECTION("reset")
        {
            TExt t("seq.nb", LF(5, 1));

            t.call("start");
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 5) });

            t.schedTicks(1_wd);
            REQUIRE(t.messagesAt(0) == ML { B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 5), i(1, 5) });

            t.call("reset");
            t.schedTicks(100);
            REQUIRE_FALSE(t.hasOutput());

            t.call("start");
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 5) });

            t.schedTicks(1_wd);
            REQUIRE(t.messagesAt(0) == ML { B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 5), i(1, 5) });
        }

        SECTION("tick")
        {
            TExt t("seq.nb", LF(3, 1));
            t.call("tick");
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 3) });

            t.schedTicks(100_wd);
            t.call("tick");
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { i(1, 3) });

            t.schedTicks(100);
            t.call("tick");
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { i(2, 3) });

            t.schedTicks(100);
            t.call("tick");
            REQUIRE(t.messagesAt(0) == ML {});
            REQUIRE(t.messagesAt(1) == ML { done });
        }

        SECTION("rewind")
        {
            TExt t("seq.nb", LF(3, 2));
            t.bang();
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 3) });

            t.schedTicks(2_wd);
            REQUIRE(t.messagesAt(0) == ML { B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 3), i(1, 3) });

            t.schedTicks(1);
            t.sendMessage("rewind");
            REQUIRE(t.messagesAt(0) == ML { B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 3), i(1, 3) });

            t.schedTicks(1);
            REQUIRE(t.messagesAt(0) == ML { B, B, B });
            REQUIRE(t.messagesAt(1) == ML { i(0, 3), i(1, 3), i(0, 3) });
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

        SECTION("bpm")
        {
            TObj t("seq.nbangs", LA(10, "120|8bpm"));
            REQUIRE_PROPERTY(t, @t, 500);
            REQUIRE_PROPERTY(t, @n, 10);
        }

        SECTION("bpm")
        {
            TObj t("seq.nbangs", LA(10, "120|16bpm"));
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

    SECTION("total")
    {
        TObj t("seq.nbangs", LA(4, "40ms", "@curve", 0.));
        REQUIRE(t.calcTotalDur() == 160);

        REQUIRE(t.calcStepDelay(0) == 40);
        REQUIRE(t.calcStepDelay(1) == 40);
        REQUIRE(t.calcStepDelay(2) == 40);
        REQUIRE(t.calcStepDelay(3) == 40);
        REQUIRE(t.calcIntervalByDur(160) == 40);

        t.setProperty("@accel", LF(4));

        REQUIRE(t.calcStepDelay(0) == 40);
        REQUIRE(t.calcStepDelay(1) == 25);
        REQUIRE(t.calcStepDelay(2) == 10);

        REQUIRE(t.calcTotalDur() == (40 + 25 + 10));
        REQUIRE(t.calcIntervalByDur(40 + 25 + 10) == Approx(40));

        t.setProperty("@curve", LF(1));
        REQUIRE(t.calcStepDelay(0) == 40);
        REQUIRE(t.calcStepDelay(1) == Approx(28.67378));
        REQUIRE(t.calcStepDelay(2) == Approx(10));
        REQUIRE(t.calcTotalDur() == Approx(78.67378));
        REQUIRE(t.calcIntervalByDur(78.67378) == Approx(40));

        t.setProperty("@curve", LF(2));
        REQUIRE(t.calcStepDelay(0) == 40);
        REQUIRE(t.calcStepDelay(1) == Approx(31.93176));
        REQUIRE(t.calcStepDelay(2) == Approx(10));
        REQUIRE(t.calcTotalDur() == Approx(81.93176));
        REQUIRE(t.calcIntervalByDur(81.93176) == Approx(40));

        t.setProperty("@curve", LF(-2));
        REQUIRE(t.calcStepDelay(0) == 40);
        REQUIRE(t.calcStepDelay(1) == Approx(18.06824));
        REQUIRE(t.calcStepDelay(2) == Approx(10));
        REQUIRE(t.calcTotalDur() == Approx(68.06824));
        REQUIRE(t.calcIntervalByDur(68.06824) == Approx(40));

        t.setProperty("@div", LF(2));
        REQUIRE(t.calcTotalDur() == Approx(68.06824 * 0.5));
    }
}
