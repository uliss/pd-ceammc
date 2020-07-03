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
#include "list_any_of.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListAnyOf, list, any_of)

using TObj = TestListAnyOf;
using TExt = TestExtListAnyOf;

static void eqThree(TObj* obj, size_t, const Atom& a)
{
    obj->sendFloat(a.asInt() == 3 ? 1 : 0, 1);
}

static void lessThree(TObj* obj, size_t, const Atom& a)
{
    obj->sendFloat(a.asInt() < 3 ? 1 : 0, 1);
}

TEST_CASE("list.any_of", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("list.any_of", L());
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
    }

#define REQUIRE_ANY(obj, lst, cb)        \
    {                                    \
        obj.setSendAtomCallback(cb);     \
        WHEN_SEND_LIST_TO(0, obj, lst);  \
        REQUIRE_THAT(t, outputTrue(&t)); \
    }

#define REQUIRE_NOT_ANY(obj, lst, cb)     \
    {                                     \
        obj.setSendAtomCallback(cb);      \
        WHEN_SEND_LIST_TO(0, obj, lst);   \
        REQUIRE_THAT(t, outputFalse(&t)); \
    }

    SECTION("connect")
    {
        TObj t("list.any_of", L());

        REQUIRE_ANY(t, LF(3), eqThree);
        REQUIRE_ANY(t, LF(3, 1), eqThree);
        REQUIRE_ANY(t, LF(1, 2, 3), eqThree);

        REQUIRE_NOT_ANY(t, LF(1), eqThree);
        REQUIRE_NOT_ANY(t, L(), eqThree);
        REQUIRE_NOT_ANY(t, LF(1, 2, 4), eqThree);
        REQUIRE_NOT_ANY(t, LF(-9, 4, 10), eqThree);

        REQUIRE_ANY(t, LF(2), lessThree);
        REQUIRE_ANY(t, LF(0.f, 1), lessThree);
        REQUIRE_ANY(t, LF(22, 2, 10), lessThree);

        REQUIRE_NOT_ANY(t, L(), lessThree);
        REQUIRE_NOT_ANY(t, LF(4, 4, 3, 4), lessThree);
    }

    SECTION("external")
    {
        TExt t("list.any_of");
        pd::External less10("<", 10);

        t.connectTo(1, less10, 0);
        t.connectFrom(0, less10, 1);

        t.send(1, 2, 3, 4);
        REQUIRE_THAT(t, outputTrue(&t));

        t.send(8, 9);
        REQUIRE_THAT(t, outputTrue(&t));

        t.send(8, 9, 10);
        REQUIRE_THAT(t, outputTrue(&t));

        t.send(10, 11, 12);
        REQUIRE_THAT(t, outputFalse(&t));

        t.send(MLA(1, 2, 3, 4));
        REQUIRE_THAT(t, outputTrue(&t));

        t.send(MLA(8, 9));
        REQUIRE_THAT(t, outputTrue(&t));

        t.send(MLA(8, 9, 10));
        REQUIRE_THAT(t, outputTrue(&t));

        t.send(MLA(10, 11, 12));
        REQUIRE_THAT(t, outputFalse(&t));
    }
}
