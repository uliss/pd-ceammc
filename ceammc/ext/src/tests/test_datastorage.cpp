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
#include "ceammc_datastorage.h"

#include "catch.hpp"

using namespace ceammc;

class TestData : public AbstractData {
    int v_;

public:
    TestData(int v = 0)
        : v_(v)
    {
    }

    DataType type() const { return 812; }
    TestData* clone() const { return new TestData(v_); }
};

typedef DataStorage<TestData> TestStorage;
typedef Data<TestData> TestDataPtr;

TEST_CASE("DataStorage", "[ceammc::DataStorage]")
{
    SECTION("")
    {
        REQUIRE(&TestStorage::instance() == &TestStorage::instance());
        REQUIRE(TestStorage::instance().count() == 0);
        REQUIRE(TestStorage::instance().get(0) == 0);
        REQUIRE(TestStorage::instance().get(1) == 0);

        REQUIRE(TestStorage::instance().generateId() == 1);

        TestDataPtr* p = (TestDataPtr*)(0xBEEF);

        REQUIRE(TestStorage::instance().count() == 0);
        REQUIRE(TestStorage::instance().add(1, p));
        REQUIRE(TestStorage::instance().count() == 1);
        REQUIRE(TestStorage::instance().get(0) == 0);
        REQUIRE(TestStorage::instance().get(1) == p);
        // already exists
        REQUIRE(!TestStorage::instance().add(1, p));
        REQUIRE(TestStorage::instance().add(2, p));
        REQUIRE(TestStorage::instance().add(3, p));
        REQUIRE(TestStorage::instance().count() == 3);
        REQUIRE(TestStorage::instance().generateId() == 4);
        REQUIRE(!TestStorage::instance().remove(1000));
        REQUIRE(TestStorage::instance().remove(3));
        REQUIRE(TestStorage::instance().add(3, p));

        REQUIRE(TestStorage::instance().remove(2));
        REQUIRE(TestStorage::instance().get(2) == 0);
        REQUIRE(TestStorage::instance().count() == 2);
        REQUIRE(TestStorage::instance().generateId() == 2);
    }
}
