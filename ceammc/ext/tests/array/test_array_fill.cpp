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
#include "array_fill.h"
#include "test_array_base.h"

#include <math.h>

PD_COMPLETE_TEST_SETUP(ArrayFill, array, fill)

TEST_CASE("array.fill", "[externals]")
{
    pd_test_init();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("empty")
    {
        TObj t("array.fill");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @array, &s_);
        REQUIRE_PROPERTY(t, @redraw, 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("invalid")
    {
        TObj t("array.fill", LA("non-exists"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @array, "non-exists");
        REQUIRE_PROPERTY(t, @redraw, 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("fill")
    {
        TObj t("array.fill", LA("array_fill1"));

        // no array yet
        WHEN_CALL_N(t, fill, 0.1f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        ArrayPtr aptr = cnv->createArray("array_fill1", 10);

        // array created
        WHEN_CALL_N(t, fill, 0.1f);
        REQUIRE_BANG_AT_OUTLET(0, t);
        aptr->update();
        REQUIRE(aptr->size() == 10);

        for (size_t i = 0; i < aptr->size(); i++)
            REQUIRE(aptr->at(i) == Approx(0.1));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        // pattern
        WHEN_CALL_N(t, fill, 0.1f, 0.2f);
        REQUIRE_BANG_AT_OUTLET(0, t);
        aptr->update();
        REQUIRE(aptr->size() == 10);

        for (size_t i = 0; i < aptr->size(); i++) {
            REQUIRE(aptr->at(i) == Approx(i % 2 == 0 ? 0.1 : 0.2));
        }

        aptr->update();
        REQUIRE(aptr->size() == 10);

        // pattern
        WHEN_CALL_N(t, fill, 0.1f, 0.2f, 0.3f);
        REQUIRE_BANG_AT_OUTLET(0, t);
        aptr->update();
        REQUIRE(aptr->size() == 10);

        for (size_t i = 0; i < aptr->size(); i++) {
            double v = 0.1;
            if (i % 3 == 1)
                v = 0.2;
            if (i % 3 == 2)
                v = 0.3;

            REQUIRE(aptr->at(i) == Approx(v));
        }

        REQUIRE(aptr->at(9) == 0.1f);

        // pattern
        WHEN_CALL_N(t, fill, 0.1f);
        REQUIRE_BANG_AT_OUTLET(0, t);

        aptr->resize(2);
        // pattern
        WHEN_CALL_N(t, fill, 1, 2, 3);
        REQUIRE_BANG_AT_OUTLET(0, t);
        REQUIRE(aptr->at(0) == 1.f);
        REQUIRE(aptr->at(1) == 2.f);

        aptr->resize(1);
        // pattern
        WHEN_CALL_N(t, fill, 3, 4, 5);
        REQUIRE_BANG_AT_OUTLET(0, t);
        REQUIRE(aptr->at(0) == 3.f);
    }

    SECTION("range")
    {
        TObj t("array.fill", LA("array2"));

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        Array a("array_fill1");
        a.resize(5);
        a.fillWith(0.f);

        WHEN_CALL_N(t, fill, 1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_CALL_N(t, fill, 1, 2);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_SYMBOL_TO(0, t, "array_fill1");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @array, A("array_fill1"));

        WHEN_CALL_N(t, fill, "@from", 2, "@to", 3, 10);
        REQUIRE_BANG_AT_OUTLET(0, t);
        a.update();
        REQUIRE(a[0] == 0.f);
        REQUIRE(a[1] == 0.f);
        REQUIRE(a[2] == 10.f);
        REQUIRE(a[3] == 0.f);
        REQUIRE(a[4] == 0.f);

        WHEN_CALL_N(t, fill, 1);
        REQUIRE_BANG_AT_OUTLET(0, t);
        REQUIRE(a[0] == 1.f);
        REQUIRE(a[1] == 1.f);
        REQUIRE(a[2] == 1.f);
        REQUIRE(a[3] == 1.f);
        REQUIRE(a[4] == 1.f);
        a.fillWith(0.f);

        WHEN_CALL_N(t, fill, "@to", 2, "@from", 0.f, 20);
        REQUIRE_BANG_AT_OUTLET(0, t);
        WHEN_CALL_N(t, fill, "@from", 2, "@to", 3, 10);

        a.update();
        REQUIRE(a[0] == 20.f);
        REQUIRE(a[1] == 20.f);
        REQUIRE(a[2] == 10.f);
        REQUIRE(a[3] == 0.f);
        REQUIRE(a[4] == 0.f);

        WHEN_CALL_N(t, fill, "@from", 4, "@to", 2333, 44);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_CALL_N(t, fill, "@from", 3, "@to", -1, 25);
        REQUIRE_BANG_AT_OUTLET(0, t);
        REQUIRE(a[0] == 20.f);
        REQUIRE(a[0] == 20.f);
        REQUIRE(a[2] == 10.f);
        REQUIRE(a[3] == 25.f);
        REQUIRE(a[4] == 0.f);

        WHEN_CALL_N(t, fill, "@from", 2, "@to", -2, 24);
        REQUIRE_BANG_AT_OUTLET(0, t);
        REQUIRE(a[0] == 20.f);
        REQUIRE(a[1] == 20.f);
        REQUIRE(a[2] == 24.f);
        REQUIRE(a[3] == 25.f);
        REQUIRE(a[4] == 0.f);
    }

    SECTION("fill float")
    {
        TObj t("array.fill", LA("array2"));

        WHEN_SEND_FLOAT_TO(0, t, 0.1f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_SYMBOL_TO(0, t, "array_fill1");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        Array a("array_fill1");
        a.resize(5);

        WHEN_SEND_FLOAT_TO(0, t, 0.1f);
        REQUIRE_BANG_AT_OUTLET(0, t);

        for (size_t i = 0; i < a.size(); i++) {
            REQUIRE(a[i] == 0.1f);
        }
    }

    SECTION("fill list")
    {
        TObj t("array.fill", LA("array2"));

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_SYMBOL_TO(0, t, "array_fill1");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        Array a("array_fill1");
        a.resize(5);

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_BANG_AT_OUTLET(0, t);

        REQUIRE(a[0] == 1.f);
        REQUIRE(a[1] == 2.f);
        REQUIRE(a[2] == 1.f);
        REQUIRE(a[3] == 2.f);
        REQUIRE(a[4] == 1.f);
    }

    SECTION("parse range arguments")
    {
        TObj t("array.fill", LA("array_fill1"));

        size_t from = 0;
        size_t to = 0;

        REQUIRE(t.parseRange(LF(1), &from, &to) == LF(1));
        REQUIRE(from == 0);
        REQUIRE(to == 5);
        from = to = 0;

        REQUIRE(t.parseRange(LA("@from"), &from, &to) == L());

        // no params
        REQUIRE(t.parseRange(LF(2, 3), &from, &to) == LF(2, 3));
        REQUIRE(from == 0);
        REQUIRE(to == 5);
        from = to = 0;

        // @from
        REQUIRE(t.parseRange(LA("@from", 1, 2, 3), &from, &to) == LF(2, 3));
        REQUIRE(from == 1);
        REQUIRE(to == 5);
        from = to = 0;

        // @from invalid
        REQUIRE(t.parseRange(LA("@from", -1, 2, 3), &from, &to) == L());

        // @from
        REQUIRE(t.parseRange(LA("@from", 5, 2, 3), &from, &to) == L());

        // @to
        REQUIRE(t.parseRange(LA("@to", 10, 5, 3), &from, &to) == L());

        // @to
        REQUIRE(t.parseRange(LA("@to", 6, 5, 3), &from, &to) == L());

        // @to
        REQUIRE(t.parseRange(LA("@to", 5, 5, 3), &from, &to) == LF(5, 3));
        REQUIRE(from == 0);
        REQUIRE(to == 5);
        from = to = 0;

        // @to
        REQUIRE(t.parseRange(LA("@to", 3, 5, 3), &from, &to) == LF(5, 3));
        REQUIRE(from == 0);
        REQUIRE(to == 3);
        from = to = 0;

        // @to
        REQUIRE(t.parseRange(LA("@to", -1, 5, 3), &from, &to) == LF(5, 3));
        REQUIRE(from == 0);
        REQUIRE(to == 4);
        from = to = 0;

        // @to
        REQUIRE(t.parseRange(LA("@to", -3, 5, 3), &from, &to) == LF(5, 3));
        REQUIRE(from == 0);
        REQUIRE(to == 2);
        from = to = 0;

        // @to
        REQUIRE(t.parseRange(LA("@to", -5, 5, 3), &from, &to) == L());
        REQUIRE(from == 0);
        REQUIRE(to == 0);
        from = to = 0;

        // @to
        REQUIRE(t.parseRange(LA("@to", -15, 5, 3), &from, &to) == L());
        REQUIRE(from == 0);
        REQUIRE(to == 0);
        from = to = 0;

        // @from @to
        REQUIRE(t.parseRange(LA("@to", 3, "@from", 2, 5, 3), &from, &to) == LF(5, 3));
        REQUIRE(from == 2);
        REQUIRE(to == 3);
        from = to = 0;

        // @from @to
        REQUIRE(t.parseRange(LA("@to", 2, "@from", 2, 5, 3), &from, &to) == L());
        REQUIRE(from == 0);
        REQUIRE(to == 0);
        from = to = 0;

        // @from @to
        REQUIRE(t.parseRange(LA("@from", 2, "@to", 1, 5, 3), &from, &to) == L());
        REQUIRE(from == 0);
        REQUIRE(to == 0);
        from = to = 0;

        // @from @to
        REQUIRE(t.parseRange(LA("@from", 2, "@to", -2, 5, 3), &from, &to) == LF(5, 3));
        REQUIRE(from == 2);
        REQUIRE(to == 3);
        from = to = 0;
    }

    SECTION("saw")
    {
        TObj t("array.fill", LA("array_fill1"));

        Array a("array_fill1");
        a.resize(5);
        a.fillWith(0.f);

        t.m_saw(&s_, LA(4, 1));
        REQUIRE(a[0] == -1);
        REQUIRE(a[1] == Approx(-0.5));
        REQUIRE(a[2] == 0);
        REQUIRE(a[3] == 0.5);
        REQUIRE(a[4] == -1);

        t.m_saw(&s_, LA(4, 2));
        REQUIRE(a[0] == -2);
        REQUIRE(a[1] == Approx(-1));
        REQUIRE(a[2] == 0);
        REQUIRE(a[3] == 1);
        REQUIRE(a[4] == -2);

        t.m_saw(&s_, LA(4, 2, 0.25));
        REQUIRE(a[0] == Approx(-1));
        REQUIRE(a[1] == Approx(0));
        REQUIRE(a[2] == Approx(1));
        REQUIRE(a[3] == Approx(-2));
        REQUIRE(a[4] == Approx(-1));
    }
}
