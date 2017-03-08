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

#define REQUIRE_PROP(obj, name, val)                      \
    {                                                     \
        Property* p = obj.getProperty(gensym("@" #name)); \
        REQUIRE(p != 0);                                  \
        REQUIRE(p->get() == val);                         \
    }

#define REQUIRE_INDEX(obj, idx)                         \
    {                                                   \
        REQUIRE(obj.p_index() == AtomList(float(idx))); \
    }

#define REQUIRE_NO_MSG(obj) REQUIRE_FALSE(obj.hasNewMessages())

TEST_CASE("list.walk", "[PureData]")
{
    SECTION("test single")
    {
        AtomList args;
        ListWalkTest t("list.walk", args);

        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE(t.hasProperty(gensym("@direction")));
        REQUIRE(t.hasProperty(gensym("@index")));
        REQUIRE(t.hasProperty(gensym("@value")));
        REQUIRE(t.hasProperty(gensym("@mode")));
        REQUIRE(t.hasProperty(gensym("@length")));
        REQUIRE(t.hasProperty(gensym("@single")));
        REQUIRE(t.hasProperty(gensym("@loop")));
        REQUIRE(t.hasProperty(gensym("@wrap")));
        REQUIRE(t.hasProperty(gensym("@clip")));
        REQUIRE(t.hasProperty(gensym("@fold")));
        REQUIRE(t.hasProperty(gensym("@size")));

        REQUIRE(t.messageCount() == 0);

        t.sendList(AtomList::values(3, 1.0, 2.0, 3.0));

        CALL(t, current);
        REQUIRE_LIST_MSG(t, AtomList(1));

        CALL(t, next);
        REQUIRE_LIST_MSG(t, AtomList(2));
        CALL(t, current);
        REQUIRE_LIST_MSG(t, AtomList(2));

        CALL(t, next);
        REQUIRE_LIST_MSG(t, AtomList(3));
        CALL(t, current);
        REQUIRE_LIST_MSG(t, AtomList(3));

        CALL(t, next);
        REQUIRE_NO_MSG(t);
        CALL(t, current);
        REQUIRE_NO_MSG(t);

        CALL(t, reset);
        CALL(t, current);
        REQUIRE_LIST_MSG(t, AtomList(1));
        CALL(t, next);
        REQUIRE_LIST_MSG(t, AtomList(2));
        CALL(t, next);
        REQUIRE_LIST_MSG(t, AtomList(3));

        CALL(t, next);
        REQUIRE_NO_MSG(t);
        CALL(t, next);
        REQUIRE_NO_MSG(t);
        CALL(t, next);
        REQUIRE_NO_MSG(t);

        CALL(t, prev);
        REQUIRE_LIST_MSG(t, AtomList(2));
        CALL(t, prev);
        REQUIRE_LIST_MSG(t, AtomList(1));

        CALL(t, prev);
        REQUIRE_NO_MSG(t);
        CALL(t, prev);
        REQUIRE_NO_MSG(t);

        CALL(t, next);
        REQUIRE_LIST_MSG(t, AtomList(2));

        SECTION("test single step")
        {
            AtomList args;
            ListWalkTest t("list.walk", args);

            t.sendList(AtomList::values(5, 1.0, 2.0, 3.0, 4.0, 5.0));

            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(1));

            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(3));

            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(5));

            CALL1(t, next, 2);
            REQUIRE_NO_MSG(t);

            CALL1(t, prev, 2);
            REQUIRE_LIST_MSG(t, AtomList(3));

            CALL1(t, prev, 2);
            REQUIRE_LIST_MSG(t, AtomList(1));

            CALL1(t, prev, 2);
            REQUIRE_NO_MSG(t);
        }

        SECTION("test single length 2")
        {
            AtomList args(gensym("@length"), 2);

            SECTION("step 1")
            {
                ListWalkTest t("list.walk", args);

                t.sendList(AtomList::values(5, 1.0, 2.0, 3.0, 4.0, 5.0));

                CALL(t, current);
                REQUIRE_LIST_MSG(t, AtomList(1, 2));

                CALL(t, next);
                REQUIRE_LIST_MSG(t, AtomList(2, 3));

                CALL(t, next);
                REQUIRE_LIST_MSG(t, AtomList(3, 4));

                CALL(t, next);
                REQUIRE_LIST_MSG(t, AtomList(4, 5));

                CALL(t, next);
                REQUIRE_LIST_MSG(t, AtomList(5));

                CALL(t, next);
                REQUIRE_NO_MSG(t);

                CALL(t, prev);
                REQUIRE_LIST_MSG(t, AtomList(4, 5));

                CALL(t, prev);
                REQUIRE_LIST_MSG(t, AtomList(3, 4));

                CALL(t, prev);
                REQUIRE_LIST_MSG(t, AtomList(2, 3));

                CALL(t, prev);
                REQUIRE_LIST_MSG(t, AtomList(1, 2));

                CALL(t, prev);
                REQUIRE_NO_MSG(t);
            }

            SECTION("step 2")
            {
                ListWalkTest t("list.walk", args);

                t.sendList(AtomList::values(5, 1.0, 2.0, 3.0, 4.0, 5.0));

                CALL(t, current);
                REQUIRE_LIST_MSG(t, AtomList(1, 2));

                CALL1(t, next, 2);
                REQUIRE_LIST_MSG(t, AtomList(3, 4));

                CALL1(t, next, 2);
                REQUIRE_LIST_MSG(t, AtomList(5));

                CALL1(t, next, 2);
                REQUIRE_NO_MSG(t);

                CALL1(t, prev, 2);
                REQUIRE_LIST_MSG(t, AtomList(3, 4));

                CALL1(t, prev, 2);
                REQUIRE_LIST_MSG(t, AtomList(1, 2));

                CALL(t, prev);
                REQUIRE_NO_MSG(t);
            }
        }
    }

    SECTION("test wrap")
    {
        SECTION("basic")
        {
            AtomList args(gensym("@wrap"));
            ListWalkTest t("list.walk", args);

            CALL(t, current);
            REQUIRE_NO_MSG(t);
            REQUIRE_INDEX(t, 0);
            CALL(t, next);
            REQUIRE_NO_MSG(t);

            t.sendList(AtomList::values(3, 1.0, 2.0, 3.0));

            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(1));
            REQUIRE_INDEX(t, 0);

            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(2));
            REQUIRE_INDEX(t, 1);
            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(3));
            REQUIRE_INDEX(t, 2);
            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(1));
            REQUIRE_INDEX(t, 0);
            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(2));
            REQUIRE_INDEX(t, 1);
            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(3));
            REQUIRE_INDEX(t, 2);

            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(2));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(1));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(3));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(2));

            CALL(t, reset);
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(3));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(2));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(1));
        }

        SECTION("step 2")
        {
            AtomList args(gensym("@wrap"));
            ListWalkTest t("list.walk", args);

            CALL(t, current);
            REQUIRE_NO_MSG(t);
            CALL(t, next);
            REQUIRE_NO_MSG(t);

            t.sendList(AtomList::values(3, 1.0, 2.0, 3.0));

            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(1));
            REQUIRE_INDEX(t, 0);

            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(3));
            REQUIRE_INDEX(t, 2);
            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(2));
            REQUIRE_INDEX(t, 1);
            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(1));
            REQUIRE_INDEX(t, 0);

            CALL1(t, prev, 3);
            REQUIRE_LIST_MSG(t, AtomList(1));
            REQUIRE_INDEX(t, 0);

            CALL1(t, prev, 4);
            REQUIRE_LIST_MSG(t, AtomList(3));
            REQUIRE_INDEX(t, 2);
        }

        SECTION("length 2")
        {
            AtomList args(gensym("@wrap"));
            args.append(AtomList(gensym("@length"), 2));
            ListWalkTest t("list.walk", args);

            CALL(t, current);
            REQUIRE_NO_MSG(t);
            CALL(t, next);
            REQUIRE_NO_MSG(t);

            t.sendList(AtomList::values(5, 1.0, 2.0, 3.0, 4.0, 5.0));

            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(1, 2));
            REQUIRE_INDEX(t, 0);

            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(3, 4));
            REQUIRE_INDEX(t, 2);
            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(5, 1));
            REQUIRE_INDEX(t, 4);
            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(2, 3));
            REQUIRE_INDEX(t, 1);

            CALL1(t, prev, 3);
            REQUIRE_LIST_MSG(t, AtomList(4, 5));
            REQUIRE_INDEX(t, 3);
            CALL1(t, prev, 3);
            REQUIRE_LIST_MSG(t, AtomList(1, 2));
            REQUIRE_INDEX(t, 0);
        }
    }

    SECTION("test clip")
    {
        SECTION("basic")
        {
            AtomList args(gensym("@clip"));
            ListWalkTest t("list.walk", args);

            CALL(t, current);
            REQUIRE_NO_MSG(t);
            REQUIRE_INDEX(t, 0);
            CALL(t, next);
            REQUIRE_NO_MSG(t);

            t.sendList(AtomList::values(3, 1.0, 2.0, 3.0));

            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(1));
            REQUIRE_INDEX(t, 0);

            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(2));
            REQUIRE_INDEX(t, 1);
            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(3));
            REQUIRE_INDEX(t, 2);
            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(3));
            REQUIRE_INDEX(t, 2);
            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(3));
            REQUIRE_INDEX(t, 2);
            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(3));
            REQUIRE_INDEX(t, 2);

            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(2));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(1));
            REQUIRE_INDEX(t, 0);
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(1));
            REQUIRE_INDEX(t, 0);
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(1));
            REQUIRE_INDEX(t, 0);

            CALL(t, reset);
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(1));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(1));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(1));
        }

        SECTION("step 2")
        {
            AtomList args(gensym("@clip"));
            ListWalkTest t("list.walk", args);

            CALL(t, current);
            REQUIRE_NO_MSG(t);
            CALL1(t, next, 2);
            REQUIRE_NO_MSG(t);

            t.sendList(AtomList::values(3, 1.0, 2.0, 3.0));

            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(1));

            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(3));
            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(3));
            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(3));
            REQUIRE_INDEX(t, 2);

            CALL1(t, prev, 2);
            REQUIRE_LIST_MSG(t, AtomList(1));

            CALL1(t, prev, 4);
            REQUIRE_LIST_MSG(t, AtomList(1));
            REQUIRE_INDEX(t, 0);
        }

        SECTION("length 2")
        {
            AtomList args(gensym("@clip"));
            args.append(AtomList(gensym("@length"), 2));
            ListWalkTest t("list.walk", args);

            CALL(t, current);
            REQUIRE_NO_MSG(t);
            CALL(t, next);
            REQUIRE_NO_MSG(t);

            t.sendList(AtomList::values(5, 1.0, 2.0, 3.0, 4.0, 5.0));

            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(1, 2));

            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(3, 4));
            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(5, 5));
            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(5, 5));
            REQUIRE_INDEX(t, 4);

            CALL1(t, prev, 3);
            REQUIRE_LIST_MSG(t, AtomList(2, 3));
            CALL1(t, prev, 3);
            REQUIRE_LIST_MSG(t, AtomList(1, 2));
            REQUIRE_INDEX(t, 0);
        }
    }

    SECTION("test fold")
    {
        SECTION("basic")
        {
            AtomList args(gensym("@fold"));
            ListWalkTest t("list.walk", args);

            CALL(t, current);
            REQUIRE_NO_MSG(t);
            CALL(t, next);
            REQUIRE_NO_MSG(t);

            t.sendList(AtomList::values(3, 1.0, 2.0, 3.0));
            REQUIRE_PROP(t, index, AtomList(0.f));

            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(1));

            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(2));
            REQUIRE_PROP(t, index, AtomList(1));
            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(3));
            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(2));
            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(1));
            CALL(t, next);
            REQUIRE_LIST_MSG(t, AtomList(2));

            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(1));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(2));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(3));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(2));

            CALL(t, reset);
            REQUIRE_PROP(t, index, AtomList(0.f));
            CALL(t, prev);
            REQUIRE_PROP(t, index, AtomList(1));
            REQUIRE_LIST_MSG(t, AtomList(2));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(3));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(2));
            CALL(t, prev);
            REQUIRE_LIST_MSG(t, AtomList(1));
        }

        SECTION("step 2")
        {
            AtomList args(gensym("@fold"));
            ListWalkTest t("list.walk", args);

            CALL(t, current);
            REQUIRE_NO_MSG(t);
            CALL1(t, next, 2);
            REQUIRE_NO_MSG(t);

            t.sendList(AtomList::values(3, 1.0, 2.0, 3.0));

            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(1));

            // 1 2 3 2 1 2 3 2 1
            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(3));
            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(1));
            CALL1(t, next, 3);
            REQUIRE_LIST_MSG(t, AtomList(2));

            CALL1(t, prev, 3);
            REQUIRE_LIST_MSG(t, AtomList(1));

            CALL1(t, prev, 4);
            REQUIRE_LIST_MSG(t, AtomList(1));
        }

        SECTION("length 2")
        {
            AtomList args(gensym("@fold"));
            args.append(AtomList(gensym("@length"), 2));
            ListWalkTest t("list.walk", args);

            CALL(t, current);
            REQUIRE_NO_MSG(t);
            CALL(t, next);
            REQUIRE_NO_MSG(t);

            t.sendList(AtomList::values(5, 1.0, 2.0, 3.0, 4.0, 5.0));

            CALL(t, current);
            REQUIRE_LIST_MSG(t, AtomList(1, 2));

            // 1 2 3 4 5 4 3 2 1 2 3 4 5 4 3 2 1
            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(3, 4));
            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(5, 4));
            CALL1(t, next, 2);
            REQUIRE_LIST_MSG(t, AtomList(3, 2));

            CALL1(t, prev, 3);
            REQUIRE_LIST_MSG(t, AtomList(4, 5));
            CALL1(t, prev, 3);
            REQUIRE_LIST_MSG(t, AtomList(1, 2));
            CALL1(t, prev, 3);
            REQUIRE_LIST_MSG(t, AtomList(4, 3));
        }
    }
}
