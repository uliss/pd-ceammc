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
#include "math_polyeval.h"
#include "test_math_base.h"

PD_COMPLETE_TEST_SETUP(MathPolyEval, math, polyeval)

TEST_CASE("math.polyeval", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            TExt t("math.polyeval");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_LIST(t, @coeffs, L());
        }

        SECTION("args")
        {
            TExt t("math.polyeval", LF(1, 2, 3, 4));
            REQUIRE_PROPERTY_LIST(t, @coeffs, LF(1, 2, 3, 4));
        }

        SECTION("props")
        {
            TExt t("math.polyeval", LA("@coeffs", 1, 2, 3));
            REQUIRE_PROPERTY_LIST(t, @coeffs, LF(1, 2, 3));
        }
    }

    SECTION("set coeffs")
    {
        TExt t("math.polyeval", LF(1, 2));

        t->onInlet(1, L());
        REQUIRE_PROPERTY_LIST(t, @coeffs, LF(1, 2));
    }

    SECTION("eval")
    {
        TExt t("math.polyeval");
        t << 1;
        REQUIRE_FALSE(t.hasOutput());
        t << LF(1);
        REQUIRE_FALSE(t.hasOutput());

        t->setProperty("@coeffs", LF(5));
        REQUIRE_PROPERTY_LIST(t, @coeffs, LF(5));

        for (size_t i = 0; i < 10; i++) {
            t << i;
            REQUIRE(t.outputFloatAt(0) == 5);
        }

        t->setProperty("@coeffs", LF(2, 4));
        t << 0.f;
        REQUIRE(t.outputFloatAt(0) == 4);
        t << 1;
        REQUIRE(t.outputFloatAt(0) == 6);
        t << 2;
        REQUIRE(t.outputFloatAt(0) == 8);
        t << 3;
        REQUIRE(t.outputFloatAt(0) == 10);
        t << 4;
        REQUIRE(t.outputFloatAt(0) == 12);

        t->setProperty("@coeffs", LF(1, 2, 3));
        t << 0.f;
        REQUIRE(t.outputFloatAt(0) == 3);
        t << 1;
        REQUIRE(t.outputFloatAt(0) == 6);
        t << 2;
        REQUIRE(t.outputFloatAt(0) == 11);
        t << 3;
        REQUIRE(t.outputFloatAt(0) == 18);
        t << 4;
        REQUIRE(t.outputFloatAt(0) == 27);

        t << LA(0.f, 1, 2, 3, 4);
        REQUIRE(t.outputListAt(0) == LA(3, 6, 11, 18, 27));

        t << L();
        REQUIRE(t.outputListAt(0) == L());
    }
}
