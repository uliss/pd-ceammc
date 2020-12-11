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
#include "array_stddev.h"
#include "test_array_base.h"

PD_COMPLETE_TEST_SETUP(ArrayStdDeviation, array, stddev)

TEST_CASE("array.stddev", "[externals]")
{
    pd_test_init();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("empty")
    {
        TObj t("array.stddev");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @array, &s_);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("invalid")
    {
        TObj t("array.stddev", LA("non-exists"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @array, "non-exists");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("array_stddev1")
    {
        TObj t("array.stddev", LA("array_stddev1"));

        // no array yet
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        ArrayPtr aptr = cnv->createArray("array_stddev1", 4);
        Array a("array_stddev1", { 1, 3, 4, -2, 0.4, 5, 7 });
        REQUIRE(a.size() == 7);

        // array created
        WHEN_SEND_BANG_TO(0, t);
        // numpy:
        // a = np.array([1,3,4,-2,0.4,5,7])
        // print(a.std(ddof=1))
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(3.0494339833889863));
    }
}
