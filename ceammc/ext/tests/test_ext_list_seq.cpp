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
#include "../list/list_seq.h"
#include "catch.hpp"
#include "test_base.h"

#include <stdio.h>

typedef TestExternal<ListSeq> ListSeqTest;

TEST_CASE("list.seq", "[externals]")
{
    obj_init();
    test::pdPrintToStdError();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            ListSeqTest t("list.seq", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @from, 0.f);
            REQUIRE_PROPERTY(t, @to, 1.f);
            REQUIRE_PROPERTY(t, @step, 1.f);
            REQUIRE_PROPERTY(t, @closed, 0.0f);
        }

        SECTION("properties")
        {
            ListSeqTest t("list.seq", LA("@from", -10));
            REQUIRE_PROPERTY(t, @from, -10.f);
            REQUIRE_PROPERTY(t, @to, 1.f);
            REQUIRE_PROPERTY(t, @step, 1.f);
            REQUIRE_PROPERTY(t, @closed, 0.0f);

            {
                ListSeqTest t("list.seq", LA("@from", -10, "@to", -2));
                REQUIRE_PROPERTY(t, @from, -10.f);
                REQUIRE_PROPERTY(t, @to, -2.f);
                REQUIRE_PROPERTY(t, @step, 1.f);
            }

            {
                ListSeqTest t("list.seq", LA("@from", -10, "@to", -2, "@step", -1));
                REQUIRE_PROPERTY(t, @from, -10.f);
                REQUIRE_PROPERTY(t, @to, -2.f);
                REQUIRE_PROPERTY(t, @step, -1.f);
            }

            {
                ListSeqTest t("list.seq", LA("@closed"));
                REQUIRE_PROPERTY(t, @from, 0.f);
                REQUIRE_PROPERTY(t, @to, 1.f);
                REQUIRE_PROPERTY(t, @closed, 1.f);
            }

            {
                ListSeqTest t("list.seq", LA("@to", 10, "@closed"));
                REQUIRE_PROPERTY(t, @from, 0.f);
                REQUIRE_PROPERTY(t, @to, 10.f);
                REQUIRE_PROPERTY(t, @closed, 1.f);
            }
        }

        SECTION("positional arguments")
        {
            {
                ListSeqTest t("list.seq", LF(100));
                REQUIRE_PROPERTY(t, @from, 0.f);
                REQUIRE_PROPERTY(t, @to, 100.f);
                REQUIRE_PROPERTY(t, @step, 1.f);
                REQUIRE_PROPERTY(t, @closed, 0.0f);
            }

            {
                ListSeqTest t("list.seq", LF(-100));
                REQUIRE_PROPERTY(t, @from, 0.f);
                REQUIRE_PROPERTY(t, @to, -100.f);
                REQUIRE_PROPERTY(t, @step, 1.f);
            }

            {
                ListSeqTest t("list.seq", LF(1, 10));
                REQUIRE_PROPERTY(t, @from, 1.f);
                REQUIRE_PROPERTY(t, @to, 10.f);
                REQUIRE_PROPERTY(t, @step, 1.f);
            }

            {
                ListSeqTest t("list.seq", LF(10, 1));
                REQUIRE_PROPERTY(t, @from, 10.f);
                REQUIRE_PROPERTY(t, @to, 1.f);
                REQUIRE_PROPERTY(t, @step, 1.f);
            }

            {
                ListSeqTest t("list.seq", LF(2, 5, 3));
                REQUIRE_PROPERTY(t, @from, 2.f);
                REQUIRE_PROPERTY(t, @to, 5.f);
                REQUIRE_PROPERTY(t, @step, 3.f);
            }
        }
    }

    SECTION("bang")
    {
        {
            ListSeqTest t("list.seq", LF(5));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, 1, 2, 3, 4));
        }

        {
            ListSeqTest t("list.seq", LF(-5));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, -1, -2, -3, -4));
        }

        {
            ListSeqTest t("list.seq", LF(2, 8));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, LA(2, 3, 4, 5, 6, 7));
        }

        {
            ListSeqTest t("list.seq", LF(1, 3, 0.5f));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1.0f, 1.5f, 2.f, 2.5f));
        }

        {
            ListSeqTest t("list.seq", LF(1, 3, -1));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2));
        }
    }

    SECTION("list")
    {
        ListSeqTest t("list.seq", L());
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, LF(4));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, 1, 2, 3));

        WHEN_SEND_LIST_TO(0, t, LF(-4));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, -1, -2, -3));

        WHEN_SEND_LIST_TO(0, t, LF(2, 4));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(2, 3));

        WHEN_SEND_LIST_TO(0, t, LF(2, 0.f));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(2, 1));

        WHEN_SEND_LIST_TO(0, t, LA(2, 4, 0.5));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(2, 2.5, 3, 3.5));

        WHEN_SEND_LIST_TO(0, t, LA(2, 0.f, -0.5));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(2, 1.5, 1.0, 0.5));
    }

    SECTION("float")
    {
        ListSeqTest t("list.seq", L());

        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        WHEN_SEND_FLOAT_TO(0, t, 4);
        REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, 1, 2, 3));

        WHEN_SEND_FLOAT_TO(0, t, -4);
        REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, -1, -2, -3));
    }

    SECTION("@closed")
    {
        ListSeqTest t("list.seq", LA("@closed"));

        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0.f, 1));

        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, 1, 2, 3));

        WHEN_SEND_FLOAT_TO(0, t, -3);
        REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, -1, -2, -3));
    }
}
