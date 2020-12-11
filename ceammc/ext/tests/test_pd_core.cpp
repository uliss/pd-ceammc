/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "g_ceammc_draw.h"

#include <cstdlib>
#include <cstring>
#include <ctime>

#ifndef __APPLE__
extern "C" void strcpy_safe_tcl(char* dest, const char* src);
#endif

TEST_CASE("pd core", "[PureData]")
{
    SECTION("g_int2str")
    {
        REQUIRE(g_int2str(100, 0) == 0);

        char buf[16];
        REQUIRE(g_int2str(0, buf) == 1);
        REQUIRE(std::string(buf) == "0");
        REQUIRE(g_int2str(123, buf) == 3);
        REQUIRE(std::string(buf) == "123");

        REQUIRE(g_int2str(-456, buf) == 4);
        REQUIRE(std::string(buf) == "-456");

        std::srand(unsigned(std::time(0)));
        for (int i = 0; i < 100; i++) {
            int v = std::rand();
            g_int2str(v, buf);
            REQUIRE(std::to_string(v) == buf);
        }

        strcpy(buf, "...");
        REQUIRE(g_int2str(123, buf + 3) == 3);
        REQUIRE(std::string(buf) == "...123");
    }

    SECTION("g_va_int2str")
    {
        char buf[16];
        REQUIRE(g_va_int2str(buf, 16, 1, 10001) == 0);
        REQUIRE(std::string(buf) == "10001 ");

        REQUIRE(g_va_int2str(buf, 0, 2, 100, 200) != 0);
        REQUIRE(g_va_int2str(buf, 1, 2, 100, 200) != 0);
        REQUIRE(g_va_int2str(buf, 2, 2, 100, 200) != 0);
        REQUIRE(g_va_int2str(buf, 3, 2, 100, 200) != 0);
        REQUIRE(g_va_int2str(buf, 4, 2, 100, 200) != 0);
        REQUIRE(g_va_int2str(buf, 5, 2, 100, 200) != 0);
        REQUIRE(g_va_int2str(buf, 6, 2, 100, 200) != 0);
        REQUIRE(g_va_int2str(buf, 7, 2, 100, 200) != 0);
        REQUIRE(g_va_int2str(buf, 8, 2, 100, 200) != 0);
        REQUIRE(g_va_int2str(buf, 9, 2, 100, 200) == 0);

        char buf2[32];
        REQUIRE(g_va_int2str(buf2, 32, 2, 100, 200) == 0);
        REQUIRE(std::string(buf2) == "100 200 ");
    }

#ifndef __APPLE__
    SECTION("strcpy_safe_tcl")
    {
        char buf[100];
        strcpy_safe_tcl(buf, "123");
        REQUIRE(std::string(buf) == "123");

        strcpy_safe_tcl(buf, "123{");
        REQUIRE(std::string(buf) == "123(");
        strcpy_safe_tcl(buf, "123{}");
        REQUIRE(std::string(buf) == "123()");
    }
#endif
}
