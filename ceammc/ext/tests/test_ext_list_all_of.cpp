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
#include "datatype_mlist.h"
#include "../list/list_all_of.h"
#include "test_base.h"
#include "test_external.h"

#include <stdio.h>

PD_COMPLETE_TEST_SETUP(ListAllOf, list, all_of);

typedef TestExternal<ListAllOf> ListAllOfTest;

static void eqThree(ListAllOfTest* obj, size_t, const Atom& a)
{
    obj->sendFloat(a.asInt() == 3 ? 1 : 0, 1);
}

static void lessThree(ListAllOfTest* obj, size_t, const Atom& a)
{
    obj->sendFloat(a.asInt() < 3 ? 1 : 0, 1);
}

TEST_CASE("list.all_of", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        ListAllOfTest t("list.all_of", L());
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
    }

#define REQUIRE_ALL(obj, lst, cb)           \
    {                                       \
        obj.setSendAtomCallback(cb);        \
        WHEN_SEND_LIST_TO(0, obj, lst);     \
        REQUIRE_FLOAT_AT_OUTLET(0, obj, 1); \
    }

#define REQUIRE_NOT_ALL(obj, lst, cb)       \
    {                                       \
        obj.setSendAtomCallback(cb);        \
        WHEN_SEND_LIST_TO(0, obj, lst);     \
        REQUIRE_FLOAT_AT_OUTLET(0, obj, 0); \
    }

    SECTION("connect")
    {
        ListAllOfTest t("list.all_of", L());

        REQUIRE_ALL(t, L(), eqThree);
        REQUIRE_ALL(t, LF(3), eqThree);
        REQUIRE_ALL(t, LF(3, 3), eqThree);
        REQUIRE_ALL(t, LF(3, 3, 3), eqThree);

        REQUIRE_NOT_ALL(t, LF(3, 3, 4), eqThree);
        REQUIRE_NOT_ALL(t, LF(3, 4, 3), eqThree);

        REQUIRE_ALL(t, L(), lessThree);
        REQUIRE_ALL(t, LF(2), lessThree);
        REQUIRE_ALL(t, LF(0.f, 1), lessThree);
        REQUIRE_ALL(t, LF(0.f, 1, 2), lessThree);

        REQUIRE_NOT_ALL(t, LF(1, 2, 3, 4), lessThree);
    }

    SECTION("external")
    {
        TestExtListAllOf t("list.all_of");
        pd::External less10("<", LF(10));

        t.connectTo(1, less10, 0);
        t.connectFrom(0, less10, 1);

        t.send(LF(1, 2, 3, 4));
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(0) == 1);

        t.send(LF(8, 9));
        REQUIRE(t.outputFloatAt(0) == 1);

        t.send(LF(8, 9, 10));
        REQUIRE(t.outputFloatAt(0) == 0);

        t.send(DataTypeMList("(1 2 3 4 5)"));
        REQUIRE(t.outputFloatAt(0) == 1);

        t.send(DataTypeMList("(8 9)"));
        REQUIRE(t.outputFloatAt(0) == 1);

        t.send(DataTypeMList("(8 9 10)"));
        REQUIRE(t.outputFloatAt(0) == 0);

        t.send(DataTypeMList("(10 11)"));
        REQUIRE(t.outputFloatAt(0) == 0);
    }
}
