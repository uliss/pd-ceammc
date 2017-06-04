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
#include "../string/datastring.h"
#include "base_extension_test.h"

#include "catch.hpp"

using namespace ceammc;

TEST_CASE("DataString", "[external]")
{
    SECTION("create")
    {
        DataString t("");
        REQUIRE(t.str() == "");
    }

    SECTION("split")
    {
        DataString t("");
        std::vector<std::string> tokens;
        t.split(tokens);
        REQUIRE(tokens.empty());
        t.split(tokens, ":");
        REQUIRE(tokens.empty());

        t.set("abc");
        t.split(tokens);
        REQUIRE(tokens.size() == 3);
        REQUIRE(tokens[0] == "a");
        REQUIRE(tokens[1] == "b");
        REQUIRE(tokens[2] == "c");

        t.set("X");
        t.split(tokens);
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0] == "X");

        t.set("");
        t.split(tokens);
        REQUIRE(tokens.size() == 0);

        t.set("1:2:3");
        t.split(tokens, ":");
        REQUIRE(tokens.size() == 3);
        REQUIRE(tokens[0] == "1");
        REQUIRE(tokens[1] == "2");
        REQUIRE(tokens[2] == "3");

        t.set("1:2:3:");
        t.split(tokens, ":");
        REQUIRE(tokens.size() == 3);
        REQUIRE(tokens[0] == "1");
        REQUIRE(tokens[1] == "2");
        REQUIRE(tokens[2] == "3");

        t.set("::1:2:::3::::");
        t.split(tokens, ":");
        REQUIRE(tokens.size() == 3);
        REQUIRE(tokens[0] == "1");
        REQUIRE(tokens[1] == "2");
        REQUIRE(tokens[2] == "3");
    }
}
