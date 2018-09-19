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
#include "m_pd.h"
#include "test_external.h"

#include <sstream>

static std::string print_hook_result;

static void print_hook(const char* msg)
{
    print_hook_result += msg;
}

TEST_CASE("ceammc_log", "[ceammc_log]")
{
    test::pdSetPrintFunction(print_hook);

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
        REQUIRE(print_hook_result == "[ceammc] a message 123-\"abc\"\n");
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
        using namespace ceammc;

        std::vector<std::string> v({ "A", "B", "C" });
        std::ostringstream ss;
        ss << v;
        REQUIRE(ss.str() == "[A, B, C]");
    }
}

#endif // TEST_LOG_CPP
