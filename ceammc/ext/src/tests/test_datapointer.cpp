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

    DataType type() const { return 123; }

public:
    static int constructor_called;
    static int destructor_called;
};

int TestIntData::constructor_called = 0;
int TestIntData::destructor_called = 0;

static bool reg = Data::registerData(123);

TEST_CASE("datapointer", "[PureData]")
{
    SECTION("construct")
    {
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
            DataPointer dp(new TestIntData(1025));
            REQUIRE(dp.id() == 2);
        }
    }

    SECTION("destruct")
    {
        REQUIRE(TestIntData::constructor_called == 2);
        REQUIRE(TestIntData::destructor_called == 2);

        {
            DataPointer dp(new TestIntData(1025));
            REQUIRE(dp.id() == 1);
        }
    }

    SECTION("atoms")
    {
        DataPointer dp(new TestIntData(1026));
        Atom a = dp.toAtom();

        {
            DataPointer dp2(DataPointer::fromAtom(a));
            REQUIRE(dp2.refCount() == 2);
            REQUIRE(dp.refCount() == 2);

            REQUIRE(dp.data() == dp2.data());
            REQUIRE(dp.data()->type() == 123);
            REQUIRE(dp.id() == dp2.id());
        }
    }
}
