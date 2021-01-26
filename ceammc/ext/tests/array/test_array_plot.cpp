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
#include "array_plot.h"
#include "test_array_base.h"
#include "test_sound.h"

PD_COMPLETE_TEST_SETUP(ArrayPlot, array, plot)

TEST_CASE("array.plot", "[externals]")
{
    pd_test_init();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("init")
    {
        SECTION("empty")
        {
            TExt t("array.plot");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @array, "");
            REQUIRE_PROPERTY(t, @ymin, -1);
            REQUIRE_PROPERTY(t, @ymax, 1);
            REQUIRE_PROPERTY(t, @yauto, 1);
        }

        SECTION("args")
        {

            TExt t("array.plot", LA("array1", -2, 3.5));

            REQUIRE_PROPERTY(t, @array, "array1");
            REQUIRE_PROPERTY(t, @ymin, -2);
            REQUIRE_PROPERTY(t, @ymax, 3.5);
            REQUIRE_PROPERTY(t, @yauto, 1);
        }
    }

    SECTION("dsp")
    {
        TExt t("array.plot", LA("array_plot0"));

        SECTION("list")
        {
            ArrayPtr aptr = cnv->createArray("array_plot0", 2);
            t << LF(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

            REQUIRE(aptr->update());
            REQUIRE(aptr->size() == 10);

            for (size_t i = 0; i < 10; i++) {
                REQUIRE(aptr->at(i) == i + 1);
            }

            REQUIRE(t.isOutputBangAt(0));
        }

        SECTION("list yauto0")
        {
            ArrayPtr aptr1 = cnv->createArray("array_plot1", 3);
            REQUIRE(t->setProperty("@yauto", LF(0)));
            REQUIRE(t->setProperty("@ymin", LF(2)));
            REQUIRE(t->setProperty("@ymax", LF(5)));
            REQUIRE(t->setProperty("@array", LA("array_plot1")));

            t << LF(1, 2, 3, 4, 5, 6);

            REQUIRE(aptr1->update());
            REQUIRE(aptr1->size() == 6);

            REQUIRE(aptr1->at(0) == 2);
            REQUIRE(aptr1->at(1) == 2);
            REQUIRE(aptr1->at(2) == 3);
            REQUIRE(aptr1->at(3) == 4);
            REQUIRE(aptr1->at(4) == 5);
            REQUIRE(aptr1->at(5) == 5);

            REQUIRE(t.isOutputBangAt(0));
        }
    }
}
