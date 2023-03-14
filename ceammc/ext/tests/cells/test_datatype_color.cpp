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

    SECTION("brigther")
    {
        CHECK(Color().brighten(0.1).toString() == "#030303");
        CHECK(Color().brighten(0.2).toString() == "#161616");
        CHECK(Color().brighten(0.2).brighten(-0.1).toString() == "#030303");
    }

    SECTION("darken")
    {
        CHECK(Color(0xFFFFFFFF).darken(0.1).toString() == "#DEDEDE");
        CHECK(Color(0xFFFFFFFF).darken(0.2).toString() == "#BEBEBE");
        CHECK(Color(0xFFFFFFFF).darken(0.2).darken(-0.1).toString() == "#DEDEDE");
    }

    SECTION("saturate")
    {
        CHECK(Color(0x330011FF).saturate(0.1).toString() == "#4C0009");
        CHECK(Color(0x330011FF).desaturate(-0.1).toString() == "#4C0009");
        CHECK(Color(0x4C0009FF).saturate(-0.1).toString() == "#282423");
        CHECK(Color(0x4C0009FF).desaturate(0.1).toString() == "#282423");
    }

    SECTION("rotate")
    {
        Color c(0x330011FF);
        CHECK(c.rotate(360).toString() == "#330011");
        CHECK(c.rotate(-360).toString() == "#330011");
        CHECK(c.rotate(180).toString() == "#00221E");
    }

    SECTION("flip")
    {
        Color c(0xFF0000FF);
        CHECK(c.flip(0).toString() == "#D82DCE");
    }

    SECTION("contrast")
    {
        Color c0, c1;
        c0.setRgb8(100, 100, 100);
        c1.setRgb8(105, 100, 88);
        CHECK(c0.contrast(c1) == Approx(1.00442));
    }

    SECTION("maximizeLightness")
    {
        CHECK(Color(0xAA0011FF).maximizeLightness().toString() == "#FF665C");
        CHECK(Color().maximizeLightness().toString() == "#000000");
        CHECK(Color(0x000001FF).maximizeLightness().toString() == "#E9F1FF");
    }

    SECTION("calculateLuminance")
    {
        CHECK(Color().calculateLuminance() == 0);
        CHECK(Color(0xFFFFFFFF).calculateLuminance() == 1);
        CHECK(Color().setHsl(56, 0.38, 0.58).calculateLuminance() == Approx(0.45771));
    }

    SECTION("mix")
    {
        CHECK(Color().mix(Color(0x808080FF), 0.5).toString() == "#404040");
        CHECK(Color().mix(Color(0x808080FF), 1).toString() == "#808080");
        CHECK(Color().mix(Color(0x808080FF), 2).toString() == "#808080");
        CHECK(Color().mix(Color(0x808080FF), 0).toString() == "#000000");
        CHECK(Color().mix(Color(0x808080FF), -1).toString() == "#000000");
    }

    SECTION("invert")
    {
        CHECK(Color().invert().toString() == "#FFFFFF");
        CHECK(Color(0xFFFFFFFF).invert().toString() == "#000000");
        CHECK(Color(0x0F0F0FFF).invert().toString() == "#F0F0F0");
    }

    SECTION("set")
    {
        CHECK(Color().setRgb8(3, 2, 1, 0x80).toString() == "#03020180");
        CHECK(Color().setRgbf(1, 0, 1, 1).toString() == "#FF00FF");
        CHECK(Color().setRgbf(1, 0, 1, 0.5).toString() == "#FF00FF80");
        CHECK(Color().setRgbf(2, -1, 1.1, 0.5).toString() == "#FF00FF80");
        CHECK(Color().setHsl(0, 1, 1).toString() == "#FFFFFF");
        CHECK(Color().setHwb(0, 1, 0).toString() == "#FF0303");
        CHECK(Color().setHwb(180, 1, 0).toString() == "#03FFFF");
        CHECK(Color().setOkLab(1, -1, -1).toString() == "#0000FF");
        CHECK(Color().setOkLab(1, -1, 1).toString() == "#00FF00");
        CHECK(Color().setOkLab(1, 1, 1).toString() == "#FF0000");
        CHECK(Color().setOkLab(1, 1, -1).toString() == "#FF00FF");

        Color c;
        CHECK(c.setHex("#ABC"));
        REQUIRE(c.toString() == "#AABBCC");
        CHECK(c.setHex("#ABCD"));
        REQUIRE(c.toString() == "#AABBCCDD");
        CHECK(c.setHex("#ABCABC"));
        REQUIRE(c.toString() == "#ABCABC");
        CHECK(c.setHex("#ABCABCDE"));
        REQUIRE(c.toString() == "#ABCABCDE");
    }
}
