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
#include "midi_modus.h"
#include "test_midi_base.h"

PD_COMPLETE_TEST_SETUP(MidiModus, midi, modus)

TEST_CASE("midi.modus", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("midi.modus");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @root, "C");
            REQUIRE_PROPERTY(t, @scale, "chromatic");
        }

        SECTION("args")
        {
            TExt t0("midi.modus", 1);
            REQUIRE_PROPERTY(t0, @root, 1);

            TExt t1("midi.modus", "C##");
            REQUIRE_PROPERTY(t1, @root, "C##");
        }
    }

}
