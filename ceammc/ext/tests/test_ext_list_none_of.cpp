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
#include "../list/list_none_of.h"
#include "base_extension_test.h"
#include "catch.hpp"

typedef TestExtension<ListNoneOf> ListNoneOfTest;

static void eqThree(ListNoneOfTest* obj, size_t, const Atom& a)
{
    obj->sendFloat(a.asInt() == 3 ? 1 : 0, 1);
}

static void lessThree(ListNoneOfTest* obj, size_t, const Atom& a)
{
    obj->sendFloat(a.asInt() < 3 ? 1 : 0, 1);
}

TEST_CASE("list.none_of", "[externals]")
{
    obj_init();

    SECTION("init")
    {
        ListNoneOfTest t("list.none_of", AtomList());
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
    }

#define REQUIRE_NONE(obj, lst, cb)          \
    {                                       \
        obj.setSendAtomCallback(cb);        \
        WHEN_SEND_LIST_TO(0, obj, lst);     \
        REQUIRE_FLOAT_AT_OUTLET(0, obj, 1); \
    }

#define REQUIRE_SOME(obj, lst, cb)          \
    {                                       \
        obj.setSendAtomCallback(cb);        \
        WHEN_SEND_LIST_TO(0, obj, lst);     \
        REQUIRE_FLOAT_AT_OUTLET(0, obj, 0); \
    }

    SECTION("connect")
    {
        ListNoneOfTest t("list.none_off", AtomList());

        REQUIRE_NONE(t, AtomList(), eqThree);
        REQUIRE_NONE(t, L1(1), eqThree);
        REQUIRE_NONE(t, L2(1, 2), eqThree);
        REQUIRE_NONE(t, L3(1, 2, 4), eqThree);

        REQUIRE_SOME(t, L1(3), eqThree);
        REQUIRE_SOME(t, L3(1, 3, 4), eqThree);

        REQUIRE_NONE(t, L1(3), lessThree);
        REQUIRE_NONE(t, L2(3, 4), lessThree);
        REQUIRE_NONE(t, L3(3, 4, 5), lessThree);

        REQUIRE_SOME(t, L3(2, 4, 3), lessThree);
    }
}
