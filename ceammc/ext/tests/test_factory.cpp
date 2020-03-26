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

#include "catch.hpp"
#include "ceammc_factory.h"
#include "ceammc_pd.h"
#include "test_base.h"

using namespace ceammc;

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
        addProperty(new FloatProperty("@test_prop", -1));
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

class TestListMethodClass : public TestClass {
    int method_called_;
    t_symbol* sel_;
    AtomList args_;

public:
    TestListMethodClass(const PdArgs& a)
        : TestClass(a)
        , method_called_(0)
        , sel_(nullptr)
    {
    }

    int methodCalled() const { return method_called_; }
    t_symbol* methodSel() { return sel_; }
    const AtomList& methodArgs() const { return args_; }

    void m_method(t_symbol* sel, const AtomList& args)
    {
        method_called_++;
        args_ = args;
        sel_ = sel;
    }
};

template <typename T>
class PdExternalT : public ceammc::pd::External {
public:
    PdExternalT(const char* name, const AtomList& args = AtomList())
        : ceammc::pd::External(name, args)
    {
    }

    T* operator->()
    {
        return reinterpret_cast<PdObject<T>*>(object())->impl;
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

        PdExternal* ext = reinterpret_cast<PdExternal*>(f.createObject(gensym("test.new"), 2, args));

        REQUIRE(ext != 0);
        REQUIRE(ext->impl != 0);
        REQUIRE(ext->impl->owner() == &ext->pd_obj);
        REQUIRE(ext->impl->className() == gensym("test.new"));
        REQUIRE(ext->impl->classPointer() == ext->pd_obj.te_g.g_pd);
        REQUIRE(ext->impl->positionalArguments() == LA(2, "a"));
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
        PdExternal* ext = reinterpret_cast<PdExternal*>(f.createObject(name, 0, 0));
        REQUIRE(ext == 0);
        REQUIRE(destructor_called);
    }

    SECTION("arguments")
    {

        typedef PdObject<TestClass> PdExternal;

        test_reset();
        ObjectFactory<TestClass> f("test.new");
        f.setApiVersion(23);
        f.setCategory("test");
        f.setDeprecated();
        f.setDescription("test.new object");
        f.setUseInstead("test.new2");
        f.setSinceVersion(0, 2);

        t_atom args[4];
        SETFLOAT(&args[0], 2);
        SETSYMBOL(&args[1], gensym("a"));
        SETSYMBOL(&args[2], gensym("@test_prop"));
        SETFLOAT(&args[3], 33);

        PdExternal* ext = reinterpret_cast<PdExternal*>(f.createObject(gensym("test.new"), 4, args));

        REQUIRE(ext != 0);
        REQUIRE(ext->impl != 0);
        REQUIRE(ext->impl->owner() == &ext->pd_obj);
        REQUIRE(ext->impl->className() == gensym("test.new"));
        REQUIRE(ext->impl->positionalArguments() == LA(2, "a"));
        REQUIRE_PROPERTY((*ext->impl), @test_prop, 33);

        pd_free(&ext->pd_obj.te_g.g_pd);

        REQUIRE(destructor_called);
    }

    SECTION("list method")
    {
        test::pdPrintToStdError(true);

        ObjectFactory<TestListMethodClass> f("test.list_method");
        f.addAuthor("a1");
        f.addAuthor("a2");
        f.addAlias("test.lm");
        f.addAlias("test.lm2");

        typedef PdExternalT<TestListMethodClass> External;
        External t("test.list_method");
        REQUIRE(!t.isNull());
        REQUIRE(t->methodCalled() == 0);

        t.sendMessage(gensym("msg"), LA(1, 2, 3));
        REQUIRE(t->methodCalled() == 0);

        f.addMethod("msg", &TestListMethodClass::m_method);
        t.sendMessage(gensym("msg"), LA(1, 2, 3));
        REQUIRE(t->methodCalled() == 1);
        REQUIRE(t->methodSel() == gensym("msg"));
        REQUIRE(t->methodArgs() == LA(1, 2, 3));
    }

    SECTION("info")
    {
        ObjectInfoStorage::Info info;
        REQUIRE(ObjectInfoStorage::instance().find(SYM("test.new"), info));
        REQUIRE(info.api == 23);
        REQUIRE(info.deprecated);
        REQUIRE(info.since_version == ObjectInfoStorage::Version(0, 2));
        REQUIRE(info.dict["category"] == "test");
        REQUIRE(info.dict["description"] == "test.new object");
        REQUIRE(info.dict["use_instead"] == "test.new2");

        REQUIRE(ObjectInfoStorage::instance().find(SYM("test.list_method"), info));
        REQUIRE(info.authors == ObjectInfoStorage::AuthorList({ "a1", "a2" }));
        REQUIRE(info.authors == ObjectInfoStorage::AuthorList({ "a1", "a2" }));
        REQUIRE(info.aliases == ObjectInfoStorage::AliasList({ "test.lm", "test.lm2" }));
    }
}
