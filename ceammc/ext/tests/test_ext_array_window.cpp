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
#include "../array/array_window.h"
#include "ceammc_factory.h"
#include "ceammc_pd.h"
#include "test_external.h"

#include "catch.hpp"

typedef TestExternal<ArrayWindow> TestArrayWindow;
using namespace ceammc;
static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("array.window", "[externals]")
{
    //    pd_test_mod_init_array_window();

    SECTION("empty")
    {
        TestArrayWindow t("array.window");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY_NONE(t, @array);
        REQUIRE_PROPERTY_FLOAT(t, @redraw, 1);
        REQUIRE_PROPERTY_LIST(t, @type, LA("hann"));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("invalid")
    {
        TestArrayWindow t("array.window", LA("non-exists"));
        REQUIRE_PROPERTY(t, @array, "non-exists");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("array1")
    {
        TestArrayWindow t("array.window", LA("array1", "@type", "tri"));

        // no array yet
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        ArrayPtr aptr = cnv->createArray("array1", 5);
        REQUIRE(aptr.get());

        // array created
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_BANG_AT_OUTLET(0, t);

        REQUIRE(aptr->at(0) == Approx(0));
        REQUIRE(aptr->at(1) == Approx(0.5));
        REQUIRE(aptr->at(2) == Approx(1));
        REQUIRE(aptr->at(3) == Approx(0.5));
        REQUIRE(aptr->at(4) == Approx(0));
    }

    SECTION("array2")
    {
        ArrayPtr aptr = cnv->createArray("array2", 5);
        REQUIRE(aptr.get());
        REQUIRE(aptr->at(0) == Approx(0));
        REQUIRE(aptr->at(1) == Approx(0));
        REQUIRE(aptr->at(2) == Approx(0));
        REQUIRE(aptr->at(3) == Approx(0));
        REQUIRE(aptr->at(4) == Approx(0));

        TestArrayWindow t("array.window", LA("array2"));

        // no changes with out specified window
        REQUIRE(aptr->at(0) == Approx(0));
        REQUIRE(aptr->at(1) == Approx(0));
        REQUIRE(aptr->at(2) == Approx(0));
        REQUIRE(aptr->at(3) == Approx(0));
        REQUIRE(aptr->at(4) == Approx(0));
    }

    SECTION("array3")
    {
        ArrayPtr aptr = cnv->createArray("array3", 5);
        REQUIRE(aptr.get());
        REQUIRE(aptr->at(0) == Approx(0));
        REQUIRE(aptr->at(1) == Approx(0));
        REQUIRE(aptr->at(2) == Approx(0));
        REQUIRE(aptr->at(3) == Approx(0));
        REQUIRE(aptr->at(4) == Approx(0));

        TestArrayWindow t("array.window", LA("array3", "@type", "rect"));

        // explicit window specification
        REQUIRE(aptr->at(0) == Approx(1));
        REQUIRE(aptr->at(1) == Approx(1));
        REQUIRE(aptr->at(2) == Approx(1));
        REQUIRE(aptr->at(3) == Approx(1));
        REQUIRE(aptr->at(4) == Approx(1));
    }

    SECTION("types")
    {
#define CHECK_TYPE(name)                                      \
    {                                                         \
        TestArrayWindow t("array.window", LA("@type", name)); \
        REQUIRE_PROPERTY_LIST(t, @type, LA(name));            \
    }

        CHECK_TYPE("tri")
        CHECK_TYPE("welch")
        CHECK_TYPE("hann")
        CHECK_TYPE("rect")
        CHECK_TYPE("sine")
        CHECK_TYPE("hamming")
        CHECK_TYPE("blackman")
        CHECK_TYPE("nuttall")
        CHECK_TYPE("blackman-harris")
        CHECK_TYPE("flattop")
        CHECK_TYPE("gauss")

        TestArrayWindow t("array.window", LA("@type", "unknown???"));
        REQUIRE_PROPERTY_LIST(t, @type, LA("hann"));
    }

    SECTION("messages")
    {
        TestArrayWindow t("array.window");

#define CHECK_MSG(m)                            \
    {                                           \
        t.anyDispatch(SYM(m), L());             \
        REQUIRE_PROPERTY_LIST(t, @type, LA(m)); \
    }

        CHECK_MSG("tri")
        CHECK_MSG("welch")
        CHECK_MSG("hann")
        CHECK_MSG("rect")
        CHECK_MSG("sine")
        CHECK_MSG("hamming")
        CHECK_MSG("blackman")
        CHECK_MSG("nuttall")
        CHECK_MSG("blackman-harris")
        CHECK_MSG("flattop")
        CHECK_MSG("gauss")

        t.anyDispatch(SYM("unknown"), L());
        REQUIRE_PROPERTY_LIST(t, @type, LA("gauss"));
    }

    SECTION("do")
    {
        ArrayPtr aptr = cnv->createArray("array4", 5);
        REQUIRE(aptr.get());
        REQUIRE(aptr->at(0) == Approx(0));
        REQUIRE(aptr->at(1) == Approx(0));
        REQUIRE(aptr->at(2) == Approx(0));
        REQUIRE(aptr->at(3) == Approx(0));
        REQUIRE(aptr->at(4) == Approx(0));

        TestArrayWindow t("array.window", LA("array4"));

        t.anyDispatch(SYM("tri"), L());
        REQUIRE_BANG_AT_OUTLET(0, t);
        REQUIRE(aptr->at(0) == Approx(0));
        REQUIRE(aptr->at(1) == Approx(0.5));
        REQUIRE(aptr->at(2) == Approx(1));
        REQUIRE(aptr->at(3) == Approx(0.5));
        REQUIRE(aptr->at(4) == Approx(0));

        t.anyDispatch(SYM("rect"), L());
        REQUIRE(aptr->at(0) == Approx(1));
        REQUIRE(aptr->at(1) == Approx(1));
        REQUIRE(aptr->at(2) == Approx(1));
        REQUIRE(aptr->at(3) == Approx(1));
        REQUIRE(aptr->at(4) == Approx(1));
    }
}
