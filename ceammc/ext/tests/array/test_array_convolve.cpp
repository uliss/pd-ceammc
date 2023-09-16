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
#include "array_convolve.h"
#include "ceammc_canvas.h"
#include "test_array_base.h"
//#include "test_sound.h"

PD_COMPLETE_TEST_SETUP(ArrayConvolve, array, convolve)

TEST_CASE("array.convolve", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("empty")
    {
        TExt t("array.convolve");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @array, &s_);
        REQUIRE_PROPERTY(t, @ir, &s_);
    }

    SECTION("invalid")
    {
        TExt t("array.convolve", LA("non-exists", "non-ir"));
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @array, "non-exists");
        REQUIRE_PROPERTY(t, @ir, "non-ir");
    }

    SECTION("do")
    {
        ArrayPtr a0 = cnv->createArray("array_conv0", 16);
        a0->fillWith(0.);
        ArrayPtr a1 = cnv->createArray("array_conv1", 4);
        a1->fillWith(0.);

        TExt t("array.convolve", LA("array_conv0", "array_conv1"));
        t.bang();

        REQUIRE(a0->update());
        REQUIRE(a0->size() == (16 + 4 - 1));

        for (size_t i = 0; i < a0->size(); i++) {
            REQUIRE(a0->at(i) == Approx(0));
        }

        a0->resize(16);
        a0->fillWith(1);
        a1->fillWith(1 / 4.0);

        t.bang();
        REQUIRE(a0->update());
        AtomList res;

        for (size_t i = 0; i < a0->size(); i++)
            res.append(a0->at(i));

        REQUIRE(res == LX(0.25, 0.5, 0.75, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.75, 0.5, 0.25));
    }
}
