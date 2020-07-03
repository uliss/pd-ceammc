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
#include "array_resample.h"
#include "test_array_base.h"

PD_COMPLETE_TEST_SETUP(ArrayResample, array, resample)

TEST_CASE("array.resample", "[externals]")
{
    pd_test_init();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("empty")
    {
        TObj t("array.resample");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @src, "");
        REQUIRE_PROPERTY(t, @dest, "");
    }

    SECTION("simple x2")
    {
        ArrayPtr aptr0 = cnv->createArray("ars0", 20);
        ArrayPtr aptr1 = cnv->createArray("ars1", 2);

        aptr0->fillWith(0.25);

        TObj t("array.resample", LA("ars0", "ars1", "@ratio", 2.1));
        REQUIRE_PROPERTY(t, @src, "ars0");
        REQUIRE_PROPERTY(t, @dest, "ars1");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(floatAt(t) == 42);

        REQUIRE(aptr0->update());
        REQUIRE(aptr1->update());
        REQUIRE(aptr0->size() == 20);
        REQUIRE(aptr1->size() == 42);
    }

    SECTION("simple /2")
    {
        Array array0("ars0");
        ArrayPtr aptr2 = cnv->createArray("ars2", 2);

        TObj t("array.resample", LA("ars0", "ars2", "@ratio", 0.5));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(floatAt(t) == 10);

        REQUIRE(aptr2->update());
        REQUIRE(aptr2->size() == 10);
    }

    SECTION("simple 1")
    {
        Array a0("ars0");
        Array a1("ars1");
        TObj t("array.resample", LA("ars0", "ars1", "@ratio", 1));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(floatAt(t) == 20);

        REQUIRE(a0.update());
        REQUIRE(a1.update());
        REQUIRE(a1.size() == 20);

        for (size_t i = 0; i < 20; i++) {
            REQUIRE(a0[i] == Approx(0.25));
            REQUIRE(a1[i] == Approx(0.25));
        }
    }

    SECTION("inplace")
    {
        ArrayPtr aptr3 = cnv->createArray("ars3", 10);
        aptr3->fillWith(1);

        TObj t("array.resample", LA("ars3", "@ratio", 4.6));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(floatAt(t) == 46);

        REQUIRE(aptr3->update());
        REQUIRE(aptr3->size() == 46);
    }
}
