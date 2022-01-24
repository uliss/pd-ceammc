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
            BpmFullMatch p;

            REQUIRE(p.parse("120"));
            REQUIRE(p.bpm().bpm == 120);
            REQUIRE(p.bpm().beatlen == 0.25);
            REQUIRE(p.bpm().freqHz() == 2);
            REQUIRE(p.bpm().value() == 120);
            REQUIRE(p.bpm().periodMs() == 500);
            REQUIRE(p.bpm().periodSamp(10000) == 5000);

            REQUIRE(p.parse("120.5"));
            REQUIRE(p.bpm().bpm == 120.5);
            REQUIRE(p.bpm().beatlen == 0.25);
            REQUIRE(p.bpm().value() == 120.5);

            REQUIRE(p.parse("120bpm"));
            REQUIRE(p.bpm().bpm == 120);
            REQUIRE(p.bpm().beatlen == 0.25);
            REQUIRE(p.bpm().value() == 120);

            REQUIRE(p.parse("0"));
            REQUIRE(p.bpm().bpm == 0);
            REQUIRE(p.bpm().beatlen == 0.25);
            REQUIRE(p.bpm().periodMs() == 0);
            REQUIRE(p.bpm().periodSamp(44100) == 0);

            REQUIRE(p.parse("144bpm"));
            REQUIRE(p.bpm().bpm == 144);
            REQUIRE(p.bpm().beatlen == 0.25);

            REQUIRE(p.parse("50.25_bpm"));
            REQUIRE(p.bpm().bpm == 50.25);
            REQUIRE(p.bpm().beatlen == 0.25);

            REQUIRE(p.parse("50|1_bpm"));
            REQUIRE(p.bpm().bpm == 50);
            REQUIRE(p.bpm().beatlen == 1);

            REQUIRE(p.parse("50|2_bpm"));
            REQUIRE(p.bpm().bpm == 50);
            REQUIRE(p.bpm().beatlen == 0.5);

            REQUIRE(p.parse("50|2._bpm"));
            REQUIRE(p.bpm().bpm == 50);
            REQUIRE(p.bpm().beatlen == 0.75);

            REQUIRE(p.parse("50|4_bpm"));
            REQUIRE(p.bpm().bpm == 50);
            REQUIRE(p.bpm().beatlen == 0.25);

            REQUIRE(p.parse("50|1/4._bpm"));
            REQUIRE(p.bpm().bpm == 50);
            REQUIRE(p.bpm().beatlen == 0.375);

            REQUIRE(p.parse("50|16_bpm"));
            REQUIRE(p.bpm().value() == 50);
            REQUIRE(p.bpm().beatlen == 1 / 16.0);

            REQUIRE(p.parse("0"));
            REQUIRE(p.bpm().freqHz() == 0);
            REQUIRE(p.bpm().periodMs() == 0);
            REQUIRE(p.bpm().periodMs(-100) == -100);

            REQUIRE(p.parse("60|4.bpm"));

            REQUIRE(p.parse("60|1/4bpm"));

            REQUIRE_FALSE(p.parse("00"));
            REQUIRE_FALSE(p.parse("+12"));
            REQUIRE_FALSE(p.parse("-12"));
            REQUIRE_FALSE(p.parse(""));
        }

        SECTION("float")
        {
            using namespace ceammc::parser;
            BpmFullMatch p;

            REQUIRE(p.parse(A(120.5)));
            REQUIRE(p.bpm().value() == 120.5);

            REQUIRE(p.parse(A(0.0)));
            REQUIRE(p.bpm().value() == 0);

            REQUIRE_FALSE(p.parse(A(-1)));
        }

        SECTION("list")
        {
            using namespace ceammc::parser;
            BpmFullMatch p;
            SmallBpmVec res;

            REQUIRE(p.parse(LA(1, "23", "44.5_bpm", "unknown"), res) == 3);
            REQUIRE(res.size() == 3);
            REQUIRE(res[0].bpm == 1);
            REQUIRE(res[1].bpm == 23);
            REQUIRE(res[2].bpm == 44.5);
        }

        SECTION("N")
        {
            using namespace ceammc::parser;
            BpmFullMatch p;

            auto r0 = p.parseN<3>(L());
            REQUIRE(r0[0] == Bpm { 0, 0.25 });
            REQUIRE(r0[1] == Bpm { 0, 0.25 });
            REQUIRE(r0[2] == Bpm { 0, 0.25 });

            r0 = p.parseN<3>(LA(120), Bpm { 61, 0.5 });
            REQUIRE(r0[0] == Bpm { 120, 0.25 });
            REQUIRE(r0[1] == Bpm { 61, 0.5 });
            REQUIRE(r0[2] == Bpm { 61, 0.5 });

            r0 = p.parseN<3>(LA(120, "140.5|8bpm"), Bpm { 62, 2 });
            REQUIRE(r0[0] == Bpm { 120, 1 / 4.0 });
            REQUIRE(r0[1] == Bpm { 140.5, 1 / 8.0 });
            REQUIRE(r0[2] == Bpm { 62, 2 });

            r0 = p.parseN<3>(LA(120, "140.5bpm", "100|16_bpm"), Bpm { 62, 1 / 128.0 });
            REQUIRE(r0[0] == Bpm { 120, 0.25 });
            REQUIRE(r0[1] == Bpm { 140.5, 0.25 });
            REQUIRE(r0[2] == Bpm { 100, 1 / 16.0 });

            r0 = p.parseN<3>(LA(120, "???", "100|32_bpm"), Bpm { 63, 2 });
            REQUIRE(r0[0] == Bpm { 120, 0.25 });
            REQUIRE(r0[1] == Bpm { 63, 2 });
            REQUIRE(r0[2] == Bpm { 100, 1 / 32.0 });
        }
    }

    SECTION("spn")
    {
        SECTION("str")
        {
            using namespace ceammc::parser;
            SpnFullMatch p;

            REQUIRE(p.parse("C4"));
            REQUIRE(p.spn().midi() == 60);
            REQUIRE(p.spn().oct == 4);
            REQUIRE(p.spn().note == 0);
            REQUIRE(p.spn().alt == 0);
            REQUIRE(p.spn().pitch() == 0);
            REQUIRE(!p.spn().isRest());

            REQUIRE(p.parse("C#4"));
            REQUIRE(p.spn().oct == 4);
            REQUIRE(p.spn().note == 0);
            REQUIRE(p.spn().alt == 1);
            REQUIRE(p.spn().pitch() == 1);
            REQUIRE(p.spn().midi() == 61);

            REQUIRE(p.parse("C##4"));
            REQUIRE(p.spn().midi() == 62);

            REQUIRE(p.parse("Cb4"));
            REQUIRE(p.spn().midi() == 59);

            REQUIRE(p.parse("Cbb4"));
            REQUIRE(p.spn().midi() == 58);

            REQUIRE(p.parse("C4(+50c)"));
            REQUIRE(p.spn().midi() == 60.5);

            REQUIRE(p.parse("C4(+0c)"));
            REQUIRE(p.spn().midi() == 60);

            REQUIRE(p.parse("C4(+01c)"));
            REQUIRE(p.spn().midi() == Approx(60.01));

            REQUIRE(p.parse("C4(+5c)"));
            REQUIRE(p.spn().midi() == Approx(60.05));

            REQUIRE(p.parse("C4(-25c)"));
            REQUIRE(p.spn().midi() == 59.75);

            REQUIRE(p.parse("A4"));
            REQUIRE(p.spn().midi() == 69);
            REQUIRE(p.parse("B4"));
            REQUIRE(p.spn().midi() == 71);
            REQUIRE(p.parse("D4"));
            REQUIRE(p.spn().midi() == 62);
            REQUIRE(p.parse("E4"));
            REQUIRE(p.spn().midi() == 64);
            REQUIRE(p.parse("F4"));
            REQUIRE(p.spn().midi() == 65);
            REQUIRE(p.parse("G4"));
            REQUIRE(p.spn().midi() == 67);

            REQUIRE(p.parse("A4"));
            REQUIRE(p.spn().midi() == 69);
            REQUIRE(p.parse("B4"));
            REQUIRE(p.spn().midi() == 71);
            REQUIRE(p.parse("H4"));
            REQUIRE(p.spn().midi() == 71);
            REQUIRE(p.parse("D4"));
            REQUIRE(p.spn().midi() == 62);
            REQUIRE(p.parse("E4"));
            REQUIRE(p.spn().midi() == 64);
            REQUIRE(p.parse("F4"));
            REQUIRE(p.spn().midi() == 65);
            REQUIRE(p.parse("G4"));
            REQUIRE(p.spn().midi() == 67);

            REQUIRE(p.parse("C9"));
            REQUIRE(p.spn().midi() == 120);

            REQUIRE(p.parse("C0"));
            REQUIRE(p.spn().midi() == 12);

            REQUIRE(p.parse("C-1"));
            REQUIRE(p.spn().midi() == 0);

            REQUIRE(p.parse("C"));
            REQUIRE(p.spn().midi() == 60);

            REQUIRE(p.parse("C"));
            REQUIRE(p.spn().midi(5) == 72);

            REQUIRE(p.parse("Db"));
            REQUIRE(p.spn().midi() == 61);

            REQUIRE(p.parse("Dbb"));
            REQUIRE(p.spn().midi() == 60);

            REQUIRE(p.parse("Dbb^"));
            REQUIRE(p.spn().midi() == 72);

            REQUIRE(p.parse("Dbb^^"));
            REQUIRE(p.spn().midi() == 84);

            REQUIRE(p.parse("Dbb_"));
            REQUIRE(p.spn().midi() == 48);

            REQUIRE(p.parse("D#____"));
            REQUIRE(p.spn().midi() == 15);

            REQUIRE(!p.parse("C10"));
            REQUIRE(!p.parse("C-2"));
        }

        SECTION("atom")
        {
            using namespace ceammc::parser;
            SpnFullMatch p;

            REQUIRE(!p.parse(A(100)));

            REQUIRE(p.parse("C#5"));
            REQUIRE(p.spn().midi() == 73);
        }

        SECTION("next")
        {
            using namespace ceammc::parser;
            SpnFullMatch p;

            auto pitches = p.parseN<6>(LA("C", "D", "E", "F", "G", "C"));
            REQUIRE(std::none_of(pitches.begin(), pitches.end(), [](const Spn& s) { return s.isAbsOctave(); }));

            for (size_t i = 0; i + 1 < pitches.size(); i++) {
                auto& p0 = pitches[i];
                auto& p1 = pitches[i + 1];
                Spn::setNextOctave(p0, p1);
            }

            REQUIRE(std::none_of(pitches.begin(), pitches.end(), [](const Spn& s) { return s.isAbsOctave(); }));

            pitches[0].setAbsOctave(5);
            for (size_t i = 0; i + 1 < pitches.size(); i++) {
                auto& p0 = pitches[i];
                auto& p1 = pitches[i + 1];
                Spn::setNextOctave(p0, p1);
            }

            REQUIRE(std::all_of(pitches.begin(), pitches.end(), [](const Spn& s) { return s.isAbsOctave(); }));
            REQUIRE(pitches[0].oct == 5);
            REQUIRE(pitches[1].oct == 5);
            REQUIRE(pitches[2].oct == 5);
            REQUIRE(pitches[3].oct == 5);
            REQUIRE(pitches[4].oct == 5);
            REQUIRE(pitches[5].oct == 5);

            pitches = p.parseN<6>(LA("C", "A", "F", "F^", "F__", "C7"));
            REQUIRE(pitches[0].octtype == OCTAVE_REL);
            REQUIRE(pitches[1].octtype == OCTAVE_REL);
            REQUIRE(pitches[2].octtype == OCTAVE_REL);
            REQUIRE(pitches[3].octtype == OCTAVE_REL);
            REQUIRE(pitches[4].octtype == OCTAVE_REL);
            REQUIRE(pitches[5].octtype == OCTAVE_ABS);

            pitches[0].setAbsOctave(5);
            for (size_t i = 0; i + 1 < pitches.size(); i++) {
                auto& p0 = pitches[i];
                auto& p1 = pitches[i + 1];
                Spn::setNextOctave(p0, p1);
            }

            REQUIRE(pitches[0].oct == 5);
            REQUIRE(pitches[1].oct == 5);
            REQUIRE(pitches[2].oct == 5);
            REQUIRE(pitches[3].oct == 6);
            REQUIRE(pitches[4].oct == 4);
            REQUIRE(pitches[5].oct == 7);
        }
    }

    SECTION("notation")
    {
        SECTION("str")
        {
            using namespace ceammc::parser;
            NotationSingle p;

            REQUIRE(p.parse("C"));
            REQUIRE(!p.note().spn.isRest());
            REQUIRE(p.note().spn.midi() == 60);
            REQUIRE(p.note().dur.ratio() == 0.25);
            REQUIRE(p.note().dur.num == 1);
            REQUIRE(p.note().dur.den == 4);
            REQUIRE(!p.note().dur.isAbs());
            REQUIRE(p.note().dur.timeMs() == 1000);
            REQUIRE(p.note().dur.timeMs(Bpm(120)) == 500);

            REQUIRE(p.parse("C3"));
            REQUIRE(p.note().spn.midi() == 48);
            REQUIRE(p.note().dur.num == 1);
            REQUIRE(p.note().dur.den == 4);
            REQUIRE(p.note().dur.ratio() == 0.25);

            REQUIRE(p.parse("C#1|4"));
            REQUIRE(p.note().spn.midi() == 25);
            REQUIRE(p.note().dur.isAbs());
            REQUIRE(p.note().spn.oct == 1);
            REQUIRE(p.note().dur.num == 1);
            REQUIRE(p.note().dur.den == 4);
            REQUIRE(p.note().dur.ratio() == 0.25);

            REQUIRE(p.parse("Cb5|2."));
            REQUIRE(p.note().spn.midi() == 71);
            REQUIRE(p.note().spn.oct == 5);
            REQUIRE(p.note().dur.num == 3);
            REQUIRE(p.note().dur.den == 4);
            REQUIRE(p.note().dur.ratio() == 0.75);

            REQUIRE(p.parse("Cb5|2.."));
            REQUIRE(p.note().spn.midi() == 71);
            REQUIRE(p.note().spn.oct == 5);
            REQUIRE(p.note().dur.num == 7);
            REQUIRE(p.note().dur.den == 8);
            REQUIRE(p.note().dur.ratio() == 0.875);

            REQUIRE(p.parse("Cbb5|2..."));
            REQUIRE(p.note().spn.midi() == 70);
            REQUIRE(p.note().spn.oct == 5);
            REQUIRE(p.note().dur.num == 15);
            REQUIRE(p.note().dur.den == 16);
            REQUIRE(p.note().dur.ratio() == 0.9375);

            REQUIRE(p.parse("D|256"));
            REQUIRE(p.note().dur.num == 1);
            REQUIRE(p.note().dur.den == 256);

            REQUIRE(p.parse("D|2/3"));
            REQUIRE(p.note().dur.num == 2);
            REQUIRE(p.note().dur.den == 3);
            REQUIRE(p.note().dur.isAbs());

            REQUIRE(p.parse("D|1/4"));
            REQUIRE(p.note().dur.num == 1);
            REQUIRE(p.note().dur.den == 4);

            REQUIRE(p.parse("D|1/4."));
            REQUIRE(p.note().dur.num == 3);
            REQUIRE(p.note().dur.den == 8);

            REQUIRE(p.parse("C(+50c)|5/4"));
            REQUIRE(p.note().spn.midi() == 60.5);
            REQUIRE(p.note().dur.num == 5);
            REQUIRE(p.note().dur.den == 4);
            REQUIRE(p.note().dur.ratio() == 1.25);

            REQUIRE(p.parse("C(+50c)|2/4."));
            REQUIRE(p.note().spn.midi() == 60.5);
            REQUIRE(p.note().dur.num == 6);
            REQUIRE(p.note().dur.den == 8);
            REQUIRE(p.note().dur.isAbs());

            REQUIRE(p.parse("C(+25c)|2/4."));
            REQUIRE(p.note().spn.midi() == 60.25);
            REQUIRE(p.note().dur.num == 6);
            REQUIRE(p.note().dur.den == 8);
            REQUIRE(p.note().dur.isAbs());

            REQUIRE(p.parse("C(+12c)|2/4."));
            REQUIRE(p.note().spn.midi() == Approx(60.12));
            REQUIRE(p.note().dur.num == 6);
            REQUIRE(p.note().dur.den == 8);
            REQUIRE(p.note().dur.isAbs());

            REQUIRE(p.parse("C^(+50c)|*11"));
            REQUIRE(p.note().spn.midi() == 72.5);
            REQUIRE(p.note().dur.num == 11);
            REQUIRE(p.note().dur.den == 1);
            REQUIRE(p.note().dur.durtype == DURATION_REL);
            REQUIRE(p.note().dur.ratio() == 11);

            REQUIRE(p.parse("C^(+50c)|*3/16"));
            REQUIRE(p.note().dur.num == 3);
            REQUIRE(p.note().dur.den == 16);
            REQUIRE(p.note().dur.durtype == DURATION_REL);
            REQUIRE(!p.note().dur.isAbs());

            REQUIRE(p.parse("C^(-25c)|/16"));
            REQUIRE(p.note().spn.midi() == 71.75);
            REQUIRE(p.note().dur.num == 1);
            REQUIRE(p.note().dur.den == 16);
            REQUIRE(p.note().dur.durtype == DURATION_REL);
            REQUIRE(!p.note().dur.isAbs());

            REQUIRE(p.parse("R|4"));
            REQUIRE(p.note().isRest());
            REQUIRE(p.note().dur.isAbs());
            REQUIRE(p.note().dur.num == 1);
            REQUIRE(p.note().dur.den == 4);
            REQUIRE(p.note().dur.ratio() == 0.25);
            REQUIRE(p.note().dur.timeMs() == 1000);
            REQUIRE(p.note().dur.timeMs({ 60, 0.5 }) == 500);
            REQUIRE(p.note().dur.timeMs({ 60, 1 }) == 250);
            REQUIRE(p.note().dur.timeMs({ 120, 0.25 }) == 500);
            REQUIRE(p.note().dur.timeSamp(44100) == 44100);

            REQUIRE(p.parse("R|7/8"));
            REQUIRE(p.note().isRest());
            REQUIRE(p.note().dur.num == 7);
            REQUIRE(p.note().dur.den == 8);
            REQUIRE(p.note().dur.timeMs({ 60, 1 / 8.0 }) == 7000);
        }
    }

    SECTION("duration")
    {
        SECTION("str")
        {
            using namespace ceammc::parser;
            DurationFullMatch p;

            REQUIRE(p.parse("4"));
            REQUIRE(p.result().ratio() == 0.25);

            REQUIRE(p.parse("2."));
            REQUIRE(p.result().ratio() == 0.75);

            REQUIRE(p.parse("2.."));
            REQUIRE(p.result().ratio() == 0.875);

            REQUIRE(p.parse("1/8"));
            REQUIRE(p.result().ratio() == 0.125);

            REQUIRE(p.parse("8/8"));
            REQUIRE(p.result().ratio() == 1);
        }

        SECTION("Atom")
        {
            using namespace ceammc::parser;
            DurationFullMatch p;
            DurationVec out;

            REQUIRE(p.parse(A(2)));
            REQUIRE(p.result().ratio() == 0.5);

            REQUIRE(p.parse(A("2.")));
            REQUIRE(p.result().ratio() == 0.75);

            REQUIRE(p.parse(AtomList::parseString("2.")[0]));
            REQUIRE(p.result().ratio() == 0.5);

            REQUIRE(p.parse(AtomList::parseString("2_.")[0]));
            REQUIRE(p.result().ratio() == 0.75);

            REQUIRE(p.parse(AtomList::parseString("1*1/8")[0]));
            REQUIRE(p.result().ratio() == 0.125);

            REQUIRE(p.parse(AtomList::parseString("2*1/8"), out));
            REQUIRE(out.size() == 2);
            REQUIRE(out[0].ratio() == 0.125);
            REQUIRE(out[1].ratio() == 0.125);

            out.clear();
            REQUIRE(p.parse(AtomList::parseString("10*4"), out));
            REQUIRE(out.size() == 10);
            REQUIRE(std::all_of(out.begin(), out.end(), [](const Duration& d) { return d.ratio() == 0.25; }));

            out.clear();
            REQUIRE(p.parse(AtomList::parseString("4*4."), out));
            REQUIRE(out.size() == 4);
            REQUIRE(std::all_of(out.begin(), out.end(), [](const Duration& d) { return d.ratio() == 0.375; }));
        }

        SECTION("list")
        {
            using namespace ceammc::parser;
            DurationFullMatch p;
            DurationVec out;

            REQUIRE(p.parse(LA(3, "3/16", "16.", "2", "???"), out) == 4);
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
        PitchFullMatch p;

        REQUIRE(p.parse("C"));
        REQUIRE(p.spn().note == 0);
        REQUIRE(p.spn().alt == 0);
        REQUIRE(p.spn().p == 0);
        REQUIRE(p.spn().oct == 0);
        REQUIRE(p.spn().octtype == OCTAVE_REL);

        REQUIRE(p.parse("C#"));
        REQUIRE(p.spn().note == 0);
        REQUIRE(p.spn().alt == 1);
        REQUIRE(p.spn().pitch() == 1);

        REQUIRE(p.parse("Db"));
        REQUIRE(p.spn().note == 1);
        REQUIRE(p.spn().alt == -1);
        REQUIRE(p.spn().pitch() == 1);

        REQUIRE(p.parse("Eb"));
        REQUIRE(p.spn().note == 2);
        REQUIRE(p.spn().alt == -1);
        REQUIRE(p.spn().pitch() == 3);

        REQUIRE(p.parse(Atom(0.)));
        REQUIRE(p.spn().note == 0);
        REQUIRE(p.spn().alt == 0);
        REQUIRE(p.spn().pitch() == 0);

        REQUIRE(p.parse(Atom(1)));
        REQUIRE(p.spn().note == 0);
        REQUIRE(p.spn().alt == 1);
        REQUIRE(p.spn().pitch() == 1);

        REQUIRE(p.parse(Atom(2)));
        REQUIRE(p.spn().note == 1);
        REQUIRE(p.spn().alt == 0);
        REQUIRE(p.spn().pitch() == 2);

        REQUIRE(p.parse(Atom(3)));
        REQUIRE(p.spn().note == 1);
        REQUIRE(p.spn().alt == 1);
        REQUIRE(p.spn().pitch() == 3);

        REQUIRE(p.parse(Atom(4)));
        REQUIRE(p.spn().note == 2);
        REQUIRE(p.spn().alt == 0);
        REQUIRE(p.spn().pitch() == 4);

        REQUIRE(p.parse(Atom(5)));
        REQUIRE(p.spn().alt == 0);
        REQUIRE(p.spn().note == 3);
        REQUIRE(p.spn().pitch() == 5);

        REQUIRE(p.parse(Atom(6)));
        REQUIRE(p.spn().note == 3);
        REQUIRE(p.spn().alt == 1);
        REQUIRE(p.spn().pitch() == 6);

        REQUIRE(p.parse(Atom(7)));
        REQUIRE(p.spn().note == 4);
        REQUIRE(p.spn().alt == 0);
        REQUIRE(p.spn().pitch() == 7);

        REQUIRE(p.parse(Atom(8)));
        REQUIRE(p.spn().note == 4);
        REQUIRE(p.spn().alt == 1);
        REQUIRE(p.spn().pitch() == 8);

        REQUIRE(p.parse(Atom(9)));
        REQUIRE(p.spn().note == 5);
        REQUIRE(p.spn().alt == 0);
        REQUIRE(p.spn().pitch() == 9);

        REQUIRE(p.parse(Atom(10)));
        REQUIRE(p.spn().note == 5);
        REQUIRE(p.spn().alt == 1);
        REQUIRE(p.spn().pitch() == 10);

        REQUIRE(p.parse(Atom(11)));
        REQUIRE(p.spn().note == 6);
        REQUIRE(p.spn().alt == 0);
        REQUIRE(p.spn().pitch() == 11);
    }
}
