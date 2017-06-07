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
#include "catch.hpp"
#include "ceammc.hpp"
#include "ceammc_data.h"
#include "ceammc_datastorage.h"

#include <vector>

using namespace ceammc;

static const DataType DTYPE = IntData::dataType;
#define ID(n) DataDesc(DTYPE, n)

TEST_CASE("data", "[PureData]")
{
    SECTION("construct")
    {
        REQUIRE(DataStorage::instance().count() == 0);

        Data dp(new IntData(1025));
        REQUIRE(!dp.isNull());
        REQUIRE(dp.desc() == DataDesc(DTYPE, 1));

        REQUIRE(IntData::constructor_called == 1);
        REQUIRE(IntData::destructor_called == 0);

        {
            Data t(new IntData(1025));
            REQUIRE(t.desc() == DataDesc(DTYPE, 2));

            REQUIRE(DataStorage::instance().get(DataDesc(DTYPE, 1)) == &dp);
            REQUIRE(DataStorage::instance().get(DataDesc(DTYPE, 2)) == &t);
        }
    }

    SECTION("destruct")
    {
        REQUIRE(DataStorage::instance().count() == 0);

        REQUIRE(IntData::constructor_called == 2);
        REQUIRE(IntData::destructor_called == 2);

        {
            Data dp(new IntData(1025));
            REQUIRE(dp.desc() == DataDesc(DTYPE, 1));
            REQUIRE(DataStorage::instance().count() == 1);
        }
    }

    SECTION("data<->atom")
    {
        Atom a;
        a.setData(DataDesc(12, 35));
        REQUIRE(a.isData());
        REQUIRE(a.isDataType(12));
        DataDesc i = a.getData();
        REQUIRE(i.type == 12);
        REQUIRE(i.id == 35);
    }

    SECTION("clone")
    {
        Data d(new IntData(1254));
        Data* d_clone = d.clone();

        REQUIRE(d_clone != 0);
        REQUIRE(d_clone != &d);

        REQUIRE(d.data()->as<IntData>()->value() == 1254);
        REQUIRE(d_clone->data()->as<IntData>()->value() == 1254);

        REQUIRE(d == *d_clone);

        d.data()->as<IntData>()->setValue(8);
        REQUIRE_FALSE(d == *d_clone);

        delete d_clone;

        Data n;
        REQUIRE(n.clone() == 0);
    }

    SECTION("atoms")
    {
        REQUIRE(DataStorage::instance().count() == 0);

        DataT<IntData> dp(new IntData(1026));
        REQUIRE(dp.data());
        REQUIRE(dp.desc() == DataDesc(DTYPE, 1));

        Atom a = dp.toAtom();
        REQUIRE(a.isData());
        REQUIRE(a.isDataType(DTYPE));
        REQUIRE(Data::getTypedData(a.getData()) == &dp);

        REQUIRE(DataStorage::instance().get(ID(1)) == &dp);
        SharedDataPtr dp2 = Data::fromAtom(a);

        {
            SharedDataPtr dp2 = Data::fromAtom(a);
            REQUIRE(dp2);
            REQUIRE(dp2->data());

            // test copy
            REQUIRE(dp.desc() != dp2->desc());
            REQUIRE(dp.type() == dp2->type());
            REQUIRE(dp.data() != dp2->data());
            REQUIRE(dp->value() == dp2->data()->as<IntData>()->value());

            dp2->data()->as<IntData>()->setValue(20);
            REQUIRE(dp2->data()->as<IntData>()->value() == 20);

            REQUIRE(dp.data()->value() == 1026);
        }
    }

    SECTION("atomlist")
    {
        REQUIRE(DataStorage::instance().count() == 0);

        AtomList lst;

        {
            std::vector<DataT<IntData>*> data;
            for (size_t i = 0; i < 15; i++) {
                const size_t id = i + 1;
                REQUIRE(DataStorage::instance().generateId() == id);
                data.push_back(new DataT<IntData>(new IntData(int(i))));
                REQUIRE(data.back()->desc() == ID(id));
                REQUIRE(DataStorage::instance().get(ID(id)) != 0);
                REQUIRE(DataStorage::instance().get(ID(id))->type() == DTYPE);
                REQUIRE(DataStorage::instance().get(ID(id))->desc() == ID(id));
            }

            REQUIRE(data.size() == 15);
            REQUIRE(DataStorage::instance().count() == 15);

            for (size_t i = 0; i < 15; i++) {
                REQUIRE((*data[i])->value() == i);
                Atom a = data[i]->toAtom();
                lst.append(a);
            }

            REQUIRE(data[0]->data()->value() == 0);
            REQUIRE(data[1]->data()->value() == 1);

            REQUIRE(data[0]->desc() == ID(1));
            REQUIRE(data[1]->desc() == ID(2));

            REQUIRE(DataStorage::instance().get(ID(100)) == 0);

            Data* p0 = DataStorage::instance().get(ID(1));
            Data* p1 = DataStorage::instance().get(ID(2));
            REQUIRE(p0 != 0);
            REQUIRE(p0 != p1);
            REQUIRE(p0->type() == DTYPE);
            REQUIRE(p0->data() == data[0]->data());
            REQUIRE(p1->data() == data[1]->data());

            SharedDataPtr dp(Data::fromAtom(lst[5]));
            REQUIRE(dp->type() == DTYPE);
            REQUIRE(dp->data()->as<IntData>()->value() == 5);

            REQUIRE(lst.size() == 15);
            for (size_t i = 0; i < 10; i++) {
                Atom a = lst[i];
                REQUIRE(a.isData());
                REQUIRE(a.isDataType(DTYPE));
                SharedDataPtr dp = Data::fromAtom(lst[i]);
                REQUIRE(dp);
                REQUIRE(dp->type() == DTYPE);
            }

            for (size_t i = 0; i < 15; i++) {
                delete data[i];
            }

            REQUIRE(DataStorage::instance().count() == 1);
        }

        REQUIRE(DataStorage::instance().count() == 0);

        REQUIRE(lst.size() == 15);
        for (size_t i = 0; i < lst.size(); i++) {
            SharedDataPtr dp = Data::fromAtom(lst[i]);
            REQUIRE_FALSE(dp);
        }
    }

    SECTION("fromAtom")
    {
        Data d(new IntData(199));
        Atom a0 = d.toAtom();
        REQUIRE(a0.isData());
        SharedDataPtr d2 = Data::fromAtom(a0);

        REQUIRE(d2->data()->as<IntData>()->value() == 199);
        REQUIRE(d2.get() != &d);
        d2->data()->as<IntData>()->setValue(2000);
        REQUIRE(d2->data()->as<IntData>()->value() == 2000);
        REQUIRE(d.data()->as<IntData>()->value() == 199);

        Atom a1 = d2->toAtom();

        REQUIRE(a0 != a1);
        REQUIRE(a0.dataId() != a1.dataId());

        d2 = Data::fromAtom(Atom());
        REQUIRE_FALSE(d2);

        SECTION("type mismatch")
        {
            Data d0(new IntData(123));
            Data d1(new StrData("test"));

            Atom a0 = d0.toAtom();
            Atom a1 = d1.toAtom();
            a1.setData(DataDesc(a0.dataType(), a1.dataId()));
            SharedDataPtr p = Data::fromAtom(a1);

            REQUIRE(!p);
        }
    }

    SECTION("setData")
    {
        SECTION("pointer")
        {
            AbstractData* ptr = 0;
            Data d(new IntData(199));
            REQUIRE(d.data()->as<IntData>()->value() == 199);
            ptr = d.data();

            d.setData(new IntData(200));
            REQUIRE(d.data()->as<IntData>()->value() == 200);
            REQUIRE(d.data() != ptr);

            d.setData(0);
            REQUIRE(d.data() == 0);

            Data d1;
            REQUIRE(d1.data() == 0);
            d1.setData(0);
            REQUIRE(d1.data() == 0);

            d1.setData(new IntData(300));
            REQUIRE(d1.data() != 0);
        }

        SECTION("atom")
        {
            Data d0(new IntData(199));
            d0.setData(Atom(1));
            REQUIRE(d0.data()->as<IntData>()->value() == 199);

            Data d1(new IntData(256));
            REQUIRE(d0.data() != d1.data());

            d0.setData(d1.toAtom());
            REQUIRE(d0.data()->as<IntData>()->value() == 256);

            REQUIRE(d0.data() != d1.data());
        }
    }

    SECTION("toAtom")
    {
        Data d0;
        REQUIRE(d0.toAtom().isNone());
    }

    SECTION("DataT")
    {
        Data d0;
        const Data& r0 = d0;
        REQUIRE(r0.data() == 0);

        DataT<IntData> d1(new IntData(200));
        const DataT<IntData>& r1 = d1;
        REQUIRE(r1.data() != 0);
        REQUIRE(d1->value() == 200);
    }

    SECTION("==")
    {
        Data d0;
        Data d1(new IntData(123));

        REQUIRE(d0 == d0);
        REQUIRE(d0 == Data());
        REQUIRE(d1 == d1);
        REQUIRE_FALSE(d0 == d1);
        REQUIRE(d1 == Data(new IntData(123)));
        REQUIRE_FALSE(d1 == Data(new IntData(124)));
    }

    SECTION("setData")
    {
        Data d0;
        d0.setData(Atom(DataDesc(1234, 1234)));
        REQUIRE(d0.isNull());

        DataT<IntData> d1(new IntData(2));
        d1.setData(Atom(DataDesc(1234, 1234)));
        REQUIRE(d1->value() == 2);

        DataT<IntData> d2(new IntData(4));
        d1.setData(d2.toAtom());

        REQUIRE(d1->value() == 4);
        REQUIRE(d2->value() == 4);
        REQUIRE(d1.data() != d2.data());

        d2->setValue(16);
        REQUIRE(d1->value() == 4);
        REQUIRE(d2->value() == 16);
    }

    SECTION("abstract")
    {
        AbstractData* d0 = new IntData(123);
        AbstractData* d1 = new StrData("test");
        REQUIRE(!d0->AbstractData::toString().empty());
        REQUIRE(d0->AbstractData::isEqual(d0));
        REQUIRE(!d0->AbstractData::isEqual(d1));
        d0->dump();
        delete d0;
        delete d1;
    }
}
