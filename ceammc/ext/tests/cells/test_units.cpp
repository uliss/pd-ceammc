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
#include "test_base.h"

using namespace ceammc;
using namespace ceammc::units;

TEST_CASE("units", "[ceammc::ceammc_units]")
{
    SECTION("time")
    {
        UnitParseError err;
        TimeValue v(0);
        REQUIRE(v.toMs(48000) == 0);
        REQUIRE(v.unit == TimeUnits::MS);
        REQUIRE(v.value == 0);

        REQUIRE(TimeValue::parse(AtomList()).matchError(err));
        REQUIRE(TimeValue::parse(LA(100, "ms")).matchValue(v));
        REQUIRE(v.value == 100);
        REQUIRE(v.unit == TimeUnits::MS);

        REQUIRE(TimeValue::parse(LA(200, "msec")).matchValue(v));
        REQUIRE(v.value == 200);
        REQUIRE(v.unit == TimeUnits::MS);

        REQUIRE(TimeValue::parse(LA(15, "sec")).matchValue(v));
        REQUIRE(v.value == 15);
        REQUIRE(v.unit == TimeUnits::SEC);

        REQUIRE(TimeValue::parse(LA(20, "min")).matchValue(v));
        REQUIRE(v.value == 20);
        REQUIRE(v.unit == TimeUnits::MIN);

        REQUIRE(TimeValue::parse(LA(200, "hour")).matchValue(v));
        REQUIRE(v.value == 200);
        REQUIRE(v.unit == TimeUnits::HOUR);

        REQUIRE(TimeValue::parse(LA(1.5, "day")).matchValue(v));
        REQUIRE(v.value == 1.5f);
        REQUIRE(v.unit == TimeUnits::DAY);

        REQUIRE(TimeValue::parse(LA("100ms")).matchValue(v));
        REQUIRE(v.value == 100);
        REQUIRE(v.unit == TimeUnits::MS);

        REQUIRE(TimeValue::parse(LA("100.5msec")).matchValue(v));
        REQUIRE(v.value == 100.5);
        REQUIRE(v.unit == TimeUnits::MS);

        REQUIRE(TimeValue::parse(LA("100.1s")).matchValue(v));
        REQUIRE(v.value == Approx(100.1f));
        REQUIRE(v.unit == TimeUnits::SEC);

        REQUIRE(TimeValue::parse(LA("-100.5s")).matchValue(v));
        REQUIRE(v.value == Approx(-100.5));
        REQUIRE(v.unit == TimeUnits::SEC);

        REQUIRE(TimeValue::parse(LA("30min")).matchValue(v));
        REQUIRE(v.value == Approx(30));
        REQUIRE(v.unit == TimeUnits::MIN);

        REQUIRE(TimeValue::parse(LA("10")).matchValue(v));
        REQUIRE(v.value == Approx(10));
        REQUIRE(v.unit == TimeUnits::MS);

        REQUIRE(TimeValue::parse(LA(4800, "samp")).matchValue(v));
        REQUIRE(v.value == Approx(4800));
        REQUIRE(v.unit == TimeUnits::SAMPLE);
        REQUIRE(v.toMs(48000) == Approx(100));

        REQUIRE(TimeValue::parse(LA(960, "sample")).matchValue(v));
        REQUIRE(v.value == Approx(960));
        REQUIRE(v.unit == TimeUnits::SAMPLE);
        REQUIRE(v.toMs(48000) == Approx(20));

        REQUIRE(TimeValue::parse(LA("240samp")).matchValue(v));
        REQUIRE(v.value == Approx(240));
        REQUIRE(v.unit == TimeUnits::SAMPLE);

        REQUIRE(TimeValue::parse(LA("-240sample")).matchValue(v));
        REQUIRE(v.value == Approx(-240));
        REQUIRE(v.unit == TimeUnits::SAMPLE);

        // too big value
        REQUIRE(TimeValue::parse(LA("1000000000000000000000000000000000000000")).matchError(err));
        REQUIRE(TimeValue::parse(LA("10 nanosec")).matchError(err));
        REQUIRE(TimeValue::parse(LA(10, "nanosec")).matchError(err));
        REQUIRE(TimeValue::parse(LA("10", "sec")).matchError(err));
        REQUIRE(TimeValue::parse(LA(10, 20, 30)).matchError(err));
        REQUIRE(TimeValue::parse(LA(40)).matchValue(v));
        REQUIRE(v.value == Approx(40));
    }

    SECTION("compare")
    {
        REQUIRE(TimeValue(0) == TimeValue(0));
        REQUIRE(TimeValue(1) == TimeValue(1));
        REQUIRE(TimeValue(-20) == TimeValue(-20));
        REQUIRE(TimeValue(10, TimeUnits::SEC) == TimeValue(10000, TimeUnits::MS));
        REQUIRE(TimeValue(2, TimeUnits::MIN) == TimeValue(120, TimeUnits::SEC));
        REQUIRE(TimeValue(3, TimeUnits::HOUR) == TimeValue(180, TimeUnits::MIN));
        REQUIRE(TimeValue(1, TimeUnits::DAY) == TimeValue(24 * 60, TimeUnits::MIN));

        REQUIRE(TimeValue(1) != TimeValue(2));
        REQUIRE(TimeValue(2, TimeUnits::MIN) != TimeValue(121, TimeUnits::SEC));
        REQUIRE(TimeValue(2, TimeUnits::MIN) != TimeValue(2, TimeUnits::SEC));

        REQUIRE(TimeValue(0) < TimeValue(1));
        REQUIRE(TimeValue(0) <= TimeValue(1));
        REQUIRE(TimeValue(1) <= TimeValue(1));
        REQUIRE(TimeValue(59, TimeUnits::SEC) <= TimeValue(1, TimeUnits::MIN));

        REQUIRE(TimeValue(1) > TimeValue(0));
        REQUIRE(TimeValue(1) >= TimeValue(0));
        REQUIRE(TimeValue(1) >= TimeValue(1));
    }

    SECTION("fraction")
    {
        UnitParseError err;
        FractionValue v(0);
        REQUIRE(v.toFraction() == 0);
        REQUIRE(v.unit == FracUnits::FRACTION);
        REQUIRE(v.value == 0);

        REQUIRE(FractionValue::parse(LA("-10.5%")).matchValue(v));
        REQUIRE(v.value == -10.5);
        REQUIRE(v.unit == FracUnits::PERCENT);

        REQUIRE(FractionValue::parse(LA("-0.25*")).matchValue(v));
        REQUIRE(v.value == -0.25);
        REQUIRE(v.unit == FracUnits::FRACTION);

        REQUIRE(FractionValue::parse(LA("3/4")).matchValue(v));
        REQUIRE(v.value == 3);
        REQUIRE(v.toFraction() == 0.75);
        REQUIRE(v.unit == FracUnits::RATIO);
    }
}
