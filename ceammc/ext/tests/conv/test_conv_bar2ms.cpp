/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the ter of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "conv_bar2ms.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(ConvBarToMs, conv, bar2ms)

using namespace ceammc;

TEST_CASE("conv.bar2ms", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("conv.bar2ms");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);

            REQUIRE_PROPERTY_LIST(t, @bpm, LA("60|4bpm"));
            REQUIRE_PROPERTY_LIST(t, @tsig, LA("|4/4|"));
        }

        SECTION("alias")
        {
            TExt t("bar->ms");
        }
    }

    SECTION("onBang")
    {
        SECTION("default")
        {
            TExt t("conv.bar2ms");
            t.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, t, 4000);
        }

        SECTION("args")
        {
            TExt t("conv.bar2ms", LA(120));
            t.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, t, 2000);
        }

        SECTION("args")
        {
            TExt t("conv.bar2ms", LA("120bpm"));
            t.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, t, 2000);
        }

        SECTION("args")
        {
            TExt t("conv.bar2ms", LA("120|4bpm"));
            t.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, t, 2000);
        }

        SECTION("args")
        {
            TExt t("conv.bar2ms", LA("120|2/8bpm"));
            t.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, t, 2000);
        }

        SECTION("args")
        {
            TExt t("conv.bar2ms", LA(120, "|5/4|"));
            t.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, t, 2500);
        }

        SECTION("args")
        {
            TExt t("conv.bar2ms", LA(60, "|3/8|"));
            t.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1500);
        }

        SECTION("args")
        {
            TExt t("conv.bar2ms", LA("60|8bpm", "|3/8|"));
            t.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, t, 3000);
        }

        SECTION("args")
        {
            TExt t("conv.bar2ms", LA("60|4.bpm", "|3/8|"));
            t.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1000);
        }

        SECTION("args")
        {
            TExt t("conv.bar2ms", LA("60|16bpm", "|3/8|"));
            t.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, t, 6000);
        }
    }

    SECTION("onFloat")
    {
        TExt t("conv.bar2ms", "75|4.bpm");
        t << 120;
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2000);
        REQUIRE_PROPERTY_LIST(t, @bpm, LA("120|4bpm"));
        t << 30;
        REQUIRE_FLOAT_AT_OUTLET(0, t, 8000);
        REQUIRE_PROPERTY_LIST(t, @bpm, LA("30|4bpm"));

        t.bang();
        REQUIRE_FLOAT_AT_OUTLET(0, t, 8000);
    }

    SECTION("onSymbol")
    {
        TExt t("conv.bar2ms", "75|4.bpm");
        t << "125";
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1920.0);
        REQUIRE_PROPERTY_LIST(t, @bpm, LA("125"));
        t << "60|2/4";
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2000);
        REQUIRE_PROPERTY_LIST(t, @bpm, LA("60|2/4"));

        t << "60|4.bpm";
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(8000 / 3.0));
        REQUIRE_PROPERTY_LIST(t, @bpm, LA("60|4.bpm"));

        t << "invalid";
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("onList")
    {
        TExt t("conv.bar2ms");
        t << LA(40, "|5/4|");

        REQUIRE_PROPERTY_LIST(t, @bpm, LA("40|4bpm"));
        REQUIRE_PROPERTY_LIST(t, @tsig, LA("|5/4|"));

        REQUIRE_FLOAT_AT_OUTLET(0, t, 7500);

        t << LA(60);
        REQUIRE_PROPERTY_LIST(t, @bpm, LA("60|4bpm"));
        REQUIRE_PROPERTY_LIST(t, @tsig, LA("|5/4|"));
    }

    SECTION("onInlet")
    {
        TExt t("conv.bar2ms");
        t.sendListTo(LA("|1/4+3/8|"), 1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY_LIST(t, @tsig, LA("|1/4+3/8|"));

        t.bang();
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2500);
    }
}
