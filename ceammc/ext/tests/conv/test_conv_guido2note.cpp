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
#include "lex/parser_guido.h"

// PD_COMPLETE_TEST_SETUP(ConvNote2Guido, conv, note2guido)

using namespace ceammc;

//#define N2G(obj, in, out)                      \
//    {                                          \
//        WHEN_SEND_FLOAT_TO(0, obj, float(in)); \
//        REQUIRE_SYMBOL_AT_OUTLET(0, obj, out); \
//    }

TEST_CASE("conv.guido2note", "[externals]")
{
    //    pd_test_init();

    SECTION("parser")
    {
        using namespace ceammc::parser;

        auto pc = music::PitchClass::C;
        auto dur = music::Duration();
        int oct = -100;

        REQUIRE_FALSE(parse_guido_note(nullptr, pc, oct, dur));
        REQUIRE_FALSE(parse_guido_note("", pc, oct, dur));
        REQUIRE(parse_guido_note("c", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::C);
        REQUIRE(parse_guido_note("d", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::D);
        REQUIRE(parse_guido_note("e", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::E);
        REQUIRE(parse_guido_note("f", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::F);
        REQUIRE(parse_guido_note("g", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::G);
        REQUIRE(parse_guido_note("a", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::A);
        REQUIRE(parse_guido_note("b", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::B);
        REQUIRE(parse_guido_note("h", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::B);
        REQUIRE(oct == -100);
        REQUIRE_FALSE(parse_guido_note("C", pc, oct, dur));
        REQUIRE_FALSE(parse_guido_note("C..", pc, oct, dur));

        REQUIRE(parse_guido_note("do", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::C);
        REQUIRE(parse_guido_note("re", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::D);
        REQUIRE(parse_guido_note("mi", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::E);
        REQUIRE(parse_guido_note("fa", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::F);
        REQUIRE(parse_guido_note("sol", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::G);
        REQUIRE(parse_guido_note("la", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::A);
        REQUIRE(parse_guido_note("si", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::B);
        REQUIRE(parse_guido_note("ti", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::B);

        REQUIRE(parse_guido_note("cis", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::Cs);
        REQUIRE(parse_guido_note("dis", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::Ds);
        REQUIRE(parse_guido_note("fis", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::Fs);
        REQUIRE(parse_guido_note("gis", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::Gs);
        REQUIRE(parse_guido_note("ais", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::As);

        REQUIRE(parse_guido_note("d#", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::Ds);
        REQUIRE(parse_guido_note("d##", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::Dss);
        REQUIRE(parse_guido_note("d&", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::Df);
        REQUIRE(parse_guido_note("d&&", pc, oct, dur));
        REQUIRE(pc == music::PitchClass::Dff);

        REQUIRE_FALSE(parse_guido_note("c-10", pc, oct, dur));
        REQUIRE_FALSE(parse_guido_note("c10", pc, oct, dur));
        REQUIRE(parse_guido_note("c-5", pc, oct, dur));
        REQUIRE(oct == -5);
        REQUIRE(parse_guido_note("c-4", pc, oct, dur));
        REQUIRE(oct == -4);
        REQUIRE(parse_guido_note("c-3", pc, oct, dur));
        REQUIRE(oct == -3);
        REQUIRE(parse_guido_note("c-2", pc, oct, dur));
        REQUIRE(oct == -2);
        REQUIRE(parse_guido_note("c-1", pc, oct, dur));
        REQUIRE(oct == -1);
        REQUIRE(parse_guido_note("c-0", pc, oct, dur));
        REQUIRE(oct == 0);
        REQUIRE(parse_guido_note("c0", pc, oct, dur));
        REQUIRE(oct == 0);
        REQUIRE(parse_guido_note("c1", pc, oct, dur));
        REQUIRE(oct == 1);
        REQUIRE(parse_guido_note("c2", pc, oct, dur));
        REQUIRE(oct == 2);
        REQUIRE(parse_guido_note("c3", pc, oct, dur));
        REQUIRE(oct == 3);
        REQUIRE(parse_guido_note("c4", pc, oct, dur));
        REQUIRE(oct == 4);
        REQUIRE(parse_guido_note("c5", pc, oct, dur));
        REQUIRE(oct == 5);
    }
}
