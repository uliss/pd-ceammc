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
#include "catch.hpp"

#include "base_extension_test.h"
#include "ceammc_atomlist.h"

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
            REQUIRE((AtomList() += 1.0) == AtomList());
            REQUIRE((AtomList() -= 1.0) == AtomList());
            REQUIRE((AtomList() *= 1.0) == AtomList());
            REQUIRE((AtomList() /= 1.0) == AtomList());

            AtomList l = AtomList::values(3, -1.0, 0.0, 2.0);
            REQUIRE((l += 10) == AtomList::values(3, 9.0, 10.0, 12.0));
            REQUIRE((l -= 5) == AtomList::values(3, 4.0, 5.0, 7.0));
            REQUIRE((l *= 2) == AtomList::values(3, 8.0, 10.0, 14.0));
            REQUIRE((l /= 2) == AtomList::values(3, 4.0, 5.0, 7.0));
            REQUIRE((l /= 0) == AtomList::values(3, 4.0, 5.0, 7.0));

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
        REQUIRE_FALSE(AtomList().property("@a", static_cast<AtomList*>(0)));

        AtomList plist;
        REQUIRE_FALSE(AtomList().property("@a", &plist));
        REQUIRE(plist.empty());

        REQUIRE_FALSE(AtomList(gensym("@b")).property("@a", &plist));

        // test for empty property
        plist.append(23);
        REQUIRE(AtomList(gensym("value"), gensym("@a")).property("@a", &plist));
        REQUIRE(plist.empty());

        AtomList lst;
        lst.append(F(2));
        lst.append(P("@a"));
        lst.append(P("@b"));
        lst.append(F(3));
        lst.append(F(4));
        lst.append(F(5));
        lst.append(P("@c"));
        lst.append(F(6));

        REQUIRE(lst.property("@a", &plist));
        REQUIRE(plist == AtomList());

        REQUIRE(lst.property("@b", &plist));
        REQUIRE(plist == AtomList::values(3, 3.0, 4.0, 5.0));

        REQUIRE(lst.property("@a", &plist));
        REQUIRE(plist == AtomList());

        REQUIRE(lst.property("@c", &plist));
        REQUIRE(plist == AtomList(F(6)));

        REQUIRE_FALSE(lst.property("@d", &plist));
        REQUIRE(plist == AtomList(F(6)));
    }

    SECTION("test is*")
    {
        SECTION("isBang")
        {
            REQUIRE(AtomList().isBang());
            REQUIRE_FALSE(AtomList(1).isBang());
        }

        SECTION("isFloat")
        {
            REQUIRE_FALSE(AtomList().isFloat());
            REQUIRE_FALSE(AtomList(1, 2).isFloat());
            REQUIRE_FALSE(AtomList(gensym("s")).isFloat());
            REQUIRE(AtomList(Atom(1)).isFloat());
        }

        SECTION("isSymbol")
        {
            REQUIRE_FALSE(AtomList().isSymbol());
            REQUIRE_FALSE(AtomList(1).isSymbol());
            REQUIRE_FALSE(AtomList(gensym("a"), gensym("b")).isSymbol());
            REQUIRE_FALSE(AtomList(gensym("a")).isProperty());
            REQUIRE(AtomList(gensym("a")).isSymbol());
        }

        SECTION("isProperty")
        {
            REQUIRE_FALSE(AtomList().isProperty());
            REQUIRE_FALSE(AtomList(1).isProperty());
            REQUIRE_FALSE(AtomList(gensym("@a"), gensym("@b")).isProperty());
            REQUIRE_FALSE(AtomList(gensym("a")).isProperty());
            REQUIRE(AtomList(gensym("@a")).isProperty());
            REQUIRE(AtomList(gensym("@a")).isSymbol());
        }

        SECTION("isList")
        {
            REQUIRE_FALSE(AtomList().isList());
            REQUIRE_FALSE(AtomList(1).isList());
            REQUIRE_FALSE(AtomList(gensym("a")).isList());
            REQUIRE(AtomList(1, 2).isList());
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
    }

    SECTION("test map")
    {
        SECTION("float")
        {
            REQUIRE(L3(1, 2, 3).map(&mul10) == L3(10, 20, 30));
            REQUIRE(L3("a", "b", "@c").map(&neg) == L3("a", "b", "@c"));
            REQUIRE(L3("a", 100, "c").map(&neg) == L3("a", -100, "c"));
        }

        SECTION("symbol")
        {
            REQUIRE(L3(1, 2, 3).map(&toUpper) == L3(1, 2, 3));
            REQUIRE(L3("a", "b", "c").map(&toUpper) == L3("A", "B", "C"));
            REQUIRE(L3("@a", "b", "c").map(&toUpper) == L3("@A", "B", "C"));
            REQUIRE(L3("A", "B", "C").map(&toUpper) == L3("A", "B", "C"));
            REQUIRE(L3("a", 100, "c").map(&toUpper) == L3("A", 100, "C"));
        }

        SECTION("atom")
        {
            REQUIRE(L3(1, 2, 3).map(&testMap1) == L3(100, 200, 300));
            REQUIRE(L3("a", 0.01f, "@c").map(&testMap1) == L3("A", 1, "@C"));
        }
    }

    SECTION("operators")
    {
        AtomList lst = L3(10, -12, "a");
        REQUIRE(lst + 10 == L3(20, -2, "a"));
        REQUIRE(lst - 9 == L3(1, -21, "a"));
        REQUIRE(lst * 10 == L3(100, -120, "a"));
        REQUIRE(lst / 10 == L3(1, -1.2f, "a"));
    }

    SECTION("listFrom")
    {
        REQUIRE(listFrom(true) == L1(1.0f));
        REQUIRE(listFrom(false) == L1(0.0f));
        REQUIRE(listFrom(std::string("string")) == L1("string"));
        REQUIRE(listFrom(23) == L1(23));
        REQUIRE(listFrom(L3("a", "b", 3)) == L3("a", "b", 3));
        REQUIRE(listFrom(Atom(1)) == L1(1));

        using namespace std;
        vector<string> v;
        v.push_back("a");
        v.push_back("c");
        REQUIRE(listFrom(v) == L2("a", "c"));
    }

    SECTION("listTo")
    {
        SECTION("bool")
        {
            REQUIRE(atomlistToValue<bool>(AtomList(), false) == false);
            REQUIRE(atomlistToValue<bool>(AtomList(), true) == true);
            REQUIRE(atomlistToValue<bool>(L1(1), false) == true);
            REQUIRE(atomlistToValue<bool>(L1(100), false) == true);
            REQUIRE(atomlistToValue<bool>(L1(-100), false) == true);
            REQUIRE(atomlistToValue<bool>(L1(0.f), false) == false);

            REQUIRE(atomlistToValue<bool>(L1("true"), false) == true);
            REQUIRE(atomlistToValue<bool>(L1("TRUE"), false) == false);
            REQUIRE(atomlistToValue<bool>(L1("false"), false) == false);
            REQUIRE(atomlistToValue<bool>(L1("abc"), false) == false);
        }

        SECTION("int")
        {
            REQUIRE(atomlistToValue<int>(AtomList(), -1) == -1);
            REQUIRE(atomlistToValue<int>(L1(123), -1) == 123);
            REQUIRE(atomlistToValue<int>(L2(123, 124), -1) == 123);

            REQUIRE(atomlistToValue<int>(L1("a"), -1) == -1);
            REQUIRE(atomlistToValue<int>(L2("a", 123), -1) == -1);
        }

        SECTION("size")
        {
            REQUIRE(atomlistToValue<size_t>(AtomList(), 0) == 0);
            REQUIRE(atomlistToValue<size_t>(L1(123), 0) == 123);
            REQUIRE(atomlistToValue<size_t>(L1(-123), 0) == 0);
            REQUIRE(atomlistToValue<size_t>(L2(123, 124), 0) == 123);

            REQUIRE(atomlistToValue<size_t>(L1("a"), 0) == 0);
            REQUIRE(atomlistToValue<size_t>(L2("a", 123), 0) == 0);
        }

        SECTION("float")
        {
            REQUIRE(atomlistToValue<float>(AtomList(), -1) == -1.f);
            REQUIRE(atomlistToValue<float>(L1(123), -1) == 123.f);
            REQUIRE(atomlistToValue<float>(L2(123, 124), -1) == 123.f);

            REQUIRE(atomlistToValue<float>(L1("a"), -1) == -1);
            REQUIRE(atomlistToValue<float>(L2("a", 123), -1) == -1);
        }

        SECTION("Atom")
        {
            REQUIRE(atomlistToValue<Atom>(AtomList(), Atom()).isNone());
            REQUIRE(atomlistToValue<Atom>(L1(123), Atom()) == Atom(123));
            REQUIRE(atomlistToValue<Atom>(L2("a", 124), Atom()) == Atom(gensym("a")));
        }
    }
}
