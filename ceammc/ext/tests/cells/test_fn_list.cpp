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
#include "ceammc_fn_list.h"
#include "test_common.h"
#include <boost/optional/optional_io.hpp>

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
            lst.push_back(L());
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
            lst.push_back(L());
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

            l.push_back(L());
            REQUIRE(list::minmaxListSize(l) == Pair(0, 10));
        }

        SECTION("interleave")
        {
            SECTION("min length")
            {
                std::vector<AtomList> lst;
                AtomList res;
                res = list::interleaveMinLength(lst);
                REQUIRE(res == L());

                lst.push_back(AtomList::ones(3));
                res = list::interleaveMinLength(lst);
                REQUIRE(res == AtomList::ones(3));

                lst.push_back(AtomList::zeroes(5));
                res = list::interleaveMinLength(lst);
                REQUIRE(res.size() == 6);
                REQUIRE(res == LF(1., 0., 1., 0., 1., 0.));

                lst.push_back(L());
                res = list::interleaveMinLength(lst);
                REQUIRE(res == L());
            }

            SECTION("pad length")
            {
                std::vector<AtomList> l;
                REQUIRE(list::interleavePadWith(l, Atom(1.f)) == L());

                l.push_back(AtomList::ones(3));
                REQUIRE(list::interleavePadWith(l, Atom(2.f)) == AtomList::ones(3));

                l.push_back(AtomList::zeroes(1));
                REQUIRE(list::interleavePadWith(l, Atom(2.f)) == LF(1., 0., 1., 2., 1., 2.));

                l.clear();
                l.push_back(L());
                REQUIRE(list::interleavePadWith(l, Atom(1.f)) == L());
            }

            SECTION("clip length")
            {
                std::vector<AtomList> l;
                REQUIRE(list::interleaveClip(l) == L());

                l.push_back(L());
                REQUIRE(list::interleaveClip(l) == L());

                l.push_back(AtomList::ones(3));
                REQUIRE(list::interleaveClip(l) == L());

                l.clear();
                l.push_back(LF(1.0, 2.0));
                l.push_back(LF(-1.0, -2.0, -3.0, -4.0));
                REQUIRE(list::interleaveClip(l) == LF(1.0, -1.0, 2.0, -2.0, 2.0, -3.0, 2.0, -4.0));
            }

            SECTION("wrap length")
            {
                std::vector<AtomList> l;
                REQUIRE(list::interleaveWrap(l) == L());

                l.push_back(L());
                REQUIRE(list::interleaveWrap(l) == L());

                l.push_back(AtomList::ones(3));
                REQUIRE(list::interleaveWrap(l) == L());

                l.clear();
                l.push_back(LF(1.0, 2.0));
                l.push_back(LF(-1.0, -2.0, -3.0, -4.0));
                REQUIRE(list::interleaveWrap(l) == LF(1.0, -1.0, 2.0, -2.0, 1.0, -3.0, 2.0, -4.0));
            }

            SECTION("fold length")
            {
                std::vector<AtomList> l;
                REQUIRE(list::interleaveFold(l) == L());

                l.push_back(L());
                REQUIRE(list::interleaveFold(l) == L());

                l.push_back(AtomList::ones(3));
                REQUIRE(list::interleaveFold(l) == L());

                l.clear();
                l.push_back(LF(1.0, 2.0, 3.0));
                l.push_back(LF(-1.0, -2.0, -3.0, -4.0));
                REQUIRE(list::interleaveFold(l) == LF(1.0, -1.0, 2.0, -2.0, 3.0, -3.0, 2.0, -4.0));
            }
        }

        SECTION("deinterleave")
        {
            SECTION("min length")
            {
                std::vector<AtomList> l;
                list::deinterleaveMinLength(AtomList::ones(5), l);
                REQUIRE(l.empty());

                l.push_back(L());
                list::deinterleaveMinLength(AtomList::ones(3), l);
                REQUIRE(l.size() == 1);
                REQUIRE(l.front() == AtomList::ones(3));

                l.clear();
                l.push_back(L());
                l.push_back(L());
                list::deinterleaveMinLength(AtomList::ones(3), l);
                REQUIRE(l.size() == 2);
                REQUIRE(l.at(0) == AtomList::ones(2));
                REQUIRE(l.at(1) == AtomList::ones(1));

                l.clear();
                l.push_back(L());
                l.push_back(L());
                l.push_back(L());
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

                l.push_back(L());
                list::deinterleavePadWith(AtomList::ones(3), Atom(100.f), l);
                REQUIRE(l.size() == 1);
                REQUIRE(l.front() == AtomList::ones(3));

                l.clear();
                l.push_back(L());
                l.push_back(L());
                list::deinterleavePadWith(LF(1.0, 2.0, 3.0), Atom(200.f), l);
                REQUIRE(l.size() == 2);
                REQUIRE(l.at(0) == LF(1.0, 3.0));
                REQUIRE(l.at(1) == LF(2.0, 200.));

                l.clear();
                l.push_back(L());
                l.push_back(L());
                l.push_back(L());
                list::deinterleavePadWith(LF(1.0, 2.0, 3.0), Atom(200.f), l);
                REQUIRE(l.size() == 3);
                REQUIRE(l.at(0) == LF(1.0));
                REQUIRE(l.at(1) == LF(2.0));
                REQUIRE(l.at(2) == LF(3.0));

                l.clear();
                l.push_back(L());
                l.push_back(L());
                l.push_back(L());
                l.push_back(L());
                list::deinterleavePadWith(LF(1.0, 2.0, 3.0), Atom(200.f), l);
                REQUIRE(l.size() == 4);
                REQUIRE(l.at(0) == LF(1.0));
                REQUIRE(l.at(1) == LF(2.0));
                REQUIRE(l.at(2) == LF(3.0));
                REQUIRE(l.at(3) == LF(200.0));
            }
        }
    }

    SECTION("average")
    {
        REQUIRE(list::average(L()) == boost::none);
        REQUIRE(list::average(AtomList::ones(100.f)) == MaybeFloat(1));
        REQUIRE(list::average(AtomList::zeroes(10.f)) == MaybeFloat(0));
        REQUIRE(list::average(LF(1.0, 2.0, 3.0, 4.0, 5.0)) == MaybeFloat(3));

        AtomList l;
        l.append(gensym("a"));
        REQUIRE(list::average(l) == boost::none);
        l.append(gensym("b"));
        REQUIRE(list::average(l) == boost::none);
        l.append(1.0);
        REQUIRE(list::average(l) == MaybeFloat(1));
        l.append(2.0);
        REQUIRE(list::average(l) == MaybeFloat(1.5));
    }

    SECTION("count repeats")
    {
        REQUIRE(list::countRepeats(L()) == L());
        REQUIRE(list::countRepeats(AtomList::ones(2)) == AtomList(1.f, 2.f));
        REQUIRE(list::countRepeats(AtomList::zeroes(10)) == AtomList(0.f, 10.f));
        AtomList hist = list::countRepeats(LF(1.0, 2.0, 3.0, 1.0));
        std::vector<AtomList> out(2, L());

        list::deinterleaveMinLength(hist, out);
        REQUIRE(out.size() == 2);
        AtomList elements = out[0];
        elements.sort();
        AtomList repeats = out[1];
        repeats.sort();
        REQUIRE(elements == LF(1.0, 2.0, 3.0));
        REQUIRE(repeats == LF(1.0, 1.0, 2.0));

        REQUIRE(list::countRepeats(LA("a", "a")) == LA("a", 2.f));

        // normalize
        REQUIRE(list::countRepeats(LF(1, 2, 1, 3), true) == LX(1, 0.5, 2, 0.25, 3, 0.25));
        REQUIRE(list::countRepeats(LF(1, 2, 1, 3), false) == LX(1, 2, 2, 1, 3, 1));
        REQUIRE(list::countRepeats(L(), true) == L());
    }

    SECTION("calc clip index")
    {
        REQUIRE_FALSE(list::calcClipIndex(10, 4, 0));
        REQUIRE_FALSE(list::calcClipIndex(10, 0, 0));

        size_t idx = 100;
        REQUIRE_FALSE(list::calcClipIndex(10, 0, &idx));
        REQUIRE(list::calcClipIndex(10, 1, &idx));
        REQUIRE(idx == 0);
        idx = 100;
        REQUIRE(list::calcClipIndex(-1, 1, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcClipIndex(-1, 2, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcClipIndex(0, 2, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcClipIndex(1, 2, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcClipIndex(2, 2, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcClipIndex(2, 3, &idx));
        REQUIRE(idx == 2);
    }

    SECTION("calc wrap index")
    {
        REQUIRE_FALSE(list::calcWrapIndex(10, 4, 0));
        REQUIRE_FALSE(list::calcWrapIndex(10, 0, 0));

        size_t idx = 100;
        REQUIRE_FALSE(list::calcWrapIndex(10, 0, &idx));
        REQUIRE(list::calcWrapIndex(10, 1, &idx));
        REQUIRE(idx == 0);
        idx = 100;
        REQUIRE(list::calcWrapIndex(0, 3, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcWrapIndex(1, 3, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcWrapIndex(2, 3, &idx));
        REQUIRE(idx == 2);
        REQUIRE(list::calcWrapIndex(3, 3, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcWrapIndex(4, 3, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcWrapIndex(5, 3, &idx));
        REQUIRE(idx == 2);
        REQUIRE(list::calcWrapIndex(-1, 3, &idx));
        REQUIRE(idx == 2);
        REQUIRE(list::calcWrapIndex(-2, 3, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcWrapIndex(-3, 3, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcWrapIndex(-4, 3, &idx));
        REQUIRE(idx == 2);
    }

    SECTION("calc fold index")
    {
        REQUIRE_FALSE(list::calcFoldIndex(10, 4, 0));
        REQUIRE_FALSE(list::calcFoldIndex(10, 0, 0));

        size_t idx = 100;
        REQUIRE_FALSE(list::calcFoldIndex(10, 0, &idx));
        REQUIRE(list::calcFoldIndex(10, 1, &idx));
        REQUIRE(idx == 0);
        idx = 100;

        // positive odd
        REQUIRE(list::calcFoldIndex(0, 3, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcFoldIndex(1, 3, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcFoldIndex(2, 3, &idx));
        REQUIRE(idx == 2);
        REQUIRE(list::calcFoldIndex(3, 3, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcFoldIndex(4, 3, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcFoldIndex(5, 3, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcFoldIndex(6, 3, &idx));

        // negative odd
        REQUIRE(list::calcFoldIndex(0, 3, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcFoldIndex(-1, 3, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcFoldIndex(-2, 3, &idx));
        REQUIRE(idx == 2);
        REQUIRE(list::calcFoldIndex(-3, 3, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcFoldIndex(-4, 3, &idx));
        REQUIRE(idx == 0);

        // positive even
        REQUIRE(list::calcFoldIndex(0, 2, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcFoldIndex(1, 2, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcFoldIndex(2, 2, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcFoldIndex(3, 2, &idx));
        REQUIRE(idx == 1);

        // negative even
        REQUIRE(list::calcFoldIndex(0, 2, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcFoldIndex(-1, 2, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcFoldIndex(-2, 2, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcFoldIndex(-3, 2, &idx));
        REQUIRE(idx == 1);

        // positive even
        REQUIRE(list::calcFoldIndex(0, 4, &idx));
        REQUIRE(idx == 0);
        REQUIRE(list::calcFoldIndex(1, 4, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcFoldIndex(2, 4, &idx));
        REQUIRE(idx == 2);
        REQUIRE(list::calcFoldIndex(3, 4, &idx));
        REQUIRE(idx == 3);
        REQUIRE(list::calcFoldIndex(4, 4, &idx));
        REQUIRE(idx == 2);
        REQUIRE(list::calcFoldIndex(5, 4, &idx));
        REQUIRE(idx == 1);
        REQUIRE(list::calcFoldIndex(6, 4, &idx));
        REQUIRE(idx == 0);
    }

    SECTION("slice wrap")
    {
        REQUIRE(list::sliceWrap(L(), 10, 20) == L());

        REQUIRE(list::sliceWrap(AtomList(1, 2), 0, 0) == L());
        REQUIRE(list::sliceWrap(AtomList(1, 2), 0, 1) == LF(1));
        REQUIRE(list::sliceWrap(AtomList(1, 2), 0, 2) == AtomList(1, 2));

        REQUIRE(list::sliceWrap(AtomList(1, 2), 1, 0) == L());
        REQUIRE(list::sliceWrap(AtomList(1, 2), 1, 1) == LF(2));
        REQUIRE(list::sliceWrap(AtomList(1, 2), 1, 2) == AtomList(2, 1));

        REQUIRE(list::sliceWrap(AtomList(1, 2), 2, 0) == L());
        REQUIRE(list::sliceWrap(AtomList(1, 2), 2, 1) == LF(1));
        REQUIRE(list::sliceWrap(AtomList(1, 2), 2, 2) == AtomList(1, 2));

        SECTION("slice wrap negative")
        {
            REQUIRE(list::sliceWrap(AtomList(1, 2), -1, 0) == L());
            REQUIRE(list::sliceWrap(AtomList(1, 2), -1, 1) == LF(2));
            REQUIRE(list::sliceWrap(AtomList(1, 2), -2, 1) == LF(1));
            REQUIRE(list::sliceWrap(AtomList(1, 2), -1, 2) == AtomList(2, 1));
            REQUIRE(list::sliceWrap(AtomList(1, 2), -1, 5) == LF(2.0, 1.0, 2.0, 1.0, 2.0));
        }
    }

    SECTION("slice clip")
    {
        REQUIRE(list::sliceClip(L(), 10, 20) == L());

        SECTION("positive")
        {
            const AtomList LST = LF(1.0, 2.0, 3.0);

            REQUIRE(list::sliceClip(LST, 0, 0) == L());
            REQUIRE(list::sliceClip(LST, 0, 1) == LF(1));
            REQUIRE(list::sliceClip(LST, 0, 2) == AtomList(1, 2));

            REQUIRE(list::sliceClip(LST, 1, 0) == L());
            REQUIRE(list::sliceClip(LST, 1, 1) == LF(2));
            REQUIRE(list::sliceClip(LST, 1, 2) == AtomList(2, 3));

            REQUIRE(list::sliceClip(LST, 2, 0) == L());
            REQUIRE(list::sliceClip(LST, 2, 1) == LF(3));
            REQUIRE(list::sliceClip(LST, 2, 2) == AtomList(3, 3));

            REQUIRE(list::sliceClip(LST, 3, 0) == L());
            REQUIRE(list::sliceClip(LST, 3, 1) == LF(3));
            REQUIRE(list::sliceClip(LST, 3, 2) == AtomList(3, 3));
        }

        SECTION("negative")
        {
            const AtomList LST = LF(1.0, 2.0, 3.0);

            REQUIRE(list::sliceClip(LST, -1, 0) == L());
            REQUIRE(list::sliceClip(LST, -1, 1) == LF(1));
            REQUIRE(list::sliceClip(LST, -2, 1) == LF(1));
            REQUIRE(list::sliceClip(LST, -1, 2) == AtomList(1, 1));
            REQUIRE(list::sliceClip(LST, -2, 6) == LF(1.0, 1.0, 1.0, 2.0, 3.0, 3.0));
        }
    }

    SECTION("slice fold")
    {
        REQUIRE(list::sliceFold(L(), 10, 20) == L());

        SECTION("odd")
        {
            SECTION("positive")
            {
                const AtomList LST = LF(1.0, 2.0, 3.0);

                REQUIRE(list::sliceFold(LST, 0, 0) == L());
                REQUIRE(list::sliceFold(LST, 0, 1) == LF(1));
                REQUIRE(list::sliceFold(LST, 0, 2) == AtomList(1, 2));
                REQUIRE(list::sliceFold(LST, 0, 3) == LF(1., 2., 3.));
                REQUIRE(list::sliceFold(LST, 0, 4) == LF(1., 2., 3., 2.));
                REQUIRE(list::sliceFold(LST, 0, 5) == LF(1., 2., 3., 2., 1.));
                REQUIRE(list::sliceFold(LST, 0, 6) == LF(1., 2., 3., 2., 1., 2.));

                REQUIRE(list::sliceFold(LST, 1, 0) == L());
                REQUIRE(list::sliceFold(LST, 1, 1) == LF(2));
                REQUIRE(list::sliceFold(LST, 1, 2) == AtomList(2, 3));
                REQUIRE(list::sliceFold(LST, 1, 3) == LF(2., 3., 2.));
                REQUIRE(list::sliceFold(LST, 1, 4) == LF(2., 3., 2., 1.));
                REQUIRE(list::sliceFold(LST, 1, 5) == LF(2., 3., 2., 1., 2.));
                REQUIRE(list::sliceFold(LST, 1, 6) == LF(2., 3., 2., 1., 2., 3.));

                REQUIRE(list::sliceFold(LST, 2, 0) == L());
                REQUIRE(list::sliceFold(LST, 2, 1) == LF(3));
                REQUIRE(list::sliceFold(LST, 2, 2) == AtomList(3, 2));
                REQUIRE(list::sliceFold(LST, 2, 3) == LF(3., 2., 1.));
                REQUIRE(list::sliceFold(LST, 2, 4) == LF(3., 2., 1., 2.));
                REQUIRE(list::sliceFold(LST, 2, 5) == LF(3., 2., 1., 2., 3.));
                REQUIRE(list::sliceFold(LST, 2, 6) == LF(3., 2., 1., 2., 3., 2.));

                REQUIRE(list::sliceFold(LST, 3, 0) == L());
                REQUIRE(list::sliceFold(LST, 3, 1) == LF(2));
                REQUIRE(list::sliceFold(LST, 3, 2) == AtomList(2, 1));
                REQUIRE(list::sliceFold(LST, 3, 3) == LF(2., 1., 2.));
                REQUIRE(list::sliceFold(LST, 3, 4) == LF(2., 1., 2., 3.));
                REQUIRE(list::sliceFold(LST, 3, 5) == LF(2., 1., 2., 3., 2.));
                REQUIRE(list::sliceFold(LST, 3, 6) == LF(2., 1., 2., 3., 2., 1.));
            }

            SECTION("negative")
            {
                const AtomList LST = LF(1.0, 2.0, 3.0);

                REQUIRE(list::sliceFold(LST, -1, 0) == L());
                REQUIRE(list::sliceFold(LST, -1, 1) == LF(2));
                REQUIRE(list::sliceFold(LST, -1, 2) == AtomList(2, 1));
                REQUIRE(list::sliceFold(LST, -1, 3) == LF(2., 1., 2.));
                REQUIRE(list::sliceFold(LST, -1, 4) == LF(2., 1., 2., 3.));
                REQUIRE(list::sliceFold(LST, -1, 5) == LF(2., 1., 2., 3., 2.));
                REQUIRE(list::sliceFold(LST, -1, 6) == LF(2., 1., 2., 3., 2., 1.));

                REQUIRE(list::sliceFold(LST, -2, 0) == L());
                REQUIRE(list::sliceFold(LST, -2, 1) == LF(3));
                REQUIRE(list::sliceFold(LST, -2, 2) == AtomList(3, 2));
                REQUIRE(list::sliceFold(LST, -2, 3) == LF(3., 2., 1.));
                REQUIRE(list::sliceFold(LST, -2, 4) == LF(3., 2., 1., 2.));
                REQUIRE(list::sliceFold(LST, -2, 5) == LF(3., 2., 1., 2., 3.));
                REQUIRE(list::sliceFold(LST, -2, 6) == LF(3., 2., 1., 2., 3., 2.));
            }
        }

        SECTION("even")
        {
            SECTION("positive")
            {
                const AtomList LST = LF(1.0, 2.0);

                REQUIRE(list::sliceFold(LST, 0, 0) == L());
                REQUIRE(list::sliceFold(LST, 0, 1) == LF(1));
                REQUIRE(list::sliceFold(LST, 0, 2) == AtomList(1, 2));
                REQUIRE(list::sliceFold(LST, 0, 3) == LF(1., 2., 1.));
                REQUIRE(list::sliceFold(LST, 0, 4) == LF(1., 2., 1., 2.));
                REQUIRE(list::sliceFold(LST, 0, 5) == LF(1., 2., 1., 2., 1.));

                REQUIRE(list::sliceFold(LST, 1, 0) == L());
                REQUIRE(list::sliceFold(LST, 1, 1) == LF(2));
                REQUIRE(list::sliceFold(LST, 1, 2) == AtomList(2, 1));
                REQUIRE(list::sliceFold(LST, 1, 3) == LF(2., 1., 2.));
                REQUIRE(list::sliceFold(LST, 1, 4) == LF(2., 1., 2., 1.));
                REQUIRE(list::sliceFold(LST, 1, 5) == LF(2., 1., 2., 1., 2.));

                REQUIRE(list::sliceFold(LST, 2, 0) == L());
                REQUIRE(list::sliceFold(LST, 2, 1) == LF(1));
                REQUIRE(list::sliceFold(LST, 2, 2) == AtomList(1, 2));
                REQUIRE(list::sliceFold(LST, 2, 3) == LF(1., 2., 1.));
                REQUIRE(list::sliceFold(LST, 2, 4) == LF(1., 2., 1., 2.));
                REQUIRE(list::sliceFold(LST, 2, 5) == LF(1., 2., 1., 2., 1.));
            }

            SECTION("negative")
            {
                const AtomList LST = LF(1.0, 2.0);

                REQUIRE(list::sliceFold(LST, -1, 0) == L());
                REQUIRE(list::sliceFold(LST, -1, 1) == LF(2));
                REQUIRE(list::sliceFold(LST, -1, 2) == AtomList(2, 1));
                REQUIRE(list::sliceFold(LST, -1, 3) == LF(2., 1., 2.));
                REQUIRE(list::sliceFold(LST, -1, 4) == LF(2., 1., 2., 1.));
                REQUIRE(list::sliceFold(LST, -1, 5) == LF(2., 1., 2., 1., 2.));

                REQUIRE(list::sliceFold(LST, -2, 0) == L());
                REQUIRE(list::sliceFold(LST, -2, 1) == LF(1));
                REQUIRE(list::sliceFold(LST, -2, 2) == AtomList(1, 2));
                REQUIRE(list::sliceFold(LST, -2, 3) == LF(1., 2., 1.));
                REQUIRE(list::sliceFold(LST, -2, 4) == LF(1., 2., 1., 2.));
                REQUIRE(list::sliceFold(LST, -2, 5) == LF(1., 2., 1., 2., 1.));

                REQUIRE(list::sliceFold(LST, -3, 0) == L());
                REQUIRE(list::sliceFold(LST, -3, 1) == LF(2));
                REQUIRE(list::sliceFold(LST, -3, 2) == AtomList(2, 1));
                REQUIRE(list::sliceFold(LST, -3, 3) == LF(2., 1., 2.));
                REQUIRE(list::sliceFold(LST, -3, 4) == LF(2., 1., 2., 1.));
                REQUIRE(list::sliceFold(LST, -3, 5) == LF(2., 1., 2., 1., 2.));
            }
        }
    }

    SECTION("rotate")
    {
        REQUIRE(list::rotate(L(), 0) == L());
        REQUIRE(list::rotate(L(), 1) == L());
        REQUIRE(list::rotate(L(), -1) == L());

        REQUIRE(list::rotate(LF(100), 0) == LF(100));
        REQUIRE(list::rotate(LF(100), -1) == LF(100));
        REQUIRE(list::rotate(LF(100), 1) == LF(100));

        REQUIRE(list::rotate(LF(1, 2), 0) == LF(1, 2));
        REQUIRE(list::rotate(LF(1, 2), 1) == LF(2, 1));
        REQUIRE(list::rotate(LF(1, 2), 2) == LF(1, 2));
        REQUIRE(list::rotate(LF(1, 2), -1) == LF(2, 1));
        REQUIRE(list::rotate(LF(1, 2), -2) == LF(1, 2));

        REQUIRE(list::rotate(LF(1, 2, 3), 0) == LF(1, 2, 3));
        REQUIRE(list::rotate(LF(1, 2, 3), 1) == LF(2, 3, 1));
        REQUIRE(list::rotate(LF(1, 2, 3), 2) == LF(3, 1, 2));
        REQUIRE(list::rotate(LF(1, 2, 3), 3) == LF(1, 2, 3));
        REQUIRE(list::rotate(LF(1, 2, 3), 4) == LF(2, 3, 1));
        REQUIRE(list::rotate(LF(1, 2, 3), -1) == LF(3, 1, 2));
        REQUIRE(list::rotate(LF(1, 2, 3), -2) == LF(2, 3, 1));
        REQUIRE(list::rotate(LF(1, 2, 3), -3) == LF(1, 2, 3));
        REQUIRE(list::rotate(LF(1, 2, 3), -4) == LF(3, 1, 2));
    }

    SECTION("repeat")
    {
        REQUIRE(list::repeat(L(), 0) == L());
        REQUIRE(list::repeat(L(), 100) == L());
        REQUIRE(list::repeat(LF(1), 0) == L());
        REQUIRE(list::repeat(LF(1), 4) == LF(1, 1, 1, 1));
        REQUIRE(list::repeat(LF(1, 5), 2) == LF(1, 5, 1, 5));
        REQUIRE(list::repeat(LF(1, 2, 3), 2) == LF(1, 2, 3, 1, 2, 3));
    }
}
