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

TEST_CASE("parser_timeline", "[ceammc::ceammc_units]")
{
    test::pdPrintToStdError();
    TimeLine tl;

    SECTION("duration")
    {
        REQUIRE(parse_timelime("duration *", tl));
        REQUIRE(tl.duration == std::numeric_limits<t_float>::max());
        REQUIRE(parse_timelime("duration 100ms", tl));
        REQUIRE(tl.duration == 100);
        REQUIRE(parse_timelime("duration 100.25ms", tl));
        REQUIRE(tl.duration == 100.25);
        REQUIRE(parse_timelime("duration 100.125msec", tl));
        REQUIRE(tl.duration == 100.125);
        REQUIRE(parse_timelime("duration 100.25s", tl));
        REQUIRE(tl.duration == 100250);
        REQUIRE(parse_timelime("duration 1.5min", tl));
        REQUIRE(tl.duration == 90000);
        REQUIRE(parse_timelime("duration 0.25m", tl));
        REQUIRE(tl.duration == 15000);
        REQUIRE(parse_timelime("duration +0.125m", tl));
        REQUIRE(tl.duration == 7500);
        REQUIRE(parse_timelime("duration 0.5h", tl));
        REQUIRE(tl.duration == 1800000);
        REQUIRE(parse_timelime("duration 00:01", tl));
        REQUIRE(tl.duration == 1000);
        REQUIRE(parse_timelime("duration 01:01", tl));
        REQUIRE(tl.duration == 61000);
        REQUIRE(parse_timelime("duration 01:01:01", tl));
        REQUIRE(tl.duration == 3661000);
    }

    SECTION("bars")
    {
        REQUIRE(parse_timelime("duration *|4/4|", tl));
        REQUIRE(tl.duration == std::numeric_limits<t_float>::max());
        REQUIRE(parse_timelime("duration 1*|4/4|", tl));
        REQUIRE(tl.duration == 4000);
        REQUIRE(parse_timelime("duration 2*|4/4|", tl));
        REQUIRE(tl.duration == 8000);
        REQUIRE(parse_timelime("duration 10*|3/4|", tl));
        REQUIRE(tl.duration == 30000);
        REQUIRE(parse_timelime("duration 10*|3/4| 2*|4/4|", tl));
        REQUIRE(tl.duration == 38000);
    }

    SECTION("var defs")
    {
        REQUIRE(parse_timelime("var name", tl));
        REQUIRE(tl.vars.size() == 1);
        REQUIRE(tl.vars[0].name == SYM("name"));
        REQUIRE(parse_timelime("var name2", tl));
        REQUIRE(tl.vars.size() == 2);
        REQUIRE(tl.vars[1].name == SYM("name2"));

        tl.vars.clear();
        REQUIRE(parse_timelime("var name @default -50", tl));
        REQUIRE(tl.vars.size() == 1);
        REQUIRE(tl.vars[0].name == SYM("name"));
        REQUIRE(tl.vars[0].def == -50);
        REQUIRE(parse_timelime("var name2 @default -150.25", tl));
        REQUIRE(tl.vars.size() == 2);
        REQUIRE(tl.vars[0].name == SYM("name"));
        REQUIRE(tl.vars[0].def == -50);
        REQUIRE(tl.vars[1].name == SYM("name2"));
        REQUIRE(tl.vars[1].def == -150.25);

        tl.vars.clear();
        REQUIRE(parse_timelime("var name @preset 11", tl));
        REQUIRE(tl.vars.size() == 1);
        REQUIRE(tl.vars[0].name == SYM("name"));
        REQUIRE(tl.vars[0].preset == 11);
    }

    SECTION("event defs")
    {
        REQUIRE(parse_timelime("event NAME", tl));
        REQUIRE_FALSE(parse_timelime("event 1NAME", tl));

        REQUIRE(parse_timelime("event NAME !send", tl));
        REQUIRE(parse_timelime("event NAME !out", tl));
        REQUIRE(parse_timelime("event NAME !preset", tl));
        REQUIRE(parse_timelime("event NAME !osc", tl));
        REQUIRE(parse_timelime("event NAME !midi:note", tl));
        REQUIRE(parse_timelime("event NAME !midi:ctl", tl));
        REQUIRE(parse_timelime("event NAME !midi:pgm", tl));
        REQUIRE(parse_timelime("event NAME !midi:sysex", tl));
    }

    SECTION("events")
    {
        REQUIRE(parse_timelime("100ms event EVENT", tl));
        REQUIRE(parse_timelime("+100ms event EVENT", tl));
        REQUIRE(parse_timelime("00:00:25 event EVENT2", tl));
        REQUIRE(parse_timelime("#EVENT2+1ms event EVENT", tl));
        REQUIRE(parse_timelime("#EVENT2-10s event EVENT", tl));
        REQUIRE(parse_timelime("#EVENT2-10s event !send", tl));
        REQUIRE(parse_timelime("#10.1+10s event !send", tl));
        REQUIRE(parse_timelime("#10.2-10.5s event !out", tl));
    }

    SECTION("vars")
    {
        REQUIRE(parse_timelime("100ms var $var line -60 100 1000ms", tl));
        REQUIRE(parse_timelime("100ms var $var lineto 100 1000ms", tl));
        REQUIRE(parse_timelime("#event var $var set -200.5", tl));
    }
}
