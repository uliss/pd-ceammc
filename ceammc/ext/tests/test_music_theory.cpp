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

#define REQUIRE_TONALITY(p1, p2, m) REQUIRE(Tonality(PitchClass::p1, m).pitch() == PitchClass::p2)
#define REQUIRE_MAJ_FIFTH_INDEX(p, n) REQUIRE(Tonality::fifthsCircleIndex(PitchClass::p, MAJOR) == n)
#define REQUIRE_MIN_FIFTH_INDEX(p, n) REQUIRE(Tonality::fifthsCircleIndex(PitchClass::p, MINOR) == n)

TEST_CASE("MusicTheory", "[ceammc::music]")
{
    SECTION("fifthsIndex")
    {
        REQUIRE_MAJ_FIFTH_INDEX(Cf, -7);
        REQUIRE_MAJ_FIFTH_INDEX(Gf, -6);
        REQUIRE_MAJ_FIFTH_INDEX(Df, -5);
        REQUIRE_MAJ_FIFTH_INDEX(Af, -4);
        REQUIRE_MAJ_FIFTH_INDEX(Ef, -3);
        REQUIRE_MAJ_FIFTH_INDEX(Bf, -2);
        REQUIRE_MAJ_FIFTH_INDEX(F, -1);
        REQUIRE_MAJ_FIFTH_INDEX(C, 0);
        REQUIRE_MAJ_FIFTH_INDEX(G, 1);
        REQUIRE_MAJ_FIFTH_INDEX(D, 2);
        REQUIRE_MAJ_FIFTH_INDEX(A, 3);
        REQUIRE_MAJ_FIFTH_INDEX(E, 4);
        REQUIRE_MAJ_FIFTH_INDEX(B, 5);
        REQUIRE_MAJ_FIFTH_INDEX(Fs, 6);
        REQUIRE_MAJ_FIFTH_INDEX(Cs, 7);
    }

    SECTION("construct")
    {
        REQUIRE_TONALITY(Cff, Bf, MAJOR);
        REQUIRE_TONALITY(Cf, Cf, MAJOR);
        REQUIRE_TONALITY(C, C, MAJOR);
        REQUIRE_TONALITY(Cs, Cs, MAJOR);
        REQUIRE_TONALITY(Css, D, MAJOR);

        REQUIRE_TONALITY(Dff, C, MAJOR);
        REQUIRE_TONALITY(Df, Df, MAJOR);
        REQUIRE_TONALITY(D, D, MAJOR);
        REQUIRE_TONALITY(Ds, Ef, MAJOR);
        REQUIRE_TONALITY(Dss, E, MAJOR);

        REQUIRE_TONALITY(Eff, D, MAJOR);
        REQUIRE_TONALITY(Ef, Ef, MAJOR);
        REQUIRE_TONALITY(E, E, MAJOR);
        REQUIRE_TONALITY(Es, F, MAJOR);
        REQUIRE_TONALITY(Ess, Fs, MAJOR);

        REQUIRE_TONALITY(Fff, Ef, MAJOR);
        REQUIRE_TONALITY(Ff, E, MAJOR);
        REQUIRE_TONALITY(F, F, MAJOR);
        REQUIRE_TONALITY(Fs, Fs, MAJOR);
        REQUIRE_TONALITY(Fss, G, MAJOR);

        REQUIRE_TONALITY(Gff, F, MAJOR);
        REQUIRE_TONALITY(Gf, Gf, MAJOR);
        REQUIRE_TONALITY(G, G, MAJOR);
        REQUIRE_TONALITY(Gs, Af, MAJOR);
        REQUIRE_TONALITY(Gss, A, MAJOR);

        REQUIRE_TONALITY(Aff, G, MAJOR);
        REQUIRE_TONALITY(Af, Af, MAJOR);
        REQUIRE_TONALITY(A, A, MAJOR);
        REQUIRE_TONALITY(As, Bf, MAJOR);
        REQUIRE_TONALITY(Ass, B, MAJOR);

        REQUIRE_TONALITY(Bff, A, MAJOR);
        REQUIRE_TONALITY(Bf, Bf, MAJOR);
        REQUIRE_TONALITY(B, B, MAJOR);
        REQUIRE_TONALITY(Bs, C, MAJOR);
        REQUIRE_TONALITY(Bss, Cs, MAJOR);

        REQUIRE_TONALITY(Cff, Bf, MINOR);
        REQUIRE_TONALITY(Cf, B, MINOR);
        REQUIRE_TONALITY(C, C, MINOR);
        REQUIRE_TONALITY(Cs, Cs, MINOR);
        REQUIRE_TONALITY(Css, D, MINOR);

        REQUIRE_TONALITY(Dff, C, MINOR);
        REQUIRE_TONALITY(Df, Cs, MINOR);
        REQUIRE_TONALITY(D, D, MINOR);
        REQUIRE_TONALITY(Ds, Ds, MINOR);
        REQUIRE_TONALITY(Dss, E, MINOR);

        REQUIRE_TONALITY(Eff, D, MINOR);
        REQUIRE_TONALITY(Ef, Ef, MINOR);
        REQUIRE_TONALITY(E, E, MINOR);
        REQUIRE_TONALITY(Es, F, MINOR);
        REQUIRE_TONALITY(Ess, Fs, MINOR);

        REQUIRE_TONALITY(Fff, Ef, MINOR);
        REQUIRE_TONALITY(Ff, E, MINOR);
        REQUIRE_TONALITY(F, F, MINOR);
        REQUIRE_TONALITY(Fs, Fs, MINOR);
        REQUIRE_TONALITY(Fss, G, MINOR);

        REQUIRE_TONALITY(Gff, F, MINOR);
        REQUIRE_TONALITY(Gf, Fs, MINOR);
        REQUIRE_TONALITY(G, G, MINOR);
        REQUIRE_TONALITY(Gs, Gs, MINOR);
        REQUIRE_TONALITY(Gss, A, MINOR);

        REQUIRE_TONALITY(Aff, G, MINOR);
        REQUIRE_TONALITY(Af, Af, MINOR);
        REQUIRE_TONALITY(A, A, MINOR);
        REQUIRE_TONALITY(As, As, MINOR);
        REQUIRE_TONALITY(Ass, B, MINOR);

        REQUIRE_TONALITY(Bff, A, MINOR);
        REQUIRE_TONALITY(Bf, Bf, MINOR);
        REQUIRE_TONALITY(B, B, MINOR);
        REQUIRE_TONALITY(Bs, C, MINOR);
        REQUIRE_TONALITY(Bss, Cs, MINOR);
    }

    SECTION("Tonality")
    {
        Tonality t(PitchClass::As, MAJOR);

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

#define REQUIRE_KEYS(t, m, n) REQUIRE(Tonality(PitchClass::t, m).numKeys() == n);
#define REQUIRE_FLATS(t, m, n) REQUIRE(Tonality(PitchClass::t, m).numFlats() == n);
#define REQUIRE_SHARPS(t, m, n) REQUIRE(Tonality(PitchClass::t, m).numSharps() == n);

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
