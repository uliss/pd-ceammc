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
#define CATCH_CONFIG_MAIN
#include "array_load_pattern.h"
#include "catch.hpp"

TEST_CASE("arrayname_parser", "[arrayname_parser]")
{
    SECTION("arrayname_parser")
    {
        size_t argc = 0;
        std::string* argv = 0;

        REQUIRE(array_load_parse("", &argc, &argv));
        REQUIRE(argc == 0);
        REQUIRE(argv != 0);
        REQUIRE(array_load_parse("1", &argc, &argv));
        REQUIRE(argc == 1);
        REQUIRE(argv[0] == "1");

        REQUIRE(array_load_parse("abc", &argc, &argv));
        REQUIRE(argc == 1);
        REQUIRE(argv[0] == "abc");

        REQUIRE(array_load_parse("100000", &argc, &argv));
        REQUIRE(argc == 1);
        REQUIRE(argv[0] == "100000");

        REQUIRE(array_load_parse("1,2,3", &argc, &argv));
        REQUIRE(argc == 3);
        REQUIRE(argv[0] == "1");
        REQUIRE(argv[1] == "2");
        REQUIRE(argv[2] == "3");

        REQUIRE(array_load_parse("word1,word2,word3", &argc, &argv));
        REQUIRE(argc == 3);
        REQUIRE(argv[0] == "word1");
        REQUIRE(argv[1] == "word2");
        REQUIRE(argv[2] == "word3");

        REQUIRE(array_load_parse("22-24", &argc, &argv));
        REQUIRE(argc == 3);
        REQUIRE(argv[0] == "22");
        REQUIRE(argv[1] == "23");
        REQUIRE(argv[2] == "24");

        REQUIRE_FALSE(array_load_parse("a-d", &argc, &argv));
        REQUIRE(argc == 0);

        REQUIRE(array_load_parse("8-8", &argc, &argv));
        REQUIRE(argc == 1);
        REQUIRE(argv[0] == "8");

        REQUIRE_FALSE(array_load_parse("8-Z", &argc, &argv));
        REQUIRE(argc == 0);

        REQUIRE(array_load_parse("8-6", &argc, &argv));
        REQUIRE(argc == 3);
        REQUIRE(argv[0] == "8");
        REQUIRE(argv[1] == "7");
        REQUIRE(argv[2] == "6");

        REQUIRE_FALSE(array_load_parse("Z-X", &argc, &argv));
        REQUIRE(argc == 0);

        REQUIRE(array_load_parse("1,30-32", &argc, &argv));
        REQUIRE(argc == 4);
        REQUIRE(argv[0] == "1");
        REQUIRE(argv[1] == "30");
        REQUIRE(argv[2] == "31");
        REQUIRE(argv[3] == "32");

        REQUIRE(array_load_parse("1,3-5,6", &argc, &argv));
        REQUIRE(argc == 5);
        REQUIRE(argv[0] == "1");
        REQUIRE(argv[1] == "3");
        REQUIRE(argv[2] == "4");
        REQUIRE(argv[3] == "5");
        REQUIRE(argv[4] == "6");

        REQUIRE(array_load_parse("1,5-3,6", &argc, &argv));
        REQUIRE(argc == 5);
        REQUIRE(argv[0] == "1");
        REQUIRE(argv[1] == "5");
        REQUIRE(argv[2] == "4");
        REQUIRE(argv[3] == "3");
        REQUIRE(argv[4] == "6");

        REQUIRE_FALSE(array_load_parse("1,,3", &argc, &argv));
        REQUIRE_FALSE(array_load_parse(",3", &argc, &argv));
        REQUIRE_FALSE(array_load_parse("3,", &argc, &argv));
        REQUIRE_FALSE(array_load_parse("-3", &argc, &argv));
        REQUIRE_FALSE(array_load_parse("3-", &argc, &argv));
        REQUIRE_FALSE(array_load_parse("3--56", &argc, &argv));

        REQUIRE(array_load_parse("1,22-24,155,101-103", &argc, &argv));
        REQUIRE(argc == 8);
        REQUIRE(argv[0] == "1");
        REQUIRE(argv[1] == "22");
        REQUIRE(argv[2] == "23");
        REQUIRE(argv[3] == "24");
        REQUIRE(argv[4] == "155");
        REQUIRE(argv[5] == "101");
        REQUIRE(argv[6] == "102");
        REQUIRE(argv[7] == "103");

        REQUIRE(array_load_parse("100-100000", &argc, &argv));
        REQUIRE(argc == 64);
        char buf[64];
        for (int i = 0; i < 64; i++) {
            sprintf(buf, "%d", i + 100);
            REQUIRE(argv[i] == buf);
        }
    }
}
