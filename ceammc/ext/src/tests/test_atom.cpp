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
#include "ceammc_atom.h"

using namespace ceammc;

TEST_CASE("Atom", "[ceammc::Atom]")
{
    SECTION("Atom contruct")
    {
        t_atom a;
        SETFLOAT(&a, 1.1f);
        Atom fatom(a);
        REQUIRE(fatom.isFloat());
        REQUIRE_FALSE(fatom.isSymbol());
        std::string s;
        REQUIRE_FALSE(fatom.getString(s));
        t_float v;
        REQUIRE(fatom.getFloat(&v));
        CHECK(v == 1.1f);
    }

    SECTION("Float atom tests")
    {
        Atom fatom(1.99f);
        REQUIRE(fatom.isFloat());
        t_float v;
        REQUIRE(fatom.getFloat(&v));
        REQUIRE_FALSE(fatom.getFloat(NULL));
        t_symbol* s;
        REQUIRE_FALSE(fatom.getSymbol(&s));
        REQUIRE_FALSE(fatom.getSymbol(NULL));
    }

    SECTION("Symbol atom tests")
    {
        Atom satom(gensym("test"));
        REQUIRE_FALSE(satom.isFloat());
        t_symbol* s;
        REQUIRE(satom.getSymbol(&s));
        REQUIRE_FALSE(satom.getSymbol(NULL));
        t_float v;
        REQUIRE_FALSE(satom.getFloat(NULL));
        REQUIRE_FALSE(satom.getFloat(&v));
        std::string str;
        REQUIRE(satom.getString(str));
        REQUIRE(str == "test");
    }
}
