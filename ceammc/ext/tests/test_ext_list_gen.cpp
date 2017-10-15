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
#include "../list/list_gen.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListGenerate> ListGenTest;

TEST_CASE("list.gen", "[externals]")
{
    obj_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            ListGenTest t("list.gen", AtomList());
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);

            REQUIRE_PROPERTY(t, @count, 0.f);
        }

        SECTION("properties")
        {
            {
                ListGenTest t("list.gen", L2("@count", 2));
                REQUIRE_PROPERTY(t, @count, 2.f);
            }

            {
                ListGenTest t("list.gen", L2("@count", 1024));
                REQUIRE_PROPERTY(t, @count, 1024);
            }

            {
                ListGenTest t("list.gen", L2("@count", 20003));
                REQUIRE_PROPERTY(t, @count, 0.f);
            }

            {
                ListGenTest t("list.gen", L2("@count", -1));
                REQUIRE_PROPERTY(t, @count, 0.f);
            }
        }

        SECTION("positional arguments")
        {
            {
                ListGenTest t("list.gen", L1(100));
                REQUIRE_PROPERTY(t, @count, 100);
            }

            {
                ListGenTest t("list.gen", L1(1024));
                REQUIRE_PROPERTY(t, @count, 1024);
            }

            {
                ListGenTest t("list.gen", L1(100000));
                REQUIRE_PROPERTY(t, @count, 0.f);
            }

            {
                ListGenTest t("list.gen", L1(-100));
                REQUIRE_PROPERTY(t, @count, 0.f);
            }
        }

        SECTION("positional arguments and props mixed")
        {
            ListGenTest t("list.gen", L3(100, "@count", 10));
            REQUIRE_PROPERTY(t, @count, 10);
        }
    }
}
