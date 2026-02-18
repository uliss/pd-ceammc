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
        REQUIRE(path_get_dir_template("%%") == DirectoryTemplate::None);
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
        CHECK_CONTAINS("%DOC%", Document, 0, 5);
        CHECK_CONTAINS("%DOCS%", Document, 0, 6);
        CHECK_CONTAINS("%DOCUMENT%", Document, 0, 10);
        CHECK_CONTAINS("%DOCUMENTS%", Document, 0, 11);
        CHECK_CONTAINS("%IMAGE%", Picture, 0, 7);
        CHECK_CONTAINS("%PICTURE%", Picture, 0, 9);
        CHECK_CONTAINS("%PICS%", Picture, 0, 6);
        CHECK_CONTAINS("%IMG%", Picture, 0, 5);
        CHECK_CONTAINS("%TMP%", Tmp, 0, 5);

        CHECK_CONTAINS("file://%MUSIC%", Audio, 7, 7);
        CHECK_CONTAINS("file://%VIDEO%", Video, 7, 7);
        CHECK_CONTAINS("file://%MOVIE%", Video, 7, 7);
        CHECK_CONTAINS("file://%MOVIES%", Video, 7, 8);
        CHECK_CONTAINS("file://%DESKTOP%", Desktop, 7, 9);
        CHECK_CONTAINS("file://%IMAGE%", Picture, 7, 7);
        CHECK_CONTAINS("file://%PICTURE%", Picture, 7, 9);

        CHECK_CONTAINS("%ABC%", Unknown, 0, 5);
        CHECK_CONTAINS("%AUDI%", Unknown, 0, 6);
        CHECK_CONTAINS("%HOME1%", Unknown, 0, 7);
    }

    SECTION("contains")
    {
        auto fn = [](DirectoryTemplate t) {
            switch (t) {
            case None:
                return "";
            case Home:
                return "/MYHOME";
            case Audio:
                return "/media/audio";
            case Video:
                return "/media/video";
            case Picture:
                return "/pics";
            case Desktop:
                return "/desktop";
            case Document:
                return "/docs";
            case Download:
                return "/trash";
            case Tmp:
                return "/tmp";
            case Cwd:
                return "/?";
            case Unknown:
                return "???";
            }

            return "";
        };

        REQUIRE(path_dir_template_subst("~", fn) == "/MYHOME");
        REQUIRE(path_dir_template_subst("~/path", fn) == "/MYHOME/path");
        REQUIRE(path_dir_template_subst("~/папка", fn) == "/MYHOME/папка");
        REQUIRE(path_dir_template_subst("file://~/папка", fn) == "file:///MYHOME/папка");
        REQUIRE(path_dir_template_subst("file://%HOME%/папка", fn) == "file:///MYHOME/папка");
        REQUIRE(path_dir_template_subst("%HOME%/папка", fn) == "/MYHOME/папка");
        REQUIRE(path_dir_template_subst("%AUDIO%", fn) == "/media/audio");
        REQUIRE(path_dir_template_subst("%AUDIO%/папка", fn) == "/media/audio/папка");
        REQUIRE(path_dir_template_subst("%MUSIC%/папка", fn) == "/media/audio/папка");
        REQUIRE(path_dir_template_subst("%VIDEO%/папка", fn) == "/media/video/папка");
        REQUIRE(path_dir_template_subst("%MOVIE%/папка", fn) == "/media/video/папка");
        REQUIRE(path_dir_template_subst("%MOVIES%/папка", fn) == "/media/video/папка");
        REQUIRE(path_dir_template_subst("%IMAGE%/папка", fn) == "/pics/папка");
        REQUIRE(path_dir_template_subst("%IMG%/папка", fn) == "/pics/папка");
        REQUIRE(path_dir_template_subst("%PICS%/папка", fn) == "/pics/папка");
        REQUIRE(path_dir_template_subst("%PICTURE%/папка", fn) == "/pics/папка");
        REQUIRE(path_dir_template_subst("%PICTURES%/папка", fn) == "/pics/папка");
        REQUIRE(path_dir_template_subst("%DESKTOP%/папка", fn) == "/desktop/папка");
        REQUIRE(path_dir_template_subst("%DOC%/папка", fn) == "/docs/папка");
        REQUIRE(path_dir_template_subst("%DOCS%/папка", fn) == "/docs/папка");
        REQUIRE(path_dir_template_subst("%DOCUMENT%/папка", fn) == "/docs/папка");
        REQUIRE(path_dir_template_subst("%DOCUMENTS%/папка", fn) == "/docs/папка");
        REQUIRE(path_dir_template_subst("%DOWNLOAD%/папка", fn) == "/trash/папка");
        REQUIRE(path_dir_template_subst("%TMP%/папка", fn) == "/tmp/папка");
        REQUIRE(path_dir_template_subst("%CWD%/папка", fn) == "/?/папка");
    }
}
