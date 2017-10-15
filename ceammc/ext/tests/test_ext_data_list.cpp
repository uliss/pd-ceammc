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
#include "../data/data_list.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

#define REQUIRE_SIZE(obj, n) REQUIRE_PROPERTY(t, @size, float(n));

#define REQUIRE_LIST(obj, lst)               \
    {                                        \
        WHEN_SEND_BANG_TO(0, obj);           \
        REQUIRE_SIZE(obj, lst.size());       \
        REQUIRE_LIST_AT_OUTLET(0, obj, lst); \
    }

typedef TestExtension<DataList> DataListTest;

TEST_CASE("data.list", "[externals]")
{
    obj_init();

    SECTION("main")
    {

        DataListTest t("data.list", AtomList());
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);

        REQUIRE_PROPERTY(t, @size, 0.f);
        REQUIRE_LIST(t, AtomList());

        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(0, t, L3(1, 2, 3));

        REQUIRE_SIZE(t, 3.f);

        WHEN_SEND_LIST_TO(1, t, L4(1, 2, 3, 4));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @size, 4.f);

        WHEN_CALL(t, clear);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_SIZE(t, 0);

        WHEN_CALL_3(t, set, 1, 2, 3);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST(t, L3(1, 2, 3));

        WHEN_CALL_1(t, append, 4);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST(t, L4(1, 2, 3, 4));

        WHEN_CALL_2(t, append, 5, 6);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST(t, L6(1, 2, 3, 4, 5, 6));

        WHEN_CALL(t, pop);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST(t, L5(1, 2, 3, 4, 5));

        WHEN_CALL(t, clear);
        WHEN_CALL(t, pop);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_SIZE(t, 0);

        WHEN_CALL(t, remove);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        WHEN_CALL_1(t, remove, "a");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        WHEN_CALL_1(t, remove, 0.f);

        WHEN_CALL_1(t, append, 100);
        WHEN_CALL_1(t, append, 200);
        REQUIRE_LIST(t, L2(100, 200));

        WHEN_CALL_1(t, remove, 123);
        REQUIRE_LIST(t, L2(100, 200));

        WHEN_CALL_1(t, remove, -123);
        REQUIRE_LIST(t, L2(100, 200));

        WHEN_CALL_1(t, remove, 123.1f);
        REQUIRE_LIST(t, L2(100, 200));

        WHEN_CALL_1(t, remove, 1.f);
        REQUIRE_LIST(t, L1(100));

        WHEN_CALL_1(t, remove, 0.f);
        REQUIRE_LIST(t, AtomList());

        WHEN_CALL(t, insert);
        REQUIRE_LIST(t, AtomList());

        WHEN_CALL_1(t, insert, 0.f);
        REQUIRE_LIST(t, AtomList());

        WHEN_CALL_2(t, insert, 0.f, 100);
        REQUIRE_LIST(t, L1(100));

        WHEN_CALL_2(t, insert, 0.f, 200);
        REQUIRE_LIST(t, L2(200, 100));

        WHEN_CALL_2(t, insert, 1, 300);
        REQUIRE_LIST(t, L3(200, 300, 100));

        WHEN_CALL_2(t, insert, 3, 400);
        REQUIRE_LIST(t, L4(200, 300, 100, 400));

        WHEN_CALL_2(t, insert, 5, 500);
        REQUIRE_LIST(t, L4(200, 300, 100, 400));

        WHEN_CALL(t, flush);
        REQUIRE_LIST_AT_OUTLET(0, t, L4(200, 300, 100, 400));
        REQUIRE_SIZE(t, 0.f);
    }

    SECTION("construct args")
    {
        DataListTest t("data.list", L5(1, 2, 3, 4, 5));
        REQUIRE_LIST(t, L5(1, 2, 3, 4, 5));
    }
}
