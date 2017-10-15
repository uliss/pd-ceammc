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
#include "../list/list_normalize.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ListNormalize> ListNormalizeTest;

TEST_CASE("list.normalize", "[externals]")
{
    SECTION("test create with:")
    {
        SECTION("empty")
        {
            ListNormalizeTest t("list.normalize");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @by, A("sum"));
        }

        SECTION("properties")
        {
            ListNormalizeTest t("list.normalize", L2("@by", "range"));
            REQUIRE_PROPERTY(t, @by, A("range"));
        }

        SECTION("@sum")
        {
            ListNormalizeTest t("list.normalize", L1("@sum"));
            REQUIRE_PROPERTY(t, @by, A("sum"));
        }

        SECTION("@range")
        {
            ListNormalizeTest t("list.normalize", L1("@range"));
            REQUIRE_PROPERTY(t, @by, A("range"));
        }
    }

    SECTION("sum")
    {
        ListNormalizeTest t("list.normalize");
        WHEN_SEND_LIST_TO(0, t, L4(1, 2, 3, 4));
        REQUIRE_LIST_AT_OUTLET(0, t, L4(0.1f, 0.2f, 0.3f, 0.4f));

        // sum == 0
        WHEN_SEND_LIST_TO(0, t, L4(-1, -2, 1, 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        // sum == 0
        WHEN_SEND_LIST_TO(0, t, L4(-1, -2, -3, -4));
        REQUIRE_LIST_AT_OUTLET(0, t, L4(0.1f, 0.2f, 0.3f, 0.4f));
    }

    SECTION("sum")
    {
        ListNormalizeTest t("list.normalize", L1("@range"));
        WHEN_SEND_LIST_TO(0, t, L5(1, 2, 3, 4, 5));
        REQUIRE_LIST_AT_OUTLET(0, t, L5(0.0, 0.25, 0.5, 0.75, 1));

        // range == 0
        WHEN_SEND_LIST_TO(0, t, L4(1, 1, 1, 1));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }
}
