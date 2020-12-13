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
#include "tl_timeline.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(TlTimeLine, tl, timeline)

#define NO_OUTPUT_NTICKS(obj, n)        \
    {                                   \
        obj.clearAll();                 \
        obj.schedTicks(n);              \
        REQUIRE_FALSE(obj.hasOutput()); \
    }

#define NO_OUTPUT_MSEC(obj, ms)         \
    {                                   \
        obj.clearAll();                 \
        obj.schedMs(ms);                \
        REQUIRE_FALSE(obj.hasOutput()); \
    }

TEST_CASE("tl.timeline", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("tl.timeline");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @is_running, 0);
        REQUIRE_PROPERTY(t, @length, 60);
        REQUIRE_PROPERTY(t, @size, 1);
        REQUIRE_PROPERTY(t, @current, 0);
        REQUIRE_PROPERTY(t, @events, "end");
    }

    SECTION("do empty")
    {
        setTestSampleRate(44100);

        t_float tm = 100_ticks / 1000.0;
        TExt t("tl.timeline", tm);
        REQUIRE(t.object());
        REQUIRE(t->state() == tl::STATE_INIT);

        NO_OUTPUT_NTICKS(t, 50);
        REQUIRE_PROPERTY_FLOAT(t, @current, 0_ticks);

        t.call("start");
        REQUIRE(t->state() == tl::STATE_RUN);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LAX("begin", 0.f));
        REQUIRE_PROPERTY(t, @is_running, 1);

        // save start time
        auto start_tm = clock_getlogicaltime();

        NO_OUTPUT_NTICKS(t, 99);
        REQUIRE(clock_gettimesince(start_tm) == Approx(99_ticks));
        t.schedTicks(2);
        REQUIRE(clock_gettimesince(start_tm) == Approx(101_ticks));
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LAX("end", 100_ticks));
        REQUIRE_PROPERTY(t, @is_running, 0.f);

        NO_OUTPUT_NTICKS(t, 200);
        REQUIRE_PROPERTY(t, @is_running, 0.f);
    }

    SECTION("do empty")
    {
        setTestSampleRate(44100);

        t_float tm = 100_ticks / 1000.0;
        TExt t("tl.timeline", tm);
        REQUIRE(t.object());
        REQUIRE(t->state() == tl::STATE_INIT);
        REQUIRE_PROPERTY(t, @is_running, 0.f);

        // INIT -> START
        t.call("start");
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LAX("begin", 0.f));
        REQUIRE_PROPERTY(t, @is_running, 1);
        REQUIRE(t->state() == tl::STATE_RUN);

        NO_OUTPUT_NTICKS(t, 50);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(50_ticks));

        // START -> STOP
        t.call("stop");
        REQUIRE(t->state() == tl::STATE_STOP);
        REQUIRE(!t.hasOutput());
        REQUIRE_PROPERTY(t, @is_running, 0.f);

        // STOP -> START
        t.call("start");
        REQUIRE(t.outputListAt(0) == LAX("begin", 0.f));
        REQUIRE_PROPERTY(t, @is_running, 1);
        NO_OUTPUT_NTICKS(t, 99);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(99_ticks));
        t.schedTicks(2);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LAX("end", 100_ticks));
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(0_ticks));
        REQUIRE_PROPERTY(t, @is_running, 0.f);

        NO_OUTPUT_NTICKS(t, 200);
        REQUIRE_PROPERTY(t, @is_running, 0.f);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(0_ticks));
    }

    SECTION("do simple")
    {
        using namespace ceammc::tl;
        setTestSampleRate(44100);
        test::pdPrintToStdError();

        auto tm = 100_ticks / 1000;
        TExt t("tl.timeline", LF(tm));
        REQUIRE(t.object());
        REQUIRE_PROPERTY(t, @is_running, 0.f);
        REQUIRE_PROPERTY_LIST(t, @events, LAX("end"));

        t.call("add");
        REQUIRE_PROPERTY_LIST(t, @events, LAX("end"));
        REQUIRE_PROPERTY(t, @size, 1);

        t.call("add", LA(60_ticks, "ms"));
        REQUIRE_PROPERTY(t, @size, 2);
        // twice insert
        t.call("add", LA(60_ticks, "ms"));
        REQUIRE_PROPERTY(t, @size, 2);
        // negative offset
        t.call("add", LA(-30_ticks, "ms"));
        REQUIRE_PROPERTY(t, @size, 3);

        // invalid events
        t.call("add", LA(-200_ticks, "ms"));
        REQUIRE_PROPERTY(t, @size, 3);
        t.call("add", LA(101_ticks, "ms"));
        REQUIRE_PROPERTY(t, @size, 3);

        // events: 60, 70 and 100

        t.call("start");
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LAX("begin", 0.f));
        REQUIRE_PROPERTY(t, @is_running, 1);
        REQUIRE_PROPERTY(t, @current, 0.f);

        NO_OUTPUT_NTICKS(t, 59);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(59_ticks));
        // event at 60
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LX(0, 60_ticks));
        NO_OUTPUT_NTICKS(t, 8);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(69_ticks));
        // event at 70
        t.schedTicks(2);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(71_ticks));
        REQUIRE(t.outputListAt(0) == LX(1, 70_ticks));
        NO_OUTPUT_NTICKS(t, 28);
        // end event
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LAX("end", 100_ticks));
        REQUIRE_PROPERTY_FLOAT(t, @is_running, 0);

        // try to start again
        t.call("start");
        REQUIRE(t.outputListAt(0) == LAX("begin", 0.f));
        REQUIRE_PROPERTY_FLOAT(t, @is_running, 1);
    }

    SECTION("pause")
    {
        setTestSampleRate(44100);
        test::pdPrintToStdError();

        auto start_tm = clock_getlogicaltime();

        t_float tm = 100_ticks / 1000.0;
        TExt t("tl.timeline", tm);

        t.call("pause");
        REQUIRE(!t.hasOutput());

        t.call("add", LA(30_ticks, "ms"));
        REQUIRE_PROPERTY(t, @size, 2);

        // init -> start
        t.call("start");
        REQUIRE(t.outputListAt(0) == LA("begin", 0.f));
        t.call("start");
        REQUIRE(!t.hasOutput());

        // start -> pause
        NO_OUTPUT_NTICKS(t, 20);
        t.call("pause");
        REQUIRE(!t.hasOutput());
        t.call("pause");
        REQUIRE(!t.hasOutput());
        REQUIRE_PROPERTY(t, @is_running, 0.f);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(20_ticks));

        NO_OUTPUT_NTICKS(t, 100);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(20_ticks));

        // pause -> stop
        t.call("stop");
        REQUIRE(!t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(0_ticks));

        // no output on stop
        NO_OUTPUT_NTICKS(t, 10);

        // stop -> pause
        t.call("pause");
        REQUIRE(!t.hasOutput());
        REQUIRE(t->state() == tl::STATE_STOP);

        // stop -> start
        t.call("start");
        REQUIRE(t.outputListAt(0) == LAX("begin", 0.f));
        NO_OUTPUT_NTICKS(t, 10);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(10_ticks));

        // start -> pause
        t.call("pause");
        REQUIRE(t->state() == tl::STATE_PAUSE);
        REQUIRE(!t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(10_ticks));

        // same after time
        NO_OUTPUT_NTICKS(t, 10);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(10_ticks));

        // no more actions
        // pause -> pause
        t.call("pause");
        REQUIRE(t->state() == tl::STATE_PAUSE);
        REQUIRE(!t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(10_ticks));

        NO_OUTPUT_NTICKS(t, 100);

        // continue
        // pause -> start
        t.call("start");
        REQUIRE(t->state() == tl::STATE_RUN);
        REQUIRE(!t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(10_ticks));
        REQUIRE_PROPERTY_FLOAT(t, @is_running, 1);

        NO_OUTPUT_NTICKS(t, 19);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(29_ticks));

        // first event at 30'
        t.schedTicks(1);
        REQUIRE(t.outputListAt(0) == LX(0, 30_ticks));
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(30_ticks));

        NO_OUTPUT_NTICKS(t, 69);
        // last event
        // run -> stop
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LAX("end", 100_ticks));
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(0_ticks));
    }

    SECTION("goto")
    {
        setTestSampleRate(44100);
        test::pdPrintToStdError();

        auto start_tm = clock_getlogicaltime();

        t_float tm = 100_ticks / 1000.0;
        TExt t("tl.timeline", tm);

        t.call("add", LA(-20_ticks, "ms"));
        REQUIRE_PROPERTY(t, @size, 2);

        // invalid
        t.call("to_time", 100_ticks / 1000.0);
        REQUIRE(t->state() == tl::STATE_INIT);
        t.call("to_time", 101_ticks / 1000.0);
        REQUIRE(t->state() == tl::STATE_INIT);
        t.call("to_time", -101_ticks / 1000.0);
        REQUIRE(t->state() == tl::STATE_INIT);

        // init -> goto
        t.call("to_time", LA(10_ticks, "ms"));
        REQUIRE(t->state() == tl::STATE_GOTO);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(10_ticks));

        // invalid: goto -> pause
        t.call("pause");
        REQUIRE(t->state() == tl::STATE_GOTO);
        REQUIRE(!t.hasOutput());

        // invalid: goto -> stop
        t.call("stop");
        REQUIRE(t->state() == tl::STATE_GOTO);
        REQUIRE(!t.hasOutput());

        // goto -> init
        t.call("reset");
        REQUIRE(t->state() == tl::STATE_INIT);
        REQUIRE(!t.hasOutput());

        // init -> goto
        t.call("to_time", LA(20_ticks, "ms"));
        REQUIRE(t->state() == tl::STATE_GOTO);

        // goto -> run
        t.call("start");
        REQUIRE(t->state() == tl::STATE_RUN);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(20_ticks));
        NO_OUTPUT_NTICKS(t, 59);
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LX(0, 80_ticks));

        NO_OUTPUT_NTICKS(t, 10);
        t.call("pause");
        REQUIRE(t->state() == tl::STATE_PAUSE);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(91_ticks));

        // pause -> goto
        t.call("to_time", LA(70_ticks, "ms"));
        REQUIRE(t->state() == tl::STATE_GOTO);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(70_ticks));

        // goto -> goto
        t.call("to_time", LA(79_ticks, "ms"));
        REQUIRE(t->state() == tl::STATE_GOTO);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(79_ticks));

        // goto -> run
        t.call("start");
        REQUIRE(t->state() == tl::STATE_RUN);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(79_ticks));
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LX(0, 80_ticks));

        NO_OUTPUT_NTICKS(t, 18);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(99_ticks));
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LA("end", 100_ticks));

        // stop -> goto
        t.call("add", LA(20_ticks, "ms"));
        t.call("to_time", LA(40_ticks, "ms"));
        REQUIRE(t->state() == tl::STATE_GOTO);
        REQUIRE_PROPERTY_FLOAT(t, @current, Approx(40_ticks));

        // nothing
        NO_OUTPUT_NTICKS(t, 100);

        t.call("start");
        // 40 -> 80 -> 100
        NO_OUTPUT_NTICKS(t, 39);
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LX(1, 80_ticks));
    }

    SECTION("stop->run")
    {
        setTestSampleRate(44100);

        t_float tm = 100_ticks / 1000.0;
        TExt t("tl.timeline", tm);
        t.call("add", LA(30_ticks, "ms"));

        t.call("start");
        REQUIRE(t.outputListAt(0) == LA("begin", 0.f));

        // nothing
        NO_OUTPUT_NTICKS(t, 29);
        t.schedTicks(1);
        REQUIRE(t.outputListAt(0) == LX(0, 30_ticks));
    }

    SECTION("to event")
    {
        setTestSampleRate(44100);

        t_float tm = 100_ticks / 1000.0;
        TExt t("tl.timeline", tm);
        t.call("add", LA(80_ticks, "ms"));
        t.call("add", LA(30_ticks, "ms"));
        t.call("add", LA(50_ticks, "ms"));
        REQUIRE_PROPERTY_FLOAT(t, @size, 4);
        REQUIRE_PROPERTY_LIST(t, @events, LA("event2", "event3", "event1", "end"));

        t.call("to_event", 1);
        REQUIRE_PROPERTY_FLOAT(t, @current, 50_ticks);
        REQUIRE(!t.hasOutput());

        t.call("to_event", -1);
        REQUIRE_PROPERTY_FLOAT(t, @current, 100_ticks);
        REQUIRE(!t.hasOutput());

        t.call("to_event", -2);
        REQUIRE_PROPERTY_FLOAT(t, @current, 80_ticks);
        REQUIRE(!t.hasOutput());

        t.call("to_event", -5);
        REQUIRE_PROPERTY_FLOAT(t, @current, 80_ticks);
        REQUIRE(!t.hasOutput());

        t.call("to_event", -4);
        REQUIRE_PROPERTY_FLOAT(t, @current, 30_ticks);
        REQUIRE(!t.hasOutput());
        t.call("start");
        REQUIRE(t.outputListAt(0) == LX(0, 30_ticks));
        t.call("stop");

        t.call("to_event", 3);
        REQUIRE_PROPERTY_FLOAT(t, @current, 100_ticks);
        REQUIRE(!t.hasOutput());

        t.call("to_event", 4);
        REQUIRE_PROPERTY_FLOAT(t, @current, 100_ticks);
        REQUIRE(!t.hasOutput());

        t.call("start");
        REQUIRE(t.outputListAt(0) == LA("end", 100_ticks));
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LA("end", 100_ticks));
    }

    SECTION("@loop empty")
    {
        setTestSampleRate(64000);

        t_float tm = 10 / 1000.0;
        TExt t("tl.timeline", LA(tm, "@loop", 1));

        t.call("start");
        REQUIRE(t.outputListAt(0) == LA("begin", 0.f));
        NO_OUTPUT_NTICKS(t, 1);

        for (int i = 0; i < 10; i++) {
            REQUIRE_PROPERTY_FLOAT(t, @current, 1);
            NO_OUTPUT_NTICKS(t, 8);
            t.schedTicks(2);
            REQUIRE(t.outputListAt(0) == LAX("end", 10));
        }
    }

    SECTION("@loop events")
    {
        setTestSampleRate(64000);

        t_float tm = 20 / 1000.0;
        TExt t("tl.timeline", LA(tm, "@loop", 1));

        t.call("add", LA(10, "ms"));
        t.call("start");
        REQUIRE(t.outputListAt(0) == LAX("begin", 0.f));
        NO_OUTPUT_NTICKS(t, 1);

        for (int i = 0; i < 10; i++) {
            NO_OUTPUT_NTICKS(t, 8);
            t.schedTicks(2);
            REQUIRE_PROPERTY_FLOAT(t, @current, 11);

            REQUIRE(t.outputListAt(0) == LX(0, 10));
            NO_OUTPUT_NTICKS(t, 8);
            t.schedTicks(2);
            REQUIRE(t.outputListAt(0) == LAX("end", 20));
        }
    }

    SECTION("infinite")
    {
        setTestSampleRate(64000);

        TExt t("tl.timeline", LA("@mode", "inf"));
        REQUIRE(t.object());
        REQUIRE_PROPERTY(t, @mode, "inf");

        t.call("start");
        REQUIRE(t.outputListAt(0) == LA("begin", 0.f));

        NO_OUTPUT_NTICKS(t, 101);
        REQUIRE_PROPERTY_FLOAT(t, @current, 101);

        t.call("add", LA(200, "ms"));
        NO_OUTPUT_NTICKS(t, 98);
        REQUIRE_PROPERTY_FLOAT(t, @current, 199);
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LX(0, 200));

        NO_OUTPUT_NTICKS(t, 99);
        REQUIRE_PROPERTY_FLOAT(t, @current, 300);
        NO_OUTPUT_NTICKS(t, 10);

        t.call("add", LA(350_ticks, "ms"));
        NO_OUTPUT_NTICKS(t, 39);
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LX(1, 350));
        NO_OUTPUT_NTICKS(t, 9);

        t.call("add", LA(400_ticks, "ms"));
        NO_OUTPUT_NTICKS(t, 10);
        t.call("add", LA(250_ticks, "ms"));
        NO_OUTPUT_NTICKS(t, 29);
        t.call("add", LA(150_ticks, "ms"));
        REQUIRE_PROPERTY_FLOAT(t, @current, 399);
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LX(4, 400));
    }

    SECTION("clear")
    {
        setTestSampleRate(64000);
        TExt t("tl.timeline", LA(0.1, "@loop", 1));

        t.call("add", LA(30, "ms"));
        REQUIRE_PROPERTY_FLOAT(t, @size, 2);

        t.call("start");

        NO_OUTPUT_NTICKS(t, 20);
        // clear before event
        t.call("clear");
        REQUIRE_PROPERTY_FLOAT(t, @size, 1);

        NO_OUTPUT_NTICKS(t, 79);
        REQUIRE_PROPERTY_FLOAT(t, @current, 99);
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LAX("end", 100));

        // next loop iteration
        NO_OUTPUT_NTICKS(t, 49);
        t.call("add", LA(80, "ms"));
        NO_OUTPUT_NTICKS(t, 29);
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LX(0, 80));
        NO_OUTPUT_NTICKS(t, 9);

        // clear after event
        t.call("clear");
        NO_OUTPUT_NTICKS(t, 9);
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LAX("end", 100));

        // empty loop
        NO_OUTPUT_NTICKS(t, 98);
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LAX("end", 100));
    }

    SECTION("add named")
    {
        setTestSampleRate(64000);
        TExt t("tl.timeline", LA(0.1, "@loop", 1));

        t.call("add", LA("ev0", 90, "ms before end"));
        REQUIRE_PROPERTY_LIST(t, @events, LA("ev0", "end"));

        t.call("add", LA("my_event", 40, "ms"));
        REQUIRE_PROPERTY_LIST(t, @events, LA("ev0", "my_event", "end"));

        // INVALID
        t.call("add", LA("ev1", 1, "ms after end"));
        REQUIRE_PROPERTY_LIST(t, @events, LA("ev0", "my_event", "end"));

        t.call("add", LA("ev1", 20, "ms before end"));
        REQUIRE_PROPERTY_LIST(t, @events, LA("ev0", "my_event", "ev1", "end"));
    }

    SECTION("remove")
    {
        setTestSampleRate(64000);
        TExt t("timeline", LA(0.1, "@loop", 1));

        t.call("add", LA(30, "ms"));
        REQUIRE_PROPERTY_FLOAT(t, @size, 2);
        REQUIRE_PROPERTY_LIST(t, @events, LA("event1", "end"));

        t.call("start");
        NO_OUTPUT_NTICKS(t, 29);
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LX(0, 30));

        t.call("remove");
        t.call("remove", 100);
        t.call("remove", -100);

        // remove after passing event
        t.call("remove", 0);
        REQUIRE_PROPERTY_FLOAT(t, @size, 1);
        NO_OUTPUT_NTICKS(t, 68);
        t.schedTicks(3);
        REQUIRE(t.outputListAt(0) == LAX("end", 100));

        t.call("add", LA(30, "ms"));
        t.call("add", LA(90, "ms"));

        t.schedTicks(59);
        // remove before passing event
        t.call("remove", 1);
        NO_OUTPUT_NTICKS(t, 38);
        REQUIRE_PROPERTY_FLOAT(t, @current, 99);
        t.schedTicks(2);
        REQUIRE(t.outputListAt(0) == LAX("end", 100));

        REQUIRE_PROPERTY_LIST(t, @events, LA("event1", "end"));
        t.call("remove", LA("end"));
        REQUIRE_PROPERTY_LIST(t, @events, LA("event1", "end"));
        t.call("remove", 0);
        REQUIRE_PROPERTY_LIST(t, @events, LA("end"));
        t.call("remove", 0);
        REQUIRE_PROPERTY_LIST(t, @events, LA("end"));

        t.call("add", LA("abs1 50ms"));
        t.call("add", LA("event1 10ms after abs1"));
        t.call("add", LA("event2 20ms before end"));

        REQUIRE_PROPERTY_LIST(t, @events, LAX("abs1", "event1", "event2", "end"));

        t.call("remove", LA("abs1"));
        REQUIRE_PROPERTY_LIST(t, @events, LAX("event2", "end"));

        t.call("remove", LA("event2"));
        REQUIRE_PROPERTY_LIST(t, @events, LAX("end"));

        t.call("add", LA("abs0 45ms"));
        t.call("add", LA("abs1 50ms"));
        t.call("add", LA("event1 10ms before abs1"));
        REQUIRE_PROPERTY_LIST(t, @events, LA("event1", "abs0", "abs1", "end"));
        t.call("remove", LA("abs1"));
        REQUIRE_PROPERTY_LIST(t, @events, LA("abs0", "end"));
    }
}
