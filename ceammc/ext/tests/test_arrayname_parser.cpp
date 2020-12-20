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
#include "array_load_pattern.h"
#include "catch.hpp"
#include "lex/array_loader.h"

#include <cstdio>

bool parse_array_string(const std::string& str)
{
    using namespace ceammc;
    ArrayLoader loader;
    loader.setSrcSampleCount(80000);
    loader.setSrcChannels(2);
    bool res = loader.parse(str);
    loader.validateArrays();
    return res;
}

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

        REQUIRE(array_load_parse("1|2|3", &argc, &argv));
        REQUIRE(argc == 3);
        REQUIRE(argv[0] == "1");
        REQUIRE(argv[1] == "2");
        REQUIRE(argv[2] == "3");

        REQUIRE(array_load_parse("word1|word2|word3", &argc, &argv));
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

        REQUIRE(array_load_parse("1|30-32", &argc, &argv));
        REQUIRE(argc == 4);
        REQUIRE(argv[0] == "1");
        REQUIRE(argv[1] == "30");
        REQUIRE(argv[2] == "31");
        REQUIRE(argv[3] == "32");

        REQUIRE(array_load_parse("1|3-5|6", &argc, &argv));
        REQUIRE(argc == 5);
        REQUIRE(argv[0] == "1");
        REQUIRE(argv[1] == "3");
        REQUIRE(argv[2] == "4");
        REQUIRE(argv[3] == "5");
        REQUIRE(argv[4] == "6");

        REQUIRE(array_load_parse("1|5-3|6", &argc, &argv));
        REQUIRE(argc == 5);
        REQUIRE(argv[0] == "1");
        REQUIRE(argv[1] == "5");
        REQUIRE(argv[2] == "4");
        REQUIRE(argv[3] == "3");
        REQUIRE(argv[4] == "6");

        REQUIRE_FALSE(array_load_parse("1||3", &argc, &argv));
        REQUIRE_FALSE(array_load_parse("|3", &argc, &argv));
        REQUIRE_FALSE(array_load_parse("3|", &argc, &argv));
        REQUIRE_FALSE(array_load_parse("-3", &argc, &argv));
        REQUIRE_FALSE(array_load_parse("3-", &argc, &argv));
        REQUIRE_FALSE(array_load_parse("3--56", &argc, &argv));

        REQUIRE(array_load_parse("1|22-24|155|101-103", &argc, &argv));
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

    SECTION("array_guess")
    {
        std::vector<std::string> res;
        res = array_pattern_names("no-pattern");
        REQUIRE(res.empty());

        res = array_pattern_names("bad pattern: [");
        REQUIRE(res.empty());

        res = array_pattern_names("");
        REQUIRE(res.empty());

        res = array_pattern_names("bad pattern: ]");
        REQUIRE(res.empty());

        res = array_pattern_names("bad pattern: ][");
        REQUIRE(res.empty());

        res = array_pattern_names("][ bad pattern");
        REQUIRE(res.empty());

        res = array_pattern_names("[1||]");
        REQUIRE(res.empty());

        res = array_pattern_names("[1|2|]");
        REQUIRE(res.empty());

        res = array_pattern_names("[-30]");
        REQUIRE(res.empty());

        res = array_pattern_names("[a-z]");
        REQUIRE(res.empty());

        res = array_pattern_names("[-z]");
        REQUIRE(res.empty());
        res = array_pattern_names("[1-z]");
        REQUIRE(res.empty());

        res = array_pattern_names("[x-]");
        REQUIRE(res.empty());
        res = array_pattern_names("[x-1]");
        REQUIRE(res.empty());

        res = array_pattern_names("[|]");
        REQUIRE(res.empty());

        res = array_pattern_names("[]");
        REQUIRE(res.size() == 8);
        REQUIRE(res[0] == "1");
        REQUIRE(res[1] == "2");
        REQUIRE(res[2] == "3");
        REQUIRE(res[3] == "4");
        REQUIRE(res[4] == "5");
        REQUIRE(res[5] == "6");
        REQUIRE(res[6] == "7");
        REQUIRE(res[7] == "8");

        res = array_pattern_names("pref_[]_suf");
        REQUIRE(res.size() == 8);
        REQUIRE(res[0] == "pref_1_suf");
        REQUIRE(res[1] == "pref_2_suf");
        REQUIRE(res[2] == "pref_3_suf");
        REQUIRE(res[3] == "pref_4_suf");
        REQUIRE(res[4] == "pref_5_suf");
        REQUIRE(res[5] == "pref_6_suf");
        REQUIRE(res[6] == "pref_7_suf");
        REQUIRE(res[7] == "pref_8_suf");

        res = array_pattern_names("array[1|2]");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "array1");
        REQUIRE(res[1] == "array2");

        res = array_pattern_names("array[L|R|C]");
        REQUIRE(res.size() == 3);
        REQUIRE(res[0] == "arrayL");
        REQUIRE(res[1] == "arrayR");
        REQUIRE(res[2] == "arrayC");

        res = array_pattern_names("array[10-12]");
        REQUIRE(res.size() == 3);
        REQUIRE(res[0] == "array10");
        REQUIRE(res[1] == "array11");
        REQUIRE(res[2] == "array12");

        res = array_pattern_names("array[12-10]");
        REQUIRE(res.size() == 3);
        REQUIRE(res[0] == "array12");
        REQUIRE(res[1] == "array11");
        REQUIRE(res[2] == "array10");

        res = array_pattern_names("array[0]");
        REQUIRE(res[0] == "array0");
        REQUIRE(res.size() == 1);

        res = array_pattern_names("array[0-1]");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "array0");
        REQUIRE(res[1] == "array1");

        res = array_pattern_names("array[1-0]");
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "array1");
        REQUIRE(res[1] == "array0");
    }

    SECTION("lexer")
    {
        REQUIRE(parse_array_string("to array_[]"));
        REQUIRE(parse_array_string("to array_[12-17]"));
        REQUIRE(parse_array_string("to array_[l|r]"));
        REQUIRE(parse_array_string("to array_[left|center|right]"));
        REQUIRE(parse_array_string("to array_[0|1|10]"));
        REQUIRE(parse_array_string("to array_[0|1|9|13] @r"));
        REQUIRE(parse_array_string("to array_[0|1|9|13] @resize @offset 1ms"));
        REQUIRE(parse_array_string("to array_[0|1|9|13] @resize @begin 0.75 sec"));
        REQUIRE(parse_array_string("to array_[0|1|9|13] @offset 00:01"));
        REQUIRE(parse_array_string("to array_[0|1|9|13] @offset 00:00:01.01"));
        REQUIRE(parse_array_string("to array_[0|1|9|13] @l 150 ms"));
        REQUIRE(parse_array_string("to []_pat @r @g 0.5"));
        REQUIRE(parse_array_string("to array_[0-3]_gr @r @g -0.5db @resample 48000"));
        REQUIRE(parse_array_string("to a1 a2 @n"));
        REQUIRE(parse_array_string("to array_[0|1|9|13] @b 00:01 @l 30 sec"));
        REQUIRE(parse_array_string("to a1 @ch 1"));
        REQUIRE(parse_array_string("to a1 a2 @ch 1 2"));
        REQUIRE(parse_array_string("to a[1-4] @ch 2-4 1"));
        REQUIRE(parse_array_string("to \"array[1]\""));
        REQUIRE(parse_array_string("to \"@array[1]\""));
        REQUIRE(parse_array_string("@to a1 a2 @ch 4-1"));
        REQUIRE(parse_array_string("@to a1 @resample 2/1"));
        REQUIRE(parse_array_string("@to a1 @resample 48000/44100"));
        REQUIRE(parse_array_string("@to a1 @resample 24000"));
        REQUIRE(parse_array_string("@to a1 @b 1000"));
        REQUIRE(parse_array_string("@to a1 @b 1 sec"));
        REQUIRE(parse_array_string("@to a1 @b 1sec"));
        REQUIRE(parse_array_string("@to a1 @b 1s"));
        REQUIRE(parse_array_string("@to a1 @b 1 ms"));
        REQUIRE(parse_array_string("@to a1 @b 100ms"));
        REQUIRE(parse_array_string("@to a1 @b 0.5s"));
        REQUIRE(parse_array_string("@to a1 @b $-0.5s"));
        REQUIRE(parse_array_string("@to a1 @b $-1000"));
        REQUIRE(parse_array_string("@to a1 @b 00:00:01.15"));
        REQUIRE(parse_array_string("@to a1 @b 00:00:01"));
        REQUIRE(parse_array_string("@to a1 @b 00:01"));
        REQUIRE(parse_array_string("@to a1 @b 00:00.15"));
    }
}
