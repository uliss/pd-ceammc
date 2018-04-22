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
#include "../misc/speech_flite.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<SpeechFlite> FliteTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("speech.flite", "[externals]")
{
    setup_misc_speech_flite();
    setTestSampleRate();

    SECTION("construct")
    {
        FliteTest t("speech.flite");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);

        REQUIRE_PROPERTY_LIST(t, @array, L1(""));
        REQUIRE_PROPERTY_LIST(t, @voice, L1("kal16"));
    }

    SECTION("voice")
    {
        FliteTest t("speech.flite");
        REQUIRE_PROPERTY_LIST(t, @voice, L1("kal16"));
    }

    SECTION("render")
    {
        ArrayPtr arr = cnv->createArray("array2", 10);

        FliteTest t("speech.flite", L2("@array", "array2"));
        REQUIRE_PROPERTY_LIST(t, @array, L1("array2"));
        WHEN_SEND_FLOAT_TO(0, t, 1);
    }
}
