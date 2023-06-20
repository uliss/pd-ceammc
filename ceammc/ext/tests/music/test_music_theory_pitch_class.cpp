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
#include "ceammc_music_theory.h"
#include "test_base.h"

#include "catch.hpp"

#include <algorithm>
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

#define REQUIRE_CAN_ALT_TO(p1, p2)                                     \
    {                                                                  \
        PitchClass p0 = PitchClass::p1;                                \
        REQUIRE(PitchClass::tryAlterateToEqPitch(p0, PitchClass::p2)); \
        REQUIRE(p0.enharmonicEqual(PitchClass::p2));                   \
    }

#define REQUIRE_NO_ALT_TO(p1, p2)                                            \
    {                                                                        \
        PitchClass p0 = PitchClass::p1;                                      \
        REQUIRE_FALSE(PitchClass::tryAlterateToEqPitch(p0, PitchClass::p2)); \
        REQUIRE(p0 == PitchClass::p1);                                       \
    }

#define REQUIRE_ENHARM_2(p, e0, e1)                   \
    {                                                 \
        Enharmonics en = PitchClass::p.enharmonics(); \
        REQUIRE(en.size() == 2);                      \
        REQUIRE(en[0] == PitchClass::e0);             \
        REQUIRE(en[1] == PitchClass::e1);             \
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

#define REQUIRE_NO_LOW_ENHARM(p)                           \
    {                                                      \
        Enharmonics en = PitchClass::p.lowerEnharmonics(); \
        REQUIRE(en.empty());                               \
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

#define REQUIRE_MIN_SEMITONE_DISTANCE(p1, p2, n)                                       \
    {                                                                                  \
        REQUIRE(PitchClass::minSemitoneDistance(PitchClass::p1, PitchClass::p2) == n); \
    }

#define REQUIRE_MIN_SEMITONES_FROM_TO(p1, p2, n)                                      \
    {                                                                                 \
        REQUIRE(PitchClass::minSemitonesFromTo(PitchClass::p1, PitchClass::p2) == n); \
    }

#define REQUIRE_TO_STR(p, s, ns) REQUIRE(to_string(PitchClass::p, ns) == s)

#define REQUIRE_PITCH(p1, p2)       \
    {                               \
        auto res = p1;              \
        REQUIRE(res.isOk());        \
        REQUIRE(res.value() == p2); \
    }

static bool pitchClassCmp(const PitchClass& c1, const PitchClass& c2)
{
    return c1.pitchName().absolutePitch() < c2.pitchName().absolutePitch();
}

TEST_CASE("MusicTheory::PitchClass", "[ceammc::music]")
{
    SECTION("static")
    {
        REQUIRE(PitchClass::C.pitchName() == PitchName::C);
        REQUIRE(PitchClass::C.alteration() == Alteration::NATURAL);
        REQUIRE(PitchClass::C.absolutePitch() == 0);

        REQUIRE(PitchClass::D.pitchName() == PitchName::D);
        REQUIRE(PitchClass::D.alteration() == Alteration::NATURAL);
        REQUIRE(PitchClass::D.absolutePitch() == 2);

        REQUIRE(PitchClass::C != PitchClass::D);
    }

    SECTION("construct")
    {
        PitchClass p(PitchName::C, Alteration::NATURAL);
        REQUIRE(p.pitchName() == PitchName::C);
        REQUIRE(p.alteration() == Alteration::NATURAL);
        REQUIRE(p.absolutePitch() == 0);

        PitchClass p1(PitchName::C, Alteration::SHARP);
        REQUIRE(p1.pitchName() == PitchName::C);
        REQUIRE(p1.alteration() == Alteration::SHARP);
        REQUIRE(p1.absolutePitch() == 1);

        PitchClass p2(PitchName::D, Alteration::FLAT);
        REQUIRE(p2.pitchName() == PitchName::D);
        REQUIRE(p2.alteration() == Alteration::FLAT);
        REQUIRE(p2.absolutePitch() == 1);

        PitchClass p3(PitchName::E, Alteration::NATURAL);
        REQUIRE(p3.pitchName() == PitchName::E);
        REQUIRE(p3.alteration() == Alteration::NATURAL);
        REQUIRE(p3.absolutePitch() == 4);

        REQUIRE(PitchClass::C == PitchClass::C);
        REQUIRE(PitchClass::Cs == PitchClass::Cs);
        REQUIRE(PitchClass::Df.absolutePitch() == 1);
        REQUIRE(PitchClass::Cs.absolutePitch() == 1);
        REQUIRE(PitchClass::D == PitchClass::D);
        REQUIRE(PitchClass::D.absolutePitch() == 2);
    }

    SECTION("construct from MIDI value")
    {
        REQUIRE(PitchClass(0) == PitchClass::C);
        REQUIRE(PitchClass(1) == PitchClass::Cs);
        REQUIRE(PitchClass(2) == PitchClass::D);
        REQUIRE(PitchClass(3) == PitchClass::Ds);
        REQUIRE(PitchClass(4) == PitchClass::E);
        REQUIRE(PitchClass(5) == PitchClass::F);
        REQUIRE(PitchClass(6) == PitchClass::Fs);
        REQUIRE(PitchClass(7) == PitchClass::G);
        REQUIRE(PitchClass(8) == PitchClass::Gs);
        REQUIRE(PitchClass(9) == PitchClass::A);
        REQUIRE(PitchClass(10) == PitchClass::As);
        REQUIRE(PitchClass(11) == PitchClass::B);
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
        REQUIRE(PitchClass::Bss.toneUp().isError());
        REQUIRE_PITCH(PitchClass::Bs.toneUp(), PitchClass::Css);
        REQUIRE_PITCH(PitchClass::B.toneUp(), PitchClass::Cs);
        REQUIRE_PITCH(PitchClass::Bf.toneUp(), PitchClass::C);
        REQUIRE_PITCH(PitchClass::Bff.toneUp(), PitchClass::Cf);

        REQUIRE_PITCH(PitchClass::Ass.toneUp(), PitchClass::Bss);
        REQUIRE_PITCH(PitchClass::As.toneUp(), PitchClass::Bs);
        REQUIRE_PITCH(PitchClass::A.toneUp(), PitchClass::B);
        REQUIRE_PITCH(PitchClass::Af.toneUp(), PitchClass::Bf);
        REQUIRE_PITCH(PitchClass::Aff.toneUp(), PitchClass::Bff);

        REQUIRE_PITCH(PitchClass::Gss.toneUp(), PitchClass::Ass);
        REQUIRE_PITCH(PitchClass::Gs.toneUp(), PitchClass::As);
        REQUIRE_PITCH(PitchClass::G.toneUp(), PitchClass::A);
        REQUIRE_PITCH(PitchClass::Gf.toneUp(), PitchClass::Af);
        REQUIRE_PITCH(PitchClass::Gff.toneUp(), PitchClass::Aff);

        REQUIRE_PITCH(PitchClass::Fss.toneUp(), PitchClass::Gss);
        REQUIRE_PITCH(PitchClass::Fs.toneUp(), PitchClass::Gs);
        REQUIRE_PITCH(PitchClass::F.toneUp(), PitchClass::G);
        REQUIRE_PITCH(PitchClass::Ff.toneUp(), PitchClass::Gf);
        REQUIRE_PITCH(PitchClass::Fff.toneUp(), PitchClass::Gff);

        REQUIRE(PitchClass::Ess.toneUp().isError());
        REQUIRE_PITCH(PitchClass::Es.toneUp(), PitchClass::Fss);
        REQUIRE_PITCH(PitchClass::E.toneUp(), PitchClass::Fs);
        REQUIRE_PITCH(PitchClass::Ef.toneUp(), PitchClass::F);
        REQUIRE_PITCH(PitchClass::Eff.toneUp(), PitchClass::Ff);

        REQUIRE_PITCH(PitchClass::Dss.toneUp(), PitchClass::Ess);
        REQUIRE_PITCH(PitchClass::Ds.toneUp(), PitchClass::Es);
        REQUIRE_PITCH(PitchClass::D.toneUp(), PitchClass::E);
        REQUIRE_PITCH(PitchClass::Df.toneUp(), PitchClass::Ef);
        REQUIRE_PITCH(PitchClass::Dff.toneUp(), PitchClass::Eff);

        REQUIRE_PITCH(PitchClass::Css.toneUp(), PitchClass::Dss);
        REQUIRE_PITCH(PitchClass::Cs.toneUp(), PitchClass::Ds);
        REQUIRE_PITCH(PitchClass::C.toneUp(), PitchClass::D);
        REQUIRE_PITCH(PitchClass::Cf.toneUp(), PitchClass::Df);
        REQUIRE_PITCH(PitchClass::Cff.toneUp(), PitchClass::Dff);
    }

    SECTION("semitoneUp")
    {
        REQUIRE_PITCH(PitchClass::Bss.semitoneUp(), PitchClass::Css);
        REQUIRE_PITCH(PitchClass::Bs.semitoneUp(), PitchClass::Cs);
        REQUIRE_PITCH(PitchClass::B.semitoneUp(), PitchClass::C);
        REQUIRE_PITCH(PitchClass::Bf.semitoneUp(), PitchClass::Cf);
        REQUIRE_PITCH(PitchClass::Bff.semitoneUp(), PitchClass::Cff);

        REQUIRE_PITCH(PitchClass::Ass.semitoneUp(), PitchClass::Bs);
        REQUIRE_PITCH(PitchClass::As.semitoneUp(), PitchClass::B);
        REQUIRE_PITCH(PitchClass::A.semitoneUp(), PitchClass::Bf);
        REQUIRE_PITCH(PitchClass::Af.semitoneUp(), PitchClass::Bff);
        REQUIRE(PitchClass::Aff.semitoneUp().isError());

        REQUIRE_PITCH(PitchClass::Gss.semitoneUp(), PitchClass::As);
        REQUIRE_PITCH(PitchClass::Gs.semitoneUp(), PitchClass::A);
        REQUIRE_PITCH(PitchClass::G.semitoneUp(), PitchClass::Af);
        REQUIRE_PITCH(PitchClass::Gf.semitoneUp(), PitchClass::Aff);
        REQUIRE(PitchClass::Gff.semitoneUp().isError());

        REQUIRE_PITCH(PitchClass::Fss.semitoneUp(), PitchClass::Gs);
        REQUIRE_PITCH(PitchClass::Fs.semitoneUp(), PitchClass::G);
        REQUIRE_PITCH(PitchClass::F.semitoneUp(), PitchClass::Gf);
        REQUIRE_PITCH(PitchClass::Ff.semitoneUp(), PitchClass::Gff);
        REQUIRE(PitchClass::Fff.semitoneUp().isError());

        REQUIRE_PITCH(PitchClass::Ess.semitoneUp(), PitchClass::Fss);
        REQUIRE_PITCH(PitchClass::Es.semitoneUp(), PitchClass::Fs);
        REQUIRE_PITCH(PitchClass::E.semitoneUp(), PitchClass::F);
        REQUIRE_PITCH(PitchClass::Ef.semitoneUp(), PitchClass::Ff);
        REQUIRE_PITCH(PitchClass::Eff.semitoneUp(), PitchClass::Fff);

        REQUIRE_PITCH(PitchClass::Dss.semitoneUp(), PitchClass::Es);
        REQUIRE_PITCH(PitchClass::Ds.semitoneUp(), PitchClass::E);
        REQUIRE_PITCH(PitchClass::D.semitoneUp(), PitchClass::Ef);
        REQUIRE_PITCH(PitchClass::Df.semitoneUp(), PitchClass::Eff);
        REQUIRE(PitchClass::Dff.semitoneUp().isError());

        REQUIRE_PITCH(PitchClass::Css.semitoneUp(), PitchClass::Ds);
        REQUIRE_PITCH(PitchClass::Cs.semitoneUp(), PitchClass::D);
        REQUIRE_PITCH(PitchClass::C.semitoneUp(), PitchClass::Df);
        REQUIRE_PITCH(PitchClass::Cf.semitoneUp(), PitchClass::Dff);
        REQUIRE(PitchClass::Cff.semitoneUp().isError());
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

    SECTION("minSemitoneDistance")
    {
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Cff, 2);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Cf, 1);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, C, 0);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Cs, 1);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Css, 2);

        REQUIRE_MIN_SEMITONE_DISTANCE(C, Dff, 0);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Df, 1);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, D, 2);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Ds, 3);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Dss, 4);

        REQUIRE_MIN_SEMITONE_DISTANCE(C, Eff, 2);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Ef, 3);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, E, 4);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Es, 5);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Ess, 6);

        REQUIRE_MIN_SEMITONE_DISTANCE(C, Fff, 3);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Ff, 4);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, F, 5);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Fs, 6);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Fss, 5);

        REQUIRE_MIN_SEMITONE_DISTANCE(C, Gff, 5);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Gf, 6);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, G, 5);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Gs, 4);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Gss, 3);

        REQUIRE_MIN_SEMITONE_DISTANCE(C, Aff, 5);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Af, 4);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, A, 3);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, As, 2);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Ass, 1);

        REQUIRE_MIN_SEMITONE_DISTANCE(C, Bff, 3);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Bf, 2);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, B, 1);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Bs, 0);
        REQUIRE_MIN_SEMITONE_DISTANCE(C, Bss, 1);

        REQUIRE_MIN_SEMITONE_DISTANCE(B, Bff, 2);
        REQUIRE_MIN_SEMITONE_DISTANCE(B, Bf, 1);
        REQUIRE_MIN_SEMITONE_DISTANCE(B, B, 0);
        REQUIRE_MIN_SEMITONE_DISTANCE(B, Bs, 1);
        REQUIRE_MIN_SEMITONE_DISTANCE(Bs, B, 1);
        REQUIRE_MIN_SEMITONE_DISTANCE(Bss, B, 2);
        REQUIRE_MIN_SEMITONE_DISTANCE(Bss, Bss, 0);

        size_t total = PitchClass::all.size();
        for (size_t i = 0; i < 1000; i++) {
            const PitchClass& p1 = PitchClass::all[rand() % total];
            const PitchClass& p2 = PitchClass::all[rand() % total];

            size_t s1 = PitchClass::minSemitoneDistance(p1, p2);
            size_t s2 = PitchClass::minSemitoneDistance(p2, p1);

            REQUIRE(s1 == s2);
            REQUIRE(s1 < 7);
        }
    }

    SECTION("minSemitonesFromTo")
    {
        REQUIRE_MIN_SEMITONES_FROM_TO(C, C, 0);
        REQUIRE_MIN_SEMITONES_FROM_TO(C, Cs, 1);

        REQUIRE_MIN_SEMITONES_FROM_TO(C, Bss, 1);
        REQUIRE_MIN_SEMITONES_FROM_TO(Bss, C, -1);
        REQUIRE_MIN_SEMITONES_FROM_TO(C, Bs, 0);
        REQUIRE_MIN_SEMITONES_FROM_TO(Bs, C, 0);
        REQUIRE_MIN_SEMITONES_FROM_TO(C, B, -1);
        REQUIRE_MIN_SEMITONES_FROM_TO(B, C, 1);
        REQUIRE_MIN_SEMITONES_FROM_TO(C, Bf, -2);
        REQUIRE_MIN_SEMITONES_FROM_TO(Bf, C, 2);
        REQUIRE_MIN_SEMITONES_FROM_TO(C, Bff, -3);
        REQUIRE_MIN_SEMITONES_FROM_TO(Bff, C, 3);

        REQUIRE_MIN_SEMITONES_FROM_TO(B, Bs, 1);
        REQUIRE_MIN_SEMITONES_FROM_TO(Bs, B, -1);
        REQUIRE_MIN_SEMITONES_FROM_TO(Bss, B, -2);

        size_t total = PitchClass::all.size();
        for (size_t i = 0; i < 1000; i++) {
            const PitchClass& p1 = PitchClass::all[rand() % total];
            const PitchClass& p2 = PitchClass::all[rand() % total];

            int s1 = PitchClass::minSemitonesFromTo(p1, p2);
            int s2 = PitchClass::minSemitonesFromTo(p2, p1);

            REQUIRE(s1 == -s2);
            REQUIRE(abs(s1) < 7);
        }
    }

    SECTION("alterate to pattern")
    {
        // C
        REQUIRE_CAN_ALT_TO(C, Cff);
        REQUIRE_CAN_ALT_TO(C, Cf);
        REQUIRE_CAN_ALT_TO(C, C);
        REQUIRE_CAN_ALT_TO(C, Cs);
        REQUIRE_CAN_ALT_TO(C, Css);

        REQUIRE_CAN_ALT_TO(C, Dff);
        REQUIRE_CAN_ALT_TO(C, Df);
        REQUIRE_CAN_ALT_TO(C, D);
        REQUIRE_NO_ALT_TO(C, Ds);
        REQUIRE_NO_ALT_TO(C, Dss);

        REQUIRE_CAN_ALT_TO(C, Bss);
        REQUIRE_CAN_ALT_TO(C, Bs);
        REQUIRE_CAN_ALT_TO(C, B);
        REQUIRE_CAN_ALT_TO(C, Bf);
        REQUIRE_NO_ALT_TO(C, Bff);

        REQUIRE_CAN_ALT_TO(C, Eff);
        REQUIRE_NO_ALT_TO(C, Ef);
        REQUIRE_NO_ALT_TO(C, E);
        REQUIRE_NO_ALT_TO(C, Es);
        REQUIRE_NO_ALT_TO(C, Ess);

        REQUIRE_NO_ALT_TO(C, Aff);
        REQUIRE_NO_ALT_TO(C, Af);
        REQUIRE_NO_ALT_TO(C, A);
        REQUIRE_CAN_ALT_TO(C, As);
        REQUIRE_CAN_ALT_TO(C, Ass);

        // D
        REQUIRE_NO_ALT_TO(D, Cff);
        REQUIRE_NO_ALT_TO(D, Cf);
        REQUIRE_CAN_ALT_TO(D, C);
        REQUIRE_CAN_ALT_TO(D, Cs);
        REQUIRE_CAN_ALT_TO(D, Css);

        REQUIRE_CAN_ALT_TO(D, Eff);
        REQUIRE_CAN_ALT_TO(D, Ef);
        REQUIRE_CAN_ALT_TO(D, E);
        REQUIRE_NO_ALT_TO(D, Es);
        REQUIRE_NO_ALT_TO(D, Ess);

        REQUIRE_CAN_ALT_TO(D, Fff);
        REQUIRE_CAN_ALT_TO(D, Ff);
        REQUIRE_NO_ALT_TO(D, F);
        REQUIRE_NO_ALT_TO(D, Fs);
        REQUIRE_NO_ALT_TO(D, Fss);

        REQUIRE_NO_ALT_TO(D, Bff);
        REQUIRE_NO_ALT_TO(D, Bf);
        REQUIRE_NO_ALT_TO(D, B);
        REQUIRE_CAN_ALT_TO(D, Bs);
        REQUIRE_CAN_ALT_TO(D, Bss);

        // E
        REQUIRE_NO_ALT_TO(E, Dff);
        REQUIRE_NO_ALT_TO(E, Df);
        REQUIRE_CAN_ALT_TO(E, D);
        REQUIRE_CAN_ALT_TO(E, Ds);
        REQUIRE_CAN_ALT_TO(E, Dss);

        REQUIRE_CAN_ALT_TO(E, Fff);
        REQUIRE_CAN_ALT_TO(E, Ff);
        REQUIRE_CAN_ALT_TO(E, F);
        REQUIRE_CAN_ALT_TO(E, Fs);
        REQUIRE_NO_ALT_TO(E, Fss);

        REQUIRE_NO_ALT_TO(E, Cff);
        REQUIRE_NO_ALT_TO(E, Cf);
        REQUIRE_NO_ALT_TO(E, C);
        REQUIRE_NO_ALT_TO(E, Cs);
        REQUIRE_CAN_ALT_TO(E, Css);

        REQUIRE_CAN_ALT_TO(E, Gff);
        REQUIRE_CAN_ALT_TO(E, Gf);
        REQUIRE_NO_ALT_TO(E, G);
        REQUIRE_NO_ALT_TO(E, Gs);
        REQUIRE_NO_ALT_TO(E, Gss);

        // B
        REQUIRE_CAN_ALT_TO(B, Bff);
        REQUIRE_CAN_ALT_TO(B, Bf);
        REQUIRE_CAN_ALT_TO(B, B);
        REQUIRE_CAN_ALT_TO(B, Bs);
        REQUIRE_CAN_ALT_TO(B, Bss);

        REQUIRE_CAN_ALT_TO(B, Cff);
        REQUIRE_CAN_ALT_TO(B, Cf);
        REQUIRE_CAN_ALT_TO(B, C);
        REQUIRE_CAN_ALT_TO(B, Cs);
        REQUIRE_NO_ALT_TO(B, Css);

        REQUIRE_NO_ALT_TO(B, Aff);
        REQUIRE_NO_ALT_TO(B, Af);
        REQUIRE_CAN_ALT_TO(B, A);
        REQUIRE_CAN_ALT_TO(B, As);
        REQUIRE_CAN_ALT_TO(B, Ass);

        REQUIRE_CAN_ALT_TO(B, Dff);
        REQUIRE_CAN_ALT_TO(B, Df);
        REQUIRE_NO_ALT_TO(B, D);
        REQUIRE_NO_ALT_TO(B, Ds);
        REQUIRE_NO_ALT_TO(B, Dss);

        REQUIRE_NO_ALT_TO(B, Gff);
        REQUIRE_NO_ALT_TO(B, Gf);
        REQUIRE_NO_ALT_TO(B, G);
        REQUIRE_NO_ALT_TO(B, Gs);
        REQUIRE_CAN_ALT_TO(B, Gss);

        //
        REQUIRE_CAN_ALT_TO(E, F);
        REQUIRE_CAN_ALT_TO(E, Fs);
        REQUIRE_CAN_ALT_TO(E, Ff);
    }

    SECTION("enharmonic")
    {
        SECTION("equal")
        {
            REQUIRE(PitchClass::Cs.enharmonicEqual(PitchClass::Df));

            size_t total = PitchClass::all.size();
            for (size_t i = 0; i < 1000; i++) {
                const PitchClass& p1 = PitchClass::all[rand() % total];
                const PitchClass& p2 = PitchClass::all[rand() % total];

                if (p1.enharmonicEqual(p2)) {
                    REQUIRE(p2.enharmonicEqual(p1));

                    Enharmonics e1 = p1.enharmonics();
                    e1.push_back(p1);
                    Enharmonics e2 = p2.enharmonics();
                    e2.push_back(p2);

                    std::sort(e1.begin(), e1.end(), pitchClassCmp);
                    std::sort(e2.begin(), e2.end(), pitchClassCmp);

                    REQUIRE(e1 == e2);
                } else {
                    REQUIRE_FALSE(p2.enharmonicEqual(p1));
                }
            }
        }

        SECTION("upper")
        {
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
            REQUIRE_UP_ENHARM_2(Bs, C, Dff);
            REQUIRE_UP_ENHARM(B, Cf);

            size_t total = PitchClass::all.size();
            for (size_t i = 0; i < 100; i++) {
                const PitchClass& p = PitchClass::all[rand() % total];
                Enharmonics e = p.upperEnharmonics();

                for (size_t j = 0; j < e.size(); j++) {
                    REQUIRE(p.enharmonicEqual(e[j]));
                }
            }
        }

        SECTION("lower")
        {
            REQUIRE_LOW_ENHARM_2(Cff, As, Bf);
            REQUIRE_LOW_ENHARM_2(Cf, Ass, B);
            REQUIRE_LOW_ENHARM(C, Bs);
            REQUIRE_LOW_ENHARM(Cs, Bss);
            REQUIRE_NO_LOW_ENHARM(Css);

            REQUIRE_LOW_ENHARM_2(Dff, Bs, C);
            REQUIRE_LOW_ENHARM_2(Df, Bss, Cs);
            REQUIRE_LOW_ENHARM(D, Css);
            REQUIRE_NO_LOW_ENHARM(Ds);
            REQUIRE_NO_LOW_ENHARM(Dss);

            REQUIRE_LOW_ENHARM_2(Eff, Css, D);
            REQUIRE_LOW_ENHARM(Ef, Ds);
            REQUIRE_LOW_ENHARM(E, Dss);
            REQUIRE_NO_LOW_ENHARM(Es);
            REQUIRE_NO_LOW_ENHARM(Ess);

            REQUIRE_LOW_ENHARM_2(Fff, Ds, Ef);
            REQUIRE_LOW_ENHARM_2(Ff, Dss, E);
            REQUIRE_LOW_ENHARM(F, Es);
            REQUIRE_LOW_ENHARM(Fs, Ess);
            REQUIRE_NO_LOW_ENHARM(Fss);

            REQUIRE_LOW_ENHARM_2(Gff, Es, F);
            REQUIRE_LOW_ENHARM_2(Gf, Ess, Fs);
            REQUIRE_LOW_ENHARM(G, Fss);
            REQUIRE_NO_LOW_ENHARM(Gs);
            REQUIRE_NO_LOW_ENHARM(Gss);

            REQUIRE_LOW_ENHARM_2(Aff, Fss, G);
            REQUIRE_LOW_ENHARM(Af, Gs);
            REQUIRE_LOW_ENHARM(A, Gss);
            REQUIRE_NO_LOW_ENHARM(As);
            REQUIRE_NO_LOW_ENHARM(Ass);

            REQUIRE_LOW_ENHARM_2(Bff, Gss, A);
            REQUIRE_LOW_ENHARM(Bf, As);
            REQUIRE_LOW_ENHARM(B, Ass);
            REQUIRE_NO_LOW_ENHARM(Bs);
            REQUIRE_NO_LOW_ENHARM(Bss);

            size_t total = PitchClass::all.size();
            for (size_t i = 0; i < 100; i++) {
                const PitchClass& p = PitchClass::all[rand() % total];
                Enharmonics e = p.lowerEnharmonics();

                for (size_t j = 0; j < e.size(); j++) {
                    REQUIRE(p.enharmonicEqual(e[j]));
                }
            }
        }

        SECTION("both")
        {
            size_t total = PitchClass::all.size();
            for (size_t i = 0; i < 100; i++) {
                const PitchClass& p = PitchClass::all[rand() % total];
                Enharmonics e = p.enharmonics();

                REQUIRE(e.size() > 0);

                for (size_t j = 0; j < e.size(); j++) {
                    REQUIRE(p.enharmonicEqual(e[j]));
                }
            }
        }
    }

    SECTION("alterate")
    {
        REQUIRE_PITCH(PitchClass::C.alterate(1), PitchClass::Cs);
        REQUIRE_PITCH(PitchClass::C.alterate(2), PitchClass::Css);
        REQUIRE_PITCH(PitchClass::C.alterate(0), PitchClass::C);
        REQUIRE_PITCH(PitchClass::C.alterate(-1), PitchClass::Cf);
        REQUIRE_PITCH(PitchClass::C.alterate(-2), PitchClass::Cff);

        REQUIRE(PitchClass::C.alterate(3).isError());
        REQUIRE(PitchClass::C.alterate(-3).isError());
    }

    SECTION("to_string")
    {
        REQUIRE_TO_STR(Cs, "c#", NAMING_SCHEME_GUIDO);
        REQUIRE_TO_STR(Cs, "C sharp", NAMING_SCHEME_ENGLISH);
        REQUIRE_TO_STR(Cs, "C#", NAMING_SCHEME_SPN);

        REQUIRE_TO_STR(E, "E", NAMING_SCHEME_ENGLISH);

        REQUIRE_TO_STR(Cs, "Cis", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(B, "H", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Bs, "His", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Bss, "Hisis", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Bf, "B", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Bff, "Bes", NAMING_SCHEME_GERMAN);

        REQUIRE_TO_STR(E, "E", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Ef, "Es", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Eff, "Eses", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Es, "Eis", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Ess, "Eisis", NAMING_SCHEME_GERMAN);

        REQUIRE_TO_STR(A, "A", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Af, "As", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Aff, "Ases", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(As, "Ais", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Ass, "Aisis", NAMING_SCHEME_GERMAN);

        REQUIRE_TO_STR(F, "F", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Ff, "Fes", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Fff, "Feses", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Fs, "Fis", NAMING_SCHEME_GERMAN);
        REQUIRE_TO_STR(Fss, "Fisis", NAMING_SCHEME_GERMAN);

        REQUIRE_TO_STR(F, "F", NAMING_SCHEME_ABC);
        REQUIRE_TO_STR(Ff, "_F", NAMING_SCHEME_ABC);
        REQUIRE_TO_STR(Fff, "__F", NAMING_SCHEME_ABC);
        REQUIRE_TO_STR(Fs, "^F", NAMING_SCHEME_ABC);
        REQUIRE_TO_STR(Fss, "^^F", NAMING_SCHEME_ABC);

        REQUIRE_TO_STR(F, "f", NAMING_SCHEME_GUIDO);
        REQUIRE_TO_STR(Ff, "f&", NAMING_SCHEME_GUIDO);
        REQUIRE_TO_STR(Fff, "f&&", NAMING_SCHEME_GUIDO);
        REQUIRE_TO_STR(Fs, "f#", NAMING_SCHEME_GUIDO);
        REQUIRE_TO_STR(Fss, "f##", NAMING_SCHEME_GUIDO);

        REQUIRE_TO_STR(F, "F", NAMING_SCHEME_SPN);
        REQUIRE_TO_STR(Ff, "Fb", NAMING_SCHEME_SPN);
        REQUIRE_TO_STR(Fff, "Fbb", NAMING_SCHEME_SPN);
        REQUIRE_TO_STR(Fs, "F#", NAMING_SCHEME_SPN);
        REQUIRE_TO_STR(Fss, "F##", NAMING_SCHEME_SPN);
    }

    SECTION("from_string")
    {
        PitchClass p = PitchClass::C;
        REQUIRE(from_string("A flat", p));
        REQUIRE(p == PitchClass::Af);
        REQUIRE(from_string("A double flat", p));
        REQUIRE(p == PitchClass::Aff);
        REQUIRE(from_string("A", p));
        REQUIRE(p == PitchClass::A);
        REQUIRE_FALSE(from_string("A flat", p, NAMING_SCHEME_ABC));

        REQUIRE(from_string("La", p, NAMING_SCHEME_ITALIAN));
        REQUIRE(p == PitchClass::A);
        REQUIRE(from_string("La ", p, NAMING_SCHEME_ITALIAN));
        REQUIRE(p == PitchClass::A);
        REQUIRE(from_string("La diesis", p, NAMING_SCHEME_ITALIAN));
        REQUIRE(p == PitchClass::As);
        REQUIRE(from_string("La doppio diesis", p, NAMING_SCHEME_ITALIAN));
        REQUIRE(p == PitchClass::Ass);
        REQUIRE(from_string("La bemolle", p, NAMING_SCHEME_ITALIAN));
        REQUIRE(p == PitchClass::Af);
        REQUIRE(from_string("La doppio bemolle", p, NAMING_SCHEME_ITALIAN));
        REQUIRE(p == PitchClass::Aff);

        REQUIRE(from_string("Ля", p, NAMING_SCHEME_RUSSIAN));
        REQUIRE(p == PitchClass::A);
        REQUIRE(from_string("Ля ", p, NAMING_SCHEME_RUSSIAN));
        REQUIRE(p == PitchClass::A);
        REQUIRE(from_string("Ля диез", p, NAMING_SCHEME_RUSSIAN));
        REQUIRE(p == PitchClass::As);
        REQUIRE(from_string("Ля дубль-диез", p, NAMING_SCHEME_RUSSIAN));
        REQUIRE(p == PitchClass::Ass);
        REQUIRE(from_string("Ля бемоль", p, NAMING_SCHEME_RUSSIAN));
        REQUIRE(p == PitchClass::Af);
        REQUIRE(from_string("Ля дубль-бемоль", p, NAMING_SCHEME_RUSSIAN));
        REQUIRE(p == PitchClass::Aff);
    }

    SECTION("from/to")
    {
        for (size_t i = 0; i < PitchClass::all.size(); i++) {
            for (size_t ns = 0; ns < NAMING_SCHEME_ALL; ns++) {
                //                std::cerr << "\tNS: " << ns << "\n";

                std::string str = to_string(PitchClass::all[i], NamingScheme(ns));
                REQUIRE(str.size() > 0);

                PitchClass c = PitchClass::C;
                if (!from_string(str, c, NamingScheme(ns)))
                    std::cerr << "\t" << str << "\n";

                if (PitchClass::all[i] != c)
                    std::cerr << "\t" << str << "\tNS: " << ns << "\n";

                //                REQUIRE(from_string(str, c, NamingScheme(ns)));
                REQUIRE(PitchClass::all[i] == c);
            }
        }
    }
}
