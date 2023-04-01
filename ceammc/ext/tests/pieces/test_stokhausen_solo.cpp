/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "stok_solo_data.h"
#include "test_common.h"
#include "test_pieces_base.h"

// PD_COMPLETE_TEST_SETUP(PathExists, path, exists)

using namespace solo;

TEST_CASE("pieces.stok_solo~", "[externals]")
{
    test::pdPrintToStdError();

    SECTION("init")
    {
        Scheme s;
        REQUIRE(s.length() == 0);
        REQUIRE_FALSE(s.set(-1));
        REQUIRE_FALSE(s.set(0));
        REQUIRE_FALSE(s.set(3));
        REQUIRE_FALSE(s.set(4));
        REQUIRE_FALSE(s.set(5));
        REQUIRE_FALSE(s.set(6));
        REQUIRE_FALSE(s.set(7));

        REQUIRE(s.set(2));

        REQUIRE(s.a.length() == 108);
        REQUIRE(s.b.length() == 168);
        REQUIRE(s.c.length() == 66);
        REQUIRE(s.d.length() == 85);
        REQUIRE(s.e.length() == Approx(182.4));
        REQUIRE(s.f.length() == Approx(136.8));

        REQUIRE(s.a.abs_period_length == 12);
        REQUIRE(s.b.abs_period_length == 24);
        REQUIRE(s.c.abs_period_length == 6);
        REQUIRE(s.d.abs_period_length == 8.5);
        REQUIRE(s.e.abs_period_length == Approx(30.4));
        REQUIRE(s.f.abs_period_length == Approx(17.1));

        REQUIRE(s.a.periodCount() == 9);
        REQUIRE(s.b.periodCount() == 7);
        REQUIRE(s.c.periodCount() == 11);
        REQUIRE(s.d.periodCount() == 10);
        REQUIRE(s.e.periodCount() == 6);
        REQUIRE(s.f.periodCount() == 8);
        REQUIRE(s.toString(0) == "a");
    }
}
