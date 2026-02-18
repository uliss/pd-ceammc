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
#include "lex/parser_path_template.h"
#include "test_base.h"

using namespace ceammc::parser;

TEST_CASE("parser_path_template", "[ceammc::parser]")
{
    test::pdPrintToStdError();

    SECTION("contains")
    {
        REQUIRE_FALSE(path_contains_template(""));
        REQUIRE_FALSE(path_contains_template("file://"));
        REQUIRE_FALSE(path_contains_template("/some/path"));
        REQUIRE_FALSE(path_contains_template("/some/%HOME%"));
        REQUIRE(path_contains_template("%HOME%"));
        REQUIRE(path_contains_template("file://%HOME%"));
        REQUIRE(path_contains_template("file://%HOME%/"));
        REQUIRE(path_contains_template("file://%HOME%/path"));
        REQUIRE(path_contains_template("~/path"));
        REQUIRE(path_contains_template("~"));
        REQUIRE(path_contains_template("%AUDIO%"));
        REQUIRE(path_contains_template("%VIDEO%"));
        REQUIRE(path_contains_template("%MOVIE%"));
        REQUIRE(path_contains_template("%MOVIES%"));
        REQUIRE(path_contains_template("%VIDEO%"));
        REQUIRE(path_contains_template("%DESKTOP%"));
        REQUIRE(path_contains_template("%IMAGE%"));
        REQUIRE(path_contains_template("%PICTURE%"));
    }
}
