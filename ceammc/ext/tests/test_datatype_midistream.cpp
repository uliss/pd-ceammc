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
#include "../midi/datatype_midistream.h"
#include "test_base.h"

#include "catch.hpp"

using namespace ceammc;

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

TEST_CASE("DataTypeMidiStream", "[ceammc::DataTypeMidiStream]")
{
    test::pdPrintToStdError();

    SECTION("init")
    {
        DataTypeMidiStream midi;
        REQUIRE(midi.trackCount() == 1);
        REQUIRE(midi.tempo() == 120);
        REQUIRE(midi.filename() == gensym(""));
        REQUIRE(midi.midifile() != 0);
        REQUIRE(midi.totalTimeInSeconds() == 0);
        REQUIRE(midi.totalTimeInTicks() == 0);
        REQUIRE(midi.totalTimeInQuarters() == 0);

        DataTypeMidiStream midi2(TEST_DATA_DIR "/midi/test_01.mid");
        REQUIRE(midi2.is_open());
        REQUIRE(midi2.tempo() == 480);
        REQUIRE(midi2.trackCount() == 1);
        REQUIRE(midi2.filename()->s_name == std::string("test_01.mid"));
        REQUIRE(midi2.totalTimeInTicks() == 1896);
        REQUIRE(midi2.totalTimeInQuarters() == Approx(3.95));
        REQUIRE(midi2.totalTimeInSeconds() == Approx(1.975));
    }
}
