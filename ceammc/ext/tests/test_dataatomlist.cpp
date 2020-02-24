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
#include "ceammc_dataatomlist.h"
#include "test_base.h"

#include "catch.hpp"

using namespace ceammc;

TEST_CASE("DataAtomList", "[ceammc::DataAtomList]")
{
    SECTION("create")
    {
        SECTION("empty")
        {
            DataAtomList lst;
            REQUIRE(lst.size() == 0);
        }

        SECTION("atom")
        {
            DataAtomList lst(Atom(123));
            REQUIRE(lst.size() == 1);
            REQUIRE(lst[0].asAtom() == Atom(123));
        }

        SECTION("list")
        {
            DataAtomList lst(LF(1, 2, 3));
            REQUIRE(lst.size() == 3);
            REQUIRE(lst.toList() == LF(1, 2, 3));
        }
    }

    SECTION("set")
    {
        DataAtomList lst;
        REQUIRE(lst.size() == 0);

        lst.set(LF(10, 12));
        REQUIRE(lst.size() == 2);
        REQUIRE(lst[0].asAtom() == Atom(10));
        REQUIRE(lst[1].asAtom() == Atom(12));
        REQUIRE(lst.toList() == LF(10, 12));
    }

    SECTION("append")
    {
        DataAtomList lst;
        REQUIRE(lst.size() == 0);
        lst.append(Atom(23));
        REQUIRE(lst.size() == 1);

        lst.append(Atom(24));
        REQUIRE(lst.size() == 2);
        REQUIRE(lst[1].asAtom() == Atom(24));
    }

    SECTION("search")
    {
        DataAtomList lst;
        lst.append(1);
        lst.append(2);
        lst.append(3);
        lst.append(gensym("a"));
        lst.append(new IntData(100));
        lst.append(new StrData("A"));

        REQUIRE(lst.search(Atom()) == -1);
        REQUIRE(lst.search(Atom(gensym("???"))) == -1);
        REQUIRE(lst.search(Atom(0.f)) == -1);
        REQUIRE(lst.search(Atom(1)) == 0);
        REQUIRE(lst.search(Atom(2)) == 1);
        REQUIRE(lst.search(Atom(3)) == 2);
        REQUIRE(lst.search(Atom(100)) == -1);
        REQUIRE(lst.search(Atom(gensym("a"))) == 3);
        REQUIRE(lst.search(new IntData(100)) == 4);
        REQUIRE(lst.search(new IntData(99)) == -1);
        REQUIRE(lst.search(new StrData("A")) == 5);
        REQUIRE(lst.search(new StrData("B")) == -1);

        REQUIRE(lst.search(LF(2, 3)) == 1);
        REQUIRE(lst.search(LA(3, "a")) == 2);
        REQUIRE(lst.search(L()) == -1);

        lst.clear();
        REQUIRE(lst.search(L()) == -1);
        lst.append(10);
        lst.append(20);

        REQUIRE(lst.search(LF(10, 20)) == 0);

        // 10, 20, 30, 40, 30, 20, 10, 20
        lst.append(30);
        lst.append(40);
        lst.append(30);
        lst.append(20);
        lst.append(10);
        lst.append(20);

        // start
        REQUIRE(lst.search(Atom(10), 0) == 0);
        REQUIRE(lst.search(Atom(10), 1) == 6);
        REQUIRE(lst.search(Atom(10), 2) == 6);
        REQUIRE(lst.search(Atom(10), 3) == 6);
        REQUIRE(lst.search(Atom(10), 4) == 6);
        REQUIRE(lst.search(Atom(10), 5) == 6);
        REQUIRE(lst.search(Atom(10), 6) == 6);
        REQUIRE(lst.search(Atom(10), 7) == -1);
        REQUIRE(lst.search(Atom(10), 8) == -1);

        REQUIRE(lst.search(Atom(20), 0) == 1);
        REQUIRE(lst.search(Atom(20), 1) == 1);
        REQUIRE(lst.search(Atom(20), 2) == 5);
        REQUIRE(lst.search(Atom(20), 3) == 5);
        REQUIRE(lst.search(Atom(20), 4) == 5);
        REQUIRE(lst.search(Atom(20), 5) == 5);
        REQUIRE(lst.search(Atom(20), 6) == 7);
        REQUIRE(lst.search(Atom(20), 7) == 7);
        REQUIRE(lst.search(Atom(20), 8) == -1);

        REQUIRE(lst.search(Atom(30), 0) == 2);
        REQUIRE(lst.search(Atom(30), 1) == 2);
        REQUIRE(lst.search(Atom(30), 2) == 2);
        REQUIRE(lst.search(Atom(30), 3) == 4);
        REQUIRE(lst.search(Atom(30), 4) == 4);
        REQUIRE(lst.search(Atom(30), 5) == -1);
        REQUIRE(lst.search(Atom(30), 6) == -1);
        REQUIRE(lst.search(Atom(30), 7) == -1);
        REQUIRE(lst.search(Atom(30), 8) == -1);

        REQUIRE(lst.search(Atom(40), 0) == 3);
        REQUIRE(lst.search(Atom(40), 1) == 3);
        REQUIRE(lst.search(Atom(40), 2) == 3);
        REQUIRE(lst.search(Atom(40), 3) == 3);
        REQUIRE(lst.search(Atom(40), 4) == -1);
        REQUIRE(lst.search(Atom(40), 5) == -1);
        REQUIRE(lst.search(Atom(40), 6) == -1);
        REQUIRE(lst.search(Atom(40), 7) == -1);
        REQUIRE(lst.search(Atom(40), 8) == -1);

        REQUIRE(lst.search(LF(10, 20), 0) == 0);
        REQUIRE(lst.search(LF(10, 20), 1) == 6);
        REQUIRE(lst.search(LF(10, 20), 2) == 6);
        REQUIRE(lst.search(LF(10, 20), 3) == 6);
        REQUIRE(lst.search(LF(10, 20), 4) == 6);
        REQUIRE(lst.search(LF(10, 20), 5) == 6);
        REQUIRE(lst.search(LF(10, 20), 6) == 6);
        REQUIRE(lst.search(LF(10, 20), 7) == -1);
        REQUIRE(lst.search(LF(10, 20), 8) == -1);

        REQUIRE(lst.search(LF(20, 30), 0) == 1);
        REQUIRE(lst.search(LF(20, 30), 1) == 1);
        REQUIRE(lst.search(LF(20, 30), 2) == -1);
        REQUIRE(lst.search(LF(20, 30), 3) == -1);
        REQUIRE(lst.search(LF(20, 30), 4) == -1);
        REQUIRE(lst.search(LF(20, 30), 5) == -1);
        REQUIRE(lst.search(LF(20, 30), 6) == -1);
        REQUIRE(lst.search(LF(20, 30), 7) == -1);
        REQUIRE(lst.search(LF(20, 30), 8) == -1);

        REQUIRE(lst.search(LF(30, 40), 0) == 2);
        REQUIRE(lst.search(LF(30, 40), 1) == 2);
        REQUIRE(lst.search(LF(30, 40), 2) == 2);
        REQUIRE(lst.search(LF(30, 40), 3) == -1);
        REQUIRE(lst.search(LF(30, 40), 4) == -1);
        REQUIRE(lst.search(LF(30, 40), 5) == -1);
        REQUIRE(lst.search(LF(30, 40), 6) == -1);
        REQUIRE(lst.search(LF(30, 40), 7) == -1);
        REQUIRE(lst.search(LF(30, 40), 8) == -1);

        // end is too big
        REQUIRE(lst.search(Atom(10), 0, 10000) == 0);
        REQUIRE(lst.search(Atom(10), 1, 10000) == 6);
        REQUIRE(lst.search(Atom(10), 2, 10000) == 6);
        REQUIRE(lst.search(Atom(10), 3, 10000) == 6);
        REQUIRE(lst.search(Atom(10), 4, 10000) == 6);
        REQUIRE(lst.search(Atom(10), 5, 10000) == 6);
        REQUIRE(lst.search(Atom(10), 6, 10000) == 6);
        REQUIRE(lst.search(Atom(10), 7, 10000) == -1);
        REQUIRE(lst.search(Atom(10), 8, 10000) == -1);

        REQUIRE(lst.search(LF(10, 20), 0, 10000) == 0);
        REQUIRE(lst.search(LF(10, 20), 1, 10000) == 6);
        REQUIRE(lst.search(LF(10, 20), 2, 10000) == 6);
        REQUIRE(lst.search(LF(10, 20), 3, 10000) == 6);
        REQUIRE(lst.search(LF(10, 20), 4, 10000) == 6);
        REQUIRE(lst.search(LF(10, 20), 5, 10000) == 6);
        REQUIRE(lst.search(LF(10, 20), 6, 10000) == 6);
        REQUIRE(lst.search(LF(10, 20), 7, 10000) == -1);
        REQUIRE(lst.search(LF(10, 20), 8, 10000) == -1);

        // start == end
        REQUIRE(lst.search(Atom(10), 0, 0) == -1);
        REQUIRE(lst.search(Atom(10), 1, 1) == -1);
        REQUIRE(lst.search(Atom(10), 2, 2) == -1);
        REQUIRE(lst.search(Atom(10), 3, 3) == -1);
        REQUIRE(lst.search(Atom(10), 4, 4) == -1);
        REQUIRE(lst.search(Atom(10), 5, 5) == -1);
        REQUIRE(lst.search(Atom(10), 6, 6) == -1);
        REQUIRE(lst.search(Atom(10), 7, 7) == -1);

        REQUIRE(lst.search(LF(10, 20), 0, 0) == -1);
        REQUIRE(lst.search(LF(10, 20), 1, 1) == -1);
        REQUIRE(lst.search(LF(10, 20), 2, 2) == -1);
        REQUIRE(lst.search(LF(10, 20), 3, 3) == -1);
        REQUIRE(lst.search(LF(10, 20), 4, 4) == -1);
        REQUIRE(lst.search(LF(10, 20), 5, 5) == -1);
        REQUIRE(lst.search(LF(10, 20), 6, 6) == -1);
        REQUIRE(lst.search(LF(10, 20), 7, 7) == -1);
        REQUIRE(lst.search(LF(10, 20), 8, 8) == -1);

        // start > end
        REQUIRE(lst.search(Atom(10), 1, 0) == -1);
        REQUIRE(lst.search(Atom(10), 2, 1) == -1);
        REQUIRE(lst.search(Atom(10), 3, 2) == -1);
        REQUIRE(lst.search(Atom(10), 4, 3) == -1);
        REQUIRE(lst.search(Atom(10), 5, 4) == -1);
        REQUIRE(lst.search(Atom(10), 6, 5) == -1);
        REQUIRE(lst.search(Atom(10), 7, 6) == -1);
        REQUIRE(lst.search(Atom(10), 8, 7) == -1);

        REQUIRE(lst.search(LF(10, 20), 1, 0) == -1);
        REQUIRE(lst.search(LF(10, 20), 2, 1) == -1);
        REQUIRE(lst.search(LF(10, 20), 3, 2) == -1);
        REQUIRE(lst.search(LF(10, 20), 4, 3) == -1);
        REQUIRE(lst.search(LF(10, 20), 5, 4) == -1);
        REQUIRE(lst.search(LF(10, 20), 6, 5) == -1);
        REQUIRE(lst.search(LF(10, 20), 7, 6) == -1);
        REQUIRE(lst.search(LF(10, 20), 8, 7) == -1);
        REQUIRE(lst.search(LF(10, 20), 9, 8) == -1);

        // start < end < last
        // 10, 20, 30, 40, 30, 20, 10, 20
        REQUIRE(lst.search(Atom(10), 0, 1) == 0);
        REQUIRE(lst.search(Atom(10), 1, 1) == -1);
        REQUIRE(lst.search(Atom(10), 2, 1) == -1);
        REQUIRE(lst.search(Atom(10), 3, 1) == -1);
        REQUIRE(lst.search(Atom(10), 4, 1) == -1);
        REQUIRE(lst.search(Atom(10), 5, 1) == -1);
        REQUIRE(lst.search(Atom(10), 6, 1) == -1);
        REQUIRE(lst.search(Atom(10), 7, 1) == -1);
        REQUIRE(lst.search(Atom(10), 8, 1) == -1);

        REQUIRE(lst.search(Atom(10), 0, 2) == 0);
        REQUIRE(lst.search(Atom(10), 1, 2) == -1);
        REQUIRE(lst.search(Atom(10), 2, 2) == -1);
        REQUIRE(lst.search(Atom(10), 3, 2) == -1);
        REQUIRE(lst.search(Atom(10), 4, 2) == -1);
        REQUIRE(lst.search(Atom(10), 5, 2) == -1);
        REQUIRE(lst.search(Atom(10), 6, 2) == -1);
        REQUIRE(lst.search(Atom(10), 7, 2) == -1);
        REQUIRE(lst.search(Atom(10), 8, 2) == -1);

        REQUIRE(lst.search(Atom(30), 0, 5) == 2);
        REQUIRE(lst.search(Atom(30), 1, 5) == 2);
        REQUIRE(lst.search(Atom(30), 2, 5) == 2);
        REQUIRE(lst.search(Atom(30), 3, 5) == 4);
        REQUIRE(lst.search(Atom(30), 4, 5) == 4);
        REQUIRE(lst.search(Atom(30), 5, 5) == -1);
        REQUIRE(lst.search(Atom(30), 6, 5) == -1);
        REQUIRE(lst.search(Atom(30), 7, 5) == -1);
        REQUIRE(lst.search(Atom(30), 8, 5) == -1);

        REQUIRE(lst.search(LF(10, 20), 0, 1) == -1);
        REQUIRE(lst.search(LF(10, 20), 1, 1) == -1);
        REQUIRE(lst.search(LF(10, 20), 2, 1) == -1);
        REQUIRE(lst.search(LF(10, 20), 3, 1) == -1);
        REQUIRE(lst.search(LF(10, 20), 4, 1) == -1);
        REQUIRE(lst.search(LF(10, 20), 5, 1) == -1);
        REQUIRE(lst.search(LF(10, 20), 6, 1) == -1);
        REQUIRE(lst.search(LF(10, 20), 7, 1) == -1);
        REQUIRE(lst.search(LF(10, 20), 8, 1) == -1);

        REQUIRE(lst.search(LF(10, 20), 0, 2) == 0);
        REQUIRE(lst.search(LF(10, 20), 1, 2) == -1);
        REQUIRE(lst.search(LF(10, 20), 2, 2) == -1);
        REQUIRE(lst.search(LF(10, 20), 3, 2) == -1);
        REQUIRE(lst.search(LF(10, 20), 4, 2) == -1);
        REQUIRE(lst.search(LF(10, 20), 5, 2) == -1);
        REQUIRE(lst.search(LF(10, 20), 6, 2) == -1);
        REQUIRE(lst.search(LF(10, 20), 7, 2) == -1);
        REQUIRE(lst.search(LF(10, 20), 8, 2) == -1);

        REQUIRE(lst.search(LF(10, 20), 0, 4) == 0);
        REQUIRE(lst.search(LF(10, 20), 1, 4) == -1);
        REQUIRE(lst.search(LF(10, 20), 2, 4) == -1);
        REQUIRE(lst.search(LF(10, 20), 3, 4) == -1);
        REQUIRE(lst.search(LF(10, 20), 4, 4) == -1);
        REQUIRE(lst.search(LF(10, 20), 5, 4) == -1);
        REQUIRE(lst.search(LF(10, 20), 6, 4) == -1);
        REQUIRE(lst.search(LF(10, 20), 7, 4) == -1);
        REQUIRE(lst.search(LF(10, 20), 8, 4) == -1);

        REQUIRE(lst.search(LF(10, 20), 0, 8) == 0);
        REQUIRE(lst.search(LF(10, 20), 1, 8) == 6);
        REQUIRE(lst.search(LF(10, 20), 2, 8) == 6);
        REQUIRE(lst.search(LF(10, 20), 3, 8) == 6);
        REQUIRE(lst.search(LF(10, 20), 4, 8) == 6);
        REQUIRE(lst.search(LF(10, 20), 5, 8) == 6);
        REQUIRE(lst.search(LF(10, 20), 6, 8) == 6);
        REQUIRE(lst.search(LF(10, 20), 7, 8) == -1);
        REQUIRE(lst.search(LF(10, 20), 8, 8) == -1);
    }

    SECTION("contains")
    {
        DataAtomList lst;
        REQUIRE_FALSE(lst.contains(Atom(10)));
        REQUIRE_FALSE(lst.contains(DataAtom(gensym("sdf"))));
        REQUIRE_FALSE(lst.contains(Atom()));
        REQUIRE_FALSE(lst.contains(L()));

        REQUIRE_FALSE(lst.contains(new IntData(123)));

        lst.append(100);
        REQUIRE_FALSE(lst.contains(Atom(10)));
        REQUIRE_FALSE(lst.contains(DataAtom(gensym("sdf"))));
        REQUIRE_FALSE(lst.contains(new IntData(123)));

        REQUIRE(lst.contains(Atom(100)));
        REQUIRE_FALSE(lst.contains(DataAtom(gensym("100"))));
        REQUIRE_FALSE(lst.contains(new IntData(100)));

        lst.append(new IntData(444));
        REQUIRE(lst.contains(new IntData(444)));
        REQUIRE_FALSE(lst.contains(Atom(444)));

        DataAtom int444(new IntData(444));
        REQUIRE(lst.contains(int444));

        REQUIRE(lst.contains(LA(100, int444.asAtom())));

        // 100, INT(444)
        lst.append(Atom(gensym("a")));
        lst.append(Atom(gensym("b")));
        lst.append(Atom(gensym("c")));

        REQUIRE(lst.contains(Atom(gensym("b"))));
        REQUIRE(lst.contains(LA("a", "b")));
        REQUIRE(lst.contains(LA("b", "c")));
        REQUIRE_FALSE(lst.contains(LA("a", "c")));
        REQUIRE_FALSE(lst.contains(L()));
    }

    SECTION("isSingleData")
    {
        REQUIRE_FALSE(DataAtomList().isSingleData());
        REQUIRE_FALSE(DataAtomList().isSingleDataType(IntData::dataType));
        REQUIRE_FALSE(DataAtomList().isSingleDataType<StrData>());

        REQUIRE(DataAtomList({ DataPtr(new IntData(200)) }).isSingleData());
        REQUIRE(DataAtomList({ DataPtr(new IntData(200)) }).isSingleDataType(IntData::dataType));
        REQUIRE_FALSE(DataAtomList({ DataPtr(new IntData(200)) }).isSingleDataType<StrData>());

        REQUIRE_FALSE(DataAtomList({ DataPtr(new IntData(200)), DataPtr(new IntData(200)) }).isSingleData());
        REQUIRE_FALSE(DataAtomList(LF(1)).isSingleData());
        REQUIRE_FALSE(DataAtomList(LF(1, 2, 3)).isSingleData());
    }

    SECTION("as")
    {
        REQUIRE(DataAtomList().asSingle<IntData>() == nullptr);
        REQUIRE(DataAtomList({ DataPtr(new IntData(200)) }).asSingle<StrData>() == nullptr);
        REQUIRE(DataAtomList({ DataPtr(new IntData(200)) }).asSingle<IntData>() != nullptr);
        REQUIRE(DataAtomList({ DataPtr(new IntData(200)) }).asSingle<IntData>()->value() == 200);
        REQUIRE(DataAtomList({ DataPtr(new IntData(200)), DataPtr(new IntData(200)) }).asSingle<IntData>() == nullptr);
    }
}
