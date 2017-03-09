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

#define CATCH_CONFIG_MAIN
#include "../list/list_walk.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc.hpp"

#include <stdio.h>

typedef TestExtension<ListWalk> ListWalkTest;

#define CALL(obj, method)                            \
    {                                                \
        obj.storeMessageCount();                     \
        obj.m_##method(gensym(#method), AtomList()); \
    }

#define CALL1(obj, method, arg1)                         \
    {                                                    \
        obj.storeMessageCount();                         \
        obj.m_##method(gensym(#method), AtomList(arg1)); \
    }

#define REQUIRE_LIST_MSG(obj, lst)                     \
    {                                                  \
        REQUIRE(obj.hasNewMessages());                 \
        REQUIRE(obj.lastMessage().isList());           \
        REQUIRE(obj.lastMessage().listValue() == lst); \
    }

#define REQUIRE_PROP(obj, name, val)                   \
    {                                                  \
        Property* p = obj.property(gensym("@" #name)); \
        REQUIRE(p != 0);                               \
        REQUIRE(p->get() == val);                      \
    }

#define REQUIRE_INDEX(obj, idx)                         \
    {                                                   \
        REQUIRE(obj.p_index() == AtomList(float(idx))); \
    }

#define REQUIRE_NO_MSG(obj) REQUIRE_FALSE(obj.hasNewMessages())

TEST_CASE("list.walk 2", "[PureData]")
{
    SECTION("test float")
    {
        SECTION("forward")
        {
            ListWalkTest t("list.walk", AtomList::values(5, 1.0, 2.0, 3.0, 4.0, 5.0));
            REQUIRE_PROP(t, direction, AtomList(1.f));
            REQUIRE(t.setProperty("@loop", AtomList()));

            t.sendFloat(1);
            REQUIRE_LIST_MSG(t, AtomList(1));
            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(2));
            t.sendFloat(0);
            REQUIRE_LIST_MSG(t, AtomList(2));

            t.sendFloat(2);
            REQUIRE_LIST_MSG(t, AtomList(2));
            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(4));

            t.sendFloat(-4);
            REQUIRE_LIST_MSG(t, AtomList(4));
            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(5));

            t.sendFloat(-1);
            REQUIRE_LIST_MSG(t, AtomList(5));
            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(4));
        }

        SECTION("backward")
        {
            ListWalkTest t("list.walk", AtomList::values(5, 1.0, 2.0, 3.0, 4.0, 5.0));
            REQUIRE_PROP(t, direction, AtomList(1.f));
            REQUIRE(t.setProperty("@direction", AtomList(0.f)));
            REQUIRE(t.setProperty("@loop", AtomList()));

            t.sendFloat(1);
            REQUIRE_LIST_MSG(t, AtomList(1));
            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(5));

            t.sendFloat(2);
            REQUIRE_LIST_MSG(t, AtomList(5));
            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(3));

            t.sendFloat(-4);
            REQUIRE_LIST_MSG(t, AtomList(3));
            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(2));

            t.sendFloat(-1);
            REQUIRE_LIST_MSG(t, AtomList(2));
            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(3));
        }
    }

    SECTION("test set index")
    {
        ListWalkTest t("list.walk", AtomList());
        t.p_set_index(AtomList(2));
        REQUIRE_PROP(t, index, AtomList(0.0f));
        t.p_set_index(AtomList(-2));
        REQUIRE(t.p_index() == AtomList(0.0f));

        t.sendList(AtomList(2, 3));
        t.p_set_index(AtomList(0.0f));
        REQUIRE_PROP(t, index, AtomList(0.0f));
        t.p_set_index(AtomList(1.0f));
        REQUIRE_PROP(t, index, AtomList(1.0f));
        t.p_set_index(AtomList(2.0f));
        REQUIRE_PROP(t, index, AtomList(1.0f));
        t.p_set_index(AtomList(3.0f));
        REQUIRE_PROP(t, index, AtomList(1.0f));
        t.p_set_index(AtomList(4.0f));
        REQUIRE_PROP(t, index, AtomList(1.0f));

        t.p_set_index(AtomList(-1.0f));
        REQUIRE_PROP(t, index, AtomList(1.0f));
        t.p_set_index(AtomList(-2.0f));
        REQUIRE_PROP(t, index, AtomList(0.0f));
        t.p_set_index(AtomList(-3.0f));
        REQUIRE_PROP(t, index, AtomList(0.0f));
        t.p_set_index(AtomList(-4.0f));
        REQUIRE_PROP(t, index, AtomList(0.0f));
    }
}
