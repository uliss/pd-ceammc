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
#include "lex/parser_timeline.h"
#include "test_common.h"

using namespace ceammc;
using namespace ceammc::parser;

#define REQUIRE_SYM(sym, val)                     \
    {                                             \
        REQUIRE(sym);                             \
        REQUIRE(sym->s_name == std::string(val)); \
    }

TEST_CASE("parser_timeline", "[ceammc::ceammc_units]")
{
    test::pdPrintToStdError();
    TimeLine tl;

    SECTION("tl")
    {
        REQUIRE(parse_timelime("tl *", tl));
        REQUIRE(tl.duration == std::numeric_limits<t_float>::max());
        REQUIRE(parse_timelime("tl 100ms", tl));
        REQUIRE(tl.duration == 100);
        REQUIRE(parse_timelime("tl 100.25ms", tl));
        REQUIRE(tl.duration == 100.25);
        REQUIRE(parse_timelime("tl 100.125msec", tl));
        REQUIRE(tl.duration == 100.125);
        REQUIRE(parse_timelime("tl 100.25s", tl));
        REQUIRE(tl.duration == 100250);
        REQUIRE(parse_timelime("tl 1.5min", tl));
        REQUIRE(tl.duration == 90000);
        REQUIRE(parse_timelime("tl 0.25m", tl));
        REQUIRE(tl.duration == 15000);
        REQUIRE_FALSE(parse_timelime("tl +0.125m", tl));
        REQUIRE_FALSE(parse_timelime("tl -0.125m", tl));
        REQUIRE(parse_timelime("tl 0.125m", tl));
        REQUIRE(tl.duration == 7500);
        REQUIRE(parse_timelime("tl 0.5h", tl));
        REQUIRE(tl.duration == 1800000);
        REQUIRE(parse_timelime("tl 00:01", tl));
        REQUIRE(tl.duration == 1000);
        REQUIRE(parse_timelime("tl 01:01", tl));
        REQUIRE(tl.duration == 61000);
        REQUIRE(parse_timelime("tl 01:01:01", tl));
        REQUIRE(tl.duration == 3661000);
    }

    SECTION("bars")
    {
        REQUIRE(parse_timelime("tl *|4/4|", tl));
        REQUIRE(tl.duration == std::numeric_limits<t_float>::max());
        REQUIRE(parse_timelime("tl 1*|4/4|", tl));
        REQUIRE(tl.duration == 4000);
        REQUIRE(parse_timelime("tl 2*|4/4|", tl));
        REQUIRE(tl.duration == 8000);
        REQUIRE(parse_timelime("tl 10*|3/4|", tl));
        REQUIRE(tl.duration == 30000);
        REQUIRE(parse_timelime("tl 10*|3/4| 2*|4/4|", tl));
        REQUIRE(tl.duration == 38000);
    }

    SECTION("pure syntax")
    {
        SECTION("event defs")
        {
            REQUIRE(parse_timelime("event NAME", tl));
            REQUIRE_FALSE(parse_timelime("event 1NAME", tl));

            REQUIRE(parse_timelime("event NAME !send TARGET", tl));
            REQUIRE(parse_timelime("event NAME !out", tl));
            REQUIRE(parse_timelime("event NAME !preset 2", tl));
            REQUIRE(parse_timelime("event NAME !osc", tl));
            REQUIRE(parse_timelime("event NAME !midi:note 60 127", tl));
            REQUIRE(parse_timelime("event NAME !midi:note 60 127 1", tl));
            REQUIRE(parse_timelime("event NAME !midi:ctl 30 62", tl));
            REQUIRE(parse_timelime("event NAME !midi:ctl 30 62 3", tl));
            REQUIRE(parse_timelime("event NAME !midi:pgm 1", tl));
            REQUIRE(parse_timelime("event NAME !midi:pgm 5 3", tl));
            REQUIRE(parse_timelime("event NAME !midi:sysex", tl));
        }

        SECTION("events")
        {
            REQUIRE(parse_timelime("100ms event EVENT", tl));
            REQUIRE(parse_timelime("+100ms event EVENT", tl));
            REQUIRE(parse_timelime("00:00:25 event EVENT2", tl));
            REQUIRE(parse_timelime("#EVENT2+1ms event EVENT", tl));
            REQUIRE(parse_timelime("#EVENT2-10s event EVENT", tl));
            REQUIRE(parse_timelime("#EVENT2-10s event !send TARGET", tl));
            REQUIRE(parse_timelime("#10.1+10s event !send TARGET", tl));
            REQUIRE(parse_timelime("#10.2-10.5s event !out", tl));
            REQUIRE(parse_timelime("#10.2.25-10.5s event !out", tl));
            REQUIRE(parse_timelime("event NAME !send T100", tl));
            REQUIRE(parse_timelime("event NAME !send 100T", tl));
            REQUIRE_FALSE(parse_timelime("event NAME !send 100", tl));
        }

        SECTION("vars")
        {
            REQUIRE(parse_timelime("100ms var  $var !line -60 100 1000ms", tl));
            REQUIRE(parse_timelime("100ms var  $var !lineto 100 1000ms", tl));
            REQUIRE(parse_timelime("#event var $var !set -200.5", tl));
        }

        SECTION("comments")
        {
            REQUIRE(parse_timelime("//", tl));
            REQUIRE(parse_timelime(" //", tl));
            REQUIRE(parse_timelime("// ", tl));
            REQUIRE(parse_timelime("  //", tl));
        }
    }

    SECTION("data")
    {
        SECTION("bar bpm")
        {
            REQUIRE(parse_timelime("tl 4*|4/4|", tl));
            REQUIRE(parse_timelime("bpm #0 120bpm", tl));
            tl.calcBarDuration(false);
            REQUIRE(tl.duration == 8000);

            REQUIRE(parse_timelime("bpm #2 60bpm", tl));

            REQUIRE(tl.bars.size() == 1);
            REQUIRE(tl.tempo.size() == 2);
            REQUIRE(tl.calcNumBars() == 4);
            REQUIRE(tl.barTempo(0) == music::Tempo(120));
            REQUIRE(tl.barTempo(1) == music::Tempo(120));
            REQUIRE(tl.barTempo(2) == music::Tempo(60));
            REQUIRE(tl.barTempo(3) == music::Tempo(60));
            REQUIRE(tl.barTempo(4) == music::Tempo(60));
            REQUIRE(tl.barDuration(0, 1) == music::Duration(4, 4));
            REQUIRE(tl.barDuration(0, 2) == music::Duration(8, 4));
            REQUIRE(tl.barDuration(0, 3) == music::Duration(12, 4));
            REQUIRE(tl.barDuration(0, 4) == music::Duration(16, 4));
            REQUIRE(tl.barDuration(1, 1) == music::Duration(4, 4));
            REQUIRE(tl.barDuration(1, 2) == music::Duration(8, 4));
            REQUIRE(tl.barDuration(1, 3) == music::Duration(12, 4));
            REQUIRE(tl.barDuration(3, 1) == music::Duration(4, 4));
            REQUIRE(tl.barDuration(3, 2) == music::Duration(4, 4));
            REQUIRE(tl.barDuration(4, 0) == music::Duration(0, 4));
            REQUIRE(tl.tempoSegmentDuration(0) == music::Duration(8, 4));
            REQUIRE(tl.tempoSegmentDuration(1) == music::Duration(8, 4));

            tl.calcBarDuration(false);
            REQUIRE(tl.duration == 12000);
        }

        SECTION("bar bpm")
        {
            REQUIRE(parse_timelime("tl 2*|2/4| 3*|3/4| 4*|4/4|", tl));
            REQUIRE(parse_timelime("bpm #0 60bpm", tl));
            REQUIRE(tl.duration == 29000);
            REQUIRE(parse_timelime("bpm #2 120bpm", tl));
            REQUIRE(tl.duration == 16500);
            REQUIRE(parse_timelime("bpm #5 240bpm", tl));
            REQUIRE(tl.duration == 12500.0);

            REQUIRE(tl.bars.size() == 3);
            REQUIRE(tl.tempo.size() == 3);
            REQUIRE(tl.calcNumBars() == 9);
            REQUIRE(tl.barTempo(0) == music::Tempo(60));
            REQUIRE(tl.barTempo(1) == music::Tempo(60));
            REQUIRE(tl.barTempo(2) == music::Tempo(120));
            REQUIRE(tl.barTempo(3) == music::Tempo(120));
            REQUIRE(tl.barTempo(4) == music::Tempo(120));
            REQUIRE(tl.barTempo(5) == music::Tempo(240));
            REQUIRE(tl.barTempo(6) == music::Tempo(240));
            REQUIRE(tl.barTempo(7) == music::Tempo(240));
            REQUIRE(tl.barTempo(8) == music::Tempo(240));
            REQUIRE(tl.barDuration(0, 1) == music::Duration(2, 4));
            REQUIRE(tl.barDuration(0, 2) == music::Duration(4, 4));
            REQUIRE(tl.barDuration(0, 3) == music::Duration(7, 4));
            REQUIRE(tl.barDuration(0, 4) == music::Duration(10, 4));
            REQUIRE(tl.barDuration(0, 5) == music::Duration(13, 4));
            REQUIRE(tl.barDuration(0, 6) == music::Duration(17, 4));
            REQUIRE(tl.barDuration(0, 7) == music::Duration(21, 4));
            REQUIRE(tl.barDuration(0, 8) == music::Duration(25, 4));

            REQUIRE(tl.barDuration(1, 1) == music::Duration(2, 4));
            REQUIRE(tl.barDuration(1, 2) == music::Duration(5, 4));
            REQUIRE(tl.barDuration(1, 3) == music::Duration(8, 4));
            REQUIRE(tl.barDuration(1, 4) == music::Duration(11, 4));
            REQUIRE(tl.barDuration(1, 5) == music::Duration(15, 4));
            REQUIRE(tl.barDuration(1, 6) == music::Duration(19, 4));
            REQUIRE(tl.barDuration(1, 7) == music::Duration(23, 4));

            REQUIRE(tl.tempoSegmentDuration(0) == music::Duration(4, 4));
            REQUIRE(tl.tempoSegmentDuration(1) == music::Duration(9, 4));
            REQUIRE(tl.tempoSegmentDuration(2) == music::Duration(16, 4));
        }

        SECTION("accel")
        {
            REQUIRE(parse_timelime("tl 4*|4/4|", tl));
            tl.calcBarDuration(false);
            REQUIRE(tl.duration == 16000.0);
            tl.setBarBpm(0, 0, { 60 }, true);
            tl.setBarBpm(3, 0, { 120 }, false);

            REQUIRE(tl.tempo.size() == 2);
            REQUIRE(tl.calcNumBars() == 4);
            REQUIRE(tl.barTempo(0) == music::Tempo(60));
            REQUIRE(tl.barTempo(1) == music::Tempo(80));
            REQUIRE(tl.barTempo(2) == music::Tempo(100));
            REQUIRE(tl.barTempo(3) == music::Tempo(120));

            tl.calcBarDuration(false);
            REQUIRE(tl.duration == 11400.0);
        }

        SECTION("event defs")
        {
            REQUIRE(parse_timelime("event NAME", tl));
            REQUIRE(tl.event_defs.size() == 1);
            REQUIRE_SYM(tl.event_defs[0].name, "NAME");

            REQUIRE(parse_timelime("event NAME !send TARGET 1 $var @3", tl));
            REQUIRE(tl.event_defs.size() == 1);
            REQUIRE_SYM(tl.event_defs[0].name, "NAME");
            REQUIRE_SYM(tl.event_defs[0].send.target, "TARGET");
            REQUIRE(tl.event_defs[0].send.args == LA(1, "$var", "@3"));

            REQUIRE(parse_timelime("event NAME !out message $var @3", tl));
            REQUIRE(tl.event_defs.size() == 1);
            REQUIRE_SYM(tl.event_defs[0].name, "NAME");
            REQUIRE(tl.event_defs[0].out.args == LA("message", "$var", "@3"));

            REQUIRE(parse_timelime("event NAME !preset 3", tl));
            REQUIRE(tl.event_defs.size() == 1);
            REQUIRE_SYM(tl.event_defs[0].name, "NAME");
            REQUIRE(tl.event_defs[0].preset.idx == 3);
        }

        SECTION("var defs")
        {
            REQUIRE(parse_timelime("var name", tl));
            REQUIRE(tl.var_defs.size() == 1);
            REQUIRE(tl.var_defs[0].name == SYM("name"));
            REQUIRE(parse_timelime("var name2", tl));
            REQUIRE(tl.var_defs.size() == 2);
            REQUIRE(tl.var_defs[1].name == SYM("name2"));

            tl.var_defs.clear();
            REQUIRE(parse_timelime("var name @default -50", tl));
            REQUIRE(tl.var_defs.size() == 1);
            REQUIRE(tl.var_defs[0].name == SYM("name"));
            REQUIRE(tl.var_defs[0].def == -50);
            REQUIRE(parse_timelime("var name2 @default -150.25", tl));
            REQUIRE(tl.var_defs.size() == 2);
            REQUIRE(tl.var_defs[0].name == SYM("name"));
            REQUIRE(tl.var_defs[0].def == -50);
            REQUIRE(tl.var_defs[1].name == SYM("name2"));
            REQUIRE(tl.var_defs[1].def == -150.25);

            tl.var_defs.clear();
            REQUIRE(parse_timelime("var name @preset 11", tl));
            REQUIRE(tl.var_defs.size() == 1);
            REQUIRE(tl.var_defs[0].name == SYM("name"));
            REQUIRE(tl.var_defs[0].preset == 11);
        }

        SECTION("events")
        {
            REQUIRE(parse_timelime("100ms event NAME", tl));
            REQUIRE(tl.events.size() == 0);

            REQUIRE(parse_timelime("event NAME", tl));
            REQUIRE(tl.event_defs.size() == 1);
            REQUIRE_SYM(tl.event_defs[0].name, "NAME");

            REQUIRE(parse_timelime("100.5ms event NAME", tl));
            REQUIRE(tl.events.size() == 1);
            REQUIRE(tl.events.begin()->time == 100.5);
            REQUIRE(tl.events.begin()->idx == 0);

            REQUIRE(parse_timelime("1sec event NAME", tl));
            REQUIRE(tl.events.size() == 2);
            REQUIRE((++tl.events.begin())->time == 1000);
            REQUIRE((++tl.events.begin())->idx == 0);

            REQUIRE(parse_timelime("event NAME2", tl));

            REQUIRE(parse_timelime("0.5sec event NAME2", tl));
            REQUIRE(tl.events.size() == 3);
            REQUIRE(tl.eventByIdx(0).time == 100.5);
            REQUIRE(tl.eventByIdx(0).idx == 0);
            REQUIRE(tl.eventByIdx(1).time == 500);
            REQUIRE(tl.eventByIdx(1).idx == 1);
            REQUIRE(tl.eventByIdx(2).time == 1000);
            REQUIRE(tl.eventByIdx(2).idx == 0);
            REQUIRE(parse_timelime("#NAME2+100ms event NAME", tl));
            REQUIRE(tl.events.size() == 4);
            REQUIRE(tl.eventByIdx(2).time == 600);
            REQUIRE(tl.eventByIdx(2).idx == 0);

            REQUIRE(parse_timelime("#NAME2+200ms event NAME", tl));
            REQUIRE(tl.events.size() == 5);
            REQUIRE(tl.eventByIdx(3).time == 700);
            REQUIRE(tl.eventByIdx(3).idx == 0);

            REQUIRE(parse_timelime("#NAME2-50ms event NAME", tl));
            REQUIRE(tl.events.size() == 6);
            REQUIRE(tl.eventByIdx(1).time == 450);
            REQUIRE(tl.eventByIdx(1).idx == 0);

            REQUIRE(parse_timelime("#10.1 event NAME", tl));
            REQUIRE(tl.events.size() == 6);
            REQUIRE(tl.eventByIdx(1).time == 450);
            REQUIRE(tl.eventByIdx(1).idx == 0);

            REQUIRE(parse_timelime("#NAME+1ms event NAME", tl));
            REQUIRE(tl.events.size() == 11);
            REQUIRE(tl.eventByIdx(0).time == 100.5);
            REQUIRE(tl.eventByIdx(0).idx == 0);
            REQUIRE(tl.eventByIdx(1).time == 101.5);
            REQUIRE(tl.eventByIdx(1).idx == 0);
            REQUIRE(tl.eventByIdx(2).time == 450);
            REQUIRE(tl.eventByIdx(2).idx == 0);
            REQUIRE(tl.eventByIdx(3).time == 451);
            REQUIRE(tl.eventByIdx(3).idx == 0);
            REQUIRE(tl.eventByIdx(4).time == 500);
            REQUIRE(tl.eventByIdx(4).idx == 1);
            REQUIRE(tl.eventByIdx(5).time == 600);
            REQUIRE(tl.eventByIdx(5).idx == 0);
            REQUIRE(tl.eventByIdx(6).time == 601);
            REQUIRE(tl.eventByIdx(6).idx == 0);
            REQUIRE(tl.eventByIdx(7).time == 700);
            REQUIRE(tl.eventByIdx(7).idx == 0);
            REQUIRE(tl.eventByIdx(8).time == 701);
            REQUIRE(tl.eventByIdx(8).idx == 0);
            REQUIRE(tl.eventByIdx(9).time == 1000);
            REQUIRE(tl.eventByIdx(9).idx == 0);
            REQUIRE(tl.eventByIdx(10).time == 1001);
            REQUIRE(tl.eventByIdx(10).idx == 0);
        }

        SECTION("anonymous events")
        {
            REQUIRE(parse_timelime("100ms event !send TARGET value", tl));
            REQUIRE(tl.events.size() == 1);
            REQUIRE(tl.event_defs.size() == 1);
            REQUIRE_SYM(tl.event_defs[0].send.target, "TARGET");
            REQUIRE(tl.event_defs[0].send.args == LA("value"));

            REQUIRE(parse_timelime("100ms event !out 1 2 3", tl));
            REQUIRE(tl.event_defs.size() == 2);
            REQUIRE(tl.event_defs[1].out.args == LF(1, 2, 3));

            REQUIRE(parse_timelime("100ms event !preset 4", tl));
            REQUIRE(tl.event_defs.size() == 3);
            REQUIRE(tl.event_defs[2].preset.idx == 4);

            REQUIRE(parse_timelime("+10ms event !preset 4", tl));
            tl.dump();
            REQUIRE(tl.event_defs.size() == 4);
            REQUIRE(tl.eventByIdx(3).time == 110);
        }

        SECTION("bars events")
        {
            REQUIRE(parse_timelime("event BAR21", tl));
            REQUIRE(parse_timelime("tl 4*|4/4| 3*|3/8|", tl));
            REQUIRE(tl.duration == 20500);
            REQUIRE(tl.findBarTime(0, 0) == 0);
            REQUIRE(tl.findBarTime(1, 0) == 4000);
            REQUIRE(tl.findBarTime(1, 0, 0.5) == 4500);
            REQUIRE(tl.findBarTime(1, 1) == 5000);
            REQUIRE(tl.findBarTime(1, 2) == 6000);
            REQUIRE(tl.findBarTime(1, 2, 0.25) == 6250);
            REQUIRE(tl.findBarTime(1, 2, 0.5) == 6500);
            REQUIRE(tl.findBarTime(1, 3) == 7000);
            REQUIRE(tl.findBarTime(1, 3, 0.5) == 7000);
            REQUIRE(tl.findBarTime(1, 4) == 7000);
            REQUIRE(tl.findBarTime(4, 0) == 16000);
            REQUIRE(tl.findBarTime(4, 1) == 16500);

            REQUIRE(parse_timelime("#0 event BAR21", tl));
            REQUIRE(tl.events.size() == 1);
            REQUIRE(tl.eventByIdx(0).time == 0);
            REQUIRE(tl.eventByIdx(0).idx == 0);

            REQUIRE(parse_timelime("#0.2 event BAR21", tl));
            REQUIRE(tl.events.size() == 2);
            REQUIRE(tl.eventByIdx(1).time == 2000);
            REQUIRE(tl.eventByIdx(1).idx == 0);

            REQUIRE(parse_timelime("#1.2.5 event BAR21", tl));
            REQUIRE(tl.events.size() == 3);
            REQUIRE(tl.eventByIdx(2).time == 6500);
            REQUIRE(tl.eventByIdx(2).idx == 0);

            REQUIRE(parse_timelime("#4.1 event BAR21", tl));
            REQUIRE(tl.events.size() == 4);
            REQUIRE(tl.eventByIdx(3).time == 16500);
            REQUIRE(tl.eventByIdx(3).idx == 0);

            REQUIRE(parse_timelime("#5.0 event BAR21", tl));
            REQUIRE(tl.events.size() == 5);
            REQUIRE(tl.eventByIdx(4).time == 17500);
            REQUIRE(tl.eventByIdx(4).idx == 0);

            REQUIRE(parse_timelime("#5.0+50ms event BAR21", tl));
            REQUIRE(tl.events.size() == 6);
            REQUIRE(tl.eventByIdx(5).time == 17550);
            REQUIRE(tl.eventByIdx(5).idx == 0);

            REQUIRE(parse_timelime("#5.0-50ms event BAR21", tl));
            REQUIRE(tl.events.size() == 7);
            REQUIRE(tl.eventByIdx(4).time == 17450);
            REQUIRE(tl.eventByIdx(4).idx == 0);
        }

        SECTION("bars events bpm")
        {
            REQUIRE(parse_timelime("tl 4*|4/4|", tl));
            REQUIRE(tl.duration == 16000);
            REQUIRE(parse_timelime("bpm #0 120bpm", tl));
            REQUIRE(tl.duration == 8000);
            REQUIRE(parse_timelime("bpm #2 240bpm", tl));
            REQUIRE(tl.duration == 6000);
            REQUIRE(parse_timelime("bpm #3 60bpm", tl));
            REQUIRE(parse_timelime("event B", tl));
            REQUIRE(tl.duration == 9000);

            REQUIRE(tl.findBarTime(0, 0) == 0);
            REQUIRE(tl.findBarTime(0, 1) == 500);
            REQUIRE(tl.findBarTime(0, 2) == 1000);
            REQUIRE(tl.findBarTime(0, 3) == 1500);
            REQUIRE(tl.findBarTime(1, 0) == 2000);
            REQUIRE(tl.findBarTime(1, 1) == 2500);
            REQUIRE(tl.findBarTime(1, 2) == 3000);
            REQUIRE(tl.findBarTime(1, 3) == 3500);
            REQUIRE(tl.findBarTime(1, 4) == 3500);
            REQUIRE(tl.findBarTime(2, 0) == 4000);
            REQUIRE(tl.findBarTime(2, 1) == 4250);
            REQUIRE(tl.findBarTime(2, 2) == 4500);
            REQUIRE(tl.findBarTime(2, 3) == 4750);
            REQUIRE(tl.findBarTime(3, 0) == 5000);
            REQUIRE(tl.findBarTime(3, 1) == 6000);
            REQUIRE(tl.findBarTime(3, 2) == 7000);
            REQUIRE(tl.findBarTime(3, 3) == 8000);
            REQUIRE(tl.findBarTime(4, 0) == -1);

            REQUIRE(parse_timelime("#3.1 event B", tl));
            REQUIRE(tl.events.size() == 1);
            REQUIRE(tl.eventByIdx(0).time == 6000);
            REQUIRE(tl.eventByIdx(0).idx == 0);
        }
    }
}
