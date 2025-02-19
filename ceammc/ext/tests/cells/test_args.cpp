/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "args/argcheck.h"
#include "ceammc_atomlist.h"
#include "ceammc_atomlist_view.h"
#include "ceammc_data.h"
#include "datatype_string.h"

#include "catch.hpp"
#include "test_common.h"

#include <boost/preprocessor.hpp>

using namespace ceammc;

TEST_CASE("args2", "[core]")
{
    SECTION("main")
    {
        REQUIRE(args::check_args("a", Atom(1)));
        REQUIRE(args::check_args("a?", Atom(1)));
        REQUIRE(args::check_args("a+", Atom(1)));
        REQUIRE(args::check_args("a*", Atom(1)));
        REQUIRE(args::check_args("a{0,}", Atom(1)));
        REQUIRE(args::check_args("a{1,}", Atom(1)));
        REQUIRE(args::check_args("a{0,1}", Atom(1)));
        REQUIRE_FALSE(args::check_args("a a", Atom(100)));
        REQUIRE(args::check_args("a a?", Atom(100)));
        REQUIRE(args::check_args("a a*", Atom(100)));
        REQUIRE(args::check_args("a a{0,100}", Atom(100)));
        REQUIRE(args::check_args("a? a?", Atom(100)));
        REQUIRE(args::check_args("a? a?", LF(1, 2)));
        REQUIRE(args::check_args("a+", LF(1, 2)));
        REQUIRE_FALSE(args::check_args("a{3,}", LF(1, 2)));
        REQUIRE(args::check_args("a{3,}", LF(1, 2, 3)));
        REQUIRE_FALSE(args::check_args("a?", LF(1, 2, 3)));
        REQUIRE(args::check_args("a{2}", LF(1, 2)));
        REQUIRE_FALSE(args::check_args("a{2}", LF(1)));
        REQUIRE_FALSE(args::check_args("a{2}", LF(1, 2, 3)));
    }

    SECTION("bool")
    {
        REQUIRE_FALSE(args::check_args("B", Atom()));
        REQUIRE_FALSE(args::check_args("B", L()));
        REQUIRE_FALSE(args::check_args("B", LF(-1)));
        REQUIRE_FALSE(args::check_args("B", LA("abc")));

        REQUIRE(args::check_args("B", LF(0)));
        REQUIRE(args::check_args("B", LF(1)));
        REQUIRE(args::check_args("B", LA("true")));
        REQUIRE(args::check_args("B", LA("false")));
        REQUIRE_FALSE(args::check_args("B", LF(1.5)));
        REQUIRE_FALSE(args::check_args("B", LF(2)));
        REQUIRE_FALSE(args::check_args("B", LF(-0.1)));

        REQUIRE(args::check_args("B B", LF(0, 1)));
        REQUIRE(args::check_args("B+", LF(0, 1)));
    }

    SECTION("byte")
    {
        REQUIRE_FALSE(args::check_args("b", Atom()));
        REQUIRE_FALSE(args::check_args("b", L()));
        REQUIRE_FALSE(args::check_args("b", LF(-1)));
        REQUIRE_FALSE(args::check_args("b", LA("abc")));

        REQUIRE(args::check_args("b", LF(0)));
        REQUIRE(args::check_args("b", LF(1)));
        REQUIRE(args::check_args("b", LF(2)));
        REQUIRE(args::check_args("b", LF(255)));

        REQUIRE_FALSE(args::check_args("b", LF(256)));
        REQUIRE_FALSE(args::check_args("b", LF(1.5)));
        REQUIRE_FALSE(args::check_args("b", LF(-0.1)));
    }

    SECTION("int")
    {
        REQUIRE_FALSE(args::check_args("i", Atom()));
        REQUIRE_FALSE(args::check_args("i", L()));
        REQUIRE_FALSE(args::check_args("i", LA("abc")));

        REQUIRE(args::check_args("i", LF(0)));
        REQUIRE(args::check_args("i", LF(1)));
        REQUIRE(args::check_args("i", LF(2)));
        REQUIRE(args::check_args("i", LF(1000)));
        REQUIRE(args::check_args("i", LF(-11000)));

        REQUIRE_FALSE(args::check_args("i", LF(1.5)));
        REQUIRE_FALSE(args::check_args("i", LF(-0.1)));

        REQUIRE(args::check_args("i<1000", LF(999)));
        REQUIRE(args::check_args("i<1000", LF(0)));
        REQUIRE(args::check_args("i<1000", LF(-100)));
        REQUIRE_FALSE(args::check_args("i<1000", LF(1000)));

        REQUIRE_FALSE(args::check_args("i<-10", LF(-9)));
        REQUIRE_FALSE(args::check_args("i<-10", LF(-10)));
        REQUIRE(args::check_args("i<-10", LF(-11)));

        REQUIRE(args::check_args("i>100", LF(101)));
        REQUIRE_FALSE(args::check_args("i>100", LF(100)));
        REQUIRE_FALSE(args::check_args("i>100", LF(99)));

        REQUIRE(args::check_args("i>=100", LF(101)));
        REQUIRE(args::check_args("i>=100", LF(100)));
        REQUIRE_FALSE(args::check_args("i>=100", LF(99)));

        REQUIRE(args::check_args("i<=100", LF(99)));
        REQUIRE(args::check_args("i<=100", LF(100)));
        REQUIRE_FALSE(args::check_args("i<=100", LF(101)));

        REQUIRE(args::check_args("i=-13", LF(-13)));
        REQUIRE_FALSE(args::check_args("i=-13", LF(-12)));
        REQUIRE_FALSE(args::check_args("i=-13", LF(-11)));

        REQUIRE(args::check_args("i=1|2|3", LF(1)));
        REQUIRE(args::check_args("i=1|2|3", LF(2)));
        REQUIRE(args::check_args("i=1|2|3", LF(3)));
        REQUIRE_FALSE(args::check_args("i=1|2|3", LF(4)));
        REQUIRE_FALSE(args::check_args("i=1|2|3", LF(0)));
        REQUIRE(args::check_args("i=-1|2|-3", LF(-1)));
        REQUIRE(args::check_args("i=-1|2|-3", LF(-3)));
        REQUIRE(args::check_args("i=-1|2|-3", LF(2)));
        REQUIRE(args::check_args("i=11|-12|+123", LF(11)));
        REQUIRE(args::check_args("i=11|-12|+123", LF(-12)));
        REQUIRE(args::check_args("i=11|-12|+123", LF(123)));
        REQUIRE_FALSE(args::check_args("i=-1|2|-3", LF(1)));
        REQUIRE_FALSE(args::check_args("i=-1|2|-3", LF(3)));
        REQUIRE_FALSE(args::check_args("i=-1|2|-3", LF(-2)));

        REQUIRE_FALSE(args::check_args("i[-1,1]", LF(-2)));
        REQUIRE(args::check_args("i[-1,1]", LF(-1)));
        REQUIRE(args::check_args("i[-1,1]", LF(0)));
        REQUIRE(args::check_args("i[-1,1]", LF(1)));
        REQUIRE_FALSE(args::check_args("i[-1,1]", LF(2)));
        REQUIRE(args::check_args("i[-100,100]", LF(2)));
        REQUIRE(args::check_args("i[-100,100]", LF(-100)));
        REQUIRE(args::check_args("i[-100,100]", LF(100)));
        REQUIRE_FALSE(args::check_args("i[-100,100]", LF(101)));
        REQUIRE_FALSE(args::check_args("i[-100,100]", LF(-101)));

        REQUIRE_FALSE(args::check_args("i[-1,1)", LF(-2)));
        REQUIRE(args::check_args("i[-1,1)", LF(-1)));
        REQUIRE(args::check_args("i[-1,1)", LF(0)));
        REQUIRE_FALSE(args::check_args("i[-1,1)", LF(1)));
        REQUIRE_FALSE(args::check_args("i[-1,1)", LF(2)));

        REQUIRE(args::check_args("i!=0", LF(1)));
        REQUIRE(args::check_args("i!=0", LF(-1)));
        REQUIRE_FALSE(args::check_args("i!=0", LF(0)));

        REQUIRE(args::check_args("i%2", LF(2)));
        REQUIRE(args::check_args("i%2", LF(6)));
        REQUIRE(args::check_args("i%2", LF(1024)));
        REQUIRE(args::check_args("INT:i%32", LF(1024)));
        REQUIRE_FALSE(args::check_args("i%31", LF(1024)));
        REQUIRE_FALSE(args::check_args("i%2", LF(1)));
        REQUIRE_FALSE(args::check_args("i%2", LF(1111)));
        REQUIRE_FALSE(args::check_args("i%0", LF(1)));
        REQUIRE_FALSE(args::check_args("i%-123", LF(1)));

        REQUIRE(args::check_args("i^2", LF(1)));
        REQUIRE(args::check_args("i^2", LF(2)));
        REQUIRE(args::check_args("i^2", LF(8)));
        REQUIRE(args::check_args("i^2", LF(1024)));
        REQUIRE_FALSE(args::check_args("i^2", LF(3)));
        REQUIRE_FALSE(args::check_args("i^2", LF(6)));
    }

    SECTION("symbol")
    {
        REQUIRE_FALSE(args::check_args("s", Atom()));
        REQUIRE_FALSE(args::check_args("NAME:s", L()));
        REQUIRE_FALSE(args::check_args("s", LF(123)));

        REQUIRE(args::check_args("s", LA("")));
        REQUIRE(args::check_args("s", LA("ABC")));

        REQUIRE(args::check_args("s=A", LA("A")));
        REQUIRE(args::check_args("s=A:", LA("A:")));
        REQUIRE(args::check_args("s=\"A\"", LA("A")));
        REQUIRE(args::check_args("s='A'", LA("A")));
        REQUIRE(args::check_args("s='A:'", LA("A:")));
        REQUIRE(args::check_args("s='id:'", LA("id:")));
        REQUIRE(args::check_args("s='A||'", LA("A||")));
        REQUIRE(args::check_args("s=\"any!12 3|||\"", LA("any!12 3|||")));
        REQUIRE(args::check_args("s='ABCDEFGHJKLOMNP'", LA("ABCDEFGHJKLOMN")));
        REQUIRE_FALSE(args::check_args("s=A", LA("AB")));
        REQUIRE(args::check_args("s=A|B#|@c", LA("A")));
        REQUIRE(args::check_args("s=A|B#|@c", LA("B#")));
        REQUIRE(args::check_args("s=A|B#|@c", LA("@c")));
        REQUIRE_FALSE(args::check_args("MODE:s=A|B#|@c", LA("@d")));

        REQUIRE(args::check_args("s s", LA("A", "B")));
        REQUIRE(args::check_args("s{2}", LA("A", "B")));
        REQUIRE(args::check_args("s{1,}", LA("A", "B")));
        REQUIRE(args::check_args("s{1,2}", LA("A", "B")));
        REQUIRE(args::check_args("s+", LA("A", "B")));
        REQUIRE(args::check_args("s+", LA("A", "B", "C")));
        REQUIRE(args::check_args("s s*", LA("A")));
        REQUIRE(args::check_args("s s*", LA("A", "B")));
        REQUIRE(args::check_args("s s*", LA("A", "B", "C")));
        REQUIRE(args::check_args("s s*", LA("A", "B", "C", "D")));
        REQUIRE(args::check_args("s s*", LA("A", "@B")));
        REQUIRE(args::check_args("s s*", LA("A", "@B", "C")));
        REQUIRE(args::check_args("s s+", LA("A", "test")));
        REQUIRE(args::check_args("A:s B?:s*", LA("A", "B", "C")));
    }

    SECTION("time")
    {
        REQUIRE_FALSE(args::check_args("t", Atom()));
        REQUIRE_FALSE(args::check_args("TIME:t", L()));
        REQUIRE_FALSE(args::check_args("t", LA("abc")));

        REQUIRE(args::check_args("t", LF(0)));
        REQUIRE(args::check_args("t", LF(100)));
        REQUIRE_FALSE(args::check_args("t", LF(-100)));

        REQUIRE(args::check_args("t", LA("0ms")));
        REQUIRE(args::check_args("t", LA("0msec")));
        REQUIRE(args::check_args("t", LA("0s")));
        REQUIRE(args::check_args("t", LA("0sec")));
        REQUIRE(args::check_args("t", LA("123samp")));
        REQUIRE(args::check_args("t", LA("123.5_ms")));
        REQUIRE(args::check_args("t", LA("0.123_sec")));
    }

    SECTION("float")
    {
        REQUIRE_FALSE(args::check_args("f", Atom()));
        REQUIRE_FALSE(args::check_args("f", L()));
        REQUIRE_FALSE(args::check_args("IDX:f", LA("abc")));

        REQUIRE(args::check_args("f", LF(-0)));
        REQUIRE(args::check_args("f", LF(1.5)));
        REQUIRE(args::check_args("f", LF(2.25)));
        REQUIRE(args::check_args("f", LF(+1000)));
        REQUIRE(args::check_args("f", LF(-11000.0)));
        REQUIRE(args::check_args("f", LF(0.1e+32)));

        REQUIRE_FALSE(args::check_args("f>+2.5", LF(2.4999)));
        REQUIRE_FALSE(args::check_args("f>+2.5", LF(2.5)));
        REQUIRE(args::check_args("f>+2.5", LF(2.5001)));

        REQUIRE_FALSE(args::check_args("f>2.5", LF(2.4999)));
        REQUIRE_FALSE(args::check_args("f>2.5", LF(2.5)));
        REQUIRE(args::check_args("f>2.5", LF(2.5001)));

        REQUIRE_FALSE(args::check_args("f>20", LF(19.999)));
        REQUIRE_FALSE(args::check_args("f>20", LF(20)));
        REQUIRE(args::check_args("f>20", LF(20.001)));

        REQUIRE_FALSE(args::check_args("f>-10", LF(-10.001)));
        REQUIRE_FALSE(args::check_args("f>-10", LF(-10)));
        REQUIRE(args::check_args("f>-10", LF(-9.999)));

        REQUIRE_FALSE(args::check_args("f>-10.0", LF(-10.001)));
        REQUIRE_FALSE(args::check_args("f>-10.0", LF(-10)));
        REQUIRE(args::check_args("f>-10.0", LF(-9.999)));

        REQUIRE_FALSE(args::check_args("f>=-10.0", LF(-10.001)));
        REQUIRE(args::check_args("f>=-10.0", LF(-10)));
        REQUIRE(args::check_args("f>=-10.0", LF(-9.999)));

        REQUIRE_FALSE(args::check_args("f<-10.5", LF(-10.499)));
        REQUIRE_FALSE(args::check_args("f<-10.5", LF(-10.5)));
        REQUIRE(args::check_args("f<-10.5", LF(-10.501)));

        REQUIRE_FALSE(args::check_args("FREQ:f<=-10.5", LF(-10.499)));
        REQUIRE(args::check_args("f<=-10.5", LF(-10.5)));
        REQUIRE(args::check_args("f<=-10.5", LF(-10.501)));

        REQUIRE_FALSE(args::check_args("FREQ:f[1,2.5]", LF(0)));
        REQUIRE(args::check_args("FREQ:f[1,2.5]", LF(1)));
        REQUIRE(args::check_args("FREQ:f[1,2.5]", LF(2)));
        REQUIRE(args::check_args("FREQ:f[1,2.5]", LF(2.5)));
        REQUIRE_FALSE(args::check_args("FREQ:f[1,2.5]", LF(2.5001)));

        REQUIRE_FALSE(args::check_args("FREQ:f[1,2.5)", LF(0)));
        REQUIRE(args::check_args("FREQ:f[1,2.5)", LF(1)));
        REQUIRE(args::check_args("FREQ:f[1,2.5)", LF(2)));
        REQUIRE_FALSE(args::check_args("FREQ:f[1,2.5)", LF(2.5)));
        REQUIRE_FALSE(args::check_args("FREQ:f[1,2.5]", LF(2.5001)));

        REQUIRE_FALSE(args::check_args("FREQ:f!=0", LF(0)));
        REQUIRE(args::check_args("FREQ:f!=0", LF(0.0001)));

        REQUIRE_FALSE(args::check_args("FREQ:f!=-2.5", LF(-2.5)));
        REQUIRE(args::check_args("FREQ:f!=0", LF(-2.50001)));

        REQUIRE(args::check_args("FREQ:f=+1.25", LF(1.25)));
        REQUIRE(args::check_args("FREQ:f=-1.25", LF(-1.25)));
        REQUIRE(args::check_args("FREQ:f=-1.25|+1.25", LF(-1.25)));
        REQUIRE(args::check_args("FREQ:f=-1.25|+1.25", LF(1.25)));
        REQUIRE_FALSE(args::check_args("FREQ:f=-1.25|+1.25", LF(-2.25)));

        REQUIRE(args::check_args("FREQ:f~-2.5", LF(-2.5)));
        REQUIRE(args::check_args("FREQ:f~-2.5", LF(-2.500001)));
        REQUIRE(args::check_args("FREQ:f~-2.5", LF(-2.4999999)));
        REQUIRE_FALSE(args::check_args("FREQ:f~-2.5", LF(-2.4)));
    }

    SECTION("mixed")
    {
        args::ArgMatchList ml;
        REQUIRE(args::check_args("a f<0 i+ s+", LA("true", -1.5, 1, 2, 3, 4, 5, "A", "A", "B"), nullptr, &ml));
        REQUIRE(ml.size() == 4);
        REQUIRE(args::check_args("a f<0 i+ s=A|BC*", LA("true", -1.5, 2, 1, 2, 3, 4, 5, "A", "A", "BC")));
        REQUIRE(args::check_args("A:a f<0 i[1,5]+ NAME:s=A|BC+ f", LA("true", -1.5, 2, 1, 2, 3, 4, 5, "A", "A", "BC", -100), nullptr, &ml));
        REQUIRE(ml.size() == 5);
    }

    SECTION("matches")
    {
        args::ArgMatchList ml;

#define REQUIRE_MATCH(fmt, lst, n, ...)                     \
    {                                                       \
        args::ArgMatchList ml;                              \
        AtomList largs = lst;                               \
        args::check_args(fmt, largs, nullptr, &ml);         \
        REQUIRE(ml.size() == n);                            \
        REQUIRE(ml == args::ArgMatchList({ __VA_ARGS__ })); \
    }

        REQUIRE_MATCH("i?", LF(500), 1, LF(500));
        REQUIRE_MATCH("i?", L(), 1, L());
        REQUIRE_MATCH("i{2,3} i?", LF(1, 2), 2, LF(1, 2), L());
        REQUIRE_MATCH("i{2,3000} i?", LF(1, 2), 2, LF(1, 2), L());
        REQUIRE_MATCH("i{2,3} i?", LF(1, 2, 3), 2, LF(1, 2, 3), L());
        REQUIRE_MATCH("i{2,3} i? s*", LA(1, 2, 3), 3, LF(1, 2, 3), L(), L());
        REQUIRE_MATCH("i{2,3} i? s*", LA(1, 2, 3, "A"), 3, LF(1, 2, 3), L(), LA("A"));
        REQUIRE_MATCH("i{2,3} i? s*", LA(1, 2, 3, "A", "B"), 3, LF(1, 2, 3), L(), LA("A", "B"));
        REQUIRE_MATCH("i a*", LA(1, "A", "B"), 2, LF(1), LA("A", "B"));
    }

    SECTION("ArgCheck")
    {
        using namespace args;
        REQUIRE(ArgChecker("FILE:s OPTS:s*").check(LA("FILE"), nullptr));
        REQUIRE(ArgChecker("FILE:s OPTS:s*").check(LA("FILE", "ARGS"), nullptr));
        REQUIRE(ArgChecker("FILE:s OPTS:s*").check(LA("FILE", "A1", "A2"), nullptr));
        REQUIRE(ArgChecker("FILE:s OPTS:s*").check(LA("FILE", "@param", "A2"), nullptr));
        REQUIRE(!ArgChecker("FILE:s OPTS:s*").check(LA("FILE", "@param", 10), nullptr));
    }
}
