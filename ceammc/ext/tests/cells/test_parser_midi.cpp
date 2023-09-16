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
#include "catch.hpp"
#include "lex/parser_midi.h"
#include "test_base.h"

using namespace ceammc;

#define REQUIRE_MIDI(data, byte)                            \
    {                                                       \
        std::uint8_t res = 0;                               \
        REQUIRE(parser::parse_midi_byte_hex(A(data), res)); \
        REQUIRE(res == byte);                               \
    }

TEST_CASE("parser_midi", "[ceammc::lex]")
{
    test::pdPrintToStdError();

    SECTION("parse hex")
    {
        REQUIRE_MIDI(A(0.0), 0);
        REQUIRE_MIDI(A(9), 0x9);
        REQUIRE_MIDI(A(10), 0x10);
        REQUIRE_MIDI(A("0xEF"), 0xEF);
        REQUIRE_MIDI(A("0x12"), 0x12);
        REQUIRE_MIDI(A("0xF1"), 0xF1);
        REQUIRE_MIDI(A("FEh"), 0xFE);
        REQUIRE_MIDI(A("10h"), 0x10);
    }
}
