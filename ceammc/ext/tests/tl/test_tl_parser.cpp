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
#include "tl_cmd_driver.h"
#include "catch.hpp"

class Target : public ceammc::tl::TlIFace {
public:
    bool addAbsEvent(const std::string& name, double time) override
    {
        std::cerr << "add " << name << " at " << time << " ms\n";
        return true;
    }

    bool addRelEvent(const std::string& name, double time, const std::string& relName) override
    {
        std::cerr << "add " << name << " at " << time << " ms relative to " << relName << "\n";
        return true;
    }

    void error(const std::string& m) override
    {
        std::cerr << m << "\n";
    }

    bool removeEvent(const std::string& name) override
    {
        std::cerr << "remove event \"" << name << "\"\n";
        return true;
    }

    bool removeEventAtTime(double time_ms) override
    {
        std::cerr << "remove event at " << time_ms << " ms\n";
        return true;
    }

    bool removeEventAtPos(int pos) override
    {
        std::cerr << "remove event at position " << pos << "\n";
        return true;
    }

    void clearAll() override
    {
        std::cerr << "clear\n";
    }

    bool moveToEvent(const std::string& name) override
    {
        std::cerr << "move to event: " << name << "\n";
        return true;
    }

    bool moveToEvent(long idx) override {
        std::cerr << "move to event #" << idx << "\n";
        return true;
    }

    bool moveToTime(double time_ms) override
    {
        std::cerr << "move to time: " << time_ms << " ms\n";
        return true;
    }
};

TEST_CASE("tl::parser", "[tl]")
{
    SECTION("impl")
    {
        using namespace ceammc::tl;
        TlCmdDriver driver(new Target);
        //        driver.trace_parsing = true;

        // no units
        REQUIRE(!driver.parse("add 300"));
        REQUIRE(driver.parse("add 300 ms"));
        REQUIRE(driver.parse("add event1 50s"));
        REQUIRE(driver.parse("add event1 -10s"));
        REQUIRE(driver.parse("add event1 -10.2 ms"));
        REQUIRE(driver.parse("add event1 -10 sec"));
        REQUIRE(driver.parse("add event2 50.1 ms"));
        REQUIRE(driver.parse("add event2 -0.111ms"));
        REQUIRE(driver.parse("add event2 0.111s"));
        REQUIRE(driver.parse("add event2 5:45"));
        REQUIRE(driver.parse("add event2 15:45"));
        REQUIRE(driver.parse("add event2 00:04:01"));
        REQUIRE(driver.parse("add event2 15:45.1"));
        REQUIRE(driver.parse("add event2 15:45.12"));
        REQUIRE(driver.parse("add event2 15:45.123"));
        REQUIRE(driver.parse("add event2 24:15:45"));
        REQUIRE(driver.parse("add event2 24:15:45.123"));
        REQUIRE(driver.parse("add event2 0:01.500"));

        REQUIRE(driver.parse("add event2 44s after event3"));
        REQUIRE(driver.parse("add event2 -55.1 ms before event5"));

        REQUIRE(driver.parse("remove event2"));
        REQUIRE(driver.parse("remove 0"));
        REQUIRE(driver.parse("remove -1"));
        REQUIRE(driver.parse("remove -200"));
        REQUIRE(driver.parse("remove 1000"));

        // float index
        REQUIRE(!driver.parse("remove 1.1"));

        REQUIRE(driver.parse("remove_at 100ms"));
        REQUIRE(driver.parse("remove_at 10.1 sec"));
        // not units
        REQUIRE(!driver.parse("remove_at 100"));

        REQUIRE(driver.parse("clear"));

        REQUIRE(driver.parse("to_event 12"));
        REQUIRE(driver.parse("to_event 0"));
        REQUIRE(driver.parse("to_event -1"));
        REQUIRE(!driver.parse("to_event 2.34"));

        REQUIRE(driver.parse("to_time 0:42"));
        REQUIRE(driver.parse("to_time 300sec"));
        REQUIRE(driver.parse("to_time 2.1sec"));
        // no units
        REQUIRE(!driver.parse("to_time 14"));
    }
}
