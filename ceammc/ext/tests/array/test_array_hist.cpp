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
#include "array_hist.h"
#include "test_array_base.h"

PD_COMPLETE_TEST_SETUP(ArrayHist, array, hist)

TEST_CASE("array.hist", "[externals]")
{
    pd_test_init();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("empty")
    {
        TestArrayHist t("array.hist");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @array, &s_);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("invalid")
    {
        TestArrayHist t("array.hist", LA("non-exists"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @array, "non-exists");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("array1")
    {
        TestArrayHist t("array.hist", LA("array_hist1", 3));

        // no array yet
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        ArrayPtr aptr = cnv->createArray("array_hist1", 4);
        Array a("array_hist1", { 0, 0, 0, 0, 0 });
        REQUIRE(a.size() == 5);

        // array created
        WHEN_SEND_BANG_TO(0, t);
        return;
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 5, 0));

        a.set({ 0, 0, 0, -1, 0, 0 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 5, 0));

        a.set({ 0, 0, 0, -1, -1, 0 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(2, 4, 0));

        // ignore -1.1 value
        a.set({ 0, 0, 0, -1, -1.1, 0 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4, 0));

        // ignore -2 value
        a.set({ 0, 0, 0, -1, -2, 0 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 4, 0));

        a.set({ 0, 0, 0, 1, 1, 1 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 3, 3));

        // ignore 1.01
        a.set({ 0, 0, 0, 1, 1, 1.01 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 3, 2));

        a.set({ 0, 0.1, 0.2, 0.3, 0.9 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 4, 1));

        a.set({ 0, 0.1, 0.2, 0.3, 0.8 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 4, 1));

        a.set({ 0, 0.1, 0.2, 0.3, 0.7 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 4, 1));

        a.set({ 0, 0.1, 0.2, 0.3, 0.6 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 4, 1));

        a.set({ 0, 0.1, 0.2, 0.3, 0.5 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 4, 1));

        a.set({ 0, 0.1, 0.2, 0.3, 0.4 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 4, 1));

        a.set({ 0, 0.1, 0.2, 0.3, 0.3 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 5, 0));

        a.set({ 0, 0.1, 0.2, 0.3, 0.33 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 5, 0));

        a.set({ 0, 0.1, 0.2, 0.3, 0.35 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 4, 1));

        a.set({ -0.35, 0.1, 0.2, 0.3, 0.35 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 3, 1));

        a.set({ -0.5, 0.1, 0.2, 0.3, 0.35 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 3, 1));

        a.set({ -1, 0.1, 0.2, -1, 0.35 });
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(2, 2, 1));
    }
}
