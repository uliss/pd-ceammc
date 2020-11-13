/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "datatype_string.h"
#include "lex/data_string.lexer.h"
#include "lex/data_string.parser.hpp"
#include "test_base.h"
#include "test_common.h"

using namespace ceammc;

using ML = DataTypeMList;
using DD = DataTypeDict;
using DS = DataTypeString;

static AtomList parse(const char* str)
{
    AtomList res;
    DataStringLexer l(str);
    DataStringParser p(l, res);
    if (p.parse() != 0)
        res.clear();

    return res;
}

TEST_CASE("DataStringParser", "[core]")
{
    test::pdPrintToStdError();
    IntData int100(100);

    SECTION("basic atom types")
    {
        REQUIRE(parse("") == L());
        REQUIRE(parse("null") == Atom());
        REQUIRE(parse("true") == Atom(1));
        REQUIRE(parse("false") == Atom(0.f));
        REQUIRE(parse("abc") == LA("abc"));
        REQUIRE(parse("@prop") == LA("@prop"));
        REQUIRE(parse("what's up?") == LA("what's", "up?"));
        REQUIRE(parse("@prop?") == LA("@prop?"));
        REQUIRE(parse("a==b") == LA("a==b"));
        // no unicode in symbols
        REQUIRE(parse("абвгд") == L());
        // first char should not be &
        REQUIRE(parse("&abcd") == L());
        REQUIRE(parse("100") == LF(100));
        REQUIRE(parse("-100") == LF(-100));
        REQUIRE(parse("100.5") == LF(100.5));
        REQUIRE(parse("-100.5") == LF(-100.5));
        REQUIRE(parse("0xff") == LF(0xFF));
        REQUIRE(parse("-0xff") == LF(-255));
        REQUIRE(parse("0x1A") == LF(0x1A));
        REQUIRE(parse("0b000101") == LF(5));
        REQUIRE(parse("0b11111111") == LF(0xFF));
        REQUIRE(parse("0xffffffffffffffff") == L());
        REQUIRE(parse("\"a b c\"") == LA("a b c"));
        REQUIRE(parse("\"a b `\"c\"") == LA("a b \"c"));
        // empty quoted
        REQUIRE(parse("\"\"") == LA(""));
        REQUIRE(parse("\" \"") == LA(" "));
        REQUIRE(parse("\"") == L());
        REQUIRE(parse("\"`\"\"") == LA("\""));
        REQUIRE(parse("`") == L());
        REQUIRE(parse("\"``\"") == LA("`"));
        REQUIRE(parse("\"\"") == LA(""));
        REQUIRE(parse("\"абвгд©®\"") == LA("абвгд©®"));
        REQUIRE(parse("[") == L());
        REQUIRE(parse("\"[\"") == LA("["));
        REQUIRE(parse("]") == L());
        REQUIRE(parse("\"]\"") == LA("]"));
        REQUIRE(parse("a: b") == L());
        REQUIRE(parse("toomany!@#$%^&*,:{}") == L());
        REQUIRE(parse("\"toomany!@#$%^&*,:{}\"") == LA("toomany!@#$%^&*,:{}"));
    }

    SECTION("base sequence")
    {
        REQUIRE(parse("1 2 3") == LF(1, 2, 3));
        REQUIRE(parse("a \"a space\" -2 3.5 true false null")
            == LA("a", "a space", -2, 3.5, 1, 0.f, Atom()));

        // mixed syntax
        REQUIRE(parse("[] (a b c) [ ] 1 ()").size() == 5);
        REQUIRE(parse("mixed data IntData(100) is possible")
            == LA("mixed", "data", new IntData(100), "is", "possible"));
    }

    SECTION("data atom")
    {
        REQUIRE(parse("IntData(100)") == LA(new IntData(100)));
        REQUIRE(parse("StrData(abc)") == LA(new StrData("abc")));
        REQUIRE(parse("StrData(\"a b c\")") == LA(new StrData("a b c")));
        REQUIRE(parse("UnknownData(1 2 3)") == Atom());
        REQUIRE(parse("MList(a b c)") == LA(new DataTypeMList(LA("a", "b", "c"))));
    }

    SECTION("base list")
    {
        REQUIRE(parse("()") == LA(new ML()));
        REQUIRE(parse("(null)") == LA(new ML(Atom())));
        REQUIRE(parse("(null null)") == LA(new ML(Atom(), Atom())));
        REQUIRE(parse("(a b c)") == LA(new ML("a", "b", "c")));
        REQUIRE(parse("(1 2 3)") == LF(new ML(1, 2, 3)));
        REQUIRE(parse(R"(("a b" "c d"))") == LA(new ML("a b", "c d")));

        // using ctor
        REQUIRE(parse("MList(1 2 3 MList(1 2 3 4))")
            == LA(new ML(1, 2, 3, new ML(1, 2, 3, 4))));
        REQUIRE(parse("MList( 1 2 3 MList( 1 2 3 4) )")
            == LA(new ML(1, 2, 3, new ML(1, 2, 3, 4))));
        REQUIRE(parse("()") == LA(new ML()));
        REQUIRE(parse("(1 2 3 (1 2 3))") == LA(new ML(1, 2, 3, new ML(1, 2, 3))));
    }

    SECTION("base string")
    {
        // using constructor
        REQUIRE(parse("String(a b c)") == LA(new DS("a b c")));
        REQUIRE(parse("String(a)") == LA(new DS("a")));
        REQUIRE(parse("String(\"a string\")") == LA(new DS("a string")));
        REQUIRE(parse("String(IntData(1000))") == LA(new DS("1000")));
        REQUIRE(parse("String([a: b])") == LA(new DS("[a: b]")));
        REQUIRE(parse("String(())") == LA(new DS("()")));
        REQUIRE(parse("String((a b))") == LA(new DS("(a b)")));

        // using S-prefix
        REQUIRE(parse("S\"a b c\"") == LA(new DS("a b c")));
        REQUIRE(parse("S\"\"") == LA(new DS("")));
        REQUIRE(parse("S\" \"") == LA(new DS(" ")));
        REQUIRE(parse("S\"`\"\"") == LA(new DS("\"")));
        REQUIRE(parse("S\"``\"") == LA(new DS("`")));
        REQUIRE(parse("S\"`.\"") == LA(new DS(",")));
        REQUIRE(parse("S\"`:\"") == LA(new DS(";")));
        REQUIRE(parse("S\"`(`)\"") == LA(new DS("{}")));

        // mixed
        REQUIRE(parse("S\"\" S\"\" 1 2") == LA(new DS(""), new DS(""), 1, 2));
        REQUIRE(parse("(S\"\" S\"a b c\")") == LA(new ML(new DS(""), new DS("a b c"))));
    }

    SECTION("base dict")
    {
        // []-syntax
        REQUIRE(parse("[]") == LA(new DD()));
        REQUIRE(parse("[key: value]") == LA(new DD("[key: value]")));
        REQUIRE(parse("[key: 1]") == LA(new DD("[key: 1]")));
        REQUIRE(parse("[a: 1 b: 20]") == LA(new DD("[a: 1 b: 20]")));
        REQUIRE(parse("[key: 1 2 3]") == LA(new DD("[key: 1 2 3]")));
        REQUIRE(parse("[key: \"a\"]") == LA(new DD("[key: a]")));
        REQUIRE(parse("[a: [b: 1 2 3]]") == LA(new DD("[a: [b: 1 2 3]]")));

        // ctor-syntax
        REQUIRE(parse("Dict[]") == LA(new DD()));
        //        REQUIRE(parse("Dict[a: IntData(100)]") == LA(new DD("([a: 100])")));
    }

    SECTION("expr")
    {
        REQUIRE(parse("expr(100)") == LA(100));
        REQUIRE(parse("expr(-100)") == LA(-100));
        REQUIRE(parse("expr(0.5)") == LA(0.5));
        REQUIRE(parse("expr(-0.5)") == LA(-0.5));
        REQUIRE(parse("expr(1+3)") == LA(4));
        REQUIRE(parse("expr(1-3)") == LA(-2));
        REQUIRE(parse("expr(1.5-3)") == LA(-1.5));
        REQUIRE(parse("expr(1.5+-3)") == LA(-1.5));
        REQUIRE(parse("expr(\"-1.5+-3\")") == LA(-4.5));
        REQUIRE(parse("expr(\"-1.5--3\")") == LA(1.5));
        REQUIRE(parse("expr(2*3.5)") == LA(7));
        REQUIRE(parse("expr(2*-3.5)") == LA(-7));
        REQUIRE(parse("expr(\"-2*-3.5\")") == LA(7));

        const t_float m_pi = std::acos(-1);
        REQUIRE(parse("expr(2*pi)") == LX(2 * m_pi));
        const t_float m_e = std::exp(1);
        REQUIRE(parse("expr(2*e)") == LX(2 * m_e));
        REQUIRE(parse("expr(XXXX)") == LF(0));
    }
}
