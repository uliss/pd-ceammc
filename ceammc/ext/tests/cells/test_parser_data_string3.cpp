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
#include "ceammc_platform.h"
#include "datatype_color.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "datatype_string.h"
#include "lex/lemon_data_string_parser.h"
#include "test_base.h"

using namespace ceammc;

namespace {
#define REQUIRE_PARSE_STR(str, res)           \
    {                                         \
        parser::LemonDataStringParser p;      \
        REQUIRE(p.parse(str));                \
        REQUIRE(AtomList(p.result()) == res); \
    }

#define REQUIRE_NO_PARSE_STR(str)        \
    {                                    \
        parser::LemonDataStringParser p; \
        REQUIRE_FALSE(p.parse(str));     \
    }

}

TEST_CASE("datastring3", "[ceammc::data]")
{
    test::pdPrintToStdError();

    SECTION("init")
    {
        REQUIRE_PARSE_STR("abc", LA("abc"));
        REQUIRE_PARSE_STR("100ms", LA("100ms"));
        REQUIRE_PARSE_STR("top100", LA("top100"));
        REQUIRE_PARSE_STR("the1/0", LA("the1/0"));
        REQUIRE_PARSE_STR("http://ceammc.com", LA("http://ceammc.com"));
        //        REQUIRE_NO_PARSE_STR("abc[0]");
        REQUIRE_PARSE_STR("абвгд жзйк", LA("абвгд", "жзйк"));
        REQUIRE_PARSE_STR("a b c", LA("a", "b", "c"));
        REQUIRE_PARSE_STR("'a b c'", LP("'a b c'"));
        REQUIRE_PARSE_STR("\"a b c\"", LA("a b c"));
        REQUIRE_PARSE_STR("' '", LP("' '"));
        REQUIRE_PARSE_STR("\" \"", LA(" "));
        REQUIRE_PARSE_STR("\"\"", LA(""));
        REQUIRE_PARSE_STR("\",\"", LA(","));
        REQUIRE_PARSE_STR("\";\"", LA(";"));
        REQUIRE_PARSE_STR("\"(1 2 3)\"", LA("(1 2 3)"));
        REQUIRE_PARSE_STR("\"a\" \"b\" \"c d\"", LA("a", "b", "c d"));
        REQUIRE_PARSE_STR("\"I`\"m\"", LA("I\"m"));
        REQUIRE_PARSE_STR("\"a b c\"", LA("a b c"));
        REQUIRE_PARSE_STR("\" \"", LA(" "));
        REQUIRE_PARSE_STR("\"\"", LA(""));
        REQUIRE_PARSE_STR("\"(1 2 3)\"", LA("(1 2 3)"));
        REQUIRE_PARSE_STR("\"I`\"m\"", LA("I\"m"));
        REQUIRE_PARSE_STR("@a", LA("@a"));
        REQUIRE_PARSE_STR("@a 1", LA("@a", 1));
        REQUIRE_PARSE_STR("@a 1 2", LA("@a", 1, 2));
        REQUIRE_PARSE_STR("#true", LF(1));
        REQUIRE_PARSE_STR("non-true", LA("non-true"));
        REQUIRE_PARSE_STR("#false", LF(0));
        REQUIRE_PARSE_STR("1 2", LF(1, 2));
        REQUIRE_PARSE_STR("#false #true", LF(0, 1));
        REQUIRE_PARSE_STR("@a 1", LA("@a", 1));
        REQUIRE_PARSE_STR("@a 1 2 3", LA("@a", 1, 2, 3));
        REQUIRE_PARSE_STR("@a 1 2 3 @b", LA("@a", 1, 2, 3, "@b"));
        REQUIRE_PARSE_STR("@a @b", LA("@a", "@b"));
        REQUIRE_PARSE_STR("@a @b 1", LA("@a", "@b", 1));
        REQUIRE_PARSE_STR("@a @b 1 2", LA("@a", "@b", 1, 2));
        REQUIRE_PARSE_STR("@a 2 @b 1", LA("@a", 2, "@b", 1));
        REQUIRE_PARSE_STR("@a 2 3 @b", LA("@a", 2, 3, "@b"));

        REQUIRE_PARSE_STR("", L());
        REQUIRE_PARSE_STR("   ", L());
        REQUIRE_PARSE_STR("#null", LA(Atom()));
        REQUIRE_PARSE_STR("#null #null", LA(Atom(), Atom()));
        REQUIRE_PARSE_STR("      #true", LF(1));
        REQUIRE_PARSE_STR("#false", LF(0));
        REQUIRE_PARSE_STR("#false #true #false #true", LF(0, 1, 0, 1));
        REQUIRE_PARSE_STR("0", LF(0));
        REQUIRE_PARSE_STR("+0", LF(0));
        REQUIRE_PARSE_STR("-0", LF(0));
        REQUIRE_PARSE_STR("l0", LA("l0"));
        REQUIRE_PARSE_STR("+1", LF(1));
        REQUIRE_PARSE_STR("-1", LF(-1));
        REQUIRE_PARSE_STR("-42", LF(-42));
        REQUIRE_PARSE_STR("-1.750", LF(-1.75));
        REQUIRE_PARSE_STR("-1234.50", LF(-1234.5));
        REQUIRE_PARSE_STR("1234567", LF(1234567));
        REQUIRE_PARSE_STR("0x0", LF(0));
        REQUIRE_PARSE_STR("0xBEEF", LF(0xBEEF));
        REQUIRE_PARSE_STR("0b1101", LF(0b1101));
        REQUIRE_PARSE_STR("0b1101", LF(0b1101));
        REQUIRE_PARSE_STR("1 2 3 4", LF(1, 2, 3, 4));
        REQUIRE_PARSE_STR(" 1", LF(1));
        REQUIRE_PARSE_STR(" 1 2", LF(1, 2));
        REQUIRE_PARSE_STR("    1    2", LF(1, 2));
        REQUIRE_PARSE_STR("-1/4", LF(-0.25));

        REQUIRE_PARSE_STR("@abc", LA("@abc"));
        REQUIRE_PARSE_STR("@abc 1", LA("@abc", 1));
        REQUIRE_PARSE_STR("@abc 1 2", LA("@abc", 1, 2));
        REQUIRE_PARSE_STR("@abc 1 2 @b", LA("@abc", 1, 2, "@b"));
        REQUIRE_PARSE_STR("@a 1 @c", LA("@a", 1, "@c"));
        REQUIRE_PARSE_STR("@a top100 @c", LA("@a", "top100", "@c"));
        REQUIRE_PARSE_STR("@a a:b @c", LA("@a", "a:b", "@c"));

        REQUIRE_PARSE_STR("1 2 3 @a", LA(1, 2, 3, "@a"));
        REQUIRE_PARSE_STR("1 2 3 @a 1", LA(1, 2, 3, "@a", 1));
        REQUIRE_PARSE_STR("1 2 3 @a -1 -2", LA(1, 2, 3, "@a", -1, -2));
        REQUIRE_PARSE_STR("1 2 3 @a -1 -2 @b", LA(1, 2, 3, "@a", -1, -2, "@b"));

        REQUIRE_PARSE_STR("reverse(1 2 3 4)", LF(4, 3, 2, 1));
        REQUIRE_PARSE_STR("-1 reverse(1 2)", LF(-1, 2, 1));
        REQUIRE_PARSE_STR("reverse(1 2 3) 100", LF(3, 2, 1, 100));
        REQUIRE_PARSE_STR("reverse(1 2) reverse(100 200)", LF(2, 1, 200, 100));
        REQUIRE_PARSE_STR("reverse(reverse(1 2) reverse(3 4))", LF(3, 4, 1, 2));
        REQUIRE_PARSE_STR("@prop reverse(1 2 3)", LA("@prop", 3, 2, 1));
        REQUIRE_PARSE_STR("@prop reverse(    1 2 3   )", LA("@prop", 3, 2, 1));
        REQUIRE_PARSE_STR("pi() @pi", LAX(3.14159, "@pi"));
        REQUIRE_PARSE_STR("pi( ) @pi", LAX(3.14159, "@pi"));

        REQUIRE_PARSE_STR("MList()", MListAtom());
        REQUIRE_PARSE_STR("MList( )", MListAtom());
        REQUIRE_PARSE_STR("MList(1)", MListAtom(1));
        REQUIRE_PARSE_STR("MList(1 2)", MListAtom(1, 2));
        REQUIRE_PARSE_STR("MList( 1 2 3 )", MListAtom(1, 2, 3));

        REQUIRE_PARSE_STR("()", MListAtom());
        REQUIRE_PARSE_STR("( )", MListAtom());
        REQUIRE_PARSE_STR("(1)", MListAtom(1));
        REQUIRE_PARSE_STR("(1 2)", MListAtom(1, 2));
        REQUIRE_PARSE_STR("(1 2 ())", MListAtom(1, 2, MListAtom()));
        REQUIRE_PARSE_STR("(1 2 [])", MListAtom(1, 2, DictAtom()));
        REQUIRE_PARSE_STR("(1 2 [a: \"hello !\"])", MListAtom(1, 2, DictAtom("[a: \"hello !\"]")));
        REQUIRE_PARSE_STR("(1 (2 3 (4 5 6)))", MListAtom(1, MListAtom(2, 3, MListAtom(4, 5, 6))));

        REQUIRE_PARSE_STR("[]", DictAtom());
        REQUIRE_PARSE_STR("[   ]", DictAtom());
        REQUIRE_PARSE_STR("[a:]", DictAtom("[a:]"));
        REQUIRE_PARSE_STR("[a:1]", DictAtom("[a: 1]"));
        REQUIRE_PARSE_STR("[a: [b:2]]", DictAtom("[a: [b: 2]]"));
        REQUIRE_PARSE_STR("[a:  1  ]", DictAtom("[a: 1]"));
        REQUIRE_PARSE_STR("[a: 1 2 3 ]", DictAtom("[a: 1 2 3]"));
        REQUIRE_PARSE_STR("[a1: 2 3 4]", DictAtom("[a1: 2 3 4]"));
        REQUIRE_PARSE_STR("[1: 2 3 4]", DictAtom("[1: 2 3 4]"));
        REQUIRE_PARSE_STR("[a: a100]", DictAtom("[a: a100]"));
        REQUIRE_PARSE_STR("[a: reverse(1 2 3)]", DictAtom("[a: 3 2 1]"));
        REQUIRE_PARSE_STR("[ a: reverse( 1 2 3 ) ]", DictAtom("[a: 3 2 1]"));
        REQUIRE_PARSE_STR("[proto: http://]", DictAtom("[proto: http: \"//\"]"));
        REQUIRE_PARSE_STR("[proto: \"http://\"]", DictAtom("[proto: \"http://\"]"));

        REQUIRE_PARSE_STR("Dict[]", DictAtom());
        REQUIRE_PARSE_STR("Dict[   ]", DictAtom());
        REQUIRE_PARSE_STR("Dict[a:]", DictAtom("[a:]"));
        REQUIRE_PARSE_STR("Dict[a: b: c: 1 2 3]", DictAtom("[b: a: c: 1 2 3]"));
        REQUIRE_PARSE_STR("1 symbol", LA(1, "symbol"));
    }

    SECTION("env vars")
    {
        platform::set_env("VAR4", "100");
        platform::set_env("MY_HOME", "/home/ceammc");

        REQUIRE_PARSE_STR("\"%VAR4%\"", LA("100"));
        REQUIRE_PARSE_STR("\"%VAR4%/test\"", LA("100/test"));
        REQUIRE_PARSE_STR("\"test:%VAR4%\"", LA("test:100"));
        REQUIRE_PARSE_STR("\"test:%VAR?%\"", LA("test:%VAR?%"));
        REQUIRE_PARSE_STR("\"%MY_HOME%/doc/my.wav\"", LA("/home/ceammc/doc/my.wav"));
    }

    SECTION("String()")
    {
        platform::set_env("MY_HOME", "/home/ceammc");

        REQUIRE_PARSE_STR("S\"\"", StringAtom(""));
        REQUIRE_PARSE_STR("S\" \"", StringAtom(" "));
        REQUIRE_PARSE_STR("S\"   \"", StringAtom("   "));
        REQUIRE_PARSE_STR("S\"String()\"", StringAtom("String()"));
        REQUIRE_PARSE_STR("S\"%MY_HOME%\"", StringAtom("/home/ceammc"));
    }

    SECTION("Matrix")
    {
        REQUIRE_PARSE_STR("#[(1 2)]", Atom());
        REQUIRE_PARSE_STR("#[(1 0/2 +0.25) (-0 0xFF 0) (0 0 1)]", Atom());
        REQUIRE_PARSE_STR("#[(1 0 0) (0 1 0) (0 0 1)]", Atom());
        REQUIRE_PARSE_STR("#3:4[(1 2) (3 4)]", Atom());
        REQUIRE_PARSE_STR("#10:11[(1 2) (3 4)]", Atom());
    }

    SECTION("Color")
    {
        REQUIRE_PARSE_STR("#123456FA", ColorAtom(0x123456FA));
        REQUIRE_PARSE_STR("#000000FE", ColorAtom(0x000000FE));
        REQUIRE_PARSE_STR("#ABCDEF00", ColorAtom(0xABCDEF00));
        REQUIRE_PARSE_STR("#abcdef00", ColorAtom(0xABCDEF00));
        REQUIRE_PARSE_STR("#FF11bb", ColorAtom(0xFF11BBFF));
    }
}
