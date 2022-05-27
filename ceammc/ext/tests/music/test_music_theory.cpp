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
#include "test_base.h"
#include "ceammc_music_theory.h"

#include "catch.hpp"

#include <sstream>

using namespace ceammc;
using namespace ceammc::music;

#define REQUIRE_TONALITY(p1, p2, m) REQUIRE(Tonality(PitchClass::p1, m).pitch() == PitchClass::p2)
#define REQUIRE_MAJ_FIFTH_INDEX(p, n) REQUIRE(Tonality::fifthsCircleIndex(PitchClass::p, MAJOR) == n)
#define REQUIRE_MIN_FIFTH_INDEX(p, n) REQUIRE(Tonality::fifthsCircleIndex(PitchClass::p, MINOR) == n)
#define REQUIRE_KEYS(t, m, n) REQUIRE(Tonality(PitchClass::t, m).numKeys() == n);
#define REQUIRE_FLATS(t, m, n) REQUIRE(Tonality(PitchClass::t, m).numFlats() == n);
#define REQUIRE_SHARPS(t, m, n) REQUIRE(Tonality(PitchClass::t, m).numSharps() == n);
#define REQUIRE_PITCH(n, t, m, p, d)                                                             \
    {                                                                                            \
        REQUIRE(Tonality::correctAlteration(n, Tonality(PitchClass::t, m), d) == PitchClass::p); \
    }

