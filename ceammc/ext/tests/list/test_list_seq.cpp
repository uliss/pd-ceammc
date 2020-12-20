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
#include "list_seq.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListSeq, list, seq)

using TExt = TestExtListSeq;
using TObj = TestListSeq;

using TObj = TestListSeq;
using TExt = TestExtListSeq;

TEST_CASE("list.seq", "[externals]")
{
    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            TObj t("list.seq", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_THAT(t, hasProperty(&t, "@from", 0.f));
            REQUIRE_THAT(t, hasProperty(&t, "@to", 1.f));
            REQUIRE_THAT(t, hasProperty(&t, "@step", 1.f));
            REQUIRE_THAT(t, hasProperty(&t, "@closed", 0.0f));
        }

        SECTION("properties")
        {
            TObj t("list.seq", LA("@from", -10));
            REQUIRE_THAT(t, hasProperty(&t, "@from", -10.f));
            REQUIRE_THAT(t, hasProperty(&t, "@to", 1.f));
            REQUIRE_THAT(t, hasProperty(&t, "@step", 1.f));
            REQUIRE_THAT(t, hasProperty(&t, "@closed", 0.0f));

            {
                TObj t("list.seq", LA("@from", -10, "@to", -2));
                REQUIRE_THAT(t, hasProperty(&t, "@from", -10.f));
                REQUIRE_THAT(t, hasProperty(&t, "@to", -2.f));
                REQUIRE_THAT(t, hasProperty(&t, "@step", 1.f));
            }

            {
                TObj t("list.seq", LA("@from", -10, "@to", -2, "@step", -1));
                REQUIRE_THAT(t, hasProperty(&t, "@from", -10.f));
                REQUIRE_THAT(t, hasProperty(&t, "@to", -2.f));
                REQUIRE_THAT(t, hasProperty(&t, "@step", -1.f));
            }

            {
                TObj t("list.seq", LA("@closed"));
                REQUIRE_THAT(t, hasProperty(&t, "@from", 0.f));
                REQUIRE_THAT(t, hasProperty(&t, "@to", 1.f));
                REQUIRE_THAT(t, hasProperty(&t, "@closed", 1.f));
            }

            {
                TObj t("list.seq", LA("@to", 10, "@closed"));
                REQUIRE_THAT(t, hasProperty(&t, "@from", 0.f));
                REQUIRE_THAT(t, hasProperty(&t, "@to", 10.f));
                REQUIRE_THAT(t, hasProperty(&t, "@closed", 1.f));
            }
        }

        SECTION("positional arguments")
        {
            {
                TObj t("list.seq", LF(100));
                REQUIRE_THAT(t, hasProperty(&t, "@from", 0.f));
                REQUIRE_THAT(t, hasProperty(&t, "@to", 100.f));
                REQUIRE_THAT(t, hasProperty(&t, "@step", 1.f));
                REQUIRE_THAT(t, hasProperty(&t, "@closed", 0.0f));
            }

            {
                TObj t("list.seq", LF(-100));
                REQUIRE_THAT(t, hasProperty(&t, "@from", 0.f));
                REQUIRE_THAT(t, hasProperty(&t, "@to", -100.f));
                REQUIRE_THAT(t, hasProperty(&t, "@step", 1.f));
            }

            {
                TObj t("list.seq", LF(1, 10));
                REQUIRE_THAT(t, hasProperty(&t, "@from", 1.f));
                REQUIRE_THAT(t, hasProperty(&t, "@to", 10.f));
                REQUIRE_THAT(t, hasProperty(&t, "@step", 1.f));
            }

            {
                TObj t("list.seq", LF(10, 1));
                REQUIRE_THAT(t, hasProperty(&t, "@from", 10.f));
                REQUIRE_THAT(t, hasProperty(&t, "@to", 1.f));
                REQUIRE_THAT(t, hasProperty(&t, "@step", 1.f));
            }

            {
                TObj t("list.seq", LF(2, 5, 3));
                REQUIRE_THAT(t, hasProperty(&t, "@from", 2.f));
                REQUIRE_THAT(t, hasProperty(&t, "@to", 5.f));
                REQUIRE_THAT(t, hasProperty(&t, "@step", 3.f));
            }
        }
    }

    SECTION("bang")
    {
        {
            TObj t("list.seq", LF(5));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, 1, 2, 3, 4));
        }

        {
            TObj t("list.seq", LF(-5));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, -1, -2, -3, -4));
        }

        {
            TObj t("list.seq", LF(2, 8));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, LA(2, 3, 4, 5, 6, 7));
        }

        {
            TObj t("list.seq", LF(1, 3, 0.5f));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1.0f, 1.5f, 2.f, 2.5f));
        }

        {
            TObj t("list.seq", LF(1, 3, -1));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2));
        }
    }

    SECTION("list")
    {
        TObj t("list.seq", L());
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_THAT(t, outputFloat(&t, 0));

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
        TObj t("list.seq", L());

        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        WHEN_SEND_FLOAT_TO(0, t, 4);
        REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, 1, 2, 3));

        WHEN_SEND_FLOAT_TO(0, t, -4);
        REQUIRE_LIST_AT_OUTLET(0, t, LA(0.f, -1, -2, -3));
    }

    SECTION("@closed")
    {
        TObj t("list.seq", LA("@closed"));

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
