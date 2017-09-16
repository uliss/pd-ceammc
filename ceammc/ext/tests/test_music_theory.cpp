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
#include "base_extension_test.h"
#include "ceammc_music_theory.h"

#include "catch.hpp"

#include <sstream>

using namespace ceammc;
using namespace ceammc::music;

TEST_CASE("MusicTheory", "[ceammc::music]")
{
    SECTION("Tonality")
    {
        Tonality t(PitchClass::As, MAJOR);

        std::ostringstream s;
        s << t;
        REQUIRE(s.str() == "A-sharp major");

        REQUIRE(Tonality(PitchClass::A, MAJOR).name() == "A major");
        REQUIRE(Tonality(PitchClass::F, MINOR).name() == "F minor");

        REQUIRE(Tonality(PitchClass::A, MAJOR) == Tonality(PitchClass::A, MAJOR));
        REQUIRE(Tonality(PitchClass::A, MAJOR) != Tonality(PitchClass::D, MAJOR));
        REQUIRE(Tonality(PitchClass::A, MAJOR) != Tonality(PitchClass::A, MINOR));

        REQUIRE(Tonality(PitchClass::Es, MAJOR).enharmonicEqual(Tonality(PitchClass::F, MAJOR)));
        REQUIRE_FALSE(Tonality(PitchClass::Es, MAJOR).enharmonicEqual(Tonality(PitchClass::E, MAJOR)));
        REQUIRE_FALSE(Tonality(PitchClass::Es, MAJOR).enharmonicEqual(Tonality(PitchClass::F, MINOR)));

        REQUIRE(Tonality(PitchClass::Es, MAJOR).simplify() == Tonality(PitchClass::F, MAJOR));

// keys

#define REQUIRE_KEYS(t, m, n) REQUIRE(Tonality(PitchClass::t, m).keys() == n);
#define REQUIRE_FLATS(t, m, n) REQUIRE(Tonality(PitchClass::t, m).flats() == n);
#define REQUIRE_SHARPS(t, m, n) REQUIRE(Tonality(PitchClass::t, m).sharps() == n);

        REQUIRE_KEYS(Cf, MAJOR, 7);
        REQUIRE_KEYS(Gf, MAJOR, 6);
        REQUIRE_KEYS(Df, MAJOR, 5);
        REQUIRE_KEYS(Af, MAJOR, 4);
        REQUIRE_KEYS(Ef, MAJOR, 3);
        REQUIRE_KEYS(Bf, MAJOR, 2);
        REQUIRE_KEYS(F, MAJOR, 1);
        REQUIRE_KEYS(C, MAJOR, 0);
        REQUIRE_KEYS(G, MAJOR, 1);
        REQUIRE_KEYS(D, MAJOR, 2);
        REQUIRE_KEYS(A, MAJOR, 3);
        REQUIRE_KEYS(E, MAJOR, 4);
        REQUIRE_KEYS(B, MAJOR, 5);
        REQUIRE_KEYS(Fs, MAJOR, 6);
        REQUIRE_KEYS(Cs, MAJOR, 7);

        REQUIRE_SHARPS(Cf, MAJOR, 0);
        REQUIRE_SHARPS(Gf, MAJOR, 0);
        REQUIRE_SHARPS(Df, MAJOR, 0);
        REQUIRE_SHARPS(Af, MAJOR, 0);
        REQUIRE_SHARPS(Ef, MAJOR, 0);
        REQUIRE_SHARPS(Bf, MAJOR, 0);
        REQUIRE_SHARPS(F, MAJOR, 0);
        REQUIRE_SHARPS(C, MAJOR, 0);
        REQUIRE_SHARPS(G, MAJOR, 1);
        REQUIRE_SHARPS(D, MAJOR, 2);
        REQUIRE_SHARPS(A, MAJOR, 3);
        REQUIRE_SHARPS(E, MAJOR, 4);
        REQUIRE_SHARPS(B, MAJOR, 5);
        REQUIRE_SHARPS(Fs, MAJOR, 6);
        REQUIRE_SHARPS(Cs, MAJOR, 7);

        REQUIRE_FLATS(Cf, MAJOR, 7);
        REQUIRE_FLATS(Gf, MAJOR, 6);
        REQUIRE_FLATS(Df, MAJOR, 5);
        REQUIRE_FLATS(Af, MAJOR, 4);
        REQUIRE_FLATS(Ef, MAJOR, 3);
        REQUIRE_FLATS(Bf, MAJOR, 2);
        REQUIRE_FLATS(F, MAJOR, 1);
        REQUIRE_FLATS(C, MAJOR, 0);
        REQUIRE_FLATS(G, MAJOR, 0);
        REQUIRE_FLATS(D, MAJOR, 0);
        REQUIRE_FLATS(A, MAJOR, 0);
        REQUIRE_FLATS(E, MAJOR, 0);
        REQUIRE_FLATS(B, MAJOR, 0);
        REQUIRE_FLATS(Fs, MAJOR, 0);
        REQUIRE_FLATS(Cs, MAJOR, 0);

        // simplification
        REQUIRE(Tonality(PitchClass::Ass, MAJOR).name() == "B major");

#define REQUIRE_SCALE(t, p1, p2, p3, p4, p5, p6, p7) \
    {                                                \
        Scale s = t.scale();                         \
        REQUIRE(s[0] == PitchClass::p1);             \
        REQUIRE(s[1] == PitchClass::p2);             \
        REQUIRE(s[2] == PitchClass::p3);             \
        REQUIRE(s[3] == PitchClass::p4);             \
        REQUIRE(s[4] == PitchClass::p5);             \
        REQUIRE(s[5] == PitchClass::p6);             \
        REQUIRE(s[6] == PitchClass::p7);             \
    }

        REQUIRE_SCALE(Tonality(PitchClass::C, MAJOR), C, D, E, F, G, A, B);
        REQUIRE_SCALE(Tonality(PitchClass::Cs, MAJOR), Cs, Ds, Es, Fs, Gs, As, Bs);
        REQUIRE_SCALE(Tonality(PitchClass::Df, MAJOR), Df, Ef, F, Gf, Af, Bf, C);
        REQUIRE_SCALE(Tonality(PitchClass::D, MAJOR), D, E, Fs, G, A, B, Cs);
        //        REQUIRE_SCALE(Tonality(PitchClass::Ds, MAJOR), Ef, F, G, Af, Bf, C, D);
        REQUIRE_SCALE(Tonality(PitchClass::Ef, MAJOR), Ef, F, G, Af, Bf, C, D);
        REQUIRE_SCALE(Tonality(PitchClass::E, MAJOR), E, Fs, Gs, A, B, Cs, Ds);
        REQUIRE_SCALE(Tonality(PitchClass::F, MAJOR), F, G, A, Bf, C, D, E);
        REQUIRE_SCALE(Tonality(PitchClass::Fs, MAJOR), Fs, Gs, As, B, Cs, Ds, Es);
        REQUIRE_SCALE(Tonality(PitchClass::Gf, MAJOR), Gf, Af, Bf, Cf, Df, Ef, F);
        REQUIRE_SCALE(Tonality(PitchClass::G, MAJOR), G, A, B, C, D, E, Fs);
        REQUIRE_SCALE(Tonality(PitchClass::Af, MAJOR), Af, Bf, C, Df, Ef, F, G);
        REQUIRE_SCALE(Tonality(PitchClass::A, MAJOR), A, B, Cs, D, E, Fs, Gs);
        REQUIRE_SCALE(Tonality(PitchClass::Bf, MAJOR), Bf, C, D, Ef, F, G, A);
        REQUIRE_SCALE(Tonality(PitchClass::B, MAJOR), B, Cs, Ds, E, Fs, Gs, As);
    }
}
