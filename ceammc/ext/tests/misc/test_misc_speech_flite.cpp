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
#include "speech_flite.h"
#include "test_external.h"

#include "test_catch2.hpp"

PD_COMPLETE_TEST_SETUP(SpeechFlite, misc_speech, flite)

TEST_CASE("speech.flite", "[externals]")
{
    pd_test_init();
    setTestSampleRate();

    SECTION("construct")
    {
        TObj t("speech.flite");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);

        REQUIRE_PROPERTY_LIST(t, @array, LA(""));
        REQUIRE_PROPERTY_LIST(t, @voice, LA("kal16"));
    }

    SECTION("voice")
    {
        TObj t("speech.flite");
        REQUIRE_PROPERTY_LIST(t, @voice, LA("kal16"));
    }

    SECTION("render")
    {
        auto cnv = PureData::instance().findCanvas("test_canvas");
        ArrayPtr arr = cnv->createArray("array2", 10);

        TObj t("speech.flite", LA("@array", "array2"));
        REQUIRE_PROPERTY_LIST(t, @array, LA("array2"));
        WHEN_SEND_FLOAT_TO(0, t, 1);
    }
}
