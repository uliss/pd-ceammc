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
#include "ceammc_xdata.h"
#include "ceammc_xdatastorage.h"

#include "catch.hpp"

#include <algorithm>
#include <vector>

using namespace ceammc;

static const DataDesc INVALID(data::DATA_INVALID, DataId(-1));

TEST_CASE("XData", "[ceammc::XData]")
{
    SECTION("construct")
    {
        SECTION("invalid pointer")
        {
            REQUIRE_FALSE(DataPtr(0).isValid());

            DataPtr p(0);
            REQUIRE(p.desc() == INVALID);
            REQUIRE(p.data() == 0);
            REQUIRE(p.as<IntData>() == 0);
            REQUIRE(p.refCount() == 0);

            DataPtr p2 = p;
            REQUIRE(p2.desc() == INVALID);
            REQUIRE(p2.data() == 0);
            REQUIRE(p2.as<IntData>() == 0);
            REQUIRE(p2.refCount() == 0);
            REQUIRE(p == p2);
        }

        SECTION("invalid atom")
        {
            Atom a;
            DataPtr p(a);
            REQUIRE(!p.isValid());
            REQUIRE(p.desc() == INVALID);
            REQUIRE(p.data() == 0);
            REQUIRE(p.as<IntData>() == 0);
            REQUIRE(p.refCount() == 0);

            DataPtr p1(new IntData(111));
            REQUIRE(p1.isValid());
            REQUIRE(p1.refCount() == 1);

            Atom a1(p1.desc());
            REQUIRE(p1.desc().type == a1.getData().type);
            REQUIRE(p1.desc().id == a1.getData().id);
            REQUIRE(p1.desc() == a1.getData());

            // ok
            DataPtr p2(Atom(p1.desc()));
            REQUIRE(p2.isValid());
            REQUIRE(p1.refCount() == 2);
            REQUIRE(p1 == p2);
            REQUIRE(p2.desc() != INVALID);
            REQUIRE(p2.data() != 0);
            REQUIRE(p2.as<IntData>() != 0);
            REQUIRE(p2.as<IntData>()->value() == 111);
            REQUIRE(p2.refCount() == 2);
        }
    }

    SECTION("operator=")
    {
        DataPtr p(new IntData(200));
        p = p;
        REQUIRE(p.refCount() == 1);
    }

    SECTION("container")
    {
        std::vector<DataPtr> vec;
        for (size_t i = 0; i < 10; i++) {
            vec.push_back(DataPtr(new IntData(i)));
        }

        for (size_t i = 0; i < vec.size(); i++) {
            REQUIRE(vec[i].refCount() == 1);
            REQUIRE(vec[i].as<IntData>());
            REQUIRE(vec[i].as<IntData>()->value() == i);
        }

        REQUIRE(XDataStorage::instance().size() == 10);
        vec.clear();
        REQUIRE(XDataStorage::instance().size() == 0);

        DataPtr d(new StrData("test"));
        vec.assign(20, d);
        REQUIRE(XDataStorage::instance().size() == 1);

        for (size_t i = 0; i < vec.size(); i++) {
            REQUIRE(vec[i].refCount() == 21);
            REQUIRE(vec[i].as<StrData>());
            REQUIRE(vec[i].as<StrData>()->get() == "test");
        }

        vec.clear();
        REQUIRE(XDataStorage::instance().size() == 1);
    }

    SECTION("equal")
    {
        DataPtr p0(new IntData(100));
        DataPtr p1(new IntData(200));
        DataPtr p2(new IntData(100));

        REQUIRE(p0 != p1);
        REQUIRE(p1 != p0);
        REQUIRE(p0 == p0);
        REQUIRE(p0 == p2);
        REQUIRE(p1 != p2);

        p0 = p1;
        REQUIRE(p0 == p1);
        REQUIRE(p0 != p2);
        REQUIRE(DataPtr(INVALID) == DataPtr(INVALID));
        REQUIRE(DataPtr(INVALID) == DataPtr(Atom()));
    }

    SECTION("compare")
    {
        DataPtr p0(new IntData(100));
        DataPtr p1(new IntData(200));
        DataPtr p2(new IntData(300));

        REQUIRE(p0 < p1);
        REQUIRE(p1 < p2);
        REQUIRE(p0 < p2);

        REQUIRE_FALSE(p0 < p0);
        REQUIRE_FALSE(p0 < DataPtr(p0));
        REQUIRE_FALSE(p0 < DataPtr(0));
        REQUIRE(DataPtr(0) < p0);

        std::vector<DataPtr> vec;
        for (size_t i = 0; i < 5; i++) {
            vec.push_back(DataPtr(new IntData(5 - i)));
        }

        REQUIRE(vec[0].as<IntData>()->value() == 5);
        REQUIRE(vec[1].as<IntData>()->value() == 4);
        REQUIRE(vec[2].as<IntData>()->value() == 3);
        REQUIRE(vec[3].as<IntData>()->value() == 2);
        REQUIRE(vec[4].as<IntData>()->value() == 1);

        std::sort(vec.begin(), vec.end());

        REQUIRE(vec[0].as<IntData>()->value() == 1);
        REQUIRE(vec[1].as<IntData>()->value() == 2);
        REQUIRE(vec[2].as<IntData>()->value() == 3);
        REQUIRE(vec[3].as<IntData>()->value() == 4);
        REQUIRE(vec[4].as<IntData>()->value() == 5);
    }

    SECTION("dataT")
    {
        DataTPtr<IntData> p(0);
        REQUIRE_FALSE(p.isValid());

        p = DataTPtr<IntData>(new IntData(123));
        REQUIRE(p.isValid());
        REQUIRE(p->value() == 123);

        DataTPtr<IntData> p1(p);
        REQUIRE(p1->value() == 123);

        DataTPtr<IntData> p2(new IntData(1111));
        DataTPtr<StrData> p3(p2.asAtom());

        REQUIRE(p2.refCount() == 1);
        REQUIRE(p3.refCount() == 0);
        REQUIRE(p3.isNull());
        REQUIRE(!p3.isValid());
    }
}