#define REQUIRE_SCALE(t, m, p1, p2, p3, p4, p5, p6, p7) \
    {                                                   \
        PitchList s = Tonality(PitchClass::t, m).scale();   \
        REQUIRE(s[0] == PitchClass::p1);                \
        REQUIRE(s[1] == PitchClass::p2);                \
        REQUIRE(s[2] == PitchClass::p3);                \
        REQUIRE(s[3] == PitchClass::p4);                \
        REQUIRE(s[4] == PitchClass::p5);                \
        REQUIRE(s[5] == PitchClass::p6);                \
        REQUIRE(s[6] == PitchClass::p7);                \
    }

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
    }

    SECTION("keys")
    {
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
    }

    SECTION("scale")
    {
        REQUIRE_SCALE(C, MAJOR, C, D, E, F, G, A, B);
        REQUIRE_SCALE(Cs, MAJOR, Cs, Ds, Es, Fs, Gs, As, Bs);
        REQUIRE_SCALE(Df, MAJOR, Df, Ef, F, Gf, Af, Bf, C);
        REQUIRE_SCALE(D, MAJOR, D, E, Fs, G, A, B, Cs);
        REQUIRE_SCALE(Ds, MAJOR, Ef, F, G, Af, Bf, C, D);
        REQUIRE_SCALE(Ef, MAJOR, Ef, F, G, Af, Bf, C, D);
        REQUIRE_SCALE(E, MAJOR, E, Fs, Gs, A, B, Cs, Ds);
        REQUIRE_SCALE(F, MAJOR, F, G, A, Bf, C, D, E);
        REQUIRE_SCALE(Fs, MAJOR, Fs, Gs, As, B, Cs, Ds, Es);
        REQUIRE_SCALE(Gf, MAJOR, Gf, Af, Bf, Cf, Df, Ef, F);
        REQUIRE_SCALE(G, MAJOR, G, A, B, C, D, E, Fs);
        REQUIRE_SCALE(Af, MAJOR, Af, Bf, C, Df, Ef, F, G);
        REQUIRE_SCALE(A, MAJOR, A, B, Cs, D, E, Fs, Gs);
        REQUIRE_SCALE(Bf, MAJOR, Bf, C, D, Ef, F, G, A);
        REQUIRE_SCALE(B, MAJOR, B, Cs, Ds, E, Fs, Gs, As);

        REQUIRE_SCALE(C, MINOR, C, D, Ef, F, G, Af, Bf);
        REQUIRE_SCALE(Cs, MINOR, Cs, Ds, E, Fs, Gs, A, B);
        REQUIRE_SCALE(D, MINOR, D, E, F, G, A, Bf, C);
        REQUIRE_SCALE(Ds, MINOR, Ds, Es, Fs, Gs, As, B, Cs);
        REQUIRE_SCALE(Ef, MINOR, Ef, F, Gf, Af, Bf, Cf, Df);
        REQUIRE_SCALE(E, MINOR, E, Fs, G, A, B, C, D);
        REQUIRE_SCALE(F, MINOR, F, G, Af, Bf, C, Df, Ef);
        REQUIRE_SCALE(Fs, MINOR, Fs, Gs, A, B, Cs, D, E);
        REQUIRE_SCALE(G, MINOR, G, A, Bf, C, D, Ef, F);
        REQUIRE_SCALE(Af, MINOR, Af, Bf, Cf, Df, Ef, Ff, Gf);
        REQUIRE_SCALE(A, MINOR, A, B, C, D, E, F, G);
        REQUIRE_SCALE(Bf, MINOR, Bf, C, Df, Ef, F, Gf, Af);
        REQUIRE_SCALE(B, MINOR, B, Cs, D, E, Fs, G, A);
    }

    SECTION("alterations")
    {
        SECTION("major")
        {
            PitchList alt = Tonality(PitchClass::D, MAJOR).alterations();
            REQUIRE(alt.size() == 5);
            REQUIRE(alt[0] == PitchClass::Ds);
            REQUIRE(alt[1] == PitchClass::Es);
            REQUIRE(alt[2] == PitchClass::Gs);
            REQUIRE(alt[3] == PitchClass::As);
            REQUIRE(alt[4] == PitchClass::C);

            alt = Tonality(PitchClass::D, MAJOR).alterations(ALTERATE_UP);
            REQUIRE(alt.size() == 5);
            REQUIRE(alt[0] == PitchClass::Ds);
            REQUIRE(alt[1] == PitchClass::Es);
            REQUIRE(alt[2] == PitchClass::Gs);
            REQUIRE(alt[3] == PitchClass::As);
            REQUIRE(alt[4] == PitchClass::C);

            alt = Tonality(PitchClass::D, MAJOR).alterations(ALTERATE_DOWN);
            REQUIRE(alt.size() == 5);
            REQUIRE(alt[0] == PitchClass::Ef);
            REQUIRE(alt[1] == PitchClass::F);
            REQUIRE(alt[2] == PitchClass::Gs);
            REQUIRE(alt[3] == PitchClass::Bf);
            REQUIRE(alt[4] == PitchClass::C);
        }

        SECTION("minor")
        {
            PitchList alt = Tonality(PitchClass::D, MINOR).alterations();
            REQUIRE(alt.size() == 5);
            REQUIRE(alt[0] == PitchClass::Ef);
            REQUIRE(alt[1] == PitchClass::Fs);
            REQUIRE(alt[2] == PitchClass::Gs);
            REQUIRE(alt[3] == PitchClass::B);
            REQUIRE(alt[4] == PitchClass::Cs);

            alt = Tonality(PitchClass::D, MINOR).alterations(ALTERATE_UP);
            REQUIRE(alt.size() == 5);
            REQUIRE(alt[0] == PitchClass::Ef);
            REQUIRE(alt[1] == PitchClass::Fs);
            REQUIRE(alt[2] == PitchClass::Gs);
            REQUIRE(alt[3] == PitchClass::B);
            REQUIRE(alt[4] == PitchClass::Cs);

            alt = Tonality(PitchClass::D, MINOR).alterations(ALTERATE_DOWN);
            REQUIRE(alt.size() == 5);
            REQUIRE(alt[0] == PitchClass::Ef);
            REQUIRE(alt[1] == PitchClass::Fs);
            REQUIRE(alt[2] == PitchClass::Gs);
            REQUIRE(alt[3] == PitchClass::B);
            REQUIRE(alt[4] == PitchClass::Cs);
        }
    }

    SECTION("chromatics")
    {
        SECTION("major")
        {
            PitchList chrom = Tonality(PitchClass::C, MAJOR).chromatic();
            REQUIRE(chrom.size() == 12);
            REQUIRE(chrom[0] == PitchClass::C);
            REQUIRE(chrom[1] == PitchClass::Cs);
            REQUIRE(chrom[2] == PitchClass::D);
            REQUIRE(chrom[3] == PitchClass::Ds);
            REQUIRE(chrom[4] == PitchClass::E);
            REQUIRE(chrom[5] == PitchClass::F);
            REQUIRE(chrom[6] == PitchClass::Fs);
            REQUIRE(chrom[7] == PitchClass::G);
            REQUIRE(chrom[8] == PitchClass::Gs);
            REQUIRE(chrom[9] == PitchClass::A);
            REQUIRE(chrom[10] == PitchClass::Bf);
            REQUIRE(chrom[11] == PitchClass::B);

            chrom = Tonality(PitchClass::C, MAJOR).chromatic(ALTERATE_UP);
            REQUIRE(chrom.size() == 12);
            REQUIRE(chrom[0] == PitchClass::C);
            REQUIRE(chrom[1] == PitchClass::Cs);
            REQUIRE(chrom[2] == PitchClass::D);
            REQUIRE(chrom[3] == PitchClass::Ds);
            REQUIRE(chrom[4] == PitchClass::E);
            REQUIRE(chrom[5] == PitchClass::F);
            REQUIRE(chrom[6] == PitchClass::Fs);
            REQUIRE(chrom[7] == PitchClass::G);
            REQUIRE(chrom[8] == PitchClass::Gs);
            REQUIRE(chrom[9] == PitchClass::A);
            REQUIRE(chrom[10] == PitchClass::Bf);
            REQUIRE(chrom[11] == PitchClass::B);

            chrom = Tonality(PitchClass::C, MAJOR).chromatic(ALTERATE_DOWN);
            REQUIRE(chrom.size() == 12);
            REQUIRE(chrom[0] == PitchClass::C);
            REQUIRE(chrom[1] == PitchClass::Df);
            REQUIRE(chrom[2] == PitchClass::D);
            REQUIRE(chrom[3] == PitchClass::Ef);
            REQUIRE(chrom[4] == PitchClass::E);
            REQUIRE(chrom[5] == PitchClass::F);
            REQUIRE(chrom[6] == PitchClass::Fs);
            REQUIRE(chrom[7] == PitchClass::G);
            REQUIRE(chrom[8] == PitchClass::Af);
            REQUIRE(chrom[9] == PitchClass::A);
            REQUIRE(chrom[10] == PitchClass::Bf);
            REQUIRE(chrom[11] == PitchClass::B);
        }

        SECTION("minor")
        {
            PitchList chrom = Tonality(PitchClass::D, MINOR).chromatic();
            REQUIRE(chrom.size() == 12);
            REQUIRE(chrom[0] == PitchClass::D);
            REQUIRE(chrom[1] == PitchClass::Ef);
            REQUIRE(chrom[2] == PitchClass::E);
            REQUIRE(chrom[3] == PitchClass::F);
            REQUIRE(chrom[4] == PitchClass::Fs);
            REQUIRE(chrom[5] == PitchClass::G);
            REQUIRE(chrom[6] == PitchClass::Gs);
            REQUIRE(chrom[7] == PitchClass::A);
            REQUIRE(chrom[8] == PitchClass::Bf);
            REQUIRE(chrom[9] == PitchClass::B);
            REQUIRE(chrom[10] == PitchClass::C);
            REQUIRE(chrom[11] == PitchClass::Cs);

            chrom = Tonality(PitchClass::D, MINOR).chromatic(ALTERATE_UP);
            REQUIRE(chrom.size() == 12);
            REQUIRE(chrom[0] == PitchClass::D);
            REQUIRE(chrom[1] == PitchClass::Ef);
            REQUIRE(chrom[2] == PitchClass::E);
            REQUIRE(chrom[3] == PitchClass::F);
            REQUIRE(chrom[4] == PitchClass::Fs);
            REQUIRE(chrom[5] == PitchClass::G);
            REQUIRE(chrom[6] == PitchClass::Gs);
            REQUIRE(chrom[7] == PitchClass::A);
            REQUIRE(chrom[8] == PitchClass::Bf);
            REQUIRE(chrom[9] == PitchClass::B);
            REQUIRE(chrom[10] == PitchClass::C);
            REQUIRE(chrom[11] == PitchClass::Cs);

            chrom = Tonality(PitchClass::D, MINOR).chromatic(ALTERATE_DOWN);
            REQUIRE(chrom.size() == 12);
            REQUIRE(chrom[0] == PitchClass::D);
            REQUIRE(chrom[1] == PitchClass::Ef);
            REQUIRE(chrom[2] == PitchClass::E);
            REQUIRE(chrom[3] == PitchClass::F);
            REQUIRE(chrom[4] == PitchClass::Fs);
            REQUIRE(chrom[5] == PitchClass::G);
            REQUIRE(chrom[6] == PitchClass::Gs);
            REQUIRE(chrom[7] == PitchClass::A);
            REQUIRE(chrom[8] == PitchClass::Bf);
            REQUIRE(chrom[9] == PitchClass::B);
            REQUIRE(chrom[10] == PitchClass::C);
            REQUIRE(chrom[11] == PitchClass::Cs);
        }
    }

    SECTION("name")
    {
        REQUIRE(Tonality(PitchClass::Bf, MAJOR).name() == "B-flat major");
        REQUIRE(Tonality(PitchClass::Bf, MINOR).name() == "B-flat minor");
    }

    SECTION("correct alteration")
    {
        REQUIRE_PITCH(0, C, MAJOR, C, ALTERATE_UP);
        REQUIRE_PITCH(1, C, MAJOR, Cs, ALTERATE_UP);
        REQUIRE_PITCH(2, C, MAJOR, D, ALTERATE_UP);
        REQUIRE_PITCH(3, C, MAJOR, Ds, ALTERATE_UP);
        REQUIRE_PITCH(4, C, MAJOR, E, ALTERATE_UP);
        REQUIRE_PITCH(5, C, MAJOR, F, ALTERATE_UP);
        REQUIRE_PITCH(6, C, MAJOR, Fs, ALTERATE_UP);
        REQUIRE_PITCH(7, C, MAJOR, G, ALTERATE_UP);
        REQUIRE_PITCH(8, C, MAJOR, Gs, ALTERATE_UP);
        REQUIRE_PITCH(9, C, MAJOR, A, ALTERATE_UP);
        REQUIRE_PITCH(10, C, MAJOR, Bf, ALTERATE_UP);
        REQUIRE_PITCH(11, C, MAJOR, B, ALTERATE_UP);

        REQUIRE_PITCH(11, C, MAJOR, B, ALTERATE_DOWN);
        REQUIRE_PITCH(10, C, MAJOR, Bf, ALTERATE_DOWN);
        REQUIRE_PITCH(9, C, MAJOR, A, ALTERATE_DOWN);
        REQUIRE_PITCH(8, C, MAJOR, Af, ALTERATE_DOWN);
        REQUIRE_PITCH(7, C, MAJOR, G, ALTERATE_DOWN);
        REQUIRE_PITCH(6, C, MAJOR, Fs, ALTERATE_DOWN);
        REQUIRE_PITCH(5, C, MAJOR, F, ALTERATE_DOWN);
        REQUIRE_PITCH(4, C, MAJOR, E, ALTERATE_DOWN);
        REQUIRE_PITCH(3, C, MAJOR, Ef, ALTERATE_DOWN);
        REQUIRE_PITCH(2, C, MAJOR, D, ALTERATE_DOWN);
        REQUIRE_PITCH(1, C, MAJOR, Df, ALTERATE_DOWN);
        REQUIRE_PITCH(0, C, MAJOR, C, ALTERATE_DOWN);

        REQUIRE_PITCH(2, D, MINOR, D, ALTERATE_UP);
        REQUIRE_PITCH(3, D, MINOR, Ef, ALTERATE_UP);
        REQUIRE_PITCH(4, D, MINOR, E, ALTERATE_UP);
        REQUIRE_PITCH(5, D, MINOR, F, ALTERATE_UP);
        REQUIRE_PITCH(6, D, MINOR, Fs, ALTERATE_UP);
        REQUIRE_PITCH(7, D, MINOR, G, ALTERATE_UP);
        REQUIRE_PITCH(8, D, MINOR, Gs, ALTERATE_UP);
        REQUIRE_PITCH(9, D, MINOR, A, ALTERATE_UP);
        REQUIRE_PITCH(10, D, MINOR, Bf, ALTERATE_UP);
        REQUIRE_PITCH(11, D, MINOR, B, ALTERATE_UP);
        REQUIRE_PITCH(0, D, MINOR, C, ALTERATE_UP);
        REQUIRE_PITCH(1, D, MINOR, Cs, ALTERATE_UP);

        REQUIRE_PITCH(2, D, MINOR, D, ALTERATE_DOWN);
        REQUIRE_PITCH(3, D, MINOR, Ef, ALTERATE_DOWN);
        REQUIRE_PITCH(4, D, MINOR, E, ALTERATE_DOWN);
        REQUIRE_PITCH(5, D, MINOR, F, ALTERATE_DOWN);
        REQUIRE_PITCH(6, D, MINOR, Fs, ALTERATE_DOWN);
        REQUIRE_PITCH(7, D, MINOR, G, ALTERATE_DOWN);
        REQUIRE_PITCH(8, D, MINOR, Gs, ALTERATE_DOWN);
        REQUIRE_PITCH(9, D, MINOR, A, ALTERATE_DOWN);
        REQUIRE_PITCH(10, D, MINOR, Bf, ALTERATE_DOWN);
        REQUIRE_PITCH(11, D, MINOR, B, ALTERATE_DOWN);
        REQUIRE_PITCH(0, D, MINOR, C, ALTERATE_DOWN);
        REQUIRE_PITCH(1, D, MINOR, Cs, ALTERATE_DOWN);
    }

    SECTION("to_string")
    {
        REQUIRE(to_string(Tonality(PitchClass::B, MAJOR)) == "B major");
        REQUIRE(to_string(Tonality(PitchClass::Bf, MAJOR)) == "B flat major");
        REQUIRE(to_string(Tonality(PitchClass::B, MAJOR), NAMING_SCHEME_GERMAN) == "H Dur");
        REQUIRE(to_string(Tonality(PitchClass::Bf, MAJOR), NAMING_SCHEME_GERMAN) == "B Dur");
        REQUIRE(to_string(Tonality(PitchClass::Bff, MAJOR), NAMING_SCHEME_GERMAN) == "A Dur");

        REQUIRE(to_string(Tonality(PitchClass::Af, MAJOR), NAMING_SCHEME_GERMAN) == "As Dur");
        REQUIRE(to_string(Tonality(PitchClass::Ef, MAJOR), NAMING_SCHEME_GERMAN) == "Es Dur");
    }

    SECTION("from string")
    {
        Tonality t(PitchClass::C, MAJOR);
        REQUIRE_FALSE(from_string("", t, NAMING_SCHEME_ENGLISH));

#define REQUIRE_FROM_STR(str, t, m, ns)             \
    {                                               \
        Tonality ton(PitchClass::t, m);             \
        REQUIRE(from_string(str, ton, ns));         \
        REQUIRE(ton == Tonality(PitchClass::t, m)); \
    }

        REQUIRE_FROM_STR("C", C, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("D", D, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("E", E, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("F", F, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("G", G, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("A", A, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("B", B, MAJOR, NAMING_SCHEME_ENGLISH);

        REQUIRE_FROM_STR("C major", C, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("D major", D, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("E major", E, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("F major", F, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("G major", G, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("A major", A, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("B major", B, MAJOR, NAMING_SCHEME_ENGLISH);

        REQUIRE_FROM_STR("C-major", C, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("D-major", D, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("E-major", E, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("F-major", F, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("G-major", G, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("A-major", A, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("B-major", B, MAJOR, NAMING_SCHEME_ENGLISH);

        REQUIRE_FROM_STR("C maj", C, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("D maj", D, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("E maj", E, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("F maj", F, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("G maj", G, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("A maj", A, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("B maj", B, MAJOR, NAMING_SCHEME_ENGLISH);

        REQUIRE_FROM_STR("C-maj", C, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("D-maj", D, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("E-maj", E, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("F-maj", F, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("G-maj", G, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("A-maj", A, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("B-maj", B, MAJOR, NAMING_SCHEME_ENGLISH);

        REQUIRE_FROM_STR("C-dur", C, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("D-dur", D, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("E-dur", E, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("F-dur", F, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("G-dur", G, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("A-dur", A, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("B-dur", B, MAJOR, NAMING_SCHEME_ENGLISH);

        REQUIRE_FROM_STR("C dur", C, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("D dur", D, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("E dur", E, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("F dur", F, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("G dur", G, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("A dur", A, MAJOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("B dur", B, MAJOR, NAMING_SCHEME_ENGLISH);

        REQUIRE_FROM_STR("C minor", C, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("D minor", D, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("E minor", E, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("F minor", F, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("G minor", G, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("A minor", A, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("B minor", B, MINOR, NAMING_SCHEME_ENGLISH);

        REQUIRE_FROM_STR("C-minor", C, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("D-minor", D, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("E-minor", E, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("F-minor", F, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("G-minor", G, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("A-minor", A, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("B-minor", B, MINOR, NAMING_SCHEME_ENGLISH);

        REQUIRE_FROM_STR("C min", C, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("D min", D, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("E min", E, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("F min", F, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("G min", G, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("A min", A, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("B min", B, MINOR, NAMING_SCHEME_ENGLISH);

        REQUIRE_FROM_STR("C-min", C, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("D-min", D, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("E-min", E, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("F-min", F, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("G-min", G, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("A-min", A, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("B-min", B, MINOR, NAMING_SCHEME_ENGLISH);

        REQUIRE_FROM_STR("C-moll", C, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("D-moll", D, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("E-moll", E, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("F-moll", F, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("G-moll", G, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("A-moll", A, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("B-moll", B, MINOR, NAMING_SCHEME_ENGLISH);

        REQUIRE_FROM_STR("C moll", C, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("D moll", D, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("E moll", E, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("F moll", F, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("G moll", G, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("A moll", A, MINOR, NAMING_SCHEME_ENGLISH);
        REQUIRE_FROM_STR("B moll", B, MINOR, NAMING_SCHEME_ENGLISH);

        REQUIRE_FROM_STR("C#", Cs, MAJOR, NAMING_SCHEME_SPN);
        REQUIRE_FROM_STR("C# major", Cs, MAJOR, NAMING_SCHEME_SPN);
        REQUIRE_FROM_STR("C# minor", Cs, MINOR, NAMING_SCHEME_SPN);

        REQUIRE_FROM_STR("Ab", Af, MAJOR, NAMING_SCHEME_SPN);
        REQUIRE_FROM_STR("Ab major", Af, MAJOR, NAMING_SCHEME_SPN);
        REQUIRE_FROM_STR("Ab minor", Af, MINOR, NAMING_SCHEME_SPN);
    }
}
