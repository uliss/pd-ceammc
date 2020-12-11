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

#include <cstdlib>
#include <ctime>
#include <sstream>

using namespace ceammc;
using namespace ceammc::music;

#define REQUIRE_STR(P, STR)      \
    {                            \
        std::ostringstream s;    \
        s << PitchName::P;       \
        REQUIRE(s.str() == STR); \
    }

#ifdef REQUIRE_INDEX
#undef REQUIRE_INDEX
#endif

#define REQUIRE_DISTANCE(p1, p2, d) REQUIRE(PitchName::distance(PitchName::p1, PitchName::p2) == d)
#define REQUIRE_MIN_DISTANCE(p1, p2, d) REQUIRE(PitchName::minDistance(PitchName::p1, PitchName::p2) == d)
#define REQUIRE_UP_STEPS(from, to, d) REQUIRE(PitchName::upSteps(PitchName::from, PitchName::to) == d)
#define REQUIRE_DOWN_STEPS(from, to, d) REQUIRE(PitchName::downSteps(PitchName::from, PitchName::to) == d)
#define REQUIRE_MIN_STEPS(from, to, d) REQUIRE(PitchName::minSteps(PitchName::from, PitchName::to) == d)
#define REQUIRE_INDEX(p, idx) REQUIRE(PitchName::p.index() == idx)
#define REQUIRE_ABS_PITCH(p, abs_p) REQUIRE(PitchName::p.absolutePitch() == abs_p)

