/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "test_base.h"
#include "ceammc_pd.h"
#include "test_external.h"

#include "catch.hpp"

using namespace ceammc;
using namespace ceammc::pd;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("pd external", "[pd::External]")
{
    pd_init();
    ExternalOutput::setup();

    SECTION("connectTo")
    {
        External t("+");
        REQUIRE_FALSE(t.connectTo(0, NULL, 0));

        External t1("NONE");
        REQUIRE(t1.isNull());
        REQUIRE_FALSE(t1.connectTo(0, NULL, 0));

        REQUIRE_FALSE(t.connectTo(0, t1, 0));
        REQUIRE_FALSE(t.connectTo(0, t, 0));

        External t2("*");
        REQUIRE_FALSE(t.connectTo(1, t2, 0));
        REQUIRE_FALSE(t.connectTo(0, t2, 2));

        REQUIRE(t.connectTo(0, t2, 1));
        REQUIRE(t.connectTo(0, t2, 0));
    }

    SECTION("connectFrom")
    {
        External t1("NONE");
        External t2("+");
        External t3("swap");

        REQUIRE(t1.isNull());
        REQUIRE(!t2.isNull());
        REQUIRE(!t2.isNull());

        REQUIRE_FALSE(t1.connectFrom(0, t1, 0));
        REQUIRE_FALSE(t1.connectFrom(0, t2, 0));
        REQUIRE_FALSE(t1.connectFrom(0, t2, 10));

        REQUIRE_FALSE(t2.connectFrom(0, t1, 1));
        REQUIRE_FALSE(t2.connectFrom(2, t3, 0));
        REQUIRE_FALSE(t2.connectFrom(0, t3, 2));
    }
}
