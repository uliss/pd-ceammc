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
#include "../list/list_at.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListAt> ListAtTest;

TEST_CASE("list.at", "[externals]")
{
    obj_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            ListAtTest t("list.at", AtomList());
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @index, 0.f);
        }

        SECTION("properties")
        {
            ListAtTest t("list.at", L2("@index", 2));
            REQUIRE_PROPERTY(t, @index, 2.f);
        }

        SECTION("positional arguments")
        {
            ListAtTest t("list.at", L1(100));
            REQUIRE_PROPERTY(t, @index, 100);
        }

        SECTION("positional arguments and props mixed")
        {
            ListAtTest t("list.at", L3(100, "@index", 10));
            REQUIRE_PROPERTY(t, @index, 10);
        }

        SECTION("invalid args")
        {
            SECTION("props")
            {
                ListAtTest t("list.each", L2("@index", "none"));
                REQUIRE_PROPERTY(t, @index, 0.f);
            }

            SECTION("positional")
            {
                ListAtTest t("list.each", L1("none"));
                REQUIRE_PROPERTY(t, @index, 0.f);
            }
        }
    }
}
