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

#include <cstdlib>
#include <ctime>
#include <sstream>

using namespace ceammc;
using namespace ceammc::music;

#ifdef REQUIRE_INDEX
#undef REQUIRE_INDEX
#endif

#define REQUIRE_STEP_TRANS(p1, p2, n)                               \
    {                                                               \
        REQUIRE(PitchClass::p1.stepTranspose(n) == PitchClass::p2); \
    }

#define REQUIRE_ALT_TO_PATTERN(p1, p2)                                   \
    {                                                                    \
        PitchClass p0 = PitchClass::p1;                                  \
        REQUIRE(PitchClass::tryAlterateToEqPattern(p0, PitchClass::p2)); \
        REQUIRE(p0.enharmonicEqual(PitchClass::p2));                     \
    }

#define REQUIRE_ENHARM_2(p, e0, e1)                  \
    {                                                \
        Enharmonics en = PitchClass::p.enharmonic(); \
        REQUIRE(en.size() == 2);                     \
        REQUIRE(en[0] == PitchClass::e0);            \
        REQUIRE(en[1] == PitchClass::e1);            \
    }

#define REQUIRE_NO_UP_ENHARM(p)                            \
    {                                                      \
        Enharmonics en = PitchClass::p.upperEnharmonics(); \
        REQUIRE(en.empty());                               \
    }

#define REQUIRE_UP_ENHARM(p, e)                            \
    {                                                      \
        Enharmonics en = PitchClass::p.upperEnharmonics(); \
        REQUIRE(en.size() == 1);                           \
        REQUIRE(en[0].enharmonicEqual(PitchClass::p));     \
        REQUIRE(en[0] == PitchClass::e);                   \
    }

#define REQUIRE_LOW_ENHARM(p, e)                           \
    {                                                      \
        Enharmonics en = PitchClass::p.lowerEnharmonics(); \
        REQUIRE(en.size() == 1);                           \
        REQUIRE(en[0].enharmonicEqual(PitchClass::p));     \
        REQUIRE(en[0] == PitchClass::e);                   \
    }

#define REQUIRE_UP_ENHARM_2(p, e0, e1)                     \
    {                                                      \
        Enharmonics en = PitchClass::p.upperEnharmonics(); \
        REQUIRE(en.size() == 2);                           \
        REQUIRE(en[0].enharmonicEqual(PitchClass::p));     \
        REQUIRE(en[1].enharmonicEqual(PitchClass::p));     \
        REQUIRE(en[0] == PitchClass::e0);                  \
        REQUIRE(en[1] == PitchClass::e1);                  \
    }

#define REQUIRE_LOW_ENHARM_2(p, e0, e1)                    \
    {                                                      \
        Enharmonics en = PitchClass::p.lowerEnharmonics(); \
        REQUIRE(en.size() == 2);                           \
        REQUIRE(en[0].enharmonicEqual(PitchClass::p));     \
        REQUIRE(en[1].enharmonicEqual(PitchClass::p));     \
        REQUIRE(en[0] == PitchClass::e0);                  \
        REQUIRE(en[1] == PitchClass::e1);                  \
    }

