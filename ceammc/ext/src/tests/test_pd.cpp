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
#include "ceammc.hpp"
#include "../debug_gensym.h"

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

    SECTION("memrss") {
#if !defined(__FreeBSD__)
        REQUIRE(ceammc_memory_current_rss() != 0);
        REQUIRE(ceammc_memory_peak_rss() != 0);
#endif
    }

    SECTION("memsize") {
        REQUIRE(ceammc_memory_size() != 0);
    }
}
