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
#include "list_walk.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListWalk, list, walk)

using TExt = TestExtListWalk;
using TObj = TestListWalk;

#define REQUIRE_LIST_MSG(obj, lst)                     \
    {                                                  \
        REQUIRE(obj.hasNewMessages());                 \
        REQUIRE(obj.lastMessage().isList());           \
        REQUIRE(obj.lastMessage().listValue() == lst); \
    }

#define REQUIRE_FLOAT(obj, f)                                  \
    {                                                          \
        REQUIRE(obj.hasNewMessages());                         \
        REQUIRE(obj.lastMessage().isFloat());                  \
        REQUIRE(obj.lastMessage().atomValue().asFloat() == f); \
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

TEST_CASE("list.walk 2", "[externals]")
{
    pd_test_init();

    SECTION("test fold")
    {
        SECTION("basic")
        {
            TestListWalk t("list.walk", LA("@fold"));

            CALL(t, current);
            REQUIRE_THAT(t, !hasOutput(&t));
            REQUIRE_INDEX(t, 0);
            CALL(t, next);
            REQUIRE_THAT(t, !hasOutput(&t));
            REQUIRE_INDEX(t, 0);

            t.sendList(LF(1, 2, 3));
            REQUIRE_PROP(t, index, LF(0));

            CALL(t, current);
            REQUIRE_FLOAT(t, 1);
            REQUIRE_INDEX(t, 0);

            CALL(t, next);
            REQUIRE_FLOAT(t, 2);
            REQUIRE_INDEX(t, 1);
            CALL(t, next);
            REQUIRE_FLOAT(t, 3);
            REQUIRE_INDEX(t, 2);
            CALL(t, next);
            REQUIRE_FLOAT(t, 2);
            REQUIRE_INDEX(t, 1);
            CALL(t, next);
            REQUIRE_FLOAT(t, 1);
            REQUIRE_INDEX(t, 0);
            CALL(t, next);
            REQUIRE_FLOAT(t, 2);
            REQUIRE_INDEX(t, 1);

            CALL(t, prev);
            REQUIRE_FLOAT(t, 1);
            REQUIRE_INDEX(t, 0);
            CALL(t, prev);
            REQUIRE_FLOAT(t, 2);
            REQUIRE_INDEX(t, 1);
            CALL(t, prev);
            REQUIRE_FLOAT(t, 3);
            REQUIRE_INDEX(t, 2);
            CALL(t, prev);
            REQUIRE_FLOAT(t, 2);
            REQUIRE_INDEX(t, 1);

            CALL(t, reset);
            REQUIRE_PROP(t, index, LF(0));
            CALL(t, prev);
            REQUIRE_PROP(t, index, LF(1));
            REQUIRE_FLOAT(t, 2);
            CALL(t, prev);
            REQUIRE_FLOAT(t, 3);
            CALL(t, prev);
            REQUIRE_FLOAT(t, 2);
            CALL(t, prev);
            REQUIRE_FLOAT(t, 1);
            REQUIRE_INDEX(t, 0);
        }

        SECTION("step 2")
        {
            TestListWalk t("list.walk", LA("@fold"));

            CALL(t, current);
            REQUIRE_THAT(t, !hasOutput(&t));
            CALL_N(t, next, 2);
            REQUIRE_THAT(t, !hasOutput(&t));

            t.sendList(LF(1, 2, 3));

            CALL(t, current);
            REQUIRE_FLOAT(t, 1);

            // 1 2 3 2 1 2 3 2 1
            CALL_N(t, next, 2);
            REQUIRE_FLOAT(t, 3);
            CALL_N(t, next, 2);
            REQUIRE_FLOAT(t, 1);
            CALL_N(t, next, 3);
            REQUIRE_FLOAT(t, 2);
            REQUIRE_INDEX(t, 1);

            CALL_N(t, prev, 3);
            REQUIRE_FLOAT(t, 1);
            REQUIRE_INDEX(t, 0);

            CALL_N(t, prev, 4);
            REQUIRE_FLOAT(t, 1);
            REQUIRE_INDEX(t, 0);
        }

        SECTION("length 2")
        {
            TestListWalk t("list.walk", LA("@fold", "@length", 2));

            CALL(t, current);
            REQUIRE_THAT(t, !hasOutput(&t));
            CALL(t, next);
            REQUIRE_THAT(t, !hasOutput(&t));

            t.sendList(LF(1, 2, 3, 4, 5));

            CALL(t, current);
            REQUIRE_LIST_MSG(t, LF(1, 2));

            // 1 2 3 4 5 4 3 2 1 2 3 4 5 4 3 2 1
            CALL_N(t, next, 2);
            REQUIRE_LIST_MSG(t, LF(3, 4));
            CALL_N(t, next, 2);
            REQUIRE_LIST_MSG(t, LF(5, 4));
            CALL_N(t, next, 2);
            REQUIRE_LIST_MSG(t, LF(3, 2));

            CALL_N(t, prev, 3);
            REQUIRE_LIST_MSG(t, LF(4, 5));
            CALL_N(t, prev, 3);
            REQUIRE_LIST_MSG(t, LF(1, 2));
            CALL_N(t, prev, 3);
            REQUIRE_LIST_MSG(t, LF(4, 3));
        }
    }

    SECTION("test length < 1")
    {
        TestListWalk t("list.walk", L());
        REQUIRE(t.setProperty("@length", LF(0)));

        CALL(t, current);
        REQUIRE_THAT(t, !hasOutput(&t));

        t.sendList(LF(1, 2));
        CALL(t, current);
        REQUIRE_THAT(t, !hasOutput(&t));

        REQUIRE(t.setProperty("@length", LF(-10)));

        CALL(t, current);
        REQUIRE_THAT(t, !hasOutput(&t));
    }

    SECTION("test @size")
    {
        TestListWalk t("list.walk", L());
        REQUIRE_PROP(t, size, LF(0));

        t.sendList(LF(1.0, 2.0));
        REQUIRE_PROP(t, size, LF(2));
    }

    SECTION("test bang")
    {
        TestListWalk t("list.walk", LF(1, 2));
        REQUIRE(t.property("@mode")->get() == LA("single"));
        REQUIRE(t.setProperty("@loop", L()));
        REQUIRE(t.property("@mode")->get() == LA("wrap"));

        t.sendBang();
        REQUIRE_FLOAT(t, 1);
        t.sendBang();
        REQUIRE_FLOAT(t, 2);
        t.sendBang();
        REQUIRE_FLOAT(t, 1);
    }

    SECTION("test @direction")
    {
        TestListWalk t("list.walk", LF(1, 2, 3));
        REQUIRE_PROP(t, direction, LF(1));
        REQUIRE(t.setProperty("@direction", LF(0)));
        REQUIRE(t.setProperty("@loop", L()));

        t.sendBang();
        REQUIRE_FLOAT(t, 1);
        t.sendBang();
        REQUIRE_FLOAT(t, 3);
        t.sendBang();
        REQUIRE_FLOAT(t, 2);
        CALL(t, prev);
        REQUIRE_FLOAT(t, 2);
        CALL(t, next);
        REQUIRE_FLOAT(t, 1);

        REQUIRE(t.setProperty("@direction", LF(1)));
        t.sendBang();
        REQUIRE_FLOAT(t, 1);
        t.sendBang();
        REQUIRE_FLOAT(t, 2);
        CALL(t, prev);
        REQUIRE_FLOAT(t, 2);
        CALL(t, next);
        REQUIRE_FLOAT(t, 3);
    }

    SECTION("test float")
    {
        SECTION("forward")
        {
            TestListWalk t("list.walk", LF(1, 2, 3, 4, 5));
            REQUIRE_PROP(t, direction, LF(1));
            REQUIRE(t.setProperty("@loop", L()));

            t.sendFloat(1);
            REQUIRE_FLOAT(t, 1);
            CALL(t, current);
            REQUIRE_FLOAT(t, 2);
            t.sendFloat(0);
            REQUIRE_FLOAT(t, 2);

            t.sendFloat(2);
            REQUIRE_FLOAT(t, 2);
            CALL(t, current);
            REQUIRE_FLOAT(t, 4);

            t.sendFloat(-4);
            REQUIRE_FLOAT(t, 4);
            CALL(t, current);
            REQUIRE_FLOAT(t, 5);

            t.sendFloat(-1);
            REQUIRE_FLOAT(t, 5);
            CALL(t, current);
            REQUIRE_FLOAT(t, 4);
        }

        SECTION("backward")
        {
            TestListWalk t("list.walk", LF(1, 2, 3, 4, 5));
            REQUIRE_PROP(t, direction, LF(1));
            REQUIRE(t.setProperty("@direction", LF(0)));
            REQUIRE(t.setProperty("@loop", L()));

            t.sendFloat(1);
            REQUIRE_FLOAT(t, 1);
            CALL(t, current);
            REQUIRE_FLOAT(t, 5);

            t.sendFloat(2);
            REQUIRE_FLOAT(t, 5);
            CALL(t, current);
            REQUIRE_FLOAT(t, 3);

            t.sendFloat(-4);
            REQUIRE_FLOAT(t, 3);
            CALL(t, current);
            REQUIRE_FLOAT(t, 2);

            t.sendFloat(-1);
            REQUIRE_FLOAT(t, 2);
            CALL(t, current);
            REQUIRE_FLOAT(t, 3);
        }
    }

    SECTION("test set index")
    {
        TestListWalk t("list.walk", L());
        t.p_set_index(LF(2));
        REQUIRE_PROP(t, index, LF(0));
        t.p_set_index(AtomList(-2));
        REQUIRE(t.p_index() == LF(0));

        t.sendList(LF(2, 3));
        t.p_set_index(LF(0));
        REQUIRE_PROP(t, index, LF(0));
        t.p_set_index(LF(1));
        REQUIRE_PROP(t, index, LF(1));
        t.p_set_index(LF(2));
        REQUIRE_PROP(t, index, LF(1));
        t.p_set_index(LF(3));
        REQUIRE_PROP(t, index, LF(1));
        t.p_set_index(LF(4));
        REQUIRE_PROP(t, index, LF(1));

        t.p_set_index(LF(-1));
        REQUIRE_PROP(t, index, LF(1));
        t.p_set_index(LF(-2));
        REQUIRE_PROP(t, index, LF(0));
        t.p_set_index(LF(-3));
        REQUIRE_PROP(t, index, LF(0));
        t.p_set_index(LF(-4));
        REQUIRE_PROP(t, index, LF(0));
    }
}
