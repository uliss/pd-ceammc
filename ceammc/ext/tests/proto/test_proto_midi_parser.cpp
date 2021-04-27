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
#include "proto_midi_parser.h"
#include "test_base.h"

using namespace ceammc::midi;

using ivec = std::vector<int>;

TEST_CASE("midi::MidiParser", "[class]")
{
    SECTION("cc")
    {

        MidiParser p;
        ivec data;
        p.setControlChangeFn([&data](int chan, int cc, int val) {
            data = { chan, cc, val };
        });

        MidiParser::Result res;

        res = p.push({ MIDI_CONTROLCHANGE, 10, 100 });
        REQUIRE(res.err == MidiParser::NO_ERROR);
        REQUIRE(res.state == MidiParser::STATE_CONTROLCHANGE);
        REQUIRE(data == ivec { 0, 10, 100 });

        res = p.push({ MIDI_CONTROLCHANGE | 0x3, 10, 100 });
        REQUIRE(res.err == MidiParser::NO_ERROR);
        REQUIRE(res.state == MidiParser::STATE_CONTROLCHANGE);
        REQUIRE(data == ivec { 3, 10, 100 });

        res = p.push({ MIDI_CONTROLCHANGE | 0x3, 0x80, 100 });
        REQUIRE(res.err == MidiParser::ERROR_UNEXPECTED);
        REQUIRE(res.state == MidiParser::STATE_CONTROLCHANGE);

        res = p.push({ MIDI_CONTROLCHANGE | 0x7, 12, 32 });
        REQUIRE(res.err == MidiParser::NO_ERROR);
        REQUIRE(res.state == MidiParser::STATE_CONTROLCHANGE);
        REQUIRE(data == ivec { 7, 12, 32 });
    }
}
