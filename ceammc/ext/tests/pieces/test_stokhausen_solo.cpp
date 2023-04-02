/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "stok_solo_data.h"
#include "test_common.h"
#include "test_pieces_base.h"

// PD_COMPLETE_TEST_SETUP(PathExists, path, exists)

using namespace solo;

TEST_CASE("pieces.stok_solo~", "[externals]")
{
    test::pdPrintToStdError();

    SECTION("init")
    {
        Scheme s;
        REQUIRE(s.schemeLength() == 0);
        REQUIRE_FALSE(s.setScheme(-1));
        REQUIRE_FALSE(s.setScheme(0));
        REQUIRE_FALSE(s.setScheme(3));
        REQUIRE_FALSE(s.setScheme(4));
        REQUIRE_FALSE(s.setScheme(5));
        REQUIRE_FALSE(s.setScheme(6));
        REQUIRE_FALSE(s.setScheme(7));

        REQUIRE(s.setScheme(2));

        REQUIRE(s.cycles_[0].cycleLength() == 108);
        REQUIRE(s.cycles_[1].cycleLength() == 168);
        REQUIRE(s.cycles_[2].cycleLength() == 66);
        REQUIRE(s.cycles_[3].cycleLength() == 85);
        REQUIRE(s.cycles_[4].cycleLength() == Approx(182.4));
        REQUIRE(s.cycles_[5].cycleLength() == Approx(136.8));

        REQUIRE(s.cycles_[0].periodLength() == 12);
        REQUIRE(s.cycles_[1].periodLength() == 24);
        REQUIRE(s.cycles_[2].periodLength() == 6);
        REQUIRE(s.cycles_[3].periodLength() == 8.5);
        REQUIRE(s.cycles_[4].periodLength() == Approx(30.4));
        REQUIRE(s.cycles_[5].periodLength() == Approx(17.1));

        REQUIRE(s.cycles_[0].periodCount() == 9);
        REQUIRE(s.cycles_[1].periodCount() == 7);
        REQUIRE(s.cycles_[2].periodCount() == 11);
        REQUIRE(s.cycles_[3].periodCount() == 10);
        REQUIRE(s.cycles_[4].periodCount() == 6);
        REQUIRE(s.cycles_[5].periodCount() == 8);

        REQUIRE(s.periodCount() == 51);
        REQUIRE(s.schemeLength() == Approx(746.2));
    }

    SECTION("EventList")
    {
        SoloEventList l;
        REQUIRE(l.empty());
        REQUIRE(l.timeToNextEvent() == -1);
        REQUIRE_FALSE(l.isValidNext());
        REQUIRE_FALSE(l.isValidCurrent());
        REQUIRE(l.nextPtr() == nullptr);
        REQUIRE(l.currentPtr() == nullptr);

        l.add(SoloEvent(EVENT_CYCLE_A, EVENT_TRACK_MIC1, SOLO_EVENT_ON, 10));
        l.reset();

        REQUIRE_FALSE(l.empty());
        REQUIRE(l.timeToNextEvent() == -1);
        REQUIRE_FALSE(l.isValidNext());
        REQUIRE(l.isValidCurrent());
        REQUIRE(l.nextPtr() == nullptr);
        REQUIRE(l.currentPtr());

        l.add(SoloEvent(EVENT_CYCLE_A, EVENT_TRACK_MIC2, SOLO_EVENT_OFF, 100));

        REQUIRE_FALSE(l.empty());
        REQUIRE(l.timeToNextEvent() == 90);
        REQUIRE(l.isValidNext());
        REQUIRE(l.isValidCurrent());
        REQUIRE(l.nextPtr());
        REQUIRE(l.currentPtr());

        l.moveNext({});
        REQUIRE(l.timeToNextEvent() == -1);
        REQUIRE_FALSE(l.isValidNext());
        REQUIRE(l.isValidCurrent());
        REQUIRE(l.nextPtr() == nullptr);
        REQUIRE(l.currentPtr());
        REQUIRE(l.currentPtr()->absTimeMsec() == 100);

        l.add(SoloEvent(EVENT_CYCLE_A, EVENT_TRACK_FB1, SOLO_EVENT_OFF, 0));
        l.add(SoloEvent(EVENT_CYCLE_A, EVENT_TRACK_FB2, SOLO_EVENT_ON, 0));
        l.add(SoloEvent(EVENT_CYCLE_A, EVENT_TRACK_OUT1, SOLO_EVENT_ON, 0));
        l.reset();
        l.moveSame({});
        REQUIRE(l.timeToNextEvent() == 10);
        REQUIRE(l.isValidNext());
        REQUIRE(l.isValidCurrent());
        REQUIRE(l.nextPtr());
        REQUIRE(l.currentPtr());
        REQUIRE(l.currentPtr()->track() == EVENT_TRACK_OUT1);
    }

    SECTION("toString")
    {
        Scheme s(2);
        REQUIRE(s.scheme() == 2);
        REQUIRE(s.schemeLength() == Approx(746.2));
        REQUIRE(s.periodCount() == 51);
        REQUIRE(s.toString(0) == "_32_23_2_41312_12_1_11_2_E__21_11_21_3____8__3___6_");
        REQUIRE(s.calcPeriodPos(0) == 0);
        REQUIRE(s.calcPeriodPos(1) == 12);
        REQUIRE(s.calcPeriodPos(2) == 24);
        REQUIRE(s.calcPeriodPos(8) == 8 * 12);
        REQUIRE(s.calcPeriodPos(9) == 9 * 12);
        REQUIRE(s.calcPeriodPos(10) == 9 * 12 + 1 * 24);
        REQUIRE(s.calcPeriodPos(11) == 9 * 12 + 2 * 24);
        REQUIRE(s.calcPeriodPos(12) == 9 * 12 + 3 * 24);
        REQUIRE(s.calcPeriodPos(13) == 9 * 12 + 4 * 24);
        REQUIRE(s.calcPeriodPos(14) == 9 * 12 + 5 * 24);
        REQUIRE(s.calcPeriodPos(15) == 9 * 12 + 6 * 24);
        REQUIRE(s.calcPeriodPos(16) == 9 * 12 + 7 * 24);
        REQUIRE(s.calcPeriodPos(17) == 9 * 12 + 7 * 24 + 1 * 6);
        REQUIRE(s.calcPeriodPos(50) == Approx(729.1));
        REQUIRE(s.calcPeriodPos(51) == Approx(746.2));
        REQUIRE(s.calcPeriodPos(52) == 0);
    }
}
