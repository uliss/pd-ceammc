/*****************************************************************************
 * Copyright 2023 Serge Poltavsky. All rights reserved.
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
#include "metro_random.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(MetroRandom, metro, random)

#define BANG_AFTER_NTICKS(obj, n)  \
    {                              \
        obj.clearAll();            \
        obj.schedTicks(n - 1);     \
        REQUIRE(!obj.hasOutput()); \
        obj.schedTicks(1);         \
        REQUIRE(obj.hasOutput());  \
    }

#define NO_BANG_AFTER_NTICKS(obj, n)    \
    {                                   \
        obj.clearAll();                 \
        obj.schedTicks(n);              \
        REQUIRE_FALSE(obj.hasOutput()); \
    }

TEST_CASE("metro.random", "[externals]")
{
    pd_test_init();
    canvas_suspend_dsp();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("metro.random");
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @min, 100);
            REQUIRE_PROPERTY_FLOAT(t, @max, 200);
            REQUIRE_PROPERTY_FLOAT(t, @seed, 0);
        }
    }

    SECTION("other inlets")
    {
        TExt t("metro.random", LF(10, 20));
        REQUIRE_PROPERTY_FLOAT(t, @min, 10);
        REQUIRE_PROPERTY_FLOAT(t, @max, 20);
        t.sendFloatTo(11, 1);
        REQUIRE_PROPERTY_FLOAT(t, @min, 11);
        REQUIRE_PROPERTY_FLOAT(t, @max, 20);
        t.sendFloatTo(21, 2);
        REQUIRE_PROPERTY_FLOAT(t, @min, 11);
        REQUIRE_PROPERTY_FLOAT(t, @max, 21);
        t.sendListTo(LF(100, 200), 1);
        REQUIRE_PROPERTY_FLOAT(t, @min, 100);
        REQUIRE_PROPERTY_FLOAT(t, @max, 200);
    }
}
