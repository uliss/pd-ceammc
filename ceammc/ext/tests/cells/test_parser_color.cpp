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
#include "lex/parser_color.h"
#include "test_base.h"

using namespace ceammc;

TEST_CASE("parser_color", "[ceammc::ceammc_units]")
{
    test::pdPrintToStdError();

    SECTION("rgb hex")
    {
        using namespace ceammc::parser;
        RgbHexFullMatch p;

        REQUIRE(p.parse("#000000"));
        REQUIRE(p.red() == 0);
        REQUIRE(p.green() == 0);
        REQUIRE(p.blue() == 0);

        REQUIRE(p.parse("#090000"));
        REQUIRE(p.red() == 9);
        REQUIRE(p.green() == 0);
        REQUIRE(p.blue() == 0);

        REQUIRE(p.parse("#f90000"));
        REQUIRE(p.red() == 0xf9);
        REQUIRE(p.green() == 0);
        REQUIRE(p.blue() == 0);

        REQUIRE(p.parse("#F91c00"));
        REQUIRE(p.red() == 0xf9);
        REQUIRE(p.green() == 0x1C);
        REQUIRE(p.blue() == 0);
        REQUIRE(p.asInt() == 0xF91C00);

        REQUIRE(p.parse("#00003a"));
        REQUIRE(p.red() == 0);
        REQUIRE(p.green() == 0);
        REQUIRE(p.blue() == 0x3A);

        REQUIRE(!p.parse(""));
        REQUIRE(!p.parse("000000"));
        REQUIRE(!p.parse("#12345"));
        REQUIRE(!p.parse("#1234567"));
    }

    SECTION("short")
    {
        using namespace ceammc::parser;
        RgbHexFullMatch p;

        REQUIRE(p.parse("#000"));
        REQUIRE(p.red() == 0);
        REQUIRE(p.green() == 0);
        REQUIRE(p.blue() == 0);

        REQUIRE(p.parse("#123"));
        REQUIRE(p.red() == 0x11);
        REQUIRE(p.green() == 0x22);
        REQUIRE(p.blue() == 0x33);

        REQUIRE(p.parse("#ABC"));
        REQUIRE(p.red() == 0xAA);
        REQUIRE(p.green() == 0xBB);
        REQUIRE(p.blue() == 0xCC);
    }

    SECTION("atom")
    {
        using namespace ceammc::parser;
        RgbHexFullMatch p;

        REQUIRE(p.parse(A("#012345")));
        REQUIRE(p.asInt() == 0x012345);

        REQUIRE_FALSE(p.parse(A(0x112233)));
    }

    SECTION("list")
    {
        using namespace ceammc::parser;
        RgbHexFullMatch p;
        SmallColorVec res;

        REQUIRE(p.parse(LA("#012345", "#FFAA00", 200), res));
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == 0x012345);
        REQUIRE(res[1] == 0xFFAA00);
    }

    SECTION("N")
    {
        using namespace ceammc::parser;
        RgbHexFullMatch p;

        auto res = p.parseN<3>(L(), 0x000000);
        REQUIRE(res[0] == 0x000000);
        REQUIRE(res[1] == 0x000000);
        REQUIRE(res[2] == 0x000000);

        res = p.parseN<3>(LA("#FF11AA"), 0x000001);
        REQUIRE(res[0] == 0xFF11AA);
        REQUIRE(res[1] == 0x000001);
        REQUIRE(res[2] == 0x000001);

        res = p.parseN<3>(LA("#FF11AA", "#ababba"), 0x000002);
        REQUIRE(res[0] == 0xFF11AA);
        REQUIRE(res[1] == 0xABABBA);
        REQUIRE(res[2] == 0x000002);

        res = p.parseN<3>(LA("#FF11AA", "#ababba", "#123456"), 0x000003);
        REQUIRE(res[0] == 0xFF11AA);
        REQUIRE(res[1] == 0xABABBA);
        REQUIRE(res[2] == 0x123456);
    }

    SECTION("rgba hex")
    {
        using namespace ceammc::parser;
        RgbaHexFullMatch p;

        REQUIRE(p.parse("#00000000"));
        REQUIRE(p.red() == 0);
        REQUIRE(p.green() == 0);
        REQUIRE(p.blue() == 0);
        REQUIRE(p.alpha() == 0);

        REQUIRE(p.parse("#090000DA"));
        REQUIRE(p.red() == 9);
        REQUIRE(p.green() == 0);
        REQUIRE(p.blue() == 0);
        REQUIRE(p.alpha() == 0xDA);

        REQUIRE(p.parse("#f90000FF"));
        REQUIRE(p.red() == 0xf9);
        REQUIRE(p.green() == 0);
        REQUIRE(p.blue() == 0);
        REQUIRE(p.alpha() == 255);
        REQUIRE(p.norm_alpha() == 1);

        REQUIRE(p.parse("#F91c0010"));
        REQUIRE(p.red() == 0xf9);
        REQUIRE(p.green() == 0x1C);
        REQUIRE(p.blue() == 0);
        REQUIRE(p.alpha() == 0x10);
        REQUIRE(p.asInt() == 0xF91C0010);

        REQUIRE(p.parse("#00003aF0"));
        REQUIRE(p.red() == 0);
        REQUIRE(p.green() == 0);
        REQUIRE(p.blue() == 0x3A);
        REQUIRE(p.alpha() == 0xF0);

        REQUIRE(!p.parse(""));
        REQUIRE(!p.parse("000000"));
        REQUIRE(!p.parse("#000000"));
        REQUIRE(!p.parse("#12345"));
        REQUIRE(!p.parse("#1234567"));
    }

    SECTION("atom")
    {
        using namespace ceammc::parser;
        RgbaHexFullMatch p;

        REQUIRE(p.parse(A("#01234567")));
        REQUIRE(p.asInt() == 0x01234567);

        REQUIRE_FALSE(p.parse(A(0x112233)));
    }

    SECTION("N")
    {
        using namespace ceammc::parser;
        RgbaHexFullMatch p;

        auto res = p.parseN<3>(L(), 0x000000);
        REQUIRE(res[0] == 0x000000);
        REQUIRE(res[1] == 0x000000);
        REQUIRE(res[2] == 0x000000);

        res = p.parseN<3>(LA("#FF11AADD"), 0x000001);
        REQUIRE(res[0] == 0xFF11AADD);
        REQUIRE(res[1] == 0x000001);
        REQUIRE(res[2] == 0x000001);

        res = p.parseN<3>(LA("#FF11AABB", "#ababbacc"), 0x000002);
        REQUIRE(res[0] == 0xFF11AABB);
        REQUIRE(res[1] == 0xABABBACC);
        REQUIRE(res[2] == 0x000002);

        res = p.parseN<3>(LA("#FF11AABB", "#ababbacc", "#123456DD"), 0x000003);
        REQUIRE(res[0] == 0xFF11AABB);
        REQUIRE(res[1] == 0xABABBACC);
        REQUIRE(res[2] == 0x123456DD);

        res = p.parseN<3>(LA("#FF11AABB", "#ababbacc", "#123456DD", "#12345678"), 0x000003);
        REQUIRE(res[0] == 0xFF11AABB);
        REQUIRE(res[1] == 0xABABBACC);
        REQUIRE(res[2] == 0x123456DD);
    }

    SECTION("short")
    {
        using namespace ceammc::parser;
        RgbaHexFullMatch p;

        REQUIRE(p.parse("#0000"));
        REQUIRE(p.red() == 0);
        REQUIRE(p.green() == 0);
        REQUIRE(p.blue() == 0);
        REQUIRE(p.alpha() == 0);

        REQUIRE(p.parse("#1234"));
        REQUIRE(p.red() == 0x11);
        REQUIRE(p.green() == 0x22);
        REQUIRE(p.blue() == 0x33);
        REQUIRE(p.alpha() == 0x44);
    }
}
