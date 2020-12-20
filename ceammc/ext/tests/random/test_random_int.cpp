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
#include "random_int.h"
#include "test_random_base.h"

PD_COMPLETE_TEST_SETUP(RandomInt, random, int)

TEST_CASE("random.int", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("random.int");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @min, 0);
            REQUIRE_PROPERTY(t, @max, 255);
            REQUIRE_PROPERTY(t, @seed, 0);
        }

        SECTION("args max")
        {
            TObj t("random.int", LF(2));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @min, 0);
            REQUIRE_PROPERTY(t, @max, 2);
        }

        SECTION("args min/max")
        {
            TObj t("random.int", LF(-1.1, 2.1));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @min, -1);
            REQUIRE_PROPERTY(t, @max, 2);
        }

        SECTION("properties")
        {
            TObj t("random.int", LA("@max", 3, "@min", 1));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @min, 1);
            REQUIRE_PROPERTY(t, @max, 3);
        }

        SECTION("properties")
        {
            TObj t("random.int", LA(-2, 4, "@seed", 5));

            REQUIRE_PROPERTY(t, @min, -2);
            REQUIRE_PROPERTY(t, @max, 4);
            REQUIRE_PROPERTY(t, @seed, 5);
        }

        SECTION("properties")
        {
            TObj t("random.int", LA(-2, 4, 18, "@seed", 5));

            REQUIRE_PROPERTY(t, @min, 0);
            REQUIRE_PROPERTY(t, @max, 255);
            REQUIRE_PROPERTY(t, @seed, 5);
        }

        SECTION("args invalid min/max")
        {
            TObj t("random.int", LF(1, -1));

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_FALSE(t.hasNewMessages());
        }
    }

    SECTION("do")
    {
        TExt t("random.int", -2, 2);
        for (int i = 0; i < 256; i++) {
            t.bang();
            auto f = floatAt(t);
            REQUIRE(f >= -2);
            REQUIRE(f <= 2);
        }
    }

    SECTION("seed")
    {
        TExt t("random.int", "@seed", 1);
        constexpr size_t N = 10;

        std::vector<int> v0;
        for (int i = 0; i < N; i++) {
            t.bang();
            v0.push_back(floatAt(t));
        }

        std::vector<int> v1;
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
}
