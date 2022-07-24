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
#include "new_file_pattern.h"
#include "path_pattern.h"
#include "test_path_base.h"

#include <ctime>

static std::string make_tm(const char* p)
{
    auto tm = std::time(nullptr);
    char buf[100];
    std::strftime(buf, sizeof(buf), p, std::localtime(&tm));
    return buf;
}

PD_COMPLETE_TEST_SETUP(PathPattern, path, pattern)

using namespace ceammc::path;

TEST_CASE("path.pattern", "[externals]")
{
    pd_test_init();

    SECTION("ragel")
    {
        std::string res;
        REQUIRE(make_new_filename_by_pattern("record-###.wav", res) == PATTERN_DONE);
        REQUIRE(res == "record-001.wav");
        REQUIRE(make_new_filename_by_pattern("record-###.wav", res, 2) == PATTERN_DONE);
        REQUIRE(res == "record-002.wav");
        REQUIRE(make_new_filename_by_pattern("record-##.wav", res, 92) == PATTERN_DONE);
        REQUIRE(res == "record-92.wav");
        REQUIRE(make_new_filename_by_pattern("record-##.wav", res, 99) == PATTERN_DONE);
        REQUIRE(res == "record-99.wav");
        REQUIRE(make_new_filename_by_pattern("record-##.wav", res, 100) == PATTERN_DONE);
        REQUIRE(res == "record-00.wav");
        REQUIRE(make_new_filename_by_pattern("record-#.wav", res, 0) == PATTERN_DONE);
        REQUIRE(res == "record-0.wav");
        REQUIRE(make_new_filename_by_pattern("record-#.wav", res, 10) == PATTERN_DONE);
        REQUIRE(res == "record-0.wav");
        REQUIRE(make_new_filename_by_pattern("record-#.wav", res, 123) == PATTERN_DONE);
        REQUIRE(res == "record-3.wav");
        REQUIRE(make_new_filename_by_pattern("record-#################.wav", res, 123) == PATTERN_LENGTH_OVERFLOW);

        REQUIRE(make_new_filename_by_pattern("rec_%YEAR%-###.wav", res, 321) == PATTERN_DONE);
        REQUIRE(res == make_tm("rec_%Y-321.wav"));
        REQUIRE(make_new_filename_by_pattern("rec_%DATE%-###.wav", res, 321) == PATTERN_DONE);
        REQUIRE(res == make_tm("rec_%Y-%m-%d-321.wav"));
        REQUIRE(make_new_filename_by_pattern("rec_%NOW%-###.wav", res, 321) == PATTERN_DONE);
        REQUIRE(res == make_tm("rec_%Y-%m-%d_%H-%M-%S-321.wav"));
        REQUIRE(make_new_filename_by_pattern("rec_%UNKNOWN%-###.wav", res, 321) == PATTERN_DONE);
        REQUIRE(res == make_tm("rec_%%UNKNOWN%%-321.wav"));
        REQUIRE(make_new_filename_by_pattern("rec_%-###.wav", res, 3210) == PATTERN_DONE);
        REQUIRE(res == "rec_%-210.wav");
        REQUIRE(make_new_filename_by_pattern("rec_%YEAR-###.wav", res, 3210) == PATTERN_DONE);
        REQUIRE(res == "rec_%YEAR-210.wav");
        REQUIRE(make_new_filename_by_pattern("rec_%Y-###.wav", res, 3210) == PATTERN_DONE);
        REQUIRE(res == "rec_%Y-210.wav");
        REQUIRE(make_new_filename_by_pattern("rec_%?-###.wav", res, 3210) == PATTERN_DONE);
        REQUIRE(res == "rec_%?-210.wav");

        REQUIRE(make_new_filename_by_pattern("empty", res) == PATTERN_NOMATCH);
        REQUIRE(res == "empty-001");
        REQUIRE(make_new_filename_by_pattern("empty.txt", res) == PATTERN_NOMATCH);
        REQUIRE(res == "empty-001.txt");
    }

    SECTION("")
    {
        TExt t("path.pattern");

#ifdef __APPLE__
        t << "simple.wav";
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "~/simple.wav");

        t << "simple-##.wav";
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "~/simple-01.wav");

        t << "simple-######.wav";
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "~/simple-000001.wav");

        t << __FILE__;
        std::string fn(__FILE__);
        fn.insert(fn.find_last_of("."), "-001");
        REQUIRE_SYMBOL_AT_OUTLET(0, t, fn.c_str());
#endif
    }
}
