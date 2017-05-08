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
#include "../array/array_each.h"
#include "base_extension_test.h"
#include "ceammc_factory.h"
#include "ceammc_pd.h"

#include "catch.hpp"

typedef TestExtension<ArrayEach> ArrayEachTest;

using namespace ceammc;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("array.each", "[externals]")
{
    SECTION("empty")
    {
        ArrayEachTest t("array.each");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE_PROPERTY_NONE(t, @array);
        REQUIRE_PROPERTY(t, @redraw, 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("invalid")
    {
        ArrayEachTest t("array.each", L1("non-exists"));
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE_PROPERTY(t, @array, "non-exists");
        REQUIRE_PROPERTY(t, @redraw, 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("array1")
    {
        ArrayEachTest t("array.each", L1("array1"));

        // no array yet
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        ArrayPtr aptr = cnv->createArray("array1", 10);
        aptr->fillWith(-10);

        // array created
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_FLOAT_AT_OUTLET(1, t, -10);
        REQUIRE_BANG_AT_OUTLET(0, t);
    }

    SECTION("array1 connected")
    {
        ObjectFactory<ArrayEach> obj("array.each");

        t_atom argv;
        SETSYMBOL(&argv, gensym("array1"));
        void* x = ObjectFactory<ArrayEach>::object_new(0, 1, &argv);

        ObjectFactory<ArrayEach>::ObjectProxy* p = (ObjectFactory<ArrayEach>::ObjectProxy*)x;
        REQUIRE(p->impl->numInlets() == 2);
        REQUIRE(p->impl->property("@array")->get() == L1("array1"));

        Array a("array1");
        a.fillWith(4);

        pd::External mul("sqrt");
        REQUIRE(!mul.isNull());

        REQUIRE(Canvas::connect(&p->pd_obj, 1, mul.object(), 0));
        REQUIRE(mul.connectTo(0, &p->pd_obj, 1));

        // array created
        pd_bang(&p->pd_obj.te_g.g_pd);
        REQUIRE(a[0] == 2);
    }
}
