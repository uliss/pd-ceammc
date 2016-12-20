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
#include "ceammc_atomlist.h"

using namespace ceammc;

TEST_CASE("AtomList", "[ceammc::AtomList]")
{
    SECTION("construct")
    {
        AtomList l;
        REQUIRE(l.empty());
        REQUIRE(l.size() == 0);

        l.append(Atom(1.0));
        REQUIRE_FALSE(l.empty());
        REQUIRE(l.size() == 1);
    }

    SECTION("sort numeric")
    {
        AtomList l;
        l.append(Atom(10.0f));
        l.append(Atom(2.0f));
        l.append(Atom(-3.0f));

        FloatList r1 = l.asFloats();
        REQUIRE(r1[0] == 10.0f);
        REQUIRE(r1[1] == 2.0f);
        REQUIRE(r1[2] == -3.0f);

        l.sort();
        FloatList r2 = l.asFloats();
        REQUIRE(r2[0] == -3.0f);
        REQUIRE(r2[1] == 2.0f);
        REQUIRE(r2[2] == 10.0f);

        REQUIRE(r1 != r2);
    }

    SECTION("sort symbol")
    {
        AtomList l;
        l.append(Atom(gensym("b")));
        l.append(Atom(gensym("c")));
        l.append(Atom(gensym("a")));

        REQUIRE(l.at(0).asSymbol() == gensym("b"));
        REQUIRE(l.at(1).asSymbol() == gensym("c"));
        REQUIRE(l.at(2).asSymbol() == gensym("a"));

        l.sort();

        REQUIRE(l.at(0).asSymbol() == gensym("a"));
        REQUIRE(l.at(1).asSymbol() == gensym("b"));
        REQUIRE(l.at(2).asSymbol() == gensym("c"));

        l.sort();

        REQUIRE(l.at(0).asSymbol() == gensym("a"));
        REQUIRE(l.at(1).asSymbol() == gensym("b"));
        REQUIRE(l.at(2).asSymbol() == gensym("c"));
    }

    SECTION("sort mixed")
    {
        AtomList l;
        l.sort();

        l.append(Atom(gensym("a")));
        l.append(Atom(2.0));
        l.append(Atom(-3.0));
        l.append(Atom(gensym("b")));
        l.append(Atom(gensym("a")));
        REQUIRE(l.size() == 5);

        REQUIRE(l.at(0).asString() == "a");
        REQUIRE(l.at(1).asFloat() == 2.0f);
        REQUIRE(l.at(2).asFloat() == -3.0f);
        REQUIRE(l.at(3).asString() == "b");
        REQUIRE(l.at(4).asString() == "a");

        l.sort();

        REQUIRE(l.at(0).asFloat() == -3.0f);
        REQUIRE(l.at(1).asFloat() == 2.0f);
        REQUIRE(l.at(2).asString() == "a");
        REQUIRE(l.at(3).asString() == "a");
        REQUIRE(l.at(4).asString() == "b");
    }
}
