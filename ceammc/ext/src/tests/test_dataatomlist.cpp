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
#include "base_extension_test.h"
#include "ceammc_dataatomlist.h"

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
            REQUIRE(lst[0].toAtom() == Atom(123));
        }

        SECTION("list")
        {
            DataAtomList lst(L3(1, 2, 3));
            REQUIRE(lst.size() == 3);
            REQUIRE(lst.toList() == L3(1, 2, 3));
        }
    }

    SECTION("set")
    {
        DataAtomList lst;
        REQUIRE(lst.size() == 0);

        lst.set(L2(10, 12));
        REQUIRE(lst.size() == 2);
        REQUIRE(lst[0].toAtom() == Atom(10));
        REQUIRE(lst[1].toAtom() == Atom(12));
        REQUIRE(lst.toList() == L2(10, 12));
    }

    SECTION("append")
    {
        DataAtomList lst;
        REQUIRE(lst.size() == 0);
        lst.append(Atom(23));
        REQUIRE(lst.size() == 1);

        lst.append(Atom(24));
        REQUIRE(lst.size() == 2);
        REQUIRE(lst[1].toAtom() == Atom(24));
    }

    SECTION("index")
    {
        DataAtomList lst;
        lst.append(123);

        const DataAtomList& l = lst;
        REQUIRE(l[0].toAtom() == A(123));
    }
}
