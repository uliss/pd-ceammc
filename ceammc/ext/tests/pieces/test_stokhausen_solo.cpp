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
        REQUIRE(s.lengthSec() == 0);
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
        REQUIRE(s.lengthSec() == Approx(746.2));
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

        l.add(SoloEvent(CYCLE_A, TRACK_MIC1, SOLO_EVENT_ON, 10));
        l.reset();

        REQUIRE_FALSE(l.empty());
        REQUIRE(l.timeToNextEvent() == -1);
        REQUIRE_FALSE(l.isValidNext());
        REQUIRE(l.isValidCurrent());
        REQUIRE(l.nextPtr() == nullptr);
        REQUIRE(l.currentPtr());

        l.add(SoloEvent(CYCLE_A, TRACK_MIC2, SOLO_EVENT_OFF, 100));

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

        l.add(SoloEvent(CYCLE_A, TRACK_FB1, SOLO_EVENT_OFF, 0));
        l.add(SoloEvent(CYCLE_A, TRACK_FB2, SOLO_EVENT_ON, 0));
        l.add(SoloEvent(CYCLE_A, TRACK_OUT1, SOLO_EVENT_ON, 0));
        l.reset();
        l.moveSame({});
        REQUIRE(l.timeToNextEvent() == 10);
        REQUIRE(l.isValidNext());
        REQUIRE(l.isValidCurrent());
        REQUIRE(l.nextPtr());
        REQUIRE(l.currentPtr());
        REQUIRE(l.currentPtr()->track() == TRACK_OUT1);
    }

    SECTION("toString")
    {
        Scheme s(2);
        REQUIRE(s.scheme() == 2);
        REQUIRE(s.lengthSec() == Approx(746.2));
        REQUIRE(s.periodCount() == 51);
        REQUIRE(s.toString(0) == "_32_23_2_41312_12_1_11_2_E__21_11_21_3____8__3___6_");
        REQUIRE(s.periodTimeSec(0) == 0);
        REQUIRE(s.periodTimeSec(1) == 12);
        REQUIRE(s.periodTimeSec(2) == 24);
        REQUIRE(s.periodTimeSec(8) == 8 * 12);
        REQUIRE(s.periodTimeSec(9) == 9 * 12);
        REQUIRE(s.periodTimeSec(10) == 9 * 12 + 1 * 24);
        REQUIRE(s.periodTimeSec(11) == 9 * 12 + 2 * 24);
        REQUIRE(s.periodTimeSec(12) == 9 * 12 + 3 * 24);
        REQUIRE(s.periodTimeSec(13) == 9 * 12 + 4 * 24);
        REQUIRE(s.periodTimeSec(14) == 9 * 12 + 5 * 24);
        REQUIRE(s.periodTimeSec(15) == 9 * 12 + 6 * 24);
        REQUIRE(s.periodTimeSec(16) == 9 * 12 + 7 * 24);
        REQUIRE(s.periodTimeSec(17) == 9 * 12 + 7 * 24 + 1 * 6);
        REQUIRE(s.periodTimeSec(50) == Approx(729.1));
        REQUIRE(s.periodTimeSec(51) == Approx(746.2));
        REQUIRE(s.periodTimeSec(52) == 0);
    }

    SECTION("cycleBeginPeriodIndex")
    {
        Scheme s;
        s.setScheme(2);
        REQUIRE(s.cycleBeginPeriodIndex(CYCLE_A) == 0);
        REQUIRE(s.cycleBeginPeriodIndex(CYCLE_B) == 9);
        REQUIRE(s.cycleBeginPeriodIndex(CYCLE_C) == 16);
        REQUIRE(s.cycleBeginPeriodIndex(CYCLE_D) == 27);
        REQUIRE(s.cycleBeginPeriodIndex(CYCLE_E) == 37);
        REQUIRE(s.cycleBeginPeriodIndex(CYCLE_F) == 43);

        REQUIRE(s.cycleEndPeriodIndex(CYCLE_A) == 9);
        REQUIRE(s.cycleEndPeriodIndex(CYCLE_B) == 16);
        REQUIRE(s.cycleEndPeriodIndex(CYCLE_C) == 27);
        REQUIRE(s.cycleEndPeriodIndex(CYCLE_D) == 37);
        REQUIRE(s.cycleEndPeriodIndex(CYCLE_E) == 43);
        REQUIRE(s.cycleEndPeriodIndex(CYCLE_F) == 51);
    }

    SECTION("scheme2")
    {
        Scheme s;
        s.setScheme(2);
        auto p = s.periodAt(TRACK_OUT1, CYCLE_C, 0);
        REQUIRE(p->event == solo::EVENT_ON);
        REQUIRE(p->cycle() == CYCLE_C);
        REQUIRE(p->fullLengthTime() == 6);
        REQUIRE(p->attackTime() == 1.5);
        REQUIRE(p->releaseTime() == 3);

        p = s.periodAt(TRACK_OUT2, CYCLE_C, 0);
        REQUIRE(p->event == solo::EVENT_ON);
        REQUIRE(p->cycle() == CYCLE_C);
        REQUIRE(p->fullLengthTime() == 6);
        REQUIRE(p->attackTime() == 3);
        REQUIRE(p->releaseTime() == 4.5);
    }

    SECTION("EventList")
    {
        SoloEventList e;
        REQUIRE(e.empty());

        e.add(SoloEvent(CYCLE_A, TRACK_MIC1, SOLO_EVENT_ON, 100));
        REQUIRE(e.size() == 1);
        e.add(SoloEvent(CYCLE_A, TRACK_MIC2, SOLO_EVENT_ON, 100));
        REQUIRE(e.size() == 2);

        REQUIRE(e.data()[0].track() == TRACK_MIC1);
        REQUIRE(e.data()[1].track() == TRACK_MIC2);

        e.add(SoloEvent(CYCLE_A, TRACK_MIC1, SOLO_EVENT_OFF, 50));
        REQUIRE(e.size() == 3);
        REQUIRE(e.data()[0].type() == SOLO_EVENT_OFF);
        REQUIRE(e.data()[1].type() == SOLO_EVENT_ON);
        REQUIRE(e.data()[2].type() == SOLO_EVENT_ON);

        e.clear();
        REQUIRE(e.empty());

        e.addPeriod(TRACK_MIC1, Period(CYCLE_A, EVENT_OFF, 5), 2, 100);
        REQUIRE(e.size() == 1);
        REQUIRE(e.data().front().period() == 2);
        REQUIRE(e.data().front().cycle() == CYCLE_A);
        REQUIRE(e.data().front().track() == TRACK_MIC1);
        REQUIRE(e.data().front().type() == SOLO_EVENT_OFF);
        REQUIRE(e.data().front().absTimeMsec() == 100);

        e.clear();
        e.addPeriod(TRACK_MIC1, Period(CYCLE_A, EVENT_ON, 5), 74, 100.5);
        REQUIRE(e.size() == 1);
        REQUIRE(e.data().front().period() == 74);
        REQUIRE(e.data().front().cycle() == CYCLE_A);
        REQUIRE(e.data().front().track() == TRACK_MIC1);
        REQUIRE(e.data().front().type() == SOLO_EVENT_ON);
        REQUIRE(e.data().front().absTimeMsec() == 100.5);

        e.clear();
        e.addPeriod(TRACK_MIC1, Period(CYCLE_A, EVENT_ON, 5).setRelOffset(0.5, 0.25), 74, 1000);
        REQUIRE(e.size() == 3);
        REQUIRE(e.data()[0].period() == 74);
        REQUIRE(e.data()[0].cycle() == CYCLE_A);
        REQUIRE(e.data()[0].track() == TRACK_MIC1);
        REQUIRE(e.data()[0].type() == SOLO_EVENT_OFF);
        REQUIRE(e.data()[0].absTimeMsec() == 1000);
        REQUIRE(e.data()[1].period() == 74);
        REQUIRE(e.data()[1].cycle() == CYCLE_A);
        REQUIRE(e.data()[1].track() == TRACK_MIC1);
        REQUIRE(e.data()[1].type() == SOLO_EVENT_ON);
        REQUIRE(e.data()[1].absTimeMsec() == 3500);
        REQUIRE(e.data()[2].period() == 74);
        REQUIRE(e.data()[2].cycle() == CYCLE_A);
        REQUIRE(e.data()[2].track() == TRACK_MIC1);
        REQUIRE(e.data()[2].type() == SOLO_EVENT_OFF);
        REQUIRE(e.data()[2].absTimeMsec() == 4750);

        e.clear();
        e.addPeriod(TRACK_MIC1, Period(CYCLE_A, EVENT_CRESC, 16), 74, 1000);
        REQUIRE(e.size() == 65);
        REQUIRE(e.data()[0].period() == 74);
        REQUIRE(e.data()[0].absTimeMsec() == 1000);
        REQUIRE(e.data()[0].value() == 0);
        REQUIRE(e.data()[1].absTimeMsec() == 1250);
        REQUIRE(e.data()[1].value() == 1 / 64.0);
        REQUIRE(e.data()[2].absTimeMsec() == 1500);
        REQUIRE(e.data()[2].value() == 2 / 64.0);
        REQUIRE(e.data()[32].absTimeMsec() == 1000 + 250 * 32);
        REQUIRE(e.data()[32].value() == 0.5);
        REQUIRE(e.data()[64].absTimeMsec() == 17000);
        REQUIRE(e.data()[64].value() == 1);

        e.clear();
        e.addPeriod(TRACK_MIC1, Period(CYCLE_A, EVENT_CRESC, 16).setRelOffset(0.25, 0.5), 74, 1000);
        REQUIRE(e.size() == 66);
        REQUIRE(e.data()[0].period() == 74);
        REQUIRE(e.data()[0].absTimeMsec() == 1000);
        REQUIRE(e.data()[0].value() == 0);
        REQUIRE(e.data()[0].type() == SOLO_EVENT_OFF);
        REQUIRE(e.data()[1].absTimeMsec() == 5000);
        REQUIRE(e.data()[1].value() == 0);
        REQUIRE(e.data()[2].absTimeMsec() == 5125);
        REQUIRE(e.data()[2].value() == 1 / 64.0);
        REQUIRE(e.data()[3].absTimeMsec() == 5250);
        REQUIRE(e.data()[3].value() == 2 / 64.0);
        REQUIRE(e.data()[33].absTimeMsec() == 5000 + 125 * 32);
        REQUIRE(e.data()[33].value() == 0.5);
        REQUIRE(e.data()[65].absTimeMsec() == 5000 + 125 * 64);
        REQUIRE(e.data()[65].value() == 1);
    }
}
