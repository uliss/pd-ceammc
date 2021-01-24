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
#include "conv_pitch2midi.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(PitchToMIDI, conv, pitch2midi)

TEST_CASE("conv.pitch2midi", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            TExt t("conv.pitch2midi", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("alias")
        {
            TExt t("pitch->midi");
        }
    }

    SECTION("convert")
    {
        TExt t("pitch->midi");
        t << "C";
        REQUIRE(t.outputFloatAt(0) == 60);

        t << "C##";
        REQUIRE(t.outputFloatAt(0) == 62);

        t << "A";
        REQUIRE(t.outputFloatAt(0) == 69);

        t << "R";
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.isOutputFloatAt(0));
        REQUIRE(t.outputFloatAt(0) == -1);

        t << LA("R", "R");
        REQUIRE(t.outputListAt(0) == LF(-1, -1));

        t << L();
        REQUIRE(t.outputListAt(0) == L());

        t << LA("C", "H");
        REQUIRE(t.outputListAt(0) == LF(60));
    }
}
