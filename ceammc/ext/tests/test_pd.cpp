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

#include "../base/debug_gensym.h"
#include "catch.hpp"
#include "ceammc_pd.h"
#include "ceammc_platform.h"
#include "g_ceammc_draw.h"

#include <set>
#include <stdio.h>

using namespace pd;

TEST_CASE("PD", "[PureData]")
{
    SECTION("hash table size")
    {
        t_ceammc_gensym_info info;
        gensym_info(&info);

        REQUIRE(info.table_size == 1024);

        int sz = info.symbol_count;

        gensym("test");
        gensym_info(&info);
        REQUIRE(info.symbol_count == sz + 1);

#ifdef __MACOSX_CORE__
        REQUIRE(info.max_chain == 1);
        REQUIRE(info.memory_size == 188);
#endif

        char buf[20];
        for (int i = 0; i < 20000; i++) {
            sprintf(buf, "%d", i + sz);
            gensym(buf);
        }

        gensym_info(&info);
        REQUIRE(info.symbol_count == sz + 20001);

#ifdef __MACOSX_CORE__
        REQUIRE(info.max_chain == 39);
        REQUIRE(info.memory_size == 188);
#endif
    }

    SECTION("memrss")
    {
#if !defined(__FreeBSD__)
        REQUIRE(ceammc::platform::memory_current_rss() != 0);
        REQUIRE(ceammc::platform::memory_peak_rss() != 0);
#endif
    }

    SECTION("memsize")
    {
        REQUIRE(ceammc::platform::memory_size() != 0);
    }

    SECTION("test current object list")
    {
        using namespace ceammc;
        typedef std::vector<std::string> slist;
        typedef std::set<std::string> sset;
        slist l = ceammc::pd::currentListOfExternals();
        REQUIRE(l.size() > 0);

        sset s(l.begin(), l.end());
        REQUIRE(s.count("unknown") == 0);
        REQUIRE(s.count("osc~") == 1);
        REQUIRE(s.count("unpack") == 1);
        REQUIRE(s.count("pack") == 1);
        REQUIRE(s.count("list.each") == 0);
    }

    SECTION("env")
    {
        REQUIRE(ceammc::platform::get_env("TEST") == "");
        ceammc::platform::set_env("TEST", "VALUE");
        REQUIRE(ceammc::platform::get_env("TEST") == "VALUE");
    }

    SECTION("int2str")
    {
        char buf[20];
        REQUIRE(g_int2str(0, buf) == 1);
        REQUIRE(std::string("0") == buf);
    }
}
