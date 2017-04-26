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

#include "catch.hpp"
#include "ceammc.hpp"
#include "ceammc_string.h"

#include <cstring>

using namespace ceammc::string;

// strange link fix: do not remove!
static t_symbol* t = gensym("test");

TEST_CASE("ceammc_string", "[PureData]")
{
    SECTION("utf8_strlen")
    {
        REQUIRE(utf8_strlen("") == 0);
        REQUIRE(utf8_strlen("a") == 1);
        REQUIRE(utf8_strlen("abcd") == 4);

        REQUIRE(utf8_strlen("ф") == 1);
        REQUIRE(utf8_strlen("jй") == 2);
        REQUIRE(utf8_strlen("моцарт") == 6);
        REQUIRE(strlen("的") == 3);
        REQUIRE(utf8_strlen("的") == 1);
        REQUIRE(utf8_strlen("的的的 的的") == 6);

        std::string s;
        for (size_t i = 0; i < 200; i++)
            s += "jй";

        REQUIRE(utf8_strlen(s.c_str()) == 400);
        REQUIRE(s.length() == 600);
    }
}
