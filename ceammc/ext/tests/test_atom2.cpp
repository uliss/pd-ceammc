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
#include "ceammc_atom.h"

#include "catch.hpp"

using namespace ceammc;

TEST_CASE("Atom2", "[ceammc::Atom]")
{
    SECTION("data")
    {
        REQUIRE(Atom(DataDesc(1, 2)).isData());

        REQUIRE_FALSE(Atom().isData());
        REQUIRE_FALSE(Atom(0.f).isData());
        REQUIRE_FALSE(Atom(S("test")).isData());
        REQUIRE_FALSE(Atom().isDataType(1));
        REQUIRE_FALSE(Atom(1).isDataType(1));
        REQUIRE(Atom().getData().id == 0);
        REQUIRE(Atom().getData().type == 0);

        Atom a;
        a.setData(DataDesc(10, 12));
        REQUIRE(a.isData());
        REQUIRE(a.dataId() == 12);
        REQUIRE(a.dataType() == 10);
        REQUIRE(a.getData().id == 12);
        REQUIRE(a.getData().type == 10);
        REQUIRE(a.isDataType(10));
        REQUIRE_FALSE(a.isDataType(11));

        Atom b(a);
        REQUIRE(b.isData());

        REQUIRE(a == b);

        Atom c = b;
        REQUIRE(c.isData());
    }

    SECTION("predicate")
    {
        AtomList lst;
        lst.append(Atom(1));
        lst.append(Atom(2));
        lst.append(Atom(3));

        REQUIRE(lst.filtered(isData).empty());

        lst[1].setData(DataDesc(10, 12));
        REQUIRE(lst.filtered(isData).size() == 1);
    }

    SECTION("compare")
    {
        REQUIRE(D(1, 1) == D(1, 1));
        REQUIRE(D(1, 1) != D(2, 1));
        REQUIRE(D(1, 1) != D(1, 2));
        REQUIRE(D(1, 2) != D(3, 4));
    }
}
