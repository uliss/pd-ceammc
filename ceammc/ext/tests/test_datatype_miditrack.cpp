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
#include "../lib/ceammc_datatypes.h"
#include "../midi/datatype_miditrack.h"

#include "base_extension_test.h"

#include "catch.hpp"

using namespace ceammc;

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

TEST_CASE("DataTypeMidiTrack", "[ceammc::DataTypeMidiTrack]")
{
    SECTION("init")
    {
        DataTypeMidiTrack track;
        REQUIRE(track.type() == data::DATA_MIDI_TRACK);
        REQUIRE(track.eventCount() == 0);
        REQUIRE(track.begin() == track.end());
    }
}
