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
#include "ceammc_datapointer.h"
#include "ceammc_datastorage.h"

#include <vector>

using namespace ceammc;

class TestIntData : public BaseData {
    int v_;

public:
    TestIntData(int v)
        : v_(v)
    {
        constructor_called++;
    }

    ~TestIntData()
    {
        destructor_called++;
    }

    int value() const { return v_; }
    void setValue(int v) { v_ = v; }

    DataType type() const { return dataType; }
    TestIntData* clone() const { return new TestIntData(v_); }

public:
    static DataType dataType;
    static int constructor_called;
    static int destructor_called;
};

static const DataType DTYPE = 123;
DataType TestIntData::dataType = DTYPE;
int TestIntData::constructor_called = 0;
int TestIntData::destructor_called = 0;

typedef Data<TestIntData> TestData;
typedef DataStorage<TestIntData> TestStorage;

TEST_CASE("datapointer", "[PureData]")
{
    SECTION("construct")
    {
        REQUIRE(TestStorage::instance().count() == 0);

        TestData dp(new TestIntData(1025));
        REQUIRE(!dp.isNull());
        REQUIRE(dp.id() == 1);

        REQUIRE(TestIntData::constructor_called == 1);
        REQUIRE(TestIntData::destructor_called == 0);

        {
            TestData t(new TestIntData(1025));
            REQUIRE(t.id() == 2);

            REQUIRE(TestStorage::instance().get(1) == &dp);
            REQUIRE(TestStorage::instance().get(2) == &t);
        }
    }

    SECTION("destruct")
    {
        REQUIRE(TestStorage::instance().count() == 0);

        REQUIRE(TestIntData::constructor_called == 2);
        REQUIRE(TestIntData::destructor_called == 2);

        {
            TestData dp(new TestIntData(1025));
            REQUIRE(dp.id() == 1);
            REQUIRE(TestStorage::instance().count() == 1);
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

    SECTION("atoms")
    {
        REQUIRE(TestStorage::instance().count() == 0);

        TestData dp(new TestIntData(1026));
        REQUIRE(dp.type() == DTYPE);
        REQUIRE(dp.id() == 1);

        Atom a = dp.toAtom();
        REQUIRE(a.isData());
        REQUIRE(a.isDataType(DTYPE));

        REQUIRE(TestStorage::instance().get(1));

        {
            TestData::DataPtr dp2 = TestData::fromAtom(a);
            REQUIRE(dp2);

            // test copy
            REQUIRE(dp.id() != dp2->id());
            REQUIRE(dp.type() == dp2->type());
            REQUIRE(dp.data() != dp2->data());
            REQUIRE(dp.data()->value() == dp2->data()->value());

            dp2->data()->setValue(20);
            REQUIRE(dp2->data()->value() == 20);

            REQUIRE(dp.data()->value() == 1026);
        }
    }

    SECTION("atomlist")
    {
        REQUIRE(TestStorage::instance().count() == 0);

        AtomList lst;

        {
            std::vector<TestData*> data;
            for (size_t i = 0; i < 15; i++) {
                const size_t ID = i + 1;
                REQUIRE(TestStorage::instance().generateId() == ID);
                data.push_back(new TestData(new TestIntData(int(i))));
                REQUIRE(data.back()->id() == ID);
                REQUIRE(TestStorage::instance().get(ID) != 0);
                REQUIRE(TestStorage::instance().get(ID)->type() == DTYPE);
                REQUIRE(TestStorage::instance().get(ID)->id() == ID);
            }

            REQUIRE(data.size() == 15);
            REQUIRE(TestStorage::instance().count() == 15);

            for (size_t i = 0; i < 15; i++) {
                REQUIRE(data[i]->data()->value() == i);
                Atom a = data[i]->toAtom();
                lst.append(a);
            }

            REQUIRE(data[0]->data()->value() == 0);
            REQUIRE(data[1]->data()->value() == 1);

            REQUIRE(data[0]->id() == 1);
            REQUIRE(data[1]->id() == 2);

            REQUIRE(TestStorage::instance().get(100) == 0);

            TestData* p0 = TestStorage::instance().get(1);
            TestData* p1 = TestStorage::instance().get(2);
            REQUIRE(p0 != 0);
            REQUIRE(p0 != p1);
            REQUIRE(p0->type() == DTYPE);
            REQUIRE(p0->data() == data[0]->data());
            REQUIRE(p1->data() == data[1]->data());

            TestData::DataPtr dp(TestData::fromAtom(lst[5]));
            REQUIRE(dp->type() == DTYPE);
            REQUIRE(dp->data()->value() == 5);

            REQUIRE(lst.size() == 15);
            for (size_t i = 0; i < 10; i++) {
                Atom a = lst[i];
                REQUIRE(a.isData());
                REQUIRE(a.isDataType(DTYPE));
                TestData::DataPtr dp = TestData::fromAtom(lst[i]);
                REQUIRE(dp);
                REQUIRE(dp->type() == DTYPE);
            }

            for (size_t i = 0; i < 15; i++) {
                delete data[i];
            }

            REQUIRE(TestStorage::instance().count() == 1);
        }

        REQUIRE(TestStorage::instance().count() == 0);

        REQUIRE(lst.size() == 15);
        for (size_t i = 0; i < lst.size(); i++) {
            TestData::DataPtr dp = TestData::fromAtom(lst[i]);
            REQUIRE_FALSE(dp);
        }
    }

    SECTION("fromAtom")
    {
        TestData d(new TestIntData(199));
        Atom a0 = d.toAtom();
        REQUIRE(a0.isData());
        TestData::DataPtr d2 = TestData::fromAtom(a0);

        REQUIRE(d2->data()->value() == 199);
        REQUIRE(d2.get() != &d);
        d2->data()->setValue(2000);
        REQUIRE(d2->data()->value() == 2000);
        REQUIRE(d.data()->value() == 199);

        Atom a1 = d2->toAtom();

        REQUIRE(a0 != a1);
        REQUIRE(a0.dataId() != a1.dataId());
    }
}
