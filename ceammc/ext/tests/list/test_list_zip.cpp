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
#include "list_zip.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListZip, list, zip)

using TExt = TestExtListZip;
using TObj = TestListZip;

TEST_CASE("list.zip", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("list.zip");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("args")
        {
            TObj t("list.zip", LF(4));
            REQUIRE(t.numInlets() == 4);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("args")
        {
            TObj t("list.zip", LF(20));
            REQUIRE(t.numInlets() == 20);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("invalid")
        {
            SECTION("1")
            {
                TObj t("list.zip", LF(1));
                REQUIRE(t.numInlets() == 2);
                REQUIRE(t.numOutlets() == 1);
            }

            SECTION("-10")
            {
                TObj t("list.zip", LF(-10));
                REQUIRE(t.numInlets() == 2);
                REQUIRE(t.numOutlets() == 1);
            }

            SECTION("21")
            {
                TObj t("list.zip", LF(20));
                REQUIRE(t.numInlets() == 20);
                REQUIRE(t.numOutlets() == 1);
            }
        }
    }

    SECTION("act")
    {
        SECTION("default")
        {
            TExt t("list.zip");
            REQUIRE_PROPERTY(t, @lists, LA(MLA(), MLA()));

            // default @min method
            t << LF(1, 2, 3);
            REQUIRE_LIST_AT_OUTLET(0, t, L());
            REQUIRE_PROPERTY(t, @lists, LA(MLA(1, 2, 3), MLA()));

            // interleave with 4,5,6
            t.sendListTo(LF(4, 5, 6), 1);
            REQUIRE_THAT(t, !hasOutput(&t));
            REQUIRE_PROPERTY(t, @lists, LA(MLA(1, 2, 3), MLA(4, 5, 6)));

            t << LF(1, 2, 3);
            REQUIRE_LIST_AT_OUTLET(0, t, LA(1, 4, 2, 5, 3, 6));

            t << LF(1, 2);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4, 2, 5));

            t << LF(1);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4));

            t << L();
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            // interleave with 4,5
            t.sendListTo(LF(4, 5), 1);
            REQUIRE_THAT(t, !hasOutput(&t));

            t << LF(1, 2, 3);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4, 2, 5));

            t << LF(1, 2);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4, 2, 5));

            t << LF(1);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4));

            t << L();
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            // interleave with 4
            t.sendListTo(LF(4), 1);
            REQUIRE_THAT(t, !hasOutput(&t));

            t << LF(1, 2, 3);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4));

            t << LF(1, 2);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4));

            t << LF(1);
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4));

            t << L();
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            // interleave with ()
            t.sendListTo(L(), 1);

            t << LF(1, 2, 3);
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            t << LF(1, 2);
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            t << LF(1);
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            t << L();
            REQUIRE_LIST_AT_OUTLET(0, t, L());
        }

        SECTION("multiple")
        {
            TObj t("list.zip", LF(3));

            // default @min method
            WHEN_SEND_LIST_TO(0, t, LF(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            // interleave with 3,4 and 5,6
            WHEN_SEND_LIST_TO(1, t, LF(3, 4));
            REQUIRE_THAT(t, !hasOutput(&t));
            WHEN_SEND_LIST_TO(2, t, LF(5, 6));
            REQUIRE_THAT(t, !hasOutput(&t));

            WHEN_SEND_LIST_TO(0, t, LF(1, 2));
            REQUIRE_LIST_AT_OUTLET(0, t, LA(1, 3, 5, 2, 4, 6));
        }
    }
}
