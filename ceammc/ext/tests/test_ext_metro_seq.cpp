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
#include "../base/metro_seq.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<MetroSeq> MetroSeqTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("metro.seq", "[externals]")
{
    SECTION("init")
    {
        SECTION("default")
        {
            MetroSeqTest t("metro.seq");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_LIST(t, @pattern, AtomList());
            REQUIRE_PROPERTY(t, @interval, 0.f);
            REQUIRE_PROPERTY(t, @current, 0.f);

            setup_metro_seq();
        }

        SECTION("invalid")
        {
            MetroSeqTest t("metro.seq", L5(500, "A", "B", -100, 10));

            REQUIRE_PROPERTY(t, @interval, 500);
            REQUIRE_PROPERTY_LIST(t, @pattern, L4(0.f, 0.f, 1, 1));
        }

        SECTION("ok")
        {
            MetroSeqTest t("metro.seq", L5(200, 1, 1, 0.f, 1));
            REQUIRE_PROPERTY(t, @interval, 200);
            REQUIRE_PROPERTY_LIST(t, @pattern, L4(1, 1, 0.f, 1));
        }
    }

    SECTION("set pattern")
    {
        MetroSeqTest t("metro.seq");
        REQUIRE_PROPERTY_LIST(t, @pattern, AtomList());
        t.setProperty("@pattern", L4(1, 1, 0.f, 1));
        REQUIRE_PROPERTY_LIST(t, @pattern, L4(1, 1, 0.f, 1));
        t.setProperty("@pattern", L4("A", "B", -100, 10));
        REQUIRE_PROPERTY_LIST(t, @pattern, L4(0.f, 0.f, 1, 1));
        t.setProperty("@pattern", AtomList());
        REQUIRE_PROPERTY_LIST(t, @pattern, L4(0.f, 0.f, 1, 1));
    }

    SECTION("set current")
    {
        MetroSeqTest t("metro.seq");
        REQUIRE_PROPERTY(t, @current, 0.f);
        t.setProperty("@current", L1(20));
        t.setProperty("@current", L1(0.f));
        REQUIRE_PROPERTY(t, @current, 0.f);

        t.setProperty("@pattern", L4(1, 1, 0.f, 1));
        t.setProperty("@current", L1(0.f));
        REQUIRE_PROPERTY(t, @current, 0.f);
        t.setProperty("@current", L1(1.f));
        REQUIRE_PROPERTY(t, @current, 1.f);
        t.setProperty("@current", L1(2.f));
        REQUIRE_PROPERTY(t, @current, 2.f);
        t.setProperty("@current", L1(3.f));
        REQUIRE_PROPERTY(t, @current, 3.f);
        t.setProperty("@current", L1(4.f));
        REQUIRE_PROPERTY(t, @current, 3.f);
    }

    SECTION("tick")
    {
        MetroSeqTest t("metro.seq");
        t.clockTick();
        t.setProperty("@current", L1(0.f));

        t.setProperty("@pattern", L4(1, 1, 0.f, 1));
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE_PROPERTY_LIST(t, @pattern, L4(1, 1, 0.f, 1));
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
        t.setProperty("@pattern", L1(1));
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
        t.setProperty("@pattern", L2(1, 0.f));
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
        MetroSeqTest t("metro.seq");
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 0.f);
        t.setProperty("@interval", L1(100));
        t.setProperty("@pattern", L2(1, 0.0f));
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_PROPERTY(t, @current, 1.f);
        REQUIRE_BANG_AT_OUTLET(0, t);
    }
}
