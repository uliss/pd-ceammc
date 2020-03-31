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
#include "ceammc_data.h"
#include "ceammc_format.h"
#include "datatype_string.h"
#include "test_base.h"

#include "catch.hpp"

using namespace ceammc;

TEST_CASE("DataTypeString", "[core]")
{
    SECTION("create")
    {
        DataTypeString t("");
        REQUIRE(t.str() == "");

        REQUIRE(DataTypeString(1.001f).str() == "1.001");
        REQUIRE(DataTypeString(-231.001f).str() == "-231.001");

        REQUIRE(DataTypeString("A B C").str() == "A B C");
        REQUIRE(DataTypeString(std::string("A B C")).str() == "A B C");
        REQUIRE(DataTypeString(gensym("A B C")).str() == "A B C");
        REQUIRE(DataTypeString("A B C").str() == "A B C");
        REQUIRE(DataTypeString(Atom(-200)).str() == "-200");
    }

    SECTION("split")
    {
        DataTypeString t("");
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

    SECTION("set")
    {
        DataTypeString t("");
        t.set(gensym("TEST"));
        REQUIRE(t.str() == "TEST");
    }

    SECTION("remove")
    {
        SECTION("all")
        {
            REQUIRE(DataTypeString("").removeAll("") == DataTypeString(""));
            REQUIRE(DataTypeString("ABC").removeAll("") == DataTypeString("ABC"));
            REQUIRE(DataTypeString("ABC").removeAll("DEF") == DataTypeString("ABC"));
            REQUIRE(DataTypeString("").removeAll("DEF") == DataTypeString(""));
            REQUIRE(DataTypeString("ABABA").removeAll("A") == DataTypeString("BB"));
            REQUIRE(DataTypeString("ABABA").removeAll("AB") == DataTypeString("A"));
            REQUIRE(DataTypeString("ABCBA ABCBA").removeAll("ABC") == DataTypeString("BA BA"));
            REQUIRE(DataTypeString("АБВ").removeAll("А") == DataTypeString("БВ"));
        }

        SECTION("first")
        {
            REQUIRE(DataTypeString("").removeFirst("") == DataTypeString(""));
            REQUIRE(DataTypeString("ABC").removeFirst("") == DataTypeString("ABC"));
            REQUIRE(DataTypeString("ABC").removeFirst("DEF") == DataTypeString("ABC"));
            REQUIRE(DataTypeString("").removeFirst("DEF") == DataTypeString(""));
            REQUIRE(DataTypeString("ABABA").removeFirst("A") == DataTypeString("BABA"));
            REQUIRE(DataTypeString("ABABA").removeFirst("AB") == DataTypeString("ABA"));
            REQUIRE(DataTypeString("ABCBA ABCBA").removeFirst("ABC") == DataTypeString("BA ABCBA"));
            REQUIRE(DataTypeString("АБВ").removeFirst("А") == DataTypeString("БВ"));
        }

        SECTION("last")
        {
            REQUIRE(DataTypeString("").removeLast("") == DataTypeString(""));
            REQUIRE(DataTypeString("ABC").removeLast("") == DataTypeString("ABC"));
            REQUIRE(DataTypeString("ABC").removeLast("DEF") == DataTypeString("ABC"));
            REQUIRE(DataTypeString("").removeLast("DEF") == DataTypeString(""));
            REQUIRE(DataTypeString("ABABA").removeLast("A") == DataTypeString("ABAB"));
            REQUIRE(DataTypeString("ABABA").removeLast("AB") == DataTypeString("ABA"));
            REQUIRE(DataTypeString("ABCBA ABCBA").removeLast("ABC") == DataTypeString("ABCBA BA"));
            REQUIRE(DataTypeString("АБВ").removeLast("А") == DataTypeString("БВ"));
        }
    }

    SECTION("replace")
    {
        SECTION("all")
        {
            REQUIRE(DataTypeString("").replaceAll("", "ABC") == DataTypeString(""));
            REQUIRE(DataTypeString("").replaceAll("", "") == DataTypeString(""));
            REQUIRE(DataTypeString("").replaceAll("ABC", "") == DataTypeString(""));
            REQUIRE(DataTypeString("ABC").replaceAll("", "ABC") == DataTypeString("ABC"));
            REQUIRE(DataTypeString("123454321").replaceAll("1", "...") == DataTypeString("...2345432..."));
        }

        SECTION("first")
        {
            REQUIRE(DataTypeString("").replaceFirst("", "ABC") == DataTypeString(""));
            REQUIRE(DataTypeString("").replaceFirst("", "") == DataTypeString(""));
            REQUIRE(DataTypeString("").replaceFirst("ABC", "") == DataTypeString(""));
            REQUIRE(DataTypeString("ABC").replaceFirst("", "ABC") == DataTypeString("ABC"));
            REQUIRE(DataTypeString("123454321").replaceFirst("1", "...") == DataTypeString("...23454321"));
        }

        SECTION("last")
        {
            REQUIRE(DataTypeString("").replaceLast("", "ABC") == DataTypeString(""));
            REQUIRE(DataTypeString("").replaceLast("", "") == DataTypeString(""));
            REQUIRE(DataTypeString("").replaceLast("ABC", "") == DataTypeString(""));
            REQUIRE(DataTypeString("ABC").replaceLast("", "ABC") == DataTypeString("ABC"));
            REQUIRE(DataTypeString("123454321").replaceLast("1", "...") == DataTypeString("12345432..."));
        }
    }

    SECTION("case")
    {
        SECTION("upper")
        {
            REQUIRE(DataTypeString("").toUpper() == DataTypeString(""));
            REQUIRE(DataTypeString("123 56").toUpper() == DataTypeString("123 56"));
            REQUIRE(DataTypeString("abc").toUpper() == DataTypeString("ABC"));
            REQUIRE(DataTypeString("éfoo").toUpper() == DataTypeString("ÉFOO"));
            REQUIRE(DataTypeString("абвгд").toUpper() == DataTypeString("АБВГД"));
        }

        SECTION("lower")
        {
            REQUIRE(DataTypeString("").toLower() == DataTypeString(""));
            REQUIRE(DataTypeString("123 56").toLower() == DataTypeString("123 56"));
            REQUIRE(DataTypeString("abc").toLower() == DataTypeString("abc"));
            REQUIRE(DataTypeString("ABC").toLower() == DataTypeString("abc"));
            REQUIRE(DataTypeString("ÉFOO").toLower() == DataTypeString("éfoo"));
            REQUIRE(DataTypeString("АБВГД").toLower() == DataTypeString("абвгд"));
        }
    }

    SECTION("substr")
    {
        SECTION("ascii")
        {
            REQUIRE(DataTypeString("TEST STRING").substr(0, 4) == DataTypeString("TEST"));
            REQUIRE(DataTypeString("TEST STRING").substr(0, 0) == DataTypeString(""));

            REQUIRE(DataTypeString("TEST").substr(0, 40) == DataTypeString("TEST"));
            REQUIRE(DataTypeString("TEST").substr(0, 4) == DataTypeString("TEST"));
            REQUIRE(DataTypeString("TEST").substr(1, 40) == DataTypeString("EST"));
            REQUIRE(DataTypeString("TEST").substr(1, 3) == DataTypeString("EST"));
            REQUIRE(DataTypeString("TEST").substr(2, 40) == DataTypeString("ST"));
            REQUIRE(DataTypeString("TEST").substr(2, 2) == DataTypeString("ST"));
            REQUIRE(DataTypeString("TEST").substr(3, 40) == DataTypeString("T"));
            REQUIRE(DataTypeString("TEST").substr(3, 1) == DataTypeString("T"));
            REQUIRE(DataTypeString("TEST").substr(3, 40) == DataTypeString("T"));
            REQUIRE(DataTypeString("TEST").substr(4, 0) == DataTypeString(""));
            REQUIRE(DataTypeString("TEST").substr(4, 1) == DataTypeString(""));

            REQUIRE(DataTypeString("TEST").substr(-1, 40) == DataTypeString("T"));
            REQUIRE(DataTypeString("TEST").substr(-1, 1) == DataTypeString("T"));
            REQUIRE(DataTypeString("TEST").substr(-2, 40) == DataTypeString("ST"));
            REQUIRE(DataTypeString("TEST").substr(-2, 2) == DataTypeString("ST"));
            REQUIRE(DataTypeString("TEST").substr(-3, 40) == DataTypeString("EST"));
            REQUIRE(DataTypeString("TEST").substr(-3, 3) == DataTypeString("EST"));
            REQUIRE(DataTypeString("TEST").substr(-4, 40) == DataTypeString("TEST"));
            REQUIRE(DataTypeString("TEST").substr(-4, 4) == DataTypeString("TEST"));

            REQUIRE(DataTypeString("TEST").substr(-1, 1) == DataTypeString("T"));
            REQUIRE(DataTypeString("TEST").substr(-2, 1) == DataTypeString("S"));
            REQUIRE(DataTypeString("TEST").substr(-3, 1) == DataTypeString("E"));
            REQUIRE(DataTypeString("TEST").substr(-4, 1) == DataTypeString("T"));

            REQUIRE(DataTypeString("TEST").substr(0, 1) == DataTypeString("T"));
            REQUIRE(DataTypeString("TEST").substr(1, 1) == DataTypeString("E"));
            REQUIRE(DataTypeString("TEST").substr(2, 1) == DataTypeString("S"));
            REQUIRE(DataTypeString("TEST").substr(3, 1) == DataTypeString("T"));

            // out of range
            REQUIRE(DataTypeString("TEST").substr(5, 40) == DataTypeString(""));
            REQUIRE(DataTypeString("TEST").substr(5, 4) == DataTypeString(""));
            REQUIRE(DataTypeString("TEST").substr(-5, 40) == DataTypeString(""));
            REQUIRE(DataTypeString("TEST").substr(-5, 4) == DataTypeString(""));
        }

        SECTION("UTF-8")
        {
            REQUIRE(DataTypeString("ТЕСТ").substr(0, 40) == DataTypeString("ТЕСТ"));
            REQUIRE(DataTypeString("ТЕСТ").substr(0, 4) == DataTypeString("ТЕСТ"));
            REQUIRE(DataTypeString("ТЕСТ").substr(1, 40) == DataTypeString("ЕСТ"));
            REQUIRE(DataTypeString("ТЕСТ").substr(1, 3) == DataTypeString("ЕСТ"));
            REQUIRE(DataTypeString("ТЕСТ").substr(2, 40) == DataTypeString("СТ"));
            REQUIRE(DataTypeString("ТЕСТ").substr(2, 2) == DataTypeString("СТ"));
            REQUIRE(DataTypeString("ТЕСТ").substr(3, 40) == DataTypeString("Т"));
            REQUIRE(DataTypeString("ТЕСТ").substr(3, 1) == DataTypeString("Т"));
            REQUIRE(DataTypeString("ТЕСТ").substr(3, 40) == DataTypeString("Т"));
            REQUIRE(DataTypeString("ТЕСТ").substr(4, 0) == DataTypeString(""));
            REQUIRE(DataTypeString("ТЕСТ").substr(4, 1) == DataTypeString(""));

            REQUIRE(DataTypeString("ТЕСТ").substr(-1, 40) == DataTypeString("Т"));
            REQUIRE(DataTypeString("ТЕСТ").substr(-1, 1) == DataTypeString("Т"));
            REQUIRE(DataTypeString("ТЕСТ").substr(-2, 40) == DataTypeString("СТ"));
            REQUIRE(DataTypeString("ТЕСТ").substr(-2, 2) == DataTypeString("СТ"));
            REQUIRE(DataTypeString("ТЕСТ").substr(-3, 40) == DataTypeString("ЕСТ"));
            REQUIRE(DataTypeString("ТЕСТ").substr(-3, 3) == DataTypeString("ЕСТ"));
            REQUIRE(DataTypeString("ТЕСТ").substr(-4, 40) == DataTypeString("ТЕСТ"));
            REQUIRE(DataTypeString("ТЕСТ").substr(-4, 4) == DataTypeString("ТЕСТ"));

            REQUIRE(DataTypeString("ТЕСТ").substr(-1, 1) == DataTypeString("Т"));
            REQUIRE(DataTypeString("ТЕСТ").substr(-2, 1) == DataTypeString("С"));
            REQUIRE(DataTypeString("ТЕСТ").substr(-3, 1) == DataTypeString("Е"));
            REQUIRE(DataTypeString("ТЕСТ").substr(-4, 1) == DataTypeString("Т"));

            REQUIRE(DataTypeString("ТЕСТ").substr(0, 1) == DataTypeString("Т"));
            REQUIRE(DataTypeString("ТЕСТ").substr(1, 1) == DataTypeString("Е"));
            REQUIRE(DataTypeString("ТЕСТ").substr(2, 1) == DataTypeString("С"));
            REQUIRE(DataTypeString("ТЕСТ").substr(3, 1) == DataTypeString("Т"));

            // out of range
            REQUIRE(DataTypeString("ТЕСТ").substr(5, 40) == DataTypeString(""));
            REQUIRE(DataTypeString("ТЕСТ").substr(5, 4) == DataTypeString(""));
            REQUIRE(DataTypeString("ТЕСТ").substr(-5, 40) == DataTypeString(""));
            REQUIRE(DataTypeString("ТЕСТ").substr(-5, 4) == DataTypeString(""));
        }
    }

    SECTION("isEqual")
    {
        DataTypeString s0("ABC");
        DataTypeString s1("TEST");
        DataTypeString s2("ABC");
        REQUIRE(s0 == s0);
        REQUIRE(s0.isEqual(&s0));
        REQUIRE(s0 != s1);
        REQUIRE(!s0.isEqual(&s1));
        REQUIRE(s0 == s2);
        REQUIRE(s0.isEqual(&s2));
        REQUIRE(s0 != s1);
        IntData int123(123);
        REQUIRE(!s0.isEqual(&int123));
    }

    SECTION("json")
    {
        REQUIRE(DataTypeString("simple").valueToJsonString() == "\"simple\"");
        REQUIRE(DataTypeString("a b c").valueToJsonString() == "\"a b c\"");
        REQUIRE(DataTypeString("a\nb").valueToJsonString() == "\"a\\nb\"");
        REQUIRE(DataTypeString("a\tb").valueToJsonString() == "\"a\\tb\"");
        REQUIRE(DataTypeString(R"( a\b )").valueToJsonString() == R"(" a\\b ")");
        REQUIRE(DataTypeString(R"( a"b )").valueToJsonString() == R"(" a\"b ")");
        REQUIRE(DataTypeString(R"( a\"b )").valueToJsonString() == R"(" a\\\"b ")");
    }

    SECTION("StringAtom")
    {
        REQUIRE(StringAtom("abc") == StringAtom("abc"));
        REQUIRE_FALSE(StringAtom("abc") != StringAtom("abc"));
        REQUIRE_FALSE(StringAtom("abc") == StringAtom("ABC"));
        REQUIRE_FALSE(StringAtom(LF(1, 2, 3)) == StringAtom("[1 2 3]"));
    }

    SECTION("Atom")
    {
        Atom s0(new DataTypeString("s0"));
        Atom s1(new DataTypeString("s1"));

        REQUIRE(s0 < s1);

        AtomList l0({ s0, s1, s1, s1, s0 });
        l0.sort();
        REQUIRE(l0 == LA(s0, s0, s1, s1, s1));

        AtomList l1;
        l1 = std::move(l0);
        REQUIRE(l0.empty());
    }

    SECTION("to_string")
    {
        REQUIRE(to_string(Atom(new DataTypeString("a string"))) == "a string");
    }
}
