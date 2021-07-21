/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "fx_tapiir_priv.h"
#include "test_base.h"
#include "test_external.h"
#include "test_sound.h"

PD_COMPLETE_TEST_SETUP(FxTapiir, fx, tapiir_tilde)

TEST_CASE("fx.tapiir~", "[externals]")
{
    test::pdPrintToStdError();

    SECTION("init")
    {
        TObj t("fx.tapiir~");
        REQUIRE(1);
    }

    SECTION("@delays")
    {
        TObj t("fx.tapiir~", LA("@delays", 1, 2, 3, 4, 5, 6));
        REQUIRE_PROPERTY_FLOAT(t, @tap0.delay, 1);
        REQUIRE_PROPERTY_FLOAT(t, @tap1.delay, 2);
        REQUIRE_PROPERTY_FLOAT(t, @tap2.delay, 3);
        REQUIRE_PROPERTY_FLOAT(t, @tap3.delay, 4);
        REQUIRE_PROPERTY_FLOAT(t, @tap4.delay, 5);
        REQUIRE_PROPERTY_FLOAT(t, @tap5.delay, 6);
        REQUIRE_PROPERTY(t, @delays, LF(1, 2, 3, 4, 5, 6));
    }

    SECTION("@gains")
    {
        TObj t("fx.tapiir~", LA("@gains", -1, -2, -3, -4, -5, -6));

        REQUIRE_PROPERTY_FLOAT(t, @tap0.gain, -1);
        REQUIRE_PROPERTY_FLOAT(t, @tap1.gain, -2);
        REQUIRE_PROPERTY_FLOAT(t, @tap2.gain, -3);
        REQUIRE_PROPERTY_FLOAT(t, @tap3.gain, -4);
        REQUIRE_PROPERTY_FLOAT(t, @tap4.gain, -5);
        REQUIRE_PROPERTY_FLOAT(t, @tap5.gain, -6);
        REQUIRE_PROPERTY(t, @gains, LF(-1, -2, -3, -4, -5, -6));
    }

    SECTION("@outs0")
    {
        TObj t("fx.tapiir~", LA("@outs0", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY(t, @outs0, LF(0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY_FLOAT(t, @out0.tap0, 0.1);
        REQUIRE_PROPERTY_FLOAT(t, @out0.tap1, 0.2);
        REQUIRE_PROPERTY_FLOAT(t, @out0.tap2, 0.3);
        REQUIRE_PROPERTY_FLOAT(t, @out0.tap3, 0.4);
        REQUIRE_PROPERTY_FLOAT(t, @out0.tap4, 0.5);
        REQUIRE_PROPERTY_FLOAT(t, @out0.tap5, 0.6);
    }

    SECTION("@outs1")
    {
        TObj t("fx.tapiir~", LA("@outs1", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY(t, @outs1, LF(0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY_FLOAT(t, @out1.tap0, 0.1);
        REQUIRE_PROPERTY_FLOAT(t, @out1.tap1, 0.2);
        REQUIRE_PROPERTY_FLOAT(t, @out1.tap2, 0.3);
        REQUIRE_PROPERTY_FLOAT(t, @out1.tap3, 0.4);
        REQUIRE_PROPERTY_FLOAT(t, @out1.tap4, 0.5);
        REQUIRE_PROPERTY_FLOAT(t, @out1.tap5, 0.6);
    }

    SECTION("@fbs0")
    {
        TObj t("fx.tapiir~", LA("@fbs0", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY(t, @fbs0, LF(0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY_FLOAT(t, @tap0.fb0, 0.1);
        REQUIRE_PROPERTY_FLOAT(t, @tap0.fb1, 0.2);
        REQUIRE_PROPERTY_FLOAT(t, @tap0.fb2, 0.3);
        REQUIRE_PROPERTY_FLOAT(t, @tap0.fb3, 0.4);
        REQUIRE_PROPERTY_FLOAT(t, @tap0.fb4, 0.5);
        REQUIRE_PROPERTY_FLOAT(t, @tap0.fb5, 0.6);
    }

    SECTION("@fbs1")
    {
        TObj t("fx.tapiir~", LA("@fbs1", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY(t, @fbs1, LF(0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY_FLOAT(t, @tap1.fb0, 0.1);
        REQUIRE_PROPERTY_FLOAT(t, @tap1.fb1, 0.2);
        REQUIRE_PROPERTY_FLOAT(t, @tap1.fb2, 0.3);
        REQUIRE_PROPERTY_FLOAT(t, @tap1.fb3, 0.4);
        REQUIRE_PROPERTY_FLOAT(t, @tap1.fb4, 0.5);
        REQUIRE_PROPERTY_FLOAT(t, @tap1.fb5, 0.6);
    }

    SECTION("@fbs2")
    {
        TObj t("fx.tapiir~", LA("@fbs2", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY(t, @fbs2, LF(0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY_FLOAT(t, @tap2.fb0, 0.1);
        REQUIRE_PROPERTY_FLOAT(t, @tap2.fb1, 0.2);
        REQUIRE_PROPERTY_FLOAT(t, @tap2.fb2, 0.3);
        REQUIRE_PROPERTY_FLOAT(t, @tap2.fb3, 0.4);
        REQUIRE_PROPERTY_FLOAT(t, @tap2.fb4, 0.5);
        REQUIRE_PROPERTY_FLOAT(t, @tap2.fb5, 0.6);
    }

    SECTION("@fbs3")
    {
        TObj t("fx.tapiir~", LA("@fbs3", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY(t, @fbs3, LF(0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY_FLOAT(t, @tap3.fb0, 0.1);
        REQUIRE_PROPERTY_FLOAT(t, @tap3.fb1, 0.2);
        REQUIRE_PROPERTY_FLOAT(t, @tap3.fb2, 0.3);
        REQUIRE_PROPERTY_FLOAT(t, @tap3.fb3, 0.4);
        REQUIRE_PROPERTY_FLOAT(t, @tap3.fb4, 0.5);
        REQUIRE_PROPERTY_FLOAT(t, @tap3.fb5, 0.6);
    }

    SECTION("@fbs4")
    {
        TObj t("fx.tapiir~", LA("@fbs4", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY(t, @fbs4, LF(0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY_FLOAT(t, @tap4.fb0, 0.1);
        REQUIRE_PROPERTY_FLOAT(t, @tap4.fb1, 0.2);
        REQUIRE_PROPERTY_FLOAT(t, @tap4.fb2, 0.3);
        REQUIRE_PROPERTY_FLOAT(t, @tap4.fb3, 0.4);
        REQUIRE_PROPERTY_FLOAT(t, @tap4.fb4, 0.5);
        REQUIRE_PROPERTY_FLOAT(t, @tap4.fb5, 0.6);
    }

    SECTION("@fbs5")
    {
        TObj t("fx.tapiir~", LA("@fbs5", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY(t, @fbs5, LF(0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY_FLOAT(t, @tap5.fb0, 0.1);
        REQUIRE_PROPERTY_FLOAT(t, @tap5.fb1, 0.2);
        REQUIRE_PROPERTY_FLOAT(t, @tap5.fb2, 0.3);
        REQUIRE_PROPERTY_FLOAT(t, @tap5.fb3, 0.4);
        REQUIRE_PROPERTY_FLOAT(t, @tap5.fb4, 0.5);
        REQUIRE_PROPERTY_FLOAT(t, @tap5.fb5, 0.6);
    }

    SECTION("@ins0")
    {
        TObj t("fx.tapiir~", LA("@ins0", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY(t, @ins0, LF(0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY_FLOAT(t, @tap0.in0, 0.1);
        REQUIRE_PROPERTY_FLOAT(t, @tap1.in0, 0.2);
        REQUIRE_PROPERTY_FLOAT(t, @tap2.in0, 0.3);
        REQUIRE_PROPERTY_FLOAT(t, @tap3.in0, 0.4);
        REQUIRE_PROPERTY_FLOAT(t, @tap4.in0, 0.5);
        REQUIRE_PROPERTY_FLOAT(t, @tap5.in0, 0.6);
    }

    SECTION("@ins1")
    {
        TObj t("fx.tapiir~", LA("@ins1", 0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY(t, @ins1, LF(0.1, 0.2, 0.3, 0.4, 0.5, 0.6));

        REQUIRE_PROPERTY_FLOAT(t, @tap0.in1, 0.1);
        REQUIRE_PROPERTY_FLOAT(t, @tap1.in1, 0.2);
        REQUIRE_PROPERTY_FLOAT(t, @tap2.in1, 0.3);
        REQUIRE_PROPERTY_FLOAT(t, @tap3.in1, 0.4);
        REQUIRE_PROPERTY_FLOAT(t, @tap4.in1, 0.5);
        REQUIRE_PROPERTY_FLOAT(t, @tap5.in1, 0.6);
    }
}
