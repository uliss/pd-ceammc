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
#include "ceammc_dataatom.h"
#include "ceammc_xdata.h"

#include "catch.hpp"

using namespace ceammc;

TEST_CASE("DataAtom", "[ceammc::DataAtom]")
{
    SECTION("create")
    {
        DataAtom a(1.2f);
        REQUIRE(a.isAtom());
        REQUIRE(!a.isData());
        REQUIRE(a.toAtom() == Atom(1.2f));

        DataAtom b(1.2f);
        REQUIRE(a == b);
        b.set(S("ABC"));
        REQUIRE_FALSE(a == b);

        DataPtr int_data(new IntData(123));
        b.set(int_data.asAtom());

        REQUIRE(b.isData());
        REQUIRE(b.data() == int_data);
        REQUIRE(b.data()->type() == int_data.data()->type());
        REQUIRE(b.data()->toString() == int_data.data()->toString());
        REQUIRE(b.data()->isEqual(int_data.data()));

        Atom aa = int_data.asAtom();
        Atom bb = b.toAtom();

        REQUIRE(aa.dataType() == bb.dataType());
        REQUIRE(aa == bb);

        DataAtom c(b);
        REQUIRE(c.isData());
        REQUIRE(c.data()->toString() == "123");
        REQUIRE(c.data().as<IntData>()->value() == 123);

        REQUIRE(b == c);
    }

    SECTION("copy")
    {
        DataAtom a(DataPtr(new IntData(1000)));
        REQUIRE(a.isData());
        REQUIRE(a.data().isValid());
        // ref count +1 of temp DataPtr
        REQUIRE(a.data().refCount() == 2);
        REQUIRE(a.data()->as<IntData>()->value() == 1000);

        DataAtom b = a;
        REQUIRE(a == b);
        REQUIRE(b.isData());
        // ref count +1 of temp DataPtr
        REQUIRE(a.data().refCount() == 3);
        REQUIRE(b.data().refCount() == 3);
        REQUIRE(a.data() == b.data());
        REQUIRE(b.data()->as<IntData>()->value() == 1000);
    }

    SECTION("==")
    {
        SECTION("simple")
        {
            DataAtom a(1);
            DataAtom b(2);

            REQUIRE(a == a);
            REQUIRE_FALSE(a == b);
            REQUIRE(a.toAtom() == Atom(1));
            REQUIRE(b.toAtom() == Atom(2));

            DataAtom c(b);
            REQUIRE(c == b);
            REQUIRE(b == c);

            b = c;
            REQUIRE(c == b);
        }

        SECTION("mixed")
        {
            DataAtom a(1);
            DataAtom b(DataPtr(new IntData(100)));

            REQUIRE(a == a);
            REQUIRE(b == b);
            REQUIRE_FALSE(a == b);
            REQUIRE_FALSE(b == a);

            REQUIRE(a.toAtom() == Atom(1));
            REQUIRE(b.data().isValid());

            b.set(Atom(1));
            REQUIRE(a == b);
            REQUIRE(b == a);
        }

        SECTION("data")
        {
            DataTPtr<IntData> data0(new IntData(1));
            DataTPtr<IntData> data1(new IntData(100));

            DataAtom a(data0.asAtom());
            DataAtom b(data1.asAtom());

            REQUIRE(a == a);
            REQUIRE(b == b);
            REQUIRE(a.data().isValid());
            REQUIRE(b.data().isValid());
            REQUIRE(a.data()->toString() == "1");
            REQUIRE(b.data()->toString() == "100");

            REQUIRE(a.data() != b.data());
            REQUIRE_FALSE(a == b);
            REQUIRE_FALSE(b == a);
        }
    }

    SECTION("set")
    {
        DataAtom a(DataPtr(new IntData(1)));

        REQUIRE(a.isData());
        a.set(Atom(DataDesc(0, 0)));
        REQUIRE(!a.isData());
        REQUIRE(a.toAtom().isNone());
    }

    SECTION("container")
    {
        std::vector<DataAtom> a;
        a.push_back(DataAtom(123));

        DataPtr int0(new IntData(10));
        a.push_back(DataAtom(int0));
        REQUIRE(std::find(a.begin(), a.end(), DataAtom(int0)) != a.end());

        a.pop_back();
        REQUIRE(std::find(a.begin(), a.end(), DataAtom(int0)) == a.end());
    }
}
