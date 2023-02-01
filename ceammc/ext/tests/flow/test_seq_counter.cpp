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
#include "seq_counter.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(SeqCounter, seq, counter)

TEST_CASE("seq.counter", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError(true);

    SECTION("construct")
    {
        SECTION("default")
        {
            TExt t("seq.counter");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @from, 0);
            REQUIRE_PROPERTY(t, @to, 0);
            REQUIRE_PROPERTY(t, @r, "inf");
            REQUIRE_PROPERTY(t, @i, 0.);
            REQUIRE_PROPERTY(t, @ri, 0.);
            REQUIRE_PROPERTY(t, @value, 0.);
        }

        SECTION("arg0")
        {
            TExt t("seq.counter", LF(-10));
            REQUIRE_PROPERTY(t, @from, -10);
            REQUIRE_PROPERTY(t, @to, 0);
            REQUIRE_PROPERTY(t, @r, "inf");
        }

        SECTION("arg1")
        {
            TExt t("seq.counter", LF(-10, 20));
            REQUIRE_PROPERTY(t, @from, -10);
            REQUIRE_PROPERTY(t, @to, 20);
            REQUIRE_PROPERTY(t, @r, "inf");
        }

        SECTION("arg2")
        {
            TExt t("seq.counter", LF(-10, 20, 3));
            REQUIRE_PROPERTY(t, @from, -10);
            REQUIRE_PROPERTY(t, @to, 20);
            REQUIRE_PROPERTY(t, @r, 3);
        }
    }

    SECTION("do")
    {
        using M = Message;
        using ML = std::vector<M>;
        //        const auto bng = M::makeBang();
        const auto done = M(SYM("done"), L());
        auto i = [](int i) { return M(t_float(i)); };

        SECTION("empty infinine")
        {
            TExt t("seq.counter");
            t.bang();
            REQUIRE(t.messagesAt(0) == ML { M(0.) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(0.), M(0.) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(0.), M(0.), M(0.) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), i(2) });
        }

        SECTION("single @r 0")
        {
            TExt t("seq.counter", LA("@from", 10, "@to", 10, "@r", 0.));

            t.bang();
            REQUIRE_FALSE(t.hasOutput());
        }

        SECTION("single @r 1")
        {
            TExt t("seq.counter", LA("@from", 10, "@to", 10, "@r", 1));

            t.bang();
            REQUIRE(t.messagesAt(0) == ML { M(10) });
            REQUIRE(t.messagesAt(1) == ML { i(0), done });
        }

        SECTION("single @r 2")
        {
            TExt t("seq.counter", LA("@from", 10, "@to", 10, "@r", 2));

            t.bang();
            REQUIRE(t.messagesAt(0) == ML { M(10) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(10) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), done });
        }

        SECTION("single finite")
        {
            TExt t("seq.counter", LA("@from", 10, "@to", 10, "@r", 3));

            t.bang();
            REQUIRE(t.messagesAt(0) == ML { M(10) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });
            REQUIRE_PROPERTY(t, @value, 10);

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(10) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });
            REQUIRE_PROPERTY(t, @value, 10);

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(10), M(10) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), i(2), done });
        }

        SECTION("positive @r 0")
        {
            TExt t("seq.counter", LA("@from", 10, "@to", 12, "@r", 0.));

            t.bang();
            REQUIRE_FALSE(t.hasOutput());
        }

        SECTION("positive @r 1")
        {
            TExt t("seq.counter", LA("@from", 10, "@to", 12, "@r", 1));
            REQUIRE_PROPERTY(t, @value, 10);

            t.bang();
            REQUIRE(t.messagesAt(0) == ML { M(10) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });
            REQUIRE_PROPERTY(t, @value, 11);

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(11) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });
            REQUIRE_PROPERTY(t, @value, 12);

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(11), M(12) });
            REQUIRE(t.messagesAt(1) == ML { i(0), done });
            REQUIRE_PROPERTY(t, @value, 12);
        }

        SECTION("positive @r 2")
        {
            TExt t("seq.counter", LA("@from", 10, "@to", 12, "@r", 2));

            t.bang();
            REQUIRE(t.messagesAt(0) == ML { M(10) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(11) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(11), M(12) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(11), M(12), M(10) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(11), M(12), M(10), M(11) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(11), M(12), M(10), M(11), M(12) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), done });
        }

        SECTION("positive inf")
        {
            TExt t("seq.counter", LA("@from", 10, "@to", 11));

            t.bang();
            REQUIRE(t.messagesAt(0) == ML { M(10) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(11) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(11), M(10) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(11), M(10), M(11) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(11), M(10), M(11), M(10) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), i(2) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10), M(11), M(10), M(11), M(10), M(11) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), i(2) });

            t.sendBangTo(1);
            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(10) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });
        }

        SECTION("negative @r 2")
        {
            TExt t("seq.counter", LA("@from", 1, "@to", -1, "@r", 2));
            REQUIRE_PROPERTY(t, @value, 1);

            t.bang();
            REQUIRE(t.messagesAt(0) == ML { M(1) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });
            REQUIRE_PROPERTY(t, @value, 0.);

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(0.) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });
            REQUIRE_PROPERTY(t, @value, -1);

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(0.), M(-1) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });
            REQUIRE_PROPERTY(t, @value, 1);

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(0.), M(-1), M(1) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(0.), M(-1), M(1), M(0.) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(0.), M(-1), M(1), M(0.), M(-1) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), done });

            t.clearAll();
            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML {});
            t.sendBang();
            REQUIRE(t.messagesAt(1) == ML {});
            t.sendBang();
            REQUIRE_FALSE(t.hasOutput());

            t.sendBangTo(1);
            REQUIRE_FALSE(t.hasOutput());

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(0.) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });
        }

        SECTION("@fold const")
        {
            TExt t("seq.counter", LA("@from", 1, "@to", 1, "@r", 2, "@fold"));

            t.bang();
            REQUIRE(t.messagesAt(0) == ML { M(1) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(1) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), done });
        }

        SECTION("@fold positive")
        {
            TExt t("seq.counter", LA("@from", 1, "@to", 2, "@r", 2, "@fold"));
            REQUIRE_PROPERTY(t, @mode, "fold");
            REQUIRE_PROPERTY(t, @value, 1);

            t.bang();
            REQUIRE(t.messagesAt(0) == ML { M(1) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });
            REQUIRE_PROPERTY(t, @value, 2);

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });
            REQUIRE_PROPERTY(t, @value, 1);

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(2), M(1) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });
            REQUIRE_PROPERTY(t, @value, 2);

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(2), M(1), M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), done });
            REQUIRE_PROPERTY(t, @value, 2);
        }

        SECTION("@fold positive")
        {
            TExt t("seq.counter", LA("@from", 1, "@to", 3, "@r", 2, "@fold"));
            REQUIRE_PROPERTY(t, @mode, "fold");

            t.bang();
            REQUIRE(t.messagesAt(0) == ML { M(1) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(2), M(3) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(2), M(3), M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(2), M(3), M(2), M(1) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(2), M(3), M(2), M(1), M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(2), M(3), M(2), M(1), M(2), M(3) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(2), M(3), M(2), M(1), M(2), M(3), M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), done });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(1), M(2), M(3), M(2), M(1), M(2), M(3), M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), done });
        }

        SECTION("@fold negative")
        {
            TExt t("seq.counter", LA("@from", 2, "@to", 1, "@r", 2, "@fold"));
            REQUIRE_PROPERTY(t, @mode, "fold");

            t.bang();
            REQUIRE(t.messagesAt(0) == ML { M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(2), M(1) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(2), M(1), M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(2), M(1), M(2), M(1) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), done });
        }

        SECTION("@fold negative")
        {
            TExt t("seq.counter", LA("@from", 3, "@to", 1, "@r", 2, "@fold"));
            REQUIRE_PROPERTY(t, @mode, "fold");

            t.bang();
            REQUIRE(t.messagesAt(0) == ML { M(3) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(2), M(3) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(2), M(3), M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(2), M(3), M(2), M(1) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1) });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(2), M(3), M(2), M(1), M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), done });

            t.sendBang();
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(2), M(3), M(2), M(1), M(2) });
            REQUIRE(t.messagesAt(1) == ML { i(0), i(1), done });
        }

        SECTION("@i set")
        {

#define CHECK_SET_IDX(t, idx, ii, val)           \
    {                                            \
        REQUIRE(t->setProperty("@i", LF(idx)));  \
        REQUIRE_PROPERTY(t, @i, ii);             \
        REQUIRE_PROPERTY(t, @value, float(val)); \
    }

            SECTION("@wrap")
            {
                TExt t("seq.counter", LA("@from", -2, "@to", 2, "@r", 2, "@wrap"));

                CHECK_SET_IDX(t, -5, 0, -2);
                CHECK_SET_IDX(t, -4, 1, -1);
                CHECK_SET_IDX(t, -3, 2, 0);
                CHECK_SET_IDX(t, -2, 3, 1);
                CHECK_SET_IDX(t, -1, 4, 2);
                CHECK_SET_IDX(t, 0, 0, -2);
                CHECK_SET_IDX(t, 1, 1, -1);
                CHECK_SET_IDX(t, 2, 2, 0);
                CHECK_SET_IDX(t, 3, 3, 1);
                CHECK_SET_IDX(t, 4, 4, 2);
                CHECK_SET_IDX(t, 5, 0, -2);
                CHECK_SET_IDX(t, 6, 1, -1);
                CHECK_SET_IDX(t, 7, 2, 0);
                CHECK_SET_IDX(t, 8, 3, 1);
                CHECK_SET_IDX(t, 9, 4, 2);
                CHECK_SET_IDX(t, 10, 0, -2);
            }

            SECTION("@fold")
            {
                TExt t("seq.counter", LA("@from", 0., "@to", 3, "@r", 2, "@fold"));

                CHECK_SET_IDX(t, -6, 0, 0);
                CHECK_SET_IDX(t, -5, 1, 1);
                CHECK_SET_IDX(t, -4, 2, 2);
                CHECK_SET_IDX(t, -3, 3, 3);
                CHECK_SET_IDX(t, -2, 4, 2);
                CHECK_SET_IDX(t, -1, 5, 1);
                CHECK_SET_IDX(t, 0, 0, 0);
                CHECK_SET_IDX(t, 1, 1, 1);
                CHECK_SET_IDX(t, 2, 2, 2);
                CHECK_SET_IDX(t, 3, 3, 3);
                CHECK_SET_IDX(t, 4, 4, 2);
                CHECK_SET_IDX(t, 5, 5, 1);
                CHECK_SET_IDX(t, 6, 0, 0);
                CHECK_SET_IDX(t, 7, 1, 1);
                CHECK_SET_IDX(t, 8, 2, 2);
                CHECK_SET_IDX(t, 9, 3, 3);
                CHECK_SET_IDX(t, 10, 4, 2);
                CHECK_SET_IDX(t, 11, 5, 1);
            }

            SECTION("init")
            {
                TExt t("seq.counter", LA("@from", 0., "@to", 3, "@r", 2, "@wrap", "@i", -1));
                REQUIRE_PROPERTY(t, @value, 3);
                REQUIRE_PROPERTY(t, @i, 3);
                REQUIRE_PROPERTY(t, @ri, 0);

                t.sendBang();
                REQUIRE_PROPERTY(t, @value, 0.);
                REQUIRE_PROPERTY(t, @i, 0);
                REQUIRE_PROPERTY(t, @ri, 1);
            }
        }
    }

    SECTION("next")
    {
        using M = Message;
        using ML = std::vector<M>;

        SECTION("simple")
        {
            TExt t("seq.counter", LA("@from", 1, "@to", 3, "@r", 2));
            REQUIRE_PROPERTY(t, @value, 1);
            REQUIRE_PROPERTY(t, @i, 0);
            REQUIRE_PROPERTY(t, @ri, 0);

            t.sendMessage("next");
            REQUIRE(t.messagesAt(0) == ML { M(2) });
            t.sendMessage("next");
            REQUIRE(t.messagesAt(0) == ML { M(2), M(3) });
            t.sendMessage("next");
            REQUIRE(t.messagesAt(0) == ML { M(2), M(3), M(1) });
            t.sendMessage("next");
            REQUIRE(t.messagesAt(0) == ML { M(2), M(3), M(1), M(2) });
            t.sendMessage("next");
            REQUIRE(t.messagesAt(0) == ML { M(2), M(3), M(1), M(2), M(3) });
            t.sendMessage("next");
            REQUIRE(t.messagesAt(0) == ML { M(2), M(3), M(1), M(2), M(3) });
        }
    }

    SECTION("prev")
    {
        using M = Message;
        using ML = std::vector<M>;
        //        const auto bng = M::makeBang();
        const auto done = M(SYM("done"), L());
        auto i = [](int i) { return M(t_float(i)); };

        SECTION("simple")
        {
            TExt t("seq.counter", LA("@from", 1, "@to", 3, "@r", 3));
            REQUIRE_PROPERTY(t, @value, 1);
            REQUIRE_PROPERTY(t, @i, 0);
            REQUIRE_PROPERTY(t, @ri, 0);

            t.sendMessage("prev");
            REQUIRE(t.messagesAt(0) == ML { M(3) });
            t.sendMessage("prev");
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2) });
            t.sendMessage("prev");
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1) });
            t.sendMessage("prev");
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(3) });
            t.sendMessage("prev");
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(3), M(2) });
            t.sendMessage("prev");
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(3), M(2), M(1) });
            t.sendMessage("prev");
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(3), M(2), M(1) });
        }

        SECTION("<->")
        {
            TExt t("seq.counter", LA("@from", 1, "@to", 3));
            REQUIRE_PROPERTY(t, @value, 1);
            REQUIRE_PROPERTY(t, @i, 0);
            REQUIRE_PROPERTY(t, @ri, 0);

            t.sendMessage("prev");
            REQUIRE(t.messagesAt(0) == ML { M(3) });
            t.sendMessage("prev");
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2) });
            t.sendMessage("prev");
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1) });
            t.sendMessage("next");
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(2) });
            t.sendMessage("prev");
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(2), M(1) });
            t.sendMessage("next");
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(2), M(1), M(2) });
            t.sendMessage("next");
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(2), M(1), M(2), M(3) });
            t.sendMessage("next");
            REQUIRE(t.messagesAt(0) == ML { M(3), M(2), M(1), M(2), M(1), M(2), M(3), M(1) });
        }
    }
}
