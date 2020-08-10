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
#include "list_xat.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListXAt, list, xat)

using TExt = TestExtListXAt;
using TObj = TestListXAt;

TEST_CASE("list.^at", "[externals]")
{
    pd_test_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            TObj t("list.^at");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE(t.property("@default") != 0);
            REQUIRE(t.property("@default")->get()[0].isNone());
            REQUIRE_THAT(t, hasProperty(&t, "@method", "rel"));
        }

        SECTION("properties")
        {
            TObj t("list.^at", LA("@default", 2, "@fold"));
            REQUIRE(t.property("@default") != 0);
            REQUIRE(t.property("@default")->get() == Atom(2));
            REQUIRE_THAT(t, hasProperty(&t, "@method", "fold"));
        }

        SECTION("method")
        {
            SECTION("rel")
            {
                TObj t("list.^at", LA("@method", "rel"));
                REQUIRE_THAT(t, hasProperty(&t, "@method", "rel"));
            }

            SECTION("@rel")
            {
                TObj t("list.^at", LA("@rel"));
                REQUIRE_THAT(t, hasProperty(&t, "@method", "rel"));
            }

            SECTION("clip")
            {
                TObj t("list.^at", LA("@method", "clip"));
                REQUIRE_THAT(t, hasProperty(&t, "@method", "clip"));
            }

            SECTION("@clip")
            {
                TObj t("list.^at", LA("@clip"));
                REQUIRE_THAT(t, hasProperty(&t, "@method", "clip"));
            }

            SECTION("fold")
            {
                TObj t("list.^at", LA("@method", "fold"));
                REQUIRE_THAT(t, hasProperty(&t, "@method", "fold"));
            }

            SECTION("@fold")
            {
                TObj t("list.^at", LA("@fold"));
                REQUIRE_THAT(t, hasProperty(&t, "@method", "fold"));
            }

            SECTION("wrap")
            {
                TObj t("list.^at", LA("@method", "wrap"));
                REQUIRE_THAT(t, hasProperty(&t, "@method", "wrap"));
            }

            SECTION("@wrap")
            {
                TObj t("list.^at", LA("@wrap"));
                REQUIRE_THAT(t, hasProperty(&t, "@method", "wrap"));
            }
        }
    }

    SECTION("do")
    {
        SECTION("empty")
        {
            TObj t("list.^at");

            WHEN_SEND_FLOAT_TO(0, t, 0);
            REQUIRE_THAT(t, !hasOutput(&t));
            WHEN_SEND_FLOAT_TO(1, t, 0);
            REQUIRE_THAT(t, !hasOutput(&t));
            WHEN_SEND_FLOAT_TO(-1, t, 0);
            REQUIRE_THAT(t, !hasOutput(&t));
        }

        SECTION("rel")
        {
            TObj t("list.^at", LA(1, 2, 3));

            WHEN_SEND_FLOAT_TO(0, t, 0);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, 2);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, 3);
            REQUIRE_THAT(t, !hasOutput(&t));
            WHEN_SEND_FLOAT_TO(0, t, -1);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, -2);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, -3);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, -4);
            REQUIRE_THAT(t, !hasOutput(&t));
        }

        SECTION("rel @default")
        {
            TObj t("list.^at", LA(1, 2, 3, "@default", "???"));
            REQUIRE_PROPERTY(t, @method, "rel");
            REQUIRE(t.property("@default") != 0);
            REQUIRE(t.property("@default")->get() == LA("???"));

            WHEN_SEND_FLOAT_TO(0, t, 0);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, 2);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, 3);
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "???");
            WHEN_SEND_FLOAT_TO(0, t, -1);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, -2);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, -3);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, -4);
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "???");
        }

        SECTION("clip")
        {
            TObj t("list.^at", LA(1, 2, 3, "@clip"));

            WHEN_SEND_FLOAT_TO(0, t, 0);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, 2);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, 3);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, 4);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, 4000);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, -1);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, -2);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, -3);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, -4);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, -5);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, -6000);
            REQUIRE_THAT(t, outputFloat(&t, 1));
        }

        SECTION("wrap")
        {
            TObj t("list.^at", LA(1, 2, 3, "@wrap"));

            WHEN_SEND_FLOAT_TO(0, t, 0);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, 2);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, 3);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, 4);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, 5);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, 6);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, 7);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, -1);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, -2);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, -3);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, -4);
            REQUIRE_THAT(t, outputFloat(&t, 3));
        }

        SECTION("fold")
        {
            TObj t("list.^at", LA(1, 2, 3, "@fold"));

            WHEN_SEND_FLOAT_TO(0, t, 0);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, 2);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, 3);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, 4);
            REQUIRE_THAT(t, outputFloat(&t, 1));
            WHEN_SEND_FLOAT_TO(0, t, 5);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, 6);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, 7);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, -1);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, -2);
            REQUIRE_THAT(t, outputFloat(&t, 3));
            WHEN_SEND_FLOAT_TO(0, t, -3);
            REQUIRE_THAT(t, outputFloat(&t, 2));
            WHEN_SEND_FLOAT_TO(0, t, -4);
            REQUIRE_THAT(t, outputFloat(&t, 1));
        }
    }

    SECTION("list")
    {
        SECTION("rel")
        {
            TObj t("list.^at", LA(1, 2, 3, "@rel"));

            WHEN_SEND_LIST_TO(0, t, L());
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            WHEN_SEND_LIST_TO(0, t, LF(0, -1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 3, 3));

            WHEN_SEND_LIST_TO(0, t, LF(0, 100, -100));
            REQUIRE_THAT(t, outputFloat(&t, 1));
        }

        SECTION("rel + default")
        {
            TObj t("list.^at", LA(1, 2, 3, "@default", "NONE"));

            WHEN_SEND_LIST_TO(0, t, LF(0, 100, -100));
            REQUIRE_LIST_AT_OUTLET(0, t, LA(1, "NONE", "NONE"));
        }
    }

    SECTION("mlist")
    {
        TObj t("list.^at");

        WHEN_SEND_DATA_TO(1, t, MLD(1, 2, 3));
        REQUIRE_THAT(t, !hasOutput(&t));

        WHEN_SEND_FLOAT_TO(0, t, 4);
        REQUIRE_THAT(t, !hasOutput(&t));
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_THAT(t, !hasOutput(&t));
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_THAT(t, outputFloat(&t, 3));
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_THAT(t, outputFloat(&t, 2));
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_THAT(t, outputFloat(&t, 1));
    }

    SECTION("external")
    {
        TestExtListXAt t("list.^at", LF(-1));
    }
}
