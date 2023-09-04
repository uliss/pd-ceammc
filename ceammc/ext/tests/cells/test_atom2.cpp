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
#include "ceammc_atom.h"
#include "ceammc_data.h"
#include "test_base.h"

#include "catch.hpp"

using namespace ceammc;

static Atom DOLL_SYM(const char* s)
{
    return Atom::dollarSymbol(gensym(s));
}

TEST_CASE("Atom2", "[core]")
{
    SECTION("data")
    {
        SECTION("nullptr")
        {
            Atom a((AbstractData*)nullptr);
            REQUIRE(a.isNone());
            REQUIRE_FALSE(a.isData());
            REQUIRE_FALSE(a.isDataType(IntData::staticType()));
            REQUIRE_FALSE(a.isA<IntData>());
        }

        SECTION("IntData")
        {
            Atom a(new IntData(100));
            REQUIRE(a.isData());
            REQUIRE(a.isA<IntData>());
            REQUIRE(a.isDataType(IntData::staticType()));
            REQUIRE_FALSE(a.isDataType(StrData::staticType()));

            REQUIRE_FALSE(a.isNone());
            REQUIRE_FALSE(a.isBool());
            REQUIRE_FALSE(a.isFloat());
            REQUIRE_FALSE(a.isSymbol());
            REQUIRE_FALSE(a.isInteger());
            REQUIRE_FALSE(a.isProperty());

            REQUIRE(a.asBool() == false);
            REQUIRE(a.asBool(true));
            REQUIRE(a.asBool(false) == false);
            REQUIRE(a.asFloat(-10) == -10);
            REQUIRE(a.asSymbol() == &s_);
            REQUIRE(a.asData());
            REQUIRE(a.asData()->type() == IntData::staticType());
            REQUIRE(a.asData()->toString() == "IntData(100)");
            REQUIRE(a.asData()->toListString() == "IntData(100)");
            REQUIRE(a.asData()->toDictString() == "IntData[value: 100]");

            REQUIRE(a.asDataT<IntData>());
            REQUIRE(a.asDataT<IntData>()->value() == 100);

            REQUIRE(a == a);
            REQUIRE_FALSE(a != a);
        }

        SECTION("copy ctor")
        {
            Atom a0(new IntData(200));
            REQUIRE(a0.isData());
            REQUIRE(a0.isDataType(IntData::staticType()));
            REQUIRE(a0.dataType() == IntData::staticType());

            Atom a1(a0);
            REQUIRE(a1.isData());
            REQUIRE(a1.isDataType(IntData::staticType()));
            REQUIRE(a1.dataType() == IntData::staticType());

            REQUIRE(a0.asData() == a1.asData());

            REQUIRE(a0 == a1);
            REQUIRE_FALSE(a0 != a1);
        }

        SECTION("operator=")
        {
            Atom a0(new IntData(200));
            Atom a1;

            REQUIRE(a1.isNone());
            a1 = a0;
            REQUIRE(a1.isData());
            REQUIRE(a0.asData() == a1.asData());
            REQUIRE(a0.refCount() == 2);

            REQUIRE(a0 == a1);
            REQUIRE_FALSE(a0 != a1);

            // self assign
            a1 = a1;
            REQUIRE(a1 == a1);
            REQUIRE(a1 == a0);
            a1 = Atom(a0);

            REQUIRE(a1 == a1);

            Atom a2 = a1;
            a0 = a1;
            REQUIRE(a0 == a1);
            REQUIRE(a1 == a2);
            REQUIRE(a2 == a0);
        }

        SECTION("t_atom copy")
        {
            Atom a0(new IntData(200));
            Atom a1(a0.atom());
            REQUIRE(a1.isData());
            REQUIRE(a0.refCount() == 2);
        }

        SECTION("move ctor")
        {
            Atom a(100);
            Atom b = std::move(a);

            REQUIRE(a.isNone());
            REQUIRE(b.isFloat());
            REQUIRE(b.asFloat() == 100);

            Atom c(new IntData(100));
            Atom d = std::move(c);
            REQUIRE(c.isNone());
            REQUIRE(d.isData());
            REQUIRE(d.asDataT<IntData>()->value() == 100);

            std::vector<Atom> v;
            v.emplace_back(new IntData(200));
            v.emplace_back(new IntData(300));
        }

        SECTION("move assign")
        {
            Atom a(100);
            Atom b(200);

            a = std::move(b);
            REQUIRE(a.isFloat());
            REQUIRE(a == 200);
            REQUIRE(b.isNone());

            // self move
            a = std::move(a);
            REQUIRE(a.isFloat());
            REQUIRE(a.refCount() == 0);

            Atom c(new StrData("abc"));
            Atom d(new StrData("def"));
            c = std::move(d);
            REQUIRE(c.isData());
            REQUIRE(c.asDataT<StrData>()->get() == "def");
            REQUIRE(d.isNone());
        }
    }

    SECTION("operator< for data")
    {
        Atom a(new IntData(100));
        REQUIRE_FALSE(a < a);

        std::vector<Atom> l;
        l.push_back(new StrData("apple"));
        l.push_back(new IntData(100));
        l.push_back(new IntData(300));
        l.push_back(new StrData("banana"));
        l.push_back(new IntData(200));

        std::sort(l.begin(), l.end());
        REQUIRE(l[0].asDataT<IntData>()->value() == 100);
        REQUIRE(l[1].asDataT<IntData>()->value() == 200);
        REQUIRE(l[2].asDataT<IntData>()->value() == 300);
        REQUIRE(l[3].asDataT<StrData>()->get() == "apple");
        REQUIRE(l[4].asDataT<StrData>()->get() == "banana");
    }

    SECTION("operator== for data")
    {
        REQUIRE(Atom(new IntData(100)) == Atom(new IntData(100)));
        REQUIRE_FALSE(Atom(new IntData(100)) != Atom(new IntData(100)));
        REQUIRE(Atom(new IntData(100)) != Atom(new IntData(101)));
        REQUIRE_FALSE(Atom(new IntData(100)) == Atom(new IntData(101)));
    }

    SECTION("isA")
    {
        // void
        REQUIRE(Atom().isA<void>());
        REQUIRE_FALSE(Atom(1).isA<void>());
        REQUIRE_FALSE(Atom(0.f).isA<void>());
        REQUIRE_FALSE(Atom(-1).isA<void>());
        REQUIRE_FALSE(Atom(0.0001).isA<void>());
        REQUIRE_FALSE(Atom(0.999999).isA<void>());
        REQUIRE_FALSE(Atom(-0.0001).isA<void>());
        REQUIRE_FALSE(Atom(-0.999999).isA<void>());
        REQUIRE_FALSE(S("A").isA<void>());
        REQUIRE_FALSE(S("100").isA<void>());
        REQUIRE_FALSE(Atom(new IntData(100)).isA<void>());

        // bool
        REQUIRE_FALSE(Atom().isA<bool>());
        REQUIRE(Atom(1).isA<bool>());
        REQUIRE(Atom(0.f).isA<bool>());
        REQUIRE_FALSE(Atom(-1).isA<bool>());
        REQUIRE_FALSE(Atom(0.001).isA<bool>());
        REQUIRE_FALSE(S("A").isA<bool>());
        REQUIRE(S("true").isA<bool>());
        REQUIRE(S("false").isA<bool>());
        REQUIRE_FALSE(S("True").isA<bool>());
        REQUIRE_FALSE(S("False").isA<bool>());
        REQUIRE_FALSE(Atom(new IntData(100)).isA<bool>());

        // float
        REQUIRE_FALSE(Atom().isA<t_float>());
        REQUIRE(Atom(1).isA<t_float>());
        REQUIRE(Atom(0.f).isA<t_float>());
        REQUIRE(Atom(-1).isA<t_float>());
        REQUIRE(Atom(0.001).isA<t_float>());
        REQUIRE_FALSE(S("A").isA<t_float>());
        REQUIRE_FALSE(S("0.001").isA<t_float>());
        REQUIRE_FALSE(Atom(new IntData(100)).isA<t_float>());

        // int
        REQUIRE_FALSE(Atom().isA<int>());
        REQUIRE(Atom(1).isA<int>());
        REQUIRE(Atom(0.f).isA<int>());
        REQUIRE(Atom(-1).isA<int>());
        REQUIRE_FALSE(Atom(0.0001).isA<int>());
        REQUIRE_FALSE(Atom(0.999999).isA<int>());
        REQUIRE_FALSE(Atom(-0.0001).isA<int>());
        REQUIRE_FALSE(Atom(-0.999999).isA<int>());
        REQUIRE_FALSE(S("A").isA<int>());
        REQUIRE_FALSE(S("100").isA<int>());
        REQUIRE_FALSE(Atom(new IntData(100)).isA<int>());

        // symbol
        REQUIRE_FALSE(Atom().isA<t_symbol*>());
        REQUIRE_FALSE(Atom(1).isA<t_symbol*>());
        REQUIRE_FALSE(Atom(0.f).isA<t_symbol*>());
        REQUIRE_FALSE(Atom(-1).isA<t_symbol*>());
        REQUIRE_FALSE(Atom(0.0001).isA<t_symbol*>());
        REQUIRE_FALSE(Atom(0.999999).isA<t_symbol*>());
        REQUIRE(S("A").isA<t_symbol*>());
        REQUIRE(S("@A").isA<t_symbol*>());
        REQUIRE(S("100").isA<t_symbol*>());
        REQUIRE_FALSE(Atom(new IntData(100)).isA<t_symbol*>());

        // atom
        REQUIRE(Atom().isA<Atom>());
        REQUIRE(Atom(1).isA<Atom>());
        REQUIRE(Atom(0.f).isA<Atom>());
        REQUIRE(Atom(-1).isA<Atom>());
        REQUIRE(Atom(0.0001).isA<Atom>());
        REQUIRE(Atom(0.999999).isA<Atom>());
        REQUIRE(Atom(-0.0001).isA<Atom>());
        REQUIRE(Atom(-0.999999).isA<Atom>());
        REQUIRE(S("A").isA<Atom>());
        REQUIRE(S("100").isA<Atom>());
        REQUIRE(Atom(new IntData(100)).isA<Atom>());

        // data
        REQUIRE_FALSE(Atom().isA<IntData>());
        REQUIRE_FALSE(Atom(1).isA<IntData>());
        REQUIRE_FALSE(Atom(0.f).isA<IntData>());
        REQUIRE_FALSE(Atom(-1).isA<IntData>());
        REQUIRE_FALSE(Atom(0.0001).isA<IntData>());
        REQUIRE_FALSE(Atom(0.999999).isA<IntData>());
        REQUIRE_FALSE(Atom(-0.0001).isA<IntData>());
        REQUIRE_FALSE(Atom(-0.999999).isA<IntData>());
        REQUIRE_FALSE(S("A").isA<IntData>());
        REQUIRE_FALSE(S("100").isA<IntData>());
        REQUIRE_FALSE(Atom(new IntData(100)).isA<StrData>());
        REQUIRE(Atom(new IntData(100)).isA<IntData>());

        // size_t
        REQUIRE_FALSE(Atom().isA<size_t>());
        REQUIRE(Atom(1).isA<size_t>());
        REQUIRE(Atom(0.f).isA<size_t>());
        REQUIRE_FALSE(Atom(-1).isA<size_t>());
        REQUIRE_FALSE(Atom(0.0001).isA<size_t>());
        REQUIRE_FALSE(Atom(0.999999).isA<size_t>());
        REQUIRE_FALSE(Atom(-0.0001).isA<size_t>());
        REQUIRE_FALSE(Atom(-0.999999).isA<size_t>());
        REQUIRE_FALSE(S("A").isA<size_t>());
        REQUIRE_FALSE(S("100").isA<size_t>());
        REQUIRE_FALSE(Atom(new IntData(100)).isA<size_t>());
    }

    SECTION("asD")
    {
        REQUIRE(Atom().asD<IntData>() == nullptr);
        REQUIRE(A(1).asD<IntData>() == nullptr);
        REQUIRE(A("ABC").asD<IntData>() == nullptr);
        REQUIRE(Atom(new IntData(100)).asD<IntData>() != nullptr);
        REQUIRE(Atom(new IntData(100)).asD<IntData>()->value() == 100);
        // undef
        // REQUIRE(Atom(new IntData(100)).asD<StrData>() != nullptr);
    }

    SECTION("float compare")
    {
        REQUIRE(A(1.5) == 1.5);
        REQUIRE_FALSE(A(1) == 2);
        REQUIRE(A(1) != 2);
        REQUIRE_FALSE(A(1) != 1);

        REQUIRE(A(1) < 1.1);
        REQUIRE_FALSE(A(2) < 1);
        REQUIRE_FALSE(A(2) < 2);

        REQUIRE(A(2) <= 2);
        REQUIRE(A(2) <= 2.1);
        REQUIRE_FALSE(A(2) <= 1);

        REQUIRE(A(2) > 1);
        REQUIRE_FALSE(A(2) > 2);
        REQUIRE_FALSE(A(2) > 3);

        REQUIRE(A(2) >= 1);
        REQUIRE(A(2) >= 2);
        REQUIRE_FALSE(A(2) >= 3);

        REQUIRE_FALSE(A("A") == 1);
        REQUIRE(A("A") != 1);

        REQUIRE_FALSE(A("A") > 1);
        REQUIRE_FALSE(A("A") < 1);
        REQUIRE_FALSE(A("A") >= 1);
        REQUIRE_FALSE(A("A") <= 1);
    }

    SECTION("parseQuoted")
    {
        REQUIRE(A(1).parseQuoted() == A(1));
        REQUIRE(A("abc").parseQuoted() == A("abc"));
        REQUIRE(A("\"\"").parseQuoted() == A(""));
        REQUIRE(A("\"`\"`\"\"").parseQuoted() == A("\"\""));
        REQUIRE(A("\" \"").parseQuoted() == A(" "));
        REQUIRE(A("\"abcde\"").parseQuoted() == A("abcde"));
        REQUIRE(A("`").parseQuoted() == A("`"));
        REQUIRE(A("``").parseQuoted() == A("``"));
        REQUIRE(A("\"``\"").parseQuoted() == A("`"));
        REQUIRE(A("\"").parseQuoted() == A("\""));
    }

    SECTION("isQuoted")
    {
        REQUIRE_FALSE(Atom().isQuoted());
        REQUIRE_FALSE(A(1).isQuoted());
        REQUIRE_FALSE(A("abc").isQuoted());
        REQUIRE_FALSE(A("'abc'").isQuoted());
        REQUIRE_FALSE(A("\"ABC").isQuoted());
        REQUIRE_FALSE(A("ABC\"").isQuoted());
        REQUIRE(A("\"ABC\"").isQuoted());
        REQUIRE(A("\"ABC``\"").isQuoted());
        REQUIRE_FALSE(A("\"ABC`\"").isQuoted());
    }

    SECTION("beginQuote")
    {
        REQUIRE(!Atom().beginQuote());
        REQUIRE(!A(1).beginQuote());
        REQUIRE(!A("abc").beginQuote());
        REQUIRE(A("\"").beginQuote());
        REQUIRE(A("\"abc").beginQuote());
        REQUIRE(!A("`\"").beginQuote());
    }

    SECTION("endQuote")
    {
        REQUIRE(!Atom().endQuote());
        REQUIRE(!A(1).endQuote());
        REQUIRE(!A("abc").endQuote());
        REQUIRE(!A("\"abc").endQuote());
        REQUIRE(!A("a\"bc").endQuote());
        REQUIRE(!A("abc`\"").endQuote());
        REQUIRE(!A("").endQuote());
        REQUIRE(!A("`\"").endQuote());
        REQUIRE(A("\"").endQuote());
        REQUIRE(A("abc\"").endQuote());
        REQUIRE(A("abc``\"").endQuote());
        REQUIRE(!A("\"\"").endQuote());
    }

    SECTION("set* with data")
    {
        Atom int100(new IntData(100));
        REQUIRE(int100.refCount() == 1);

        Atom i0(int100);
        REQUIRE(i0.refCount() == 2);

        REQUIRE_FALSE(i0.setFloat(100));
        REQUIRE(i0.isData());
        REQUIRE(int100.isData());

        REQUIRE(i0.setFloat(200, true));
        REQUIRE(i0.isFloat());
        REQUIRE(int100.isData());
        REQUIRE(i0.refCount() == 0);
        REQUIRE(int100.refCount() == 1);

        Atom i1(int100);
        REQUIRE(i1.isData());
        REQUIRE(i1.refCount() == 2);
        REQUIRE(int100.refCount() == 2);
        REQUIRE(i1.setSymbol(SYM("tre"), true));
        REQUIRE(i1.isSymbol());
        REQUIRE(i1.refCount() == 0);
        REQUIRE(int100.refCount() == 1);
    }

    SECTION("std::swap")
    {
        Atom int100(new IntData(100));
        std::swap(int100, int100);

        REQUIRE(int100.isData());
        REQUIRE(int100.asD<IntData>()->value() == 100);

        Atom f(2.5);
        std::swap(int100, f);
        REQUIRE(f.isData());
        REQUIRE(int100.isFloat());
        REQUIRE(f.refCount() == 1);
    }

    SECTION("detach")
    {
        using IntAtom = DataAtom<IntData>;

        Atom i0(new IntData(100));
        IntAtom i1(i0);

        REQUIRE(i0 == i1);
        REQUIRE(i0.refCount() == 2);
        REQUIRE(i1.refCount() == 2);
        REQUIRE(i0.asData() == i1.asData());

        REQUIRE(i1.detachData());

        REQUIRE(i0 == i1);
        REQUIRE(i0.refCount() == 1);
        REQUIRE(i1.refCount() == 1);
        REQUIRE(i0.asData() != i1.asData());

        i1->setValue(101);
        REQUIRE(i0 != i1);
        REQUIRE(i0.refCount() == 1);
        REQUIRE(i1.refCount() == 1);
    }

    SECTION("single detach")
    {
        Atom i(new IntData(-100));
        auto p0 = i.asData();

        REQUIRE(i.detachData());
        auto p1 = i.asData();
        REQUIRE(p0 != p1);
        REQUIRE(p1->toString() == "IntData(-100)");
        REQUIRE(i.refCount() == 1);
    }

    SECTION("floatAt*")
    {
        REQUIRE(S("A").asFloatGreaterThen(0, 5000) == F(5000));
        REQUIRE(F(0.99).asFloatGreaterThen(1, 5000) == F(5000));
        REQUIRE(F(1).asFloatGreaterThen(1, 5000) == F(5000));
        REQUIRE(F(1.125).asFloatGreaterThen(1, 5000) == F(1.125));

        REQUIRE(S("A").asFloatGreaterEqual(0, 5000) == F(5000));
        REQUIRE(F(0.99).asFloatGreaterEqual(1, 5000) == F(5000));
        REQUIRE(F(1).asFloatGreaterEqual(1, 5000) == F(1));
        REQUIRE(F(1.125).asFloatGreaterEqual(1, 5000) == F(1.125));

        REQUIRE(S("A").asFloatLessEqual(0, -5000) == F(-5000));
        REQUIRE(F(0.75).asFloatLessEqual(1, -5000) == F(0.75));
        REQUIRE(F(1).asFloatLessEqual(1, -5000) == F(1));
        REQUIRE(F(1.125).asFloatLessEqual(1, -5000) == F(-5000));

        REQUIRE(S("A").asFloatLessThen(0, -5000) == F(-5000));
        REQUIRE(F(0.75).asFloatLessThen(1, -5000) == F(0.75));
        REQUIRE(F(1).asFloatLessThen(1, -5000) == F(-5000));
        REQUIRE(F(1.125).asFloatLessThen(1, -5000) == F(-5000));

        // range
        REQUIRE(S("A").asFloatInClosedInterval(-1, 1, 0) == F(0));
        REQUIRE(F(-1.1).asFloatInClosedInterval(-1, 1, 0) == F(-1));
        REQUIRE(F(-1).asFloatInClosedInterval(-1, 1, 0) == F(-1));
        REQUIRE(F(0).asFloatInClosedInterval(-1, 1, 0) == F(0));
        REQUIRE(F(1).asFloatInClosedInterval(-1, 1, 0) == F(1));
        REQUIRE(F(1.1).asFloatInClosedInterval(-1, 1, 0) == F(1));

        // range
        REQUIRE(S("A").asFloatInOpenInterval(-1, 1, 0) == F(0));
        REQUIRE(F(-1.1).asFloatInOpenInterval(-1, 1, 0) == F(0));
        REQUIRE(F(-1).asFloatInOpenInterval(-1, 1, 0) == F(0));
        REQUIRE(F(-0.5).asFloatInOpenInterval(-1, 1, 0) == F(-0.5));
        REQUIRE(F(0).asFloatInOpenInterval(-1, 1, 0) == F(0));
        REQUIRE(F(0.5).asFloatInOpenInterval(-1, 1, 0) == F(0.5));
        REQUIRE(F(1).asFloatInOpenInterval(-1, 1, 0) == F(0));
        REQUIRE(F(1.1).asFloatInOpenInterval(-1, 1, 0) == F(0));

        // range
        REQUIRE(S("A").asFloatInLeftOpenInterval(-1, 1, 0) == F(0));
        REQUIRE(F(-1.1).asFloatInLeftOpenInterval(-1, 1, 0) == F(0));
        REQUIRE(F(-1).asFloatInLeftOpenInterval(-1, 1, 0) == F(0));
        REQUIRE(F(-0.5).asFloatInLeftOpenInterval(-1, 1, 0) == F(-0.5));
        REQUIRE(F(0).asFloatInLeftOpenInterval(-1, 1, 0) == F(0));
        REQUIRE(F(0.5).asFloatInLeftOpenInterval(-1, 1, 0) == F(0.5));
        REQUIRE(F(1).asFloatInLeftOpenInterval(-1, 1, 0) == F(1));
        REQUIRE(F(1.1).asFloatInLeftOpenInterval(-1, 1, 0) == F(1));

        // range
        REQUIRE(S("A").asFloatInRightOpenInterval(-1, 1, 0) == F(0));
        REQUIRE(F(-1.1).asFloatInRightOpenInterval(-1, 1, 0) == F(-1));
        REQUIRE(F(-1).asFloatInRightOpenInterval(-1, 1, 0) == F(-1));
        REQUIRE(F(-0.5).asFloatInRightOpenInterval(-1, 1, 0) == F(-0.5));
        REQUIRE(F(0).asFloatInRightOpenInterval(-1, 1, 0) == F(0));
        REQUIRE(F(0.5).asFloatInRightOpenInterval(-1, 1, 0) == F(0.5));
        REQUIRE(F(1).asFloatInRightOpenInterval(-1, 1, 0) == F(0));
        REQUIRE(F(1.1).asFloatInRightOpenInterval(-1, 1, 0) == F(0));

        // int
        REQUIRE(S("A").asIntGreaterThen(100, 5000) == F(5000));
        REQUIRE(F(99).asIntGreaterThen(100, 5000) == F(5000));
        REQUIRE(F(100).asIntGreaterThen(100, 5000) == F(5000));
        REQUIRE(F(101).asIntGreaterThen(100, 5000) == F(101));

        REQUIRE(S("A").asIntGreaterEqual(100, 5000) == F(5000));
        REQUIRE(F(99).asIntGreaterEqual(100, 5000) == F(5000));
        REQUIRE(F(100).asIntGreaterEqual(100, 5000) == F(100));
        REQUIRE(F(101).asIntGreaterEqual(100, 5000) == F(101));

        REQUIRE(S("A").asIntLessEqual(100, -5000) == F(-5000));
        REQUIRE(F(99).asIntLessEqual(100, -5000) == F(99));
        REQUIRE(F(100).asIntLessEqual(100, -5000) == F(100));
        REQUIRE(F(101).asIntLessEqual(100, -5000) == F(-5000));

        REQUIRE(S("A").asIntLessThen(100, -5000) == F(-5000));
        REQUIRE(F(99).asIntLessThen(100, -5000) == F(99));
        REQUIRE(F(100).asIntLessThen(100, -5000) == F(-5000));
        REQUIRE(F(101).asIntLessThen(100, -5000) == F(-5000));

        // range
        REQUIRE(S("A").asIntInClosedInterval(-1, 1, 0) == F(0));
        REQUIRE(F(-2).asIntInClosedInterval(-1, 1, 0) == F(-1));
        REQUIRE(F(-1).asIntInClosedInterval(-1, 1, 0) == F(-1));
        REQUIRE(F(0).asIntInClosedInterval(-1, 1, 0) == F(0));
        REQUIRE(F(1).asIntInClosedInterval(-1, 1, 0) == F(1));
        REQUIRE(F(2).asIntInClosedInterval(-1, 1, 0) == F(1));
    }

    SECTION("comma")
    {
        REQUIRE_FALSE(Atom().isComma());
        REQUIRE(Atom::comma().isComma());
        REQUIRE(Atom::comma().type() == Atom::COMMA);

        Atom a(1);
        REQUIRE_FALSE(a.isComma());
        a.setComma();
        REQUIRE(a.isComma());

        REQUIRE(a == Atom::comma());
    }

    SECTION("semicolon")
    {
        REQUIRE_FALSE(Atom().isSemicolon());
        REQUIRE(Atom::semicolon().isSemicolon());
        REQUIRE(Atom::semicolon().type() == Atom::SEMICOLON);

        Atom a(1);
        REQUIRE_FALSE(a.isSemicolon());
        a.setSemicolon();
        REQUIRE(a.isSemicolon());

        REQUIRE(a == Atom::semicolon());
    }

    SECTION("expandDollarArgs")
    {
        using Val = Maybe<Atom>;
        REQUIRE(Atom(1).expandDollarArgs({}) == Val(1));
        REQUIRE(A("test").expandDollarArgs({}).value() == A("test"));
        REQUIRE(A("$0").expandDollarArgs({}).value() == A("$0"));

        auto dz = canvas_info_dollarzero(canvas_getcurrent());
        char buf[32];
        sprintf(buf, "%d", dz);
        REQUIRE(Atom::dollar(0).expandDollarArgs(LF(10, 20, 30), false).value() == A(buf));
        sprintf(buf, "%d-test", dz);
        REQUIRE(DOLL_SYM("$0-test").expandDollarArgs(LF(10, 20, 30), false).value() == A(buf));
        sprintf(buf, "%d-test", dz);
        REQUIRE(DOLL_SYM("$1-$2-$3-$4").expandDollarArgs(LF(10, 20, 30), false).value() == A("10-20-30-$4"));
        REQUIRE_FALSE(DOLL_SYM("$1-$2-$3-$4").expandDollarArgs(LF(10, 20, 30), true));
        REQUIRE(DOLL_SYM("$1-$2-$3").expandDollarArgs(LF(10, 20, 30), true).value() == A("10-20-30"));

        sprintf(buf, "%d-$1", dz);
        REQUIRE(DOLL_SYM("$0-$1").expandDollarArgs(canvas_getcurrent(), false).value() == A(buf));
        REQUIRE(DOLL_SYM("$0-$1").expandDollarArgs(canvas_getcurrent(), true).isNull());
        REQUIRE_FALSE(DOLL_SYM("$0-$1").expandDollarArgs(canvas_getcurrent(), true));
    }

    SECTION("c-string equal")
    {
        REQUIRE(A("test") == "test");
        REQUIRE(A("test") != "Test");
        REQUIRE_FALSE(A("test") != "test");
        REQUIRE_FALSE(A("test") == "Test");
        REQUIRE_FALSE(A(1) == "Test");
        REQUIRE_FALSE(Atom() == "Test");
        REQUIRE_FALSE(Atom::comma() == ",");
        REQUIRE_FALSE(Atom::semicolon() == ";");
        REQUIRE_FALSE(Atom::dollar(1) == "Test");
        REQUIRE_FALSE(Atom::dollarSymbol(SYM("test-$0")) == "test-$0");
    }

    SECTION("t_symbol* equal")
    {
        REQUIRE(A("test") == SYM("test"));
        REQUIRE(A("test") != SYM("Test"));
        REQUIRE_FALSE(A(1) == SYM("test"));
        REQUIRE_FALSE(Atom() == SYM("test"));
        REQUIRE_FALSE(Atom::comma() == SYM("test"));
        REQUIRE_FALSE(Atom::semicolon() == SYM("test"));
        REQUIRE_FALSE(Atom::dollar(1) == SYM("test"));
        REQUIRE_FALSE(Atom::dollarSymbol(SYM("test-$0")) == SYM("test"));
    }
}