TEST_CASE("MusicTheory::PitchClass", "[ceammc::music]")
{
    SECTION("construct")
    {
        PitchClass p(PitchName::C);
        REQUIRE(p.pitchName() == PitchName::C);
        REQUIRE(p.alteration() == Alteration::NATURAL);
        REQUIRE(p);

        PitchClass p2(PitchName::D, Alteration::FLAT);
        REQUIRE(p2.pitchName() == PitchName::D);
        REQUIRE(p2.alteration() == Alteration::FLAT);
        REQUIRE(p2);
    }

    SECTION("pitch compare")
    {
        REQUIRE(PitchClass::C == PitchClass::C);
        REQUIRE(PitchClass::C != PitchClass::Cs);
        REQUIRE(PitchClass::Cs != PitchClass::Df);
    }

    SECTION("absolute pitch")
    {
        REQUIRE(PitchClass::C.absolutePitch() == 0);
        REQUIRE(PitchClass::Cs.absolutePitch() == 1);
        REQUIRE(PitchClass::Css.absolutePitch() == 2);

        REQUIRE(PitchClass::Dff.absolutePitch() == 0);
        REQUIRE(PitchClass::Df.absolutePitch() == 1);
        REQUIRE(PitchClass::D.absolutePitch() == 2);
        REQUIRE(PitchClass::Ds.absolutePitch() == 3);
        REQUIRE(PitchClass::Dss.absolutePitch() == 4);

        REQUIRE(PitchClass::Eff.absolutePitch() == 2);
        REQUIRE(PitchClass::Ef.absolutePitch() == 3);
        REQUIRE(PitchClass::E.absolutePitch() == 4);
        REQUIRE(PitchClass::Es.absolutePitch() == 5);
        REQUIRE(PitchClass::Ess.absolutePitch() == 6);

        REQUIRE(PitchClass::Fff.absolutePitch() == 3);
        REQUIRE(PitchClass::Ff.absolutePitch() == 4);
        REQUIRE(PitchClass::F.absolutePitch() == 5);
        REQUIRE(PitchClass::Fs.absolutePitch() == 6);
        REQUIRE(PitchClass::Fss.absolutePitch() == 7);

        REQUIRE(PitchClass::Gff.absolutePitch() == 5);
        REQUIRE(PitchClass::Gf.absolutePitch() == 6);
        REQUIRE(PitchClass::G.absolutePitch() == 7);
        REQUIRE(PitchClass::Gs.absolutePitch() == 8);
        REQUIRE(PitchClass::Gss.absolutePitch() == 9);

        REQUIRE(PitchClass::Aff.absolutePitch() == 7);
        REQUIRE(PitchClass::Af.absolutePitch() == 8);
        REQUIRE(PitchClass::A.absolutePitch() == 9);
        REQUIRE(PitchClass::As.absolutePitch() == 10);
        REQUIRE(PitchClass::Ass.absolutePitch() == 11);

        REQUIRE(PitchClass::Bff.absolutePitch() == 9);
        REQUIRE(PitchClass::Bf.absolutePitch() == 10);
        REQUIRE(PitchClass::B.absolutePitch() == 11);
        REQUIRE(PitchClass::Bs.absolutePitch() == 0);
        REQUIRE(PitchClass::Bss.absolutePitch() == 1);

        REQUIRE(PitchClass::Cff.absolutePitch() == 10);
        REQUIRE(PitchClass::Cf.absolutePitch() == 11);
    }

    SECTION("simplify")
    {
        SECTION("simplify full")
        {
            REQUIRE(PitchClass::Cff.simplifyFull() == PitchClass::Bf);
            REQUIRE(PitchClass::Cf.simplifyFull() == PitchClass::B);
            REQUIRE(PitchClass::C.simplifyFull() == PitchClass::C);
            REQUIRE(PitchClass::Cs.simplifyFull() == PitchClass::Cs);
            REQUIRE(PitchClass::Css.simplifyFull() == PitchClass::D);

            REQUIRE(PitchClass::Dff.simplifyFull() == PitchClass::C);
            REQUIRE(PitchClass::Df.simplifyFull() == PitchClass::Df);
            REQUIRE(PitchClass::D.simplifyFull() == PitchClass::D);
            REQUIRE(PitchClass::Ds.simplifyFull() == PitchClass::Ds);
            REQUIRE(PitchClass::Dss.simplifyFull() == PitchClass::E);

            REQUIRE(PitchClass::Eff.simplifyFull() == PitchClass::D);
            REQUIRE(PitchClass::Ef.simplifyFull() == PitchClass::Ef);
            REQUIRE(PitchClass::E.simplifyFull() == PitchClass::E);
            REQUIRE(PitchClass::Es.simplifyFull() == PitchClass::F);
            REQUIRE(PitchClass::Ess.simplifyFull() == PitchClass::Fs);

            REQUIRE(PitchClass::Fff.simplifyFull() == PitchClass::Ef);
            REQUIRE(PitchClass::Ff.simplifyFull() == PitchClass::E);
            REQUIRE(PitchClass::F.simplifyFull() == PitchClass::F);
            REQUIRE(PitchClass::Fs.simplifyFull() == PitchClass::Fs);
            REQUIRE(PitchClass::Fss.simplifyFull() == PitchClass::G);

            REQUIRE(PitchClass::Gff.simplifyFull() == PitchClass::F);
            REQUIRE(PitchClass::Gf.simplifyFull() == PitchClass::Gf);
            REQUIRE(PitchClass::G.simplifyFull() == PitchClass::G);
            REQUIRE(PitchClass::Gs.simplifyFull() == PitchClass::Gs);
            REQUIRE(PitchClass::Gss.simplifyFull() == PitchClass::A);

            REQUIRE(PitchClass::Aff.simplifyFull() == PitchClass::G);
            REQUIRE(PitchClass::Af.simplifyFull() == PitchClass::Af);
            REQUIRE(PitchClass::A.simplifyFull() == PitchClass::A);
            REQUIRE(PitchClass::As.simplifyFull() == PitchClass::As);
            REQUIRE(PitchClass::Ass.simplifyFull() == PitchClass::B);

            REQUIRE(PitchClass::Bff.simplifyFull() == PitchClass::A);
            REQUIRE(PitchClass::Bf.simplifyFull() == PitchClass::Bf);
            REQUIRE(PitchClass::B.simplifyFull() == PitchClass::B);
            REQUIRE(PitchClass::Bs.simplifyFull() == PitchClass::C);
            REQUIRE(PitchClass::Bss.simplifyFull() == PitchClass::Cs);
        }

        SECTION("simplify doubles")
        {
            REQUIRE(PitchClass::Cff.simplifyDouble() == PitchClass::Bf);
            REQUIRE(PitchClass::Cf.simplifyDouble() == PitchClass::Cf);
            REQUIRE(PitchClass::C.simplifyDouble() == PitchClass::C);
            REQUIRE(PitchClass::Cs.simplifyDouble() == PitchClass::Cs);
            REQUIRE(PitchClass::Css.simplifyDouble() == PitchClass::D);

            REQUIRE(PitchClass::Dff.simplifyDouble() == PitchClass::C);
            REQUIRE(PitchClass::Df.simplifyDouble() == PitchClass::Df);
            REQUIRE(PitchClass::D.simplifyDouble() == PitchClass::D);
            REQUIRE(PitchClass::Ds.simplifyDouble() == PitchClass::Ds);
            REQUIRE(PitchClass::Dss.simplifyDouble() == PitchClass::E);

            REQUIRE(PitchClass::Eff.simplifyDouble() == PitchClass::D);
            REQUIRE(PitchClass::Ef.simplifyDouble() == PitchClass::Ef);
            REQUIRE(PitchClass::E.simplifyDouble() == PitchClass::E);
            REQUIRE(PitchClass::Es.simplifyDouble() == PitchClass::Es);
            REQUIRE(PitchClass::Ess.simplifyDouble() == PitchClass::Fs);

            REQUIRE(PitchClass::Fff.simplifyDouble() == PitchClass::Ef);
            REQUIRE(PitchClass::Ff.simplifyDouble() == PitchClass::Ff);
            REQUIRE(PitchClass::F.simplifyDouble() == PitchClass::F);
            REQUIRE(PitchClass::Fs.simplifyDouble() == PitchClass::Fs);
            REQUIRE(PitchClass::Fss.simplifyDouble() == PitchClass::G);

            REQUIRE(PitchClass::Gff.simplifyDouble() == PitchClass::F);
            REQUIRE(PitchClass::Gf.simplifyDouble() == PitchClass::Gf);
            REQUIRE(PitchClass::G.simplifyDouble() == PitchClass::G);
            REQUIRE(PitchClass::Gs.simplifyDouble() == PitchClass::Gs);
            REQUIRE(PitchClass::Gss.simplifyDouble() == PitchClass::A);

            REQUIRE(PitchClass::Aff.simplifyDouble() == PitchClass::G);
            REQUIRE(PitchClass::Af.simplifyDouble() == PitchClass::Af);
            REQUIRE(PitchClass::A.simplifyDouble() == PitchClass::A);
            REQUIRE(PitchClass::As.simplifyDouble() == PitchClass::As);
            REQUIRE(PitchClass::Ass.simplifyDouble() == PitchClass::B);

            REQUIRE(PitchClass::Bff.simplifyDouble() == PitchClass::A);
            REQUIRE(PitchClass::Bf.simplifyDouble() == PitchClass::Bf);
            REQUIRE(PitchClass::B.simplifyDouble() == PitchClass::B);
            REQUIRE(PitchClass::Bs.simplifyDouble() == PitchClass::Bs);
            REQUIRE(PitchClass::Bss.simplifyDouble() == PitchClass::Cs);
        }
    }

    SECTION("toneUp")
    {
        REQUIRE_FALSE(PitchClass::Bss.toneUp());
        REQUIRE(PitchClass::Bs.toneUp());
        REQUIRE(PitchClass::Bs.toneUp() == PitchClass::Css);
        REQUIRE(PitchClass::B.toneUp() == PitchClass::Cs);
        REQUIRE(PitchClass::Bf.toneUp() == PitchClass::C);
        REQUIRE(PitchClass::Bff.toneUp() == PitchClass::Cf);

        REQUIRE(PitchClass::Ass.toneUp() == PitchClass::Bss);
        REQUIRE(PitchClass::As.toneUp() == PitchClass::Bs);
        REQUIRE(PitchClass::A.toneUp() == PitchClass::B);
        REQUIRE(PitchClass::Af.toneUp() == PitchClass::Bf);
        REQUIRE(PitchClass::Aff.toneUp() == PitchClass::Bff);

        REQUIRE(PitchClass::Gss.toneUp() == PitchClass::Ass);
        REQUIRE(PitchClass::Gs.toneUp() == PitchClass::As);
        REQUIRE(PitchClass::G.toneUp() == PitchClass::A);
        REQUIRE(PitchClass::Gf.toneUp() == PitchClass::Af);
        REQUIRE(PitchClass::Gff.toneUp() == PitchClass::Aff);

        REQUIRE(PitchClass::Fss.toneUp() == PitchClass::Gss);
        REQUIRE(PitchClass::Fs.toneUp() == PitchClass::Gs);
        REQUIRE(PitchClass::F.toneUp() == PitchClass::G);
        REQUIRE(PitchClass::Ff.toneUp() == PitchClass::Gf);
        REQUIRE(PitchClass::Fff.toneUp() == PitchClass::Gff);

        REQUIRE_FALSE(PitchClass::Ess.toneUp());
        REQUIRE(PitchClass::Es.toneUp());
        REQUIRE(PitchClass::Es.toneUp() == PitchClass::Fss);
        REQUIRE(PitchClass::E.toneUp() == PitchClass::Fs);
        REQUIRE(PitchClass::Ef.toneUp() == PitchClass::F);
        REQUIRE(PitchClass::Eff.toneUp() == PitchClass::Ff);

        REQUIRE(PitchClass::Dss.toneUp() == PitchClass::Ess);
        REQUIRE(PitchClass::Ds.toneUp() == PitchClass::Es);
        REQUIRE(PitchClass::D.toneUp() == PitchClass::E);
        REQUIRE(PitchClass::Df.toneUp() == PitchClass::Ef);
        REQUIRE(PitchClass::Dff.toneUp() == PitchClass::Eff);

        REQUIRE(PitchClass::Css.toneUp() == PitchClass::Dss);
        REQUIRE(PitchClass::Cs.toneUp() == PitchClass::Ds);
        REQUIRE(PitchClass::C.toneUp() == PitchClass::D);
        REQUIRE(PitchClass::Cf.toneUp() == PitchClass::Df);
        REQUIRE(PitchClass::Cff.toneUp() == PitchClass::Dff);
    }

    SECTION("semitoneUp")
    {
        REQUIRE(PitchClass::Bss.semitoneUp() == PitchClass::Css);
        REQUIRE(PitchClass::Bs.semitoneUp() == PitchClass::Cs);
        REQUIRE(PitchClass::B.semitoneUp() == PitchClass::C);
        REQUIRE(PitchClass::Bf.semitoneUp() == PitchClass::Cf);
        REQUIRE(PitchClass::Bff.semitoneUp() == PitchClass::Cff);

        REQUIRE(PitchClass::Ass.semitoneUp() == PitchClass::Bs);
        REQUIRE(PitchClass::As.semitoneUp() == PitchClass::B);
        REQUIRE(PitchClass::A.semitoneUp() == PitchClass::Bf);
        REQUIRE(PitchClass::Af.semitoneUp() == PitchClass::Bff);
        REQUIRE_FALSE(PitchClass::Aff.semitoneUp());

        REQUIRE(PitchClass::Gss.semitoneUp() == PitchClass::As);
        REQUIRE(PitchClass::Gs.semitoneUp() == PitchClass::A);
        REQUIRE(PitchClass::G.semitoneUp() == PitchClass::Af);
        REQUIRE(PitchClass::Gf.semitoneUp() == PitchClass::Aff);
        REQUIRE_FALSE(PitchClass::Gff.semitoneUp());

        REQUIRE(PitchClass::Fss.semitoneUp() == PitchClass::Gs);
        REQUIRE(PitchClass::Fs.semitoneUp() == PitchClass::G);
        REQUIRE(PitchClass::F.semitoneUp() == PitchClass::Gf);
        REQUIRE(PitchClass::Ff.semitoneUp() == PitchClass::Gff);
        REQUIRE_FALSE(PitchClass::Fff.semitoneUp());

        REQUIRE(PitchClass::Ess.semitoneUp() == PitchClass::Fss);
        REQUIRE(PitchClass::Es.semitoneUp() == PitchClass::Fs);
        REQUIRE(PitchClass::E.semitoneUp() == PitchClass::F);
        REQUIRE(PitchClass::Ef.semitoneUp() == PitchClass::Ff);
        REQUIRE(PitchClass::Eff.semitoneUp() == PitchClass::Fff);

        REQUIRE(PitchClass::Dss.semitoneUp() == PitchClass::Es);
        REQUIRE(PitchClass::Ds.semitoneUp() == PitchClass::E);
        REQUIRE(PitchClass::D.semitoneUp() == PitchClass::Ef);
        REQUIRE(PitchClass::Df.semitoneUp() == PitchClass::Eff);
        REQUIRE_FALSE(PitchClass::Dff.semitoneUp());

        REQUIRE(PitchClass::Css.semitoneUp() == PitchClass::Ds);
        REQUIRE(PitchClass::Cs.semitoneUp() == PitchClass::D);
        REQUIRE(PitchClass::C.semitoneUp() == PitchClass::Df);
        REQUIRE(PitchClass::Cf.semitoneUp() == PitchClass::Dff);
        REQUIRE_FALSE(PitchClass::Cff.semitoneUp());
    }

    SECTION("stepTranspose")
    {
        REQUIRE_STEP_TRANS(Cff, Dff, 1);
        REQUIRE_STEP_TRANS(Cf, Df, 1);
        REQUIRE_STEP_TRANS(C, D, 1);
        REQUIRE_STEP_TRANS(Cs, Ds, 1);
        REQUIRE_STEP_TRANS(Css, Dss, 1);

        REQUIRE_STEP_TRANS(Eff, Fff, 1);
        REQUIRE_STEP_TRANS(Ef, Ff, 1);
        REQUIRE_STEP_TRANS(E, F, 1);
        REQUIRE_STEP_TRANS(Es, Fs, 1);
        REQUIRE_STEP_TRANS(Ess, Fss, 1);

        REQUIRE_STEP_TRANS(Cff, Fff, 3);
        REQUIRE_STEP_TRANS(Cf, Ff, 3);
        REQUIRE_STEP_TRANS(C, F, 3);
        REQUIRE_STEP_TRANS(Cs, Fs, 3);
        REQUIRE_STEP_TRANS(Css, Fss, 3);

        REQUIRE_STEP_TRANS(Fff, Cff, -3);
        REQUIRE_STEP_TRANS(Ff, Cf, -3);
        REQUIRE_STEP_TRANS(F, C, -3);
        REQUIRE_STEP_TRANS(Fs, Cs, -3);
        REQUIRE_STEP_TRANS(Fss, Css, -3);

        REQUIRE_STEP_TRANS(C, B, -1);
        REQUIRE_STEP_TRANS(B, C, 1);

        REQUIRE_STEP_TRANS(E, F, 1);
        REQUIRE_STEP_TRANS(F, E, -1);

        REQUIRE_STEP_TRANS(C, G, 4);
        REQUIRE_STEP_TRANS(G, C, -4);

        REQUIRE_STEP_TRANS(C, A, 5);
        REQUIRE_STEP_TRANS(A, C, -5);

        REQUIRE_STEP_TRANS(C, C, 7);
        REQUIRE_STEP_TRANS(C, C, -7);
        REQUIRE_STEP_TRANS(C, C, 14);
        REQUIRE_STEP_TRANS(C, C, -14);
        REQUIRE_STEP_TRANS(C, C, 7000);
        REQUIRE_STEP_TRANS(C, C, -7000);
    }

    SECTION("enharmonic")
    {
        REQUIRE(PitchClass::Cs.enharmonicEqual(PitchClass::Df));

        REQUIRE_UP_ENHARM(C, Dff);
        REQUIRE_UP_ENHARM(Cs, Df);
        REQUIRE_UP_ENHARM_2(Css, D, Eff);
        REQUIRE_NO_UP_ENHARM(Dff);
        REQUIRE_NO_UP_ENHARM(Df);
        REQUIRE_UP_ENHARM(D, Eff);
        REQUIRE_UP_ENHARM_2(Ds, Ef, Fff);
        REQUIRE_UP_ENHARM_2(Dss, E, Ff);
        REQUIRE_NO_UP_ENHARM(Eff);
        REQUIRE_UP_ENHARM(Ef, Fff);
        REQUIRE_UP_ENHARM(E, Ff);
        REQUIRE_UP_ENHARM_2(Es, F, Gff);
        REQUIRE_UP_ENHARM_2(Ess, Fs, Gf);
        REQUIRE_UP_ENHARM_2(Ess, Fs, Gf);
        REQUIRE_UP_ENHARM(F, Gff);
        REQUIRE_UP_ENHARM(Fs, Gf);
        REQUIRE_UP_ENHARM_2(Fss, G, Aff);
        REQUIRE_NO_UP_ENHARM(Gff);
        REQUIRE_NO_UP_ENHARM(Gf);
        REQUIRE_UP_ENHARM(G, Aff);
        REQUIRE_UP_ENHARM(Gs, Af);
        REQUIRE_UP_ENHARM_2(Gss, A, Bff);
        REQUIRE_NO_UP_ENHARM(Aff);
        REQUIRE_NO_UP_ENHARM(Af);
        REQUIRE_UP_ENHARM(A, Bff);
        REQUIRE_UP_ENHARM_2(As, Bf, Cff);
        REQUIRE_UP_ENHARM_2(Ass, B, Cf);

        REQUIRE_ALT_TO_PATTERN(E, F);
        REQUIRE_ALT_TO_PATTERN(E, Fs);
        REQUIRE_ALT_TO_PATTERN(E, Ff);

        //        REQUIRE_UP_ENHARM(B, Cf);
        //        REQUIRE_ENHARM_2(Bf, Cff, As);
        //        REQUIRE_ENHARM_2(Cff, Bf, As);
        //        REQUIRE_ENHARM_2(Bf, Cff, As);

        //        REQUIRE_ENHARM_2(B, Cf, Ass);
        //        REQUIRE_ENHARM_2(C, Dff, Bs);
        //        REQUIRE_ENHARM_2(Cs, Df, Bss);
        //        REQUIRE_ENHARM_2(Df, Cs, Bss);
        //        REQUIRE_ENHARM_2(D, Eff, Css);
    }
}
