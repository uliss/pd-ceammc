/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef TEST_LOG_CPP
#define TEST_LOG_CPP

#include "catch.hpp"
#include "ceammc_log.h"
#include "test_base.h"

#include <sstream>
#include <thread>

static std::string log_str;

static void print_hook(const char* msg)
{
    log_str += msg;
}

TEST_CASE("ceammc_log", "[ceammc_log]")
{
    test::pdSetPrintFunction(print_hook);
    log_str.clear();

    SECTION("operator<< t_symbol*")
    {
        std::ostringstream ss;
        t_symbol* t = gensym("ABC");
        ss << t;
        REQUIRE(ss.str() == "\"ABC\"");
    }

    SECTION("Debug")
    {
        LIB_DBG << "a message " << 123 << '-' << gensym("abc");
        REQUIRE(log_str == "verbose(3): [ceammc] a message 123-\"abc\"\n");
    }

    SECTION("Post")
    {
        LIB_POST << "a message " << 123 << '-' << gensym("abc");
        REQUIRE(log_str == "[ceammc] a message 123-\"abc\"\n");
    }

    SECTION("Log")
    {
        LIB_LOG << "test message";
        REQUIRE(log_str == "verbose(4): [ceammc] test message\n");
    }

    SECTION("Error")
    {
        LIB_ERR << "error message";
        REQUIRE(log_str == "error: [ceammc] error message\n"
                           "verbose(4): ... you might be able to track this down from the Find menu.\n");
    }

    SECTION("None")
    {
        {
            LogNone l;
            l << "test";
            REQUIRE(l.str() == "");
        }

        REQUIRE(log_str.empty());
    }

    SECTION("endl")
    {
        LogPdObject log(nullptr, LogLevel::LOG_POST);
        log << "1";
        log.endl();
        log << "2";
        log.flush();
        REQUIRE(log_str == "[ceammc] 1\n"
                           "[ceammc] 2\n");
        log_str.clear();
    }

    SECTION("operator<< std::vector<int>")
    {
        using namespace ceammc;

        std::vector<int> v({ 1, 2, 3, 4, 5 });
        std::ostringstream ss;
        ss << v;
        REQUIRE(ss.str() == "[1, 2, 3, 4, 5]");
    }

    SECTION("operator<< std::vector<std::string>")
    {
        std::vector<std::string> v({ "A", "B", "C" });
        std::ostringstream ss;
        ss << v;
        REQUIRE(ss.str() == "[A, B, C]");
    }

    SECTION("operator<< std::vector<t_symbol*>")
    {
        std::vector<t_symbol*> v({ gensym("A") });
        std::ostringstream ss;
        ss << v;
        REQUIRE(ss.str() == "[\"A\"]");
    }

    SECTION("thread")
    {
        SECTION("same thread")
        {
            SECTION("debug")
            {
                ThreadPdLogger log;
                log.debug("debug");
                test::pdRunMainLoopMs(10);

                REQUIRE(log_str == "verbose(3): [ceammc] [debug] debug\n");
            }

            SECTION("post")
            {
                ThreadPdLogger log;
                log.post("post");
                test::pdRunMainLoopMs(10);

                REQUIRE(log_str == "[ceammc] post\n");
            }

            SECTION("post")
            {
                ThreadPdLogger log;
                log.verbose("verbose");
                test::pdRunMainLoopMs(10);

                REQUIRE(log_str == "verbose(4): [ceammc] [verbose] verbose\n");
            }

            SECTION("error")
            {
                ThreadPdLogger log;
                log.error("error");
                test::pdRunMainLoopMs(10);

                REQUIRE(log_str == "error: [ceammc] [error] error\n");
            }
        }

        SECTION("prefix")
        {
            ThreadPdLogger log("prefix");
            log.post("post");
            test::pdRunMainLoopMs(10);

            REQUIRE(log_str == "[ceammc] [prefix] post\n");
        }

        SECTION("other thread")
        {
            std::thread th([]() {
                ThreadPdLogger log;
                log.error("thread error");
                test::pdRunMainLoopMs(10);
            });

            test::pdRunMainLoopMs(10);
            th.join();
            REQUIRE(log_str == "error: [ceammc] [error] thread error\n");
        }
    }
}

#endif // TEST_LOG_CPP
