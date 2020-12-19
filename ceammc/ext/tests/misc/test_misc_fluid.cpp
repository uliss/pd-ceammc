/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "catch.hpp"
#include "fluid.h"
#include "test_base.h"
#include "test_external.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(Fluid, misc, fluid)

#define SF_DIR PROJECT_SOURCE_DIR "/ceammc/extra/fluidsynth/fluidsynth/sf2"
#define SF_NAME "VintageDreamsWaves-v2.sf2"
#define SF_FILE SF_DIR "/" SF_NAME

TEST_CASE("misc.fluid~", "[externals]")
{
    pd_test_init();

    SECTION("main")
    {
        TestExtFluid t("fluid~");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE(t.numOutputChannels() == 2);
        REQUIRE_PROPERTY(t, @version, "2.0.0");
        REQUIRE_PROPERTY(t, @sf, "");
        REQUIRE_PROPERTY_LIST(t, @soundfonts, L());
        REQUIRE_PROPERTY_FLOAT(t, @reverb_room, 0.2);
        REQUIRE_PROPERTY_FLOAT(t, @reverb_damp, 0.);
        REQUIRE_PROPERTY_FLOAT(t, @reverb_width, 0.5);
        REQUIRE_PROPERTY_FLOAT(t, @reverb_level, 0.9);

        REQUIRE_PROPERTY_FLOAT(t, @gain, 0.6);
        REQUIRE_PROPERTY(t, @poly, 256);

        REQUIRE_PROPERTY(t, @avoices, 0);
        REQUIRE_PROPERTY(t, @bufsize, 64);
    }

    SECTION("load")
    {
        TestExtFluid t("fluid~", LA("@sf", SF_FILE));
        REQUIRE_PROPERTY(t, @sf, SF_FILE);
    }
}
