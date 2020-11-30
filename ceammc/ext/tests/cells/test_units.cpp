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
#include "ceammc_units.h"
#include "lex/units.lexer.h"
#include "test_base.h"

using namespace ceammc;
using namespace ceammc::units;

TEST_CASE("units", "[ceammc::ceammc_units]")
{
    test::pdPrintToStdError();

    SECTION("lexer")
    {
        using namespace ceammc::units;
        UnitsLexer lexer("1000");
        REQUIRE(lexer.parseSingle() >= 0);
        REQUIRE(lexer.values.n == 1);
        REQUIRE(lexer.values.back().unit == UnitsLexer::U_MSEC);
        REQUIRE(lexer.values.back().type == UnitsLexer::T_LONG);
        REQUIRE(lexer.values.back().val.int_val == 1000);

        lexer.in("10ms 20ms");
        REQUIRE(lexer.parseSingle() >= 0);
        REQUIRE(lexer.values.n == 1);
        REQUIRE(lexer.values.back().unit == UnitsLexer::U_MSEC);
        REQUIRE(lexer.values.back().val.dbl_val == 10);

        lexer.in("30:20:10.01");
        REQUIRE(lexer.parseSingle() >= 0);
        REQUIRE(lexer.values.n == 1);
        REQUIRE(lexer.values.back().unit == UnitsLexer::U_SMPTE);
        REQUIRE(lexer.values.back().type == UnitsLexer::T_SMPTE);
        REQUIRE(lexer.values.back().val.smpte_val.hour == 30);
        REQUIRE(lexer.values.back().val.smpte_val.min == 20);
        REQUIRE(lexer.values.back().val.smpte_val.sec == 10);
        REQUIRE(lexer.values.back().val.smpte_val.frame == 1);

        lexer.in("20:10.01");
        REQUIRE(lexer.parseSingle() >= 0);
        REQUIRE(lexer.values.n == 1);
        REQUIRE(lexer.values.back().unit == UnitsLexer::U_SMPTE);
        REQUIRE(lexer.values.back().type == UnitsLexer::T_SMPTE);
        REQUIRE(lexer.values.back().val.smpte_val.hour == 0);
        REQUIRE(lexer.values.back().val.smpte_val.min == 20);
        REQUIRE(lexer.values.back().val.smpte_val.sec == 10);
        REQUIRE(lexer.values.back().val.smpte_val.frame == 1);

        lexer.in("11ms $-20ms");
        REQUIRE(lexer.parseSingle() >= 0);
        REQUIRE(lexer.values.n == 1);
        REQUIRE(lexer.values.back().unit == UnitsLexer::U_MSEC);
        REQUIRE(lexer.values.back().val.dbl_val == 11);

        lexer.in("$-15ms 20ms");
        REQUIRE(lexer.parseSingle() >= 0);
        REQUIRE(lexer.values.n == 1);
        REQUIRE(lexer.values.back().unit == UnitsLexer::U_MSEC);
        REQUIRE(lexer.values.back().val.dbl_val == -15);

        lexer.in("$-16ms $-20ms");
        REQUIRE(lexer.parseSingle() >= 0);
        REQUIRE(lexer.values.n == 1);
        REQUIRE(lexer.values.back().unit == UnitsLexer::U_MSEC);
        REQUIRE(lexer.values.back().val.dbl_val == -16);

        lexer.in("$-16ms invalid");
        REQUIRE(lexer.parseSingle() < 0);

        lexer.in("100 invalid");
        REQUIRE(lexer.parseSingle() < 0);

        lexer.in("100% invalid");
        REQUIRE(lexer.parseSingle() < 0);

        lexer.in("12ms 20ms");
        REQUIRE(lexer.parseMany() == UnitsLexer::STATUS_EOF);
        REQUIRE(lexer.values.n == 2);
        REQUIRE(lexer.values[0].unit == UnitsLexer::U_MSEC);
        REQUIRE(lexer.values[0].val.dbl_val == 12);
        REQUIRE(lexer.values[1].unit == UnitsLexer::U_MSEC);
        REQUIRE(lexer.values[1].val.dbl_val == 20);

        lexer.in("12ms 20ms -1%");
        REQUIRE(lexer.parseMany() == UnitsLexer::STATUS_EOF);
        REQUIRE(lexer.values.n == 3);
        REQUIRE(lexer.values[0].unit == UnitsLexer::U_MSEC);
        REQUIRE(lexer.values[0].val.dbl_val == 12);
        REQUIRE(lexer.values[1].unit == UnitsLexer::U_MSEC);
        REQUIRE(lexer.values[1].val.dbl_val == 20);
        REQUIRE(lexer.values[2].unit == UnitsLexer::U_PERCENT);
        REQUIRE(lexer.values[2].val.dbl_val == -1);

        lexer.in("12ms invalid");
        REQUIRE(lexer.parseMany() == UnitsLexer::STATUS_UNKNOWN_TOKEN);

        lexer.in("12ms invalid 20ms");
        REQUIRE(lexer.parseMany() == UnitsLexer::STATUS_UNKNOWN_TOKEN);

        lexer.in("12ms 1bpm 20ms unknown");
        REQUIRE(lexer.parseMany() == UnitsLexer::STATUS_UNKNOWN_TOKEN);

        lexer.in("12ms $-20ms unknown");
        REQUIRE(lexer.parseMany() == UnitsLexer::STATUS_UNKNOWN_TOKEN);

        lexer.in("$-2ms $-22ms");
        REQUIRE(lexer.parseMany() == UnitsLexer::STATUS_EOF);
        REQUIRE(lexer.values.n == 2);
        REQUIRE(lexer.values[0].unit == UnitsLexer::U_MSEC);
        REQUIRE(lexer.values[0].val.dbl_val == -2);
        REQUIRE(lexer.values[1].unit == UnitsLexer::U_MSEC);
        REQUIRE(lexer.values[1].val.dbl_val == -22);
    }

    SECTION("time")
    {
        UnitParseError err;
        TimeValue v(0);

        REQUIRE(v.toMs() == 0);
        REQUIRE(v.units() == TimeValue::MS);
        REQUIRE(v.value() == 0);

        REQUIRE(TimeValue::parse(AtomList()).matchError(err));

        REQUIRE(TimeValue::parse(LA("100ms")).matchValue(v));
        REQUIRE(v.value() == 100);
        REQUIRE(v.units() == TimeValue::MS);

        REQUIRE(TimeValue::parse(LA("102_ms")).matchValue(v));
        REQUIRE(v.value() == 102);
        REQUIRE(v.units() == TimeValue::MS);

        REQUIRE(TimeValue::parse(LA("100.5msec")).matchValue(v));
        REQUIRE(v.value() == 100.5);
        REQUIRE(v.units() == TimeValue::MS);

        REQUIRE(TimeValue::parse(LA("-100.5_msec")).matchValue(v));
        REQUIRE(v.value() == -100.5);
        REQUIRE(v.units() == TimeValue::MS);

        REQUIRE(TimeValue::parse(LA("100.1s")).matchValue(v));
        REQUIRE(v.value() == Approx(100.1f));
        REQUIRE(v.units() == TimeValue::SEC);

        REQUIRE(TimeValue::parse(LA("10.25_s")).matchValue(v));
        REQUIRE(v.value() == Approx(10.25));
        REQUIRE(v.units() == TimeValue::SEC);

        REQUIRE(TimeValue::parse(LA("-100.5s")).matchValue(v));
        REQUIRE(v.value() == Approx(-100.5));
        REQUIRE(v.units() == TimeValue::SEC);

        REQUIRE(TimeValue::parse(LA("-20_sec")).matchValue(v));
        REQUIRE(v.value() == Approx(-20));
        REQUIRE(v.units() == TimeValue::SEC);

        REQUIRE(TimeValue::parse(LA("30min")).matchValue(v));
        REQUIRE(v.value() == Approx(30));
        REQUIRE(v.units() == TimeValue::MIN);

        REQUIRE(TimeValue::parse(LA("1.25_h")).matchValue(v));
        REQUIRE(v.value() == Approx(1.25));
        REQUIRE(v.units() == TimeValue::HOUR);

        REQUIRE(TimeValue::parse(LA("1.25_d")).matchValue(v));
        REQUIRE(v.value() == Approx(1.25));
        REQUIRE(v.units() == TimeValue::DAY);

        REQUIRE(TimeValue::parse(LA("4800samp")).matchValue(v));
        REQUIRE(v.value() == Approx(4800));
        REQUIRE(v.units() == TimeValue::SAMPLE);
        v.setSamplerate(48000);
        REQUIRE(v.toMs() == Approx(100));

        REQUIRE(TimeValue::parse(LA("960_samp")).matchValue(v));
        REQUIRE(v.value() == Approx(960));
        REQUIRE(v.units() == TimeValue::SAMPLE);
        v.setSamplerate(48000);
        REQUIRE(v.toMs() == Approx(20));

        REQUIRE(TimeValue::parse(LA("240_sa")).matchValue(v));
        REQUIRE(v.value() == Approx(240));
        v.setSamplerate(48000);
        REQUIRE(v.units() == TimeValue::SAMPLE);

        REQUIRE(TimeValue::parse(LA("-240sa")).matchValue(v));
        REQUIRE(v.value() == Approx(-240));
        v.setSamplerate(48000);
        REQUIRE(v.units() == TimeValue::SAMPLE);

        REQUIRE(TimeValue::parse(LA("00:02")).matchValue(v));
        REQUIRE(v.value() == Approx(2000));
        REQUIRE(v.toMs() == 2000);
        REQUIRE(v.units() == TimeValue::SMPTE);
        REQUIRE(TimeValue::parse(LA("00:01.24")).matchValue(v));
        REQUIRE(v.value() == Approx(1024));
        REQUIRE(v.units() == TimeValue::SMPTE);
        REQUIRE(TimeValue::parse(LA("01:00.01")).matchValue(v));
        REQUIRE(v.value() == Approx(60001));
        REQUIRE(v.units() == TimeValue::SMPTE);
        REQUIRE(TimeValue::parse(LA("00:01:00.21")).matchValue(v));
        REQUIRE(v.value() == Approx(60021));
        REQUIRE(v.units() == TimeValue::SMPTE);
        REQUIRE(TimeValue::parse(LA("01:00:00")).matchValue(v));
        REQUIRE(v.value() == Approx(3600000));
        REQUIRE(v.units() == TimeValue::SMPTE);

        // too big value
        REQUIRE(TimeValue::parse(LA("1000000000000000000000000000000000000000")).matchError(err));
        REQUIRE(TimeValue::parse(LA("10 nanosec")).matchError(err));
        REQUIRE(TimeValue::parse(LA(10, "nanosec")).matchError(err));
        REQUIRE(TimeValue::parse(LA("10", "sec")).matchError(err));
        REQUIRE(TimeValue::parse(LA(10, 20, 30)).matchError(err));
        REQUIRE(TimeValue::parse(LA(40)).matchValue(v));
        REQUIRE(v.value() == Approx(40));
    }

    SECTION("end offset")
    {
        TimeValue v(0);
        REQUIRE(TimeValue::parse(LA("$-100")).matchValue(v));
        REQUIRE(v.value() == -100);
        REQUIRE(v.units() == TimeValue::MS);

        REQUIRE(TimeValue::parse(LA("end-120")).matchValue(v));
        REQUIRE(v.value() == -120);
        REQUIRE(v.units() == TimeValue::MS);

        REQUIRE(TimeValue::parse(LA("$-1.5ms")).matchValue(v));
        REQUIRE(v.value() == -1.5);
        REQUIRE(v.units() == TimeValue::MS);

        REQUIRE(TimeValue::parse(LA("$-4samp")).matchValue(v));
        REQUIRE(v.value() == -4);
        REQUIRE(v.units() == TimeValue::SAMPLE);

        REQUIRE(TimeValue::parse(LA("$-5_sec")).matchValue(v));
        REQUIRE(v.value() == -5);
        REQUIRE(v.units() == TimeValue::SEC);

        REQUIRE(TimeValue::parse(LA("$-2.5_min")).matchValue(v));
        REQUIRE(v.value() == -2.5);
        REQUIRE(v.units() == TimeValue::MIN);

        REQUIRE_FALSE(TimeValue::parse(LA("$+1.5ms")).matchValue(v));
        REQUIRE_FALSE(TimeValue::parse(LA("$+200")).matchValue(v));
    }

    SECTION("compare")
    {
        REQUIRE(TimeValue(0) == TimeValue(0));
        REQUIRE(TimeValue(1) == TimeValue(1));
        REQUIRE(TimeValue(-20) == TimeValue(-20));
        REQUIRE(TimeValue(10, TimeValue::SEC) == TimeValue(10000, TimeValue::MS));
        REQUIRE(TimeValue(2, TimeValue::MIN) == TimeValue(120, TimeValue::SEC));
        REQUIRE(TimeValue(3, TimeValue::HOUR) == TimeValue(180, TimeValue::MIN));
        REQUIRE(TimeValue(1, TimeValue::DAY) == TimeValue(24 * 60, TimeValue::MIN));

        REQUIRE(TimeValue(1) != TimeValue(2));
        REQUIRE(TimeValue(2, TimeValue::MIN) != TimeValue(121, TimeValue::SEC));
        REQUIRE(TimeValue(2, TimeValue::MIN) != TimeValue(2, TimeValue::SEC));

        REQUIRE(TimeValue(0) < TimeValue(1));
        REQUIRE(TimeValue(0) <= TimeValue(1));
        REQUIRE(TimeValue(1) <= TimeValue(1));
        REQUIRE(TimeValue(59, TimeValue::SEC) <= TimeValue(1, TimeValue::MIN));

        REQUIRE(TimeValue(1) > TimeValue(0));
        REQUIRE(TimeValue(1) >= TimeValue(0));
        REQUIRE(TimeValue(1) >= TimeValue(1));
    }

    SECTION("fraction")
    {
        UnitParseError err;
        FractionValue v;
        REQUIRE(v.toFraction() == 0);
        REQUIRE(v.units() == FractionValue::PHASE);
        REQUIRE(v.value() == 0);

        REQUIRE(FractionValue::parse(LA("-10.5%")).matchValue(v));
        REQUIRE(v.value() == -10.5);
        REQUIRE(v.units() == FractionValue::PERCENT);

        REQUIRE(FractionValue::parse(LA("-0.25*")).matchValue(v));
        REQUIRE(v.value() == -0.25);
        REQUIRE(v.units() == FractionValue::PHASE);

        REQUIRE(FractionValue::parse(LA("3/4")).matchValue(v));
        REQUIRE(v.value() == 3);
        REQUIRE(v.toFraction() == 0.75);
        REQUIRE(v.units() == FractionValue::RATIO);

        REQUIRE_FALSE(FractionValue::parse(LA("300000000000000000000/4")).matchValue(v));
        REQUIRE_FALSE(FractionValue::parse(LA("300000000000000000000/4000000000000000000000000")).matchValue(v));
        REQUIRE_FALSE(FractionValue::parse(LA("3/4000000000000000000000000")).matchValue(v));
        REQUIRE_FALSE(FractionValue::parse(LA("1/0")).matchValue(v));
    }

    SECTION("freq")
    {
        UnitParseError err;
        FreqValue v;
        REQUIRE_FALSE(v.isFraction());
        REQUIRE_FALSE(v.isTime());
        REQUIRE(v.isFrequency());
        REQUIRE(v.type() == UnitType::FREQ);
        REQUIRE(v.value() == 0);
        REQUIRE(v.units() == FreqValue::HZ);
        REQUIRE(v.toHerz() == 0);

        REQUIRE_FALSE(FreqValue::parse(L()).matchValue(v));
        REQUIRE(FreqValue::parse(LF(200)).matchValue(v));
        REQUIRE(v.value() == 200);
        REQUIRE(v.units() == FreqValue::HZ);

        REQUIRE_FALSE(FreqValue::parse(LF(1, 2)).matchValue(v));
        REQUIRE_FALSE(FreqValue::parse(LA("")).matchValue(v));
        REQUIRE(FreqValue::parse(LA("100.5hz")).matchValue(v));
        REQUIRE(v.value() == 100.5);
        REQUIRE(v.units() == FreqValue::HZ);
        REQUIRE(FreqValue::parse(LA("120bpm")).matchValue(v));
        REQUIRE(v.value() == 120);
        REQUIRE(v.toHerz() == 2);
        REQUIRE(v.units() == FreqValue::BPM);
        REQUIRE(FreqValue::parse(LA("250ms")).matchValue(v));
        REQUIRE(v.value() == 250);
        REQUIRE(v.toHerz() == 4);
        REQUIRE(v.units() == FreqValue::MS);
        REQUIRE_FALSE(FreqValue::parse(LA("0ms")).matchValue(v));
    }
}
