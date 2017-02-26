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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ceammc_fn_list.h"

using namespace ceammc;

TEST_CASE("list functions", "[ceammc::list]")
{
    SECTION("interleave")
    {
        SECTION("shortest")
        {
            std::vector<AtomList> lst;
            REQUIRE(list::shortestListSize(lst) == 0);

            lst.push_back(AtomList::ones(10));
            REQUIRE(list::shortestListSize(lst) == 10);
            lst.push_back(AtomList::ones(9));
            REQUIRE(list::shortestListSize(lst) == 9);
            lst.push_back(AtomList::ones(10));
            REQUIRE(list::shortestListSize(lst) == 9);
            lst.push_back(AtomList::ones(3));
            REQUIRE(list::shortestListSize(lst) == 3);
            lst.push_back(AtomList());
            REQUIRE(list::shortestListSize(lst) == 0);
        }

        SECTION("longest")
        {
            std::vector<AtomList> lst;
            REQUIRE(list::longestListSize(lst) == 0);

            lst.push_back(AtomList::ones(1));
            REQUIRE(list::longestListSize(lst) == 1);
            lst.push_back(AtomList::ones(10));
            REQUIRE(list::longestListSize(lst) == 10);
            lst.push_back(AtomList::ones(10));
            REQUIRE(list::longestListSize(lst) == 10);
            lst.push_back(AtomList::ones(3));
            REQUIRE(list::longestListSize(lst) == 10);
            lst.push_back(AtomList());
            REQUIRE(list::longestListSize(lst) == 10);
        }

        SECTION("minmax")
        {
            typedef std::pair<size_t, size_t> Pair;
            std::vector<AtomList> l;
            REQUIRE(list::minmaxListSize(l) == Pair(0, 0));

            l.push_back(AtomList::ones(2));
            REQUIRE(list::minmaxListSize(l) == Pair(2, 2));

            l.push_back(AtomList::ones(4));
            REQUIRE(list::minmaxListSize(l) == Pair(2, 4));

            l.push_back(AtomList::ones(3));
            REQUIRE(list::minmaxListSize(l) == Pair(2, 4));

            l.push_back(AtomList::ones(10));
            REQUIRE(list::minmaxListSize(l) == Pair(2, 10));

            l.push_back(AtomList());
            REQUIRE(list::minmaxListSize(l) == Pair(0, 10));
        }

        SECTION("interleave")
        {
            SECTION("min length")
            {
                std::vector<AtomList> lst;
                AtomList res;
                res = list::interleaveMinLength(lst);
                REQUIRE(res == AtomList());

                lst.push_back(AtomList::ones(3));
                res = list::interleaveMinLength(lst);
                REQUIRE(res == AtomList::ones(3));

                lst.push_back(AtomList::zeroes(5));
                res = list::interleaveMinLength(lst);
                REQUIRE(res.size() == 6);
                REQUIRE(res == AtomList::values(6, 1., 0., 1., 0., 1., 0.));

                lst.push_back(AtomList());
                res = list::interleaveMinLength(lst);
                REQUIRE(res == AtomList());
            }

            SECTION("pad length")
            {
                std::vector<AtomList> l;
                REQUIRE(list::interleavePadWith(l, Atom(1.f)) == AtomList());

                l.push_back(AtomList::ones(3));
                REQUIRE(list::interleavePadWith(l, Atom(2.f)) == AtomList::ones(3));

                l.push_back(AtomList::zeroes(1));
                REQUIRE(list::interleavePadWith(l, Atom(2.f)) == AtomList::values(6, 1., 0., 1., 2., 1., 2.));

                l.clear();
                l.push_back(AtomList());
                REQUIRE(list::interleavePadWith(l, Atom(1.f)) == AtomList());
            }

            SECTION("clip length")
            {
                std::vector<AtomList> l;
                REQUIRE(list::interleaveClip(l) == AtomList());

                l.push_back(AtomList());
                REQUIRE(list::interleaveClip(l) == AtomList());

                l.push_back(AtomList::ones(3));
                REQUIRE(list::interleaveClip(l) == AtomList());

                l.clear();
                l.push_back(AtomList::values(2, 1.0, 2.0));
                l.push_back(AtomList::values(4, -1.0, -2.0, -3.0, -4.0));
                REQUIRE(list::interleaveClip(l) == AtomList::values(8, 1.0, -1.0, 2.0, -2.0, 2.0, -3.0, 2.0, -4.0));
            }

            SECTION("wrap length")
            {
                std::vector<AtomList> l;
                REQUIRE(list::interleaveWrap(l) == AtomList());

                l.push_back(AtomList());
                REQUIRE(list::interleaveWrap(l) == AtomList());

                l.push_back(AtomList::ones(3));
                REQUIRE(list::interleaveWrap(l) == AtomList());

                l.clear();
                l.push_back(AtomList::values(2, 1.0, 2.0));
                l.push_back(AtomList::values(4, -1.0, -2.0, -3.0, -4.0));
                REQUIRE(list::interleaveWrap(l) == AtomList::values(8, 1.0, -1.0, 2.0, -2.0, 1.0, -3.0, 2.0, -4.0));
            }

            SECTION("fold length")
            {
                std::vector<AtomList> l;
                REQUIRE(list::interleaveFold(l) == AtomList());

                l.push_back(AtomList());
                REQUIRE(list::interleaveFold(l) == AtomList());

                l.push_back(AtomList::ones(3));
                REQUIRE(list::interleaveFold(l) == AtomList());

                l.clear();
                l.push_back(AtomList::values(3, 1.0, 2.0, 3.0));
                l.push_back(AtomList::values(4, -1.0, -2.0, -3.0, -4.0));
                REQUIRE(list::interleaveFold(l) == AtomList::values(8, 1.0, -1.0, 2.0, -2.0, 3.0, -3.0, 2.0, -4.0));
            }
        }

        SECTION("deinterleave")
        {
            SECTION("min length")
            {
                std::vector<AtomList> l;
                list::deinterleaveMinLength(AtomList::ones(5), l);
                REQUIRE(l.empty());

                l.push_back(AtomList());
                list::deinterleaveMinLength(AtomList::ones(3), l);
                REQUIRE(l.size() == 1);
                REQUIRE(l.front() == AtomList::ones(3));

                l.clear();
                l.push_back(AtomList());
                l.push_back(AtomList());
                list::deinterleaveMinLength(AtomList::ones(3), l);
                REQUIRE(l.size() == 2);
                REQUIRE(l.at(0) == AtomList::ones(2));
                REQUIRE(l.at(1) == AtomList::ones(1));

                l.clear();
                l.push_back(AtomList());
                l.push_back(AtomList());
                l.push_back(AtomList());
                list::deinterleaveMinLength(AtomList::ones(3), l);
                REQUIRE(l.size() == 3);
                REQUIRE(l.at(0) == AtomList::ones(1));
                REQUIRE(l.at(1) == AtomList::ones(1));
                REQUIRE(l.at(2) == AtomList::ones(1));
            }

            SECTION("pad")
            {
                std::vector<AtomList> l;
                list::deinterleavePadWith(AtomList::ones(5), Atom(100.f), l);
                REQUIRE(l.empty());

                l.push_back(AtomList());
                list::deinterleavePadWith(AtomList::ones(3), Atom(100.f), l);
                REQUIRE(l.size() == 1);
                REQUIRE(l.front() == AtomList::ones(3));

                l.clear();
                l.push_back(AtomList());
                l.push_back(AtomList());
                list::deinterleavePadWith(AtomList::values(3, 1.0, 2.0, 3.0), Atom(200.f), l);
                REQUIRE(l.size() == 2);
                REQUIRE(l.at(0) == AtomList::values(2, 1.0, 3.0));
                REQUIRE(l.at(1) == AtomList::values(2, 2.0, 200.));

                l.clear();
                l.push_back(AtomList());
                l.push_back(AtomList());
                l.push_back(AtomList());
                list::deinterleavePadWith(AtomList::values(3, 1.0, 2.0, 3.0), Atom(200.f), l);
                REQUIRE(l.size() == 3);
                REQUIRE(l.at(0) == AtomList::values(1, 1.0));
                REQUIRE(l.at(1) == AtomList::values(1, 2.0));
                REQUIRE(l.at(2) == AtomList::values(1, 3.0));

                l.clear();
                l.push_back(AtomList());
                l.push_back(AtomList());
                l.push_back(AtomList());
                l.push_back(AtomList());
                list::deinterleavePadWith(AtomList::values(3, 1.0, 2.0, 3.0), Atom(200.f), l);
                REQUIRE(l.size() == 4);
                REQUIRE(l.at(0) == AtomList::values(1, 1.0));
                REQUIRE(l.at(1) == AtomList::values(1, 2.0));
                REQUIRE(l.at(2) == AtomList::values(1, 3.0));
                REQUIRE(l.at(3) == AtomList::values(1, 200.0));
            }
        }
    }

    SECTION("average")
    {
        REQUIRE(list::average(AtomList()) == 0.f);
        REQUIRE(list::average(AtomList::ones(100.f)) == 1.f);
        REQUIRE(list::average(AtomList::zeroes(10.f)) == 0.f);
        REQUIRE(list::average(AtomList::values(5, 1.0, 2.0, 3.0, 4.0, 5.0)) == 3.f);

        AtomList l;
        l.append(gensym("a"));
        REQUIRE(list::average(l) == 0.f);
        l.append(gensym("b"));
        REQUIRE(list::average(l) == 0.f);
        l.append(1.0);
        REQUIRE(list::average(l) == 1.f);
        l.append(2.0);
        REQUIRE(list::average(l) == 1.5f);
    }

    SECTION("count repeats")
    {
        REQUIRE(list::countRepeats(AtomList()) == AtomList());
        REQUIRE(list::countRepeats(AtomList::ones(2)) == AtomList(1.f, 2.f));
        REQUIRE(list::countRepeats(AtomList::zeroes(10)) == AtomList(0.f, 10.f));
        AtomList hist = list::countRepeats(AtomList::values(4, 1.0, 2.0, 3.0, 1.0));
        std::vector<AtomList> out(2, AtomList());

        list::deinterleaveMinLength(hist, out);
        REQUIRE(out.size() == 2);
        AtomList elements = out[0];
        elements.sort();
        AtomList repeats = out[1];
        repeats.sort();
        REQUIRE(elements == AtomList::values(3, 1.0, 2.0, 3.0));
        REQUIRE(repeats == AtomList::values(3, 1.0, 1.0, 2.0));

        REQUIRE(list::countRepeats(AtomList(gensym("a"), gensym("a"))) == AtomList(gensym("a"), 2.f));
    }
}
