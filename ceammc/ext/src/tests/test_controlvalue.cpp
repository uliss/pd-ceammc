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
#include "ceammc_controlvalue.h"

using namespace ceammc;

TEST_CASE("ControlValue", "[ceammc::ControlValue]")
{
    ControlValue v1;
    REQUIRE(v1.isNone());
    REQUIRE(v1.type() == ControlValue::NONE);

    ControlValue v2(1.f);
    REQUIRE(v2.isFloat());

    ControlValue v3(gensym("a"));
    REQUIRE(v3.isSymbol());

    t_atom atoms[2];
    SETFLOAT(&atoms[0], 1);
    SETSYMBOL(&atoms[1], gensym("b"));
    ControlValue v4(2, &atoms[0]);
    REQUIRE(v4.isList());

    ControlValue v5(gensym("c"), 2, &atoms[0]);
    REQUIRE(v5.isAny());

    SECTION("compare")
    {
        REQUIRE(v1 == v1);
        REQUIRE(v2 == v2);
        REQUIRE(v3 == v3);
        REQUIRE(v4 == v4);
        REQUIRE(v5 == v5);

        REQUIRE(v1 != v2);
        REQUIRE(v1 != v3);
        REQUIRE(v1 != v4);
        REQUIRE_FALSE(v1 == v5);

        REQUIRE(v2 != v3);
        REQUIRE(v2 != v4);
        REQUIRE(v2 != v5);

        REQUIRE(v3 != v4);
        REQUIRE(v3 != v5);

        REQUIRE(v4 != v5);
    }

    SECTION("compare2")
    {
        REQUIRE(v2 == ControlValue(1.f));
        REQUIRE(v2 != ControlValue(1.0001f));

        REQUIRE(v3 == ControlValue(gensym("a")));
        REQUIRE(v3 != ControlValue(gensym("b")));

        AtomList l1;
        REQUIRE(v4 != ControlValue(l1));
        l1.append(1);
        l1.append(gensym("b"));
        REQUIRE(v4 == ControlValue(l1));

        REQUIRE(v5 != ControlValue(gensym("a"), l1));
        REQUIRE(v5 != ControlValue(gensym("c"), AtomList()));
        REQUIRE(v5 == ControlValue(gensym("c"), l1));

        AtomList l2;
        l2.append(gensym("a"));
        l2.append(gensym("b"));
        l2.append(gensym("c"));

        ControlValue c1(l2);
        ControlValue c2(gensym("a"), l2);
        REQUIRE(c1 != c2);
    }

    SECTION("set")
    {
        v1.setFloat(1.f);
        REQUIRE(v1.isFloat());
        v1.setSymbol(gensym("a"));
        REQUIRE(v1.isSymbol());
        v1.setList(AtomList());
        REQUIRE(v1.isList());
        v1.setAny(gensym("b"), AtomList());
        REQUIRE(v1.isAny());
        v1.setList(2, &atoms[0]);
        REQUIRE(v1.isList());
        v1.setAny(gensym("c"), 2, &atoms[0]);
        REQUIRE(v1.isAny());
    }
}
