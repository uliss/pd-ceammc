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
#include "ceammc_data.h"
#include "ceammc_platform.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "datatype_string.h"
#include "test_base.h"
#include "test_common.h"

using ML = DataTypeMList;
using DD = DataTypeDict;
using DS = DataTypeString;

static AtomList parse(const char* str)
{
    auto res = parseDataString(str);
    if (res)
        return res.result();
    else
        return {};
}

TEST_CASE("DataStringParser", "[core]")
{
    test::pdPrintToStdError();
    IntData int100(100);

    SECTION("basic atom types")
    {
        REQUIRE(parse("") == L());
        REQUIRE(parse("#null") == Atom());
        REQUIRE(parse("#true") == Atom(1));
        REQUIRE(parse("#false") == Atom(0.f));
        REQUIRE(parse("abc") == LA("abc"));
        REQUIRE(parse("@prop") == LA("@prop"));
        REQUIRE(parse("what's up?") == LA("what's", "up?"));
        REQUIRE(parse("@prop?") == LA("@prop?"));
        REQUIRE(parse("a==b") == LA("a==b"));
        // no unicode in symbols
        REQUIRE(parse("абвгд") == LA("абвгд"));
        // first char should not be &
        REQUIRE(parse("&abcd") == LA("&abcd"));
        REQUIRE(parse("100") == LF(100));
        REQUIRE(parse("-100") == LF(-100));
        REQUIRE(parse("100.5") == LF(100.5));
        REQUIRE(parse("-100.5") == LF(-100.5));
        REQUIRE(parse("0xff") == LF(0xFF));
        REQUIRE(parse("-0xff") == LA("-0xff"));
        REQUIRE(parse("0x1A") == LF(0x1A));
        REQUIRE(parse("0b000101") == LF(5));
        REQUIRE(parse("0b11111111") == LF(0xFF));
        //        REQUIRE(parse("0xffffffffffffffff") == L());
        REQUIRE(parse("\"a b c\"") == LA("a b c"));
        REQUIRE(parse(R"("a b `"c")") == LA("a b \"c"));
        // empty quoted
        REQUIRE(parse("\"\"") == LA(""));
        REQUIRE(parse("\" \"") == LA(" "));
        REQUIRE(parse("\"") == L());
        REQUIRE(parse("\"`\"\"") == LA("\""));
        REQUIRE(parse("`") == LA("`"));
        REQUIRE(parse("\"``\"") == LA("`"));
        REQUIRE(parse("\"\"") == LA(""));
        REQUIRE(parse("\"абвгд©®\"") == LA("абвгд©®"));
        REQUIRE(parse("[") == L());
        REQUIRE(parse("\"[\"") == LA("["));
        REQUIRE(parse("]") == LA("]"));
        REQUIRE(parse("\"]\"") == LA("]"));
        REQUIRE(parse("a: b") == LA("a:", "b"));
        REQUIRE(parse("toomany!@#$%^&*,:{}") == LA("toomany!@#$%^&*,:{}"));
        REQUIRE(parse("\"toomany!@#$%^&*,:{}\"") == LA("toomany!@#$%^&*,:{}"));
    }

    SECTION("units")
    {
        REQUIRE(parse("100ms") == LA("100ms"));
        REQUIRE(parse("0.5_ms") == LA("0.5_ms"));
        REQUIRE(parse("-12.5ms") == LA("-12.5ms"));

        REQUIRE(parse("5%") == LA("5%"));
        REQUIRE(parse("-5%") == LA("-5%"));
    }

    SECTION("base sequence")
    {
        REQUIRE(parse("1 2 3") == LF(1, 2, 3));
        REQUIRE(parse("a \"a space\" -2 3.5 #true #false #null")
            == LA("a", "a space", -2, 3.5, 1, 0.f, Atom()));

        // mixed syntax
        REQUIRE(parse("[] (a b c) [ ] 1 ()").size() == 5);
        REQUIRE(parse("mixed data IntData(100) is possible")
            == LA("mixed", "data", new IntData(100), "is", "possible"));
    }

    SECTION("data atom")
    {
        REQUIRE(parse("IntData(100)") == LA(new IntData(100)));
        REQUIRE(parse("IntData(-100)") == LA(new IntData(-100)));
        REQUIRE(parse("StrData(abc)") == LA(new StrData("abc")));
        REQUIRE(parse("StrData(\"a b c\")") == LA(new StrData("a b c")));
        REQUIRE(parse("UnknownData(1 2 3)") == Atom());
        REQUIRE(parse("MList(a b c)") == LA(new DataTypeMList(LA("a", "b", "c"))));
    }

    SECTION("base list")
    {
        REQUIRE(parse("()") == LA(new ML()));
        REQUIRE(parse("(#null)") == LA(new ML(Atom())));
        REQUIRE(parse("(#null #null)") == LA(new ML(Atom(), Atom())));
        REQUIRE(parse("(a b c)") == LA(new ML("a", "b", "c")));
        REQUIRE(parse("(1 2 -3)") == LF(new ML(1, 2, -3)));
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
        REQUIRE(parse("String(IntData(1000))") == LA(new DS("IntData(1000)")));
        REQUIRE(parse("String([a: b])") == LA(new DS("[a: b]")));
        REQUIRE(parse("String(())") == LA(new DS("()")));
        REQUIRE(parse("String((a b))") == LA(new DS("(a b)")));

        // using S-prefix
        REQUIRE(parse("S\"a b c\"") == LA(new DS("a b c")));
        REQUIRE(parse("S\"\"") == LA(new DS("")));
        REQUIRE(parse("S\" \"") == LA(new DS(" ")));
        REQUIRE(parse("S\"``\"") == LA(new DS("`")));
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
        REQUIRE(parse("[key: 1 2 -3]") == LA(new DD("[key: 1 2 -3]")));
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
        REQUIRE(parse("expr(XXXX)").empty());
        REQUIRE(parse("expr(\"min(100,200)\")") == LF(100));
    }

    SECTION("seq()")
    {
        REQUIRE(parse("seq()").empty());
        REQUIRE(parse("seq(abc)").empty());
        REQUIRE(parse("seq(1)") == LF(1));
        REQUIRE(parse("seq(-10)") == LF(-10));
        REQUIRE(parse("seq(0.5)") == LF(0.5));
        REQUIRE(parse("seq(1 1)") == LF(1));
        REQUIRE(parse("seq(0 1)") == LF(0, 1));
        REQUIRE(parse("seq(1 0)") == LF(1, 0));
        REQUIRE(parse("seq(-2 1)") == LF(-2, -1, 0, 1));
        REQUIRE(parse("seq(1 -2)") == LF(1, 0, -1, -2));
        REQUIRE(parse("seq(1 2000)") == LF(1, 2000));
        REQUIRE(parse("seq(0.5 2.5)") == LF(0.5, 1.5, 2.5));
        REQUIRE(parse("seq(0.5 2.4)") == LF(0.5, 1.5));
        REQUIRE(parse("seq(0 1 0)").empty());
        REQUIRE(parse("seq(0 1 -0.1)").empty());
        REQUIRE(parse("seq(0 1 0.5)") == LF(0, 0.5, 1));
        REQUIRE(parse("seq(0 1 1)") == LF(0, 1));
        REQUIRE(parse("seq(1 2 1.1)") == LF(1));
        REQUIRE(parse("seq(1.5 2.5 1.1)") == LF(1.5));
        REQUIRE(parse("seq(2 -2 0.5)") == LF(2, 1.5, 1, 0.5, 0, -0.5, -1, -1.5, -2));
        REQUIRE(parse("seq(bs())") == LF(64));
    }

    SECTION("env()")
    {
        REQUIRE(parse("env()").empty());
        REQUIRE(parse("env(10)").empty());
        REQUIRE(parse("env(A B C)").empty());

#ifdef __MACH__
        const auto home_dir = platform::home_directory();
        REQUIRE(parse("env(%HOME%)") == LA(home_dir.c_str()));
        REQUIRE(parse("env(%HOME%/test.wav)") == LA((home_dir + "/test.wav").c_str()));
#endif
    }

    SECTION("db2amp()")
    {
        REQUIRE(parse("db2amp()") == LF(0));
        REQUIRE(parse("db2amp(ABC)") == LF(0));
        REQUIRE(parse("db2amp(1 2 3)") == LF(0));
        REQUIRE(parse("db2amp(0)") == LX(1));
        REQUIRE(parse("db2amp(-6)") == LX(0.501187));
    }

    SECTION("amp2db()")
    {
        REQUIRE(parse("amp2db()") == LF(-144));
        REQUIRE(parse("amp2db(ABC)") == LF(-144));
        REQUIRE(parse("amp2db(1 2 3)") == LF(-144));
        REQUIRE(parse("amp2db(0)") == LF(-144));
        REQUIRE(parse("amp2db(-1)") == LF(-144));
        REQUIRE(parse("amp2db(1)") == LF(0));
        REQUIRE(parse("amp2db(0.501187)") == LX(-6));
    }

    SECTION("repeat()")
    {
        REQUIRE(parse("repeat()").empty());
        REQUIRE(parse("repeat(A B)").empty());
        REQUIRE(parse("repeat(100)").empty());
        REQUIRE(parse("repeat(-1 100)").empty());
        REQUIRE(parse("repeat(0 100)").empty());
        REQUIRE(parse("repeat(1 A)") == LA("A"));
        REQUIRE(parse("repeat(2 A)") == LA("A", "A"));
        REQUIRE(parse("repeat(2 A B)") == LA("A", "B", "A", "B"));
        REQUIRE(parse("repeat(2 repeat(1 A C))") == LA("A", "C", "A", "C"));
        REQUIRE(parse("repeat(1024 A B C)").empty());
    }

    SECTION("reverse()")
    {
        REQUIRE(parse("reverse()").empty());
        REQUIRE(parse("reverse(A)") == LA("A"));
        REQUIRE(parse("reverse(A B)") == LA("B", "A"));
    }

    SECTION("rtree()")
    {
        REQUIRE(parse("rtree()").empty());
        REQUIRE(parse("rtree(1)").empty());
        REQUIRE(parse("rtree(1 2)").empty());
        REQUIRE(parse("rtree(x ())").empty());
        REQUIRE(parse("rtree(1 (1))") == LF(1));
        REQUIRE(parse("rtree(1 (1 1))") == LF(0.5, 0.5));
        REQUIRE(parse("rtree(2 (1 1))") == LF(1, 1));
        REQUIRE(parse("rtree(2 (1 1 (1 1)))") == LF(1, 0.5, 0.5));
        REQUIRE(parse("rtree(1 (1 1 2))") == LF(0.25, 0.25, 0.5));
        REQUIRE(parse("rtree(1 (1 1 2 (2 1 1)))") == LF(0.25, 0.25, 0.25, 0.125, 0.125));
    }

    SECTION("euclid()")
    {
        REQUIRE(parse("euclid()").empty());
        REQUIRE(parse("euclid(1)").empty());
        REQUIRE(parse("euclid(A B)").empty());
        REQUIRE(parse("euclid(10 1)").empty());
        REQUIRE(parse("euclid(-1 10)").empty());
        REQUIRE(parse("euclid(1 -2)").empty());
        REQUIRE(parse("euclid(0 4)") == LF(0, 0, 0, 0));
        REQUIRE(parse("euclid(1 1)") == LF(1));
        REQUIRE(parse("euclid(2 2)") == LF(1, 1));
        REQUIRE(parse("euclid(3 3)") == LF(1, 1, 1));
        REQUIRE(parse("euclid(2 3)") == LF(1, 0, 1));
        REQUIRE(parse("euclid(2 5)") == LF(1, 0, 0, 1, 0));
        REQUIRE(parse("euclid(7 16)") == LF(1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0));
        REQUIRE(parse("euclid(3 7)") == LF(1, 0, 0, 1, 0, 1, 0));
        REQUIRE(parse("euclid(3 4)") == LF(1, 0, 1, 1));
        REQUIRE(parse("euclid(3 5)") == LF(1, 0, 1, 0, 1));
        REQUIRE(parse("euclid(3 8)") == LF(1, 0, 0, 1, 0, 0, 1, 0));
        //        REQUIRE(parse("euclid(3 7)") == LF(1, 0, 1, 0, 0, 1, 0));
    }

    SECTION("hexbeat()")
    {
        REQUIRE(parse("hexbeat()").empty());
        REQUIRE(parse("hexbeat(1)").empty());
        REQUIRE(parse("hexbeat(F1)") == LF(1, 1, 1, 1, 0, 0, 0, 1));
        REQUIRE(parse("hexbeat(1F)") == LF(0, 0, 0, 1, 1, 1, 1, 1));
        REQUIRE(parse("hexbeat(#F2)") == LF(1, 1, 1, 1, 0, 0, 1, 0));
        REQUIRE(parse("hexbeat(#2)") == LF(0, 0, 1, 0));
        REQUIRE(parse("hexbeat(0x3)").empty());
        REQUIRE(parse("hexbeat(\"0x3\")") == LF(0, 0, 1, 1));
        REQUIRE(parse("hexbeat(\"4\")") == LF(0, 1, 0, 0));
    }
}
