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
#include "args/arg_group_and.h"
#include "args/arg_group_or.h"
#include "args/arg_is_bool.h"
#include "args/arg_is_float.h"
#include "args/arg_is_symbol.h"
#include "ceammc_args.h"
#include "ceammc_atomlist.h"
#include "ceammc_atomlist_view.h"
#include "datatype_string.h"

#include "catch.hpp"
#include "test_common.h"

using namespace ceammc;

TEST_CASE("args", "[core]")
{
    SECTION("AtomChecker")
    {
        SECTION("checkType")
        {
            CheckerContext ctx;

            // bool
            ArgIsBool is_bool;
            REQUIRE(is_bool.checkAtom(A(1), ctx));
            REQUIRE(ctx.error.empty());
            REQUIRE(is_bool.checkAtom(A(0.f), ctx));
            REQUIRE(is_bool.checkAtom(A("true"), ctx));
            REQUIRE(is_bool.checkAtom(A("false"), ctx));
            REQUIRE_FALSE(is_bool.checkAtom(A("True"), ctx));
            REQUIRE(ctx.error == "not a bool value: True");
            REQUIRE_FALSE(is_bool.checkAtom(A(-200), ctx));
            REQUIRE(ctx.error == "not a bool value: -200");
        }

        SECTION("checkFloat")
        {
            CheckerContext ctx;
            ArgIsFloat is_float;

#define REQUIRE_FLOAT(x, a, t, p)        \
    {                                    \
        x.setCheck(ArgIsFloat::t, p);    \
        REQUIRE(x.checkAtom(A(a), ctx)); \
    }

#define REQUIRE_FLOAT_FALSE(x, a, t, p)        \
    {                                          \
        x.setCheck(ArgIsFloat::t, p);          \
        REQUIRE_FALSE(x.checkAtom(A(a), ctx)); \
    }

            // ==
            REQUIRE_FLOAT(is_float, A(1), FLOAT_EQUAL, 1);
            REQUIRE_FLOAT_FALSE(is_float, A(1), FLOAT_EQUAL, 2);
            REQUIRE(ctx.error == "expected value is 2.0, got 1.0");
            // !=
            REQUIRE_FLOAT_FALSE(is_float, A(1), FLOAT_NOT_EQUAL, 1);
            REQUIRE_FLOAT(is_float, A(1), FLOAT_NOT_EQUAL, 2);
            // >=
            REQUIRE_FLOAT(is_float, A(2), FLOAT_GREATER_EQUAL, 2);
            REQUIRE_FLOAT(is_float, A(3), FLOAT_GREATER_EQUAL, 2);
            REQUIRE_FLOAT_FALSE(is_float, A(2), FLOAT_GREATER_EQUAL, 3);
            // <
            REQUIRE_FLOAT(is_float, A(2), FLOAT_LESS, 3);
            REQUIRE_FLOAT_FALSE(is_float, A(3), FLOAT_LESS, 3);
            REQUIRE_FLOAT_FALSE(is_float, A(4), FLOAT_LESS, 3);
            // >
            REQUIRE_FLOAT(is_float, A(2), FLOAT_GREATER, 1);
            REQUIRE_FLOAT_FALSE(is_float, A(2), FLOAT_GREATER, 2);
            REQUIRE_FLOAT_FALSE(is_float, A(1), FLOAT_GREATER, 2);
            // <=
            REQUIRE_FLOAT(is_float, A(2), FLOAT_LESS_EQUAL, 2);
            REQUIRE_FLOAT(is_float, A(2), FLOAT_LESS_EQUAL, 3);
            REQUIRE_FLOAT_FALSE(is_float, A(2), FLOAT_LESS_EQUAL, 1);
        }

        SECTION("checkSym")
        {
            CheckerContext ctx;
            ArgIsSymbol is_symbol;

#define REQUIRE_SYMBOL(x, a, t, p)       \
    {                                    \
        x.setCheck(ArgIsSymbol::t, p);   \
        REQUIRE(x.checkAtom(A(a), ctx)); \
    }

#define REQUIRE_SYMBOL_FALSE(x, a, t, p)       \
    {                                          \
        x.setCheck(ArgIsSymbol::t, p);         \
        REQUIRE_FALSE(x.checkAtom(A(a), ctx)); \
    }

            // ==
            REQUIRE_SYMBOL(is_symbol, "A", SYM_EQUAL, "A");
            REQUIRE_SYMBOL_FALSE(is_symbol, "A", SYM_EQUAL, "B");
            // !=
            REQUIRE_SYMBOL(is_symbol, "A", SYM_NOT_EQUAL, "B");
            REQUIRE_SYMBOL_FALSE(is_symbol, "A", SYM_NOT_EQUAL, "A");
            // startswith
            REQUIRE_SYMBOL(is_symbol, "A", SYM_BEGINS_WITH, "A");
            REQUIRE_SYMBOL(is_symbol, "ABC", SYM_BEGINS_WITH, "AB");
            REQUIRE_SYMBOL_FALSE(is_symbol, "ABC", SYM_BEGINS_WITH, "BC");
            REQUIRE_SYMBOL_FALSE(is_symbol, "", SYM_BEGINS_WITH, "BC");
            REQUIRE_SYMBOL(is_symbol, "", SYM_BEGINS_WITH, "");
            REQUIRE_SYMBOL_FALSE(is_symbol, "ABC", SYM_BEGINS_WITH, "ABCD");

            // contains
            REQUIRE_SYMBOL(is_symbol, "ABC", SYM_CONTAINS, "A");
            REQUIRE_SYMBOL(is_symbol, "ABC", SYM_CONTAINS, "B");
            REQUIRE_SYMBOL(is_symbol, "ABC", SYM_CONTAINS, "C");
            REQUIRE_SYMBOL(is_symbol, "ABC", SYM_CONTAINS, "AB");
            REQUIRE_SYMBOL(is_symbol, "ABC", SYM_CONTAINS, "BC");
            REQUIRE_SYMBOL(is_symbol, "ABC", SYM_CONTAINS, "ABC");
            REQUIRE_SYMBOL(is_symbol, "A", SYM_CONTAINS, "");
            REQUIRE_SYMBOL(is_symbol, "", SYM_CONTAINS, "");

            REQUIRE_SYMBOL_FALSE(is_symbol, "", SYM_CONTAINS, "A");
            REQUIRE_SYMBOL_FALSE(is_symbol, "A", SYM_CONTAINS, "B");
            REQUIRE_SYMBOL_FALSE(is_symbol, "abc", SYM_CONTAINS, "C");

            // match
            REQUIRE_SYMBOL(is_symbol, "ABC", SYM_MATCH, ".+");
            REQUIRE_SYMBOL(is_symbol, "A", SYM_MATCH, ".+");
            REQUIRE_SYMBOL_FALSE(is_symbol, "", SYM_MATCH, ".+");
            REQUIRE_SYMBOL(is_symbol, "abcde", SYM_MATCH, "[a-e]+");
            REQUIRE_SYMBOL_FALSE(is_symbol, "abcde", SYM_MATCH, "[a-d]+");
            REQUIRE_SYMBOL(is_symbol, "ABC", SYM_MATCH, ".*");
            REQUIRE_SYMBOL(is_symbol, "", SYM_MATCH, ".*");
            REQUIRE_SYMBOL(is_symbol, "array0", SYM_MATCH, "array`d");
            REQUIRE_SYMBOL(is_symbol, "array9", SYM_MATCH, "array`d");
            REQUIRE_SYMBOL_FALSE(is_symbol, "array10", SYM_MATCH, "array`d");
            REQUIRE_SYMBOL(is_symbol, "array1", SYM_MATCH, "array`d((1..3))");
            REQUIRE_SYMBOL(is_symbol, "array12", SYM_MATCH, "array`d((1..3))");
            REQUIRE_SYMBOL(is_symbol, "array123", SYM_MATCH, "array`d((1..3))");
            REQUIRE_SYMBOL_FALSE(is_symbol, "array1234", SYM_MATCH, "array`d((1..3))");
        }

        SECTION("object")
        {
            CheckerContext ctx;

#define MATCH(x, lst, n)              \
    {                                 \
        auto res = x.check(lst, ctx); \
        REQUIRE(ctx.error == "");     \
        REQUIRE(res.ok);              \
        REQUIRE(res.matched == n);    \
    }

#define NO_MATCH(x, lst)              \
    {                                 \
        auto res = x.check(lst, ctx); \
        REQUIRE_FALSE(res.ok);        \
        REQUIRE(res.matched == 0);    \
    }
        }
    }

    SECTION("OrGroupCheck")
    {
        CheckerContext ctx;

        ArgGroupOr g;
        NO_MATCH(g, L());
        NO_MATCH(g, LF(1));
        ctx.error.clear();

        auto f2 = new ArgIsFloat;
        f2->setRepeats(2);
        g.insertChild(ArgCheckerNodePtr(f2));

        // (f[2])
        MATCH(g, LF(1, 2), 2);
        MATCH(g, LF(1, 2, 3), 2);
        MATCH(g, LA(1, 2, "A"), 2);
        NO_MATCH(g, LF(1));
        NO_MATCH(g, LA(1, "A"));
        NO_MATCH(g, LA(1, "A", 2));

        auto s3 = new ArgIsSymbol;
        s3->setRepeats(3);
        g.insertChild(ArgCheckerNodePtr(s3));

        // (f[2]|s[3])
        MATCH(g, LF(1, 2), 2);
        MATCH(g, LF(1, 2, 3), 2);
        MATCH(g, LA("a", "b", "c"), 3);
        MATCH(g, LA("a", "b", "c", "d"), 3);
        NO_MATCH(g, L());
        NO_MATCH(g, LF(1));
        NO_MATCH(g, LA("a"));
        NO_MATCH(g, LA("a", "b"));

        auto s1 = new ArgIsSymbol;
        s1->setCheck(ArgIsSymbol::SYM_MATCH, "A+");
        g.insertChild(ArgCheckerNodePtr(s1));

        // (f[2]|s[3]|"A+")
        MATCH(g, LF(1, 2), 2);
        MATCH(g, LF(1, 2, 3), 2);
        MATCH(g, LA("a", "b", "c"), 3);
        MATCH(g, LA("a", "b", "c", "d"), 3);
        MATCH(g, LA("A"), 1);
        MATCH(g, LA("AA"), 1);
        MATCH(g, LA("AAA"), 1);
        MATCH(g, LA("A", "???"), 1);
        NO_MATCH(g, L());
        NO_MATCH(g, LF(1));
        NO_MATCH(g, LA(1, "A"));
    }

    SECTION("AndGroupCheck")
    {
        CheckerContext ctx;

        ArgGroupAnd g;
        MATCH(g, L(), 0);
        MATCH(g, LF(1), 0);

        g.insertChild(ArgCheckerNodePtr(new ArgIsFloat));
        g.insertChild(ArgCheckerNodePtr(new ArgIsSymbol));
        g.insertChild(ArgCheckerNodePtr(new ArgIsFloat));

        // f s f
        MATCH(g, LA(1, "abc", 2), 3);
        MATCH(g, LA(1, "abc", 2, 3, 4, 5), 3);
        NO_MATCH(g, L());
        NO_MATCH(g, LA("s"));
        NO_MATCH(g, LA(1));
        NO_MATCH(g, LA(1, 2));
        NO_MATCH(g, LA(1, "s"));
        NO_MATCH(g, LA(1, "s", "s"));
    }

    SECTION("format")
    {
        SECTION("bool")
        {
            SECTION("b")
            {
                ArgChecker ck("b");
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(0)));
                REQUIRE(ck.check(LA("true")));
                REQUIRE(ck.check(LA("false")));
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE_FALSE(ck.check(LF(1, 1)));
                REQUIRE_FALSE(ck.check(LA("ABC")));
            }

            SECTION("b b")
            {
                ArgChecker ck("b b");
                REQUIRE(ck.check(LF(1, 0)));
                REQUIRE(ck.check(LF(0, 1)));
                REQUIRE(ck.check(LA("true", 1)));
                REQUIRE(ck.check(LA("false", 1)));
                REQUIRE_FALSE(ck.check(LF(1)));
                REQUIRE_FALSE(ck.check(LF(1, 2)));
                REQUIRE_FALSE(ck.check(LF(0, 1, 2)));
            }

            SECTION("b+")
            {
                ArgChecker ck("b+");
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(1, 0)));
                REQUIRE(ck.check(LF(1, 0, 1)));
                REQUIRE_FALSE(ck.check(LF(1, 0, 1, 200)));
            }

            SECTION("b*")
            {
                ArgChecker ck("b*");
                REQUIRE(ck.check(L()));
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(1, 0)));
                REQUIRE(ck.check(LF(1, 0, 1)));
                REQUIRE_FALSE(ck.check(LF(1, 0, 1, 200)));
            }

            SECTION("b[2]")
            {
                ArgChecker ck("b[2]");
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE_FALSE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(1, 0)));
                REQUIRE_FALSE(ck.check(LF(1, 100)));
                REQUIRE_FALSE(ck.check(LF(1, 0, 1)));
            }

            SECTION("b[2..]")
            {
                ArgChecker ck("b[2..]");
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE_FALSE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(1, 0)));
                REQUIRE(ck.check(LF(1, 0, 0)));
                REQUIRE(ck.check(LF(1, 0, 0, 1)));
                REQUIRE_FALSE(ck.check(LF(1, 100)));
            }

            SECTION("b[2..4]")
            {
                ArgChecker ck("b[2..4]");
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE_FALSE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(1, 0)));
                REQUIRE(ck.check(LF(1, 0, 0)));
                REQUIRE(ck.check(LF(1, 0, 0, 1)));
                REQUIRE_FALSE(ck.check(LF(1, 0, 0, 1, 1)));
                REQUIRE_FALSE(ck.check(LF(1, 0, 0, 1, 1, 0)));
                REQUIRE_FALSE(ck.check(LF(1, 100)));
            }
        }

        SECTION("float")
        {
            SECTION("f")
            {
                ArgChecker ck("f");
                REQUIRE(ck.check(LF(0)));
                REQUIRE(ck.check(LF(0.5)));
                REQUIRE(ck.check(LF(-0.5)));
                REQUIRE(ck.check(LF(10000)));
                REQUIRE_FALSE(ck.check(LA("ABC")));
            }

            SECTION("f f")
            {
                ArgChecker ck("f f");
                REQUIRE(ck.check(LF(0, -10.5)));
                REQUIRE(ck.check(LF(0.5, 0.001)));
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE_FALSE(ck.check(LF(10000)));
                REQUIRE_FALSE(ck.check(LA(1, "A")));
            }

            SECTION("f b")
            {
                ArgChecker ck("f b");
                REQUIRE(ck.check(LA(100, "true")));
                REQUIRE(ck.check(LF(0.5, 1)));
                REQUIRE_FALSE(ck.check(LF(0.51, 0.5)));
            }

            SECTION("f>0")
            {
                ArgChecker ck("f>0");
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(0.1)));
                REQUIRE_FALSE(ck.check(LF(-0.1)));
                REQUIRE_FALSE(ck.check(LF(0)));
            }

            SECTION("f>0+")
            {
                ArgChecker ck("f>0+");
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(1, 0.1, 0.33)));
                REQUIRE(ck.check(LF(0.1)));
                REQUIRE_FALSE(ck.check(LF(0.1, 1, 0)));
            }

            SECTION("f=1.5")
            {
                ArgChecker ck("f=1.5");
                REQUIRE(ck.check(LF(1.5)));
                REQUIRE_FALSE(ck.check(LF(1.51)));
            }

            SECTION("f!=1")
            {
                ArgChecker ck("f!=1[1]");
                REQUIRE(ck.check(LF(-1)));
                REQUIRE(ck.check(LF(0.9)));
                REQUIRE(ck.check(LF(1.1)));
                REQUIRE_FALSE(ck.check(LF(1)));
            }

            SECTION("f>=0")
            {
                ArgChecker ck("f>=0[2]");
                REQUIRE(ck.check(LF(0, 0)));
                REQUIRE(ck.check(LF(0, 0.1)));
                REQUIRE(ck.check(LF(2, 0.1)));
                REQUIRE_FALSE(ck.check(LF(-0.1, 0.1)));
                REQUIRE_FALSE(ck.check(LF(2, -0.1)));
            }

            SECTION("f=1f=2")
            {
                ArgChecker ck("f=1f=2");
                REQUIRE(ck.check(LF(1, 2)));
            }

            SECTION("f=1[3]f=2")
            {
                ArgChecker ck("f=1[3] f=2");
                REQUIRE(ck.check(LF(1, 1, 1, 2)));
            }
        }

        SECTION("symbol")
        {
            SECTION("s")
            {
                ArgChecker ck("s");
                REQUIRE(ck.check(LA("a")));
                REQUIRE(ck.check(LA("abc")));
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE_FALSE(ck.check(LF(1000)));
                REQUIRE_FALSE(ck.check(LA("a", "b")));
            }

            SECTION("s s?")
            {
                ArgChecker ck("s s?");
                REQUIRE(ck.check(LA("a")));
                REQUIRE(ck.check(LA("abc")));
                REQUIRE(ck.check(LA("a", "b")));
                REQUIRE_FALSE(ck.check(LA("a", "b", "c")));
            }

            SECTION("s s=abc")
            {
                ArgChecker ck("s s=abc");
                REQUIRE(ck.check(LA("a", "abc")));
                REQUIRE(ck.check(LA("b", "abc")));
                REQUIRE_FALSE(ck.check(LA("a")));
                REQUIRE_FALSE(ck.check(LA("abc")));
                REQUIRE_FALSE(ck.check(LA("a", "abcd")));
                REQUIRE_FALSE(ck.check(LA("a", 0xABCD)));
            }

            SECTION("s s!=abc")
            {
                ArgChecker ck("s s!=abc");
                REQUIRE(ck.check(LA("a", "abD")));
                REQUIRE(ck.check(LA("b", "abV")));
                REQUIRE_FALSE(ck.check(LA("b", "abc")));
                REQUIRE_FALSE(ck.check(LA("b", 0xABC)));
                REQUIRE_FALSE(ck.check(LA("a")));
                REQUIRE_FALSE(ck.check(LA("abc")));
            }

            SECTION("^abc")
            {
                ArgChecker ck("^abc");
                REQUIRE(ck.check(LA("abc")));
                REQUIRE(ck.check(LA("abcd")));
                REQUIRE(ck.check(LA("abcd0")));
                REQUIRE_FALSE(ck.check(LA("ab")));
                REQUIRE_FALSE(ck.check(LA("_abc")));
                REQUIRE_FALSE(ck.check(LF(0xABC)));
            }

            SECTION("~abc")
            {
                ArgChecker ck("~abc");
                REQUIRE(ck.check(LA("abc")));
                REQUIRE(ck.check(LA("abcd")));
                REQUIRE(ck.check(LA("aaaabcd")));
                REQUIRE_FALSE(ck.check(LA("ab")));
                REQUIRE_FALSE(ck.check(LA("")));
                REQUIRE_FALSE(ck.check(LF(0xABC)));
            }

            SECTION(".abc$")
            {
                ArgChecker ck(".wav$");
                REQUIRE(ck.check(LA(".wav")));
                REQUIRE(ck.check(LA("1.wav")));
                REQUIRE_FALSE(ck.check(LA("1.wave")));
                REQUIRE_FALSE(ck.check(LA("1.abc")));
                REQUIRE_FALSE(ck.check(LF(0xABC)));
            }

            SECTION("\"b+\"")
            {
                ArgChecker ck("\"b+\"");
                REQUIRE(ck.check(LA("b")));
                REQUIRE(ck.check(LA("bb")));
                REQUIRE(ck.check(LA("bbbbbbbb")));
                REQUIRE_FALSE(ck.check(LA("B")));
                REQUIRE_FALSE(ck.check(LA("bee")));
            }
        }

        SECTION("atom")
        {
            SECTION("a")
            {
                ArgChecker ck("a");
                REQUIRE(ck.check(LA("a")));
                REQUIRE(ck.check(LA("abc")));
                REQUIRE(ck.check(LA(1)));
                REQUIRE(ck.check(LA(-1)));
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE_FALSE(ck.check(LF(1, 2)));
                REQUIRE_FALSE(ck.check(LA("a", "b")));
            }
        }

        SECTION("and group")
        {
            SECTION("f f*")
            {
                ArgChecker ck("f f*");
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(1, 2)));
                REQUIRE(ck.check(LF(1, 2, 3)));
                REQUIRE_FALSE(ck.check(LA("a")));
                REQUIRE_FALSE(ck.check(LA(1, "a")));
            }

            SECTION("s+ f+")
            {
                ArgChecker ck("s+ f+");
                REQUIRE(ck.check(LA("a", 1)));
                REQUIRE(ck.check(LA("a", "b", "c", 1)));
                REQUIRE(ck.check(LA("a", 1, 2, 3, 4)));
                REQUIRE(ck.check(LA("a", "b", "c", 1, 2, 3)));
                REQUIRE_FALSE(ck.check(LA("a", "b", "c", 1, 2, 3, "?")));
                REQUIRE_FALSE(ck.check(LA(1, "a", "b", "c", 1, 2, 3)));
            }

            SECTION("s* f*")
            {
                ArgChecker ck("s* f*");
                REQUIRE(ck.check(L()));
                REQUIRE(ck.check(LA("a")));
                REQUIRE(ck.check(LA("a", "b", "c")));
                REQUIRE(ck.check(LA(1)));
                REQUIRE(ck.check(LA(1, 2, 3, 4)));
                REQUIRE(ck.check(LA("a", 1)));
                REQUIRE(ck.check(LA("a", 1, 2, 3)));
                REQUIRE(ck.check(LA("a", "b", 1, 2, 3)));

                REQUIRE_FALSE(ck.check(LA(1, "a", "b", "c")));
                REQUIRE_FALSE(ck.check(LA(1, 2, 3, "a")));
            }

            SECTION("f* f=1")
            {
                ArgChecker ck("f* f=1");
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE_FALSE(ck.check(LF(1)));
                REQUIRE_FALSE(ck.check(LF(3, 2, 1)));
            }

            SECTION("f!=1* f=1")
            {
                ArgChecker ck("f!=1* f=1");
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(2, 1)));
                REQUIRE(ck.check(LF(3, 2, 1)));
                REQUIRE(ck.check(LF(4, 3, 2, 1)));
            }
        }

        SECTION("or group")
        {
            SECTION("f|s")
            {
                ArgChecker ck("(f|s)");
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LA("X")));
                REQUIRE_FALSE(ck.check(LA(1, "X")));
            }

            SECTION("(f|b)")
            {
                ArgChecker ck("(f|b)");
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LA("true")));
                REQUIRE(ck.check(LA("false")));
                REQUIRE_FALSE(ck.check(LA("???")));
            }

            SECTION("(s|b)")
            {
                ArgChecker ck("(s|b)");
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LA("true")));
                REQUIRE(ck.check(LA("false")));
                REQUIRE_FALSE(ck.check(LA(2)));
                REQUIRE_FALSE(ck.check(LA(2, 3)));
            }

            SECTION("(f=1|f=2|f=3)")
            {
                ArgChecker ck("(f=1|f=2|f=3)");
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(2)));
                REQUIRE(ck.check(LF(3)));
                REQUIRE_FALSE(ck.check(LF(4)));
                REQUIRE_FALSE(ck.check(LA("ABC")));
            }

            SECTION("(f=1|f=2)+")
            {
                ArgChecker ck("(f=1|f=2)+");
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(1, 1)));
                REQUIRE(ck.check(LF(2, 1)));
                REQUIRE(ck.check(LF(2, 1, 1)));
                REQUIRE(ck.check(LF(2, 2, 2, 2)));
            }

            SECTION("(f=1|f=2)*")
            {
                ArgChecker ck("(f=1|f=2)*");
                REQUIRE(ck.check(L()));
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(1, 1)));
                REQUIRE(ck.check(LF(2, 1)));
                REQUIRE(ck.check(LF(2, 1, 1)));
                REQUIRE(ck.check(LF(2, 2, 2, 2)));
            }

            SECTION("(f=1|f=2)?")
            {
                ArgChecker ck("(f=1|f=2)?");
                REQUIRE(ck.check(L()));
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(2)));
                REQUIRE_FALSE(ck.check(LF(2, 1)));
                REQUIRE_FALSE(ck.check(LF(2, 1, 1)));
                REQUIRE_FALSE(ck.check(LF(2, 2, 2, 2)));
            }

            SECTION("(f=1|f=2)[2]")
            {
                ArgChecker ck("(f=1|f=2)[2]");
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE_FALSE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(1, 1)));
                REQUIRE(ck.check(LF(1, 2)));
                REQUIRE(ck.check(LF(2, 1)));
                REQUIRE(ck.check(LF(2, 2)));
                REQUIRE_FALSE(ck.check(LF(2, 1, 1)));
                REQUIRE_FALSE(ck.check(LF(2, 2, 2, 2)));
            }

            SECTION("(f=1|f=2)[2..]")
            {
                ArgChecker ck("(f=1|f=2)[2..]");
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE_FALSE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(1, 1)));
                REQUIRE(ck.check(LF(1, 2)));
                REQUIRE(ck.check(LF(2, 1)));
                REQUIRE(ck.check(LF(2, 2)));
                REQUIRE(ck.check(LF(2, 1, 1)));
                REQUIRE(ck.check(LF(2, 2, 2, 2)));
            }

            SECTION("(f=1|f=2)[2..3]")
            {
                ArgChecker ck("(f=1|f=2)[2..3]");
                REQUIRE_FALSE(ck.check(L()));
                REQUIRE_FALSE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(1, 1)));
                REQUIRE(ck.check(LF(1, 2)));
                REQUIRE(ck.check(LF(2, 1)));
                REQUIRE(ck.check(LF(2, 2)));
                REQUIRE(ck.check(LF(2, 1, 1)));
                REQUIRE_FALSE(ck.check(LF(2, 2, 2, 2)));
            }

            SECTION("(f=1|f=2)[1..2]")
            {
                ArgChecker ck("(f=1|f=2)[1..2]");
                REQUIRE(ck.check(LF(1)));
                REQUIRE(ck.check(LF(1, 1)));
                REQUIRE(ck.check(LF(2)));
                REQUIRE(ck.check(LF(2, 1)));
                REQUIRE(ck.check(LF(2, 2)));
                REQUIRE(ck.check(LF(1, 2)));
                REQUIRE_FALSE(ck.check(LF(1, 10)));
            }
        }

        SECTION("\"b+\"")
        {
            ArgChecker ck("\"b+\"");
            REQUIRE(ck.check(LA("b")));
            REQUIRE(ck.check(LA("bb")));
            REQUIRE(ck.check(LA("bbbbbbbb")));
            REQUIRE_FALSE(ck.check(LA("B")));
            REQUIRE_FALSE(ck.check(LA("bee")));
        }

        SECTION("'b+'")
        {
            ArgChecker ck("'b+'");
            REQUIRE(ck.check(LA("b")));
            REQUIRE(ck.check(LA("bb")));
            REQUIRE(ck.check(LA("bbbbbbbb")));
            REQUIRE_FALSE(ck.check(LA("B")));
            REQUIRE_FALSE(ck.check(LA("bee")));
        }

        SECTION("````")
        {
            ArgChecker ck("'(`'a`' )+'");
            REQUIRE(ck.check(LA("'a' ")));
            REQUIRE(ck.check(LA("'a' 'a' ")));
        }

        SECTION("array`d+")
        {
            ArgChecker ck("'array`d+'");
            REQUIRE(ck.check(LA("array0")));
            REQUIRE(ck.check(LA("array1230")));
            REQUIRE_FALSE(ck.check(LA("arrayABC")));
        }
    }

    SECTION("data")
    {
        SECTION("d")
        {
            ArgChecker ck("d");
            REQUIRE_FALSE(ck.check(LF(1)));
            REQUIRE_FALSE(ck.check(LA("abc")));
        }

        SECTION("d=String")
        {
            ArgChecker ck("d=String");
            REQUIRE_FALSE(ck.check(LF(1)));
            REQUIRE_FALSE(ck.check(LA("abc")));
        }
    }

    SECTION("int")
    {
        SECTION("i")
        {
            ArgChecker ck("i");
            REQUIRE(ck.check(LF(-1)));
            REQUIRE_FALSE(ck.check(LF(-2.5)));
            REQUIRE_FALSE(ck.check(LA("A")));
        }

        SECTION("i2..4")
        {
            ArgChecker ck("i2..4");
            REQUIRE_FALSE(ck.check(LF(1)));
            REQUIRE(ck.check(LF(2)));
            REQUIRE(ck.check(LF(3)));
            REQUIRE(ck.check(LF(4)));
            REQUIRE_FALSE(ck.check(LF(5)));
            REQUIRE_FALSE(ck.check(LF(2.1)));
        }

        SECTION("i=-1")
        {
            ArgChecker ck("i=-1");
            REQUIRE(ck.check(LF(-1)));
            REQUIRE_FALSE(ck.check(LF(-2)));
        }

        SECTION("i!=3")
        {
            ArgChecker ck("i!=3");
            REQUIRE(ck.check(LF(4)));
            REQUIRE_FALSE(ck.check(LF(3)));
        }

        SECTION("i>0")
        {
            ArgChecker ck("i>0");
            REQUIRE(ck.check(LF(1)));
            REQUIRE_FALSE(ck.check(LF(0)));
        }

        SECTION("i>=0")
        {
            ArgChecker ck("i>=0");
            REQUIRE(ck.check(LF(1)));
            REQUIRE(ck.check(LF(0)));
            REQUIRE_FALSE(ck.check(LF(-1)));
        }

        SECTION("i<10000")
        {
            ArgChecker ck("i<10000");
            REQUIRE(ck.check(LF(1)));
            REQUIRE_FALSE(ck.check(LF(10000)));
        }

        SECTION("i<=19")
        {
            ArgChecker ck("i<=19");
            REQUIRE(ck.check(LF(18)));
            REQUIRE(ck.check(LF(19)));
            REQUIRE_FALSE(ck.check(LF(20)));
        }

        SECTION("i-1..1")
        {
            ArgChecker ck("i-1..1");
            REQUIRE_FALSE(ck.check(LF(-2)));
            REQUIRE(ck.check(LF(-1)));
            REQUIRE(ck.check(LF(0)));
            REQUIRE(ck.check(LF(1)));
            REQUIRE_FALSE(ck.check(LF(2)));
        }

        SECTION("i2..4+")
        {
            ArgChecker ck("i2..4+");
            REQUIRE(ck.check(LF(2, 3, 4)));
        }

        SECTION("i2..4[1..2]")
        {
            ArgChecker ck("i2..4[1..2]");
            REQUIRE(ck.check(LF(2)));
            REQUIRE(ck.check(LF(2, 3)));
        }

        SECTION("i%2=0")
        {
            ArgChecker ck("i%2=0");
            REQUIRE(ck.check(LF(0)));
            REQUIRE(ck.check(LF(2)));
            REQUIRE(ck.check(LF(4)));
            REQUIRE(ck.check(LF(-2)));
            REQUIRE_FALSE(ck.check(LF(1)));
            REQUIRE_FALSE(ck.check(LF(-1)));
        }

        SECTION("i%0=0")
        {
            ArgChecker ck("i%0=0");
            REQUIRE_FALSE(ck.check(LF(0)));
            REQUIRE_FALSE(ck.check(LF(2)));
            REQUIRE_FALSE(ck.check(LF(4)));
            REQUIRE_FALSE(ck.check(LF(-2)));
            REQUIRE_FALSE(ck.check(LF(1)));
            REQUIRE_FALSE(ck.check(LF(-1)));
        }

        SECTION("i^2")
        {
            ArgChecker ck("i^2");
            REQUIRE(ck.check(LF(1)));
            REQUIRE(ck.check(LF(2)));
            REQUIRE(ck.check(LF(4)));
            REQUIRE(ck.check(LF(8)));
            REQUIRE(ck.check(LF(16)));
            REQUIRE(ck.check(LF(32)));
            REQUIRE(ck.check(LF(256)));
            REQUIRE(ck.check(LF(512)));
            REQUIRE_FALSE(ck.check(LF(0)));
            REQUIRE_FALSE(ck.check(LF(3)));
            REQUIRE_FALSE(ck.check(LF(-1)));
        }
    }
}
