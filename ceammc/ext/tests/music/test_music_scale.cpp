/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "ceammc_crc32.h"
#include "ceammc_music_scale.h"
#include "test_base.h"

#include "catch.hpp"

using namespace ceammc;
using namespace ceammc::music;

TEST_CASE("scale", "[ceammc::music]")
{
    SECTION("degree to scale")
    {
        SECTION("chromatic 12")
        {
            auto ch12 = ScaleLibrary::instance().find("chromatic");
            REQUIRE(ch12);
            REQUIRE(ch12->size() == 12);
            REQUIRE(ch12->pitchesPerOctave() == 12);
            REQUIRE(ch12->name() == "chromatic");
            REQUIRE(ch12->degrees() == LF(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11));

            for (int i = -100; i < 100; i++)
                REQUIRE(ch12->degreeToKey(i) == i);
        }

        SECTION("chromatic 24")
        {
            auto ch24 = ScaleLibrary::instance().find("chromatic24");
            REQUIRE(ch24);
            REQUIRE(ch24->size() == 24);
            REQUIRE(ch24->pitchesPerOctave() == 24);
            REQUIRE(ch24->name() == "chromatic24");

            for (int i = -100; i < 100; i++)
                REQUIRE(ch24->degreeToKey(i) == i);
        }

        SECTION("major")
        {
            auto maj = ScaleLibrary::instance().find("major");
            REQUIRE(maj);
            REQUIRE(maj->size() == 7);
            REQUIRE(maj->pitchesPerOctave() == 12);
            REQUIRE(maj->degrees() == LF(0, 2, 4, 5, 7, 9, 11));
            REQUIRE(maj->name() == "major");

            REQUIRE(maj->degreeToKey(0) == 0);
            REQUIRE(maj->degreeToKey(1) == 2);
            REQUIRE(maj->degreeToKey(2) == 4);
            REQUIRE(maj->degreeToKey(3) == 5);
            REQUIRE(maj->degreeToKey(4) == 7);
            REQUIRE(maj->degreeToKey(5) == 9);
            REQUIRE(maj->degreeToKey(6) == 11);
            REQUIRE(maj->degreeToKey(7) == 12);
            REQUIRE(maj->degreeToKey(8) == 14);
            REQUIRE(maj->degreeToKey(9) == 16);

            REQUIRE(maj->degreeToKey(-1) == -1);
            REQUIRE(maj->degreeToKey(-2) == -3);
            REQUIRE(maj->degreeToKey(-3) == -5);
            REQUIRE(maj->degreeToKey(-4) == -7);
            REQUIRE(maj->degreeToKey(-5) == -8);
            REQUIRE(maj->degreeToKey(-6) == -10);
            REQUIRE(maj->degreeToKey(-7) == -12);
            REQUIRE(maj->degreeToKey(-8) == -13);
            REQUIRE(maj->degreeToKey(-9) == -15);
        }

        SECTION("whole")
        {
            auto whole = ScaleLibrary::instance().find("whole");
            REQUIRE(whole);
            REQUIRE(whole->size() == 6);
            REQUIRE(whole->pitchesPerOctave() == 12);
            REQUIRE(whole->degrees() == LF(0, 2, 4, 6, 8, 10));
            REQUIRE(whole->name() == "whole");

            for (int i = -16; i < 16; i++)
                REQUIRE(whole->degreeToKey(i) == i * 2);
        }

        SECTION("all")
        {
            for (auto& s : ScaleLibrary::instance().all()) {
                REQUIRE(ScaleLibrary::instance().find(s->name().c_str()));
                REQUIRE(ScaleLibrary::instance().find(s->name().c_str())->name() == s->name());
            }
        }
    }

    SECTION("Scale")
    {
        SECTION("find")
        {
            auto scale = ScaleLibrary::instance().findByHash("chromatic"_hash);
            REQUIRE(scale);

            for (int i = 0; i < 12; i++) {
                REQUIRE(scale->find(i));
            }

            REQUIRE_FALSE(scale->find(-1));
            REQUIRE_FALSE(scale->find(12));
        }

        SECTION("find nearest")
        {
            t_float res = 0;
            auto s0 = ScaleLibrary::instance().findByHash("chromatic"_hash);
            REQUIRE(s0);

            for (int i = 0; i < 12; i++) {
                REQUIRE(s0->findNearest(i, res));
                REQUIRE(i == res);
            }

            REQUIRE_FALSE(s0->findNearest(-1, res));
            REQUIRE_FALSE(s0->findNearest(12, res));

            auto maj = ScaleLibrary::instance().findByHash("major"_hash);
            REQUIRE(maj);
            REQUIRE(maj->findNearest(0, res));
            REQUIRE(res == 0);
            REQUIRE(maj->findNearest(1, res));
            REQUIRE(res == 2);
            REQUIRE(maj->findNearest(2, res));
            REQUIRE(res == 2);
            REQUIRE(maj->findNearest(3, res));
            REQUIRE(res == 4);
            REQUIRE(maj->findNearest(4, res));
            REQUIRE(res == 4);
            REQUIRE(maj->findNearest(5, res));
            REQUIRE(res == 5);
            REQUIRE(maj->findNearest(6, res));
            REQUIRE(res == 7);
            REQUIRE(maj->findNearest(7, res));
            REQUIRE(res == 7);
            REQUIRE(maj->findNearest(8, res));
            REQUIRE(res == 9);
            REQUIRE(maj->findNearest(9, res));
            REQUIRE(res == 9);
            REQUIRE(maj->findNearest(10, res));
            REQUIRE(res == 11);
            REQUIRE(maj->findNearest(11, res));
            REQUIRE(res == 11);
        }
    }
}
