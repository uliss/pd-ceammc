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
#include "ceammc_data.h"
#include "ceammc_dataatom.h"
#include "test_base.h"

#include "catch.hpp"

using namespace ceammc;

TEST_CASE("DataAtom", "[ceammc::DataAtom]")
{
    SECTION("create")
    {
        DataAtom a(1.2f);
        REQUIRE(a.isAtom());
        REQUIRE(a.isFloat());
        REQUIRE(!a.isData());
        REQUIRE(a.asAtom() == Atom(1.2f));

        DataAtom b(1.2f);
        REQUIRE(a == b);
        b.set(S("ABC"));
        REQUIRE(b.isSymbol());
        REQUIRE_FALSE(a == b);

        DataPtr int_data(new IntData(123));
        b.set(int_data.asAtom());

        REQUIRE(b.isData());
        REQUIRE(b.isValid());
        REQUIRE(b.data() == int_data);
        REQUIRE(b.data()->type() == int_data.data()->type());
        REQUIRE(b.data()->toString() == int_data.data()->toString());
        REQUIRE(b.data()->isEqual(int_data.data()));

        Atom aa = int_data.asAtom();
        Atom bb = b.asAtom();

        REQUIRE(aa.dataType() == bb.dataType());
        REQUIRE(aa == bb);

        DataAtom c(b);
        REQUIRE(c.isData());
        REQUIRE(c.data()->toString() == "123");
        REQUIRE(c.data().as<IntData>()->value() == 123);

        REQUIRE(b == c);

        SECTION("float")
        {
            DataAtom d(1000.5);
            REQUIRE(d.isValid());
            REQUIRE(d.isFloat());
            REQUIRE(d.asFloat() == Approx(1000.5));
            REQUIRE(d.asSymbol(nullptr) == nullptr);
        }

        SECTION("symbol")
        {
            DataAtom d(SYM("ABC"));
            REQUIRE(d.isValid());
            REQUIRE(d.isSymbol());
            REQUIRE(d.asSymbol() == SYM("ABC"));
            REQUIRE(d.asFloat() == 0);
        }

        SECTION("null")
        {
            REQUIRE(!DataAtom().isValid());
            REQUIRE(DataAtom().isNull());

            REQUIRE(DataAtom(DataPtr(nullptr)).isNull());
        }
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
            REQUIRE(a.asAtom() == Atom(1));
            REQUIRE(b.asAtom() == Atom(2));

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

            REQUIRE(a.asAtom() == Atom(1));
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
        REQUIRE(a.asAtom().isNone());

        a.set(100);
        REQUIRE(a.isFloat());
        REQUIRE(a.asFloat() == 100);
        a.set(SYM("ABC"));
        REQUIRE(a.isSymbol());
        REQUIRE(a.asSymbol() == SYM("ABC"));

        // move
        DataAtom b;
        b.set(DataPtr(new IntData(10)));
        REQUIRE(b.isData());
        REQUIRE(b.data()->toString() == "10");

        b.set(DataPtr(new StrData("ABC")));
        REQUIRE(b.isData());
        REQUIRE(b.data()->toString() == "ABC");
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

    SECTION("isDataType")
    {
        REQUIRE_FALSE(DataAtom().isDataType(IntData::dataType));
        REQUIRE(DataAtom(DataPtr(new IntData(100))).isDataType(IntData::dataType));
        REQUIRE_FALSE(DataAtom(DataPtr(new IntData(100))).isDataType(StrData::dataType));

        REQUIRE(DataAtom(DataPtr(new IntData(100))).isDataType<IntData>());
        REQUIRE_FALSE(DataAtom(DataPtr(new IntData(100))).isDataType<StrData>());
    }

    SECTION("as")
    {
        auto int_data = new IntData(100);
        auto pint = DataPtr(int_data);

        REQUIRE(DataAtom(Atom(123)).as<StrData>() == nullptr);
        REQUIRE(DataAtom(pint).as<StrData>() == nullptr);
        REQUIRE(DataAtom(pint).as<IntData>() != nullptr);
        REQUIRE(DataAtom(pint).as<IntData>()->value() == 100);

        REQUIRE(DataAtom(pint).asAtom().isData());
        REQUIRE(DataAtom(pint).asFloat(-100) == -100);
        REQUIRE(DataAtom(pint).asSymbol(nullptr) == nullptr);
    }

    SECTION("hash")
    {
        REQUIRE(DataAtom(100).hash_value() == DataAtom(100).hash_value());
        REQUIRE(DataAtom(100).hash_value() != DataAtom(101).hash_value());
        REQUIRE(DataAtom(SYM("A")).hash_value() == DataAtom(SYM("A")).hash_value());
        REQUIRE(DataAtom(SYM("A")).hash_value() != DataAtom(SYM("AB")).hash_value());

        auto i0 = new IntData(100);
        auto p0 = DataPtr(i0);
        auto i1 = new IntData(100);
        auto p1 = DataPtr(i1);
        auto i2 = new IntData(101);
        auto p2 = DataPtr(i2);

        REQUIRE(DataAtom(p0).hash_value() == DataAtom(p0).hash_value());
        REQUIRE(DataAtom(p0).hash_value() != DataAtom(p1).hash_value());
        REQUIRE(DataAtom(p0).hash_value() != DataAtom(p2).hash_value());
    }

    SECTION("operator=")
    {
        DataAtom df(100);
        DataAtom di(DataPtr(new IntData(10)));
        DataAtom ds(DataPtr(new StrData("ABC")));
        DataAtom ds1(DataPtr(new StrData("ABCD")));

        di = di;
        REQUIRE(di.isData());
        REQUIRE(di.data()->toString() == "10");

        di = df;
        REQUIRE(di.isFloat());
        REQUIRE(di.asFloat() == 100);

        di = ds;
        REQUIRE(di.isData());
        REQUIRE(di.data()->toString() == "ABC");
        REQUIRE(ds.isData());
        REQUIRE(ds.data()->toString() == "ABC");

        auto ctor = StrData::constructor_called;
        auto dtor = StrData::destructor_called;

        di = std::move(ds);
        REQUIRE(di.isData());
        REQUIRE(di.data()->toString() == "ABC");
        REQUIRE_FALSE(ds.isData());
        REQUIRE(ds.isNull());
        REQUIRE(ds.data().isNull());

        REQUIRE(ctor == StrData::constructor_called);
        REQUIRE(dtor == StrData::destructor_called);
    }
}
