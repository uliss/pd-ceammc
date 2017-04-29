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

#include <stdexcept>

#include "../base/debug_gensym.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc.hpp"
#include "ceammc_factory.h"

using namespace ceammc;

// strange link fix: do not remove!
static t_symbol* t = gensym("test");
static bool destructor_called = false;

static void test_reset()
{
    destructor_called = false;
}

static void test_throw()
{
    throw std::runtime_error("error");
}

class TestClass : public BaseObject {
public:
    TestClass(const PdArgs& a)
        : BaseObject(a)
    {
        createProperty(new FloatProperty("@test_prop", -1));
    }

    ~TestClass()
    {
        destructor_called = true;
    }
};

class ExceptionTest : public TestClass {
public:
    ExceptionTest(const PdArgs& a)
        : TestClass(a)
    {
        throw std::runtime_error("error");
    }
};

TEST_CASE("ceammc_factory", "[PureData]")
{
    obj_init();
    SECTION("new")
    {
        typedef PdObject<TestClass> PdExternal;

        test_reset();
        ObjectFactory<TestClass> f("test.new");

        t_atom args[2];
        SETFLOAT(&args[0], 2);
        SETSYMBOL(&args[1], gensym("a"));

        PdExternal* ext = reinterpret_cast<PdExternal*>(f.object_new(gensym("test.new"), 2, args));

        REQUIRE(ext != 0);
        REQUIRE(ext->impl != 0);
        REQUIRE(ext->impl->owner() == &ext->pd_obj);
        REQUIRE(ext->impl->className() == "test.new");
        REQUIRE(ext->impl->positionalArguments() == L2(2, "a"));
        REQUIRE_PROPERTY((*ext->impl), @test_prop, -1);

        pd_free(&ext->pd_obj.te_g.g_pd);

        REQUIRE(destructor_called);
    }

    SECTION("exception")
    {
        typedef PdObject<ExceptionTest> PdExternal;

        test_reset();
        ObjectFactory<ExceptionTest> f("test.exception");

        REQUIRE(destructor_called == false);
        t_symbol* name = gensym("test.new");
        REQUIRE_THROWS(test_throw());
        PdExternal* ext = reinterpret_cast<PdExternal*>(f.object_new(name, 0, 0));
        REQUIRE(ext == 0);
        REQUIRE(destructor_called);
    }

    SECTION("arguments")
    {

        typedef PdObject<TestClass> PdExternal;

        test_reset();
        ObjectFactory<TestClass> f("test.new");

        t_atom args[4];
        SETFLOAT(&args[0], 2);
        SETSYMBOL(&args[1], gensym("a"));
        SETSYMBOL(&args[2], gensym("@test_prop"));
        SETFLOAT(&args[3], 33);

        PdExternal* ext = reinterpret_cast<PdExternal*>(f.object_new(gensym("test.new"), 4, args));

        REQUIRE(ext != 0);
        REQUIRE(ext->impl != 0);
        REQUIRE(ext->impl->owner() == &ext->pd_obj);
        REQUIRE(ext->impl->className() == "test.new");
        REQUIRE(ext->impl->positionalArguments() == L2(2, "a"));
        REQUIRE_PROPERTY((*ext->impl), @test_prop, 33);

        pd_free(&ext->pd_obj.te_g.g_pd);

        REQUIRE(destructor_called);
    }
}
