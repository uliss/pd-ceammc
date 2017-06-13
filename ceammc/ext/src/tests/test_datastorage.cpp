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

static const int DTYPE = 11;
#define ID(n) DataDesc(DTYPE, n)

TEST_CASE("DataStorage", "[ceammc::DataStorage]")
{
    SECTION("")
    {
        REQUIRE(&DataStorage::instance() == &DataStorage::instance());
        REQUIRE(DataStorage::instance().count() == 0);
        REQUIRE(DataStorage::instance().get(DataDesc(0, 0)) == 0);
        REQUIRE(DataStorage::instance().get(DataDesc(0, 1)) == 0);
        REQUIRE(DataStorage::instance().get(DataDesc(1, 0)) == 0);
        REQUIRE(DataStorage::instance().get(DataDesc(1, 1)) == 0);

        REQUIRE(DataStorage::instance().generateId() == 1);

        Data* p = (Data*)(0xBEEF);

        REQUIRE(DataStorage::instance().count() == 0);
        REQUIRE(DataStorage::instance().add(DataDesc(DTYPE, 1), p));
        REQUIRE(DataStorage::instance().count() == 1);
        REQUIRE(DataStorage::instance().get(ID(0)) == 0);
        REQUIRE(DataStorage::instance().get(ID(1)) == p);
        // already exists
        REQUIRE(!DataStorage::instance().add(ID(1), p));
        REQUIRE(DataStorage::instance().add(ID(2), p));
        REQUIRE(DataStorage::instance().add(ID(3), p)); // 1 2 3
        REQUIRE(DataStorage::instance().count() == 3);
        REQUIRE(DataStorage::instance().generateId() == 4);
        REQUIRE(!DataStorage::instance().remove(ID(1000)));
        REQUIRE(DataStorage::instance().remove(ID(3))); // 1 2
        REQUIRE(DataStorage::instance().add(ID(3), p));

        REQUIRE(DataStorage::instance().remove(ID(2))); // 1 3
        REQUIRE(DataStorage::instance().get(ID(2)) == 0);
        REQUIRE(DataStorage::instance().count() == 2);
        REQUIRE(DataStorage::instance().generateId() == 2);
    }

    SECTION("errors") {
        REQUIRE(DataStorage::instance().generateNewDesc(0) == DataDesc(0, 0));
    }
}
