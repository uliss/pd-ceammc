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
#include "metro_pattern.h"
#include "test_base.h"
#include "test_external.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

PD_COMPLETE_TEST_SETUP(MetroPattern, metro, pattern)

#define BANG_AFTER_NTICKS(obj, n)  \
    {                              \
        obj.clearAll();            \
        obj.schedTicks(n - 1);     \
        REQUIRE(!obj.hasOutput()); \
        obj.schedTicks(1);         \
        REQUIRE(obj.hasOutput());  \
    }

#define NO_BANG_AFTER_NTICKS(obj, n)    \
    {                                   \
        obj.clearAll();                 \
        obj.schedTicks(n);              \
        REQUIRE_FALSE(obj.hasOutput()); \
    }

TEST_CASE("metro.pattern", "[externals]")
{
    pd_test_init();
    canvas_suspend_dsp();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("metro.pattern");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY_LIST(t, @pattern, L());
            REQUIRE_PROPERTY(t, @current, 0.f);
            REQUIRE_PROPERTY_FLOAT(t, @sync, 0);

            REQUIRE_FALSE(t.next());
        }

        SECTION("wrong init")
        {
            TObj t("metro.pattern", LA("A", "B", 200, 0.f, -100));
            REQUIRE_PROPERTY_LIST(t, @pattern, L());
            REQUIRE_PROPERTY(t, @current, 0.f);
        }

        SECTION("ok init")
        {
            TObj t("metro.pattern", LA(10, 20, 30, 40, 10));
            REQUIRE_PROPERTY_LIST(t, @pattern, LA(10, 20, 30, 40, 10));
            REQUIRE_PROPERTY(t, @current, 0.f);
        }
    }

    SECTION("pattern")
    {
        TObj t("metro.pattern");
        REQUIRE_PROPERTY_LIST(t, @pattern, L());
        t.setProperty("@pattern", LA("A", "B"));
        REQUIRE_PROPERTY_LIST(t, @pattern, L());
        t.setProperty("@pattern", LA(-10, 10));
        REQUIRE_PROPERTY_LIST(t, @pattern, L());
        t.setProperty("@pattern", LF(10, 20));
        REQUIRE_PROPERTY_LIST(t, @pattern, LF(10, 20));
        t.setProperty("@pattern", L());
        REQUIRE_PROPERTY_LIST(t, @pattern, LF(10, 20));
        t.setProperty("@current", LF(1));
        t.setProperty("@pattern", LF(10, 20, 30));
        REQUIRE_PROPERTY_LIST(t, @pattern, LF(10, 20, 30));
        REQUIRE_PROPERTY(t, @current, 1);

        t.setProperty("@pattern", LF(100));
        REQUIRE_PROPERTY_LIST(t, @pattern, LF(100));
        REQUIRE_PROPERTY(t, @current, 1);
    }

    SECTION("currentDelay")
    {
        TObj t("metro.pattern");
        REQUIRE(t.currentDelay() == 0.f);

        t.setProperty("@pattern", LF(10, 20, 30));
        REQUIRE(t.currentDelay() == 10);
        t.setProperty("@current", LF(1));
        REQUIRE(t.currentDelay() == 20);
        t.setProperty("@current", LF(2));
        REQUIRE(t.currentDelay() == 30);
        t.setProperty("@current", LF(3));
        REQUIRE(t.currentDelay() == 10);
        t.setProperty("@current", LF(4));
        REQUIRE(t.currentDelay() == 10);
        t.setProperty("@current", LF(100));
        REQUIRE(t.currentDelay() == 10);
    }

    SECTION("next")
    {
        TObj t("metro.pattern");
        REQUIRE(t.currentDelay() == 0.f);
        REQUIRE_FALSE(t.next());
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE(t.currentDelay() == 0);

        // one-element pattern
        t.setProperty("@pattern", LF(10));
        REQUIRE(t.next());
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE(t.currentDelay() == 10);
        REQUIRE(t.next());
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE(t.currentDelay() == 10);

        // two-element pattern
        t.setProperty("@pattern", LF(10, 20));
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
        t.setProperty("@pattern", LF(10, 20, 30, 40));
        t.setProperty("@current", LF(0.f));
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
        TObj t("metro.pattern", LF(100, 200, 300));

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

        TObj t2("metro.pattern");
        t2.storeMessageCount(0);
        t2.tick();
        REQUIRE(t2.currentDelay() == 0);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t2);
    }

    SECTION("onFloat")
    {
        TObj t("metro.pattern", LF(100, 200, 300));

        REQUIRE(t.currentDelay() == 100);
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.currentDelay() == 100);
        REQUIRE_PROPERTY(t, @current, 0.f);

        WHEN_SEND_FLOAT_TO(0, t, 0.f);
        REQUIRE(t.currentDelay() == 100);
        REQUIRE_PROPERTY(t, @current, 0.f);

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.currentDelay() == 100);
        REQUIRE_PROPERTY(t, @current, 0.f);
    }

    SECTION("external")
    {
        setTestSampleRate(44100);
        REQUIRE(1_tick != 0);

        TExt t("metro.pattern", LF(5_ticks, 10_ticks));
        REQUIRE(clock_getlogicaltime() == 0_tick);

        t.clearAll();
        t << 1;
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.outputFloatAt(1) == 0.f);

        REQUIRE(t->currentDelay() == Approx(5_ticks));
        REQUIRE(t->currentIndex() == 0);
        BANG_AFTER_NTICKS(t, 5);
        REQUIRE(t.outputFloatAt(1) == 1);
        BANG_AFTER_NTICKS(t, 10);
        REQUIRE(t.outputFloatAt(1) == 0);
        BANG_AFTER_NTICKS(t, 5);
        BANG_AFTER_NTICKS(t, 10);

        // off
        REQUIRE(t->currentDelay() == Approx(5_ticks));
        t.schedTicks(1);
        t << 0.f;
        NO_BANG_AFTER_NTICKS(t, 100);

        // on
        t << 1.f;
        REQUIRE(t->currentDelay() == Approx(5_ticks));
        REQUIRE(t.hasOutput());
        BANG_AFTER_NTICKS(t, 5);

        // off
        t.schedTicks(8);
        t << 0.f;
        NO_BANG_AFTER_NTICKS(t, 100);

        t << 1.f;
        REQUIRE(t->currentDelay() == Approx(10_ticks));
        REQUIRE(t.hasOutput());
        BANG_AFTER_NTICKS(t, 10);
    }

    SECTION("@sync")
    {
        setTestSampleRate(44100);

        TExt t("metro.pattern", LA(3_ticks, 2_ticks, "@sync", 1));
        REQUIRE(t->property("@pattern")->get() == LX(3_ticks, 2_ticks));

        t.clearAll();
        t << 1.f;

        REQUIRE(t.outputFloatAt(1) == 0);
        REQUIRE(t.isOutputBangAt(0));

        // ???
        BANG_AFTER_NTICKS(t, 4);
        REQUIRE(t.outputFloatAt(1) == 1);
        REQUIRE(t.isOutputBangAt(0));

        BANG_AFTER_NTICKS(t, 2);
        REQUIRE(t.outputFloatAt(1) == 0);
        REQUIRE(t.isOutputBangAt(0));

        BANG_AFTER_NTICKS(t, 3);
        BANG_AFTER_NTICKS(t, 2);

        // sync
        t->setProperty("@pattern", LF(4_ticks, 5_ticks));
        BANG_AFTER_NTICKS(t, 3);
        BANG_AFTER_NTICKS(t, 2);
        REQUIRE(t->property("@pattern")->get() == LX(4_ticks, 5_ticks));
        BANG_AFTER_NTICKS(t, 4);
        BANG_AFTER_NTICKS(t, 5);

        BANG_AFTER_NTICKS(t, 4);
        // sync
        t->setProperty("@pattern", LF(3_ticks, 2_ticks));
        BANG_AFTER_NTICKS(t, 5);
        REQUIRE(t->property("@pattern")->get() == LX(3_ticks, 2_ticks));
        BANG_AFTER_NTICKS(t, 3);
        BANG_AFTER_NTICKS(t, 2);
    }
}
