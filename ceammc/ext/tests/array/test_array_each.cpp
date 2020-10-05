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
#include "array_each.h"
#include "test_array_base.h"

PD_COMPLETE_TEST_SETUP(ArrayEach, array, each)

TEST_CASE("array.each", "[externals]")
{
    pd_test_init();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("empty")
    {
        TObj t("array.each");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE_PROPERTY(t, @array, &s_);
        REQUIRE_PROPERTY(t, @redraw, 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("invalid")
    {
        TObj t("array.each", LA("non-exists"));
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE_PROPERTY(t, @array, "non-exists");
        REQUIRE_PROPERTY(t, @redraw, 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("array_each1")
    {
        TObj t("array.each", LA("array_each"));

        // no array yet
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        ArrayPtr aptr = cnv->createArray("array_each", 10);
        aptr->fillWith(-10);

        // array created
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_FLOAT_AT_OUTLET(1, t, -10);
        REQUIRE_BANG_AT_OUTLET(0, t);
    }

    SECTION("array1 connected")
    {
        TExt t("array.each", "array_each1");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE_PROPERTY(t, @array, "array_each1");

        External mul("*", 2);

        // [list.each] X [* 2]
        REQUIRE(t.object());
        REQUIRE(mul.object());
        REQUIRE(t.connectTo(1, mul, 0));
        REQUIRE(t.connectFrom(0, mul, 1));

        ArrayPtr aptr = cnv->createArray("array_each1", 10);
        aptr->fillWith(4);

        t.bang();
        REQUIRE(t.lastMessage(0).isBang());

        for (auto f : *aptr) {
            REQUIRE(f == 8);
        }
    }
}
