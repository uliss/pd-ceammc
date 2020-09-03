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
#include "test_base.h"
#include "ceammc_music_theory_keyboard.h"

#include "catch.hpp"

using namespace ceammc;
using namespace ceammc::music;
using namespace ceammc::music::keyboard;

TEST_CASE("MusicTheory::Keyboard", "[ceammc::music]")
{
    SECTION("is_white_key")
    {
        REQUIRE(is_white_key(0));
        REQUIRE(is_white_key(2));
        REQUIRE(is_white_key(4));
        REQUIRE(is_white_key(5));
        REQUIRE(is_white_key(7));
        REQUIRE(is_white_key(9));
        REQUIRE(is_white_key(11));

        REQUIRE_FALSE(is_white_key(1));
        REQUIRE_FALSE(is_white_key(3));
        REQUIRE_FALSE(is_white_key(6));
        REQUIRE_FALSE(is_white_key(8));
        REQUIRE_FALSE(is_white_key(10));

        REQUIRE(is_white_key(12));
        REQUIRE(is_white_key(14));
        REQUIRE(is_white_key(16));
        REQUIRE(is_white_key(17));
        REQUIRE(is_white_key(19));
        REQUIRE(is_white_key(21));
        REQUIRE(is_white_key(23));

        REQUIRE_FALSE(is_white_key(13));
        REQUIRE_FALSE(is_white_key(15));
        REQUIRE_FALSE(is_white_key(18));
        REQUIRE_FALSE(is_white_key(20));
        REQUIRE_FALSE(is_white_key(22));
    }

    SECTION("is_black_key")
    {
        REQUIRE_FALSE(is_black_key(0));
        REQUIRE_FALSE(is_black_key(2));
        REQUIRE_FALSE(is_black_key(4));
        REQUIRE_FALSE(is_black_key(5));
        REQUIRE_FALSE(is_black_key(7));
        REQUIRE_FALSE(is_black_key(9));
        REQUIRE_FALSE(is_black_key(11));

        REQUIRE(is_black_key(1));
        REQUIRE(is_black_key(3));
        REQUIRE(is_black_key(6));
        REQUIRE(is_black_key(8));
        REQUIRE(is_black_key(10));

        REQUIRE_FALSE(is_black_key(12));
        REQUIRE_FALSE(is_black_key(14));
        REQUIRE_FALSE(is_black_key(16));
        REQUIRE_FALSE(is_black_key(17));
        REQUIRE_FALSE(is_black_key(19));
        REQUIRE_FALSE(is_black_key(21));
        REQUIRE_FALSE(is_black_key(23));

        REQUIRE(is_black_key(13));
        REQUIRE(is_black_key(15));
        REQUIRE(is_black_key(18));
        REQUIRE(is_black_key(20));
        REQUIRE(is_black_key(22));
    }

    SECTION("wkey_to_key")
    {
        REQUIRE(wkey_to_key(0) == 0);
        REQUIRE(wkey_to_key(1) == 2);
        REQUIRE(wkey_to_key(2) == 4);
        REQUIRE(wkey_to_key(3) == 5);
        REQUIRE(wkey_to_key(4) == 7);
        REQUIRE(wkey_to_key(5) == 9);
        REQUIRE(wkey_to_key(6) == 11);
        REQUIRE(wkey_to_key(7) == 12);
        REQUIRE(wkey_to_key(8) == 14);
        REQUIRE(wkey_to_key(9) == 16);
        REQUIRE(wkey_to_key(10) == 17);
        REQUIRE(wkey_to_key(11) == 19);
        REQUIRE(wkey_to_key(12) == 21);
        REQUIRE(wkey_to_key(13) == 23);
        REQUIRE(wkey_to_key(14) == 24);
    }
}
