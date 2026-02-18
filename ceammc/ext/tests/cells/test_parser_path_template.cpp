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

#define CHECK_CONTAINS(path, type, b, l)                                               \
    {                                                                                  \
        REQUIRE(path_get_dir_template(path, &begin, &end) == DirectoryTemplate::type); \
        REQUIRE(begin == &path[b]);                                                    \
        REQUIRE(end == &path[b + l]);                                                  \
    }

TEST_CASE("parser_path_template", "[ceammc::parser]")
{
    test::pdPrintToStdError();

    SECTION("contains")
    {
        const char* begin = 0;
        const char* end = 0;
        DirectoryTemplate t;

        REQUIRE(path_get_dir_template("") == DirectoryTemplate::None);
        REQUIRE(path_get_dir_template("", &begin, &end) == DirectoryTemplate::None);
        REQUIRE(path_get_dir_template("file://") == DirectoryTemplate::None);
        REQUIRE(path_get_dir_template("/some/path") == DirectoryTemplate::None);
        REQUIRE(path_get_dir_template("/some/%HOME%") == DirectoryTemplate::None);
        REQUIRE(path_get_dir_template("HOME%") == DirectoryTemplate::None);
        REQUIRE(path_get_dir_template("%HOME") == DirectoryTemplate::None);
        CHECK_CONTAINS("%HOME%", Home, 0, 6);
        CHECK_CONTAINS("file://%HOME%", Home, 7, 6);
        CHECK_CONTAINS("file://%HOME%/path", Home, 7, 6);
        CHECK_CONTAINS("file://%HOME%/path/path/path", Home, 7, 6);
        CHECK_CONTAINS("~/path", Home, 0, 1);
        CHECK_CONTAINS("~", Home, 0, 1);
        CHECK_CONTAINS("%MUSIC%", Audio, 0, 7);
        CHECK_CONTAINS("%VIDEO%", Video, 0, 7);
        CHECK_CONTAINS("%MOVIE%", Video, 0, 7);
        CHECK_CONTAINS("%MOVIES%", Video, 0, 8);
        CHECK_CONTAINS("%DESKTOP%", Desktop, 0, 9);
        CHECK_CONTAINS("%IMAGE%", Picture, 0, 7);
        CHECK_CONTAINS("%PICTURE%", Picture, 0, 9);

        CHECK_CONTAINS("file://%MUSIC%", Audio, 7, 7);
        CHECK_CONTAINS("file://%VIDEO%", Video, 7, 7);
        CHECK_CONTAINS("file://%MOVIE%", Video, 7, 7);
        CHECK_CONTAINS("file://%MOVIES%", Video, 7, 8);
        CHECK_CONTAINS("file://%DESKTOP%", Desktop, 7, 9);
        CHECK_CONTAINS("file://%IMAGE%", Picture, 7, 7);
        CHECK_CONTAINS("file://%PICTURE%", Picture, 7, 9);
    }
}
