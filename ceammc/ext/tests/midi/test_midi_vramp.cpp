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
#include "midi_velocity_ramp.h"
#include "test_midi_base.h"

PD_COMPLETE_TEST_SETUP(MidiVelocityRamp, midi, vel_ramp)

TEST_CASE("midi.vramp", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("midi.vramp");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);

            // clang-format off
            REQUIRE_PROPERTY(t, @n, 0);
            REQUIRE_PROPERTY(t, @from, 127);
            REQUIRE_PROPERTY(t, @to, 127);
            REQUIRE_PROPERTY(t, @auto, 0.);
        }

        SECTION("args 1")
        {
            TExt t("midi.vramp", LF(10));

            REQUIRE_PROPERTY(t, @n, 0);
            REQUIRE_PROPERTY(t, @from, 10);
            REQUIRE_PROPERTY(t, @to, 127);
            REQUIRE_PROPERTY(t, @auto, 0.);
        }

        SECTION("args 2")
        {
            TExt t("midi.vramp", LF(10, 20));

            REQUIRE_PROPERTY(t, @n, 0);
            REQUIRE_PROPERTY(t, @from, 10);
            REQUIRE_PROPERTY(t, @to, 20);
            REQUIRE_PROPERTY(t, @auto, 0.);
        }

        SECTION("args 3")
        {
            TExt t("midi.vramp", LF(10, 20, 30));

            REQUIRE_PROPERTY(t, @n, 30);
            REQUIRE_PROPERTY(t, @from, 10);
            REQUIRE_PROPERTY(t, @to, 20);
            REQUIRE_PROPERTY(t, @auto, 0.);
            // clang-format on
        }
    }

    SECTION("list @n 2")
    {
        TObj t("midi.vramp", LF(10, 20, 2));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LF(60, 127));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 10));

        WHEN_SEND_LIST_TO(0, t, LF(60, 0));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 0));

        WHEN_SEND_LIST_TO(0, t, LF(61, 127));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(61, 15));

        WHEN_SEND_LIST_TO(0, t, LF(61, 0));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(61, 0));

        WHEN_SEND_LIST_TO(0, t, LF(62, 127));
        REQUIRE_BANG_AT_OUTLET(1, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 20));

        WHEN_SEND_LIST_TO(0, t, LF(62, 0));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 0));

        WHEN_SEND_LIST_TO(0, t, LF(63, 127));
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(63, 20));

        WHEN_SEND_LIST_TO(0, t, LF(63, 0));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(63, 0));

        t.m_reset(&s_, {});
        WHEN_SEND_LIST_TO(0, t, LF(64, 127));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 10));

        t.setProperty("@n", LF(0));
        WHEN_SEND_LIST_TO(0, t, LF(65, 127));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(65, 10));
        WHEN_SEND_LIST_TO(0, t, LF(66, 127));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(66, 10));
        WHEN_SEND_LIST_TO(0, t, LF(67, 0));
        REQUIRE_LIST_AT_OUTLET(0, t, LF(67, 0));
    }

    SECTION("float")
    {
        TObj t("midi.vramp", LF(30, 10, 2));

        WHEN_SEND_FLOAT_TO(0, t, 60);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(60, 30));

        WHEN_SEND_FLOAT_TO(0, t, 61);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(61, 20));

        WHEN_SEND_FLOAT_TO(0, t, -62);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(62, 0));

        WHEN_SEND_FLOAT_TO(0, t, 63);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(63, 10));

        WHEN_SEND_FLOAT_TO(0, t, 64);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(64, 10));
    }
}
