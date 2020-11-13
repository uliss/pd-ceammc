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

#define REQUIRE_TGL(t, tgl, dur, idx)                       \
    do {                                                    \
        REQUIRE(t.messagesAt(0).back() == M(t_float(tgl))); \
        REQUIRE(t.messagesAt(1).back() == M(t_float(dur))); \
        REQUIRE(t.messagesAt(2).back() == M(idx));          \
    } while (0)

#define REQUIRE_ON(t, dur, idx) REQUIRE_TGL(t, 1, dur, idx)
#define REQUIRE_OFF(t, dur, idx) REQUIRE_TGL(t, 0, dur, idx)

#define REQUIRE_N(t, n0, n1, n2)               \
    do {                                       \
        REQUIRE(t.messagesAt(0).size() == n0); \
        REQUIRE(t.messagesAt(1).size() == n1); \
        REQUIRE(t.messagesAt(2).size() == n2); \
    } while (0)

constexpr unsigned long long operator"" _idx(unsigned long long idx)
{
    return idx;
}

TEST_CASE("seq.toggles", "[externals]")
{
    pd_test_init();
    setup_seq_bangs();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    using M = Message;
    const M done(SYM("done"), L());
    const M m_on(1);
    const M m_off(0.);

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("seq.toggles");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 3);
            REQUIRE_PROPERTY(t, @bpm, 60);
            REQUIRE_PROPERTY(t, @div, 4);
            REQUIRE_PROPERTY(t, @pattern, L());
            REQUIRE_PROPERTY(t, @dur, 0.75);
        }

        SECTION("args")
        {
            TObj t("seq.toggles", LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @pattern, LF(1, 2, 3));
        }

        SECTION("args invalid")
        {
            TObj t("seq.toggles", LA("A", 2, 3, "[a: 123]"));
            REQUIRE_PROPERTY(t, @pattern, LF(2, 3));
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
            REQUIRE(t.messagesAt(2).empty());
        }

        SECTION("single")
        {
            TExt t("seq.t", LA(1, "@bpm", 600, "@div", 4));

            t.sendBang(); // on
            REQUIRE_N(t, 1, 1, 1);
            REQUIRE_ON(t, 100, 0_idx);

            t.schedTicks(75); // off
            REQUIRE_N(t, 2, 1, 1);
            REQUIRE_OFF(t, 100, 0_idx);

            t.schedTicks(25); // next
            REQUIRE_N(t, 2, 1, 2);
            REQUIRE_OFF(t, 100, done);
        }

        SECTION("many")
        {
            TExt t("seq.t", LA(1, 2, 0.5, "@bpm", 600, "@div", 40, "@dur", 0.25));

            t.sendBang(); // on
            REQUIRE_N(t, 1, 1, 1);
            REQUIRE_ON(t, 10, 0_idx);

            t.schedTicks(3); // off
            REQUIRE_N(t, 2, 1, 1);
            REQUIRE_OFF(t, 10, 0_idx);

            t.schedTicks(7); // on
            REQUIRE_N(t, 3, 2, 2);
            REQUIRE_ON(t, 20, 1_idx);

            t.schedTicks(5); // off
            REQUIRE_N(t, 4, 2, 2);
            REQUIRE_OFF(t, 20, 1_idx);

            t.schedTicks(15); // on
            REQUIRE_N(t, 5, 3, 3);
            REQUIRE_ON(t, 5, 2_idx);

            t.schedTicks(2); // off
            REQUIRE_N(t, 6, 3, 3);
            REQUIRE_OFF(t, 5, 2_idx);

            t.schedTicks(3); // done
            REQUIRE_N(t, 6, 3, 4);
            REQUIRE_OFF(t, 5, done);
        }

        SECTION("@dur=0")
        {
            TExt t("seq.t", LA(2, 1, "@bpm", 600, "@div", 40, "@dur", 0.f));

            t.sendBang(); // on
            REQUIRE_N(t, 1, 1, 1);
            REQUIRE_ON(t, 20, 0_idx);

            t.schedTicks(1); // off
            REQUIRE_N(t, 2, 1, 1);
            REQUIRE_OFF(t, 20, 0_idx);

            t.schedTicks(19); // on
            REQUIRE_N(t, 3, 2, 2);
            REQUIRE_ON(t, 10, 1_idx);

            t.schedTicks(1); // off
            REQUIRE_N(t, 4, 2, 2);
            REQUIRE_OFF(t, 10, 1_idx);

            t.schedTicks(9); // done
            REQUIRE_N(t, 4, 2, 3);
            REQUIRE_OFF(t, 10, done);
        }

        SECTION("@dur=1")
        {
            TExt t("seq.t", LA(2, 1, "@bpm", 600, "@div", 40, "@dur", 1));

            t.sendBang(); // on
            REQUIRE_N(t, 1, 1, 1);
            REQUIRE_ON(t, 20, 0_idx);

            t.schedTicks(19); // none
            REQUIRE_N(t, 1, 1, 1);

            t.schedTicks(1); // off-on
            REQUIRE_N(t, 3, 2, 2);

            REQUIRE(t.messagesAt(0)[0] == M(1));
            REQUIRE(t.messagesAt(0)[1] == M(0.));
            REQUIRE(t.messagesAt(0)[2] == M(1));
            REQUIRE_ON(t, 10, 1_idx);

            t.schedTicks(9); // none
            REQUIRE_N(t, 3, 2, 2);

            t.schedTicks(1); // off
            REQUIRE_N(t, 4, 2, 3);
            REQUIRE(t.messagesAt(0)[0] == M(1));
            REQUIRE(t.messagesAt(0)[1] == M(0.));
            REQUIRE(t.messagesAt(0)[2] == M(1));
            REQUIRE(t.messagesAt(0)[3] == M(0.));

            REQUIRE_OFF(t, 10, done);
        }
    }

    SECTION("stop")
    {
        SECTION("inactive")
        {
            TExt t("seq.t", LA(2, 1, "@bpm", 600, "@div", 40));

            REQUIRE_N(t, 0, 0, 0);

            t.sendMessage("stop");
            REQUIRE_N(t, 0, 0, 0);

            t.schedTicks(40);
            REQUIRE_N(t, 0, 0, 0);
        }

        SECTION("while on")
        {
            TExt t("seq.t", LA(1, 2, "@bpm", 600, "@div", 40, "@dur", 0.75));

            t.sendBang();
            REQUIRE_N(t, 1, 1, 1);
            REQUIRE_ON(t, 10, 0_idx);

            t.schedTicks(5); // active
            t.sendMessage("stop");
            REQUIRE_N(t, 2, 1, 1);
            REQUIRE_OFF(t, 10, 0_idx);

            // check no changes
            t.schedTicks(10);
            REQUIRE_N(t, 2, 1, 1);
        }

        SECTION("after off")
        {
            TExt t("seq.t", LA(1, 2, "@bpm", 600, "@div", 40, "@dur", 0.75));

            t.sendBang();
            REQUIRE_N(t, 1, 1, 1);
            REQUIRE_ON(t, 10, 0_idx);

            t.schedTicks(9); // active
            t.sendMessage("stop");
            REQUIRE_N(t, 2, 1, 1);
            REQUIRE_OFF(t, 10, 0_idx);

            // check no changes
            t.schedTicks(10);
            REQUIRE_N(t, 2, 1, 1);
        }
    }

    SECTION("reset")
    {
        SECTION("inactive")
        {
            TExt t("seq.t", LA(1, 2, "@bpm", 600, "@div", 40, "@dur", 0.75));

            t.sendMessage("reset");
            REQUIRE_N(t, 0, 0, 0);
        }

        SECTION("while on")
        {
            TExt t("seq.t", LA(1, 2, "@bpm", 600, "@div", 40, "@dur", 0.75));

            t.sendBang();
            REQUIRE_N(t, 1, 1, 1);
            REQUIRE_ON(t, 10, 0_idx);

            t.schedTicks(5);
            t.sendMessage("reset");
            REQUIRE_N(t, 2, 1, 1);
            REQUIRE_OFF(t, 10, 0_idx);

            t.schedTicks(1); // again
            REQUIRE_N(t, 3, 2, 2);
            REQUIRE_ON(t, 10, 0_idx);

            t.schedTicks(8);
            t.sendMessage("reset");
            REQUIRE_N(t, 4, 2, 2);
            REQUIRE_OFF(t, 10, 0_idx);

            t.schedTicks(1);
            REQUIRE_N(t, 5, 3, 3);
            REQUIRE_ON(t, 10, 0_idx);
        }
    }
}