TEST_CASE("MusicTheory::PitchName", "[ceammc::music]")
{
    SECTION("equal")
    {
        REQUIRE(PitchName::C == PitchName::C);
        REQUIRE(PitchName::D == PitchName::D);
        REQUIRE(PitchName::E == PitchName::E);
        REQUIRE(PitchName::F == PitchName::F);
        REQUIRE(PitchName::G == PitchName::G);
        REQUIRE(PitchName::A == PitchName::A);
        REQUIRE(PitchName::B == PitchName::B);

        REQUIRE(PitchName::C != PitchName::D);
        REQUIRE(PitchName::D != PitchName::E);
        REQUIRE(PitchName::E != PitchName::F);

        REQUIRE(PitchName::C.type() == PITCH_NAME_C);
        REQUIRE(PitchName::D.type() == PITCH_NAME_D);
        REQUIRE(PitchName::E.type() == PITCH_NAME_E);
        REQUIRE(PitchName::F.type() == PITCH_NAME_F);
        REQUIRE(PitchName::G.type() == PITCH_NAME_G);
        REQUIRE(PitchName::A.type() == PITCH_NAME_A);
        REQUIRE(PitchName::B.type() == PITCH_NAME_B);
    }

    SECTION("add")
    {
        REQUIRE(PitchName::C + 1 == PitchName::D);
        REQUIRE(PitchName::C + 2 == PitchName::E);
        REQUIRE(PitchName::C + 3 == PitchName::F);
        REQUIRE(PitchName::C + 4 == PitchName::G);
        REQUIRE(PitchName::C + 5 == PitchName::A);
        REQUIRE(PitchName::C + 6 == PitchName::B);
        REQUIRE(PitchName::C + 7 == PitchName::C);
        REQUIRE(PitchName::C + 50 == PitchName::D);
        REQUIRE(PitchName::C + 702 == PitchName::E);

        REQUIRE(PitchName::C + -1 == PitchName::B);
        REQUIRE(PitchName::C + -2 == PitchName::A);
        REQUIRE(PitchName::C + -3 == PitchName::G);
        REQUIRE(PitchName::C + -4 == PitchName::F);
        REQUIRE(PitchName::C + -5 == PitchName::E);
        REQUIRE(PitchName::C + -6 == PitchName::D);
        REQUIRE(PitchName::C + -7 == PitchName::C);
        REQUIRE(PitchName::C + -50 == PitchName::B);
        REQUIRE(PitchName::C + -702 == PitchName::A);

        REQUIRE(PitchName::C - 1 == PitchName::B);
        REQUIRE(PitchName::C - 2 == PitchName::A);
        REQUIRE(PitchName::C - 3 == PitchName::G);
        REQUIRE(PitchName::C - 4 == PitchName::F);
        REQUIRE(PitchName::C - 5 == PitchName::E);
        REQUIRE(PitchName::C - 6 == PitchName::D);
        REQUIRE(PitchName::C - 7 == PitchName::C);
        REQUIRE(PitchName::C - 50 == PitchName::B);
        REQUIRE(PitchName::C - 702 == PitchName::A);

        REQUIRE(PitchName::C - -1 == PitchName::D);
        REQUIRE(PitchName::C - -2 == PitchName::E);
        REQUIRE(PitchName::C - -3 == PitchName::F);
        REQUIRE(PitchName::C - -4 == PitchName::G);
        REQUIRE(PitchName::C - -5 == PitchName::A);
        REQUIRE(PitchName::C - -6 == PitchName::B);
        REQUIRE(PitchName::C - -7 == PitchName::C);
        REQUIRE(PitchName::C - -50 == PitchName::D);
        REQUIRE(PitchName::C - -702 == PitchName::E);
    }

    SECTION("operator<<")
    {
        REQUIRE_STR(C, "C");
        REQUIRE_STR(D, "D");
        REQUIRE_STR(E, "E");
        REQUIRE_STR(F, "F");
        REQUIRE_STR(G, "G");
        REQUIRE_STR(A, "A");
        REQUIRE_STR(B, "B");
    }

    SECTION("distance")
    {
        REQUIRE_DISTANCE(C, C, 0);
        REQUIRE_DISTANCE(C, D, 1);
        REQUIRE_DISTANCE(D, C, 1);
        REQUIRE_DISTANCE(C, E, 2);
        REQUIRE_DISTANCE(C, B, 6);
        REQUIRE_DISTANCE(B, C, 6);
    }

    SECTION("minDistance")
    {
        REQUIRE_MIN_DISTANCE(C, C, 0);
        REQUIRE_MIN_DISTANCE(C, D, 1);
        REQUIRE_MIN_DISTANCE(D, C, 1);
        REQUIRE_MIN_DISTANCE(C, E, 2);
        REQUIRE_MIN_DISTANCE(C, B, 1);
        REQUIRE_MIN_DISTANCE(B, C, 1);
        REQUIRE_MIN_DISTANCE(C, F, 3);
        REQUIRE_MIN_DISTANCE(C, G, 3);
        REQUIRE_MIN_DISTANCE(C, A, 2);
    }

    SECTION("upSteps")
    {
        REQUIRE_UP_STEPS(C, C, 0);
        REQUIRE_UP_STEPS(C, D, 1);
        REQUIRE_UP_STEPS(C, E, 2);
        REQUIRE_UP_STEPS(C, B, 6);
        REQUIRE_UP_STEPS(B, C, 1);
        REQUIRE_UP_STEPS(D, C, 6);
        REQUIRE_UP_STEPS(A, C, 2);
        REQUIRE_UP_STEPS(C, A, 5);
    }

    SECTION("downSteps")
    {
        REQUIRE_DOWN_STEPS(C, C, 0);
        REQUIRE_DOWN_STEPS(C, D, 6);
        REQUIRE_DOWN_STEPS(C, E, 5);
        REQUIRE_DOWN_STEPS(C, B, 1);
        REQUIRE_DOWN_STEPS(B, C, 6);
        REQUIRE_DOWN_STEPS(D, C, 1);
        REQUIRE_DOWN_STEPS(A, C, 5);
        REQUIRE_DOWN_STEPS(C, A, 2);
    }

    SECTION("minSteps")
    {
        REQUIRE_MIN_STEPS(C, C, 0);
        REQUIRE_MIN_STEPS(C, D, 1);
        REQUIRE_MIN_STEPS(C, E, 2);
        REQUIRE_MIN_STEPS(C, F, 3);
        REQUIRE_MIN_STEPS(C, G, -3);
        REQUIRE_MIN_STEPS(C, B, -1);
        REQUIRE_MIN_STEPS(B, C, 1);
        REQUIRE_MIN_STEPS(D, C, -1);
        REQUIRE_MIN_STEPS(A, C, 2);
        REQUIRE_MIN_STEPS(C, A, -2);

        srand(time(0));

        for (size_t i = 0; i < 500; i++) {
            PitchName p1 = PitchName::C + rand() % 7;
            PitchName p2 = PitchName::C + rand() % 7;

            REQUIRE(abs(PitchName::minSteps(p1, p2)) <= 3);
            REQUIRE(PitchName::minDistance(p1, p2) <= 3);
        }
    }

    SECTION("index")
    {
        REQUIRE_INDEX(C, 0);
        REQUIRE_INDEX(D, 1);
        REQUIRE_INDEX(E, 2);
        REQUIRE_INDEX(F, 3);
        REQUIRE_INDEX(G, 4);
        REQUIRE_INDEX(A, 5);
        REQUIRE_INDEX(B, 6);
    }

    SECTION("absolutePitch")
    {
        REQUIRE_ABS_PITCH(C, 0);
        REQUIRE_ABS_PITCH(D, 2);
        REQUIRE_ABS_PITCH(E, 4);
        REQUIRE_ABS_PITCH(F, 5);
        REQUIRE_ABS_PITCH(G, 7);
        REQUIRE_ABS_PITCH(A, 9);
        REQUIRE_ABS_PITCH(B, 11);
    }

    SECTION("to_string")
    {
        REQUIRE(to_string(PitchName::C) == "C");
        REQUIRE(to_string(PitchName::C, NAMING_SCHEME_ITALIAN) == "Do");

        for (int ns = 0; ns < NAMING_SCHEME_ABC; ns++) {
            for (int i = 0; i < 7; i++) {
                PitchName name0 = PitchName::C + i;
                std::string str = to_string(name0, NamingScheme(ns));

                PitchName name1 = PitchName::C;
                REQUIRE(from_string(str, name1, NamingScheme(ns)));
            }
        }
    }

    SECTION("from_string")
    {
        PitchName n = PitchName::C;
        REQUIRE_FALSE(from_string("Sol", n));
        REQUIRE(n == PitchName::C);
        REQUIRE_FALSE(from_string("Sol", n, NAMING_SCHEME_ENGLISH));
        REQUIRE(n == PitchName::C);
        REQUIRE(from_string("Sol", n, NAMING_SCHEME_ITALIAN));
        REQUIRE(n == PitchName::G);
        REQUIRE(from_string("La", n, NAMING_SCHEME_ALL));
        REQUIRE(n == PitchName::A);

        REQUIRE_FALSE(from_string("???", n));
        REQUIRE_FALSE(from_string("???", n, NAMING_SCHEME_ALL));
    }
}
