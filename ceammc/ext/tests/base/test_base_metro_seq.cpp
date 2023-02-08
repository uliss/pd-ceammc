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
#include "catch.hpp"
#include "metro_seq.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(MetroSeq, metro, seq);

TEST_CASE("metro.seq", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("metro.seq");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_LIST(t, @pattern, L());
            REQUIRE_PROPERTY(t, @interval, 0.f);
            REQUIRE_PROPERTY(t, @current, 0.f);

            setup_metro_seq();
        }

        SECTION("invalid")
        {
            TExt t("metro.seq", LA(500, "A", "B", -100, 10));

            REQUIRE_PROPERTY(t, @interval, 500);
            REQUIRE_PROPERTY_LIST(t, @pattern, LA(0.f, 0.f, 1, 1));
        }

        SECTION("ok")
        {
            TExt t("metro.seq", LA(200, 1, 1, 0.f, 1));
            REQUIRE_PROPERTY(t, @interval, 200);
            REQUIRE_PROPERTY_LIST(t, @pattern, LA(1, 1, 0.f, 1));
        }
    }

    SECTION("set pattern")
    {
        TObj t("metro.seq");
        REQUIRE_PROPERTY_LIST(t, @pattern, L());
        t.setProperty("@pattern", LF(1, 1, 0, 1));
        REQUIRE_PROPERTY_LIST(t, @pattern, LF(1, 1, 0, 1));
        t.setProperty("@pattern", LA("A", "B", -100, 10));
        REQUIRE_PROPERTY_LIST(t, @pattern, LF(0, 0, 1, 1));
        t.setProperty("@pattern", L());
        REQUIRE_PROPERTY_LIST(t, @pattern, LF(0, 0, 1, 1));
    }

    SECTION("set pattern 2nd inlet")
    {
        TExt t("metro.seq");
        REQUIRE_PROPERTY_LIST(t, @pattern, L());

        t.sendListTo(LF(1, 1, 0, 1), 1);
        REQUIRE_PROPERTY_LIST(t, @pattern, LF(1, 1, 0, 1));
        t.sendListTo(LA("A", "B", -100, 10), 1);
        REQUIRE_PROPERTY_LIST(t, @pattern, LF(0, 0, 1, 1));
        t.sendListTo(L(), 1);
        REQUIRE_PROPERTY_LIST(t, @pattern, LF(0, 0.f, 1, 1));
    }

    SECTION("set current")
    {
        TObj t("metro.seq");
        REQUIRE_PROPERTY(t, @current, 0.f);
        t.setProperty("@current", LF(20));
        t.setProperty("@current", LF(0.f));
        REQUIRE_PROPERTY(t, @current, 0.f);

        t.setProperty("@pattern", LA(1, 1, 0.f, 1));
        t.setProperty("@current", LF(0.f));
        REQUIRE_PROPERTY(t, @current, 0.f);
        t.setProperty("@current", LF(1.f));
        REQUIRE_PROPERTY(t, @current, 1.f);
        t.setProperty("@current", LF(2.f));
        REQUIRE_PROPERTY(t, @current, 2.f);
        t.setProperty("@current", LF(3.f));
        REQUIRE_PROPERTY(t, @current, 3.f);
        t.setProperty("@current", LF(4.f));
        REQUIRE_PROPERTY(t, @current, 3.f);
    }

    SECTION("tick")
    {
        TObj t("metro.seq");
        t.clockTick();
        t.setProperty("@current", LF(0.f));

        t.setProperty("@pattern", LA(1, 1, 0.f, 1));
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE_PROPERTY_LIST(t, @pattern, LA(1, 1, 0.f, 1));
        t.storeAllMessageCount();
        t.clockTick();
        REQUIRE_PROPERTY(t, @current, 1.f);
        REQUIRE_BANG_AT_OUTLET(0, t);
        t.storeAllMessageCount();
        t.clockTick();
        REQUIRE_PROPERTY(t, @current, 2.f);
        REQUIRE_BANG_AT_OUTLET(0, t);
        t.storeAllMessageCount();
        t.clockTick();
        REQUIRE_PROPERTY(t, @current, 3.f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.storeAllMessageCount();
        t.clockTick();
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE_BANG_AT_OUTLET(0, t);

        // one-element
        t.setProperty("@pattern", LF(1));
        REQUIRE_PROPERTY(t, @current, 0.f);
        t.storeAllMessageCount();
        t.clockTick();
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE_BANG_AT_OUTLET(0, t);
        t.storeAllMessageCount();
        t.clockTick();
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE_BANG_AT_OUTLET(0, t);
        t.storeAllMessageCount();
        t.clockTick();
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE_BANG_AT_OUTLET(0, t);

        // two-element
        t.setProperty("@pattern", LF(1, 0.f));
        REQUIRE_PROPERTY(t, @current, 0.f);
        t.storeAllMessageCount();
        t.clockTick();
        REQUIRE_PROPERTY(t, @current, 1.f);
        REQUIRE_BANG_AT_OUTLET(0, t);
        t.storeAllMessageCount();
        t.clockTick();
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        t.storeAllMessageCount();
        t.clockTick();
        REQUIRE_PROPERTY(t, @current, 1.f);
        REQUIRE_BANG_AT_OUTLET(0, t);
        t.storeAllMessageCount();
        t.clockTick();
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("onFloat")
    {
        TObj t("metro.seq");
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 0.f);
        t.setProperty("@interval", LF(100));
        t.setProperty("@pattern", LA(1, 0.0f));
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_PROPERTY(t, @current, 1.f);
        REQUIRE_BANG_AT_OUTLET(0, t);
    }
}
