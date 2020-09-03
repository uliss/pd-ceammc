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
#include "ceammc_music_theory_pitch_class.h"
#include "ceammc_music_theory_names.h"

#include "catch.hpp"

#include <cstdlib>
#include <ctime>
#include <set>
#include <sstream>

using namespace ceammc;
using namespace ceammc::music;

#define REQUIRE_ALTERATE(a, n)         \
    {                                  \
        Alteration at = Alteration::a; \
        REQUIRE(at.alterate(n));       \
    }

#define REQUIRE_NOT_ALTERATE(a, n)     \
    {                                  \
        Alteration at = Alteration::a; \
        REQUIRE_FALSE(at.alterate(n)); \
    }

TEST_CASE("MusicTheory::Alteration", "[ceammc::music]")
{
    SECTION("construct")
    {
        REQUIRE(Alteration::DOUBLE_FLAT == Alteration::DOUBLE_FLAT);
        REQUIRE(Alteration::DOUBLE_FLAT != Alteration::FLAT);

        REQUIRE(Alteration::DOUBLE_FLAT.type() == ALTERATION_DOUBLE_FLAT);
        REQUIRE(Alteration::DOUBLE_FLAT.semitones() == -2);
        REQUIRE(Alteration::DOUBLE_FLAT.cents() == -200);
    }

    SECTION("compare")
    {
        REQUIRE(Alteration::DOUBLE_FLAT < Alteration::FLAT);
        REQUIRE(Alteration::FLAT < Alteration::NATURAL);
        REQUIRE(Alteration::NATURAL < Alteration::SHARP);
        REQUIRE(Alteration::SHARP < Alteration::DOUBLE_SHARP);
        REQUIRE(Alteration::DOUBLE_FLAT < Alteration::DOUBLE_SHARP);

        REQUIRE(Alteration::SHARP > Alteration::FLAT);
        REQUIRE(Alteration::SHARP >= Alteration::FLAT);
        REQUIRE(Alteration::SHARP < Alteration::DOUBLE_SHARP);
        REQUIRE(Alteration::SHARP <= Alteration::DOUBLE_SHARP);
    }

    SECTION("increment/decrement")
    {
        Alteration a = Alteration::DOUBLE_FLAT;
        REQUIRE(++a);
        REQUIRE(a == Alteration::FLAT);
        REQUIRE(++a);
        REQUIRE(a == Alteration::NATURAL);
        REQUIRE(++a);
        REQUIRE(a == Alteration::SHARP);
        REQUIRE(++a);
        REQUIRE(a == Alteration::DOUBLE_SHARP);
        REQUIRE_FALSE(++a);
        REQUIRE(a == Alteration::DOUBLE_SHARP);

        REQUIRE(--a);
        REQUIRE(a == Alteration::SHARP);
        REQUIRE(--a);
        REQUIRE(a == Alteration::NATURAL);
        REQUIRE(--a);
        REQUIRE(a == Alteration::FLAT);
        REQUIRE(--a);
        REQUIRE(a == Alteration::DOUBLE_FLAT);
        REQUIRE_FALSE(--a);
        REQUIRE(a == Alteration::DOUBLE_FLAT);
    }

    SECTION("names")
    {
        REQUIRE(Alteration::DOUBLE_FLAT.shortName() == std::string("bb"));
        REQUIRE(Alteration::DOUBLE_FLAT.fullName() == std::string("double flat"));

        REQUIRE(Alteration::FLAT.shortName() == std::string("b"));
        REQUIRE(Alteration::FLAT.fullName() == std::string("flat"));

        REQUIRE(Alteration::NATURAL.shortName() == std::string(""));
        REQUIRE(Alteration::NATURAL.fullName() == std::string("natural"));

        REQUIRE(Alteration::SHARP.shortName() == std::string("#"));
        REQUIRE(Alteration::SHARP.fullName() == std::string("sharp"));

        REQUIRE(Alteration::DOUBLE_SHARP.shortName() == std::string("##"));
        REQUIRE(Alteration::DOUBLE_SHARP.fullName() == std::string("double sharp"));

        std::ostringstream s;
        s << Alteration::DOUBLE_SHARP;
        REQUIRE(s.str() == "##");
    }

    SECTION("alterate")
    {
        REQUIRE_ALTERATE(NATURAL, 0);
        REQUIRE_ALTERATE(NATURAL, 1);
        REQUIRE_ALTERATE(NATURAL, 2);
        REQUIRE_ALTERATE(NATURAL, -1);
        REQUIRE_ALTERATE(NATURAL, -2);
        REQUIRE_NOT_ALTERATE(NATURAL, 3);
        REQUIRE_NOT_ALTERATE(NATURAL, -3);

        REQUIRE_ALTERATE(DOUBLE_FLAT, 0);
        REQUIRE_ALTERATE(DOUBLE_FLAT, 1);
        REQUIRE_ALTERATE(DOUBLE_FLAT, 2);
        REQUIRE_ALTERATE(DOUBLE_FLAT, 3);
        REQUIRE_ALTERATE(DOUBLE_FLAT, 4);
        REQUIRE_NOT_ALTERATE(DOUBLE_FLAT, 5);
        REQUIRE_NOT_ALTERATE(DOUBLE_FLAT, -1);

        REQUIRE_ALTERATE(DOUBLE_SHARP, 0);
        REQUIRE_ALTERATE(DOUBLE_SHARP, -1);
        REQUIRE_ALTERATE(DOUBLE_SHARP, -2);
        REQUIRE_ALTERATE(DOUBLE_SHARP, -3);
        REQUIRE_ALTERATE(DOUBLE_SHARP, -4);
        REQUIRE_NOT_ALTERATE(DOUBLE_SHARP, -5);
        REQUIRE_NOT_ALTERATE(DOUBLE_SHARP, 1);
    }

    SECTION("names")
    {
        REQUIRE(to_string(Alteration::FLAT) == "flat");
        REQUIRE(to_string(Alteration::SHARP) == "sharp");
        REQUIRE(to_string(Alteration::DOUBLE_SHARP) == "double sharp");
        REQUIRE(to_string(Alteration::DOUBLE_SHARP, NAMING_SCHEME_ITALIAN) == "doppio diesis");
        REQUIRE(to_string(Alteration::DOUBLE_SHARP, NAMING_SCHEME_GERMAN) == "Doppelkreuz");
        REQUIRE(to_string(Alteration::DOUBLE_SHARP, NAMING_SCHEME_FRENCH) == "double dièse");
        REQUIRE(to_string(Alteration::DOUBLE_SHARP, NAMING_SCHEME_RUSSIAN) == "дубль-диез");
        REQUIRE(to_string(Alteration::DOUBLE_SHARP, NAMING_SCHEME_SPN) == "##");
        REQUIRE(to_string(Alteration::DOUBLE_SHARP, NAMING_SCHEME_SPN_UTF8) == "♯♯");
        REQUIRE(to_string(Alteration::DOUBLE_SHARP, NAMING_SCHEME_GUIDO) == "##");
        REQUIRE(to_string(Alteration::DOUBLE_SHARP, NAMING_SCHEME_ABC) == "^^");

        for (int ns = 0; ns <= NAMING_SCHEME_ABC; ns++) {
            Alteration a = Alteration::DOUBLE_FLAT;
            std::set<std::string> names;
            do {
                std::string s = to_string(a, (NamingScheme)ns);
                names.insert(s);
            } while (++a);

            // check for unique name
            REQUIRE(names.size() == 5);
        }
    }

    SECTION("to/from string")
    {
        for (size_t ns = 0; ns < NAMING_SCHEME_ALL; ns++) {
            Alteration a0 = Alteration::DOUBLE_FLAT;
            do {
                std::string str = to_string(a0, NamingScheme(ns));
                Alteration a1 = Alteration::NATURAL;
                REQUIRE(from_string(str, a1, NamingScheme(ns)));
                REQUIRE(a0 == a1);
            } while (a0.alterate(1));
        }
    }

    SECTION("from string")
    {
        Alteration a = Alteration::NATURAL;
        REQUIRE(from_string("sharp", a));
        REQUIRE(a == Alteration::SHARP);

        REQUIRE(from_string("##", a, NAMING_SCHEME_SPN));
        REQUIRE(a == Alteration::DOUBLE_SHARP);

        REQUIRE_FALSE(from_string("&", a, NAMING_SCHEME_SPN));
        REQUIRE(a == Alteration::DOUBLE_SHARP);

        REQUIRE(from_string("&", a, NAMING_SCHEME_ALL));
        REQUIRE(a == Alteration::FLAT);

        REQUIRE(from_string("", a, NAMING_SCHEME_ALL));
        REQUIRE(a == Alteration::NATURAL);
    }
}
