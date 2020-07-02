/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "catch.hpp"
#include "ceammc_object_info.h"

using namespace ceammc;

TEST_CASE("ceammc::ObjectInfoStorage", "[core]")
{
    SECTION("basic")
    {
        REQUIRE(&ObjectInfoStorage::instance() == &ObjectInfoStorage::instance());
        REQUIRE_FALSE(ObjectInfoStorage::instance().hasInfo(nullptr));
        ObjectInfoStorage::instance().info(nullptr).dict["test"] = "abc";
        REQUIRE(ObjectInfoStorage::instance().hasInfo(nullptr));
        REQUIRE(ObjectInfoStorage::instance().info(nullptr).dict["test"] == "abc");
        REQUIRE(ObjectInfoStorage::instance().info(nullptr).deprecated == false);
        REQUIRE(ObjectInfoStorage::instance().info(nullptr).api == 0);
        REQUIRE(ObjectInfoStorage::instance().libName() == "ceammc");
        REQUIRE(ObjectInfoStorage::instance().libStrVersion() == "0.9");
    }
}
