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
#include "ceammc_datatypes.h"
#include "ceammc_data.h"
#include "ceammc_datastorage.h"

#include "catch.hpp"

using namespace ceammc;

static const DataDesc INVALID(data::DATA_INVALID, DataId(-1));

TEST_CASE("XDataStorage", "[ceammc::XDataStorage]")
{
    SECTION("")
    {
        REQUIRE(&DataStorage::instance() == &DataStorage::instance());
        REQUIRE(DataStorage::instance().size() == 0);
        REQUIRE(DataStorage::instance().add(0) == INVALID);
        REQUIRE(DataStorage::instance().refCount(INVALID) == 0);
        REQUIRE(DataStorage::instance().acquire(INVALID) == 0);
    }

    SECTION("create simple")
    {
        IntData* data = new IntData(123);
        DataPtr d(data);

        REQUIRE(DataStorage::instance().size() == 1);
        REQUIRE(DataStorage::instance().refCount(d.desc()) == 1);

        // new pointer
        DataPtr d2(d);
        REQUIRE(DataStorage::instance().size() == 1);
        REQUIRE(DataStorage::instance().refCount(d2.desc()) == 2);
        REQUIRE(DataStorage::instance().acquire(d2.desc()) == data);

        REQUIRE(DataStorage::instance().refCount(d.desc()) == 3);
        DataStorage::instance().release(d2.desc());
        REQUIRE(DataStorage::instance().refCount(d.desc()) == 2);

        REQUIRE(d2->type() == d->type());
        REQUIRE(d2->isEqual(d.data()));

        REQUIRE(d2->as<IntData>() == data);
        REQUIRE(d->as<IntData>()->value() == 123);
        REQUIRE(d2->as<IntData>()->value() == 123);

        REQUIRE(d2->as<StrData>() == 0);
    }

    SECTION("operator=")
    {
        REQUIRE(DataStorage::instance().size() == 0);
        IntData* d0 = new IntData(1024);
        StrData* d1 = new StrData("ABC");

        DataPtr p0(d0);
        DataPtr p1(d1);

        REQUIRE(DataStorage::instance().size() == 2);

        SECTION("copy")
        {
            DataPtr p2 = p1;
            REQUIRE(p2.as<StrData>()->get() == "ABC");
            REQUIRE(DataStorage::instance().refCount(p1.desc()) == 2);
            REQUIRE(p2.refCount() == 2);
            REQUIRE(p1.refCount() == 2);
        }

        REQUIRE(DataStorage::instance().refCount(p1.desc()) == 1);
        REQUIRE(p1.refCount() == 1);

        SECTION("operator=")
        {
            REQUIRE(p1.refCount() == 1);
            REQUIRE(p0.refCount() == 1);
            REQUIRE(p1.as<StrData>()->get() == "ABC");
            REQUIRE(p1.as<IntData>() == 0);
            REQUIRE(DataStorage::instance().size() == 2);

            p1 = p0;

            REQUIRE(DataStorage::instance().size() == 1);
            REQUIRE(p1.refCount() == 2);
            REQUIRE(p0.refCount() == 2);
        }
    }
}
