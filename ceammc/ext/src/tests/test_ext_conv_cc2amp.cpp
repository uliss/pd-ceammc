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

            REQUIRE_PROPERTY(t, @v0, 0.f);
            REQUIRE_PROPERTY(t, @v1, 1.f);
            REQUIRE_PROPERTY(t, @clip, 1.f);
        }

        SECTION("property arguments")
        {
            CC2AmpTest t("conv.cc2amp", L4("@v0", 10, "@v1", 100));

            REQUIRE_PROPERTY(t, @v0, 10);
            REQUIRE_PROPERTY(t, @v1, 100);
        }

        SECTION("positional arguments")
        {
            CC2AmpTest t("conv.cc2amp", L2(20, 40));

            REQUIRE_PROPERTY(t, @v0, 20);
            REQUIRE_PROPERTY(t, @v1, 40);
        }

        SECTION("mixed arguments")
        {
            CC2AmpTest t("conv.cc2amp", L6(20, 40, "@v0", 1, "@v1", -1));

            REQUIRE_PROPERTY(t, @v0, 1);
            REQUIRE_PROPERTY(t, @v1, -1);
        }
    }

    SECTION("convert")
    {
        SECTION("default")
        {
            CC2AmpTest t("conv.cc2amp", AtomList());

            WHEN_SEND_FLOAT_TO(0, t, 0);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0.f);

            WHEN_SEND_FLOAT_TO(0, t, 127);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1.f);

            WHEN_SEND_FLOAT_TO(0, t, 63.5);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0.5f);

            WHEN_SEND_FLOAT_TO(0, t, -100);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0.f);

            WHEN_SEND_FLOAT_TO(0, t, 128);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1.f);

            WHEN_SEND_FLOAT_TO(0, t, 1000);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1.f);
        }
    }
}
