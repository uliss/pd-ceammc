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
#include "ceammc_data.h"
#include "list_convolve.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListConvolve, list, convolve)

using MA = MListAtom;

TEST_CASE("list.convolve", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("empty")
        {
            TObj t("list.convolve");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @a, L());
            REQUIRE_PROPERTY(t, @b, L());
            REQUIRE_PROPERTY(t, @mode, "full");
            REQUIRE_PROPERTY(t, @full, 1);
            REQUIRE_PROPERTY(t, @valid, 0.);
            REQUIRE_PROPERTY(t, @same, 0.);
        }

        SECTION("args")
        {
            TExt t("list.convolve", LA(1, "b", 2, 3));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @a, L());
            REQUIRE_PROPERTY(t, @b, LF(1, 0, 2, 3));
        }

        SECTION("props")
        {
            TExt t("list.convolve", LA("@a", 1, "@b", 2, 3));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @a, LF(1));
            REQUIRE_PROPERTY(t, @b, LF(2, 3));
        }
    }

    SECTION("2nd inlet")
    {
        TExt t("list.convolve");

        REQUIRE_PROPERTY(t, @a, L());
        REQUIRE_PROPERTY(t, @b, L());

        t.sendListTo(LF(1, 2, 3), 1);
        REQUIRE_PROPERTY(t, @a, L());
        REQUIRE_PROPERTY(t, @b, LF(1, 2, 3));
    }

    SECTION("do")
    {
        TExt t("list.convolve", LF(1, 2, 3));

#define REQUIRE_CONV(t, a, b, c)         \
    {                                    \
        t->setProperty("@b", b);         \
        t << a;                          \
        REQUIRE(t.hasOutputAt(0));       \
        REQUIRE(t.outputListAt(0) == c); \
    }

        REQUIRE_CONV(t, LF(0, 0, 0), LF(1, 2, 3), LF(0, 0, 0, 0, 0));
        REQUIRE_CONV(t, LF(1, 2, 3), LF(0, 0, 0), LF(0, 0, 0, 0, 0));

        // commutative
        REQUIRE_CONV(t, LF(1, 1, 1), LF(2, 2, 2), LF(2, 4, 6, 4, 2));
        REQUIRE_CONV(t, LF(2, 2, 2), LF(1, 1, 1), LF(2, 4, 6, 4, 2));

        // k*g(n)
        REQUIRE_CONV(t, LF(0.5, 0.5, 0.5), LF(2, 2, 2), LF(1, 2, 3, 2, 1));
        REQUIRE_CONV(t, LF(0.5, 0.5, 0.5), LF(0, 2, 2, 2), LF(0, 1, 2, 3, 2, 1));
        REQUIRE_CONV(t, LF(0.5, 0.5, 0.5), LF(0, 0, 2, 2, 2), LF(0, 0, 1, 2, 3, 2, 1));

        // assoc
        const auto a0 = LF(1, 2, 3);
        const auto b0 = LF(4, 5, 6);
        const auto c0 = LF(7, 8, 9);
        const auto d0 = LF(28, 123, 336, 530, 594, 387, 162);
        REQUIRE_CONV(t, a0, b0, LF(4, 13, 28, 27, 18));
        REQUIRE_CONV(t, LF(4, 13, 28, 27, 18), c0, d0);
        REQUIRE_CONV(t, b0, c0, LF(28, 67, 118, 93, 54));
        REQUIRE_CONV(t, a0, LF(28, 67, 118, 93, 54), d0);

        // distrib
        const auto a1 = LF(1, 2, 3);
        const auto b1 = LF(4, 5, 6);
        const auto c1 = LF(6, 5, 4);
        const auto r1 = LF(7, 8, 9);

        REQUIRE_CONV(t, a1, b1, LF(4, 13, 28, 27, 18));
        REQUIRE_CONV(t, a1, c1, LF(6, 17, 32, 23, 12));
        REQUIRE_CONV(t, a1, LF(10, 10, 10), LF(10, 30, 60, 50, 30));

        // invalid
        t->setProperty("@a", L());
        t->setProperty("@b", L());

        t << L();
        REQUIRE(!t.hasOutputAt(0));

        t << LF(1, 3);
        REQUIRE(!t.hasOutputAt(0));

        t.sendListTo(LF(3), 1);
        t << 2;
        REQUIRE(t.outputListAt(0) == LF(6));
    }
}
