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
#include "data_list.h"
#include "test_base.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(DataList, data, list)

#define REQUIRE_SIZE(obj, n) REQUIRE_PROPERTY(t, @size, float(n));

#define REQUIRE_LIST(obj, lst)               \
    {                                        \
        WHEN_SEND_BANG_TO(0, obj);           \
        REQUIRE_SIZE(obj, lst.size());       \
        REQUIRE_LIST_AT_OUTLET(0, obj, lst); \
    }

#define REQUIRE_FLOAT(obj, f)               \
    {                                       \
        WHEN_SEND_BANG_TO(0, obj);          \
        REQUIRE_FLOAT_AT_OUTLET(0, obj, f); \
    }

TEST_CASE("data.list", "[externals]")
{
    pd_test_init();

    SECTION("main")
    {
        TObj t("data.list", L());
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);

        REQUIRE_PROPERTY_FLOAT(t, @size, 0);
        REQUIRE_PROPERTY_FLOAT(t, @empty, 1);
        REQUIRE_LIST(t, L());

        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3));

        REQUIRE_SIZE(t, 3.f);

        WHEN_SEND_LIST_TO(1, t, LF(1, 2, 3, 4));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @size, 4.f);

        WHEN_CALL(t, clear);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_SIZE(t, 0);

        WHEN_CALL_N(t, set, 1, 2, 3);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST(t, LF(1, 2, 3));

        WHEN_CALL_N(t, append, 4);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST(t, LF(1, 2, 3, 4));

        WHEN_CALL_N(t, append, 5, 6);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST(t, LA(1, 2, 3, 4, 5, 6));

        WHEN_CALL(t, pop);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST(t, LA(1, 2, 3, 4, 5));

        WHEN_CALL(t, clear);
        WHEN_CALL(t, pop);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_SIZE(t, 0);

        WHEN_CALL(t, removeAt);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        WHEN_CALL_N(t, removeAt, "a");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        WHEN_CALL_N(t, removeAt, 0.f);

        WHEN_CALL_N(t, append, 100);
        WHEN_CALL_N(t, append, 200);
        REQUIRE_LIST(t, LF(100, 200));

        WHEN_CALL_N(t, removeAt, 123);
        REQUIRE_LIST(t, LF(100, 200));

        WHEN_CALL_N(t, removeAt, -123);
        REQUIRE_LIST(t, LF(100, 200));

        WHEN_CALL_N(t, removeAt, 123.1f);
        REQUIRE_LIST(t, LF(100, 200));

        WHEN_CALL_N(t, removeAt, 1.f);
        REQUIRE_FLOAT(t, 100);

        WHEN_CALL_N(t, removeAt, 0.f);
        REQUIRE_LIST(t, L());

        WHEN_CALL(t, insert);
        REQUIRE_LIST(t, L());

        WHEN_CALL_N(t, insert, 0.f);
        REQUIRE_LIST(t, L());

        WHEN_CALL_N(t, insert, 0.f, 100);
        REQUIRE_FLOAT(t, 100);

        WHEN_CALL_N(t, insert, 0.f, 200);
        REQUIRE_LIST(t, LF(200, 100));

        WHEN_CALL_N(t, insert, 1, 300);
        REQUIRE_LIST(t, LF(200, 300, 100));

        WHEN_CALL_N(t, insert, 3, 400);
        REQUIRE_LIST(t, LF(200, 300, 100, 400));

        WHEN_CALL_N(t, insert, 5, 500);
        REQUIRE_LIST(t, LF(200, 300, 100, 400));
    }

    SECTION("construct args")
    {
        TObj t("data.list", LF(1, 2, 3, 4, 5));
        REQUIRE_LIST(t, LF(1, 2, 3, 4, 5));

        REQUIRE_PROPERTY(t, @size, 5);
        REQUIRE_PROPERTY(t, @empty, 0);
        REQUIRE_PROPERTY(t, @value, 1, 2, 3, 4, 5);
    }

    SECTION("construct args")
    {
        TObj t("data.list", AtomList::parseString("\"a b c\" S\"a b c\""));

        REQUIRE_PROPERTY(t, @size, 2);
        REQUIRE_PROPERTY(t, @empty, 0);
        REQUIRE_PROPERTY(t, @value, "a b c", StringAtom("a b c"));
    }

    SECTION("input")
    {
        TExt t("data.list", LF(1, 2, 3));

        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3));

        t << "ABC";
        REQUIRE(t.outputListAt(0) == LA("ABC"));

        t << LA("A", "B", "C");
        REQUIRE(t.outputListAt(0) == LA("A", "B", "C"));
    }
}
