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
#include "test_seq_base.h"

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
            REQUIRE_PROPERTY(t, @t, 250);
            REQUIRE_PROPERTY(t, @dur, 4000);
            REQUIRE_PROPERTY(t, @n, 16);
        }

        SECTION("args")
        {
            TObj t("seq.matrix", LF(10, 200));
            REQUIRE_PROPERTY(t, @n, 10);
            REQUIRE_PROPERTY(t, @t, 200);
            REQUIRE_PROPERTY(t, @dur, 2000);
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
        auto ri = [](int i, size_t n) { return M(SYM("ri"), LF(t_float(i), t_float(n))); };
        auto i = [](int i, int n) { return M(SYM("i"), LF(t_float(i), t_float(n))); };
        const M done(SYM("done"), AtomListView());
        auto cc = [](int n) { return M(SYM("@current_col"), LF(t_float(n))); };
        auto gc = [](int n) { return M(SYM("get"), LA("col", t_float(n))); };

        SECTION("empty")
        {
            TExt t("seq.matrix", LA("@n", 0.));

            t.sendBang();
            REQUIRE(t.messagesAt(0).empty());
            REQUIRE(t.messagesAt(1).empty());
        }

        SECTION("single")
        {
            TExt t("seq.matrix", LA("@n", 1, "@t", 2, "@r", 2));

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { cc(0), gc(0) });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 1) });

            t.schedTicks(2_wd);
            REQUIRE(t.messagesAt(0) == ML { cc(0), gc(0), cc(0), gc(0) });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 1), ri(1, 2), i(0, 1) });

            t.schedTicks(2);
            REQUIRE(t.messagesAt(0) == ML { cc(0), gc(0), cc(0), gc(0) });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 2), i(0, 1), ri(1, 2), i(0, 1), done });
        }

        SECTION("number")
        {
            TExt t("seq.matrix", LA("@n", 1, "@t", 2, "@r", 1));

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { cc(0), gc(0) });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 1) });

            t.schedTicks(2_wd);
            REQUIRE(t.messagesAt(0) == ML { cc(0), gc(0) });
            REQUIRE(t.messagesAt(1) == ML { ri(0, 1), i(0, 1), done });
        }

        SECTION("dec")
        {
            TExt t("seq.matrix", LA("@n", 3, "@t", 2, "@mode", "dec"));

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { cc(2), gc(2) });
            REQUIRE(t.messagesAt(1) == ML { ri(0, size_t(-1)), i(2, 3) });

            t.schedTicks(2_wd);
            REQUIRE(t.messagesAt(0) == ML { cc(2), gc(2), cc(1), gc(1) });
            REQUIRE(t.messagesAt(1) == ML { ri(0, size_t(-1)), i(2, 3), i(1, 3) });

            t.sendMessage("rewind");
            t.schedTicks(2);
            REQUIRE(t.messagesAt(0) == ML { cc(2), gc(2), cc(1), gc(1), cc(2), gc(2) });
            REQUIRE(t.messagesAt(1) == ML { ri(0, size_t(-1)), i(2, 3), i(1, 3), ri(0, size_t(-1)), i(2, 3) });
        }
    }
}
