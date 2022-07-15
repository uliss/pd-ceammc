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
#include "ceammc_atomlist.h"
#include "ceammc_data.h"
#include "ceammc_string.h"
#include "datatype_mlist.h"
#include "lex/parser_strings.h"
#include "test_common.h"
#include "test_datatypes.h"

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
        REQUIRE_FALSE(is_quoted_string((char*)0));
        REQUIRE_FALSE(is_quoted_string((t_symbol*)0));
        REQUIRE_FALSE(is_quoted_string(""));
        REQUIRE_FALSE(is_quoted_string("\""));
        REQUIRE_FALSE(is_quoted_string("abc"));
        REQUIRE_FALSE(is_quoted_string("123"));
        REQUIRE_FALSE(is_quoted_string("\"123"));
        REQUIRE_FALSE(is_quoted_string("123\""));
        REQUIRE_FALSE(is_quoted_string("\"`\""));

        REQUIRE(is_quoted_string("\"\""));
        REQUIRE(is_quoted_string("\" \""));
        REQUIRE(is_quoted_string("\"123\""));
        REQUIRE(is_quoted_string("\"a b\""));
        REQUIRE(is_quoted_string("\"А Б В Г Д\""));
        REQUIRE(is_quoted_string("\"👽👾🤖🎃\""));
        REQUIRE(is_quoted_string("\"a``\""));
        REQUIRE(is_quoted_string("\"`\" asb `\"\""));
        REQUIRE(is_quoted_string("\"```\"`/`:`.\""));

        REQUIRE_FALSE(is_quoted_string(L()));
        REQUIRE_FALSE(is_quoted_string(LF(1)));
        REQUIRE_FALSE(is_quoted_string(LF(1, 2)));
        REQUIRE_FALSE(is_quoted_string(LA("A")));
        REQUIRE(is_quoted_string(LA("\"\"")));
        REQUIRE(is_quoted_string(LA("\"a b c\"")));
        REQUIRE(is_quoted_string(AtomList::parseString("\"a\"")));
        REQUIRE(is_quoted_string(AtomList::parseString("\"a b c\"")));
        REQUIRE_FALSE(is_quoted_string(AtomList::parseString("\"a b c")));
        REQUIRE_FALSE(is_quoted_string(AtomList::parseString("a b c\"")));
    }

    SECTION("data_string_end")
    {
        REQUIRE_FALSE(quoted_string_end((char*)nullptr));
        REQUIRE_FALSE(quoted_string_end((t_symbol*)nullptr));
        REQUIRE_FALSE(quoted_string_end(""));
        REQUIRE_FALSE(quoted_string_end("`"));
        REQUIRE_FALSE(quoted_string_end("``"));
        REQUIRE_FALSE(quoted_string_end("`\""));
        REQUIRE(quoted_string_end("\""));
        REQUIRE(quoted_string_end("abcd\""));
        REQUIRE(quoted_string_end("abcd@\""));
        REQUIRE(quoted_string_end("abcd`@\""));
        REQUIRE(quoted_string_end("абвгд\""));
        REQUIRE(quoted_string_end("`.``(`)\""));
        REQUIRE(quoted_string_end("``\""));
    }

    SECTION("string_need_quotes")
    {
        REQUIRE_FALSE(string_need_quotes((char*)nullptr));
        REQUIRE_FALSE(string_need_quotes(""));
        REQUIRE_FALSE(string_need_quotes("a"));
        REQUIRE_FALSE(string_need_quotes("1"));
        REQUIRE_FALSE(string_need_quotes("abc"));
        REQUIRE_FALSE(string_need_quotes("'"));
        REQUIRE(string_need_quotes(" "));
        REQUIRE(string_need_quotes(","));
        REQUIRE(string_need_quotes(";"));
        REQUIRE(string_need_quotes("\\"));
        REQUIRE(string_need_quotes("\""));
        REQUIRE_FALSE(string_need_quotes("there's"));
        REQUIRE(string_need_quotes("there\"s"));
        REQUIRE(string_need_quotes("the space"));
    }

    SECTION("escape_and_quote")
    {
#define CHECK_ESCAPE_AND_QUOTE(s, res, n)                               \
    {                                                                   \
        StaticString str;                                               \
        CHECK(n == escape_and_quote(s, str));                           \
        CHECK(std::string(str.data(), str.data() + str.size()) == res); \
    }

        CHECK(AtomList::parseString("\\ \\ ") == A("  "));
        CHECK(AtomList::parseString("\\\\") == A("\\"));
        CHECK(AtomList::parseString("\\,") == A(","));
        CHECK(AtomList::parseString("\\;") == A(";"));
        CHECK(AtomList::parseString("   ").empty());
        CHECK(AtomList::parseString(",") == Atom::comma());
        CHECK(AtomList::parseString(";") == Atom::semicolon());
        CHECK(AtomList::parseString("$0")[0].atom().a_type == A_DOLLAR);

        CHECK_ESCAPE_AND_QUOTE("", "\"\"", 0);
        CHECK_ESCAPE_AND_QUOTE(" ", "\" \"", 1);
        CHECK_ESCAPE_AND_QUOTE("  ", "\"  \"", 2);
        CHECK_ESCAPE_AND_QUOTE("a,b,c;", "\"a,b,c;\"", 3);
        CHECK_ESCAPE_AND_QUOTE(R"("quotes")", R"("`"quotes`"")", 2);
        CHECK_ESCAPE_AND_QUOTE("fn()", "\"fn()\"", 2);
        CHECK_ESCAPE_AND_QUOTE("#true", "\"#true\"", 1);
        CHECK_ESCAPE_AND_QUOTE(" @,:;#%", "\" @,:;#%\"", 7);
        CHECK_ESCAPE_AND_QUOTE("Dict[]", R"("Dict[]")", 2);
        CHECK_ESCAPE_AND_QUOTE(R"(abc)", R"("abc")", 0);
        CHECK_ESCAPE_AND_QUOTE(R"(1)", R"("1")", 0);
        CHECK_ESCAPE_AND_QUOTE(R"(`abc`)", R"("``abc``")", 2);
    }

    SECTION("raw_list_to_string")
    {
        SECTION("StaticString")
        {
#define REQUIRE_STATIC_STR(lst, s1)          \
    {                                        \
        string::StaticString str;            \
        raw_list_to_string(lst.view(), str); \
        REQUIRE(str == s1);                  \
    }

            REQUIRE_STATIC_STR(L(), "");
            REQUIRE_STATIC_STR(LF(1), "1");
            REQUIRE_STATIC_STR(LF(1234.25), "1234.25");
            REQUIRE_STATIC_STR(LF(1, 2, -3), "1 2 -3");
            REQUIRE_STATIC_STR(AtomList::parseString(" a  b  c "), "a b c");
            REQUIRE_STATIC_STR(AtomList::parseString("1,2,3;"), "1, 2, 3;");
            REQUIRE_STATIC_STR(LA(Atom()), "#null");
            REQUIRE_STATIC_STR(LA(Atom(), "ABC"), "#null ABC");
        }

        SECTION("SmallString")
        {
#define REQUIRE_SMALL_STR(lst, s1)                          \
    {                                                       \
        string::SmallString str;                            \
        raw_list_to_string(lst.view(), str);                \
        REQUIRE(std::string(str.data(), str.size()) == s1); \
    }

            REQUIRE_SMALL_STR(L(), "");
            REQUIRE_SMALL_STR(LF(1), "1");
            REQUIRE_SMALL_STR(LF(1234.25), "1234.25");
            REQUIRE_SMALL_STR(LF(1, 2, -3), "1 2 -3");
            REQUIRE_SMALL_STR(AtomList::parseString(" a  b  c "), "a b c");
            REQUIRE_SMALL_STR(AtomList::parseString("1,2,3;"), "1, 2, 3;");
            REQUIRE_SMALL_STR(LA(Atom()), "#null");
            REQUIRE_SMALL_STR(LA(Atom(), "ABC"), "#null ABC");
        }

        SECTION("MediumString")
        {
#define REQUIRE_MEDIUM_STR(lst, s1)                         \
    {                                                       \
        string::MediumString str;                           \
        raw_list_to_string(lst.view(), str);                \
        REQUIRE(std::string(str.data(), str.size()) == s1); \
    }

            REQUIRE_MEDIUM_STR(L(), "");
            REQUIRE_MEDIUM_STR(LF(1), "1");
            REQUIRE_MEDIUM_STR(LF(1234.25), "1234.25");
            REQUIRE_MEDIUM_STR(LF(1, 2, -3), "1 2 -3");
            REQUIRE_MEDIUM_STR(AtomList::parseString(" a  b  c "), "a b c");
            REQUIRE_MEDIUM_STR(AtomList::parseString("1,2,3;"), "1, 2, 3;");
            REQUIRE_MEDIUM_STR(LA(Atom()), "#null");
            REQUIRE_MEDIUM_STR(LA(Atom(), "ABC"), "#null ABC");
        }
    }

    SECTION("parsed_atom_to_string")
    {
        SECTION("SmallString")
        {
#define REQUIRE_ATOM_STR(a, str)                             \
    {                                                        \
        SmallString buf;                                     \
        parsed_atom_to_string(a, buf);                       \
        REQUIRE(std::string(buf.data(), buf.size()) == str); \
    }

            using IntA = DataAtom<IntData>;
            using StrA = DataAtom<StrData>;

            REQUIRE_ATOM_STR(Atom(), "#null");
            REQUIRE_ATOM_STR(A(""), "\"\"");
            REQUIRE_ATOM_STR(A(-12.5), "-12.5");
            REQUIRE_ATOM_STR(A("ABC"), "ABC");
            REQUIRE_ATOM_STR(A("pi()"), "\"pi()\"");
            REQUIRE_ATOM_STR(A("A B C"), "\"A B C\"");
            REQUIRE_ATOM_STR(A("(())"), "\"(())\"");
            REQUIRE_ATOM_STR(A(";"), "\";\"");
            REQUIRE_ATOM_STR(A(","), "\",\"");
            REQUIRE_ATOM_STR(A(" "), "\" \"");
            REQUIRE_ATOM_STR(A("`"), "\"``\"");
            REQUIRE_ATOM_STR(A("\""), "\"`\"\"");
            REQUIRE_ATOM_STR(A("#true"), "\"#true\"");
            REQUIRE_ATOM_STR(A("#false"), "\"#false\"");
            REQUIRE_ATOM_STR(A("#null"), "\"#null\"");
            REQUIRE_ATOM_STR(A("[a: 1]"), "\"[a: 1]\"");
            REQUIRE_ATOM_STR(IntA(100), "IntData(100)");
            REQUIRE_ATOM_STR(StrA("100"), "StrData(100)");
            REQUIRE_ATOM_STR(StrA("1 2 3"), "StrData(1 2 3)");
            REQUIRE_ATOM_STR(MListAtom(LF(1, 2, 3)), "(1 2 3)");
            REQUIRE_ATOM_STR(MListAtom(LA("A", "B", "C")), "(A B C)");
            REQUIRE_ATOM_STR(MListAtom(LA("A B C")), "(\"A B C\")");
        }
    }

    SECTION("parsed_atom_to_raw_string")
    {
#define REQUIRE_ATOM_TO_RAW_STRING(a, str) \
    {                                      \
        StaticString buf;                  \
        parsed_atom_to_raw_string(a, out); \
        REQUIRE(out == str);               \
    }
    }

    SECTION("escape_and_quote")
    {
        Atom a;
        a.setComma();

        escape_and_quote(a);
        REQUIRE(a == A(R"(",")"));
        escape_and_quote(a);
        REQUIRE(a == A(R"("`",`"")"));

        a.setSemicolon();
        escape_and_quote(a);
        REQUIRE(a == A(R"(";")"));

        a = A("");
        escape_and_quote(a);
        REQUIRE(a == A(R"("")"));

        a = A(" ");
        escape_and_quote(a);
        REQUIRE(a == A(R"(" ")"));

        a = A("   ");
        escape_and_quote(a);
        REQUIRE(a == A(R"("   ")"));

        REQUIRE(escape_and_quote(A("")) == A("\"\""));
        REQUIRE(escape_and_quote(A(&s_)) == A("\"\""));
        REQUIRE(escape_and_quote(A("#")) == A("\"#\""));
        REQUIRE(escape_and_quote(A("%")) == A("\"%\""));
        REQUIRE(escape_and_quote(A(":")) == A("\":\""));
        REQUIRE(escape_and_quote(A("@prop")) == A(R"("@prop")"));
        REQUIRE(escape_and_quote(A("abc")) == A("abc"));
        REQUIRE(escape_and_quote(A("a b c")) == A(R"("a b c")"));
        REQUIRE(escape_and_quote(A("[")) == A(R"("[")"));
        REQUIRE(escape_and_quote(A("]")) == A(R"("]")"));
        REQUIRE(escape_and_quote(A("(")) == A(R"("(")"));
        REQUIRE(escape_and_quote(A(")")) == A("\")\""));

        REQUIRE(escape_and_quote(L()) == L());
        REQUIRE(escape_and_quote(LF(1, 2, 3)) == LF(1, 2, 3));
        REQUIRE(escape_and_quote(LA(1, "B", "C D", Atom::semicolon())) == LA(1, "B", "\"C D\"", "\";\""));
    }

    SECTION("unquote_and_unescape")
    {
        REQUIRE(unquote_and_unescape(Atom()) == Atom());
        REQUIRE(unquote_and_unescape(A(100)) == A(100));
        REQUIRE(unquote_and_unescape(Atom::comma()) == Atom::comma());
        REQUIRE(unquote_and_unescape(Atom::semicolon()) == Atom::semicolon());
        REQUIRE(unquote_and_unescape(A("")) == A(""));
        REQUIRE(unquote_and_unescape(A(&s_)) == A(&s_));
        REQUIRE(unquote_and_unescape(A(" ")) == A(" "));
        REQUIRE(unquote_and_unescape(A("A B C")) == A("A B C"));
        REQUIRE(unquote_and_unescape(A("\"\"")) == A(""));
        REQUIRE(unquote_and_unescape(A("\" \"")) == A(" "));
        REQUIRE(unquote_and_unescape(A("\"a b c\"")) == A("a b c"));
        REQUIRE(unquote_and_unescape(A("\"``abc``\"")) == A("`abc`"));
        REQUIRE(unquote_and_unescape(A("\"`\"abc`\"\"")) == A("\"abc\""));
        REQUIRE(unquote_and_unescape(A("S\"\"")) == A(""));
        REQUIRE(unquote_and_unescape(A("S\" \"")) == A(" "));
        REQUIRE(unquote_and_unescape(A("S\"a b c\"")) == A("a b c"));
        REQUIRE(unquote_and_unescape(A("S\"``abc``\"")) == A("`abc`"));
        REQUIRE(unquote_and_unescape(A("S\"`\"abc`\"\"")) == A("\"abc\""));
        REQUIRE(unquote_and_unescape(A("\"%HOME%\"")) == A("%HOME%"));
        REQUIRE(unquote_and_unescape(A("\"%HOME%\"")) == A("%HOME%"));
        REQUIRE(unquote_and_unescape(A("abc\"")) == A("abc\""));
        REQUIRE(unquote_and_unescape(A("\"abc")) == A("\"abc"));
        REQUIRE(unquote_and_unescape(A("S\"abc")) == A("S\"abc"));
        REQUIRE(unquote_and_unescape(A("R\"abc\"")) == A("R\"abc\""));
    }
}
