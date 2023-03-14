/*****************************************************************************
 * Copyright 2023 Serge Poltavsky. All rights reserved.
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
#include "ceammc_data.h"
#include "datatype_color.h"
#include "test_common.h"

#include <boost/optional/optional_io.hpp>

using namespace ceammc;
using Color = DataTypeColor;

TEST_CASE("DataTypeColor", "[core]")
{
    SECTION("init")
    {
        SECTION("empty")
        {
            Color c;
            REQUIRE(c.toList() == LF(0, 0, 0, 1));
            REQUIRE(c.toString() == "#000000");
        }

        SECTION("hex")
        {
            Color s(0xAABBCCFF);
            REQUIRE(s.toString() == "#AABBCC");
            REQUIRE(s.toList() == LAX(0.666667, 0.733333, 0.8, 1));
            REQUIRE(s.toListString() == "Color(0.666667 0.733333 0.8 1)");
            REQUIRE(s.toDictString() == "Color[hex: #AABBCC]");
        }

        SECTION("hex alpha")
        {
            Color s(0xAABBCC80);
            REQUIRE(s.toString() == "#AABBCC80");
            REQUIRE(s.toList() == LAX(0.666667, 0.733333, 0.8, 0.501961));
            REQUIRE(s.toListString() == "Color(0.666667 0.733333 0.8 0.501961)");
            REQUIRE(s.toDictString() == "Color[hex: #AABBCC80]");
        }

        SECTION("args")
        {
            Color s(LF(1, 0, 1));
            REQUIRE(s.toList(true) == LF(1, 0, 1, 1));
            REQUIRE(s.toListString() == "Color(1 0 1 1)");
            REQUIRE(s.toDictString() == "Color[hex: #FF00FF]");
        }

        SECTION("args invalid")
        {
            Color s(LF(2, -1, 2));
            REQUIRE(s.toList(true) == LF(1, 0, 1, 1));
            REQUIRE(s.toListString() == "Color(1 0 1 1)");
            REQUIRE(s.toDictString() == "Color[hex: #FF00FF]");
        }

        SECTION("args hex")
        {
            Color s(LA("#ABC"));
            REQUIRE(s.toString() == "#AABBCC");
        }
    }

    SECTION("copy")
    {
        Color s0(0xFF);
        Color s1(s0);

        REQUIRE(s0.isEqual(&s1));
        REQUIRE(s1.isEqual(&s0));
    }

    SECTION("clone")
    {
        Color s0(0xFFAA);

        Color* s1 = s0.cloneT<Color>();
        REQUIRE(*s1 == s0);
        delete s1;
    }

    SECTION("parse")
    {
#define REQUIRE_DATA_STR(str, color)       \
    {                                      \
        Color c;                           \
        REQUIRE(c.setFromDataString(str)); \
        REQUIRE(c == color);               \
    }

        REQUIRE_DATA_STR("Color() ", Color());
        REQUIRE_DATA_STR("Color( ) ", Color());
        REQUIRE_DATA_STR("Color(1 2 3)", Color(LF(1, 2, 3)));
        Color s;
        REQUIRE_FALSE(s.setFromDataString("Color( "));
    }

    SECTION("export")
    {
        Color s;
        REQUIRE(s.toList() == LF(0, 0, 0, 1));
        REQUIRE(s.toString() == "#000000");
        REQUIRE(s.toListString() == "Color(0 0 0 1)");
        REQUIRE(s.toDictString() == "Color[hex: #000000]");
    }

    SECTION("to string export")
    {
        using MA = MListAtom;
        SECTION("toListStringContent")
        {
            CHECK(Color().toListStringContent() == "0 0 0 1");
        }

        SECTION("toListString")
        {
            CHECK(Color().toListString() == "Color(0 0 0 1)");
        }

        SECTION("toString")
        {
            CHECK(Color().toString() == "#000000");
        }

        SECTION("toDictString")
        {
            CHECK(Color().toDictString() == "Color[hex: #000000]");
        }
    }
}
