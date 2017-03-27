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
#include "ceammc_atomlist.h"

using namespace ceammc;

#define P(str) Atom(gensym(#str))
#define F(n)   Atom(float(n))

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
        lst.append(P(@a));
        lst.append(P(@b));
        lst.append(F(3));
        lst.append(F(4));
        lst.append(F(5));
        lst.append(P(@c));
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
}
