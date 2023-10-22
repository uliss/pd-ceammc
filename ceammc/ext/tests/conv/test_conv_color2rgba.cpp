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
#include "conv_color2rgba.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(ConvColor2RGBA, conv, color2rgba)

TEST_CASE("conv.color2rgba", "[externals]")
{
    pd_test_init();

    SECTION("constuct")
    {
        SECTION("default")
        {
            TExt t("conv.color2rgba");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 4);
        }

        SECTION("@pack")
        {
            TExt t("conv.color2rgba", "@pack");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("alias")
        {
            TExt t("color->rgba");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 4);
        }
    }

    SECTION("convert")
    {
        SECTION("@pack")
        {
            SECTION("default")
            {
                TExt t("color->rgba", "@pack");
                t << "#1234";
                REQUIRE(t.outputListAt(0) == LF(0x11, 0x22, 0x33, 0x44));
                t << "#123";
                REQUIRE(t.outputListAt(0) == LF(0x11, 0x22, 0x33, 0xff));
                t << "red";
                REQUIRE(t.outputListAt(0) == LF(0xff, 0x00, 0x00, 0xff));

                t << ColorAtom(0x00FF00FF);
                REQUIRE(t.outputListAt(0) == LF(0x00, 0xff, 0x00, 0xff));
            }

            SECTION("float")
            {
                TExt t("color->rgba", "@pack", "@float");
                t << "red";
                REQUIRE(t.outputListAt(0) == LF(1, 0, 0, 1));

                t << ColorAtom(0x00FF00FF);
                REQUIRE(t.outputListAt(0) == LF(0, 1, 0, 1));
            }
        }

        SECTION("normal")
        {
            SECTION("default")
            {
                TExt t("color->rgba");
                t << "#1234";
                REQUIRE(t.outputFloatAt(0) == 0x11);
                REQUIRE(t.outputFloatAt(1) == 0x22);
                REQUIRE(t.outputFloatAt(2) == 0x33);
                REQUIRE(t.outputFloatAt(3) == 0x44);

                t << "red";
                REQUIRE(t.outputFloatAt(0) == 0xff);
                REQUIRE(t.outputFloatAt(1) == 0x00);
                REQUIRE(t.outputFloatAt(2) == 0x00);
                REQUIRE(t.outputFloatAt(3) == 0xff);

                t << ColorAtom(0x00FF00FF);
                REQUIRE(t.outputFloatAt(0) == 0x00);
                REQUIRE(t.outputFloatAt(1) == 0xff);
                REQUIRE(t.outputFloatAt(2) == 0x00);
                REQUIRE(t.outputFloatAt(3) == 0xff);
            }

            SECTION("float")
            {
                TExt t("color->rgba", "@float");
                t << "red";
                REQUIRE(t.outputFloatAt(0) == 1);
                REQUIRE(t.outputFloatAt(1) == 0);
                REQUIRE(t.outputFloatAt(2) == 0);
                REQUIRE(t.outputFloatAt(3) == 1);

                t << ColorAtom(0x00FF00FF);
                REQUIRE(t.outputFloatAt(0) == 0);
                REQUIRE(t.outputFloatAt(1) == 1);
                REQUIRE(t.outputFloatAt(2) == 0);
                REQUIRE(t.outputFloatAt(3) == 1);
            }
        }
    }
}
