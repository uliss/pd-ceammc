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
#include "ceammc_music_theory_tempo.h"
#include "lex/parser_music.h"
#include "test_base.h"

using namespace ceammc;

TEST_CASE("parser_music", "[ceammc::ceammc_units]")
{
    test::pdPrintToStdError();

    SECTION("bpm")
    {
        SECTION("str")
        {
            using namespace ceammc::parser;
            using namespace ceammc::music;
            Tempo t;

            REQUIRE(parse_tempo("120", t));
            REQUIRE(t.bpm() == 120);
            REQUIRE(t.beatDuration().ratio() == 0.25);
            REQUIRE(t.beatDurationMs() == 500);
            //            REQUIRE(bpm.beatPeriodSamp(10000) == 5000);

            REQUIRE(parse_tempo("120.5", t));
            REQUIRE(t.bpm() == 120.5);
            REQUIRE(t.beatDuration().ratio() == 0.25);

            REQUIRE(parse_tempo("120bpm", t));
            REQUIRE(t.bpm() == 120);
            REQUIRE(t.beatDuration().ratio() == 0.25);

            REQUIRE(parse_tempo("0", t));
            REQUIRE(t.bpm() == 0);
            REQUIRE(t.beatDuration().ratio() == 0.25);
            REQUIRE(t.beatDurationMs() == 0);
            //            REQUIRE(bpm.beatPeriodSamp(44100) == 0);

            REQUIRE(parse_tempo("144bpm", t));
            REQUIRE(t.bpm() == 144);
            REQUIRE(t.beatDuration().ratio() == 0.25);

            REQUIRE(parse_tempo("50.25_bpm", t));
            REQUIRE(t.bpm() == 50.25);
            REQUIRE(t.beatDuration().ratio() == 0.25);

            REQUIRE(parse_tempo("50|1_bpm", t));
            REQUIRE(t.bpm() == 50);
            REQUIRE(t.beatDuration().ratio() == 1);

            REQUIRE(parse_tempo("50|2_bpm", t));
            REQUIRE(t.bpm() == 50);
            REQUIRE(t.beatDuration().ratio() == 0.5);

            REQUIRE(parse_tempo("50|2._bpm", t));
            REQUIRE(t.bpm() == 50);
            REQUIRE(t.beatDuration().ratio() == 0.75);

            REQUIRE(parse_tempo("50|4_bpm", t));
            REQUIRE(t.bpm() == 50);
            REQUIRE(t.beatDuration().ratio() == 0.25);

            REQUIRE(parse_tempo("50|1/4._bpm", t));
            REQUIRE(t.bpm() == 50);
            REQUIRE(t.beatDuration().ratio() == 0.375);

            REQUIRE(parse_tempo("50|16_bpm", t));
            REQUIRE(t.beatDuration().ratio() == 1 / 16.0);

            REQUIRE(parse_tempo("0", t));
            REQUIRE(t.beatDurationMs() == 0);
            //            REQUIRE(t.beatDurationMs(-100) == -100);

            REQUIRE(parse_tempo("60|4.bpm", t));

            REQUIRE(parse_tempo("60|1/4bpm", t));

            REQUIRE_FALSE(parse_tempo("00", t));
            REQUIRE_FALSE(parse_tempo("+12", t));
            REQUIRE_FALSE(parse_tempo("-12", t));
            REQUIRE_FALSE(parse_tempo("", t));
        }

        SECTION("float")
        {
            using namespace ceammc::parser;
            using namespace ceammc::music;
            Tempo t;

            REQUIRE(parse_tempo(A(120.5), t));
            REQUIRE(t.bpm() == 120.5);

            REQUIRE(parse_tempo(A(0.0), t));
            REQUIRE(t.bpm() == 0);

            REQUIRE_FALSE(parse_tempo(A(-1), t));
        }

        SECTION("list")
        {
            using namespace ceammc::parser;
            using namespace ceammc::music;

            std::vector<Tempo> res;

            REQUIRE(parse_tempo(LA(1, "23", "44.5_bpm", "unknown"), res) == 3);
            REQUIRE(res.size() == 3);
            REQUIRE(res[0].bpm() == 1);
            REQUIRE(res[1].bpm() == 23);
            REQUIRE(res[2].bpm() == 44.5);
        }

        SECTION("N")
        {
            using namespace ceammc::parser;
            using namespace ceammc::music;

            auto r0 = parser::parse_tempo_n<3>(L(), { 0, 4 });
            REQUIRE(r0[0] == Tempo { 0, 4 });
            REQUIRE(r0[1] == Tempo { 0, 4 });
            REQUIRE(r0[2] == Tempo { 0, 4 });

            r0 = parser::parse_tempo_n<3>(LA(120), Tempo { 61, 2 });
            REQUIRE(r0[0] == Tempo { 120, 4 });
            REQUIRE(r0[1] == Tempo { 61, 2 });
            REQUIRE(r0[2] == Tempo { 61, 2 });

            r0 = parser::parse_tempo_n<3>(LA(120, "140.5|8bpm"), Tempo { 62, 2 });
            REQUIRE(r0[0] == Tempo { 120, 4 });
            REQUIRE(r0[1] == Tempo { 140.5, 8 });
            REQUIRE(r0[2] == Tempo { 62, 2 });

            r0 = parser::parse_tempo_n<3>(LA(120, "140.5bpm", "100|16_bpm"), Tempo { 62, 128 });
            REQUIRE(r0[0] == Tempo { 120, 4 });
            REQUIRE(r0[1] == Tempo { 140.5, 4 });
            REQUIRE(r0[2] == Tempo { 100, 16 });

            r0 = parser::parse_tempo_n<3>(LA(120, "???", "100|32_bpm"), Tempo { 63, 2 });
            REQUIRE(r0[0] == Tempo { 120, 4 });
            REQUIRE(r0[1] == Tempo { 63, 2 });
            REQUIRE(r0[2] == Tempo { 100, 32 });
        }
    }

    SECTION("spn")
    {
        SECTION("str")
        {
            using namespace ceammc::parser;
            using namespace ceammc::music;
            Spn spn;

            REQUIRE(parse_spn("C4", spn));
            REQUIRE(spn.pitch() == PitchClass::C);
            REQUIRE(spn.octave() == Octave(4));
            REQUIRE(spn.deviation() == 0);
            REQUIRE(!spn.isRest());

            REQUIRE(parse_spn("C#4", spn));
            REQUIRE(spn.pitch() == PitchClass::Cs);
            REQUIRE(spn.octave() == Octave(4));
            REQUIRE(spn.deviation() == 0);

            REQUIRE(parse_spn("C##4", spn));
            REQUIRE(spn.pitch() == music::PitchClass::Css);
            REQUIRE(spn.octave() == Octave(4));
            REQUIRE(spn.deviation() == 0);

            REQUIRE(parse_spn("Cb4", spn));
            REQUIRE(spn.pitch() == music::PitchClass::Cf);
            REQUIRE(spn.octave() == Octave(4));
            REQUIRE(spn.deviation() == 0);

            REQUIRE(parse_spn("Cbb4", spn));
            REQUIRE(spn.pitch() == music::PitchClass::Cff);
            REQUIRE(spn.octave() == Octave(4));
            REQUIRE(spn.deviation() == 0);

            REQUIRE(parse_spn("C4(+50c)", spn));
            REQUIRE(spn.pitch() == music::PitchClass::C);
            REQUIRE(spn.octave() == Octave(4));
            REQUIRE(spn.deviation() == 50);
            REQUIRE(!spn.isRest());

            REQUIRE(parse_spn("C4(+0c)", spn));
            REQUIRE(spn.pitch() == music::PitchClass::C);
            REQUIRE(spn.octave() == Octave(4));
            REQUIRE(spn.deviation() == 0);
            REQUIRE(!spn.isRest());

            REQUIRE(parse_spn("C4(+01c)", spn));
            REQUIRE(spn.pitch() == music::PitchClass::C);
            REQUIRE(spn.octave() == Octave(4));
            REQUIRE(spn.deviation() == 1);
            REQUIRE(!spn.isRest());

            REQUIRE(parse_spn("C4(+05c)", spn));
            REQUIRE(spn.pitch() == music::PitchClass::C);
            REQUIRE(spn.octave() == Octave(4));
            REQUIRE(spn.deviation() == 5);
            REQUIRE(!spn.isRest());

            REQUIRE(parse_spn("C4(-25c)", spn));
            REQUIRE(spn.pitch() == music::PitchClass::C);
            REQUIRE(spn.octave() == Octave(4));
            REQUIRE(spn.deviation() == -25);
            REQUIRE(!spn.isRest());

            REQUIRE(parse_spn("A4", spn));
            REQUIRE(spn.pitch() == music::PitchClass::A);
            REQUIRE(spn.asMidi() == 69);
            REQUIRE(parse_spn("B4", spn));
            REQUIRE(spn.asMidi() == 71);
            REQUIRE(parse_spn("D4", spn));
            REQUIRE(spn.asMidi() == 62);
            REQUIRE(parse_spn("E4", spn));
            REQUIRE(spn.asMidi() == 64);
            REQUIRE(parse_spn("F4", spn));
            REQUIRE(spn.asMidi() == 65);
            REQUIRE(parse_spn("G4", spn));
            REQUIRE(spn.asMidi() == 67);

            REQUIRE(parse_spn("A4", spn));
            REQUIRE(spn.asMidi() == 69);
            REQUIRE(parse_spn("B4", spn));
            REQUIRE(spn.asMidi() == 71);
            REQUIRE(parse_spn("H4", spn));
            REQUIRE(spn.asMidi() == 71);
            REQUIRE(parse_spn("D4", spn));
            REQUIRE(spn.asMidi() == 62);
            REQUIRE(parse_spn("E4", spn));
            REQUIRE(spn.asMidi() == 64);
            REQUIRE(parse_spn("F4", spn));
            REQUIRE(spn.asMidi() == 65);
            REQUIRE(parse_spn("G4", spn));
            REQUIRE(spn.asMidi() == 67);

            REQUIRE(parse_spn("C9", spn));
            REQUIRE(spn.asMidi() == 120);

            REQUIRE(parse_spn("C0", spn));
            REQUIRE(spn.asMidi() == 12);

            REQUIRE(parse_spn("C-1", spn));
            REQUIRE(spn.asMidi() == 0);

            REQUIRE(parse_spn("C", spn));
            REQUIRE(spn.asMidi() == 60);

            REQUIRE(parse_spn("C", spn));
            REQUIRE(spn.asMidi(5) == 72);

            REQUIRE(parse_spn("Db", spn));
            REQUIRE(spn.asMidi() == 61);

            REQUIRE(parse_spn("Dbb", spn));
            REQUIRE(spn.asMidi() == 60);

            REQUIRE(parse_spn("Dbb^", spn));
            REQUIRE(spn.asMidi() == 72);

            REQUIRE(parse_spn("Dbb^^", spn));
            REQUIRE(spn.asMidi() == 84);

            REQUIRE(parse_spn("Dbb_", spn));
            REQUIRE(spn.asMidi() == 48);

            REQUIRE(parse_spn("D#____", spn));
            REQUIRE(spn.asMidi() == 15);

            REQUIRE(!parse_spn("C10", spn));
            REQUIRE(!parse_spn("C-2", spn));
        }

        SECTION("atom")
        {
            using namespace ceammc::parser;
            music::Spn spn;

            REQUIRE(!parse_spn(A(100), spn));

            REQUIRE(parse_spn("C#5", spn));
            REQUIRE(spn.toMidi().isOk());
            REQUIRE(spn.toMidi().value() == 73);
        }

        SECTION("next")
        {
            using namespace ceammc::parser;
            using namespace ceammc::music;

            auto pitches = parse_spn_n<6>(LA("C", "D", "E", "F", "G", "C"));
            REQUIRE(std::none_of(pitches.begin(), pitches.end(), [](const music::Spn& s) { return s.isAbs(); }));

            //            Spn::setAbsOctaves(pitches.begin(), pitches.end());

            for (size_t i = 0; i + 1 < pitches.size(); i++) {
                auto& p0 = pitches[i];
                auto& p1 = pitches[i + 1];
                p1.octave().setAbsOctave(p0.octave());
            }

            REQUIRE(std::none_of(pitches.begin(), pitches.end(), [](const music::Spn& s) { return s.isAbs(); }));

            pitches[0].setOctave(Octave(5));
            for (size_t i = 0; i + 1 < pitches.size(); i++) {
                auto& p0 = pitches[i];
                auto& p1 = pitches[i + 1];
                p1.octave().setAbsOctave(p0.octave());
            }

            REQUIRE(std::all_of(pitches.begin(), pitches.end(), [](const music::Spn& s) { return s.isAbs(); }));
            REQUIRE(pitches[0].octave() == Octave(5));
            REQUIRE(pitches[1].octave() == Octave(5));
            REQUIRE(pitches[2].octave() == Octave(5));
            REQUIRE(pitches[3].octave() == Octave(5));
            REQUIRE(pitches[4].octave() == Octave(5));
            REQUIRE(pitches[5].octave() == Octave(5));

            pitches = parse_spn_n<6>(LA("C", "A", "F", "F^", "F__", "C7"));
            REQUIRE(pitches[0].octave().type() == OCTAVE_REL);
            REQUIRE(pitches[1].octave().type() == OCTAVE_REL);
            REQUIRE(pitches[2].octave().type() == OCTAVE_REL);
            REQUIRE(pitches[3].octave().type() == OCTAVE_REL);
            REQUIRE(pitches[4].octave().type() == OCTAVE_REL);
            REQUIRE(pitches[5].octave().type() == OCTAVE_ABS);

            pitches[0].setOctave(Octave(5));
            for (size_t i = 0; i + 1 < pitches.size(); i++) {
                auto& p0 = pitches[i];
                auto& p1 = pitches[i + 1];
                p1.octave().setAbsOctave(p0.octave());
            }

            REQUIRE(pitches[0].octave() == Octave(5));
            REQUIRE(pitches[1].octave() == Octave(5));
            REQUIRE(pitches[2].octave() == Octave(5));
            REQUIRE(pitches[3].octave() == Octave(6));
            REQUIRE(pitches[4].octave() == Octave(4));
            REQUIRE(pitches[5].octave() == Octave(7));
        }
    }

    SECTION("notation")
    {
        SECTION("str")
        {
            using namespace ceammc::parser;
            using namespace ceammc::music;
            Notation n;

            REQUIRE(parse_notation("C", n));
            REQUIRE(!n.isRest());
            REQUIRE(n.asMidi() == 60);
            REQUIRE(n.duration().ratio() == 0.25);
            REQUIRE(n.duration().numerator() == 1);
            REQUIRE(n.duration().division() == 4);
            REQUIRE(!n.isAbsPitch());
            REQUIRE(n.timeMs({}) == 1000);
            REQUIRE(n.timeMs(Tempo(120, 4)) == 500);

            REQUIRE(parse_notation("C3", n));
            REQUIRE(n.asMidi() == 48);
            REQUIRE(n.duration().numerator() == 1);
            REQUIRE(n.duration().division() == 4);
            REQUIRE(n.duration().ratio() == 0.25);

            REQUIRE(parse_notation("C#1|4", n));
            REQUIRE(n.asMidi() == 25);
            REQUIRE(n.isAbsPitch());
            REQUIRE(n.octave() == Octave(1));
            REQUIRE(n.duration().numerator() == 1);
            REQUIRE(n.duration().division() == 4);
            REQUIRE(n.duration().ratio() == 0.25);
            REQUIRE(n.timeMs({}) == 1000);

            REQUIRE(parse_notation("Cb5|2.", n));
            REQUIRE(n.asMidi() == 71);
            REQUIRE(n.octave() == Octave(5));
            REQUIRE(n.duration().numerator() == 1);
            REQUIRE(n.duration().division() == 2);
            REQUIRE(n.duration().dots() == 1);
            REQUIRE(n.duration().ratio() == 0.75);

            REQUIRE(parse_notation("Cb5|2..", n));
            REQUIRE(n.asMidi() == 71);
            REQUIRE(n.octave() == Octave(5));
            REQUIRE(n.duration().numerator() == 1);
            REQUIRE(n.duration().division() == 2);
            REQUIRE(n.duration().dots() == 2);
            REQUIRE(n.duration().ratio() == 0.875);

            REQUIRE(parse_notation("Cbb5|2...", n));
            REQUIRE(n.asMidi() == 70);
            REQUIRE(n.octave() == Octave(5));
            REQUIRE(n.duration().numerator() == 1);
            REQUIRE(n.duration().division() == 2);
            REQUIRE(n.duration().dots() == 3);
            REQUIRE(n.duration().ratio() == 0.9375);

            REQUIRE(parse_notation("D|256", n));
            REQUIRE(n.duration().numerator() == 1);
            REQUIRE(n.duration().division() == 256);

            REQUIRE(parse_notation("D|2/3", n));
            REQUIRE(n.duration().numerator() == 2);
            REQUIRE(n.duration().division() == 3);
            REQUIRE(!n.isAbsPitch());

            REQUIRE(parse_notation("D|1/4", n));
            REQUIRE(n.duration().numerator() == 1);
            REQUIRE(n.duration().division() == 4);
            REQUIRE(n.duration().ratio() == 0.25);
            REQUIRE(n.timeMs({}) == 1000);

            REQUIRE(parse_notation("D|1/4.", n));
            REQUIRE(n.duration().numerator() == 1);
            REQUIRE(n.duration().division() == 4);
            REQUIRE(n.duration().dots() == 1);
            REQUIRE(n.duration().ratio() == 0.375);
            REQUIRE(n.timeMs({}) == 1500);

            REQUIRE(parse_notation("C(+50c)|5/4", n));
            REQUIRE(n.asMidi() == 60.5);
            REQUIRE(n.duration().numerator() == 5);
            REQUIRE(n.duration().division() == 4);
            REQUIRE(n.duration().ratio() == 1.25);

            REQUIRE(parse_notation("C(+50c)|2/4.", n));
            REQUIRE(n.asMidi() == 60.5);
            REQUIRE(n.duration().numerator() == 2);
            REQUIRE(n.duration().division() == 4);
            REQUIRE(n.duration().dots() == 1);
            REQUIRE(!n.isAbsPitch());

            REQUIRE(parse_notation("C(+25c)|2/4.", n));
            REQUIRE(n.asMidi() == 60.25);
            REQUIRE(n.duration().numerator() == 2);
            REQUIRE(n.duration().division() == 4);
            REQUIRE(n.duration().dots() == 1);
            REQUIRE(!n.isAbsPitch());

            REQUIRE(parse_notation("C(+12c)|2/4.", n));
            REQUIRE(n.asMidi() == Approx(60.12));
            REQUIRE(n.duration().numerator() == 2);
            REQUIRE(n.duration().division() == 4);
            REQUIRE(n.duration().dots() == 1);
            REQUIRE(n.duration().type() == DURATION_ABS);
            REQUIRE(!n.isAbsPitch());

            REQUIRE(parse_notation("C^(+50c)|*11", n));
            REQUIRE(n.asMidi() == 72.5);
            REQUIRE(n.duration().numerator() == 11);
            REQUIRE(n.duration().division() == 1);
            REQUIRE(n.duration().type() == DURATION_REL);
            REQUIRE(n.duration().ratio() == 11);

            REQUIRE(parse_notation("C^(+50c)|*3/16", n));
            REQUIRE(n.duration().numerator() == 3);
            REQUIRE(n.duration().division() == 16);
            REQUIRE(n.duration().type() == DURATION_REL);
            REQUIRE(!n.isAbsPitch());

            REQUIRE(parse_notation("C^(-25c)|/16", n));
            REQUIRE(n.asMidi() == 71.75);
            REQUIRE(n.duration().numerator() == 1);
            REQUIRE(n.duration().division() == 16);
            REQUIRE(n.duration().type() == DURATION_REL);
            REQUIRE(!n.isAbsPitch());

            REQUIRE(parse_notation("R|4", n));
            REQUIRE(n.isRest());
            REQUIRE(n.isAbsPitch());
            REQUIRE(n.duration().numerator() == 1);
            REQUIRE(n.duration().division() == 4);
            REQUIRE(n.duration().ratio() == 0.25);
            REQUIRE(n.timeMs({}) == 1000);
            REQUIRE(n.timeMs({ 60, 4 }) == 1000);
            REQUIRE(n.timeMs({ 120, 4 }) == 500);
            REQUIRE(n.timeMs({ 60, 2 }) == 500);
            REQUIRE(n.timeMs({ 60, 1 }) == 250);
            REQUIRE(n.timeMs({ 120, 4 }) == 500);

            REQUIRE(parse_notation("R|7/8", n));
            REQUIRE(n.isRest());
            REQUIRE(n.duration().numerator() == 7);
            REQUIRE(n.duration().division() == 8);
            REQUIRE(n.timeMs({ 60, 8 }) == 7000);
        }
    }

    SECTION("duration")
    {
        SECTION("struct")
        {
            using namespace ceammc::parser;
            using namespace ceammc::music;
            Duration dur;

            REQUIRE(dur.type() == DURATION_ABS);
            REQUIRE(dur.ratio() == 0.25);
            REQUIRE(dur.division() == 4);
            REQUIRE(dur.dots() == 0);
            REQUIRE(dur.numerator() == 1);
            REQUIRE(dur.timeMs({ 60, 4 }) == 1000);
            REQUIRE(dur.timeMs({ 120, 4 }) == 500);
            REQUIRE(dur.timeMs({ 120, 8 }) == 1000);
            REQUIRE(dur.timeMs({ 120, 16 }) == 2000);
            REQUIRE(dur.timeMs({ 120, 32 }) == 4000);

            REQUIRE(dur.timeMs({ 120, { 1, 8 } }) == 1000);
            REQUIRE(dur.timeMs({ 120, { 2, 8 } }) == 500);
            REQUIRE(dur.timeMs({ 120, { 4, 8 } }) == 250);
            REQUIRE(dur.timeMs({ 120, { 8, 8 } }) == 125);
        }

        SECTION("str")
        {
            using namespace ceammc::parser;
            using namespace ceammc::music;
            music::Duration dur;

            REQUIRE(parse_duration("4", dur));
            REQUIRE(dur.ratio() == 0.25);
            REQUIRE(dur.type() == DURATION_ABS);
            REQUIRE(dur.division() == 4);
            REQUIRE(dur.numerator() == 1);
            REQUIRE(dur.dots() == 0);

            REQUIRE(dur.timeMs({ 60, 4 }) == 1000);

            REQUIRE(parse_duration("2.", dur));
            REQUIRE(dur.ratio() == 0.75);
            REQUIRE(dur.type() == DURATION_ABS);
            REQUIRE(dur.division() == 2);
            REQUIRE(dur.numerator() == 1);
            REQUIRE(dur.dots() == 1);

            REQUIRE(parse_duration("2..", dur));
            REQUIRE(dur.ratio() == 0.875);

            REQUIRE(parse_duration("1/8", dur));
            REQUIRE(dur.ratio() == 0.125);

            REQUIRE(parse_duration("8/8", dur));
            REQUIRE(dur.ratio() == 1);
        }

        SECTION("Atom")
        {
            using namespace ceammc::parser;
            music::Duration dur;
            std::vector<music::Duration> out;

            REQUIRE(parse_duration(A(2), dur));
            REQUIRE(dur.ratio() == 0.5);

            REQUIRE(parse_duration(A("2."), dur));
            REQUIRE(dur.ratio() == 0.75);

            REQUIRE(parse_duration(AtomList::parseString("2.")[0], dur));
            REQUIRE(dur.ratio() == 0.5);

            REQUIRE(parse_duration(AtomList::parseString("2_.")[0], dur));
            REQUIRE(dur.ratio() == 0.75);

            REQUIRE(parse_duration(AtomList::parseString("1*1/8")[0], dur));
            REQUIRE(dur.ratio() == 0.125);

            REQUIRE(parse_duration(AtomList::parseString("2*1/8"), out));
            REQUIRE(out.size() == 2);
            REQUIRE(out[0].ratio() == 0.125);
            REQUIRE(out[1].ratio() == 0.125);

            out.clear();
            REQUIRE(parse_duration(AtomList::parseString("10*4"), out));
            REQUIRE(out.size() == 10);
            REQUIRE(std::all_of(out.begin(), out.end(), [](const music::Duration& d) { return d.ratio() == 0.25; }));

            out.clear();
            REQUIRE(parse_duration(AtomList::parseString("4*4."), out));
            REQUIRE(out.size() == 4);
            REQUIRE(std::all_of(out.begin(), out.end(), [](const music::Duration& d) { return d.ratio() == 0.375; }));
        }

        SECTION("list")
        {
            using namespace ceammc::parser;
            std::vector<music::Duration> out;

            REQUIRE(parse_duration(LA(3, "3/16", "16.", "2", "???"), out) == 4);
            REQUIRE(out.size() == 4);

            REQUIRE(out[0].ratio() == Approx(1 / 3.0));
            REQUIRE(out[1].ratio() == Approx(3 / 16.0));
            REQUIRE(out[2].ratio() == Approx(3 / 32.0));
            REQUIRE(out[3].ratio() == Approx(0.5));
        }
    }

    SECTION("pitch")
    {
        using namespace ceammc::parser;
        using namespace ceammc::music;
        PitchClass pc = PitchClass::C;

        REQUIRE(parse_pitch_class("C", pc));
        REQUIRE(pc == PitchClass::C);

        REQUIRE(parse_pitch_class("C#", pc));
        REQUIRE(pc == PitchClass::Cs);

        REQUIRE(parse_pitch_class("C##", pc));
        REQUIRE(pc == PitchClass::Css);

        REQUIRE(parse_pitch_class("Db", pc));
        REQUIRE(pc == PitchClass::Df);

        REQUIRE(parse_pitch_class("Dbb", pc));
        REQUIRE(pc == PitchClass::Dff);

        REQUIRE(parse_pitch_class("Eb", pc));
        REQUIRE(pc == PitchClass::Ef);

        REQUIRE(parse_pitch_class(Atom(0.), pc));
        REQUIRE(pc == PitchClass::C);

        REQUIRE(parse_pitch_class(Atom(1), pc));
        REQUIRE(pc == PitchClass::Cs);

        REQUIRE(parse_pitch_class(Atom(2), pc));
        REQUIRE(pc == PitchClass::D);

        REQUIRE(parse_pitch_class(Atom(3), pc));
        REQUIRE(pc == PitchClass::Ds);

        REQUIRE(parse_pitch_class(Atom(4), pc));
        REQUIRE(pc == PitchClass::E);

        REQUIRE(parse_pitch_class(Atom(5), pc));
        REQUIRE(pc == PitchClass::F);

        REQUIRE(parse_pitch_class(Atom(6), pc));
        REQUIRE(pc == PitchClass::Fs);

        REQUIRE(parse_pitch_class(Atom(7), pc));
        REQUIRE(pc == PitchClass::G);

        REQUIRE(parse_pitch_class(Atom(8), pc));
        REQUIRE(pc == PitchClass::Gs);

        REQUIRE(parse_pitch_class(Atom(9), pc));
        REQUIRE(pc == PitchClass::A);

        REQUIRE(parse_pitch_class(Atom(10), pc));
        REQUIRE(pc == PitchClass::As);

        REQUIRE(parse_pitch_class(Atom(11), pc));
        REQUIRE(pc == PitchClass::B);
    }

    SECTION("chord")
    {
        using namespace ceammc::parser;
        using Vec = std::vector<std::uint8_t>;

        music::ChordClass chord;

        REQUIRE(parse_chord_class("Cmaj", chord));
        REQUIRE(chord.type() == Vec { 0, 4, 7 });
        REQUIRE(parse_chord_class("Cmaj7", chord));
        REQUIRE(chord.type() == Vec { 0, 4, 7, 11 });
        REQUIRE(parse_chord_class("Cmaj9", chord));
        REQUIRE(chord.type() == Vec { 0, 4, 7, 11, 14 });
        REQUIRE(parse_chord_class("Cmaj11", chord));
        REQUIRE(chord.type() == Vec { 0, 4, 7, 11, 14, 17 });
        REQUIRE(parse_chord_class("Cmaj13", chord));
        REQUIRE(chord.type() == Vec { 0, 4, 7, 11, 14, 17, 21 });
    }
}
