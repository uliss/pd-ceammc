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

#include <algorithm>
#include <cctype>

static t_float mul10(t_float v)
{
    return v * 10;
}

static t_float neg(t_float v)
{
    return -v;
}

static t_symbol* toUpper(t_symbol* s)
{
    std::string str(s->s_name);
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return gensym(str.c_str());
}

static Atom testMap1(const Atom& a)
{
    Atom res(a);

    if (a.isFloat())
        res.setFloat(a.asFloat() * 100);

    if (a.isSymbol())
        res.setSymbol(toUpper(a.asSymbol()));

    return res;
}

using namespace ceammc;

TEST_CASE("AtomList2", "[ceammc::AtomList]")
{
    SECTION("resize")
    {
        SECTION("clip")
        {
            AtomList l;
            l.resizeClip(10);
            REQUIRE(l.empty());

            l.append(1.f);

            l.resizeClip(10);
            REQUIRE(l == AtomList::ones(10));
            l.resizeClip(1);

            l.append(2.f);
            l.append(3.f);

            AtomList newl = l;
            newl.resizeClip(0);
            REQUIRE(newl.empty());

            newl = l;
            newl.resizeClip(1);
            REQUIRE(newl.size() == 1);
            REQUIRE(newl[0] == 1.f);

            newl = l;
            newl.resizeClip(2);
            REQUIRE(newl.size() == 2);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);

            newl = l;
            newl.resizeClip(3);
            REQUIRE(newl.size() == 3);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);
            REQUIRE(newl[2] == 3.f);

            newl = l;
            newl.resizeClip(4);
            REQUIRE(newl.size() == 4);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);
            REQUIRE(newl[2] == 3.f);
            REQUIRE(newl[3] == 3.f);

            newl = l;
            newl.resizeClip(5);
            REQUIRE(newl.size() == 5);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);
            REQUIRE(newl[2] == 3.f);
            REQUIRE(newl[3] == 3.f);
            REQUIRE(newl[4] == 3.f);
        }

        SECTION("wrap")
        {
            AtomList l;
            l.resizeWrap(10);
            REQUIRE(l.empty());

            l.append(1.f);

            l.resizeWrap(10);
            REQUIRE(l == AtomList::ones(10));
            l.resizeWrap(1);

            l.append(2.f);
            l.append(3.f);

            AtomList newl = l;
            newl.resizeWrap(0);
            REQUIRE(newl.empty());

            newl = l;
            newl.resizeWrap(1);
            REQUIRE(newl.size() == 1);
            REQUIRE(newl[0] == 1.f);

            newl = l;
            newl.resizeWrap(2);
            REQUIRE(newl.size() == 2);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);

            newl = l;
            newl.resizeWrap(3);
            REQUIRE(newl.size() == 3);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);
            REQUIRE(newl[2] == 3.f);

            newl = l;
            newl.resizeWrap(4);
            REQUIRE(newl.size() == 4);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);
            REQUIRE(newl[2] == 3.f);
            REQUIRE(newl[3] == 1.f);

            newl = l;
            newl.resizeWrap(5);
            REQUIRE(newl.size() == 5);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);
            REQUIRE(newl[2] == 3.f);
            REQUIRE(newl[3] == 1.f);
            REQUIRE(newl[4] == 2.f);

            newl = l;
            newl.resizeWrap(6);
            REQUIRE(newl.size() == 6);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);
            REQUIRE(newl[2] == 3.f);
            REQUIRE(newl[3] == 1.f);
            REQUIRE(newl[4] == 2.f);
            REQUIRE(newl[5] == 3.f);

            newl = l;
            newl.resizeWrap(7);
            REQUIRE(newl.size() == 7);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);
            REQUIRE(newl[2] == 3.f);
            REQUIRE(newl[3] == 1.f);
            REQUIRE(newl[4] == 2.f);
            REQUIRE(newl[5] == 3.f);
            REQUIRE(newl[6] == 1.f);
        }

        SECTION("fold")
        {
            AtomList l;
            l.resizeFold(10);
            REQUIRE(l.empty());

            l.append(1.f);

            l.resizeFold(10);
            REQUIRE(l == AtomList::ones(10));
            l.resizeFold(1);

            l.append(2.f);
            l.append(3.f);

            AtomList newl = l;
            newl.resizeFold(0);
            REQUIRE(newl.empty());

            newl = l;
            newl.resizeFold(1);
            REQUIRE(newl.size() == 1);
            REQUIRE(newl[0] == 1.f);

            newl = l;
            newl.resizeFold(2);
            REQUIRE(newl.size() == 2);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);

            newl = l;
            newl.resizeFold(3);
            REQUIRE(newl.size() == 3);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);
            REQUIRE(newl[2] == 3.f);

            newl = l;
            newl.resizeFold(4);
            REQUIRE(newl.size() == 4);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);
            REQUIRE(newl[2] == 3.f);
            REQUIRE(newl[3] == 2.f);

            newl = l;
            newl.resizeFold(5);
            REQUIRE(newl.size() == 5);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);
            REQUIRE(newl[2] == 3.f);
            REQUIRE(newl[3] == 2.f);
            REQUIRE(newl[4] == 1.f);

            newl = l;
            newl.resizeFold(6);
            REQUIRE(newl.size() == 6);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);
            REQUIRE(newl[2] == 3.f);
            REQUIRE(newl[3] == 2.f);
            REQUIRE(newl[4] == 1.f);
            REQUIRE(newl[5] == 2.f);

            newl = l;
            newl.resizeFold(7);
            REQUIRE(newl.size() == 7);
            REQUIRE(newl[0] == 1.f);
            REQUIRE(newl[1] == 2.f);
            REQUIRE(newl[2] == 3.f);
            REQUIRE(newl[3] == 2.f);
            REQUIRE(newl[4] == 1.f);
            REQUIRE(newl[5] == 2.f);
            REQUIRE(newl[6] == 3.f);
        }
    }

    SECTION("range")
    {
        AtomList l;
        Atom min, max;
        REQUIRE(l.range(min, max) == false);

        l.append(10.f);
        REQUIRE(l.range(min, max));
        REQUIRE(min == 10.f);
        REQUIRE(max == 10.f);

        l.append(-3.f);
        REQUIRE(l.range(min, max));
        REQUIRE(min == -3.f);
        REQUIRE(max == 10.f);

        l.append(2.f);
        REQUIRE(l.range(min, max));
        REQUIRE(min == -3.f);
        REQUIRE(max == 10.f);

        l.append(20.f);
        REQUIRE(l.range(min, max));
        REQUIRE(min == -3.f);
        REQUIRE(max == 20.f);
    }

    SECTION("double operators")
    {
        SECTION("assign")
        {
            REQUIRE((L() += 1.0) == L());
            REQUIRE((L() -= 1.0) == L());
            REQUIRE((L() *= 1.0) == L());
            REQUIRE((L() /= 1.0) == L());

            AtomList l = LF(-1.0, 0.0, 2.0);
            REQUIRE((l += 10) == LF(9.0, 10.0, 12.0));
            REQUIRE((l -= 5) == LF(4.0, 5.0, 7.0));
            REQUIRE((l *= 2) == LF(8.0, 10.0, 14.0));
            REQUIRE((l /= 2) == LF(4.0, 5.0, 7.0));
            REQUIRE((l /= 0) == LF(4.0, 5.0, 7.0));

            l.clear();
            l.append(2.0);
            l.append(gensym("a"));
            l += 10;
            REQUIRE(l[0] == 12.0);
            REQUIRE(l[1] == gensym("a"));
            l -= 5;
            REQUIRE(l[0] == 7.0);
            REQUIRE(l[1] == gensym("a"));
            l *= 8;
            REQUIRE(l[0] == 56.0);
            REQUIRE(l[1] == gensym("a"));
            l /= 4;
            REQUIRE(l[0] == 14.0);
            REQUIRE(l[1] == gensym("a"));
            l /= 0;
            REQUIRE(l[0] == 14.0);
            REQUIRE(l[1] == gensym("a"));
        }
    }

    SECTION("property get atomlist")
    {
        // empty data
        REQUIRE_FALSE(L().property("@a", static_cast<AtomList*>(0)));

        AtomList plist;
        REQUIRE_FALSE(L().property("@a", &plist));
        REQUIRE(plist.empty());

        REQUIRE_FALSE(AtomList(gensym("@b")).property("@a", &plist));

        // test for empty property
        plist.append(23);
        REQUIRE(AtomList(gensym("value"), gensym("@a")).property("@a", &plist));
        REQUIRE(plist.empty());

        AtomList lst;
        lst.append(F(2));
        lst.append(A("@a"));
        lst.append(A("@b"));
        lst.append(F(3));
        lst.append(F(4));
        lst.append(F(5));
        lst.append(A("@c"));
        lst.append(F(6));

        REQUIRE(lst.property("@a", &plist));
        REQUIRE(plist == L());

        REQUIRE(lst.property("@b", &plist));
        REQUIRE(plist == LF(3.0, 4.0, 5.0));

        REQUIRE(lst.property("@a", &plist));
        REQUIRE(plist == L());

        REQUIRE(lst.property("@c", &plist));
        REQUIRE(plist == AtomList(F(6)));

        REQUIRE_FALSE(lst.property("@d", &plist));
        REQUIRE(plist == AtomList(F(6)));
    }

    SECTION("test is*")
    {
        SECTION("isBang")
        {
            REQUIRE(L().isBang());
            REQUIRE_FALSE(LF(1).isBang());
        }

        SECTION("isAtom")
        {
            REQUIRE_FALSE(L().isAtom());
            REQUIRE_FALSE(LF(1, 2).isAtom());
            REQUIRE(LA("ABC").isAtom());
            REQUIRE(LA(100).isAtom());
        }

        SECTION("isBool")
        {
            REQUIRE_FALSE(L().isBool());
            REQUIRE_FALSE(LF(1, 2).isBool());
            REQUIRE_FALSE(LA("True").isBool());
            REQUIRE(LA("true").isBool());
            REQUIRE(LA("false").isBool());
            REQUIRE(LA(1).isBool());
            REQUIRE(LA(0.0).isBool());
        }

        SECTION("isFloat")
        {
            REQUIRE_FALSE(L().isFloat());
            REQUIRE_FALSE(AtomList(1, 2).isFloat());
            REQUIRE_FALSE(AtomList(gensym("s")).isFloat());
            REQUIRE(AtomList(Atom(1)).isFloat());
        }

        SECTION("isSymbol")
        {
            REQUIRE_FALSE(L().isSymbol());
            REQUIRE_FALSE(LF(1).isSymbol());
            REQUIRE_FALSE(AtomList(gensym("a"), gensym("b")).isSymbol());
            REQUIRE_FALSE(AtomList(gensym("a")).isProperty());
            REQUIRE(AtomList(gensym("a")).isSymbol());
        }

        SECTION("isProperty")
        {
            REQUIRE_FALSE(L().isProperty());
            REQUIRE_FALSE(LF(1).isProperty());
            REQUIRE_FALSE(AtomList(gensym("@a"), gensym("@b")).isProperty());
            REQUIRE_FALSE(AtomList(gensym("a")).isProperty());
            REQUIRE(AtomList(gensym("@a")).isProperty());
            REQUIRE(AtomList(gensym("@a")).isSymbol());
        }

        SECTION("isList")
        {
            REQUIRE_FALSE(L().isList());
            REQUIRE_FALSE(LF(1).isList());
            REQUIRE_FALSE(AtomList(gensym("a")).isList());
            REQUIRE(AtomList(1, 2).isList());
        }

        SECTION("isData")
        {
            REQUIRE_FALSE(L().isData());
            REQUIRE_FALSE(LF(1).isData());
            REQUIRE_FALSE(AtomList(gensym("a")).isData());
            REQUIRE_FALSE(AtomList(1, 2).isData());
            REQUIRE_FALSE(AtomList(Atom(DataDesc(1, 1)), Atom(DataDesc(1, 1))).isData());
            REQUIRE(AtomList(Atom(DataDesc(1, 1))).isData());
        }

        SECTION("isDataType")
        {
            REQUIRE_FALSE(L().isDataType(1));
            REQUIRE_FALSE(LF(1).isDataType(1));
            REQUIRE_FALSE(AtomList(gensym("a")).isDataType(1));
            REQUIRE_FALSE(AtomList(1, 2).isDataType(1));
            REQUIRE_FALSE(AtomList(Atom(DataDesc(1, 1)), Atom(DataDesc(1, 1))).isDataType(1));
            REQUIRE(AtomList(Atom(DataDesc(1, 10))).isDataType(1));
            REQUIRE_FALSE(AtomList(Atom(DataDesc(1, 10))).isDataType(2));
        }

        SECTION("all")
        {
            AtomList bng;
            AtomList flt(1);
            AtomList sym(gensym("a"));
            AtomList prop(gensym("@prop"));
            AtomList lst(1, 2);

            REQUIRE(bng.isBang());
            REQUIRE(flt.isFloat());
            REQUIRE(sym.isSymbol());
            REQUIRE(prop.isSymbol());
            REQUIRE(prop.isProperty());
            REQUIRE(lst.isList());
        }

        SECTION("isA<>")
        {
            REQUIRE_FALSE(L().isA<bool>());
            REQUIRE_FALSE(L().isA<t_float>());
            REQUIRE_FALSE(L().isA<int>());
            REQUIRE_FALSE(L().isA<t_symbol*>());
            REQUIRE_FALSE(L().isA<Atom>());
            REQUIRE(L().isA<AtomList>());

            // bool
            REQUIRE(LA("true").isA<bool>());
            REQUIRE(LA("false").isA<bool>());
            REQUIRE(LA(1).isA<bool>());
            REQUIRE(LF(0).isA<bool>());

            REQUIRE_FALSE(LA("True").isA<bool>());
            REQUIRE_FALSE(LA("False").isA<bool>());
            REQUIRE_FALSE(LA("ABC").isA<bool>());
            REQUIRE_FALSE(LA("@true").isA<bool>());
            REQUIRE_FALSE(LF(-1).isA<bool>());
            REQUIRE_FALSE(LF(0.0001).isA<bool>());
            REQUIRE_FALSE(LF(1.0001).isA<bool>());
            REQUIRE_FALSE(LA("true", "true").isA<bool>());
            REQUIRE_FALSE(LA("false", "B").isA<bool>());
            REQUIRE_FALSE(LF(1, 2).isA<bool>());
            REQUIRE_FALSE(LF(0, 0).isA<bool>());

            // float
            REQUIRE(LA(1).isA<t_float>());
            REQUIRE(LF(0).isA<t_float>());
            REQUIRE(LA(-1).isA<t_float>());
            REQUIRE(LF(0.5).isA<t_float>());
            REQUIRE(LA(-0.1).isA<t_float>());

            REQUIRE_FALSE(LF(1, 2).isA<t_float>());
            REQUIRE_FALSE(LA("").isA<t_float>());
            REQUIRE_FALSE(LA("100").isA<t_float>());
            REQUIRE_FALSE(LA("ABC").isA<t_float>());
            REQUIRE_FALSE(LA("@prop").isA<t_float>());

            // int
            REQUIRE(LA(1).isA<int>());
            REQUIRE(LF(0).isA<int>());
            REQUIRE(LA(-1).isA<int>());
            REQUIRE_FALSE(LF(0.0001).isA<int>());
            REQUIRE_FALSE(LA(-0.0001).isA<int>());
            REQUIRE_FALSE(LA(1.0001).isA<int>());
            REQUIRE_FALSE(LA(-1.0001).isA<int>());
            REQUIRE_FALSE(LA(-0.9999).isA<int>());
            REQUIRE_FALSE(LA(0.9999).isA<int>());

            REQUIRE_FALSE(LF(1, 2).isA<int>());
            REQUIRE_FALSE(LA("").isA<int>());
            REQUIRE_FALSE(LA("100").isA<int>());
            REQUIRE_FALSE(LA("ABC").isA<int>());
            REQUIRE_FALSE(LA("@prop").isA<int>());

            // symbol
            REQUIRE(LA("").isA<t_symbol*>());
            REQUIRE(LA("100").isA<t_symbol*>());
            REQUIRE(LA("ABC").isA<t_symbol*>());
            REQUIRE(LA("@prop").isA<t_symbol*>());

            REQUIRE_FALSE(L().isA<t_symbol*>());
            REQUIRE_FALSE(LF(0).isA<t_symbol*>());
            REQUIRE_FALSE(LA("A", "B").isA<t_symbol*>());

            // atom
            REQUIRE(LA(1).isA<Atom>());
            REQUIRE(LA(1.6).isA<Atom>());
            REQUIRE(LA("A").isA<Atom>());
            REQUIRE(LA("@prop").isA<Atom>());
            REQUIRE(LA("").isA<Atom>());

            REQUIRE_FALSE(L().isA<Atom>());
            REQUIRE_FALSE(LA(1, 2).isA<Atom>());
            REQUIRE_FALSE(LA("A", 1).isA<Atom>());

            // list
            REQUIRE(L().isA<AtomList>());
            REQUIRE(LA("A").isA<AtomList>());
            REQUIRE(LF(1).isA<AtomList>());
            REQUIRE(LA("true").isA<AtomList>());
            REQUIRE(LF(1, 2).isA<AtomList>());
            REQUIRE(LA("Ab", 1, 2).isA<AtomList>());
        }
    }

    SECTION("test map")
    {
        SECTION("float")
        {
            REQUIRE(LF(1, 2, 3).map(&mul10) == LF(10, 20, 30));
            REQUIRE(LA("a", "b", "@c").map(&neg) == LA("a", "b", "@c"));
            REQUIRE(LA("a", 100, "c").map(&neg) == LA("a", -100, "c"));
        }

        SECTION("symbol")
        {
            REQUIRE(LF(1, 2, 3).map(&toUpper) == LF(1, 2, 3));
            REQUIRE(LA("a", "b", "c").map(&toUpper) == LA("A", "B", "C"));
            REQUIRE(LA("@a", "b", "c").map(&toUpper) == LA("@A", "B", "C"));
            REQUIRE(LA("A", "B", "C").map(&toUpper) == LA("A", "B", "C"));
            REQUIRE(LA("a", 100, "c").map(&toUpper) == LA("A", 100, "C"));
        }

        SECTION("atom")
        {
            REQUIRE(LF(1, 2, 3).map(&testMap1) == LF(100, 200, 300));
            REQUIRE(LA("a", 0.01f, "@c").map(&testMap1) == LA("A", 1, "@C"));
        }

        SECTION("map atom")
        {
            auto fn = [](const Atom& a) { return Atom(a.isFloat()); };
            REQUIRE(LA(1, "a", 3).map(fn) == LF(1, 0.f, 1));
        }

        SECTION("map float")
        {
            auto fn = [](t_float f) { return f * 2; };
            REQUIRE(LF(1, 2, 3).mapFloat(fn) == LF(2, 4, 6));
        }
    }

    SECTION("operators")
    {
        AtomList lst = LA(10, -12, "a");
        REQUIRE(lst + 10 == LA(20, -2, "a"));
        REQUIRE(lst - 9 == LA(1, -21, "a"));
        REQUIRE(lst * 10 == LA(100, -120, "a"));
        REQUIRE(lst / 10 == LA(1, -1.2f, "a"));
    }

    SECTION("listFrom")
    {
        REQUIRE(listFrom(true) == LF(1.0f));
        REQUIRE(listFrom(false) == LF(0.0f));
        REQUIRE(listFrom(std::string("string")) == LA("string"));
        REQUIRE(listFrom(23) == LF(23));
        REQUIRE(listFrom(LA("a", "b", 3)) == LA("a", "b", 3));
        REQUIRE(listFrom(Atom(1)) == LF(1));

        using namespace std;
        vector<string> v;
        v.push_back("a");
        v.push_back("c");
        REQUIRE(listFrom(v) == LA("a", "c"));
    }

    SECTION("listTo")
    {
        SECTION("bool")
        {
            REQUIRE(atomlistToValue<bool>(L(), false) == false);
            REQUIRE(atomlistToValue<bool>(L(), true) == true);
            REQUIRE(atomlistToValue<bool>(LF(1), false) == true);
            REQUIRE(atomlistToValue<bool>(LF(100), false) == true);
            REQUIRE(atomlistToValue<bool>(LF(-100), false) == true);
            REQUIRE(atomlistToValue<bool>(LF(0.f), false) == false);

            REQUIRE(atomlistToValue<bool>(LA("true"), false) == true);
            REQUIRE(atomlistToValue<bool>(LA("TRUE"), false) == false);
            REQUIRE(atomlistToValue<bool>(LA("false"), false) == false);
            REQUIRE(atomlistToValue<bool>(LA("abc"), false) == false);
        }

        SECTION("int")
        {
            REQUIRE(atomlistToValue<int>(L(), -1) == -1);
            REQUIRE(atomlistToValue<int>(LF(123), -1) == 123);
            REQUIRE(atomlistToValue<int>(LF(123, 124), -1) == 123);

            REQUIRE(atomlistToValue<int>(LA("a"), -1) == -1);
            REQUIRE(atomlistToValue<int>(LA("a", 123), -1) == -1);
        }

        SECTION("size")
        {
            REQUIRE(atomlistToValue<size_t>(L(), 0) == 0);
            REQUIRE(atomlistToValue<size_t>(LF(123), 0) == 123);
            REQUIRE(atomlistToValue<size_t>(LF(-123), 0) == 0);
            REQUIRE(atomlistToValue<size_t>(LF(123, 124), 0) == 123);

            REQUIRE(atomlistToValue<size_t>(LA("a"), 0) == 0);
            REQUIRE(atomlistToValue<size_t>(LA("a", 123), 0) == 0);
        }

        SECTION("float")
        {
            REQUIRE(atomlistToValue<float>(L(), -1) == -1.f);
            REQUIRE(atomlistToValue<float>(LF(123), -1) == 123.f);
            REQUIRE(atomlistToValue<float>(LF(123, 124), -1) == 123.f);

            REQUIRE(atomlistToValue<float>(LA("a"), -1) == -1);
            REQUIRE(atomlistToValue<float>(LA("a", 123), -1) == -1);
        }

        SECTION("Atom")
        {
            REQUIRE(atomlistToValue<Atom>(L(), Atom()).isNone());
            REQUIRE(atomlistToValue<Atom>(LF(123), Atom()) == Atom(123));
            REQUIRE(atomlistToValue<Atom>(LA("a", 124), Atom()) == Atom(gensym("a")));
        }
    }

    SECTION("operator+")
    {
        REQUIRE(LF(1, 2) + LF(3, 4) == LF(1, 2, 3, 4));
        REQUIRE(LF(1, 2) + Atom(3) == LF(1, 2, 3));
        REQUIRE(Atom(0.f) + LF(1, 2) == LF(0.f, 1, 2));
    }

    SECTION("floatIterator")
    {
        AtomList a(LA(1, "a", -2, 3));
        REQUIRE(*std::min_element(a.beginFilter(isFloat), a.endFilter()) == LX(-2));
        REQUIRE(*std::max_element(a.beginFilter(isFloat), a.endFilter()) == LX(3));
        REQUIRE(*std::min_element(a.begin(), a.end()) == LX(-2));
        REQUIRE(*std::max_element(a.begin(), a.end()) == LA("a"));

        AtomList a1(LA("a", "b", "c", "d", "e"));
        REQUIRE(std::min_element(a1.beginFilter(isFloat), a1.endFilter()) == a1.endFilter());
        REQUIRE(std::max_element(a1.beginFilter(isFloat), a1.endFilter()) == a1.endFilter());
        REQUIRE(std::min_element(a1.begin(), a1.end()) != a1.end());
        REQUIRE(std::max_element(a1.begin(), a1.end()) != a1.end());

        AtomList a2(LA("a", "b", 1, "c", "d", "e"));
        REQUIRE(std::min_element(a2.beginFilter(isFloat), a2.endFilter()) != a2.endFilter());
        REQUIRE(std::max_element(a2.beginFilter(isFloat), a2.endFilter()) != a2.endFilter());
        REQUIRE(std::min_element(a2.begin(), a2.end()) != a2.end());
        REQUIRE(std::max_element(a2.begin(), a2.end()) != a2.end());

        AtomList a3(LF(1, 2, 3));
        REQUIRE(std::min_element(a3.beginFilter(isSymbol), a3.endFilter()) == a3.endFilter());
        REQUIRE(std::max_element(a3.beginFilter(isSymbol), a3.endFilter()) == a3.endFilter());
    }

    SECTION("has property")
    {
        REQUIRE_FALSE(L().hasProperty("@name"));
        REQUIRE_FALSE(LF(1, 2, 3).hasProperty("@name"));
        REQUIRE_FALSE(LA(1, 2, "abc").hasProperty("@name"));
        REQUIRE_FALSE(LA(1, 2, "@abc").hasProperty("@name"));
        REQUIRE(LA(1, "@name", "abc").hasProperty("@name"));
    }

    SECTION("t_atom")
    {
        t_atom a[3];
        SETFLOAT(a + 0, 0);
        SETFLOAT(a + 1, 1);
        SETFLOAT(a + 2, 2);
        REQUIRE(AtomList(int(3), a) == LF(0, 1, 2));
        REQUIRE(AtomList(size_t(2), a + 1) == LF(1, 2));
    }

    SECTION("*At")
    {
        REQUIRE(LF(1.1, 2).intAt(0, -100) == 1);
        REQUIRE(LF(1, 2.999).intAt(1, -100) == 2);
        REQUIRE(LF(1, 2).intAt(2, -100) == -100);
        REQUIRE(LA("A", 2).intAt(0, -1) == -1);
        REQUIRE(L().intAt(0, -100) == -100);

        REQUIRE(LF(1.5, 2).floatAt(0, -100) == Approx(1.5));
        REQUIRE(LF(1, 2.25).floatAt(1, -100) == Approx(2.25));
        REQUIRE(LF(1, 2).floatAt(2, -1) == -1);
        REQUIRE(L().floatAt(0, -100) == -100);

        REQUIRE(LA("A", 1, 3.5).symbolAt(0, SYM("?")) == LA("A"));
        REQUIRE(LA("A", 1, 3.5).symbolAt(1, SYM("?")) == LA("?"));
        REQUIRE(LA("A", 1, 3.5).symbolAt(2, SYM("?")) == LA("?"));
        REQUIRE(LA("A", 1, 3.5).symbolAt(3, SYM("?")) == LA("?"));
        REQUIRE(L().symbolAt(0, nullptr) == nullptr);
    }

    SECTION("isA")
    {
        REQUIRE_FALSE(L().isA<t_symbol*>());
        REQUIRE_FALSE(L().isA<Atom>());
        REQUIRE(L().isA<AtomList>());

        REQUIRE(LA("true").isA<bool>());
        REQUIRE(LA("false").isA<bool>());
        REQUIRE_FALSE(LA("abc").isA<bool>());
        REQUIRE(LF(1).isA<bool>());
        REQUIRE(LF(0).isA<bool>());
        REQUIRE_FALSE(L().isA<bool>());
        REQUIRE_FALSE(LF(0.999).isA<bool>());
        REQUIRE_FALSE(LF(1, 1).isA<bool>());
        REQUIRE_FALSE(LF(0.001).isA<bool>());

        REQUIRE(LF(1).isA<t_float>());
        REQUIRE(LF(0.001).isA<t_float>());
        REQUIRE_FALSE(L().isA<t_float>());
        REQUIRE_FALSE(LA("abc").isA<t_float>());
        REQUIRE_FALSE(LF(1, 2).isA<t_float>());

        REQUIRE(LF(1).isA<int>());
        REQUIRE(LF(-100).isA<int>());
        REQUIRE_FALSE(L().isA<int>());
        REQUIRE_FALSE(LF(0.5).isA<int>());
        REQUIRE_FALSE(LA("abc").isA<int>());
        REQUIRE_FALSE(LF(1, 2).isA<int>());

        REQUIRE(LA("abc").isA<t_symbol*>());
        REQUIRE(LA("@prop").isA<t_symbol*>());
        REQUIRE_FALSE(L().isA<t_symbol*>());
        REQUIRE_FALSE(LF(0.5).isA<t_symbol*>());
        REQUIRE_FALSE(LF(1, 2).isA<t_symbol*>());

        REQUIRE(LA("abc").isA<Atom>());
        REQUIRE(LA("@prop").isA<Atom>());
        REQUIRE(LF(0.5).isA<Atom>());
        REQUIRE_FALSE(L().isA<Atom>());
        REQUIRE_FALSE(LF(1, 2).isA<Atom>());

        REQUIRE_FALSE(L().isInteger());
        REQUIRE_FALSE(LF(1, 2).isInteger());
        REQUIRE(LF(1).isInteger());
        REQUIRE_FALSE(LF(0.5).isInteger());
    }
}
