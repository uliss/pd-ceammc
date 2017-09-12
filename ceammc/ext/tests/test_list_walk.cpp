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

#include "../list/list_walk.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc.hpp"

#include <stdio.h>

typedef TestExtension<ListWalk> ListWalkTest;

TEST_CASE("list.walk", "[PureData]")
{
    SECTION("test single")
    {
        AtomList args;
        ListWalkTest t("list.walk", args);

        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE(t.hasProperty("@direction"));
        REQUIRE(t.hasProperty("@index"));
        REQUIRE(t.hasProperty("@value"));
        REQUIRE(t.hasProperty("@mode"));
        REQUIRE(t.hasProperty("@length"));
        REQUIRE(t.hasProperty("@single"));
        REQUIRE(t.hasProperty("@loop"));
        REQUIRE(t.hasProperty("@wrap"));
        REQUIRE(t.hasProperty("@clip"));
        REQUIRE(t.hasProperty("@fold"));
        REQUIRE(t.hasProperty("@size"));
        REQUIRE(t.property("@size")->readonly());
        REQUIRE(!t.property("@index")->readonly());

        REQUIRE(t.messageCount() == 0);

        t.sendList(AtomList::values(3, 1.0, 2.0, 3.0));

        CALL(t, current);
        REQUIRE_LIST_MSG(t, AtomList(1));
        REQUIRE_INDEX(t, 0);

        CALL(t, next);
        REQUIRE_LIST_MSG(t, AtomList(2));
        REQUIRE_INDEX(t, 1);
        CALL(t, current);
        REQUIRE_LIST_MSG(t, AtomList(2));

        CALL(t, next);
        REQUIRE_LIST_MSG(t, AtomList(3));
        REQUIRE_INDEX(t, 2);
        CALL(t, current);
        REQUIRE_LIST_MSG(t, AtomList(3));

        CALL(t, next);
        REQUIRE_NO_MSG(t);
        REQUIRE_INDEX(t, 2);

        CALL(t, current);
        REQUIRE_NO_MSG(t);

        CALL(t, reset);
        REQUIRE_INDEX(t, 0);
        CALL(t, current);
        REQUIRE_LIST_MSG(t, AtomList(1));
        CALL(t, next);
        REQUIRE_LIST_MSG(t, AtomList(2));
        CALL(t, next);
        REQUIRE_LIST_MSG(t, AtomList(3));

        CALL(t, next);
        REQUIRE_NO_MSG(t);
        REQUIRE_INDEX(t, 2);
        CALL(t, next);
        REQUIRE_NO_MSG(t);
        REQUIRE_INDEX(t, 2);
        CALL(t, next);
        REQUIRE_NO_MSG(t);
        REQUIRE_INDEX(t, 2);

        CALL(t, prev);
        REQUIRE_LIST_MSG(t, AtomList(2));
        CALL(t, prev);
        REQUIRE_LIST_MSG(t, AtomList(1));

        CALL(t, prev);
        REQUIRE_NO_MSG(t);
        REQUIRE_INDEX(t, 0);
        CALL(t, prev);
        REQUIRE_NO_MSG(t);
        REQUIRE_INDEX(t, 0);

        CALL(t, next);
        REQUIRE_LIST_MSG(t, AtomList(2));
        REQUIRE_INDEX(t, 1);

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
            REQUIRE_INDEX(t, 0);
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
                REQUIRE_INDEX(t, 0);
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
                REQUIRE_INDEX(t, 4);

                CALL1(t, prev, 2);
                REQUIRE_LIST_MSG(t, AtomList(3, 4));

                CALL1(t, prev, 2);
                REQUIRE_LIST_MSG(t, AtomList(1, 2));

                CALL(t, prev);
                REQUIRE_NO_MSG(t);
                REQUIRE_INDEX(t, 0);
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
}
