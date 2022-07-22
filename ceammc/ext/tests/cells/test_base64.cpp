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
#include "catch.hpp"
#include "ceammc.h"
#include "ceammc_base64.h"

using namespace ceammc;

TEST_CASE("base64", "[PureData]")
{
    SECTION("encode")
    {
        REQUIRE(base64_encode("a") == "YQ==");
        REQUIRE(base64_encode("1") == "MQ==");
        REQUIRE(base64_encode("") == "");
        REQUIRE(base64_encode(" ") == "IA==");
        REQUIRE(base64_encode("  ") == "ICA=");
        REQUIRE(base64_encode("абвг") == "0LDQsdCy0LM=");
    }
}
