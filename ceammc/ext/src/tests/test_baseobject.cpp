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

#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_object.h"

using namespace ceammc;

class EXT_A : public BaseObject {
public:
    EXT_A(const PdArgs& a)
        : BaseObject(a)
    {
        createProperty(new FloatProperty("@a", -1));
    }
};

class EXT_B : public EXT_A {
public:
    EXT_B(const PdArgs& a)
        : EXT_A(a)
    {
        createProperty(new FloatProperty("@b", -2));
    }
};

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

    SECTION("test check args")
    {
        SECTION("single")
        {
            BaseObject b(PdArgs(AtomList(), gensym("testname"), 0));
            REQUIRE(b.checkArg(A(10), BaseObject::ARG_FLOAT));
            REQUIRE(b.checkArg(A(10), BaseObject::ARG_INT));
            REQUIRE(b.checkArg(A(10), BaseObject::ARG_NATURAL));
            REQUIRE(b.checkArg(A(1), BaseObject::ARG_BOOL));
            REQUIRE(b.checkArg(A(0.0f), BaseObject::ARG_BOOL));
            REQUIRE_FALSE(b.checkArg(A(10), BaseObject::ARG_SYMBOL));
            REQUIRE_FALSE(b.checkArg(A(10), BaseObject::ARG_PROPERTY));
            REQUIRE_FALSE(b.checkArg(A(10), BaseObject::ARG_SNONPROPERTY));
            REQUIRE_FALSE(b.checkArg(A(10), BaseObject::ARG_BOOL));
            REQUIRE_FALSE(b.checkArg(A(0.1), BaseObject::ARG_BOOL));

            REQUIRE(b.checkArg(A(1.1f), BaseObject::ARG_FLOAT));
            REQUIRE_FALSE(b.checkArg(A(1.1f), BaseObject::ARG_INT));
            REQUIRE_FALSE(b.checkArg(A(1.1f), BaseObject::ARG_NATURAL));
            REQUIRE_FALSE(b.checkArg(A(1.1f), BaseObject::ARG_SYMBOL));
            REQUIRE_FALSE(b.checkArg(A(1.1f), BaseObject::ARG_PROPERTY));
            REQUIRE_FALSE(b.checkArg(A(1.1f), BaseObject::ARG_SNONPROPERTY));

            REQUIRE(b.checkArg(A(-10), BaseObject::ARG_FLOAT));
            REQUIRE(b.checkArg(A(-10), BaseObject::ARG_INT));
            REQUIRE_FALSE(b.checkArg(A(-10), BaseObject::ARG_NATURAL));
            REQUIRE_FALSE(b.checkArg(A(-10), BaseObject::ARG_SYMBOL));
            REQUIRE_FALSE(b.checkArg(A(-10), BaseObject::ARG_PROPERTY));
            REQUIRE_FALSE(b.checkArg(A(-10), BaseObject::ARG_SNONPROPERTY));

            REQUIRE_FALSE(b.checkArg(A("a"), BaseObject::ARG_FLOAT));
            REQUIRE_FALSE(b.checkArg(A("a"), BaseObject::ARG_INT));
            REQUIRE_FALSE(b.checkArg(A("a"), BaseObject::ARG_NATURAL));
            REQUIRE(b.checkArg(A("a"), BaseObject::ARG_SYMBOL));
            REQUIRE_FALSE(b.checkArg(A("a"), BaseObject::ARG_PROPERTY));
            REQUIRE(b.checkArg(A("a"), BaseObject::ARG_SNONPROPERTY));

            REQUIRE_FALSE(b.checkArg(A("@a"), BaseObject::ARG_FLOAT));
            REQUIRE_FALSE(b.checkArg(A("@a"), BaseObject::ARG_INT));
            REQUIRE_FALSE(b.checkArg(A("@a"), BaseObject::ARG_NATURAL));
            REQUIRE(b.checkArg(A("@a"), BaseObject::ARG_SYMBOL));
            REQUIRE(b.checkArg(A("@a"), BaseObject::ARG_PROPERTY));
            REQUIRE_FALSE(b.checkArg(A("@a"), BaseObject::ARG_SNONPROPERTY));
        }

        SECTION("lists")
        {
            BaseObject b(PdArgs(AtomList(), gensym("testname"), 0));
            REQUIRE(b.checkArgs(L1(2), BaseObject::ARG_FLOAT));
            REQUIRE(b.checkArgs(L1("a"), BaseObject::ARG_SYMBOL));
            REQUIRE_FALSE(b.checkArgs(L1(2), BaseObject::ARG_SYMBOL));
            REQUIRE_FALSE(b.checkArgs(L1("a"), BaseObject::ARG_FLOAT));

            REQUIRE(b.checkArgs(L2(2, 2), BaseObject::ARG_FLOAT, BaseObject::ARG_FLOAT));
            REQUIRE(b.checkArgs(L2(2, "a"), BaseObject::ARG_FLOAT, BaseObject::ARG_SYMBOL));
            REQUIRE(b.checkArgs(L3(2, "a", "@a"), BaseObject::ARG_FLOAT,
                BaseObject::ARG_SYMBOL, BaseObject::ARG_PROPERTY));
            REQUIRE(b.checkArgs(L4(-22, "a", "@a", 100), BaseObject::ARG_FLOAT,
                BaseObject::ARG_SYMBOL, BaseObject::ARG_PROPERTY, BaseObject::ARG_NATURAL));

            REQUIRE_FALSE(b.checkArgs(L4(-2, "a", "@a", 100.1f), BaseObject::ARG_NATURAL,
                BaseObject::ARG_SYMBOL, BaseObject::ARG_PROPERTY, BaseObject::ARG_FLOAT));
            REQUIRE_FALSE(b.checkArgs(L4(-2, "a", "@a", 100.1f), BaseObject::ARG_INT,
                BaseObject::ARG_INT, BaseObject::ARG_PROPERTY, BaseObject::ARG_FLOAT));
            REQUIRE_FALSE(b.checkArgs(L4(-2, "a", "@a", 100.1f), BaseObject::ARG_INT,
                BaseObject::ARG_SYMBOL, BaseObject::ARG_INT, BaseObject::ARG_FLOAT));
            REQUIRE_FALSE(b.checkArgs(L4(-2, "a", "@a", 100.1f), BaseObject::ARG_INT,
                BaseObject::ARG_INT, BaseObject::ARG_SNONPROPERTY, BaseObject::ARG_NATURAL));
        }
    }

    SECTION("parseProperties")
    {
        SECTION("only props")
        {
            BaseObject b(PdArgs(L5("@p1", 1, "@p2", 2, 3), gensym("testname"), 0));
            REQUIRE(b.positionalArguments() == AtomList());

            b.createProperty(new FloatProperty("@p1", -1));
            b.createProperty(new ListProperty("@p2"));
            b.createProperty(new FloatProperty("@p3", -1));
            b.parseProperties();

            REQUIRE_PROPERTY(b, @p1, 1);
            REQUIRE(b.hasProperty(gensym("@p2")));
            REQUIRE(b.property(gensym("@p2"))->get() == L2(2, 3));
            REQUIRE_PROPERTY(b, @p3, -1);

            REQUIRE(b.positionalArguments().empty());
        }

        SECTION("only raw args")
        {
            BaseObject b(PdArgs(L5(1, 2, "a", "b", "c"), gensym("testname"), 0));
            REQUIRE(b.positionalArguments() == L5(1, 2, "a", "b", "c"));

            b.createProperty(new FloatProperty("@p1", -1));
            b.createProperty(new ListProperty("@p2"));
            b.createProperty(new FloatProperty("@p3", -1));
            b.parseProperties();

            REQUIRE_PROPERTY(b, @p1, -1);
            REQUIRE(b.hasProperty(gensym("@p2")));
            REQUIRE(b.property(gensym("@p2"))->get() == AtomList());
            REQUIRE_PROPERTY(b, @p3, -1);

            REQUIRE(b.positionalArguments() == L5(1, 2, "a", "b", "c"));
        }

        SECTION("props and raw args")
        {
            BaseObject b(PdArgs(L5(1, 2, "@p1", "@p2", "c"), gensym("testname"), 0));
            b.createProperty(new FloatProperty("@p1", -1));
            b.createProperty(new ListProperty("@p2"));
            b.parseProperties();

            REQUIRE_PROPERTY(b, @p1, -1);
            REQUIRE(b.hasProperty(gensym("@p2")));
            REQUIRE(b.property(gensym("@p2"))->get() == L1("c"));

            REQUIRE(b.positionalArguments() == L2(1, 2));

            b.parseProperties();
            REQUIRE(b.positionalArguments() == L2(1, 2));
        }
    }

    SECTION("inheritance")
    {
        {
            EXT_B b(PdArgs(AtomList(), gensym("ext.b"), 0));
            b.parseProperties();
            REQUIRE_PROPERTY(b, @a, -1);
            REQUIRE_PROPERTY(b, @b, -2);

            REQUIRE(b.positionalArguments().empty());

            REQUIRE(b.positionalFloatArgument(0, -1) == -1);
            REQUIRE(b.positionalFloatArgument(100, -1) == -1);
            REQUIRE(b.positionalFloatArgument(-100, -1) == -1);

            REQUIRE(b.positionalSymbolArgument(0, 0) == 0);
            REQUIRE(b.positionalSymbolArgument(10, 0) == 0);
            REQUIRE(b.positionalSymbolArgument(-10, 0) == 0);
        }

        {
            EXT_B b(PdArgs(L2("@a", 100), gensym("ext.b"), 0));
            b.parseProperties();
            REQUIRE_PROPERTY(b, @a, 100);
            REQUIRE_PROPERTY(b, @b, -2);

            REQUIRE(b.positionalArguments().empty());

            REQUIRE(b.positionalFloatArgument(0, -1) == -1);
            REQUIRE(b.positionalFloatArgument(100, -1) == -1);
            REQUIRE(b.positionalFloatArgument(-100, -1) == -1);

            REQUIRE(b.positionalSymbolArgument(0, 0) == 0);
            REQUIRE(b.positionalSymbolArgument(10, 0) == 0);
            REQUIRE(b.positionalSymbolArgument(-10, 0) == 0);
        }

        {
            EXT_B b(PdArgs(L2("@b", 200), gensym("ext.b"), 0));
            b.parseProperties();
            REQUIRE_PROPERTY(b, @a, -1);
            REQUIRE_PROPERTY(b, @b, 200);

            REQUIRE(b.positionalArguments().empty());

            REQUIRE(b.positionalFloatArgument(0, -1) == -1);
            REQUIRE(b.positionalFloatArgument(100, -1) == -1);
            REQUIRE(b.positionalFloatArgument(-100, -1) == -1);

            REQUIRE(b.positionalSymbolArgument(0, 0) == 0);
            REQUIRE(b.positionalSymbolArgument(10, 0) == 0);
            REQUIRE(b.positionalSymbolArgument(-10, 0) == 0);
        }

        {
            EXT_B b(PdArgs(L4("@b", 200, "@a", -100), gensym("ext.b"), 0));
            b.parseProperties();
            REQUIRE_PROPERTY(b, @a, -100);
            REQUIRE_PROPERTY(b, @b, 200);

            REQUIRE(b.positionalArguments().empty());

            REQUIRE(b.positionalFloatArgument(0, -1) == -1);
            REQUIRE(b.positionalFloatArgument(100, -1) == -1);
            REQUIRE(b.positionalFloatArgument(-100, -1) == -1);

            REQUIRE(b.positionalSymbolArgument(0, 0) == 0);
            REQUIRE(b.positionalSymbolArgument(10, 0) == 0);
            REQUIRE(b.positionalSymbolArgument(-10, 0) == 0);
        }

        {
            EXT_B b(PdArgs(L6(1, 2, "@b", 200, "@a", -100), gensym("ext.b"), 0));
            b.parseProperties();
            REQUIRE_PROPERTY(b, @a, -100);
            REQUIRE_PROPERTY(b, @b, 200);

            REQUIRE(b.positionalArguments() == L2(1, 2));

            REQUIRE(b.positionalFloatArgument(0, -1) == 1);
            REQUIRE(b.positionalFloatArgument(1, -1) == 2);
            REQUIRE(b.positionalFloatArgument(2, -1) == -1);
            REQUIRE(b.positionalFloatArgument(-1, -100) == -100);

            REQUIRE(b.positionalSymbolArgument(0, 0) == 0);
            REQUIRE(b.positionalSymbolArgument(10, 0) == 0);
            REQUIRE(b.positionalSymbolArgument(-10, 0) == 0);

            REQUIRE(b.positionalArgument(0) == A(1));
            REQUIRE(b.positionalArgument(1) == A(2));
            REQUIRE(b.positionalArgument(2).isNone());
            REQUIRE(b.positionalArgument(3).isNone());

            REQUIRE(b.positionalSymbolArgument(10, 0) == 0);
            REQUIRE(b.positionalSymbolArgument(-10, 0) == 0);
        }

        {
            EXT_B b(PdArgs(L6("first", "second", "@b", 200, "@a", -100), gensym("ext.b"), 0));
            b.parseProperties();
            REQUIRE_PROPERTY(b, @a, -100);
            REQUIRE_PROPERTY(b, @b, 200);

            REQUIRE(b.positionalFloatArgument(0) == 0.f);
            REQUIRE(b.positionalFloatArgument(1) == 0.f);
            REQUIRE(b.positionalFloatArgument(2) == 0.f);

            REQUIRE(b.positionalSymbolArgument(0, 0) == S("first"));
            REQUIRE(b.positionalSymbolArgument(1, 0) == S("second"));
            REQUIRE(b.positionalSymbolArgument(2, 0) == 0);
        }
    }
}
