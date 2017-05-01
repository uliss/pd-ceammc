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
#include "../conv/conv_cc2amp.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<CC2Amp> CC2AmpTest;

TEST_CASE("conv.cc2amp", "[externals]")
{
    obj_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            CC2AmpTest t("conv.cc2amp", AtomList());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @from, 0.f);
            REQUIRE_PROPERTY(t, @to, 1.f);
        }

        SECTION("property arguments")
        {
            CC2AmpTest t("conv.cc2amp", L4("@from", 10, "@to", 100));

            REQUIRE_PROPERTY(t, @from, 10);
            REQUIRE_PROPERTY(t, @to, 100);
        }

        SECTION("positional arguments")
        {
            CC2AmpTest t("conv.cc2amp", L2(20, 40));

            REQUIRE_PROPERTY(t, @from, 20);
            REQUIRE_PROPERTY(t, @to, 40);
        }

        SECTION("mixed arguments")
        {
            CC2AmpTest t("conv.cc2amp", L6(20, 40, "@from", 1, "@to", -1));

            REQUIRE_PROPERTY(t, @from, 1);
            REQUIRE_PROPERTY(t, @to, -1);
        }
    }

    SECTION("convert")
    {
#define C2A(obj, in, out)                             \
    {                                                 \
        WHEN_SEND_FLOAT_TO(0, obj, float(in));        \
        REQUIRE_FLOAT_AT_OUTLET(0, obj, double(out)); \
    }

        SECTION("default")
        {
            CC2AmpTest t("conv.cc2amp", AtomList());
            REQUIRE_PROPERTY(t, @from, 0.f);
            REQUIRE_PROPERTY(t, @to, 1.f);

            C2A(t, 0, 0);
            C2A(t, 127, 1);
            C2A(t, 63.5, 0.5);

            // clip
            C2A(t, -100, 0);
            C2A(t, 128, 1);
            C2A(t, 128, 1);
        }

        SECTION("default")
        {
            CC2AmpTest t("conv.cc2amp", L2(-1, 1));

            C2A(t, 0, -1);
            C2A(t, 127, 1);
            C2A(t, 63.5, 0);

            C2A(t, 60, -0.0551181);
            C2A(t, 61, -0.0393700);
            C2A(t, 62, -0.0236220);
            C2A(t, 63, -0.0078740);
            C2A(t, 64, +0.0078740);
            C2A(t, 65, +0.0236220);
            C2A(t, 66, +0.0393700);
            C2A(t, 67, +0.0551181);
            C2A(t, 68, +0.0708661);
            C2A(t, 69, +0.0866141);
            C2A(t, 70, +0.1023622);
        }
    }
}
