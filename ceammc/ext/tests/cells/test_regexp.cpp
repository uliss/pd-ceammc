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
#include "ceammc_regexp.h"
using namespace ceammc;

TEST_CASE("regexp", "[core]")
{
    SECTION("escape")
    {
        REQUIRE(regexp::escape("") == "");
        REQUIRE(regexp::escape("abc") == "abc");
        REQUIRE(regexp::escape("`a") == "\\a");
        REQUIRE(regexp::escape("`a``ABC ") == "\\a`ABC ");
        REQUIRE(regexp::escape("`a`") == "\\a\\");
        REQUIRE(regexp::escape("``") == "`");
        REQUIRE(regexp::escape("`.") == "\\.");
        REQUIRE(regexp::escape("```") == "`\\");
        REQUIRE(regexp::escape("`~") == "\\~");
        REQUIRE(regexp::escape("~") == "~");
        REQUIRE(regexp::escape("`'") == "\\'");
        REQUIRE(regexp::escape("..") == ",");
        REQUIRE(regexp::escape("`:") == ";");
        REQUIRE(regexp::escape("`~(") == "\\~(");
        REQUIRE(regexp::escape("((") == "{");
        REQUIRE(regexp::escape("))") == "}");
        REQUIRE(regexp::escape(")`~") == ")\\~");
        REQUIRE(regexp::escape("~abc~") == "~abc~");
        REQUIRE(regexp::escape("A((2..10))") == "A{2,10}");
        REQUIRE(regexp::escape("(2..10)") == "(2,10)");
        REQUIRE(regexp::escape("(test)string") == "(test)string");
        REQUIRE(regexp::escape("[a-z]((2..3))") == "[a-z]{2,3}");
        REQUIRE(regexp::escape("[a-z]((2..3))`.") == "[a-z]{2,3}\\.");
        REQUIRE(regexp::escape("[a-z]((2..3))`..+") == "[a-z]{2,3}\\..+");
    }

    SECTION("unescape")
    {
        REQUIRE(regexp::unescape("\\a") == "`a");
        REQUIRE(regexp::unescape("") == "");
        REQUIRE(regexp::unescape("abc") == "abc");
        REQUIRE(regexp::unescape("\\a`ABC ") == "`a``ABC ");
        REQUIRE(regexp::unescape("\\a\\") == "`a`");
        REQUIRE(regexp::unescape("`") == "``");
        REQUIRE(regexp::unescape("`\\") == "```");
        REQUIRE(regexp::unescape("~") == "~");
        REQUIRE(regexp::unescape(",") == "..");
        REQUIRE(regexp::unescape(";") == "`:");
        REQUIRE(regexp::unescape("{") == "((");
        REQUIRE(regexp::unescape("}") == "))");
        REQUIRE(regexp::unescape(")~") == ")~");
        REQUIRE(regexp::unescape("~abc~") == "~abc~");
        REQUIRE(regexp::unescape("A{1,2}") == "A((1..2))");
        REQUIRE(regexp::unescape("[a-z]{2,3}\\..+") == "[a-z]((2..3))`..+");
    }
}
