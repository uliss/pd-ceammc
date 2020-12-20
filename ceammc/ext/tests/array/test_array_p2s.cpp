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
#include "array_p2s.h"
#include "test_array_base.h"

PD_COMPLETE_TEST_SETUP(ArrayPhaseToSample, array, p2s)

#define ON_FLOAT_REQUIRE(in, t, out)        \
    {                                       \
        WHEN_SEND_FLOAT_TO(0, t, in);       \
        REQUIRE_FLOAT_AT_OUTLET(0, t, out); \
    }

#define ON_LIST_REQUIRE(in, t, out)        \
    {                                      \
        WHEN_SEND_LIST_TO(0, t, in);       \
        REQUIRE_LIST_AT_OUTLET(0, t, out); \
    }

TEST_CASE("array.p2s", "[externals]")
{
    pd_test_init();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            cnv->createArray("array_p2s", 10);

            TObj t("array.p2s");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @array, &s_);

            ON_FLOAT_REQUIRE(0, t, 0);
            ON_FLOAT_REQUIRE(0.5, t, 0);
            ON_FLOAT_REQUIRE(1, t, 0);

            ON_LIST_REQUIRE(LF(0.125, 0.25, 0.3), t, LF(0.f, 0.f, 0.f));
        }

        SECTION("symbol arguments")
        {
            TObj t("array.p2s", A("array_p2s"));
            REQUIRE_PROPERTY(t, @array, "array_p2s");

            ON_FLOAT_REQUIRE(0, t, 0);
            ON_FLOAT_REQUIRE(0.5, t, 5);
            ON_FLOAT_REQUIRE(1, t, 10);

            ON_FLOAT_REQUIRE(-1, t, 0);
            ON_FLOAT_REQUIRE(2, t, 10);

            ON_LIST_REQUIRE(LF(0.125, 0.25, 0.375), t, LF(1.25, 2.5, 3.75));
            ON_LIST_REQUIRE(LF(-1, -20, 40), t, LF(0.f, 0.f, 10));
        }
    }
}
