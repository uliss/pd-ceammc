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

        Data int_data(new IntData(123));
        b.set(int_data.toAtom());

        REQUIRE(b.isData());
        REQUIRE(b.data() != int_data.data());
        REQUIRE(b.data()->type() == int_data.data()->type());
        REQUIRE(b.data()->toString() == int_data.data()->toString());
        REQUIRE(b.data()->isEqual(int_data.data()));
        REQUIRE(int_data.data()->isEqual(b.data()));

        Atom aa = int_data.toAtom();
        Atom bb = b.toAtom();

        REQUIRE(aa.dataType() == bb.dataType());
        REQUIRE(aa != bb);

        Data* aptr = DataStorage::instance().get(b.dataPtr()->desc());
        Data* bptr = DataStorage::instance().get(int_data.desc());

        REQUIRE(aptr != bptr);

        DataAtom c(b);
        REQUIRE(c.isData());
        REQUIRE(c.data()->toString() == "123");
        REQUIRE(((IntData*)c.data())->value() == 123);

        REQUIRE(b == c);
    }

    SECTION("copy")
    {
        DataAtom a(Data(new IntData(1000)));
        REQUIRE(a.isData());
        REQUIRE(a.data());
        REQUIRE(a.as<IntData>()->value() == 1000);

        DataAtom b = a;
        REQUIRE(a == b);
        REQUIRE(b.isData());
        REQUIRE(a.data() != b.data());
        REQUIRE(a.dataPtr() != b.dataPtr());
        REQUIRE(*a.dataPtr() == *b.dataPtr());
        REQUIRE(a.dataPtr()->desc().id != b.dataPtr()->desc().id);
        REQUIRE(b.as<IntData>()->value() == 1000);

        b.as<IntData>()->setValue(2000);
        REQUIRE_FALSE(a == b);

        b = a;
        REQUIRE(a == b);
        REQUIRE(b.isData());
        REQUIRE(a.data() != b.data());
    }

    SECTION("isEqual")
    {
        SECTION("simple")
        {
            DataAtom a(1);
            REQUIRE(a.isEqual(Atom(1)));
            REQUIRE_FALSE(a.isEqual(Atom(2)));
        }

        SECTION("mixed")
        {
            Data data(new IntData(1));
            DataAtom a(1);
            REQUIRE(a.isEqual(Atom(1)));
            REQUIRE_FALSE(a.isEqual(data.toAtom()));

            DataAtom b(data.toAtom());
            REQUIRE_FALSE(b.isEqual(Atom(1)));
            REQUIRE(b.isEqual(data.toAtom()));
        }

        SECTION("data")
        {
            DataT<IntData> data0(new IntData(1));
            DataT<IntData> data1(new IntData(100));

            DataAtom a(data0.toAtom());
            DataAtom b(data1.toAtom());

            REQUIRE(a.isEqual(a.toAtom()));
            REQUIRE(b.isEqual(b.toAtom()));
            REQUIRE_FALSE(a.isEqual(b.toAtom()));
            REQUIRE_FALSE(b.isEqual(a.toAtom()));
            REQUIRE_FALSE(a.isEqual(Atom(DataDesc(100, 100))));

            data0->setValue(100);
            // nothing changes since DataAtom has own copy
            REQUIRE(a.isEqual(a.toAtom()));
            REQUIRE(b.isEqual(b.toAtom()));
            REQUIRE_FALSE(a.isEqual(b.toAtom()));
            REQUIRE_FALSE(b.isEqual(a.toAtom()));

            a.data()->as<IntData>()->setValue(100);
            REQUIRE(a.isEqual(b.toAtom()));
            REQUIRE(b.isEqual(a.toAtom()));
        }
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
        }

        SECTION("mixed")
        {
            DataAtom a(1);

            Data data(new IntData(1));
            DataAtom b(data.toAtom());

            REQUIRE(a == a);
            REQUIRE(b == b);
            REQUIRE_FALSE(a == b);
            REQUIRE_FALSE(b == a);

            REQUIRE(a.toAtom() == Atom(1));
            REQUIRE(b.data());

            b.set(Atom(1));
            REQUIRE(a == b);
            REQUIRE(b == a);
        }

        SECTION("data")
        {
            DataT<IntData> data0(new IntData(1));
            DataT<IntData> data1(new IntData(100));

            DataAtom a(data0.toAtom());
            DataAtom b(data1.toAtom());

            REQUIRE(a == a);
            REQUIRE(b == b);
            REQUIRE(a.data());
            REQUIRE(b.data());
            REQUIRE(a.data()->toString() == "1");
            REQUIRE(b.data()->toString() == "100");
            REQUIRE(a.dataPtr());
            REQUIRE(b.dataPtr());

            REQUIRE(a.data() != b.data());
            REQUIRE(a.dataPtr() != b.dataPtr());

            REQUIRE_FALSE(a.data()->isEqual(b.data()));
            REQUIRE_FALSE(b.data()->isEqual(a.data()));
            REQUIRE_FALSE(a == b);
            REQUIRE_FALSE(b == a);
        }
    }

    SECTION("set")
    {
        DataT<IntData> data0(new IntData(1));
        DataAtom a(data0.toAtom());

        REQUIRE(a.isData());
        a.set(Atom(DataDesc(0, 0)));
        REQUIRE(!a.isData());
        REQUIRE(a.toAtom().isNone());
    }

    SECTION("container")
    {
        std::vector<DataAtom> a;
        a.push_back(DataAtom(123));

        Data int0(new IntData(10));
        a.push_back(DataAtom(int0));
        REQUIRE(std::find(a.begin(), a.end(), DataAtom(int0)) != a.end());

        a.pop_back();
        REQUIRE(std::find(a.begin(), a.end(), DataAtom(int0)) == a.end());
    }
}
