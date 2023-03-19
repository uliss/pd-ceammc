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
#include "data_color.h"
#include "test_data_base.h"

#define REQUIRE_COLOR_OUTPUT(t, color)                  \
    {                                                   \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);               \
        REQUIRE(t.lastMessage(0).atomValue() == color); \
    }

PD_COMPLETE_TEST_SETUP(DataColor, data, color)

using DColor = DataTypeColor;
using ColorA = ColorAtom;

TEST_CASE("data.color", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TExt t("data.color");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendBang();
            REQUIRE_COLOR_OUTPUT(t, ColorA());
        }

        SECTION("args")
        {
            TExt t("data.color", LA("Color(red)"));

            t.sendBang();
            REQUIRE_COLOR_OUTPUT(t, ColorA(0xFF0000FF));
        }

        SECTION("prop")
        {
            TExt t("data.color", LA("@value", "Color(blue)"));

            t.sendBang();
            REQUIRE_COLOR_OUTPUT(t, ColorA(0x0000FFFF));
        }

        SECTION("hex RGB")
        {
            TExt t("data.color", LA("Color(#BAD)"));

            t.sendBang();
            REQUIRE_COLOR_OUTPUT(t, ColorA(0xBBAADDFF));
        }

        SECTION("hex RGBA")
        {
            TExt t("data.color", LA("Color(#BAD2)"));

            t.sendBang();
            REQUIRE_COLOR_OUTPUT(t, ColorA(0xBBAADD22));
        }

        SECTION("hex RRGGBB")
        {
            TExt t("data.color", LA("Color(#123456)"));

            t.sendBang();
            REQUIRE_COLOR_OUTPUT(t, ColorA(0x123456FF));
        }

        SECTION("hex RRGGBBAA")
        {
            TExt t("data.color", LA("Color(#12345678)"));

            t.sendBang();
            REQUIRE_COLOR_OUTPUT(t, ColorA(0x12345678));
        }

        SECTION("data RGB short")
        {
            TExt t("data.color", LA("#ABC"));

            t.sendBang();
            REQUIRE_COLOR_OUTPUT(t, ColorA(0xAABBCCFF));
        }

        SECTION("data RGBA short")
        {
            TExt t("data.color", LA("#1234"));

            t.sendBang();
            REQUIRE_COLOR_OUTPUT(t, ColorA(0x11223344));
        }

        SECTION("data RRGGBB")
        {
            TExt t("data.color", LA("#123456"));

            t.sendBang();
            REQUIRE_COLOR_OUTPUT(t, ColorA(0x123456FF));
        }

        SECTION("data RRGGBBAA")
        {
            TExt t("data.color", LA("#12345678"));

            t.sendBang();
            REQUIRE_COLOR_OUTPUT(t, ColorA(0x12345678));
        }
    }

    SECTION("data")
    {
        TExt t("data.color");

        t << ColorA(0xFFAABBFF);
        REQUIRE_COLOR_OUTPUT(t, ColorA(0xFFAABBFF));
    }

    SECTION("second inlet")
    {
        TExt t("data.color");
        t.sendListTo({ ColorAtom(0xFFAABBCC) }, 1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFFAABBCC));

        t.sendSymbolTo("Color(lime)", 1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x00FF00FF));

        t.sendSymbolTo("Color(#DEF)", 1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xDDEEFFFF));
    }

    SECTION("methods")
    {
        TExt t("data.color");

        SECTION("hex")
        {
            t.call("hex", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x000000FF));
            t.call("hex", LA("#ABC"));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xAABBCCFF));
            t.call("hex", LA("#ABC3"));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xAABBCC33));
            t.call("hex", LA("#AABBCC"));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xAABBCCFF));
            t.call("hex", LA("#AABBCC11"));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xAABBCC11));

            t.call("hex", LA("#invalid"));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xAABBCC11));
        }

        SECTION("rgb")
        {
            t.call("rgb", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x000000FF));
            t.call("rgb", LF(1, 0, 0));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFF0000FF));
            t.call("rgb", LF(1, 0.5, 0));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFF8000FF));
            t.call("rgb", LF(1, 0.5, 0.25));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFF8040FF));
            t.call("rgb", LF(1, 0.5, 0.25, 0.125));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFF804020));
        }

        SECTION("rgb8")
        {
            t.call("rgb8", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x000000FF));
            t.call("rgb8", LF(255, 0, 0));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFF0000FF));
            t.call("rgb8", LF(255, 0x60, 0));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFF6000FF));
            t.call("rgb8", LF(255, 0x20, 0x30));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFF2030FF));
            t.call("rgb8", LF(255, 254, 253, 252));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFFFEFDFC));
        }

        SECTION("red")
        {
            t.call("red", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x000000FF));
            t.call("red", LF(0.5));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x800000FF));
        }

        SECTION("green")
        {
            t.call("green", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x000000FF));
            t.call("green", LF(0.5));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x008000FF));
        }

        SECTION("blue")
        {
            t.call("blue", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x000000FF));
            t.call("blue", LF(0.5));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x000080FF));
        }

        SECTION("alpha")
        {
            t.call("alpha", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x000000FF));
            t.call("alpha", LF(0.5));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x00000080));
        }

        SECTION("darken")
        {
            t.call("hex", LA("#F00"));
            t.call("darken", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFF0000FF));
            t.call("darken", LF(0.25));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xA10000FF));
            t.call("darken", LF(0.25));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x4B0000FF));
            t.call("darken", LF(0.5));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x010000FF));
            t.call("darken", LF(0.5));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x000000FF));
        }

        SECTION("brighten")
        {
            t.call("hex", LA("#100"));
            t.call("brighten", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x110000FF));
            t.call("brighten", LF(0.25));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x53342FFF));
            t.call("brighten", LF(0.25));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x9D7A74FF));
            t.call("brighten", LF(0.5));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFFFFFFFF));
        }

        SECTION("grayscale")
        {
            t.call("hex", LA("#F00"));
            t.call("grayscale", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x888888FF));
            t.call("grayscale", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x888888FF));
        }

        SECTION("invert")
        {
            t.call("hex", LA("#F00"));
            t.call("invert", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x00FFFFFF));
            t.call("invert", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFF0000FF));
        }

        SECTION("mix")
        {
            t.call("hex", LA("#F00"));
            t.call("mix", L());
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFF0000FF));
            t.call("mix", LA("#169"));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFF0000FF));
            t.call("mix", LA("#169", 0.));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0xFF0000FF));
            t.call("mix", LA("#169", 0.5));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x88334DFF));
            t.call("mix", LA("#169", 1));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x116699FF));
            t.call("mix", LA("green", 0.5));
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x09734DFF));

            t.sendBang();
            REQUIRE_PROPERTY_LIST(t, @value, ColorAtom(0x09734DFF));
        }
    }
}
