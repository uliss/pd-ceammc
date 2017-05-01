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
#include "../conv/conv_lin2lin.h"
#include "base_extension_test.h"
#include "catch.hpp"

typedef TestExtension<Lin2Lin> Lin2LinTest;

TEST_CASE("conv.lin2lin", "[externals]")
{
    obj_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            Lin2LinTest t("conv.lin2lin", AtomList());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @in_from, 0.f);
            REQUIRE_PROPERTY(t, @in_to, 127.f);
            REQUIRE_PROPERTY(t, @out_from, 0.f);
            REQUIRE_PROPERTY(t, @out_to, 1.f);
            REQUIRE_PROPERTY(t, @clip, S("minmax"));
        }

        SECTION("property arguments")
        {
            {
                Lin2LinTest t("conv.lin2lin", L4("@in_from", 10, "@in_to", 100));
                REQUIRE_PROPERTY(t, @in_from, 10.f);
                REQUIRE_PROPERTY(t, @in_to, 100.f);
                REQUIRE_PROPERTY(t, @out_from, 0.f);
                REQUIRE_PROPERTY(t, @out_to, 1.f);
                REQUIRE_PROPERTY(t, @clip, S("minmax"));
            }

            {
                Lin2LinTest t("conv.lin2lin", L4("@out_from", 10, "@out_to", 100));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 10);
                REQUIRE_PROPERTY(t, @out_to, 100);
                REQUIRE_PROPERTY(t, @clip, S("minmax"));
            }

            // clip
            {
                Lin2LinTest t("conv.lin2lin", L2("@clip", "noclip"));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 0.f);
                REQUIRE_PROPERTY(t, @out_to, 1);
                REQUIRE_PROPERTY(t, @clip, S("noclip"));
            }

            // invalid clip
            {
                Lin2LinTest t("conv.lin2lin", L2("@clip", "invalid"));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 0.f);
                REQUIRE_PROPERTY(t, @out_to, 1);
                // default
                REQUIRE_PROPERTY(t, @clip, S("minmax"));
            }

            // alias @noclip
            {
                Lin2LinTest t("conv.lin2lin", L1("@noclip"));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 0.f);
                REQUIRE_PROPERTY(t, @out_to, 1);
                REQUIRE_PROPERTY(t, @clip, S("noclip"));
            }

            // alias @minmax
            {
                Lin2LinTest t("conv.lin2lin", L1("@minmax"));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 0.f);
                REQUIRE_PROPERTY(t, @out_to, 1);
                REQUIRE_PROPERTY(t, @clip, S("minmax"));
            }

            // alias @min
            {
                Lin2LinTest t("conv.lin2lin", L1("@min"));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 0.f);
                REQUIRE_PROPERTY(t, @out_to, 1);
                REQUIRE_PROPERTY(t, @clip, S("min"));
            }

            // alias @max
            {
                Lin2LinTest t("conv.lin2lin", L1("@max"));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 0.f);
                REQUIRE_PROPERTY(t, @out_to, 1);
                REQUIRE_PROPERTY(t, @clip, S("max"));
            }
        }

        SECTION("positional arguments")
        {
            {
                Lin2LinTest t("conv.lin2lin", L2(20, 40));
                REQUIRE_PROPERTY(t, @in_from, 20);
                REQUIRE_PROPERTY(t, @in_to, 40);
                REQUIRE_PROPERTY(t, @out_from, 0.f);
                REQUIRE_PROPERTY(t, @out_to, 1);
            }

            {
                Lin2LinTest t("conv.lin2lin", L4(20, 40, 1, 2));
                REQUIRE_PROPERTY(t, @in_from, 20);
                REQUIRE_PROPERTY(t, @in_to, 40);
                REQUIRE_PROPERTY(t, @out_from, 1);
                REQUIRE_PROPERTY(t, @out_to, 2);
            }
        }

        SECTION("mixed arguments")
        {
            Lin2LinTest t("conv.lin2lin", L3(20, 40, "@noclip"));

            REQUIRE_PROPERTY(t, @in_from, 20);
            REQUIRE_PROPERTY(t, @in_to, 40);
            REQUIRE_PROPERTY(t, @out_from, 0.f);
            REQUIRE_PROPERTY(t, @out_to, 1);
            REQUIRE_PROPERTY(t, @clip, S("noclip"));
        }
    }

    SECTION("convert")
    {

#define REQUIRE_L2E(t, in, out)             \
    {                                       \
        WHEN_SEND_FLOAT_TO(0, t, in);       \
        REQUIRE_FLOAT_AT_OUTLET(0, t, out); \
    }

        SECTION("default")
        {
            Lin2LinTest t("conv.lin2lin", AtomList());

            REQUIRE_L2E(t, 0, 0);
            REQUIRE_L2E(t, 127, 1);
        }

        SECTION("clip")
        {
            SECTION("minmax")
            {
                Lin2LinTest t("conv.lin2lin", L1("@minmax"));

                REQUIRE_L2E(t, -10, 0);
                REQUIRE_L2E(t, 200, 1);
            }

            SECTION("min")
            {
                Lin2LinTest t("conv.lin2lin", L1("@min"));

                REQUIRE_L2E(t, -10, 0);
                REQUIRE_L2E(t, 254, 2);
            }

            SECTION("max")
            {
                Lin2LinTest t("conv.lin2lin", L1("@max"));

                REQUIRE_L2E(t, -127, -1);
                REQUIRE_L2E(t, 254, 1);
            }

            SECTION("noclip")
            {
                Lin2LinTest t("conv.lin2lin", L1("@noclip"));

                REQUIRE_L2E(t, -127, -1);
                REQUIRE_L2E(t, 254, 2);
            }
        }

        SECTION("conv")
        {
            Lin2LinTest t("conv.lin2lin", L4(-5, 5, 1, 2));
            REQUIRE_L2E(t, -5, 1);
            REQUIRE_L2E(t, -4, 1.1);
            REQUIRE_L2E(t, -3, 1.2);
            REQUIRE_L2E(t, -2, 1.3);
            REQUIRE_L2E(t, -1, 1.4);
            REQUIRE_L2E(t, +0, 1.5);
            REQUIRE_L2E(t, +1, 1.6);
            REQUIRE_L2E(t, +2, 1.7);
            REQUIRE_L2E(t, +3, 1.8);
            REQUIRE_L2E(t, +4, 1.9);
            REQUIRE_L2E(t, +5, 2);
        }
    }
}
