/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "hoa_recomposer.h"
#include "test_spat_base.h"

PD_COMPLETE_TEST_SETUP(HoaRecomposer, spat, hoa_recomposer)

TEST_CASE("hoa.recomposer~", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("defaults")
        {
            TExt t("hoa.recomposer~");
            REQUIRE_PROPERTY_FLOAT(t, @order, 1);
            REQUIRE_PROPERTY_FLOAT(t, @n, 3);
            REQUIRE_PROPERTY_FLOAT(t, @ramp, 100);
            REQUIRE_PROPERTY(t, @mode, S("free"));

            REQUIRE(t->numInlets() == 3);
            REQUIRE(t->numOutlets() == 3);
            REQUIRE(t->numInputChannels() == 3);
            REQUIRE(t->numOutputChannels() == 3);
        }

        SECTION("order")
        {
            TExt t("hoa.recomposer~", 2);
            REQUIRE_PROPERTY_FLOAT(t, @order, 2);
            REQUIRE_PROPERTY_FLOAT(t, @n, 5);
            REQUIRE_PROPERTY_FLOAT(t, @ramp, 100);
            REQUIRE_PROPERTY(t, @mode, S("free"));

            REQUIRE(t->numInlets() == 5);
            REQUIRE(t->numOutlets() == 5);
            REQUIRE(t->numInputChannels() == 5);
            REQUIRE(t->numOutputChannels() == 5);
        }

        SECTION("@order")
        {
            TExt t("hoa.recomposer~", "@order", 3);
            REQUIRE_PROPERTY_FLOAT(t, @order, 3);
            REQUIRE_PROPERTY_FLOAT(t, @n, 7);
            REQUIRE_PROPERTY_FLOAT(t, @ramp, 100);
            REQUIRE_PROPERTY(t, @mode, S("free"));

            REQUIRE(t->numInlets() == 7);
            REQUIRE(t->numOutlets() == 7);
            REQUIRE(t->numInputChannels() == 7);
            REQUIRE(t->numOutputChannels() == 7);
        }

        SECTION("@n")
        {
            TExt t("hoa.recomposer~", 1, 10);
            REQUIRE_PROPERTY_FLOAT(t, @order, 1);
            REQUIRE_PROPERTY_FLOAT(t, @n, 10);
            REQUIRE_PROPERTY_FLOAT(t, @ramp, 100);
            REQUIRE_PROPERTY(t, @mode, S("free"));

            REQUIRE(t->numInlets() == 10);
            REQUIRE(t->numOutlets() == 3);
            REQUIRE(t->numInputChannels() == 10);
            REQUIRE(t->numOutputChannels() == 3);
        }

        SECTION("@n")
        {
            TExt t("hoa.recomposer~", 1, "@n", 5);
            REQUIRE_PROPERTY_FLOAT(t, @order, 1);
            REQUIRE_PROPERTY_FLOAT(t, @n, 5);
            REQUIRE_PROPERTY_FLOAT(t, @ramp, 100);
            REQUIRE_PROPERTY(t, @mode, S("free"));

            REQUIRE(t->numInlets() == 5);
            REQUIRE(t->numOutlets() == 3);
            REQUIRE(t->numInputChannels() == 5);
            REQUIRE(t->numOutputChannels() == 3);
        }

        SECTION("@n=0") // auto-calc
        {
            TExt t("hoa.recomposer~", 1, "@n", 0);
            REQUIRE_PROPERTY_FLOAT(t, @order, 1);
            REQUIRE_PROPERTY_FLOAT(t, @n, 3);
            REQUIRE_PROPERTY_FLOAT(t, @ramp, 100);
            REQUIRE_PROPERTY(t, @mode, S("free"));

            REQUIRE(t->numInlets() == 3);
            REQUIRE(t->numOutlets() == 3);
            REQUIRE(t->numInputChannels() == 3);
            REQUIRE(t->numOutputChannels() == 3);
        }

        SECTION("@n=-1") // auto-calc
        {
            TExt t("hoa.recomposer~", 1, "@n", -1);
            REQUIRE_PROPERTY_FLOAT(t, @order, 1);
            REQUIRE_PROPERTY_FLOAT(t, @n, 3);
            REQUIRE_PROPERTY_FLOAT(t, @ramp, 100);
            REQUIRE_PROPERTY(t, @mode, S("free"));

            REQUIRE(t->numInlets() == 3);
            REQUIRE(t->numOutlets() == 3);
            REQUIRE(t->numInputChannels() == 3);
            REQUIRE(t->numOutputChannels() == 3);

            REQUIRE(t->propSetRamp(1000));
            REQUIRE_PROPERTY_FLOAT(t, @ramp, 1000);
        }

        SECTION("@fisheye") // auto-calc
        {
            TExt t("hoa.recomposer~", 1, "@fisheye");
            REQUIRE_PROPERTY_FLOAT(t, @order, 1);
            REQUIRE_PROPERTY_FLOAT(t, @n, 3);
            REQUIRE_PROPERTY_FLOAT(t, @ramp, 100);
            REQUIRE_PROPERTY(t, @mode, S("fisheye"));

            REQUIRE(t->numInlets() == 4);
            REQUIRE(t->numOutlets() == 3);
            REQUIRE(t->numInputChannels() == 4);
            REQUIRE(t->numOutputChannels() == 3);

            REQUIRE_FALSE(t->propSetRamp(1000));
        }
    }
}
