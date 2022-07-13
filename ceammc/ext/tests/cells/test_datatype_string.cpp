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
#include "ceammc_datastorage.h"
#include "ceammc_format.h"
#include "datatype_string.h"
#include "test_base.h"

#include "catch.hpp"

using namespace ceammc;
using DSTR = DataTypeString;

TEST_CASE("DataTypeString", "[core]")
{
    test::pdPrintToStdError();

    SECTION("create")
    {
        DSTR t("");
        REQUIRE(t.typeName() == "String");
        REQUIRE(t.str() == "");

        REQUIRE(DSTR(1.001f).str() == "1.001");
        REQUIRE(DSTR(-231.001f).str() == "-231.001");

        REQUIRE(DSTR("A B C").str() == "A B C");
        REQUIRE(DSTR(std::string("A B C")).str() == "A B C");
        REQUIRE(DSTR(gensym("A B C")).str() == "A B C");
        REQUIRE(DSTR("A B C").str() == "A B C");
        REQUIRE(DSTR(Atom(-200)).str() == "-200");
    }

    SECTION("copy")
    {
        DSTR s0("s0");
        DSTR s1 = s0;
        REQUIRE(s1 == s0);
        REQUIRE_FALSE(s1 != s0);
        REQUIRE(s1.str() == "s0");

        DSTR s2("");
        s2 = s1;
        REQUIRE(s2.str() == "s0");
    }

    SECTION("move")
    {
        DSTR s0("s0");
        DSTR s1 = std::move(s0);
        REQUIRE(s1 != s0);
        REQUIRE_FALSE(s1 == s0);
        REQUIRE(s1.str() == "s0");
        REQUIRE(s0.str() == "");

        DSTR s2("");
        s2 = std::move(s1);
        REQUIRE(s2.str() == "s0");
        REQUIRE(s1.str() == "");
        REQUIRE(s0.str() == "");
    }

    SECTION("clear")
    {
        DSTR s("абвгд");
        REQUIRE(s.length() == 5);
        s.clear();
        REQUIRE(s.length() == 0);
        REQUIRE(s.str() == "");
    }

    SECTION("split")
    {
        DSTR t("");
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
        DSTR t("");
        t.set(gensym("TEST"));
        REQUIRE(t.str() == "TEST");
    }

    SECTION("remove")
    {
        SECTION("all")
        {
            REQUIRE(DSTR("").removeAll("") == DSTR(""));
            REQUIRE(DSTR("ABC").removeAll("") == DSTR("ABC"));
            REQUIRE(DSTR("ABC").removeAll("DEF") == DSTR("ABC"));
            REQUIRE(DSTR("").removeAll("DEF") == DSTR(""));
            REQUIRE(DSTR("ABABA").removeAll("A") == DSTR("BB"));
            REQUIRE(DSTR("ABABA").removeAll("AB") == DSTR("A"));
            REQUIRE(DSTR("ABCBA ABCBA").removeAll("ABC") == DSTR("BA BA"));
            REQUIRE(DSTR("АБВ").removeAll("А") == DSTR("БВ"));
        }

        SECTION("first")
        {
            REQUIRE(DSTR("").removeFirst("") == DSTR(""));
            REQUIRE(DSTR("ABC").removeFirst("") == DSTR("ABC"));
            REQUIRE(DSTR("ABC").removeFirst("DEF") == DSTR("ABC"));
            REQUIRE(DSTR("").removeFirst("DEF") == DSTR(""));
            REQUIRE(DSTR("ABABA").removeFirst("A") == DSTR("BABA"));
            REQUIRE(DSTR("ABABA").removeFirst("AB") == DSTR("ABA"));
            REQUIRE(DSTR("ABCBA ABCBA").removeFirst("ABC") == DSTR("BA ABCBA"));
            REQUIRE(DSTR("АБВ").removeFirst("А") == DSTR("БВ"));
        }

        SECTION("last")
        {
            REQUIRE(DSTR("").removeLast("") == DSTR(""));
            REQUIRE(DSTR("ABC").removeLast("") == DSTR("ABC"));
            REQUIRE(DSTR("ABC").removeLast("DEF") == DSTR("ABC"));
            REQUIRE(DSTR("").removeLast("DEF") == DSTR(""));
            REQUIRE(DSTR("ABABA").removeLast("A") == DSTR("ABAB"));
            REQUIRE(DSTR("ABABA").removeLast("AB") == DSTR("ABA"));
            REQUIRE(DSTR("ABCBA ABCBA").removeLast("ABC") == DSTR("ABCBA BA"));
            REQUIRE(DSTR("АБВ").removeLast("А") == DSTR("БВ"));
        }
    }

    SECTION("replace")
    {
        SECTION("all")
        {
            REQUIRE(DSTR("").replaceAll("", "ABC") == DSTR(""));
            REQUIRE(DSTR("").replaceAll("", "") == DSTR(""));
            REQUIRE(DSTR("").replaceAll("ABC", "") == DSTR(""));
            REQUIRE(DSTR("ABC").replaceAll("", "ABC") == DSTR("ABC"));
            REQUIRE(DSTR("123454321").replaceAll("1", "...") == DSTR("...2345432..."));
        }

        SECTION("first")
        {
            REQUIRE(DSTR("").replaceFirst("", "ABC") == DSTR(""));
            REQUIRE(DSTR("").replaceFirst("", "") == DSTR(""));
            REQUIRE(DSTR("").replaceFirst("ABC", "") == DSTR(""));
            REQUIRE(DSTR("ABC").replaceFirst("", "ABC") == DSTR("ABC"));
            REQUIRE(DSTR("123454321").replaceFirst("1", "...") == DSTR("...23454321"));
        }

        SECTION("last")
        {
            REQUIRE(DSTR("").replaceLast("", "ABC") == DSTR(""));
            REQUIRE(DSTR("").replaceLast("", "") == DSTR(""));
            REQUIRE(DSTR("").replaceLast("ABC", "") == DSTR(""));
            REQUIRE(DSTR("ABC").replaceLast("", "ABC") == DSTR("ABC"));
            REQUIRE(DSTR("123454321").replaceLast("1", "...") == DSTR("12345432..."));
        }
    }

    SECTION("case")
    {
        SECTION("upper")
        {
            REQUIRE(DSTR("").toUpper() == DSTR(""));
            REQUIRE(DSTR("123 56").toUpper() == DSTR("123 56"));
            REQUIRE(DSTR("abc").toUpper() == DSTR("ABC"));
            REQUIRE(DSTR("éfoo").toUpper() == DSTR("ÉFOO"));
            REQUIRE(DSTR("абвгд").toUpper() == DSTR("АБВГД"));
        }

        SECTION("lower")
        {
            REQUIRE(DSTR("").toLower() == DSTR(""));
            REQUIRE(DSTR("123 56").toLower() == DSTR("123 56"));
            REQUIRE(DSTR("abc").toLower() == DSTR("abc"));
            REQUIRE(DSTR("ABC").toLower() == DSTR("abc"));
            REQUIRE(DSTR("ÉFOO").toLower() == DSTR("éfoo"));
            REQUIRE(DSTR("АБВГД").toLower() == DSTR("абвгд"));
        }
    }

    SECTION("substr")
    {
        SECTION("ascii")
        {
            REQUIRE(DSTR("TEST DSTRING").substr(0, 4) == DSTR("TEST"));
            REQUIRE(DSTR("TEST DSTRING").substr(0, 0) == DSTR(""));

            REQUIRE(DSTR("TEST").substr(0, 40) == DSTR("TEST"));
            REQUIRE(DSTR("TEST").substr(0, 4) == DSTR("TEST"));
            REQUIRE(DSTR("TEST").substr(1, 40) == DSTR("EST"));
            REQUIRE(DSTR("TEST").substr(1, 3) == DSTR("EST"));
            REQUIRE(DSTR("TEST").substr(2, 40) == DSTR("ST"));
            REQUIRE(DSTR("TEST").substr(2, 2) == DSTR("ST"));
            REQUIRE(DSTR("TEST").substr(3, 40) == DSTR("T"));
            REQUIRE(DSTR("TEST").substr(3, 1) == DSTR("T"));
            REQUIRE(DSTR("TEST").substr(3, 40) == DSTR("T"));
            REQUIRE(DSTR("TEST").substr(4, 0) == DSTR(""));
            REQUIRE(DSTR("TEST").substr(4, 1) == DSTR(""));

            REQUIRE(DSTR("TEST").substr(-1, 40) == DSTR("T"));
            REQUIRE(DSTR("TEST").substr(-1, 1) == DSTR("T"));
            REQUIRE(DSTR("TEST").substr(-2, 40) == DSTR("ST"));
            REQUIRE(DSTR("TEST").substr(-2, 2) == DSTR("ST"));
            REQUIRE(DSTR("TEST").substr(-3, 40) == DSTR("EST"));
            REQUIRE(DSTR("TEST").substr(-3, 3) == DSTR("EST"));
            REQUIRE(DSTR("TEST").substr(-4, 40) == DSTR("TEST"));
            REQUIRE(DSTR("TEST").substr(-4, 4) == DSTR("TEST"));

            REQUIRE(DSTR("TEST").substr(-1, 1) == DSTR("T"));
            REQUIRE(DSTR("TEST").substr(-2, 1) == DSTR("S"));
            REQUIRE(DSTR("TEST").substr(-3, 1) == DSTR("E"));
            REQUIRE(DSTR("TEST").substr(-4, 1) == DSTR("T"));

            REQUIRE(DSTR("TEST").substr(0, 1) == DSTR("T"));
            REQUIRE(DSTR("TEST").substr(1, 1) == DSTR("E"));
            REQUIRE(DSTR("TEST").substr(2, 1) == DSTR("S"));
            REQUIRE(DSTR("TEST").substr(3, 1) == DSTR("T"));

            // out of range
            REQUIRE(DSTR("TEST").substr(5, 40) == DSTR(""));
            REQUIRE(DSTR("TEST").substr(5, 4) == DSTR(""));
            REQUIRE(DSTR("TEST").substr(-5, 40) == DSTR(""));
            REQUIRE(DSTR("TEST").substr(-5, 4) == DSTR(""));
        }

        SECTION("UTF-8")
        {
            REQUIRE(DSTR("ТЕСТ").substr(0, 40) == DSTR("ТЕСТ"));
            REQUIRE(DSTR("ТЕСТ").substr(0, 4) == DSTR("ТЕСТ"));
            REQUIRE(DSTR("ТЕСТ").substr(1, 40) == DSTR("ЕСТ"));
            REQUIRE(DSTR("ТЕСТ").substr(1, 3) == DSTR("ЕСТ"));
            REQUIRE(DSTR("ТЕСТ").substr(2, 40) == DSTR("СТ"));
            REQUIRE(DSTR("ТЕСТ").substr(2, 2) == DSTR("СТ"));
            REQUIRE(DSTR("ТЕСТ").substr(3, 40) == DSTR("Т"));
            REQUIRE(DSTR("ТЕСТ").substr(3, 1) == DSTR("Т"));
            REQUIRE(DSTR("ТЕСТ").substr(3, 40) == DSTR("Т"));
            REQUIRE(DSTR("ТЕСТ").substr(4, 0) == DSTR(""));
            REQUIRE(DSTR("ТЕСТ").substr(4, 1) == DSTR(""));

            REQUIRE(DSTR("ТЕСТ").substr(-1, 40) == DSTR("Т"));
            REQUIRE(DSTR("ТЕСТ").substr(-1, 1) == DSTR("Т"));
            REQUIRE(DSTR("ТЕСТ").substr(-2, 40) == DSTR("СТ"));
            REQUIRE(DSTR("ТЕСТ").substr(-2, 2) == DSTR("СТ"));
            REQUIRE(DSTR("ТЕСТ").substr(-3, 40) == DSTR("ЕСТ"));
            REQUIRE(DSTR("ТЕСТ").substr(-3, 3) == DSTR("ЕСТ"));
            REQUIRE(DSTR("ТЕСТ").substr(-4, 40) == DSTR("ТЕСТ"));
            REQUIRE(DSTR("ТЕСТ").substr(-4, 4) == DSTR("ТЕСТ"));

            REQUIRE(DSTR("ТЕСТ").substr(-1, 1) == DSTR("Т"));
            REQUIRE(DSTR("ТЕСТ").substr(-2, 1) == DSTR("С"));
            REQUIRE(DSTR("ТЕСТ").substr(-3, 1) == DSTR("Е"));
            REQUIRE(DSTR("ТЕСТ").substr(-4, 1) == DSTR("Т"));

            REQUIRE(DSTR("ТЕСТ").substr(0, 1) == DSTR("Т"));
            REQUIRE(DSTR("ТЕСТ").substr(1, 1) == DSTR("Е"));
            REQUIRE(DSTR("ТЕСТ").substr(2, 1) == DSTR("С"));
            REQUIRE(DSTR("ТЕСТ").substr(3, 1) == DSTR("Т"));

            // out of range
            REQUIRE(DSTR("ТЕСТ").substr(5, 40) == DSTR(""));
            REQUIRE(DSTR("ТЕСТ").substr(5, 4) == DSTR(""));
            REQUIRE(DSTR("ТЕСТ").substr(-5, 40) == DSTR(""));
            REQUIRE(DSTR("ТЕСТ").substr(-5, 4) == DSTR(""));
        }
    }

    SECTION("isEqual")
    {
        DSTR s0("ABC");
        DSTR s1("TEST");
        DSTR s2("ABC");
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
        REQUIRE(DSTR("simple").toJsonString() == "\"simple\"");
        REQUIRE(DSTR("a b c").toJsonString() == "\"a b c\"");
        REQUIRE(DSTR("a\nb").toJsonString() == "\"a\\nb\"");
        REQUIRE(DSTR("a\tb").toJsonString() == "\"a\\tb\"");
        REQUIRE(DSTR(R"( a\b )").toJsonString() == R"(" a\\b ")");
        REQUIRE(DSTR(R"( a"b )").toJsonString() == R"(" a\"b ")");
        REQUIRE(DSTR(R"( a\"b )").toJsonString() == R"(" a\\\"b ")");
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
        Atom s0(new DSTR("s0"));
        Atom s1(new DSTR("s1"));

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
        CHECK(to_string(StringAtom("spaceless")) == "S\"spaceless\"");
        CHECK(to_string(StringAtom("")) == "S\"\"");
        CHECK(to_string(StringAtom(" ")) == "S\" \"");
        CHECK(to_string(StringAtom("with spaces")) == "S\"with spaces\"");
        CHECK(to_string(StringAtom("with 'single quotes'")) == "S\"with 'single quotes'\"");
        CHECK(to_string(StringAtom(R"(with "double quotes")")) == R"(S"with `"double quotes`"")");
        CHECK(to_string(StringAtom(R"(with,commas)")) == R"(S"with,commas")");
        CHECK(to_string(StringAtom(R"(with;semicolon)")) == R"(S"with;semicolon")");
        CHECK(to_string(StringAtom(R"(with:)")) == R"(S"with:")");
        CHECK(to_string(StringAtom(R"(with {} braces)")) == R"(S"with {} braces")");
        CHECK(to_string(StringAtom(R"(with @at)")) == R"(S"with @at")");
        CHECK(to_string(StringAtom(R"(with ``)")) == R"(S"with ````")");
        CHECK(to_string(StringAtom(R"(with \ slash)")) == R"(S"with `/ slash")");

        REQUIRE(StringAtom("a b").asData()->toString() == "S\"a b\"");
        REQUIRE(Atom(new DataTypeString("a b c")).asData()->toString() == "S\"a b c\"");
        REQUIRE(to_string(Atom(new DataTypeString("a b c"))) == "S\"a b c\"");
    }

    SECTION("create via factory")
    {
        auto fn = DataStorage::instance().fromListFunction("String");
        REQUIRE(fn);
        Atom pstr = fn(LA("abc"));
        REQUIRE(pstr.isA<DataTypeString>());
        REQUIRE(pstr.asD<DataTypeString>()->str() == "abc");
    }

    SECTION("clone")
    {
        DSTR s("abcd");
        std::unique_ptr<DSTR> p(s.clone());
        REQUIRE(&s != p.get());
        REQUIRE(p->str() == "abcd");
    }

    SECTION("parse")
    {
        REQUIRE(parseDataString("S\"a b c\"") == StringAtom("a b c"));
        REQUIRE(parseDataString("String(a b c)") == StringAtom("a b c"));
        REQUIRE(parseDataString("String(\"a b c\")") == StringAtom("a b c"));
        REQUIRE(parseDataString("String(\"a b\" c)") == StringAtom("a b c"));
    }
}
