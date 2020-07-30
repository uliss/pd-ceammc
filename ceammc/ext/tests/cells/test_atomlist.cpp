/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#include "test_common.h"

#include <boost/optional/optional_io.hpp>
#include <cstdarg>

static Atom atomAdd(const Atom& a, const Atom& b)
{
    return Atom(a.asFloat() + b.asFloat());
}

static float atomSum(const Atom& a, const Atom& b)
{
    return a.asFloat(0) + b.asFloat(0);
}

static t_float floatSum(t_float a, t_float b)
{
    return a + b;
}

static float atomMul(const Atom& a, const Atom& b)
{
    return a.asFloat(1) * b.asFloat(1);
}

static t_float floatMul(t_float a, t_float b)
{
    return a * b;
}

TEST_CASE("AtomList", "[ceammc::AtomList]")
{
    SECTION("test of tests")
    {
        REQUIRE(L() == AtomList());
        REQUIRE(LF(1) == AtomList({ 1 }));
        REQUIRE(LF(1, 2) == AtomList({ 1, 2 }));
        REQUIRE(LF(1, 2, 3) == AtomList({ 1, 2, 3 }));
        REQUIRE(LA("a", 1) == AtomList(A("a"), A(1)));
        REQUIRE(LA(1) == AtomList(A(1)));
        REQUIRE(LA(1, 2, 3, 4, "A", "B", "C") == AtomList({ A(1), A(2), A(3), A(4), A("A"), A("B"), A("C") }));
    }

    SECTION("construct")
    {
        AtomList l;
        REQUIRE(l.empty());
        REQUIRE(l.size() == 0);
        REQUIRE(l.min() == 0);
        REQUIRE(l.max() == 0);
        REQUIRE(l.first() == 0);
        REQUIRE(l.last() == 0);
        REQUIRE(l.relativeAt(0) == 0);
        REQUIRE(l.relativeAt(1) == 0);
        REQUIRE(l.relativeAt(-1) == 0);

        l.append(Atom(1.0));
        REQUIRE_FALSE(l.empty());
        REQUIRE(l.size() == 1);
    }

    SECTION("sort numeric")
    {
        AtomList l({ 10, 2, -3 });
        l.sort();
        REQUIRE(l == LF(-3, 2, 10));
    }

    SECTION("sort symbol")
    {
        AtomList l = LA("b", "c", "a");
        l.sort();
        REQUIRE(l == LA("a", "b", "c"));
        l.sort();
        REQUIRE(l == LA("a", "b", "c"));
    }

    SECTION("sort mixed")
    {
        AtomList l;
        l.sort();
        REQUIRE(l.max() == 0);
        REQUIRE(l.min() == 0);

        l = LA("a", 2, -3, "b", "a");
        l.sort();
        REQUIRE(l == LA(-3, 2, "a", "a", "b"));

        REQUIRE(l.max() != 0);
        REQUIRE(l.max()->asSymbol() == SYM("b"));
        REQUIRE(l.min() != 0);
        REQUIRE(l.min()->asFloat() == -3.0f);

        const AtomList c1 = l;
        REQUIRE(c1.max() != 0);
        REQUIRE(c1.max()->asSymbol() == SYM("b"));
        REQUIRE(c1.min() != 0);
        REQUIRE(c1.min()->asFloat() == -3.0f);
        REQUIRE(c1.first()->asFloat() == -3.0f);
        REQUIRE(c1.last()->asSymbol() == SYM("b"));

        REQUIRE(c1.relativeAt(0)->asFloat() == -3.0f);
        REQUIRE(c1.relativeAt(-1)->asSymbol() == SYM("b"));
        REQUIRE(c1.relativeAt(1000) == 0);
    }

    SECTION("compare")
    {
        AtomList l1, l2;
        REQUIRE(l1 == l1);
        REQUIRE(l1 == l2);
        REQUIRE_FALSE(l1 != l1);
        REQUIRE_FALSE(l1 != l2);

        l2.append(Atom(1.0));
        REQUIRE(l1 != l2);
        REQUIRE_FALSE(l1 == l2);

        REQUIRE(LA("A", "C") != L());
        REQUIRE(LA("A", "C") != LA("A", "B"));
        REQUIRE(LA("A", "C") != LA("A", 0xB));
        REQUIRE(LA("A", "B") == LA("A", "B"));
    }

    SECTION("filtered")
    {
        AtomList l = LA("a", 2, 3, "b");
        REQUIRE(l.filtered(nullptr) == l);
        REQUIRE(l.filtered(isFloat) == LF(2, 3));
        REQUIRE(l.filtered(isSymbol) == LA("a", "b"));
    }

    SECTION("clear")
    {
        AtomList l;
        l.clear();
        REQUIRE(l.empty());
        l = LF(1, 2, 3, 4, 5);
        REQUIRE(l.size() == 5);
        l.clear();
        REQUIRE(l.empty());
    }

    SECTION("insert")
    {
        AtomList l;
        REQUIRE(l.insert(0, Atom(1.5)));
        REQUIRE(l == LF(1.5));

        REQUIRE(l.insert(0, Atom(-2.5)));
        REQUIRE(l == LF(-2.5, 1.5));

        REQUIRE(l.insert(2, Atom(15)));
        REQUIRE(l == LF(-2.5, 1.5, 15));

        REQUIRE(l.insert(1, Atom(8)));
        REQUIRE(l == LF(-2.5, 8, 1.5, 15));

        REQUIRE_FALSE(l.insert(20, Atom(150)));
        REQUIRE(l == LF(-2.5, 8, 1.5, 15));

        l.clear();
        REQUIRE(l.insert(0, LF(1, 2, 3)));
        REQUIRE(l == LF(1, 2, 3));
        REQUIRE(l.insert(0, LF(100, 200)));
        REQUIRE(l == LF(100, 200, 1, 2, 3));
        REQUIRE(l.insert(0, L()));
        REQUIRE(l == LF(100, 200, 1, 2, 3));
        REQUIRE(l.insert(1, LA("ABC", "DEF")));
        REQUIRE(l == LA(100, "ABC", "DEF", 200, 1, 2, 3));
        REQUIRE_FALSE(l.insert(200, L()));
        REQUIRE_FALSE(l.insert(200, LF(2)));
    }

    SECTION("remove")
    {
        AtomList l({ 1, 2, 3 });

        REQUIRE_FALSE(l.remove(20));
        REQUIRE(l == LF(1, 2, 3));
        REQUIRE(l.remove(1));
        REQUIRE(l == LF(1, 3));
        REQUIRE(l.remove(0));
        REQUIRE(l == LF(3));
        REQUIRE(l.remove(0));
        REQUIRE(l == L());
        REQUIRE_FALSE(l.remove(3));
        REQUIRE(l == L());
    }

    SECTION("count")
    {
        REQUIRE(L().count(1.0) == 0);
        REQUIRE(L().count(isFloat) == 0);

        REQUIRE(LF(1, 2, 3).count(-1) == 0);
        REQUIRE(LF(1, 2, 2, 3, 3, 3).count(1) == 1);
        REQUIRE(LF(1, 2, 2, 3, 3, 3).count(2) == 2);
        REQUIRE(LF(1, 2, 2, 3, 3, 3).count(3) == 3);
        REQUIRE(LF(1, 1, 1, 1).count(1) == 4);

        Atom d100(new IntData(100));
        Atom d200(new IntData(200));
        Atom d100_new(new IntData(100));

        Atom d_abc(new StrData("ABC"));
        Atom d_abcd(new StrData("ABCD"));

        REQUIRE(LA(d100, d200).count(d100) == 1);
        REQUIRE(LA(d100_new, d200).count(d100) == 1);
        REQUIRE(LA(d100_new, d100).count(d200) == 0);
        REQUIRE(LA(d100_new, d100).count(d_abc) == 0);

        REQUIRE(LA(d100, d100).count(d100) == 2);
        REQUIRE(LA(d_abc, d_abcd).count(d_abc) == 1);
    }

    SECTION("fill")
    {
        AtomList l({ 1, 2, 3 });
        l.fill(4.f);
        REQUIRE(l.count(4.f) == 3);

        AtomList l2;
        l2.fill(1.1, 10);
        REQUIRE(l2.size() == 10);
    }

    SECTION("allOf")
    {
        AtomList l;
        REQUIRE_FALSE(l.allOf(isFloat));
        l.fill(1.f, 10);
        REQUIRE(l.allOf(isFloat));
        REQUIRE_FALSE(l.allOf(isSymbol));
    }

    SECTION("anyOf")
    {
        AtomList l;
        REQUIRE_FALSE(l.anyOf(isFloat));
        l.fill(1.f, 10);
        REQUIRE(l.anyOf(isFloat));
        REQUIRE_FALSE(l.anyOf(isSymbol));
        l.append(gensym("test"));
        REQUIRE(l.anyOf(isSymbol));
    }

    SECTION("noneOf")
    {
        AtomList l;
        REQUIRE(l.noneOf(isFloat));
        l.fill(1.f, 10);
        REQUIRE_FALSE(l.noneOf(isFloat));
        REQUIRE(l.noneOf(isSymbol));
    }

    SECTION("shuffle")
    {
        AtomList l;
        l.shuffle();
    }

    SECTION("reverse")
    {
        AtomList l({ 1, 2, 3 });
        l.reverse();
        REQUIRE(l == LF(3, 2, 1));
    }

    SECTION("removeAll")
    {
        AtomList l({ 1, 2, 3, 1, 1 });

        l.removeAll(1);
        REQUIRE(l.size() == 2);
        REQUIRE(l == LF(2, 3));
    }

    SECTION("removeAllPred")
    {
        AtomList l({ 1, 2, 3, 1, 1 });
        l.append(gensym("a"));

        l.removeAll(isFloat);
        REQUIRE(l == LA("a"));
        REQUIRE(l.size() == 1);
    }

    SECTION("replaceAll")
    {
        AtomList l({ 1, 2, 3, 1, 1 });
        l.replaceAll(1.f, -1.f);
        REQUIRE(l == LF(-1, 2, 3, -1, -1));
    }

    SECTION("replaceAllPred")
    {
        AtomList l({ 1, 2, 3 });
        l.replaceAll(isFloat, A("x"));
        REQUIRE(l == LA("x", "x", "x"));
    }

    SECTION("contains")
    {
        AtomList l({ 1, 2, 3, 1, 1 });

        REQUIRE(l.contains(1.f));
        REQUIRE(l.contains(2.f));
        REQUIRE(l.contains(3.f));

        Atom d100(new IntData(100));
        Atom d200(new IntData(200));
        Atom d100x(new IntData(100));

        REQUIRE(LA(d100, d100).contains(d100));
        REQUIRE(LA(d100, d100).contains(d100x));
        REQUIRE_FALSE(LA(d100, d100).contains(d200));
    }

    SECTION("findPos")
    {
        AtomList l({ 1, 2, 3, 1, 1 });

        REQUIRE(l.findPos(3.f) == 2);
        REQUIRE(l.findPos(2.f) == 1);
        REQUIRE(l.findPos(1.f) == 0);
        REQUIRE(l.findPos(100.f) == -1);
        REQUIRE(l.findPos(gensym("a")) == -1);
    }

    SECTION("findPos predicate")
    {
        AtomList l = LA(1, "b", "@a", 3, 1);
        REQUIRE(l.findPos(isFloat) == 0);
        REQUIRE(l.findPos(isProperty) == 2);
        REQUIRE(l.findPos(isSymbol) == 1);
    }

    SECTION("clipAt")
    {
        AtomList l;
        REQUIRE(l.clipAt(0) == 0);

        REQUIRE(LF(1, 2, 3).clipAt(-2)->asFloat() == 1.f);
        REQUIRE(LF(1, 2, 3).clipAt(-1)->asFloat() == 1.f);
        REQUIRE(LF(1, 2, 3).clipAt(0)->asFloat() == 1.f);
        REQUIRE(LF(1, 2, 3).clipAt(1)->asFloat() == 2.f);
        REQUIRE(LF(1, 2, 3).clipAt(2)->asFloat() == 3.f);
        REQUIRE(LF(1, 2, 3).clipAt(3)->asFloat() == 3.f);
        REQUIRE(LF(1, 2, 3).clipAt(4)->asFloat() == 3.f);
        REQUIRE(LF(1, 2, 3).clipAt(200)->asFloat() == 3.f);
    }

    SECTION("clipAt1")
    {
        AtomList l;
        REQUIRE(l.clipAt(0) == 0);
        l.append(1.f);

        const AtomList cl(l);
        REQUIRE(cl.clipAt(0)->asFloat() == 1.f);
        REQUIRE(cl.clipAt(1)->asFloat() == 1.f);
        REQUIRE(cl.clipAt(2)->asFloat() == 1.f);
    }

    SECTION("wrapAt2")
    {
        AtomList l;
        REQUIRE(l.wrapAt(0) == 0);
        l.append(1.f);
        l.append(2.f);

        const AtomList cl(l);
        REQUIRE(cl.wrapAt(0)->asFloat() == 1.f);
        REQUIRE(cl.wrapAt(1)->asFloat() == 2.f);
        REQUIRE(cl.wrapAt(2)->asFloat() == 1.f);
        REQUIRE(cl.wrapAt(3)->asFloat() == 2.f);
        REQUIRE(cl.wrapAt(4)->asFloat() == 1.f);
        REQUIRE(cl.wrapAt(200)->asFloat() == 1.f);

        REQUIRE(cl.wrapAt(-1)->asFloat() == 2.f);
        REQUIRE(cl.wrapAt(-2)->asFloat() == 1.f);
        REQUIRE(cl.wrapAt(-3)->asFloat() == 2.f);
        REQUIRE(cl.wrapAt(-4)->asFloat() == 1.f);

        AtomList l2;
        l2.append(1.f);
        l2.append(2.f);
        l2.append(3.f);

        REQUIRE(l2.wrapAt(6)->asFloat() == 1.f);
        REQUIRE(l2.wrapAt(5)->asFloat() == 3.f);
        REQUIRE(l2.wrapAt(4)->asFloat() == 2.f);
        REQUIRE(l2.wrapAt(3)->asFloat() == 1.f);
        REQUIRE(l2.wrapAt(2)->asFloat() == 3.f);
        REQUIRE(l2.wrapAt(1)->asFloat() == 2.f);
        REQUIRE(l2.wrapAt(0)->asFloat() == 1.f);
        REQUIRE(l2.wrapAt(-1)->asFloat() == 3.f);
        REQUIRE(l2.wrapAt(-2)->asFloat() == 2.f);
        REQUIRE(l2.wrapAt(-3)->asFloat() == 1.f);
        REQUIRE(l2.wrapAt(-4)->asFloat() == 3.f);
        REQUIRE(l2.wrapAt(-5)->asFloat() == 2.f);
        REQUIRE(l2.wrapAt(-6)->asFloat() == 1.f);
    }

    SECTION("wrapAt1")
    {
        AtomList l;
        REQUIRE(l.wrapAt(0) == 0);
        l.append(1.f);

        const AtomList cl(l);
        REQUIRE(cl.wrapAt(0)->asFloat() == 1.f);
        REQUIRE(cl.wrapAt(1)->asFloat() == 1.f);
        REQUIRE(cl.wrapAt(2)->asFloat() == 1.f);
        REQUIRE(cl.wrapAt(3)->asFloat() == 1.f);
        REQUIRE(cl.wrapAt(4)->asFloat() == 1.f);
    }

    SECTION("foldAt3")
    {
        AtomList l;
        REQUIRE(l.foldAt(0) == 0);
        l.append(1.f);
        l.append(2.f);
        l.append(3.f);

        const AtomList cl(l);
        REQUIRE(cl.foldAt(-6)->asFloat() == 3.f);
        REQUIRE(cl.foldAt(-5)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(-4)->asFloat() == 1.f);
        REQUIRE(cl.foldAt(-3)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(-2)->asFloat() == 3.f);
        REQUIRE(cl.foldAt(-1)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(0)->asFloat() == 1.f);
        REQUIRE(cl.foldAt(1)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(2)->asFloat() == 3.f);
        REQUIRE(cl.foldAt(3)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(4)->asFloat() == 1.f);
        REQUIRE(cl.foldAt(5)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(6)->asFloat() == 3.f);
        REQUIRE(cl.foldAt(7)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(8)->asFloat() == 1.f);
    }

    SECTION("foldAt2")
    {
        AtomList l;
        REQUIRE(l.foldAt(0) == 0);
        l.append(1.f);
        l.append(2.f);

        const AtomList cl(l);
        REQUIRE(cl.foldAt(0)->asFloat() == 1.f);
        REQUIRE(cl.foldAt(1)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(2)->asFloat() == 1.f);
        REQUIRE(cl.foldAt(3)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(4)->asFloat() == 1.f);
        REQUIRE(cl.foldAt(5)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(6)->asFloat() == 1.f);
        REQUIRE(cl.foldAt(7)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(8)->asFloat() == 1.f);
    }

    SECTION("foldAt1")
    {
        const AtomList cl(1);
        REQUIRE(cl.foldAt(0));
        //        REQUIRE(cl.foldAt(0)->asFloat() == 1.f);
        //        REQUIRE(cl.foldAt(1)->asFloat() == 1.f);
        //        REQUIRE(cl.foldAt(2)->asFloat() == 1.f);
        //        REQUIRE(cl.foldAt(3)->asFloat() == 1.f);
        //        REQUIRE(cl.foldAt(4)->asFloat() == 1.f);
        //        REQUIRE(cl.foldAt(5)->asFloat() == 1.f);
    }

    SECTION("foldAt4")
    {
        AtomList l;
        REQUIRE(l.foldAt(0) == 0);

        const AtomList cl(LF(1, 2, 3, 4));
        REQUIRE(cl.at(0) == 1.f);
        REQUIRE(cl.foldAt(0)->asFloat() == 1.f);
        REQUIRE(cl.foldAt(1)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(2)->asFloat() == 3.f);
        REQUIRE(cl.foldAt(3)->asFloat() == 4.f);
        REQUIRE(cl.foldAt(4)->asFloat() == 3.f);
        REQUIRE(cl.foldAt(5)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(6)->asFloat() == 1.f);
        REQUIRE(cl.foldAt(7)->asFloat() == 2.f);
        REQUIRE(cl.foldAt(8)->asFloat() == 3.f);

        REQUIRE(cl.foldAt(-1)->asFloat() == 2);
        REQUIRE(cl.foldAt(-2)->asFloat() == 3);
        REQUIRE(cl.foldAt(-3)->asFloat() == 4);
        REQUIRE(cl.foldAt(-4)->asFloat() == 3);
        REQUIRE(cl.foldAt(-5)->asFloat() == 2);
        REQUIRE(cl.foldAt(-6)->asFloat() == 1);
        REQUIRE(cl.foldAt(-7)->asFloat() == 2);
        REQUIRE(cl.foldAt(-8)->asFloat() == 3);
    }

    SECTION("toPdData")
    {
        AtomList l;
        l.append(1.f);
        l.append(gensym("a"));
        t_atom* atoms = l.toPdData();
        REQUIRE(atoms != 0);
        REQUIRE(atom_getfloat(&atoms[0]) == 1.f);
        REQUIRE(atom_getsymbol(&atoms[1]) == gensym("a"));
    }

    SECTION("subMin")
    {
        AtomList l1;
        AtomList l2;

        REQUIRE(AtomList::sub(l1, l1) == l1);
        REQUIRE(AtomList::sub(l1, l2) == l1);

        l1.append(1.f);
        REQUIRE(AtomList::sub(l1, l1) == AtomList::zeroes(1));
        REQUIRE(AtomList::sub(l2, l1) == L());
        REQUIRE(AtomList::sub(l1, l2) == L());

        l2.append(2.f);
        REQUIRE(AtomList::sub(l1, l2) == AtomList::filled(-1.f, 1));
        REQUIRE(AtomList::sub(l2, l1) == AtomList::filled(1.f, 1));
    }

    SECTION("subPadZero")
    {
        AtomList l1;
        AtomList l2;

        REQUIRE(AtomList::sub(l1, l1, AtomList::PADZERO) == L());
        REQUIRE(AtomList::sub(l1, l2, AtomList::PADZERO) == L());

        l1.append(1.f);
        REQUIRE(AtomList::sub(l1, l1, AtomList::PADZERO) == AtomList::zeroes(1));
        REQUIRE(AtomList::sub(l2, l1, AtomList::PADZERO) == AtomList::filled(-1.f, 1));
    }

    SECTION("subClip")
    {
        AtomList l1;
        AtomList l2;

        REQUIRE(AtomList::sub(l1, l1, AtomList::CLIP) == L());
        REQUIRE(AtomList::sub(l1, l2, AtomList::CLIP) == L());

        l1.append(1.f);
        REQUIRE(AtomList::sub(l1, l1, AtomList::CLIP) == AtomList::zeroes(1));
        REQUIRE(AtomList::sub(l2, l1, AtomList::CLIP) == L());
        REQUIRE(AtomList::sub(l1, l2, AtomList::CLIP) == L());

        l2.append(2.f);
        REQUIRE(AtomList::sub(l1, l2, AtomList::CLIP) == AtomList::filled(-1.f, 1));
        REQUIRE(AtomList::sub(l2, l1, AtomList::CLIP) == AtomList::filled(1.f, 1));

        l1.append(2.f);
        l1.append(3.f);
        l1.append(4.f);

        REQUIRE(AtomList::sub(l1, l2, AtomList::CLIP) == LF(-1.f, 0.f, 1.f, 2.f));
        REQUIRE(AtomList::sub(l2, l1, AtomList::CLIP) == LF(1.f, 0.f, -1.f, -2.f));
    }

    SECTION("subWrap")
    {
        AtomList l1;
        AtomList l2;

        REQUIRE(AtomList::sub(l1, l1, AtomList::WRAP) == L());
        REQUIRE(AtomList::sub(l1, l2, AtomList::WRAP) == L());

        l1.append(1.f);
        REQUIRE(AtomList::sub(l1, l1, AtomList::WRAP) == AtomList::zeroes(1));
        REQUIRE(AtomList::sub(l2, l1, AtomList::WRAP) == L());
        REQUIRE(AtomList::sub(l1, l2, AtomList::WRAP) == L());

        l2.append(2.f);
        REQUIRE(AtomList::sub(l1, l2, AtomList::WRAP) == AtomList::filled(-1.f, 1));
        REQUIRE(AtomList::sub(l2, l1, AtomList::WRAP) == AtomList::filled(1.f, 1));

        l1.append(2.f);
        l1.append(3.f);
        l1.append(4.f);
        l1.append(5.f);
        l1.append(6.f);
        l1.append(7.f);
        l1.append(8.f);

        l2.append(1.f);
        l2.append(3.f);

        // [1, 2, 3, 4, 5, 6, 7, 8]
        // [2, 1, 3]
        // [2, 1, 3, 2, 1, 3, 2, 1] - wrapped version
        // [-1,1, 0, 2, 4, 3, 5, 7] - diff
        REQUIRE(AtomList::sub(l1, l2, AtomList::WRAP) == LF(-1., 1., 0., 2., 4., 3., 5., 7.));
        REQUIRE(AtomList::sub(l2, l1, AtomList::WRAP) == LF(1., -1., 0., -2., -4., -3., -5., -7.));
    }

    SECTION("subFold")
    {
        AtomList l1;
        AtomList l2;

        REQUIRE(AtomList::sub(l1, l1, AtomList::FOLD) == L());
        REQUIRE(AtomList::sub(l1, l2, AtomList::FOLD) == L());

        l1.append(1.f);
        REQUIRE(AtomList::sub(l1, l1, AtomList::FOLD) == AtomList::zeroes(1));
        REQUIRE(AtomList::sub(l2, l1, AtomList::FOLD) == L());
        REQUIRE(AtomList::sub(l1, l2, AtomList::FOLD) == L());

        l2.append(2.f);
        REQUIRE(AtomList::sub(l1, l2, AtomList::FOLD) == AtomList::filled(-1.f, 1));
        REQUIRE(AtomList::sub(l2, l1, AtomList::FOLD) == AtomList::filled(1.f, 1));

        l1.append(2.f);
        l1.append(3.f);
        l1.append(4.f);
        l1.append(5.f);
        l1.append(6.f);
        l1.append(7.f);
        l1.append(8.f);

        l2.append(1.f);
        l2.append(3.f);

        // [1, 2, 3, 4, 5, 6, 7, 8]
        // [2, 1, 3]
        // [2, 1, 3, 1, 2, 1, 3, 1] - folded version
        // [-1,1, 0, 3, 3, 5, 4, 7] - diff
        REQUIRE(AtomList::sub(l1, l2, AtomList::FOLD) == LF(-1., 1., 0., 3., 3., 5., 4., 7.));
        REQUIRE(AtomList::sub(l2, l1, AtomList::FOLD) == LF(1., -1., 0., -3., -3., -5., -4., -7.));
    }

    SECTION("resizePad")
    {
        AtomList l1;
        l1.resizePad(3, 1.f);
        REQUIRE(l1.size() == 3);
        REQUIRE(l1.at(0) == 1.f);
        REQUIRE(l1.at(1) == 1.f);
        REQUIRE(l1.at(2) == 1.f);

        l1.resizePad(1, 2.f);
        REQUIRE(l1.size() == 1);
        REQUIRE(l1.at(0) == 1.f);

        l1.resizePad(0, 100.f);
        REQUIRE(l1.empty());
    }

    SECTION("property")
    {
        AtomList l;
        REQUIRE_FALSE(l.property(SYM("@test"), (Atom*)0));
        REQUIRE_FALSE(l.hasProperty("@test"));

        Atom p1;
        REQUIRE_FALSE(l.property(SYM("@test"), &p1));

        l.append(1.f);
        l.append(2.f);
        l.append(gensym("a"));
        l.append(gensym("@test"));
        REQUIRE_FALSE(l.property(SYM("@test"), &p1));
        REQUIRE(l.hasProperty("@test"));
        l.append(3.f);
        REQUIRE(l.property(SYM("@test"), &p1));
        REQUIRE(l.hasProperty("@test"));
        REQUIRE(p1.asFloat() == 3.f);

        l.append(gensym("@test2"));
        REQUIRE(l.hasProperty("@test2"));
        l.append(4.f);

        Atom p2;
        REQUIRE(l.property(SYM("@test2"), &p2));
        REQUIRE(p2.asFloat() == 4.f);

        p1 = p2; // reset
        REQUIRE(l.property(SYM("@test"), &p1));
        REQUIRE(p1.asFloat() == 3.f);

        l.clear();
        l.append(gensym("@a"));
        l.append(gensym("@b"));
        l.append(gensym("@c"));

        p1 = Atom();
        REQUIRE_FALSE(l.property(SYM("@a"), &p1));
        REQUIRE(p1.isNone());
        REQUIRE_FALSE(l.property(SYM("@b"), &p1));
        REQUIRE(p1.isNone());
        REQUIRE_FALSE(l.property(SYM("@c"), &p1));
        REQUIRE(p1.isNone());
    }

    SECTION("test values")
    {
        AtomList l({ 1.f, 2.f, 3.f });
        REQUIRE(l.size() == 3);
        REQUIRE(l.at(0).isFloat());
        REQUIRE(l.at(1).isFloat());
        REQUIRE(l.at(2).isFloat());

        REQUIRE(l.at(0).asFloat() == 1.f);
        REQUIRE(l.at(1).asFloat() == 2.f);
        REQUIRE(l.at(2).asFloat() == 3.f);
    }

    SECTION("test ones")
    {
        AtomList l = AtomList::ones(4);
        REQUIRE(l.size() == 4);
        REQUIRE(l == LF(1.f, 1.f, 1.f, 1.f));

        REQUIRE(l.at(0).asFloat() == 1.f);
        REQUIRE(l.at(1).asFloat() == 1.f);
        REQUIRE(l.at(2).asFloat() == 1.f);
        REQUIRE(l.at(3).asFloat() == 1.f);

        REQUIRE(AtomList::ones(0) == L());
        REQUIRE(AtomList::zeroes(0) == L());
    }

    SECTION("test all properties")
    {
        AtomList l;
        l.append(1.f);
        l.append(gensym("v"));
        l.append(gensym("@a"));
        l.append(gensym("@b"));
        l.append(1.111f);
        l.append(gensym("@c"));
        l.append(1.56f);
        l.append(2.f);
        l.append(gensym("v"));

        std::deque<AtomList> props = l.properties();
        REQUIRE(props.size() == 3);
        REQUIRE(props[0].size() == 1);
        REQUIRE(props[0][0].asSymbol() == gensym("@a"));
        REQUIRE(props[1].size() == 2);
        REQUIRE(props[1][0].asSymbol() == gensym("@b"));
        REQUIRE(props[1][1].asFloat() == 1.111f);
        REQUIRE(props[2].size() == 4);
        REQUIRE(props[2][0].asSymbol() == gensym("@c"));
        REQUIRE(props[2][1].asFloat() == 1.56f);
        REQUIRE(props[2][2].asFloat() == 2.f);
        REQUIRE(props[2][3].asSymbol() == gensym("v"));
    }

    SECTION("test slice")
    {
        AtomList l = LF(1.f, 2.f, 3.f, 4.f, 5.f);
        REQUIRE(l.slice(0) == LF(1.f, 2.f, 3.f, 4.f, 5.f));
        REQUIRE(l.slice(1) == LF(2.f, 3.f, 4.f, 5.f));
        REQUIRE(l.slice(2) == LF(3.f, 4.f, 5.f));
        REQUIRE(l.slice(3) == LF(4.f, 5.f));
        REQUIRE(l.slice(4) == LF(5.f));
        REQUIRE(l.slice(5) == L());

        REQUIRE(l.slice(-1) == LF(5.f));
        REQUIRE(l.slice(-2) == LF(4.f, 5.f));
        REQUIRE(l.slice(-3) == LF(3.f, 4.f, 5.f));
        REQUIRE(l.slice(-4) == LF(2.f, 3.f, 4.f, 5.f));
        REQUIRE(l.slice(-5) == LF(1.f, 2.f, 3.f, 4.f, 5.f));

        AtomList l2;
        REQUIRE(l2.slice(0) == L());
        REQUIRE(l2.slice(1) == L());
        REQUIRE(l2.slice(-1) == L());
    }

    SECTION("test slice2")
    {
        AtomList l = LF(1, 2, 3, 4, 5);
        REQUIRE(l.slice(0, -1) == LF(1, 2, 3, 4, 5));
        REQUIRE(l.slice(10, -1) == L());
        REQUIRE(l.slice(1, 5) == LF(2, 3, 4, 5));
        REQUIRE(l.slice(2, -1) == LF(3, 4, 5));
        REQUIRE(l.slice(2, -10) == LF(3, 2, 1));
        REQUIRE(l.slice(-2, -3) == LF(4, 3));
        REQUIRE(l.slice(-10, -2) == L());
        REQUIRE(l.slice(-3, -2) == LF(3, 4));
        REQUIRE(l.slice(-20, -10) == L());
        REQUIRE(l.slice(-5, -5) == LF(1));
        REQUIRE(l.slice(-5, 0) == LF(1));
        REQUIRE(l.slice(-5, 1) == LF(1, 2));
        REQUIRE(l.slice(4, 5) == LF(5));

        AtomList l2;
        REQUIRE(l2.slice(0, 0) == L());
        REQUIRE(l2.slice(0, 1) == L());
        REQUIRE(l2.slice(0, -1) == L());
        REQUIRE(l2.slice(-1, -1) == L());
        REQUIRE(l2.slice(-1, 0) == L());
    }

    SECTION("test slice3")
    {
        AtomList l = LF(1.f, 2.f, 3.f, 4.f, 5.f);
        REQUIRE(l.slice(0, -1, 0) == L());
        REQUIRE(l.slice(0, -1, 1) == LF(1, 2, 3, 4, 5));
        REQUIRE(l.slice(0, -1, 2) == LF(1, 3, 5));
        REQUIRE(l.slice(1, -1, 2) == LF(2, 4));
        REQUIRE(l.slice(10, -1) == L());
        REQUIRE(l.slice(10, -1, 2) == L());
        REQUIRE(l.slice(1, 5) == LF(2.f, 3.f, 4.f, 5.f));
        REQUIRE(l.slice(1, 5, 2) == LF(2.f, 4.f));
        REQUIRE(l.slice(2, -1) == LF(3, 4, 5));
        REQUIRE(l.slice(2, -1, 3) == LF(3.f));
        REQUIRE(l.slice(2, -10) == LF(3, 2, 1));
        REQUIRE(l.slice(-2, -3) == LF(4, 3));
        REQUIRE(l.slice(-10, -2) == L());
        REQUIRE(l.slice(-3, -2) == LF(3, 4));
        REQUIRE(l.slice(-20, -10) == L());
        REQUIRE(l.slice(-5, -5) == LF(1));
        REQUIRE(l.slice(-5, 0) == LF(1));
        REQUIRE(l.slice(-5, 1) == LF(1, 2));
        REQUIRE(l.slice(4, 5) == LF(5.f));

        AtomList l2;
        REQUIRE(l2.slice(0, 0) == L());
        REQUIRE(l2.slice(0, 1) == L());
        REQUIRE(l2.slice(0, -1) == L());
        REQUIRE(l2.slice(-1, -1) == L());
        REQUIRE(l2.slice(-1, 0) == L());
    }

    SECTION("test atomlistToValue")
    {
        REQUIRE(atomlistToValue<bool>(AtomList::ones(10), false) == true);
        REQUIRE(atomlistToValue<float>(AtomList::ones(10), 0.f) == 1.f);
        REQUIRE(atomlistToValue<double>(AtomList::ones(10), 0) == 1);
        REQUIRE(atomlistToValue<int>(AtomList::ones(10), 112) == 1);
        REQUIRE(atomlistToValue<size_t>(AtomList::ones(10), 12) == 1);
        REQUIRE(atomlistToValue<size_t>(LF(-1.f), 12) == 12);

        REQUIRE(atomlistToValue<float>(L(), 10.f) == 10.f);
        REQUIRE(atomlistToValue<double>(L(), 11.) == 11.);
        REQUIRE(atomlistToValue<int>(L(), 10) == 10);
        REQUIRE(atomlistToValue<size_t>(L(), 20) == 20);
    }

    SECTION("addMin")
    {
        AtomList l1;
        AtomList l2;

        REQUIRE(AtomList::add(l1, l1) == l1);
        REQUIRE(AtomList::add(l1, l2) == l1);

        l1.append(1.f);
        REQUIRE(AtomList::add(l1, l1) == AtomList::filled(2.f, 1));
        REQUIRE(AtomList::add(l2, l1) == L());
        REQUIRE(AtomList::add(l1, l2) == L());

        l2.append(2.f);
        REQUIRE(AtomList::add(l1, l2) == AtomList::filled(3.f, 1));
        REQUIRE(AtomList::add(l2, l1) == AtomList::filled(3.f, 1));
    }

    SECTION("addPadZero")
    {
        AtomList l1;
        AtomList l2;

        REQUIRE(AtomList::add(l1, l1, AtomList::PADZERO) == L());
        REQUIRE(AtomList::add(l1, l2, AtomList::PADZERO) == L());

        l1.append(1.f);
        REQUIRE(AtomList::add(l1, l1, AtomList::PADZERO) == AtomList::filled(2.f, 1));
        REQUIRE(AtomList::add(l2, l1, AtomList::PADZERO) == AtomList::filled(1.f, 1));
    }

    SECTION("addClip")
    {
        AtomList l1;
        AtomList l2;

        REQUIRE(AtomList::add(l1, l1, AtomList::CLIP) == L());
        REQUIRE(AtomList::add(l1, l2, AtomList::CLIP) == L());

        l1.append(1.f);
        REQUIRE(AtomList::add(l1, l1, AtomList::CLIP) == AtomList::filled(2.f, 1));
        REQUIRE(AtomList::add(l2, l1, AtomList::CLIP) == L());
        REQUIRE(AtomList::add(l1, l2, AtomList::CLIP) == L());

        l2.append(2.f);
        REQUIRE(AtomList::add(l1, l2, AtomList::CLIP) == AtomList::filled(3.f, 1));
        REQUIRE(AtomList::add(l2, l1, AtomList::CLIP) == AtomList::filled(3.f, 1));

        l1.append(2.f);
        l1.append(3.f);
        l1.append(4.f);

        // [2, 2, 2, 2]
        // [1, 2, 3, 4]
        // [3, 4, 5, 6]
        REQUIRE(AtomList::add(l1, l2, AtomList::CLIP) == LF(3.f, 4.f, 5.f, 6.f));
        REQUIRE(AtomList::add(l2, l1, AtomList::CLIP) == LF(3.f, 4.f, 5.f, 6.f));
    }

    SECTION("addWrap")
    {
        AtomList l1;
        AtomList l2;

        REQUIRE(AtomList::add(l1, l1, AtomList::WRAP) == L());
        REQUIRE(AtomList::add(l1, l2, AtomList::WRAP) == L());

        l1.append(1.f);
        REQUIRE(AtomList::add(l1, l1, AtomList::WRAP) == AtomList::filled(2.f, 1));
        REQUIRE(AtomList::add(l2, l1, AtomList::WRAP) == L());
        REQUIRE(AtomList::add(l1, l2, AtomList::WRAP) == L());

        l2.append(2.f);
        REQUIRE(AtomList::add(l1, l2, AtomList::WRAP) == AtomList::filled(3.f, 1));
        REQUIRE(AtomList::add(l2, l1, AtomList::WRAP) == AtomList::filled(3.f, 1));

        l1.append(2.f);
        l1.append(3.f);
        l1.append(4.f);
        l1.append(5.f);
        l1.append(6.f);
        l1.append(7.f);
        l1.append(8.f);

        l2.append(1.f);
        l2.append(3.f);

        // [1, 2, 3, 4, 5, 6, 7, 8]
        // [2, 1, 3]
        // [2, 1, 3, 2, 1, 3, 2, 1] - wrapped version
        // [3, 3, 6, 6, 6, 9, 9, 9] - sum
        REQUIRE(AtomList::add(l1, l2, AtomList::WRAP) == LF(3., 3., 6., 6., 6., 9., 9., 9.));
        REQUIRE(AtomList::add(l2, l1, AtomList::WRAP) == LF(3., 3., 6., 6., 6., 9., 9., 9.));
    }

    SECTION("addFold")
    {
        AtomList l1;
        AtomList l2;

        REQUIRE(AtomList::add(l1, l1, AtomList::FOLD) == L());
        REQUIRE(AtomList::add(l1, l2, AtomList::FOLD) == L());

        l1.append(1.f);
        REQUIRE(AtomList::add(l1, l1, AtomList::FOLD) == AtomList::filled(2.f, 1));
        REQUIRE(AtomList::add(l2, l1, AtomList::FOLD) == L());
        REQUIRE(AtomList::add(l1, l2, AtomList::FOLD) == L());

        l2.append(2.f);
        REQUIRE(AtomList::add(l1, l2, AtomList::FOLD) == AtomList::filled(3.f, 1));
        REQUIRE(AtomList::add(l2, l1, AtomList::FOLD) == AtomList::filled(3.f, 1));

        l1.append(2.f);
        l1.append(3.f);
        l1.append(4.f);
        l1.append(5.f);
        l1.append(6.f);
        l1.append(7.f);
        l1.append(8.f);

        l2.append(1.f);
        l2.append(3.f);

        // [1, 2, 3, 4, 5, 6, 7, 8]
        // [2, 1, 3]
        // [2, 1, 3, 1, 2, 1, 3, 1] - folded version
        // [3, 3, 6, 5, 7, 7, 10, 9] - sum
        REQUIRE(AtomList::add(l1, l2, AtomList::FOLD) == LF(3., 3., 6., 5., 7., 7., 10., 9.));
        REQUIRE(AtomList::add(l2, l1, AtomList::FOLD) == LF(3., 3., 6., 5., 7., 7., 10., 9.));
    }

    SECTION("listFrom templates")
    {
        SECTION("bool")
        {
            REQUIRE(listFrom(true) == AtomList::ones(1));
            REQUIRE(listFrom(false) == AtomList::zeroes(1));
        }

        SECTION("number")
        {
            REQUIRE(listFrom(1) == AtomList::ones(1));
            REQUIRE(listFrom(0) == AtomList::zeroes(1));
            REQUIRE(listFrom(1.) == AtomList::ones(1));
            REQUIRE(listFrom(0.) == AtomList::zeroes(1));
            REQUIRE(listFrom(1.f) == AtomList::ones(1));
            REQUIRE(listFrom(0.f) == AtomList::zeroes(1));
        }

        SECTION("t_symbol")
        {
            REQUIRE(listFrom(gensym("a")).size() == 1);
            REQUIRE(listFrom(gensym("a")).at(0).asSymbol() == gensym("a"));
        }
    }

    SECTION("atomlistToValue templates")
    {
        SECTION("bool")
        {
            REQUIRE(atomlistToValue<bool>(L(), true) == true);
            REQUIRE(atomlistToValue<bool>(L(), false) == false);
            REQUIRE(atomlistToValue<bool>(AtomList::filled(0.f, 1), true) == false);
            REQUIRE(atomlistToValue<bool>(AtomList::filled(1.f, 1), true) == true);

            REQUIRE(atomlistToValue<bool>(AtomList::filled(0.f, 11), true) == false);
            REQUIRE(atomlistToValue<bool>(AtomList::filled(1.f, 11), true) == true);
            REQUIRE(atomlistToValue<bool>(AtomList::filled(-10.f, 11), false) == true);

            AtomList l;
            l.append(gensym("true"));
            REQUIRE(atomlistToValue<bool>(l, true) == true);

            l.clear();
            l.append(gensym("false"));
            REQUIRE(atomlistToValue<bool>(l, true) == false);

            l.clear();
            l.append(gensym("TRUE"));
            REQUIRE(atomlistToValue<bool>(l, true) == false);

            l.clear();
            l.append(gensym("UNKNOWN"));
            REQUIRE(atomlistToValue<bool>(l, true) == false);

            l.clear();
            l.append(Atom());
            REQUIRE(atomlistToValue<bool>(l, true) == false);
        }
    }

    SECTION("test reduce")
    {
        SECTION("template")
        {
            AtomList l;
            REQUIRE(l.reduce<t_float>(1, &atomSum) == MaybeFloat(1));
            REQUIRE(l.reduce<t_float>(1, &atomMul) == MaybeFloat(1));
            l.append(1.f);
            l.append(2.f);
            REQUIRE(l.reduce<t_float>(0.5f, &atomSum) == MaybeFloat(3.5));
            REQUIRE(l.reduce<t_float>(2, &atomMul) == MaybeFloat(4));
            l.append(gensym("a"));
            REQUIRE(l.reduce<t_float>(0.5f, &atomSum) == MaybeFloat(3.5));
            REQUIRE(l.reduce<t_float>(2, &atomMul) == MaybeFloat(4));
            l.append(2.f);
            REQUIRE(l.reduce<t_float>(0.5f, &atomSum) == MaybeFloat(5.5));
            REQUIRE(l.reduce<t_float>(2, &atomMul) == MaybeFloat(8));
        }

        SECTION("float")
        {

            AtomList l;
            REQUIRE(l.reduceFloat(1.f, &floatSum) == boost::none);
            REQUIRE(l.reduceFloat(1.f, &floatMul) == boost::none);
            l.append(1.f);
            l.append(2.f);
            REQUIRE(l.reduceFloat(0.5f, &floatSum) == MaybeFloat(3.5));
            REQUIRE(l.reduceFloat(2.f, &floatMul) == MaybeFloat(4));
            l.append(gensym("a"));
            REQUIRE(l.reduceFloat(0.5f, &floatSum) == MaybeFloat(3.5));
            REQUIRE(l.reduceFloat(2.f, &floatMul) == MaybeFloat(4));
            l.append(2.f);
            REQUIRE(l.reduceFloat(0.5f, &floatSum) == MaybeFloat(5.5));
            REQUIRE(l.reduceFloat(2.f, &floatMul) == MaybeFloat(8));
        }
    }

    SECTION("test sum")
    {
        REQUIRE(L().sum() == boost::none);
        REQUIRE(LA("none").sum() == boost::none);
        REQUIRE(*LF(1).sum() == 1);
        REQUIRE(*LF(1, 2, 3, 4, 5).sum() == 15);
        REQUIRE(*LA("ABC", 1, 2, 3, 4, 5).sum() == 15);
    }

    SECTION("test product")
    {
        REQUIRE(L().product() == boost::none);
        REQUIRE(LA("none").product() == boost::none);
        REQUIRE(*LF(0).product() == 0);
        REQUIRE(*LF(1).product() == 1);
        REQUIRE(*LF(1, 2, 3).product() == 6);
        REQUIRE(*LF(1, 2, 3, 4).product() == 24);
        REQUIRE(*LA("ABC", 1, 2, 3, 4).product() == 24);
    }
}
