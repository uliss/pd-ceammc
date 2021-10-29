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
#include "random_float.h"
#include "test_random_base.h"

PD_COMPLETE_TEST_SETUP(RandomFloat, random, float)

TEST_CASE("random.float", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("random.float");
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @min, 0);
            REQUIRE_PROPERTY(t, @max, 1);
            REQUIRE_PROPERTY(t, @seed, 0);
        }

        SECTION("args max")
        {
            TObj t("random.float", LF(2));
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @min, 0);
            REQUIRE_PROPERTY(t, @max, 2);
        }

        SECTION("args min/max")
        {
            TObj t("random.float", LF(-1, 2));
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @min, -1);
            REQUIRE_PROPERTY(t, @max, 2);
        }

        SECTION("properties")
        {
            TObj t("random.float", LA("@max", 3, "@min", 1));

            REQUIRE_PROPERTY(t, @min, 1);
            REQUIRE_PROPERTY(t, @max, 3);
        }

        SECTION("properties")
        {
            TObj t("random.float", LA(-2, 4, "@seed", 5));

            REQUIRE_PROPERTY(t, @min, -2);
            REQUIRE_PROPERTY(t, @max, 4);
            REQUIRE_PROPERTY(t, @seed, 5);
        }

        SECTION("args invalid min/max")
        {
            TObj t("random.float", LF(1, -1));

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_FALSE(t.hasNewMessages());
        }
    }

    SECTION("do")
    {
        TExt t("random.float", 3, 4);
        for (int i = 0; i < 256; i++) {
            t.bang();
            auto f = floatAt(t);
            REQUIRE(f >= 3);
            REQUIRE(f < 4);
        }
    }

    SECTION("seed")
    {
        TExt t("random.f", "@seed", 1);
        constexpr size_t N = 10;

        std::vector<t_float> v0;
        for (int i = 0; i < N; i++) {
            t.bang();
            v0.push_back(floatAt(t));
        }

        std::vector<t_float> v1;
        for (int i = 0; i < N; i++) {
            t.bang();
            v1.push_back(floatAt(t));
        }

        REQUIRE(v0 != v1);

        t->setProperty("@seed", LF(1));
        for (int i = 0; i < N; i++) {
            t.bang();
            v1[i] = floatAt(t);
        }

        REQUIRE(v0 == v1);
    }

    SECTION("inlets")
    {
        TExt t("random.float", 3, 4);

        REQUIRE_PROPERTY(t, @min, 3);
        REQUIRE_PROPERTY(t, @max, 4);

        t.sendFloatTo(-100, 1);
        REQUIRE_PROPERTY(t, @min, -100);
        REQUIRE_PROPERTY(t, @max, 4);

        t.sendFloatTo(200, 2);
        REQUIRE_PROPERTY(t, @min, -100);
        REQUIRE_PROPERTY(t, @max, 200);

        t.sendListTo(LF(1, 2), 1);
        REQUIRE_PROPERTY(t, @min, 1);
        REQUIRE_PROPERTY(t, @max, 2);
    }
}
