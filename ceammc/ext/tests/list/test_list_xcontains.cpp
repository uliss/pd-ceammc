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
#include "list_xcontains.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListXContains, list, xcontains)

TEST_CASE("list.^contains", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TestListXContains t("list.^contains");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
        }
    }

    SECTION("empty")
    {
        TestListXContains t("list.^contains");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_DATA_TO(0, t, IntData(100));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
    }

    SECTION("empty")
    {
        TestListXContains t("list.^contains", LA(1, 2, 3, "A"));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_THAT(t, outputTrue(&t));

        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_THAT(t, outputTrue(&t));

        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_THAT(t, outputTrue(&t));

        // symbol
        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_THAT(t, outputTrue(&t));

        // list
        WHEN_SEND_LIST_TO(0, t, LF(1, 10));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, LF(1));
        REQUIRE_THAT(t, outputTrue(&t));

        WHEN_SEND_LIST_TO(0, t, LF(2));
        REQUIRE_THAT(t, outputTrue(&t));

        WHEN_SEND_LIST_TO(0, t, LF(3));
        REQUIRE_THAT(t, outputTrue(&t));

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_THAT(t, outputTrue(&t));

        WHEN_SEND_LIST_TO(0, t, LF(2, 3));
        REQUIRE_THAT(t, outputTrue(&t));

        WHEN_SEND_LIST_TO(0, t, LF(1, 3));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_THAT(t, outputTrue(&t));

        // data
        WHEN_SEND_DATA_TO(0, t, IntData(100));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        IntA d0(1000);
        StrA d1("ABC");
        AtomList l0(d0, d1);

        WHEN_SEND_LIST_TO(1, t, l0);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_DATA_TO(0, t, IntData(1000));
        REQUIRE_THAT(t, outputTrue(&t));
        WHEN_SEND_DATA_TO(0, t, IntData(999));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        WHEN_SEND_DATA_TO(0, t, StrData("abc"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        WHEN_SEND_DATA_TO(0, t, StrData("ABC"));
        REQUIRE_THAT(t, outputTrue(&t));

        WHEN_SEND_DATA_TO(1, t, MLD(1, "TEST", 2));
        REQUIRE_NO_MSG(t);
        WHEN_SEND_DATA_TO(0, t, MLD(1));
        REQUIRE_THAT(t, outputTrue(&t));
        WHEN_SEND_DATA_TO(0, t, MLD("TEST"));
        REQUIRE_THAT(t, outputTrue(&t));
        WHEN_SEND_DATA_TO(0, t, MLD(1, "TEST"));
        REQUIRE_THAT(t, outputTrue(&t));
        WHEN_SEND_DATA_TO(0, t, MLD(3));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        WHEN_SEND_DATA_TO(0, t, MLD("TEST", 3));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_THAT(t, outputTrue(&t));
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_THAT(t, outputTrue(&t));
        WHEN_SEND_FLOAT_TO(0, t, 3);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        WHEN_SEND_SYMBOL_TO(0, t, "TEST");
        REQUIRE_THAT(t, outputTrue(&t));
        WHEN_SEND_SYMBOL_TO(0, t, "TEST1");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
    }

    SECTION("inlet 2")
    {
        TestListXContains t("list.^contains", LF(2));
    }
}
