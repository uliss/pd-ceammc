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
#include "../data/set_size.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<SetSize> SetSizeTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("set.size", "[externals]")
{
    setup_set0x2esize();

    SECTION("create")
    {
        SetSizeTest t("set.size");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        SetSizeTest t("set.size");
        WHEN_SEND_TDATA_TO(0, t, DataTypeSet());
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(Atom(1)));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L3(1, 2, 3)));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 3);
    }
}
