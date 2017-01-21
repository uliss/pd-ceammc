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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ceammc_timeline.h"

using namespace ceammc::tl;

void tl_action(TimelineData* d)
{
}

struct CbActionTest {
    static TimelineData* orig;
    static void action(TimelineData* d)
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

        CueStorage::add(c1);
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
        REQUIRE(CueStorage::at(cnv, 0) == c3);
        REQUIRE(CueStorage::at(cnv, 0)->name() == "cue_0");

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
    }

    SECTION("TimelineData")
    {
        TimelineData t1((t_canvas*)0xBEEF, (t_object*)0xBEE);
        REQUIRE(t1.canvas() == (t_canvas*)0xBEEF);
        REQUIRE(t1.object() == (t_object*)0xBEE);
        REQUIRE(t1.xPos() == 0);
        REQUIRE(t1.action() == 0);
        t1.triggerAction();

        t1.setXPos(100);
        REQUIRE(t1.xPos() == 100);

        t1.setAction(tl_action);
        REQUIRE(t1.action() == (void*)tl_action);

        t1.setAction(&CbActionTest::action);
        CbActionTest::orig = &t1;
        t1.triggerAction();
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
}
