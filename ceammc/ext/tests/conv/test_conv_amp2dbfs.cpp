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
#include "conv_amp2dbfs.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(Amp2dbfs, conv, amp2dbfs)

using namespace ceammc;

#define A2D(obj, in, out)                             \
    {                                                 \
        WHEN_SEND_FLOAT_TO(0, obj, float(in));        \
        REQUIRE_FLOAT_AT_OUTLET(0, obj, double(out)); \
    }

TEST_CASE("conv.amp2dbfs", "[externals]")
{
    pd_test_init();

    SECTION("test create with:")
    {
        TObj t("conv.amp2dbfs");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("convert")
    {
        TObj t("conv.amp2dbfs");

        A2D(t, 1, 0);
        A2D(t, 0.5, -6.0206);
        A2D(t, 0, -144);

        WHEN_SEND_LIST_TO(0, t, LA(1, 0.5, 2, 0.f));
        REQUIRE_LIST_AT_OUTLET(0, t, LX(0, -6.0206f, 6.0206f, -144));
    }

    SECTION("create")
    {
    }
}
