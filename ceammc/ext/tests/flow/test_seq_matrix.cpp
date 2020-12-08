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
#include "seq_matrix.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(SeqMatrix, seq, matrix)

TEST_CASE("seq.matrix", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("seq.matrix");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @t, 100);
            REQUIRE_PROPERTY(t, @dur, 0);
            REQUIRE_PROPERTY(t, @n, L());
        }

        SECTION("args")
        {
            TObj t("seq.bangs", LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @n, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @t, 100);
            REQUIRE_PROPERTY(t, @dur, 600);
        }

        SECTION("args invalid")
        {
            TObj t("seq.bangs", LA("A", 2, 3, "[a: 123]"));
            REQUIRE_PROPERTY(t, @p, LF(2, 3));
        }

        SECTION("ext")
        {
            TExt t("seq.matrix");
        }
    }

    SECTION("process")
    {
        using M = Message;
        using ML = std::vector<M>;
        const M B = M::makeBang();
        auto ri = [](int i, int n) { return M(SYM("ri"), LF(t_float(i), t_float(n))); };
        auto i = [](int i, int n) { return M(SYM("i"), LF(t_float(i), t_float(n))); };
        auto ed = [](t_float f) { return M(SYM("ed"), LF(f)); };
        const M done(SYM("done"), AtomListView());

        SECTION("empty")
        {
            TExt t("seq.b");

            t.sendBang();
            REQUIRE(t.messagesAt(0).empty());
            REQUIRE(t.messagesAt(1).empty());
        }

        SECTION("empty pattern")
        {
            TExt t("seq.b", LA("@t", 10));

            t.sendBang();
            REQUIRE_FALSE(t.hasOutput());
        }

        SECTION("single pattern")
        {
            TExt t("seq.b", LA(1, "@t", 10));

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 1), ed(10) });

            t.schedTicks(10);
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 1), ed(10), done });
        }

        SECTION("many")
        {
            TExt t("seq.b", LA(1, 2, 0.5, "@t", 10));
            REQUIRE_PROPERTY(t, @dur, 35);

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 3), ed(10) });

            t.schedTicks(10);
            REQUIRE(t.messagesAt(0) == ML { B, B });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 3), ed(10), i(1, 3), ed(20) });

            t.schedTicks(20);
            REQUIRE(t.messagesAt(0) == ML { B, B, B });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 3), ed(10), i(1, 3), ed(20), i(2, 3), ed(5) });

            t.schedTicks(5);
            REQUIRE(t.messagesAt(0) == ML { B, B, B });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 3), ed(10), i(1, 3), ed(20), i(2, 3), ed(5), done });

            t.schedTicks(5);
            REQUIRE(t.messagesAt(0) == ML { B, B, B });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 3), ed(10), i(1, 3), ed(20), i(2, 3), ed(5), done });
        }

        SECTION("repeats")
        {
            TExt t("seq.b", LA(2, 1, "@t", "10ms", "@r", 2));

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 2), ed(20) });

            t.schedTicks(20);
            REQUIRE(t.messagesAt(0) == ML { B, B });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 2), ed(20), i(1, 2), ed(10) });

            t.schedTicks(10);
            REQUIRE(t.messagesAt(0) == ML { B, B, B });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 2), ed(20), i(1, 2), ed(10), ri(1, 2), i(0, 2), ed(20) });

            t.schedTicks(20);
            REQUIRE(t.messagesAt(0) == ML { B, B, B, B });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 2), ed(20), i(1, 2), ed(10), ri(1, 2), i(0, 2), ed(20), i(1, 2), ed(10) });

            t.schedTicks(10);
            REQUIRE(t.messagesAt(0) == ML { B, B, B, B });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 2), ed(20), i(1, 2), ed(10), ri(1, 2), i(0, 2), ed(20), i(1, 2), ed(10), done });
        }

        SECTION("manual")
        {
            TExt t("seq.b", LA(2, 1, "@t", "10ms", "@r", 2));
            t.call("tick");
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 2), ed(20) });

            t.call("tick");
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { i(1, 2), ed(10) });

            t.call("tick");
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { ri(1, 2), i(0, 2), ed(20) });

            t.call("tick");
            REQUIRE(t.messagesAt(0) == ML { B });
            REQUIRE(t.messagesAt(1) == ML { i(1, 2), ed(10) });

            t.call("tick");
            REQUIRE(t.messagesAt(0) == ML {});
            REQUIRE(t.messagesAt(1) == ML { done });
        }

        SECTION("control")
        {
            SECTION("bang")
            {
                TExt t("seq.b", LA(1, 2, 3, "@t", 1, "@r", 2));
                t.sendBang();
                t.schedTicks(2);
                REQUIRE(t.messagesAt(0) == ML { B, B });
                REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), ed(1), i(1, 3), ed(2) });

                // should reset
                t.sendBang();
                t.schedTicks(2);
                REQUIRE(t.messagesAt(0) == ML { B, B, B, B });
                REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), ed(1), i(1, 3), ed(2), ri(0, 2), i(0, 3), ed(1), i(1, 3), ed(2) });

                t.bang();
                t.schedTicks(8);
                REQUIRE(t.messagesAt(0) == ML { B, B, B, B, B });
                REQUIRE(t.messagesAt(1) == ML { //
                            ri(0, 2), i(0, 3), ed(1), i(1, 3), ed(2), i(2, 3), ed(3), //
                            ri(1, 2), i(0, 3), ed(1), i(1, 3), ed(2) });

                t.sendBang();
                REQUIRE(t.messagesAt(0) == ML { B, B, B, B, B, B });
                REQUIRE(t.messagesAt(1) == ML { //
                            ri(0, 2), i(0, 3), ed(1), i(1, 3), ed(2), i(2, 3), ed(3), //
                            ri(1, 2), i(0, 3), ed(1), i(1, 3), ed(2), //
                            ri(0, 2), i(0, 3), ed(1) });
            }

            SECTION("start")
            {
                TExt t("seq.b", LA(1, 2, 3, "@t", 1, "@r", 2));
                t.call("start");
                t.schedTicks(2);
                REQUIRE(t.messagesAt(0) == ML { B, B });
                REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), ed(1), i(1, 3), ed(2) });

                t->stop();
                t.schedTicks(10); // no change after stop
                REQUIRE(t.messagesAt(0) == ML { B, B });
                REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), ed(1), i(1, 3), ed(2) });

                t.sendMessage("start");
                REQUIRE(t.messagesAt(0) == ML { B, B, B });
                REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 3), ed(1), i(1, 3), ed(2), i(2, 3), ed(3) });

                t->stop();
                t.schedTicks(10); // no change after stop
                t.sendMessage("start");
                REQUIRE(t.messagesAt(0) == ML { B, B, B, B });
                REQUIRE(t.messagesAt(1) == ML { //
                            ri(0, 2), i(0, 3), ed(1), i(1, 3), ed(2), i(2, 3), ed(3), //
                            ri(1, 2), i(0, 3), ed(1) });

                t->stop();
                t.schedTicks(10); // no change after stop
                t.sendMessage("start");
                REQUIRE(t.messagesAt(0) == ML { B, B, B, B, B });
                REQUIRE(t.messagesAt(1) == ML { //
                            ri(0, 2), i(0, 3), ed(1), i(1, 3), ed(2), i(2, 3), ed(3), //
                            ri(1, 2), i(0, 3), ed(1), i(1, 3), ed(2) });
            }
        }
    }
}
