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
#include "color_scale.h"
#include "test_data_base.h"

#define REQUIRE_COLOR_OUTPUT(t, color)                  \
    {                                                   \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);               \
        REQUIRE(t.lastMessage(0).atomValue() == color); \
    }

PD_COMPLETE_TEST_SETUP(ColorScale, color, scale)

using DColor = DataTypeColor;
using ColorA = ColorAtom;

TEST_CASE("color.scale", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TExt t("color.scale");
        REQUIRE(t.numInlets());
        REQUIRE(t.numOutlets());
        REQUIRE_PROPERTY(t, @scale, L());
        REQUIRE_PROPERTY(t, @mode, LA("color"));
        REQUIRE_PROPERTY(t, @color, LF(1));
        REQUIRE_PROPERTY(t, @hex, LF(0));
        REQUIRE_PROPERTY(t, @rgb, LF(0));
        REQUIRE_PROPERTY(t, @interp, LA("none"));
    }

    SECTION("interp")
    {
        SECTION("short")
        {
            TExt t("color.scale", "Color(#ed514e)", "Color(#3451a4)", "Color(#ed514e)", "@hex", "@interp", "short");
            t << 0.0;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#ED514E");
            t << 0.5;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#3451A4");
            t << 1;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#ED514E");

            t << 0.25;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#A249A6");
            t << 0.75;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#A249A6");
        }

        SECTION("long")
        {
            TExt t("color.scale", "Color(#ed514e)", "Color(#3451a4)", "Color(#ed514e)", "@hex", "@interp", "long");
            t << 0.0;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#ED514E");
            t << 0.5;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#3451A4");
            t << 1;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#ED514E");

            t << 0.25;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#0D8A2D");
            t << 0.75;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#0D8A2D");
        }

        SECTION("inc")
        {
            TExt t("color.scale", "Color(#ed514e)", "Color(#3451a4)", "Color(#ed514e)", "@hex", "@interp", "inc");
            t << 0.0;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#ED514E");
            t << 0.5;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#3451A4");
            t << 1;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#ED514E");

            t << 0.25;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#0D8A2D");
            t << 0.75;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#A249A6");
        }

        SECTION("dec")
        {
            TExt t("color.scale", "Color(#ed514e)", "Color(#3451a4)", "Color(#ed514e)", "@hex", "@interp", "dec");
            t << 0.0;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#ED514E");
            t << 0.5;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#3451A4");
            t << 1;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#ED514E");

            t << 0.25;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#A249A6");
            t << 0.75;
            REQUIRE_SYMBOL_AT_OUTLET(0, t, "#0D8A2D");
        }
    }
}
