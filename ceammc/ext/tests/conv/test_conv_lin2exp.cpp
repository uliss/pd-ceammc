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
#include "conv_lin2exp.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(Lin2Exp, conv, lin2exp)

TEST_CASE("conv.lin2exp", "[externals]")
{
    pd_test_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            TObj t("conv.lin2exp", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @in_from, 0.f);
            REQUIRE_PROPERTY(t, @in_to, 127.f);
            REQUIRE_PROPERTY(t, @out_from, 0.01f);
            REQUIRE_PROPERTY(t, @out_to, 1.f);
            REQUIRE_PROPERTY(t, @clip, S("minmax"));
        }

        SECTION("property arguments")
        {
            {
                TObj t("conv.lin2exp", LA("@in_from", 10, "@in_to", 100));
                REQUIRE_PROPERTY(t, @in_from, 10.f);
                REQUIRE_PROPERTY(t, @in_to, 100.f);
                REQUIRE_PROPERTY(t, @out_from, 0.01f);
                REQUIRE_PROPERTY(t, @out_to, 1.f);
                REQUIRE_PROPERTY(t, @clip, S("minmax"));
            }

            {
                TObj t("conv.lin2exp", LA("@out_from", 10, "@out_to", 100));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 10);
                REQUIRE_PROPERTY(t, @out_to, 100);
                REQUIRE_PROPERTY(t, @clip, S("minmax"));
            }

            // clip
            {
                TObj t("conv.lin2exp", LA("@clip", "noclip"));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 0.01f);
                REQUIRE_PROPERTY(t, @out_to, 1);
                REQUIRE_PROPERTY(t, @clip, S("noclip"));
            }

            // invalid clip
            {
                TObj t("conv.lin2exp", LA("@clip", "invalid"));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 0.01f);
                REQUIRE_PROPERTY(t, @out_to, 1);
                // default
                REQUIRE_PROPERTY(t, @clip, S("minmax"));
            }

            // alias @noclip
            {
                TObj t("conv.lin2exp", LA("@noclip"));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 0.01f);
                REQUIRE_PROPERTY(t, @out_to, 1);
                REQUIRE_PROPERTY(t, @clip, S("noclip"));
            }

            // alias @minmax
            {
                TObj t("conv.lin2exp", LA("@minmax"));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 0.01f);
                REQUIRE_PROPERTY(t, @out_to, 1);
                REQUIRE_PROPERTY(t, @clip, S("minmax"));
            }

            // alias @min
            {
                TObj t("conv.lin2exp", LA("@min"));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 0.01f);
                REQUIRE_PROPERTY(t, @out_to, 1);
                REQUIRE_PROPERTY(t, @clip, S("min"));
            }

            // alias @max
            {
                TObj t("conv.lin2exp", LA("@max"));
                REQUIRE_PROPERTY(t, @in_from, 0.f);
                REQUIRE_PROPERTY(t, @in_to, 127);
                REQUIRE_PROPERTY(t, @out_from, 0.01f);
                REQUIRE_PROPERTY(t, @out_to, 1);
                REQUIRE_PROPERTY(t, @clip, S("max"));
            }
        }

        SECTION("positional arguments")
        {
            {
                TObj t("conv.lin2exp", LF(20, 40));
                REQUIRE_PROPERTY(t, @in_from, 20);
                REQUIRE_PROPERTY(t, @in_to, 40);
                REQUIRE_PROPERTY(t, @out_from, 0.01f);
                REQUIRE_PROPERTY(t, @out_to, 1);
            }

            {
                TObj t("conv.lin2exp", LF(20, 40, 1, 2));
                REQUIRE_PROPERTY(t, @in_from, 20);
                REQUIRE_PROPERTY(t, @in_to, 40);
                REQUIRE_PROPERTY(t, @out_from, 1);
                REQUIRE_PROPERTY(t, @out_to, 2);
            }
        }

        SECTION("mixed arguments")
        {
            TObj t("conv.lin2exp", LA(20, 40, "@noclip"));

            REQUIRE_PROPERTY(t, @in_from, 20);
            REQUIRE_PROPERTY(t, @in_to, 40);
            REQUIRE_PROPERTY(t, @out_from, 0.01f);
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
            TObj t("conv.lin2exp", L());

            REQUIRE_L2E(t, 0, 0.01);
            REQUIRE_L2E(t, 127, 1);
        }

        SECTION("clip")
        {
            SECTION("minmax")
            {
                TObj t("conv.lin2exp", LA("@minmax"));

                REQUIRE_L2E(t, -10, 0.01);
                REQUIRE_L2E(t, 200, 1);
            }

            SECTION("min")
            {
                TObj t("conv.lin2exp", LA("@min"));

                REQUIRE_L2E(t, -10, 0.01);
                REQUIRE_L2E(t, 254, 100);
            }

            SECTION("max")
            {
                TObj t("conv.lin2exp", LA("@max"));

                REQUIRE_L2E(t, -127, 0.0001);
                REQUIRE_L2E(t, 254, 1);
            }

            SECTION("noclip")
            {
                TObj t("conv.lin2exp", LA("@noclip"));

                REQUIRE_L2E(t, -127, 0.0001);
                REQUIRE_L2E(t, 254, 100);
            }
        }

        SECTION("conv")
        {
            TObj t("conv.lin2lin", LF(-5, 5, 1, 2));
            REQUIRE_L2E(t, -5, 1);
            REQUIRE_L2E(t, -4, 1.0717734625363);
            REQUIRE_L2E(t, -3, 1.148698354997);
            REQUIRE_L2E(t, -2, 1.2311444133449);
            REQUIRE_L2E(t, -1, 1.3195079107729);
            REQUIRE_L2E(t, +0, 1.4142135623731);
            REQUIRE_L2E(t, +1, 1.5157165665104);
            REQUIRE_L2E(t, +2, 1.6245047927125);
            REQUIRE_L2E(t, +3, 1.7411011265922);
            REQUIRE_L2E(t, +4, 1.8660659830736);
            REQUIRE_L2E(t, +5, 2);
        }
    }
}
