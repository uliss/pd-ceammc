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
#include "ceammc.hpp"

extern "C" void pd_init();

#include <set>
#include <stdio.h>

using namespace pd;

TEST_CASE("PD", "[PureData]")
{
    SECTION("hash table size")
    {
        const int OFFSET = 7;

        t_ceammc_gensym_info info;
        gensym_info(&info);

        REQUIRE(info.table_size == 1024);

        REQUIRE(info.symbol_count == 0 + OFFSET);

        gensym("test");
        gensym_info(&info);
        REQUIRE(info.symbol_count == 1 + OFFSET);
        REQUIRE(info.max_chain == 1);
        REQUIRE(info.memory_size == 64);

        char buf[20];
        for (int i = 0; i < 20000; i++) {
            sprintf(buf, "%d", i + OFFSET);
            gensym(buf);
        }

        gensym_info(&info);
        REQUIRE(info.symbol_count == 20001 + OFFSET);
        REQUIRE(info.max_chain == 38);
    }

    SECTION("memrss")
    {
#if !defined(__FreeBSD__)
        REQUIRE(ceammc_memory_current_rss() != 0);
        REQUIRE(ceammc_memory_peak_rss() != 0);
#endif
    }

    SECTION("memsize")
    {
        REQUIRE(ceammc_memory_size() != 0);
    }

    SECTION("test current object list")
    {
        pd_init();

        using namespace ceammc;
        typedef std::vector<std::string> slist;
        typedef std::set<std::string> sset;
        slist l = currentExtensionList();
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
        REQUIRE(ceammc::get_env("TEST") == "");
        ceammc::set_env("TEST", "VALUE");
        REQUIRE(ceammc::get_env("TEST") == "VALUE");
    }
}
