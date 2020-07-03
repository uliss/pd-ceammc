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
#include "list_unpack.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListUnpack, list, unpack)

using TExt = TestExtListUnpack;
using TObj = TestListUnpack;

TEST_CASE("list.unpack", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("list.unpack");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("args")
        {
            TObj t("list.unpack", { 6 });
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 6);
        }

        SECTION("invalid")
        {
            SECTION("")
            {
                TObj t("list.unpack", { -1 });
                REQUIRE(t.numInlets() == 1);
                REQUIRE(t.numOutlets() == 1);
            }

            SECTION("")
            {
                TObj t("list.unpack", { 33 });
                REQUIRE(t.numInlets() == 1);
                REQUIRE(t.numOutlets() == 32);
            }
        }
    }

    SECTION("do")
    {
        TObj t("list.unpack", { 3 });

        WHEN_SEND_LIST_TO(0, t, {});
        REQUIRE_THAT(t, !hasOutput(&t));

        WHEN_SEND_LIST_TO(0, t, { 1 });
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_THAT(t, outputFloat(&t, 1));

        WHEN_SEND_LIST_TO(0, t, LA(1, "b"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "b");
        REQUIRE_THAT(t, outputFloat(&t, 1));

        WHEN_SEND_LIST_TO(0, t, LA(1, "b", 100));
        REQUIRE_FLOAT_AT_OUTLET(2, t, 100);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "b");
        REQUIRE_THAT(t, outputFloat(&t, 1));

        WHEN_SEND_LIST_TO(0, t, LA(1, "b", 100, 101));
        REQUIRE_FLOAT_AT_OUTLET(2, t, 100);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "b");
        REQUIRE_THAT(t, outputFloat(&t, 1));

        WHEN_SEND_TDATA_TO(0, t, MLD());
        REQUIRE_THAT(t, !hasOutput(&t));

        WHEN_SEND_TDATA_TO(0, t, MLD(1));
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_THAT(t, outputFloat(&t, 1));

        WHEN_SEND_TDATA_TO(0, t, MLD(1, "a"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "a");
        REQUIRE_THAT(t, outputFloat(&t, 1));

        WHEN_SEND_TDATA_TO(0, t, MLD(1, "a", 2));
        REQUIRE_FLOAT_AT_OUTLET(2, t, 2);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "a");
        REQUIRE_THAT(t, outputFloat(&t, 1));

        WHEN_SEND_TDATA_TO(0, t, MLD(1, "a", 2, 3, 4, 5, 6));
        REQUIRE_FLOAT_AT_OUTLET(2, t, 2);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "a");
        REQUIRE_THAT(t, outputFloat(&t, 1));
    }
}
