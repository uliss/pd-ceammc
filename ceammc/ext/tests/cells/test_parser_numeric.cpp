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
#include "lex/parser_numeric.h"
#include "test_base.h"

using namespace ceammc;

TEST_CASE("parser_numeric", "[ceammc::ceammc_units]")
{
    test::pdPrintToStdError();

    SECTION("int")
    {
        using namespace ceammc::parser;
        NumericFullMatch p;

        REQUIRE(p.parse("1"));
        REQUIRE(p.type() == TYPE_INT);
        REQUIRE(p.isInt());
        REQUIRE(p.asFloat() == 1);

        REQUIRE(p.parse("-1"));
        REQUIRE(p.isInt());
        REQUIRE(p.asFloat() == -1);

        REQUIRE(p.parse("+1"));
        REQUIRE(p.isInt());
        REQUIRE(p.asFloat() == 1);

        REQUIRE(p.parse("123456789"));
        REQUIRE(p.isInt());
        REQUIRE(p.asFloat() == 123456789);

        REQUIRE(p.parse("-1234567890"));
        REQUIRE(p.isInt());
        REQUIRE(p.asFloat() == -1234567890);

        REQUIRE(p.parse("+12345678"));
        REQUIRE(p.isInt());
        REQUIRE(p.asFloat() == 12345678);

        REQUIRE(p.parse("0"));
        REQUIRE(p.isInt());
        REQUIRE(p.asFloat() == 0);

        REQUIRE(p.parse("-0"));
        REQUIRE(p.isInt());
        REQUIRE(p.asFloat() == 0);

        REQUIRE(p.parse("+0"));
        REQUIRE(p.isInt());
        REQUIRE(p.asFloat() == 0);

        REQUIRE(!p.parse("00"));
        REQUIRE(!p.parse("0123"));
        REQUIRE(!p.parse("123abc"));
        REQUIRE(!p.parse("abc123abc"));
        REQUIRE(!p.parse("123abc"));
        REQUIRE(!p.parse("abc123abc"));
        REQUIRE(!p.parse(" 123"));
        REQUIRE(!p.parse("123 "));
        REQUIRE(!p.parse(" "));
        REQUIRE(!p.parse(""));
    }

    SECTION("bin")
    {
        using namespace ceammc::parser;
        NumericFullMatch p;

        REQUIRE(p.parse("0b1"));
        REQUIRE(p.type() == TYPE_BIN);
        REQUIRE(p.isBin());
        REQUIRE(p.asFloat() == 0b1);

        REQUIRE(p.parse("0b0"));
        REQUIRE(p.isBin());
        REQUIRE(p.asFloat() == 0b0);

        REQUIRE(p.parse("0b0111010101"));
        REQUIRE(p.isBin());
        REQUIRE(p.asFloat() == 0b0111010101);

        REQUIRE(!p.parse("b10"));
        REQUIRE(!p.parse("0b"));
        REQUIRE(!p.parse("-0b101"));
        REQUIRE(!p.parse("+0b101"));
    }

    SECTION("hex")
    {
        using namespace ceammc::parser;
        NumericFullMatch p;

        REQUIRE(p.parse("0x1"));
        REQUIRE(p.type() == TYPE_HEX);
        REQUIRE(p.isHex());
        REQUIRE(p.asFloat() == 0x1);

        REQUIRE(p.parse("0x0"));
        REQUIRE(p.isHex());
        REQUIRE(p.asFloat() == 0x0);

        REQUIRE(p.parse("0x123"));
        REQUIRE(p.isHex());
        REQUIRE(p.asFloat() == 0x123);

        REQUIRE(p.parse("0xDEADBEEF"));
        REQUIRE(p.isHex());
        REQUIRE(p.asFloat() == 0xDEADBEEF);

        REQUIRE(!p.parse("x10"));
        REQUIRE(!p.parse("0x"));
        REQUIRE(!p.parse("-0x123"));
        REQUIRE(!p.parse("+0x123"));
    }

    SECTION("ratio")
    {
        using namespace ceammc::parser;
        NumericFullMatch p;
        t_int n, d;

        REQUIRE(p.parse("1/1"));
        REQUIRE(p.type() == TYPE_RATIO);
        REQUIRE(p.isRatio());

        REQUIRE(p.getRatio(n, d));
        REQUIRE(n == 1);
        REQUIRE(d == 1);
        REQUIRE(p.asFloat() == 1);

        REQUIRE(p.parse("1/2"));
        REQUIRE(p.isRatio());
        REQUIRE(p.getRatio(n, d));
        REQUIRE(n == 1);
        REQUIRE(d == 2);
        REQUIRE(p.asFloat() == 0.5);

        REQUIRE(p.parse("+3/4"));
        REQUIRE(p.isRatio());
        REQUIRE(p.getRatio(n, d));
        REQUIRE(n == 3);
        REQUIRE(d == 4);
        REQUIRE(p.asFloat() == 0.75);

        REQUIRE(p.parse("-1/8"));
        REQUIRE(p.isRatio());
        REQUIRE(p.getRatio(n, d));
        REQUIRE(n == -1);
        REQUIRE(d == 8);
        REQUIRE(p.asFloat() == -0.125);

        REQUIRE(p.parse("0/2"));
        REQUIRE(p.isRatio());
        REQUIRE(p.getRatio(n, d));
        REQUIRE(n == 0);
        REQUIRE(d == 2);
        REQUIRE(p.asFloat() == 0);

        REQUIRE(p.parse("0/0"));
        REQUIRE(p.isRatio());
        REQUIRE(p.getRatio(n, d));
        REQUIRE(n == 0);
        REQUIRE(d == 0);
        REQUIRE(p.asFloat() == 0);

        REQUIRE(!p.parse("1/"));
        REQUIRE(!p.parse("/2"));
        REQUIRE(!p.parse("a/2"));
        REQUIRE(!p.parse("00/1"));
        REQUIRE(!p.parse("1/-2"));
        REQUIRE(!p.parse("1/+2"));
    }

    SECTION("phase")
    {
        using namespace ceammc::parser;
        NumericFullMatch p;

        REQUIRE(p.parse("0.125*"));
        REQUIRE(p.type() == TYPE_PHASE);
        REQUIRE(p.asFloat() == 0.125);

        REQUIRE(p.parse("+0.25*"));
        REQUIRE(p.type() == TYPE_PHASE);
        REQUIRE(p.asFloat() == 0.25);

        REQUIRE(p.parse("-0.25*"));
        REQUIRE(p.type() == TYPE_PHASE);
        REQUIRE(p.asFloat() == -0.25);

        REQUIRE(p.parse("0.0*"));
        REQUIRE(p.type() == TYPE_PHASE);
        REQUIRE(p.asFloat() == 0);

        REQUIRE(p.parse("0*"));
        REQUIRE(p.type() == TYPE_PHASE);
        REQUIRE(p.asFloat() == 0);

        REQUIRE(p.parse("1*"));
        REQUIRE(p.type() == TYPE_PHASE);
        REQUIRE(p.asFloat() == 1);
    }

    SECTION("percent")
    {
        using namespace ceammc::parser;
        NumericFullMatch p;

        REQUIRE(p.parse("0%"));
        REQUIRE(p.type() == TYPE_PERCENT);
        REQUIRE(p.asFloat() == 0);

        REQUIRE(p.parse("25%"));
        REQUIRE(p.type() == TYPE_PERCENT);
        REQUIRE(p.asFloat() == 0.25);

        REQUIRE(p.parse("-25%"));
        REQUIRE(p.type() == TYPE_PERCENT);
        REQUIRE(p.asFloat() == -0.25);

        REQUIRE(p.parse("+25%"));
        REQUIRE(p.type() == TYPE_PERCENT);
        REQUIRE(p.asFloat() == 0.25);

        REQUIRE(p.parse("12.5%"));
        REQUIRE(p.type() == TYPE_PERCENT);
        REQUIRE(p.asFloat() == 0.125);
    }

    SECTION("float")
    {
        using namespace ceammc::parser;
        NumericFullMatch p;

        REQUIRE(p.parse("1.0"));
        REQUIRE(p.type() == TYPE_FLOAT);
        REQUIRE(p.isFloat());
        REQUIRE(p.asFloat() == 1.0);

        REQUIRE(p.parse("1.25"));
        REQUIRE(p.isFloat());
        REQUIRE(p.asFloat() == 1.25);

        REQUIRE(p.parse("-1.25"));
        REQUIRE(p.isFloat());
        REQUIRE(p.asFloat() == -1.25);

        REQUIRE(p.parse("+0.125"));
        REQUIRE(p.isFloat());
        REQUIRE(p.asFloat() == 0.125);

        REQUIRE(p.parse("+0.0"));
        REQUIRE(p.isFloat());
        REQUIRE(p.asFloat() == 0.0);

        REQUIRE(p.parse("+1234.5"));
        REQUIRE(p.isFloat());
        REQUIRE(p.asFloat() == 1234.5);

        REQUIRE(p.parse("inf"));
        REQUIRE(p.isInfinity());
        REQUIRE(p.asFloat() == std::numeric_limits<double>::infinity());

        REQUIRE(p.parse("+inf"));
        REQUIRE(p.isInfinity());
        REQUIRE(std::isinf(p.asFloat()));

        REQUIRE(p.parse("-inf"));
        REQUIRE(p.isInfinity());
        REQUIRE(std::isinf(p.asFloat()));
    }

    SECTION("atom")
    {
        using namespace ceammc::parser;
        NumericFullMatch p;

        REQUIRE(p.parse(A(123)));
        REQUIRE(p.isInt());
        REQUIRE(p.asFloat() == 123);

        REQUIRE(p.parse(A(0.0)));
        REQUIRE(p.isInt());
        REQUIRE(p.asFloat() == 0);

        REQUIRE(p.parse(A(-123)));
        REQUIRE(p.isInt());
        REQUIRE(p.asFloat() == -123);

        REQUIRE(p.parse(A(-1.5)));
        REQUIRE(p.isFloat());
        REQUIRE(p.asFloat() == -1.5);

        REQUIRE(p.parse(A(2.5)));
        REQUIRE(p.isFloat());
        REQUIRE(p.asFloat() == 2.5);

        REQUIRE(p.parse(A("1/2")));
        REQUIRE(p.isRatio());
        REQUIRE(p.asFloat() == 0.5);

        REQUIRE(p.parse(A("+100.5")));
        REQUIRE(p.isFloat());
        REQUIRE(p.asFloat() == 100.5);

        REQUIRE(!p.parse(Atom()));
        REQUIRE(!p.parse(A("")));
    }

    SECTION("parseAll")
    {
        using namespace ceammc::parser;
        NumericFullMatch p;

        SmallFVec fv;

        REQUIRE(p.parseAll(LF(1, 2, 3), fv));
        REQUIRE(fv[0] == 1);
        REQUIRE(fv[1] == 2);
        REQUIRE(fv[2] == 3);
        REQUIRE(fv.size() == 3);
        fv.clear();

        REQUIRE(p.parseAll(LA("0xFF", "0b101", "100.25"), fv));
        REQUIRE(fv[0] == 0xFF);
        REQUIRE(fv[1] == 0b101);
        REQUIRE(fv[2] == 100.25);
        fv.clear();

        REQUIRE(p.parseAll(LA("+100", "-1/2", "-2"), fv));
        REQUIRE(fv[0] == 100);
        REQUIRE(fv[1] == -0.5);
        REQUIRE(fv[2] == -2);
        fv.clear();

        REQUIRE(p.parseAll(L(), fv));

        REQUIRE(!p.parseAll(LA("+200", "??", 100), fv));
        REQUIRE(!p.parseAll(LA("+200", "??"), fv));
    }

    SECTION("some")
    {
        using namespace ceammc::parser;
        NumericMatchSome p;
        const char* end = "_";
        REQUIRE(p.parseSingle("123", end));
        REQUIRE(*end == '\0');
        REQUIRE(p.size() == 1);

        REQUIRE(p.parseSingle("     123", end));
        REQUIRE(*end == '\0');
        REQUIRE(p.size() == 2);

        REQUIRE(p.parseSingle("123    ", end));
        REQUIRE(*end == '\0');
        REQUIRE(p.size() == 3);

        REQUIRE(p.parseSingle("  \t  123    ", end));
        REQUIRE(*end == '\0');
        REQUIRE(p.size() == 4);

        REQUIRE_FALSE(p.parseSingle("  ", end));
        REQUIRE(*end == '\0');
        REQUIRE(p.size() == 4);

        REQUIRE_FALSE(p.parseSingle("  ?x", end));
        REQUIRE(*end == '?');
        REQUIRE(p.size() == 4);

        REQUIRE_FALSE(p.parseSingle("123xyz", end));
        REQUIRE(*end == 'x');
        REQUIRE(p.size() == 4);

        REQUIRE(p.parseSingle("123 xyz", end));
        REQUIRE(*end == 'x');
        REQUIRE(p.size() == 5);

        REQUIRE(p.parseSingle("123  \t    ? ", end));
        REQUIRE(*end == '?');
        REQUIRE(p.size() == 6);

        REQUIRE_FALSE(p.parseSingle("ABC", end));
        REQUIRE(*end == 'A');
        REQUIRE(p.size() == 6);
    }

    SECTION("many")
    {
        using namespace ceammc::parser;
        NumericMatchSome p;
        const char* end = "_";

        REQUIRE_FALSE(p.parseMany("", end));

        REQUIRE(p.parseMany("123", end));
        REQUIRE(*end == '\0');
        REQUIRE(p.size() == 1);
        REQUIRE(p.isIntAt(0));

        REQUIRE(p.parseMany("123 456.75", end));
        REQUIRE(*end == '\0');
        REQUIRE(p.size() == 2);
        REQUIRE(p.isIntAt(0));
        REQUIRE(p.isFloatAt(1));

        REQUIRE(p.parseMany("1/2 -20 +11.5 0xFF 0b1011 XXX", end));
        REQUIRE(p.size() == 5);
        REQUIRE(p.asFloatAt(0) == 0.5);
        REQUIRE(p.asFloatAt(1) == -20);
        REQUIRE(p.asFloatAt(2) == 11.5);
        REQUIRE(p.asFloatAt(3) == 0xFF);
        REQUIRE(p.asFloatAt(4) == 0b1011);
    }
}
