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

#include "catch.hpp"
#include "ceammc_object.h"

using namespace ceammc;

TEST_CASE("BaseObject", "[ceammc::BaseObject]")
{
    SECTION("test prop key")
    {
        REQUIRE(BaseObject::tryGetPropKey(gensym("@")) == 0);
        REQUIRE(BaseObject::tryGetPropKey(gensym("@a")) == 0);
        REQUIRE(BaseObject::tryGetPropKey(gensym("@a?")) != 0);
        REQUIRE(BaseObject::tryGetPropKey(gensym("@a?")) == gensym("@a"));
        REQUIRE(BaseObject::tryGetPropKey(gensym("@?")) == gensym("@"));
    }

    SECTION("test construct")
    {
        BaseObject b(PdArgs(AtomList(), gensym("testname"), 0));
        REQUIRE(b.owner() == 0);
        REQUIRE(b.className() == "testname");
        REQUIRE(b.receive() == 0);
        REQUIRE(b.numOutlets() == 0);
        REQUIRE(b.numInlets() == 0);

        REQUIRE_FALSE(b.hasProperty("@?"));
        REQUIRE(b.property("@?") == 0);

        b.createProperty(new IntProperty("@int"));
        REQUIRE(b.hasProperty("@int"));
        REQUIRE(b.property("@int") != 0);
        REQUIRE(b.setProperty("@int", AtomList(2)));
        REQUIRE(b.property("@int")->get() == AtomList(2));

        b.createProperty(new IntProperty("@int_ro", -10, true));
        REQUIRE(b.hasProperty("@int_ro"));
        REQUIRE(b.property("@int_ro") != 0);
        REQUIRE(b.property("@int_ro")->readonly());
        REQUIRE_FALSE(b.setProperty("@int_ro", AtomList(2)));
    }
}
