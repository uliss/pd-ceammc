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
#include "../base/metro_pattern.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<MetroPattern> MetroPatternTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("metro.pattern", "[externals]")
{
    SECTION("init")
    {
        SECTION("default")
        {
            MetroPatternTest t("metro.pattern");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @pattern, AtomList());
            REQUIRE_PROPERTY(t, @current, 0.f);

            REQUIRE_FALSE(t.next());

            setup_metro_pattern();
        }

        SECTION("wrong init")
        {
            MetroPatternTest t("metro.pattern", L5("A", "B", 200, 0.f, -100));
            REQUIRE_PROPERTY_LIST(t, @pattern, L1(200));
            REQUIRE_PROPERTY(t, @current, 0.f);
        }

        SECTION("ok init")
        {
            MetroPatternTest t("metro.pattern", L5(10, 20, 30, 40, 10));
            REQUIRE_PROPERTY_LIST(t, @pattern, L5(10, 20, 30, 40, 10));
            REQUIRE_PROPERTY(t, @current, 0.f);
        }
    }

    SECTION("pattern")
    {
        MetroPatternTest t("metro.pattern");
        REQUIRE_PROPERTY_LIST(t, @pattern, AtomList());
        t.setProperty("@pattern", L2("A", "B"));
        REQUIRE_PROPERTY_LIST(t, @pattern, AtomList());
        t.setProperty("@pattern", L2(-10, 10));
        REQUIRE_PROPERTY_LIST(t, @pattern, AtomList());
        t.setProperty("@pattern", L2(10, 20));
        REQUIRE_PROPERTY_LIST(t, @pattern, L2(10, 20));
        t.setProperty("@pattern", AtomList());
        REQUIRE_PROPERTY_LIST(t, @pattern, L2(10, 20));
        t.setProperty("@current", L1(1));
        t.setProperty("@pattern", L3(10, 20, 30));
        REQUIRE_PROPERTY_LIST(t, @pattern, L3(10, 20, 30));
        REQUIRE_PROPERTY(t, @current, 1);

        t.setProperty("@pattern", L1(100));
        REQUIRE_PROPERTY_LIST(t, @pattern, L1(100));
        REQUIRE_PROPERTY(t, @current, 0.f);
    }

    SECTION("currentDelay")
    {
        MetroPatternTest t("metro.pattern");
        REQUIRE(t.currentDelay() == 0.f);

        t.setProperty("@pattern", L3(10, 20, 30));
        REQUIRE(t.currentDelay() == 10);
        t.setProperty("@current", L1(1));
        REQUIRE(t.currentDelay() == 20);
        t.setProperty("@current", L1(2));
        REQUIRE(t.currentDelay() == 30);
        t.setProperty("@current", L1(3));
        REQUIRE(t.currentDelay() == 10);
        t.setProperty("@current", L1(4));
        REQUIRE(t.currentDelay() == 10);
        t.setProperty("@current", L1(100));
        REQUIRE(t.currentDelay() == 10);
    }

    SECTION("next")
    {
        MetroPatternTest t("metro.pattern");
        REQUIRE(t.currentDelay() == 0.f);
        REQUIRE_FALSE(t.next());
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE(t.currentDelay() == 0);

        // one-element pattern
        t.setProperty("@pattern", L1(10));
        REQUIRE(t.next());
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE(t.currentDelay() == 10);
        REQUIRE(t.next());
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE(t.currentDelay() == 10);

        // two-element pattern
        t.setProperty("@pattern", L2(10, 20));
        REQUIRE(t.next());
        REQUIRE_PROPERTY(t, @current, 1.f);
        REQUIRE(t.currentDelay() == 20);
        REQUIRE(t.next());
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE(t.currentDelay() == 10);
        REQUIRE(t.next());
        REQUIRE_PROPERTY(t, @current, 1.f);
        REQUIRE(t.currentDelay() == 20);

        // 8-element pattern
        t.setProperty("@pattern", L4(10, 20, 30, 40));
        t.setProperty("@current", L1(0.f));
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE(t.next());
        REQUIRE_PROPERTY(t, @current, 1.f);
        REQUIRE(t.next());
        REQUIRE_PROPERTY(t, @current, 2.f);
        REQUIRE(t.next());
        REQUIRE_PROPERTY(t, @current, 3.f);
        REQUIRE(t.next());
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE(t.next());
    }

    SECTION("tick")
    {
        MetroPatternTest t("metro.pattern", L3(100, 200, 300));

        REQUIRE(t.currentDelay() == 100);
        t.tick();
        REQUIRE_BANG_AT_OUTLET(0, t);
        REQUIRE(t.currentDelay() == 200);
        t.tick();
        REQUIRE_BANG_AT_OUTLET(0, t);
        REQUIRE(t.currentDelay() == 300);
        t.tick();
        REQUIRE_BANG_AT_OUTLET(0, t);
        REQUIRE(t.currentDelay() == 100);
        t.tick();
        REQUIRE_BANG_AT_OUTLET(0, t);
        REQUIRE(t.currentDelay() == 200);
        t.tick();
        REQUIRE_BANG_AT_OUTLET(0, t);

        MetroPatternTest t2("metro.pattern");
        t2.storeMessageCount(0);
        t2.tick();
        REQUIRE(t2.currentDelay() == 0);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t2);
    }

    SECTION("onFloat")
    {
        MetroPatternTest t("metro.pattern", L3(100, 200, 300));

        REQUIRE(t.currentDelay() == 100);
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.currentDelay() == 200);
        REQUIRE_PROPERTY(t, @current, 1);

        WHEN_SEND_FLOAT_TO(0, t, 0.f);
        REQUIRE(t.currentDelay() == 200);
        REQUIRE_PROPERTY(t, @current, 1);

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.currentDelay() == 300);
        REQUIRE_PROPERTY(t, @current, 2);
    }
}
