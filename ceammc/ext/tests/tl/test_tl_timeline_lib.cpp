/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#include "ceammc_timeline.h"

using namespace ceammc::tl;

void tl_action(TimelineData* d, int v)
{
}

struct CbActionTest {
    static TimelineData* orig;
    static void action(TimelineData* d, int v)
    {
        REQUIRE(orig == d);
    }
};

TimelineData* CbActionTest::orig = 0;

TEST_CASE("timeline", "[ceammc::timelime]")
{
    SECTION("CueData")
    {
        CueData c1(0, 0);
        REQUIRE(c1.canvas() == 0);
        REQUIRE(c1.xPos() == 0);
        REQUIRE(c1.index() == -1);
        REQUIRE(c1.name() == "cue_x");

        c1.setIndex(11);
        REQUIRE(c1.index() == 11);
        REQUIRE(c1.name() == "cue_11");

        c1.setXPos(103);
        REQUIRE(c1.xPos() == 103);

        SECTION("==")
        {
            CueData c1(0, 0);
            CueData c2(0, 0);
            REQUIRE(c1 == c2);

            CueData c3(0, (t_object*)100);
            REQUIRE_FALSE(c1 == c3);
        }
    }

    SECTION("CueStorage")
    {
        t_canvas* cnv = 0;
        t_object* obj1 = 0;
        CueData* c1 = new CueData(cnv, obj1);
        REQUIRE(CueStorage::cueCount(cnv) == 0);
        REQUIRE_FALSE(CueStorage::exists(cnv));
        REQUIRE_FALSE(CueStorage::exists(c1));
        REQUIRE(CueStorage::index(c1) == -1);
        REQUIRE(CueStorage::at(cnv, 0) == 0);
        CueStorage::sort(cnv);
        CueStorage::enumerate(cnv);
        REQUIRE(CueStorage::index(c1) == -1);

        REQUIRE_FALSE(CueStorage::add(0));
        REQUIRE(CueStorage::add(c1));
        REQUIRE(CueStorage::cueCount(cnv) == 1);
        REQUIRE(CueStorage::exists(cnv));
        REQUIRE(CueStorage::exists(c1));
        REQUIRE(CueStorage::index(c1) == 0);
        REQUIRE(CueStorage::at(cnv, 0) == c1);
        REQUIRE(CueStorage::at(cnv, 1) == 0);
        REQUIRE(CueStorage::at(cnv, 0)->name() == "cue_0");
        CueStorage::sort(cnv);
        CueStorage::enumerate(cnv);
        REQUIRE(CueStorage::at(cnv, 0)->name() == "cue_0");
        REQUIRE(CueStorage::index(c1) == 0);

        CueStorage::add(c1);
        REQUIRE(CueStorage::cueCount(cnv) == 1);
        REQUIRE(CueStorage::exists(cnv));
        REQUIRE(CueStorage::exists(c1));
        REQUIRE(CueStorage::index(c1) == 0);
        CueStorage::sort(cnv);
        CueStorage::enumerate(cnv);
        REQUIRE(CueStorage::at(cnv, 0)->name() == "cue_0");
        REQUIRE(CueStorage::index(c1) == 0);

        CueData* c2 = new CueData(cnv, (t_object*)100);

        CueStorage::add(c2);
        REQUIRE(CueStorage::cueCount(cnv) == 2);
        REQUIRE(CueStorage::exists(cnv));
        REQUIRE(CueStorage::exists(c1));
        REQUIRE(CueStorage::exists(c2));
        REQUIRE(CueStorage::index(c1) == 0);
        REQUIRE(CueStorage::index(c2) == 1);
        REQUIRE(CueStorage::at(cnv, 0)->name() == "cue_0");
        REQUIRE(CueStorage::at(cnv, 1)->name() == "cue_1");
        CueStorage::sort(cnv);
        CueStorage::enumerate(cnv);
        REQUIRE(CueStorage::at(cnv, 0) == c1);
        REQUIRE(CueStorage::at(cnv, 1) == c2);
        REQUIRE(CueStorage::at(cnv, 0)->name() == "cue_0");
        REQUIRE(CueStorage::at(cnv, 1)->name() == "cue_1");

        CueStorage::at(cnv, 0)->setXPos(10);
        CueStorage::at(cnv, 1)->setXPos(20);
        CueStorage::sort(cnv);
        REQUIRE(CueStorage::at(cnv, 0) == c1);
        REQUIRE(CueStorage::at(cnv, 1) == c2);
        REQUIRE(CueStorage::at(cnv, 0)->name() == "cue_0");
        REQUIRE(CueStorage::at(cnv, 1)->name() == "cue_1");

        CueStorage::at(cnv, 0)->setXPos(20);
        CueStorage::at(cnv, 1)->setXPos(10);
        CueStorage::sort(cnv);
        REQUIRE(CueStorage::at(cnv, 1) == c1);
        REQUIRE(CueStorage::at(cnv, 0) == c2);
        REQUIRE(CueStorage::at(cnv, 0)->name() == "cue_1");
        REQUIRE(CueStorage::at(cnv, 1)->name() == "cue_0");
        CueStorage::enumerate(cnv);
        REQUIRE(CueStorage::at(cnv, 0)->name() == "cue_0");
        REQUIRE(CueStorage::at(cnv, 1)->name() == "cue_1");

        CueData* c3 = new CueData(cnv, (t_object*)111);
        c3->setXPos(-100);
        CueStorage::add(c3);
        REQUIRE(CueStorage::find(cnv, (t_object*)111) == c3);
        REQUIRE(CueStorage::find((t_canvas*)0xBEE, (t_object*)111) == 0);
        REQUIRE(CueStorage::find(cnv, (t_object*)112) == 0);
        REQUIRE(CueStorage::at(cnv, 0) == c3);
        REQUIRE(CueStorage::at(cnv, 0)->name() == "cue_0");

        REQUIRE_FALSE(CueStorage::remove(0));
        CueStorage::remove(c3);
        CueStorage::remove(c1);
        REQUIRE_FALSE(CueStorage::exists(c1));
        REQUIRE_FALSE(CueStorage::exists(c3));
        REQUIRE(CueStorage::exists(c2));
        REQUIRE(CueStorage::cueCount(cnv) == 1);
        REQUIRE(CueStorage::at(cnv, 0) == c2);
        REQUIRE(CueStorage::at(cnv, 0)->name() == "cue_0");

        CueStorage::remove(c2);
        REQUIRE_FALSE(CueStorage::exists(cnv));

        delete c1;
        delete c2;
        delete c3;

        SECTION("cueList")
        {
            REQUIRE(CueStorage::cueList((CueData*)0) == 0);
            REQUIRE(CueStorage::find(0, 0) == 0);
        }
    }

    SECTION("TimelineData")
    {
        TimelineData t1((t_canvas*)0xBEEF, (t_object*)0xBEE);
        REQUIRE(t1.canvas() == (t_canvas*)0xBEEF);
        REQUIRE(t1.object() == (t_object*)0xBEE);
        REQUIRE(t1.xPos() == 0);
        REQUIRE(t1.action() == 0);
        t1.triggerAction(1);

        t1.setXPos(100);
        REQUIRE(t1.xPos() == 100);

        t1.setAction(tl_action);
        REQUIRE(t1.action() == (void*)tl_action);

        t1.setAction(&CbActionTest::action);
        CbActionTest::orig = &t1;
        t1.triggerAction(1);
    }

    SECTION("UIStorage")
    {
        TimelineData t1((t_canvas*)0x111, (t_object*)0x222);
        TimelineData t2((t_canvas*)0x333, (t_object*)0x444);

        REQUIRE(UIStorage::at(0) == 0);
        REQUIRE_FALSE(UIStorage::exists(&t1));
        REQUIRE(UIStorage::size() == 0);

        UIStorage::add(&t1);
        REQUIRE(UIStorage::exists(&t1));
        REQUIRE(UIStorage::at(0) == &t1);
        REQUIRE(UIStorage::at(1) == 0);
        REQUIRE(UIStorage::size() == 1);

        UIStorage::add(&t1);
        REQUIRE(UIStorage::exists(&t1));
        REQUIRE(UIStorage::at(0) == &t1);
        REQUIRE(UIStorage::at(1) == 0);
        REQUIRE(UIStorage::size() == 1);

        UIStorage::add(&t2);
        REQUIRE(UIStorage::exists(&t1));
        REQUIRE(UIStorage::exists(&t2));
        REQUIRE(UIStorage::at(0) == &t1);
        REQUIRE(UIStorage::at(1) == &t2);
        REQUIRE(UIStorage::at(2) == 0);
        REQUIRE(UIStorage::size() == 2);

        UIStorage::remove(&t1);
        REQUIRE(!UIStorage::exists(&t1));
        REQUIRE(UIStorage::exists(&t2));
        REQUIRE(UIStorage::at(0) == &t2);
        REQUIRE(UIStorage::at(1) == 0);
        REQUIRE(UIStorage::size() == 1);

        UIStorage::remove(&t2);
        REQUIRE(!UIStorage::exists(&t1));
        REQUIRE(!UIStorage::exists(&t2));
        REQUIRE(UIStorage::at(0) == 0);
        REQUIRE(UIStorage::size() == 0);
    }

    SECTION("trigger_actions")
    {
        t_canvas* cnv1 = (t_canvas*)0xBEEF;
        t_canvas* cnv2 = (t_canvas*)0xBEEB;
        t_object* obj1 = (t_object*)0x11;
        t_object* obj2 = (t_object*)0x22;
        t_object* obj3 = (t_object*)0x33;
        t_object* obj4 = (t_object*)0x44;
        t_object* obj5 = (t_object*)0x55;
        REQUIRE(trigger_actions(0, 0) == 0);

        CueData cue1(cnv1, obj1);
        REQUIRE(trigger_actions(cnv1, 0) == 0);

        CueStorage::add(&cue1);
        REQUIRE(trigger_actions(cnv1, 0) == 0);

        cue1.setXPos(20);

        TimelineData tl1(cnv1, obj2);
        UIStorage::add(&tl1);
        tl1.setXPos(5);
        CbActionTest::orig = &tl1;
        tl1.setAction(&CbActionTest::action);
        REQUIRE(trigger_actions(cnv1, 0) == 0);

        tl1.setXPos(30);
        REQUIRE(trigger_actions(cnv1, 0) == 1);

        // other canvas
        TimelineData tl2(cnv2, obj3);
        tl2.setXPos(30);
        UIStorage::add(&tl2);
        REQUIRE(trigger_actions(cnv1, 0) == 1);

        TimelineData tl3(cnv1, obj3);
        tl3.setXPos(50);
        UIStorage::add(&tl3);
        REQUIRE(trigger_actions(cnv1, 0) == 2);

        // add new trigger
        CueData cue2(cnv1, obj4);
        cue2.setXPos(40);
        CueStorage::add(&cue2);

        REQUIRE(trigger_actions(cnv1, 0) == 1);
        REQUIRE(trigger_actions(cnv1, 1) == 1);

        // add new trigger
        CueData cue3(cnv1, obj5);
        cue3.setXPos(100);
        CueStorage::add(&cue3);

        REQUIRE(trigger_actions(cnv1, 0) == 1);
        REQUIRE(trigger_actions(cnv1, 1) == 1);
        REQUIRE(trigger_actions(cnv1, 2) == 0);
        REQUIRE(trigger_actions(cnv1, 3) == 0);

        CueStorage::remove(&cue1);
        CueStorage::remove(&cue2);
        CueStorage::remove(&cue3);

        UIStorage::remove(&tl1);
        UIStorage::remove(&tl2);
        UIStorage::remove(&tl3);
    }
}
