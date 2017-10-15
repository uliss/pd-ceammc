/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#include "ceammc_atom.h"
#include "ceammc_atomlist.h"
#include "ceammc_format.h"
#include "ceammc_message.h"

using namespace ceammc;

TEST_CASE("format", "[ceammc::format]")
{
    SECTION("atom format")
    {
        REQUIRE(to_string(Atom(1.1111f)) == "1.1111");
        REQUIRE(to_string(Atom(-3.1415f)) == "-3.1415");
        REQUIRE(to_string(Atom(123)) == "123");
        REQUIRE(to_string(Atom(gensym("1.1"))) == "1.1");

        // float
        REQUIRE(to_float_string(123) == "123");
        REQUIRE(to_float_string(123.12f) == "123.12");
        REQUIRE(to_float_string(gensym("1.1")) == "");

        // hex
        REQUIRE(to_hex_string(255) == "FF");
        REQUIRE(to_hex_string(gensym("255")) == "");

        // float range
        REQUIRE(to_float_range_string(123, -1, 1) == "1");
        REQUIRE(to_float_range_string(-123, -1, 1) == "-1");
        REQUIRE(to_float_range_string(0.5, -1, 1) == "0.5");
        REQUIRE(to_float_range_string(-0.5, -1, 1) == "-0.5");
        REQUIRE(to_float_range_string(-1, -1, 1) == "-1");
        REQUIRE(to_float_range_string(1, -1, 1) == "1");
        REQUIRE(to_float_range_string(gensym("255"), -1, 1) == "");
    }

    SECTION("atomlist format")
    {
        REQUIRE(to_string(AtomList::values(3, 1.f, 2.f, 3.f)) == "1 2 3");
        REQUIRE(to_string(AtomList()) == "");
    }

    SECTION("message format")
    {
        REQUIRE(to_string(Message(-0.45f)) == "-0.45");
        REQUIRE(to_string(Message(gensym("a b c"))) == "a b c");
        REQUIRE(to_string(Message(AtomList::ones(2))) == "1 1");
        REQUIRE(to_string(Message()) == "");
        REQUIRE(to_string(Message(gensym("a"), AtomList::zeroes(1))) == "a 0");
    }
}
