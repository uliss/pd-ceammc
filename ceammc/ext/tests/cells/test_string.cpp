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
#include "ceammc_string.h"

#include <cstring>

using namespace ceammc::string;

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

    SECTION("utf8_substr")
    {
        REQUIRE(utf8_substr("", 0, 0) == "");
        REQUIRE(utf8_substr("TEST", 4, 0) == "");
        REQUIRE(utf8_substr("ТЕСТ", 4, 0) == "");

        REQUIRE(utf8_substr("TEST", 5, 0) == "");
        REQUIRE(utf8_substr("ТЕСТ", 5, 0) == "");

        REQUIRE(utf8_substr("TEST", 0, 1) == "T");
        REQUIRE(utf8_substr("TEST", 1, 1) == "E");
        REQUIRE(utf8_substr("TEST", 2, 1) == "S");
        REQUIRE(utf8_substr("TEST", 3, 1) == "T");

        REQUIRE(utf8_substr("TEST", 0, 2) == "TE");
        REQUIRE(utf8_substr("TEST", 1, 2) == "ES");
        REQUIRE(utf8_substr("TEST", 2, 2) == "ST");
        REQUIRE(utf8_substr("TEST", 3, 2) == "T");

        REQUIRE(utf8_substr("TEST", 0, 3) == "TES");
        REQUIRE(utf8_substr("TEST", 1, 3) == "EST");
        REQUIRE(utf8_substr("TEST", 2, 3) == "ST");
        REQUIRE(utf8_substr("TEST", 3, 3) == "T");

        REQUIRE(utf8_substr("TEST", 0, 4) == "TEST");
        REQUIRE(utf8_substr("TEST", 1, 4) == "EST");
        REQUIRE(utf8_substr("TEST", 2, 4) == "ST");
        REQUIRE(utf8_substr("TEST", 3, 4) == "T");

        REQUIRE(utf8_substr("TEST", 0, 5) == "TEST");
        REQUIRE(utf8_substr("TEST", 1, 5) == "EST");
        REQUIRE(utf8_substr("TEST", 2, 5) == "ST");
        REQUIRE(utf8_substr("TEST", 3, 5) == "T");

        REQUIRE(utf8_substr("TEST", -4, 0) == "");
        REQUIRE(utf8_substr("TEST", -3, 0) == "");
        REQUIRE(utf8_substr("TEST", -2, 0) == "");
        REQUIRE(utf8_substr("TEST", -1, 0) == "");

        REQUIRE(utf8_substr("TEST", -4, 1) == "T");
        REQUIRE(utf8_substr("TEST", -3, 1) == "E");
        REQUIRE(utf8_substr("TEST", -2, 1) == "S");
        REQUIRE(utf8_substr("TEST", -1, 1) == "T");

        REQUIRE(utf8_substr("TEST", -4, 2) == "TE");
        REQUIRE(utf8_substr("TEST", -3, 2) == "ES");
        REQUIRE(utf8_substr("TEST", -2, 2) == "ST");
        REQUIRE(utf8_substr("TEST", -1, 2) == "T");

        REQUIRE(utf8_substr("TEST", -4, 3) == "TES");
        REQUIRE(utf8_substr("TEST", -3, 3) == "EST");
        REQUIRE(utf8_substr("TEST", -2, 3) == "ST");
        REQUIRE(utf8_substr("TEST", -1, 3) == "T");

        REQUIRE(utf8_substr("TEST", -4, 4) == "TEST");
        REQUIRE(utf8_substr("TEST", -3, 4) == "EST");
        REQUIRE(utf8_substr("TEST", -2, 4) == "ST");
        REQUIRE(utf8_substr("TEST", -1, 4) == "T");

        REQUIRE(utf8_substr("TEST", -4, 5) == "TEST");
        REQUIRE(utf8_substr("TEST", -3, 5) == "EST");
        REQUIRE(utf8_substr("TEST", -2, 5) == "ST");
        REQUIRE(utf8_substr("TEST", -1, 5) == "T");

        SECTION("utf8")
        {
            REQUIRE(utf8_substr("ТЕСТ", 0, 1) == "Т");
            REQUIRE(utf8_substr("ТЕСТ", 1, 1) == "Е");
            REQUIRE(utf8_substr("ТЕСТ", 2, 1) == "С");
            REQUIRE(utf8_substr("ТЕСТ", 3, 1) == "Т");

            REQUIRE(utf8_substr("ТЕСТ", 0, 2) == "ТЕ");
            REQUIRE(utf8_substr("ТЕСТ", 1, 2) == "ЕС");
            REQUIRE(utf8_substr("ТЕСТ", 2, 2) == "СТ");
            REQUIRE(utf8_substr("ТЕСТ", 3, 2) == "Т");

            REQUIRE(utf8_substr("ТЕСТ", 0, 3) == "ТЕС");
            REQUIRE(utf8_substr("ТЕСТ", 1, 3) == "ЕСТ");
            REQUIRE(utf8_substr("ТЕСТ", 2, 3) == "СТ");
            REQUIRE(utf8_substr("ТЕСТ", 3, 3) == "Т");

            REQUIRE(utf8_substr("ТЕСТ", 0, 4) == "ТЕСТ");
            REQUIRE(utf8_substr("ТЕСТ", 1, 4) == "ЕСТ");
            REQUIRE(utf8_substr("ТЕСТ", 2, 4) == "СТ");
            REQUIRE(utf8_substr("ТЕСТ", 3, 4) == "Т");

            REQUIRE(utf8_substr("ТЕСТ", 0, 5) == "ТЕСТ");
            REQUIRE(utf8_substr("ТЕСТ", 1, 5) == "ЕСТ");
            REQUIRE(utf8_substr("ТЕСТ", 2, 5) == "СТ");
            REQUIRE(utf8_substr("ТЕСТ", 3, 5) == "Т");

            REQUIRE(utf8_substr("ТЕСТ", -4, 0) == "");
            REQUIRE(utf8_substr("ТЕСТ", -3, 0) == "");
            REQUIRE(utf8_substr("ТЕСТ", -2, 0) == "");
            REQUIRE(utf8_substr("ТЕСТ", -1, 0) == "");

            REQUIRE(utf8_substr("ТЕСТ", -4, 1) == "Т");
            REQUIRE(utf8_substr("ТЕСТ", -3, 1) == "Е");
            REQUIRE(utf8_substr("ТЕСТ", -2, 1) == "С");
            REQUIRE(utf8_substr("ТЕСТ", -1, 1) == "Т");

            REQUIRE(utf8_substr("ТЕСТ", -4, 2) == "ТЕ");
            REQUIRE(utf8_substr("ТЕСТ", -3, 2) == "ЕС");
            REQUIRE(utf8_substr("ТЕСТ", -2, 2) == "СТ");
            REQUIRE(utf8_substr("ТЕСТ", -1, 2) == "Т");

            REQUIRE(utf8_substr("ТЕСТ", -4, 3) == "ТЕС");
            REQUIRE(utf8_substr("ТЕСТ", -3, 3) == "ЕСТ");
            REQUIRE(utf8_substr("ТЕСТ", -2, 3) == "СТ");
            REQUIRE(utf8_substr("ТЕСТ", -1, 3) == "Т");

            REQUIRE(utf8_substr("ТЕСТ", -4, 4) == "ТЕСТ");
            REQUIRE(utf8_substr("ТЕСТ", -3, 4) == "ЕСТ");
            REQUIRE(utf8_substr("ТЕСТ", -2, 4) == "СТ");
            REQUIRE(utf8_substr("ТЕСТ", -1, 4) == "Т");

            REQUIRE(utf8_substr("ТЕСТ", -4, 5) == "ТЕСТ");
            REQUIRE(utf8_substr("ТЕСТ", -3, 5) == "ЕСТ");
            REQUIRE(utf8_substr("ТЕСТ", -2, 5) == "СТ");
            REQUIRE(utf8_substr("ТЕСТ", -1, 5) == "Т");
        }
    }

    SECTION("splitByChar")
    {
        std::vector<std::string> chars;
        utf8_split_by_char(chars, "");
        REQUIRE(chars.empty());

        utf8_split_by_char(chars, "abc");
        REQUIRE(chars.size() == 3);
        REQUIRE(chars[0] == "a");
        REQUIRE(chars[1] == "b");
        REQUIRE(chars[2] == "c");

        utf8_split_by_char(chars, "абв 123");
        REQUIRE(chars.size() == 7);
        REQUIRE(chars[0] == "а");
        REQUIRE(chars[1] == "б");
        REQUIRE(chars[2] == "в");
        REQUIRE(chars[3] == " ");
        REQUIRE(chars[4] == "1");
        REQUIRE(chars[5] == "2");
        REQUIRE(chars[6] == "3");

        utf8_split_by_char(chars, "1的Ж");
        REQUIRE(chars.size() == 3);
        REQUIRE(chars[0] == "1");
        REQUIRE(chars[1] == "的");
        REQUIRE(chars[2] == "Ж");
    }

    SECTION("utf8_to_upper/lower")
    {
        REQUIRE(utf8_to_upper("abcde 12345 çå") == "ABCDE 12345 ÇÅ");
        REQUIRE(utf8_to_upper("абвгд") == "АБВГД");
        REQUIRE(utf8_to_upper("ß") == "SS");
        REQUIRE(utf8_to_lower(utf8_to_upper("абвгд").c_str()) == "абвгд");
        REQUIRE(utf8_to_lower(utf8_to_upper("abcde 12345 çå").c_str()) == "abcde 12345 çå");
    }

    SECTION("utf8_insert")
    {
        REQUIRE(utf8_insert("", 0, "") == "");
        REQUIRE(utf8_insert("a", 0, "") == "a");
        REQUIRE(utf8_insert("", 0, "Ф") == "Ф");
        REQUIRE(utf8_insert("абвгд", 0, "АБВ") == "АБВабвгд");
        REQUIRE(utf8_insert("абвгд", 1, "АБВ") == "аАБВбвгд");
        REQUIRE(utf8_insert("абвгд", 2, "АБВ") == "абАБВвгд");
        REQUIRE(utf8_insert("абвгд", 3, "АБВ") == "абвАБВгд");
        REQUIRE(utf8_insert("абвгд", 4, "АБВ") == "абвгАБВд");
        REQUIRE(utf8_insert("абвгд", 5, "АБВ") == "абвгдАБВ");
        REQUIRE(utf8_insert("абвгд", -1, "АБВ") == "абвгдАБВ");
        REQUIRE(utf8_insert("абвгд", -2, "АБВ") == "абвгАБВд");
        REQUIRE(utf8_insert("абвгд", -3, "АБВ") == "абвАБВгд");
        REQUIRE(utf8_insert("абвгд", -4, "АБВ") == "абАБВвгд");
        REQUIRE(utf8_insert("абвгд", -5, "АБВ") == "аАБВбвгд");
        REQUIRE(utf8_insert("абвгд", -6, "АБВ") == "АБВабвгд");
        REQUIRE_THROWS(utf8_insert("абвгд", 6, "АБВ"), std::exception());
        REQUIRE_THROWS(utf8_insert("абвгд", -7, "АБВ"), std::exception());
    }

    SECTION("starts_with")
    {
        REQUIRE(starts_with("abc", ""));
        REQUIRE(starts_with("abc", "a"));
        REQUIRE(starts_with("abc", "ab"));
        REQUIRE(starts_with("abc", "abc"));
        REQUIRE_FALSE(starts_with("abc", "abcd"));
        REQUIRE_FALSE(starts_with("abc", "b"));

        // utf8
        REQUIRE(starts_with("абв", ""));
        REQUIRE(starts_with("абв", "а"));
        REQUIRE(starts_with("абв", "аб"));
        REQUIRE(starts_with("абв", "абв"));
        REQUIRE_FALSE(starts_with("абв", "абвг"));
        REQUIRE_FALSE(starts_with("абв", "б"));
    }

    SECTION("ends_with")
    {
        REQUIRE(ends_with("file.wav", ".wav"));
        REQUIRE(ends_with("abc", ""));
        REQUIRE(ends_with("abc", "c"));
        REQUIRE(ends_with("abc", "bc"));
        REQUIRE(ends_with("abc", "abc"));
        REQUIRE_FALSE(ends_with("abc", "ab"));
        REQUIRE_FALSE(ends_with("abc", "b"));

        // utf8
        REQUIRE(ends_with("береза", ""));
        REQUIRE(ends_with("береза", "еза"));
        REQUIRE(ends_with("береза", "береза"));
        REQUIRE_FALSE(ends_with("береза", "абв"));
    }

    SECTION("contains")
    {
        REQUIRE(contains("abc", ""));
        REQUIRE(contains("abc", "a"));
        REQUIRE(contains("abc", "b"));
        REQUIRE(contains("abcd", "bc"));
        REQUIRE(contains("", ""));
        REQUIRE_FALSE(contains("", "abc"));
        REQUIRE_FALSE(contains("asdab", "abc"));

        // utf8
        REQUIRE(contains("абв", ""));
        REQUIRE(contains("абв", "а"));
        REQUIRE(contains("абв", "аб"));
        REQUIRE(contains("абвг", "бв"));
        REQUIRE_FALSE(contains("", "абв"));
        REQUIRE_FALSE(contains("вапьты", "абв"));
    }

    SECTION("pd_string_match")
    {
        std::string str;
        REQUIRE(pd_string_match("\"\"", str));
        REQUIRE(str == "");
        REQUIRE(pd_string_match("\" \"", str));
        REQUIRE(str == " ");
        REQUIRE(pd_string_match("\"wasn`\"t\"", str));
        REQUIRE(str == "wasn`\"t");
        REQUIRE(pd_string_match("\"`\"a b c`\"\"", str));
        REQUIRE(str == "`\"a b c`\"");
        REQUIRE_FALSE(pd_string_match(R"("""")", str));
        REQUIRE(pd_string_match(R"("`"`"")", str));
        REQUIRE(pd_string_match(R"("`.")", str));
        REQUIRE(pd_string_match(R"("`:")", str));
        REQUIRE(pd_string_match(R"("`(")", str));
        REQUIRE(pd_string_match(R"("`/")", str));
        REQUIRE(pd_string_match(R"("``")", str));
        REQUIRE_FALSE(pd_string_match(R"("```")", str));
        REQUIRE_FALSE(pd_string_match(R"("`n")", str));
        REQUIRE_FALSE(pd_string_match(R"("`"``"")", str));
    }

    SECTION("pd_string_unescape")
    {
        REQUIRE(pd_string_unescape("") == "");
        REQUIRE(pd_string_unescape(" ") == " ");
        REQUIRE(pd_string_unescape("wasn't") == "wasn't");
        REQUIRE(pd_string_unescape("`\"") == "\"");
        REQUIRE(pd_string_unescape("`'") == "'");
        REQUIRE(pd_string_unescape("``'") == "`'");
        REQUIRE(pd_string_unescape("``") == "`");
        REQUIRE(pd_string_unescape("````") == "``");
        REQUIRE(pd_string_unescape("`(") == "{");
        REQUIRE(pd_string_unescape("`)") == "}");
        REQUIRE(pd_string_unescape("`.") == ",");
        REQUIRE(pd_string_unescape("`:") == ";");
        REQUIRE(pd_string_unescape("'") == "'");
        REQUIRE(pd_string_unescape("'") == "'");
        REQUIRE(pd_string_unescape(" `\" ") == " \" ");
    }

    SECTION("pd_string_parse")
    {
        std::string str;
        REQUIRE_FALSE(pd_string_parse("''", str));
        REQUIRE(pd_string_parse("\"\"", str));
        REQUIRE(str == "");
        REQUIRE_FALSE(pd_string_parse("' '", str));
        REQUIRE(pd_string_parse("\" \"", str));
        REQUIRE(str == " ");
        REQUIRE_FALSE(pd_string_parse("wasn't", str));
        REQUIRE(pd_string_parse("\"wasn't\"", str));
        REQUIRE(str == "wasn't");
        REQUIRE(pd_string_parse("\"a`.b`.c\"", str));
        REQUIRE(str == "a,b,c");

        // check self-reference passing
        std::string sp("\" \"");
        REQUIRE(pd_string_parse(sp, sp));
        REQUIRE(sp == " ");
    }

    SECTION("is_pd_string")
    {
        REQUIRE_FALSE(is_pd_string(""));
        REQUIRE_FALSE(is_pd_string("\""));
        REQUIRE_FALSE(is_pd_string("abc"));
        REQUIRE_FALSE(is_pd_string("123"));
        REQUIRE_FALSE(is_pd_string("\"123"));
        REQUIRE_FALSE(is_pd_string("123\""));
        REQUIRE_FALSE(is_pd_string("\"`\""));

        REQUIRE(is_pd_string("\"\""));
        REQUIRE(is_pd_string("\" \""));
        REQUIRE(is_pd_string("\"123\""));
        REQUIRE(is_pd_string("\"a b\""));
        REQUIRE(is_pd_string("\"a``\""));
        REQUIRE(is_pd_string("\"`\" asb `\"\""));
        REQUIRE(is_pd_string("\"```\"`/`:`.\""));
    }
}
