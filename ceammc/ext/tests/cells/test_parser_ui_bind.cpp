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
#include "lex/parser_ui_bind.h"
#include "test_common.h"

using namespace ceammc;
using namespace ceammc::parser;

TEST_CASE("parser_ui_bind", "[ceammc::ceammc_units]")
{
    test::pdPrintToStdError();
    UIBindOptions opts;

    SECTION("MIDI")
    {
        REQUIRE(parse_ui_bind("cc[0]", opts));
        REQUIRE(opts.midi_chan == 0);
        REQUIRE(opts.midi_param == 0);
        REQUIRE(opts.midi_value == 0);
        REQUIRE(opts.cmp == UI_BIND_CMP_NONE);
        REQUIRE(opts.type == UI_BIND_MIDI_CC);
        REQUIRE(opts.checkMidi(0, 0, 100));

        REQUIRE(parse_ui_bind("cc[0]>63", opts));
        REQUIRE(opts.midi_chan == 0);
        REQUIRE(opts.midi_param == 0);
        REQUIRE(opts.midi_value == 63);
        REQUIRE(opts.cmp == UI_BIND_CMP_GT);
        REQUIRE(opts.type == UI_BIND_MIDI_CC);

        REQUIRE(parse_ui_bind("cc[0]>=63", opts));
        REQUIRE(opts.midi_chan == 0);
        REQUIRE(opts.midi_param == 0);
        REQUIRE(opts.midi_value == 63);
        REQUIRE(opts.cmp == UI_BIND_CMP_GE);
        REQUIRE(opts.type == UI_BIND_MIDI_CC);

        REQUIRE(parse_ui_bind("cc[1]<120", opts));
        REQUIRE(opts.midi_chan == 0);
        REQUIRE(opts.midi_param == 1);
        REQUIRE(opts.midi_value == 120);
        REQUIRE(opts.cmp == UI_BIND_CMP_LT);
        REQUIRE(opts.type == UI_BIND_MIDI_CC);

        REQUIRE(parse_ui_bind("cc[1:0]<120", opts));
        REQUIRE(opts.midi_chan == 0);
        REQUIRE(opts.midi_param == 1);
        REQUIRE(opts.midi_value == 120);
        REQUIRE(opts.cmp == UI_BIND_CMP_LT);
        REQUIRE(opts.type == UI_BIND_MIDI_CC);

        REQUIRE(parse_ui_bind("cc[1:2]<=120", opts));
        REQUIRE(opts.midi_chan == 2);
        REQUIRE(opts.midi_param == 1);
        REQUIRE(opts.midi_value == 120);
        REQUIRE(opts.cmp == UI_BIND_CMP_LE);
        REQUIRE(opts.type == UI_BIND_MIDI_CC);

        REQUIRE(parse_ui_bind("cc[20:16]==120", opts));
        REQUIRE(opts.midi_chan == 16);
        REQUIRE(opts.midi_param == 20);
        REQUIRE(opts.midi_value == 120);
        REQUIRE(opts.cmp == UI_BIND_CMP_EQ);
        REQUIRE(opts.type == UI_BIND_MIDI_CC);

        REQUIRE(parse_ui_bind("note[20:12]==14", opts));
        REQUIRE(opts.midi_chan == 12);
        REQUIRE(opts.midi_param == 20);
        REQUIRE(opts.midi_value == 14);
        REQUIRE(opts.cmp == UI_BIND_CMP_EQ);
        REQUIRE(opts.type == UI_BIND_MIDI_NOTE);

        REQUIRE(parse_ui_bind("pgm[2:12]", opts));
        REQUIRE(opts.midi_chan == 12);
        REQUIRE(opts.midi_param == 2);
        REQUIRE(opts.midi_value == 14);
        REQUIRE(opts.cmp == UI_BIND_CMP_EQ);
        REQUIRE(opts.type == UI_BIND_MIDI_PGM);
    }

    SECTION("keys")
    {
        REQUIRE(parse_ui_bind("key=32", opts));
        REQUIRE(opts.midi_chan == 0);
        REQUIRE(opts.midi_param == 0);
        REQUIRE(opts.midi_value == 0);
        REQUIRE(opts.key_mode == 0);
        REQUIRE(opts.key_code == 32);
        REQUIRE(opts.cmp == UI_BIND_CMP_EQ);
        REQUIRE(opts.type == UI_BIND_KEY_CODE);

        REQUIRE(parse_ui_bind("key[ctl]=32", opts));
        REQUIRE(opts.midi_chan == 0);
        REQUIRE(opts.midi_param == 0);
        REQUIRE(opts.midi_value == 0);
        REQUIRE(opts.key_mode == UI_BIND_MODE_CTL);
        REQUIRE(opts.cmp == UI_BIND_CMP_EQ);
        REQUIRE(opts.type == UI_BIND_KEY_CODE);

        REQUIRE(parse_ui_bind("key[ctl+alt]=32", opts));
        REQUIRE(opts.midi_chan == 0);
        REQUIRE(opts.midi_param == 0);
        REQUIRE(opts.midi_value == 0);
        REQUIRE(opts.key_mode & UI_BIND_MODE_CTL);
        REQUIRE(opts.key_mode & UI_BIND_MODE_ALT);
        REQUIRE(opts.cmp == UI_BIND_CMP_EQ);
        REQUIRE(opts.type == UI_BIND_KEY_CODE);
    }

    SECTION("keyname")
    {
        REQUIRE(parse_ui_bind("keyname=Space", opts));
        REQUIRE(opts.midi_chan == 0);
        REQUIRE(opts.midi_param == 0);
        REQUIRE(opts.midi_value == 0);
        REQUIRE(opts.key_mode == 0);
        REQUIRE(opts.key_code == 0);
        REQUIRE(opts.key_name == std::string("Space"));
        REQUIRE(opts.cmp == UI_BIND_CMP_EQ);
        REQUIRE(opts.type == UI_BIND_KEY_NAME);
    }
}
