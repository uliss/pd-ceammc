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
#include "list_crosscorr.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListCrosscorr, list, crosscorr)

using MA = MListAtom;

TEST_CASE("list.correlate", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("empty")
        {
            TObj t("list.correlate");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @a, L());
            REQUIRE_PROPERTY(t, @b, L());
            REQUIRE_PROPERTY(t, @mode, "valid");
            REQUIRE_PROPERTY(t, @valid, 1);
            REQUIRE_PROPERTY(t, @full, 0);
            REQUIRE_PROPERTY(t, @same, 0);
        }

        SECTION("args")
        {
            TExt t("list.correlate", LA(1, "b", 2, 3));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @a, L());
            REQUIRE_PROPERTY(t, @b, LF(1, 0, 2, 3));
        }

        SECTION("props")
        {
            TExt t("list.correlate", LA("@a", 1, "@b", 2, 3));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @a, LF(1));
            REQUIRE_PROPERTY(t, @b, LF(2, 3));
        }
    }

    SECTION("2nd inlet")
    {
        TExt t("list.correlate");

        REQUIRE_PROPERTY(t, @a, L());
        REQUIRE_PROPERTY(t, @b, L());

        t.sendListTo(LF(1, 2, 3), 1);
        REQUIRE_PROPERTY(t, @a, L());
        REQUIRE_PROPERTY(t, @b, LF(1, 2, 3));
    }

    SECTION("do")
    {
#define REQUIRE_CORR(t, a, b, c)         \
    {                                    \
        t->setProperty("@b", b);         \
        t << a;                          \
        REQUIRE(t.hasOutputAt(0));       \
        REQUIRE(t.outputListAt(0) == c); \
    }

        SECTION("valid")
        {
            TExt t("list.correlate", LA("@valid"));
            REQUIRE_PROPERTY(t, @mode, LA("valid"));

            REQUIRE_CORR(t, LF(1, 2, 3), LF(2), LF(2, 4, 6));
            REQUIRE_CORR(t, LF(1, 2, 3), LF(0.5), LF(0.5, 1, 1.5));

            REQUIRE_CORR(t, LF(1, 2, 3), LF(1, 0), LF(1, 2));
            REQUIRE_CORR(t, LF(1, 2, 3), LF(1, 0, 2), LF(7));
            REQUIRE_CORR(t, LF(1, 2, 3), LF(1, 0, 2, 5), LF(19, 7));
            REQUIRE_CORR(t, LF(1, 2, 3), LF(1, 0, 2, 5, -1), LF(9, 19, 7));
        }

        SECTION("full")
        {
            TExt t("list.correlate", LA("@full"));
            REQUIRE_PROPERTY(t, @mode, LA("full"));

            REQUIRE_CORR(t, LF(1, 2, 3), LF(2), LF(2, 4, 6));
            REQUIRE_CORR(t, LF(1, 2, 3), LF(0.5), LF(0.5, 1, 1.5));

            REQUIRE_CORR(t, LF(1, 2, 3), LF(1, 0), LF(0, 1, 2, 3));
            REQUIRE_CORR(t, LF(1, 2, 3), LF(1, 0, 2), LF(2, 4, 7, 2, 3));
            REQUIRE_CORR(t, LF(1, 2, 3), LF(1, 0, 2, 5), LF(5, 12, 19, 7, 2, 3));
            REQUIRE_CORR(t, LF(1, 2, 3), LF(1, 0, 2, 5, -1), LF(-1, 3, 9, 19, 7, 2, 3));
        }
    }
}
