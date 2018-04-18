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
#include "../conv/conv_dbfs2amp.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<Dbfs2amp> Dbfs2AmpTest;

using namespace ceammc;
static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

#define D2A(obj, in, out)                             \
    {                                                 \
        WHEN_SEND_FLOAT_TO(0, obj, float(in));        \
        REQUIRE_FLOAT_AT_OUTLET(0, obj, double(out)); \
    }

TEST_CASE("conv.dbfs2amp", "[externals]")
{
    setup_conv_dbfs2amp();

    SECTION("test create with:")
    {
        Dbfs2AmpTest t("conv.dbfs2amp");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("convert")
    {
        Dbfs2AmpTest t("conv.dbfs2amp");

        D2A(t, 0, 1);
        D2A(t, 6.0206, 2);
        D2A(t, -6.0206, 0.5);
        D2A(t, -144, 0);
        D2A(t, -200, 0);

        WHEN_SEND_LIST_TO(0, t, L4(0.f, -6.0206f, 6.0206f, -144));
        REQUIRE_LIST_AT_OUTLET(0, t, ListApprox(1, 0.5, 2, 0.f));
    }
}
