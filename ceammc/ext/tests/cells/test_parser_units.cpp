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
#include "lex/parser_units.h"
#include "test_base.h"

using namespace ceammc;

TEST_CASE("parser_units", "[ceammc::ceammc_units]")
{
    test::pdPrintToStdError();

    SECTION("hz")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("120_hz"));
        REQUIRE(p.type() == TYPE_HZ);
        REQUIRE(p.value() == 120);
        REQUIRE(p.position() == POSITION_ABS);

        REQUIRE(p.parse("125hz"));
        REQUIRE(p.type() == TYPE_HZ);
        REQUIRE(p.value() == 125);

        REQUIRE(p.parse("+12.5Hz"));
        REQUIRE(p.type() == TYPE_HZ);
        REQUIRE(p.value() == 12.5);

        REQUIRE(p.parse(A("-100_Hz")));
        REQUIRE(p.type() == TYPE_HZ);
        REQUIRE(p.value() == -100);

        REQUIRE(p.parse(A("+0.25_Hz")));
        REQUIRE(p.type() == TYPE_HZ);
        REQUIRE(p.value() == 0.25);

        REQUIRE(p.parse("0Hz"));
        REQUIRE(p.type() == TYPE_HZ);
        REQUIRE(p.value() == 0);

        REQUIRE(p.parse(A("-10.25_Hz")));
        REQUIRE(p.type() == TYPE_HZ);
        REQUIRE(p.value() == -10.25);

        REQUIRE(!p.parse("10_herz"));
        REQUIRE(!p.parse("0x10_herz"));

        REQUIRE(p.parse(A(100)));
        REQUIRE(p.type() == TYPE_INT);
        REQUIRE(p.value() == 100);

        REQUIRE(p.parseAs(A(100), TYPE_HZ));
        REQUIRE(p.parseAs(A("200_hz"), TYPE_HZ));
        REQUIRE_FALSE(p.parseAs(A("200_rad"), TYPE_HZ));

        REQUIRE(p.parse("+100"));
        REQUIRE(p.value() == 100);
        REQUIRE(p.type() == TYPE_INT);

        REQUIRE(p.parse("-1234.5"));
        REQUIRE(p.value() == -1234.5);
        REQUIRE(p.type() == TYPE_FLOAT);

        UnitVec res;
        REQUIRE(p.parse(LA("+100", "34.5_hz", "-20.5"), res) == 3);
        REQUIRE(res.size() == 3);
        REQUIRE(res[0].type == TYPE_INT);
        REQUIRE(res[0].value == 100);
        REQUIRE(res[1].type == TYPE_HZ);
        REQUIRE(res[1].value == 34.5);
        REQUIRE(res[2].type == TYPE_FLOAT);
        REQUIRE(res[2].value == -20.5);
    }

    SECTION("samples")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("12345_sa"));
        REQUIRE(p.value() == 12345);
        REQUIRE(p.type() == TYPE_SAMP);

        REQUIRE(p.parse("+12345_samp"));
        REQUIRE(p.value() == 12345);
        REQUIRE(p.type() == TYPE_SAMP);

        REQUIRE(p.parse("-1.5samp"));
        REQUIRE(p.value() == -1.5);
        REQUIRE(p.type() == TYPE_SAMP);
    }

    SECTION("rad")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("0rad"));
        REQUIRE(p.value() == 0);
        REQUIRE(p.type() == TYPE_RADIAN);

        REQUIRE(p.parse("+12rad"));
        REQUIRE(p.value() == 12);
        REQUIRE(p.type() == TYPE_RADIAN);

        REQUIRE(p.parse("-12.5_rad"));
        REQUIRE(p.value() == -12.5);
        REQUIRE(p.type() == TYPE_RADIAN);
    }

    SECTION("deg")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("0deg"));
        REQUIRE(p.value() == 0);
        REQUIRE(p.type() == TYPE_DEGREE);

        REQUIRE(p.parse("+12deg"));
        REQUIRE(p.value() == 12);
        REQUIRE(p.type() == TYPE_DEGREE);

        REQUIRE(p.parse("-12.5_deg"));
        REQUIRE(p.value() == -12.5);
        REQUIRE(p.type() == TYPE_DEGREE);
    }

    SECTION("db")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("0db"));
        REQUIRE(p.value() == 0);
        REQUIRE(p.type() == TYPE_DB);

        REQUIRE(p.parse("+12dbfs"));
        REQUIRE(p.value() == 12);
        REQUIRE(p.type() == TYPE_DB);

        REQUIRE(p.parse("-12.5_db"));
        REQUIRE(p.value() == -12.5);
        REQUIRE(p.type() == TYPE_DB);
    }

    SECTION("day")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("0d"));
        REQUIRE(p.value() == 0);
        REQUIRE(p.type() == TYPE_DAY);

        REQUIRE(p.parse("+12_day"));
        REQUIRE(p.value() == 12);
        REQUIRE(p.type() == TYPE_DAY);

        REQUIRE(p.parse("-12.5_d"));
        REQUIRE(p.value() == -12.5);
        REQUIRE(p.type() == TYPE_DAY);
    }

    SECTION("%")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("0%"));
        REQUIRE(p.value() == 0);
        REQUIRE(p.type() == TYPE_PERCENT);

        REQUIRE(p.parse("+12%"));
        REQUIRE(p.value() == 12);
        REQUIRE(p.type() == TYPE_PERCENT);

        REQUIRE(p.parse("-12.5%"));
        REQUIRE(p.value() == -12.5);
        REQUIRE(p.type() == TYPE_PERCENT);
    }

    SECTION("*")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("0*"));
        REQUIRE(p.value() == 0);
        REQUIRE(p.type() == TYPE_PHASE);

        REQUIRE(p.parse("+0.5*"));
        REQUIRE(p.value() == 0.5);
        REQUIRE(p.type() == TYPE_PHASE);

        REQUIRE(p.parse("-0.25*"));
        REQUIRE(p.value() == -0.25);
        REQUIRE(p.type() == TYPE_PHASE);
    }

    SECTION("hour")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("0h"));
        REQUIRE(p.value() == 0);
        REQUIRE(p.type() == TYPE_HOUR);

        REQUIRE(p.parse("+12_hour"));
        REQUIRE(p.value() == 12);
        REQUIRE(p.type() == TYPE_HOUR);

        REQUIRE(p.parse("-12.5_h"));
        REQUIRE(p.value() == -12.5);
        REQUIRE(p.type() == TYPE_HOUR);
    }

    SECTION("min")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("0m"));
        REQUIRE(p.value() == 0);
        REQUIRE(p.type() == TYPE_MIN);

        REQUIRE(p.parse("+12_min"));
        REQUIRE(p.value() == 12);
        REQUIRE(p.type() == TYPE_MIN);

        REQUIRE(p.parse("-12.5_m"));
        REQUIRE(p.value() == -12.5);
        REQUIRE(p.type() == TYPE_MIN);
    }

    SECTION("sec")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("0s"));
        REQUIRE(p.value() == 0);
        REQUIRE(p.type() == TYPE_SEC);

        REQUIRE(p.parse("+12_sec"));
        REQUIRE(p.value() == 12);
        REQUIRE(p.type() == TYPE_SEC);

        REQUIRE(p.parse("-12.5_s"));
        REQUIRE(p.value() == -12.5);
        REQUIRE(p.type() == TYPE_SEC);
    }

    SECTION("msec")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("0ms"));
        REQUIRE(p.value() == 0);
        REQUIRE(p.type() == TYPE_MSEC);

        REQUIRE(p.parse("+12_msec"));
        REQUIRE(p.value() == 12);
        REQUIRE(p.type() == TYPE_MSEC);

        REQUIRE(p.parse("-12.5_ms"));
        REQUIRE(p.value() == -12.5);
        REQUIRE(p.type() == TYPE_MSEC);
    }

    SECTION("cent")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("0c"));
        REQUIRE(p.value() == 0);
        REQUIRE(p.type() == TYPE_CENT);

        REQUIRE(p.parse("+12_cent"));
        REQUIRE(p.value() == 12);
        REQUIRE(p.type() == TYPE_CENT);

        REQUIRE(p.parse("-12.5_c"));
        REQUIRE(p.value() == -12.5);
        REQUIRE(p.type() == TYPE_CENT);
    }

    SECTION("semitone")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("0semi"));
        REQUIRE(p.value() == 0);
        REQUIRE(p.type() == TYPE_SEMITONE);

        REQUIRE(p.parse("+12_semi"));
        REQUIRE(p.value() == 12);
        REQUIRE(p.type() == TYPE_SEMITONE);

        REQUIRE(p.parse("-12.5_semitone"));
        REQUIRE(p.value() == -12.5);
        REQUIRE(p.type() == TYPE_SEMITONE);
    }

    SECTION("smpte")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("11:22:33.00"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.smpte().timeSec(25) == (11 * 3600 + 22 * 60 + 33));

        REQUIRE(p.parse("+11:22:33.04"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.smpte().timeSec(32) == (11 * 3600 + 22 * 60 + 33 + 4 / 32.0));

        REQUIRE(p.parse("-11:22:33.10"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.smpte().timeSec(32) == -(11 * 3600 + 22 * 60 + 33 + 10 / 32.0));

        REQUIRE(p.parse("-09:22:33.55"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.smpte().timeSec(32) == -(9 * 3600 + 22 * 60 + 33 + 1.0));

        REQUIRE(p.parse("00:22:33.1"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.smpte().timeSec(32) == (22 * 60 + 33 + 1 / 32.0));

        REQUIRE(p.parse("00:01.24"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.smpte().frame == 24);
        REQUIRE(p.smpte().sec == 1);
        REQUIRE(p.smpte().timeSec(64) == (1 + 24 / 64.0));

        REQUIRE(p.parse("00:01.24"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.smpte().frame == 24);
        REQUIRE(p.smpte().sec == 1);
        REQUIRE(p.smpte().timeSec(100) == Approx(1.24));

        REQUIRE(p.parse("22:33.1"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.smpte().timeSec(32) == (22 * 60 + 33 + 1 / 32.0));

        REQUIRE(p.parse("+02:33.1"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.smpte().timeSec(32) == (2 * 60 + 33 + 1 / 32.0));

        REQUIRE(p.parse("-02:13.1"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.smpte().timeSec(32) == -(2 * 60 + 13 + 1 / 32.0));

        REQUIRE(p.parse("-02:13"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.smpte().timeSec(32) == -(2 * 60 + 13));

        REQUIRE(p.parse("02:59"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.smpte().timeSec(32) == (2 * 60 + 59));

        REQUIRE(p.parse("$-02:59"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.position() == POSITION_END);
        REQUIRE(p.smpte().timeSec(32) == -(2 * 60 + 59));

        REQUIRE(p.parse("cur+02:59"));
        REQUIRE(p.type() == TYPE_SMPTE);
        REQUIRE(p.position() == POSITION_CURRENT);
        REQUIRE(p.smpte().timeSec(32) == (2 * 60 + 59));

        REQUIRE(!p.parse("2:59"));
        REQUIRE(!p.parse("1:02:59"));
        REQUIRE(!p.parse("02:60"));
    }

    SECTION("bpm")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        REQUIRE(p.parse("120bpm"));
        REQUIRE(p.bpm().bpm == 120);
        REQUIRE(p.bpm().beatlen == 0.25);
        REQUIRE(p.type() == TYPE_BPM);

        REQUIRE(p.parse("60-8_bpm"));
        REQUIRE(p.bpm().bpm == 60);
        REQUIRE(p.bpm().beatlen == 0.125);
        REQUIRE(p.type() == TYPE_BPM);

        REQUIRE(p.parse("144-4._bpm"));
        REQUIRE(p.bpm().bpm == 144);
        REQUIRE(p.bpm().beatlen == 0.375);
        REQUIRE(p.type() == TYPE_BPM);

        REQUIRE(p.parse("12-1/8bpm"));
        REQUIRE(p.bpm().bpm == 12);
        REQUIRE(p.bpm().beatlen == 0.125);
        REQUIRE(p.type() == TYPE_BPM);

        REQUIRE(p.parse("4.5-3/4_bpm"));
        REQUIRE(p.bpm().bpm == 4.5);
        REQUIRE(p.bpm().beatlen == 0.75);
        REQUIRE(p.type() == TYPE_BPM);

        REQUIRE(!p.parse("+12_bpm"));
        REQUIRE(!p.parse("-12_bpm"));
        REQUIRE(!p.parse("$-12_bpm"));
    }

    SECTION("many")
    {
        using namespace ceammc::parser;
        UnitsFullMatch p;

        auto res7 = p.parseN<7>(LA("120hz", "00:01", "$-100_samp", "+50.5ms", 100, 200.25, "???"), Unit(0, TYPE_BPM));

        REQUIRE(res7[0].type == TYPE_HZ);
        REQUIRE(res7[0].value == 120);

        REQUIRE(res7[1].type == TYPE_SMPTE);
        REQUIRE(res7[1].smpte.timeSec() == 1);

        REQUIRE(res7[2].type == TYPE_SAMP);
        REQUIRE(res7[2].value == -100);
        REQUIRE(res7[2].pos == POSITION_END);

        REQUIRE(res7[3].type == TYPE_MSEC);
        REQUIRE(res7[3].value == 50.5);
        REQUIRE(res7[3].pos == POSITION_ABS);

        REQUIRE(res7[4].type == TYPE_INT);
        REQUIRE(res7[4].value == 100);

        REQUIRE(res7[5].type == TYPE_FLOAT);
        REQUIRE(res7[5].value == 200.25);

        REQUIRE(res7[6].type == TYPE_BPM);
        REQUIRE(res7[6].value == 0);
    }
}
