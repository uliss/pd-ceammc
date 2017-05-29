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

class TestIntData : public Data {
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

public:
    static DataType dataType;
    static int constructor_called;
    static int destructor_called;
};

static const DataType DTYPE = 123;
DataType TestIntData::dataType = DTYPE;
int TestIntData::constructor_called = 0;
int TestIntData::destructor_called = 0;

static bool reg = Data::registerData(123);

TEST_CASE("datapointer", "[PureData]")
{
    SECTION("construct")
    {
        REQUIRE(DataStorage::instance().count(DTYPE) == 0);

        DataPointer dp(new TestIntData(1025));
        REQUIRE(dp.refCount() == 1);
        REQUIRE(!dp.isNull());
        REQUIRE(dp.id() == 1);

        REQUIRE(TestIntData::constructor_called == 1);
        REQUIRE(TestIntData::destructor_called == 0);

        // copy
        DataPointer dp2(dp);
        REQUIRE(dp.data() == dp2.data());

        REQUIRE(TestIntData::constructor_called == 1);
        REQUIRE(TestIntData::destructor_called == 0);

        REQUIRE(dp.refCount() == 2);
        REQUIRE(dp2.refCount() == 2);

        dp = dp2;
        REQUIRE(dp.refCount() == 2);
        REQUIRE(dp2.refCount() == 2);
        REQUIRE(dp.data() == dp2.data());

        REQUIRE(TestIntData::constructor_called == 1);
        REQUIRE(TestIntData::destructor_called == 0);

        {
            DataPointer t(new TestIntData(1025));
            REQUIRE(t.id() == 2);

            REQUIRE(DataStorage::instance().get(DTYPE, 1) == &dp);
            REQUIRE(DataStorage::instance().get(DTYPE, 2) == &t);
        }
    }

    SECTION("destruct")
    {
        REQUIRE(DataStorage::instance().count(DTYPE) == 0);

        REQUIRE(TestIntData::constructor_called == 2);
        REQUIRE(TestIntData::destructor_called == 2);

        {
            DataPointer dp(new TestIntData(1025));
            REQUIRE(dp.id() == 1);
            REQUIRE(DataStorage::instance().count(DTYPE) == 1);
        }
    }

    SECTION("atoms")
    {
        REQUIRE(DataStorage::instance().count(DTYPE) == 0);

        DataPointer dp(new TestIntData(1026));
        Atom a = dp.toAtom();

        {
            DataPointer dp2(DataPointer::fromAtom(a));
            REQUIRE(dp2.refCount() == 2);
            REQUIRE(dp.refCount() == 2);

            REQUIRE(dp.data() == dp2.data());
            REQUIRE(dp.data()->type() == 123);
            REQUIRE(dp.id() == dp2.id());

            TestIntData* ptr = dp2.as<TestIntData>();
            REQUIRE(ptr != 0);
            REQUIRE(ptr->type() == 123);
            REQUIRE(ptr->value() == 1026);

            ptr->setValue(20);
        }

        REQUIRE(dp.as<TestIntData>()->value() == 20);
    }

    SECTION("atomlist")
    {
        REQUIRE(DataStorage::instance().count(DTYPE) == 0);

        AtomList lst;

        {
            std::vector<DataPointer*> data;
            for (size_t i = 0; i < 15; i++) {
                const size_t ID = i + 1;
                REQUIRE(DataStorage::instance().generateId(DTYPE) == ID);
                data.push_back(new DataPointer(new TestIntData(int(i))));
                REQUIRE(data.back()->id() == ID);
                REQUIRE(DataStorage::instance().get(DTYPE, data.back()->id()) != 0);
                REQUIRE(DataStorage::instance().get(DTYPE, data.back()->id())->type() == DTYPE);
                REQUIRE(DataStorage::instance().get(DTYPE, data.back()->id())->id() == ID);
                REQUIRE(DataStorage::instance().get(DTYPE, ID) == data.back());
            }

            REQUIRE(data.size() == 15);
            REQUIRE(DataStorage::instance().count(DTYPE) == 15);

            for (size_t i = 0; i < 15; i++) {
                REQUIRE(data[i]->refCount() == 1);
                REQUIRE(data[i]->as<TestIntData>()->value() == i);
                Atom a = data[i]->toAtom();
                REQUIRE(a.isFloat());
                t_atom* a1 = reinterpret_cast<t_atom*>(&a);
                REQUIRE((a1->a_w.w_index & 0xFF) == i + 1);
                lst.append(a);

                REQUIRE(data[i]->refCount() == 1);
                REQUIRE(data[i]->as<TestIntData>()->value() == i);
            }

            REQUIRE(data[0]->as<TestIntData>()->value() == 0);
            REQUIRE(data[1]->as<TestIntData>()->value() == 1);

            REQUIRE(data[0]->id() == 1);
            REQUIRE(data[1]->id() == 2);

            REQUIRE(DataStorage::instance().get(124, 1) == 0);
            REQUIRE(DataStorage::instance().get(123, 100) == 0);

            DataPointer* p0 = DataStorage::instance().get(123, 1);
            DataPointer* p1 = DataStorage::instance().get(123, 2);
            REQUIRE(p0 != 0);
            REQUIRE(p0 != p1);
            REQUIRE(p0->type() == 123);
            REQUIRE(p0->data() == data[0]->data());
            REQUIRE(p1->data() == data[1]->data());

            DataPointer dp(DataPointer::fromAtom(lst[5]));
            REQUIRE(dp.type() == 123);
            REQUIRE(dp.as<TestIntData>()->value() == 5);

            REQUIRE(lst.size() == 15);
            for (size_t i = 0; i < 10; i++) {
                Atom a = lst[i];
                REQUIRE(a.isFloat());
                t_atom* a1 = reinterpret_cast<t_atom*>(&a);
                REQUIRE((a1->a_w.w_index & 0xFF) == i + 1);

                DataPointer dp = DataPointer::fromAtom(lst[i]);
                REQUIRE(dp.type() == 123);
                REQUIRE(dp.id() == i + 1);
            }

            REQUIRE(DataStorage::instance().count(DTYPE) == 15);

            for (size_t i = 0; i < 15; i++) {
                delete data[i];
            }

            // pointer
            REQUIRE(DataStorage::instance().count(DTYPE) == 1);
        }

        REQUIRE(DataStorage::instance().count(DTYPE) == 0);

        REQUIRE(lst.size() == 15);
        for (size_t i = 0; i < lst.size(); i++) {
            DataPointer dp = DataPointer::fromAtom(lst[i]);
            REQUIRE(dp.isNull());

            REQUIRE(DataPointer::isData(lst[i]));
        }
    }

    SECTION("isData")
    {
        REQUIRE_FALSE(DataPointer::isData(Atom()));
        REQUIRE_FALSE(DataPointer::isData(Atom(123)));
    }
}
