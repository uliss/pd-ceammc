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
#include "datatype_mlist.h"
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
        , f(0)
    {
        addProperty(new FloatProperty("@test_prop", -1))->setArgIndex(0);
        addProperty(new ListProperty("@l"))->setArgIndex(1);
    }

    void onFloat(t_float v) override
    {
        f = v;
    }

    void onList(const AtomListView& lv) override
    {
        l = lv;
    }

    void parseProperties() override
    {
        std::cerr << "Parse properties....\n";
        BaseObject::parseProperties();
    }

    ~TestClass()
    {
        destructor_called = true;
    }

public:
    t_float f;
    AtomList l;
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

    void m_method(t_symbol* sel, const AtomListView& args)
    {
        method_called_++;
        args_ = args;
        sel_ = sel;
    }
};

class TestDataClass : public TestClass {
public:
    TestDataClass(const PdArgs& a)
        : TestClass(a)
        , d_int(0)
        , d_str("")
    {
    }

    void onDataT(const DataAtom<IntData>& d) { d_int = d; }
    void onDataT(const DataAtom<StrData>& d) { d_str = d; }
    void onData(const Atom& d) { d_any = d; }

public:
    DataAtom<IntData> d_int;
    DataAtom<StrData> d_str;
    Atom d_any;
};

class TestProperties : public TestClass {
public:
    TestProperties(const PdArgs& a)
        : TestClass(a)
    {
    }

public:
    SymbolProperty* sym_;
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

template <class T>
class ExtT {
    PdObject<T>* ext_ = 0;

public:
    ExtT(ObjectFactory<T>& f, const char* name, const AtomList& args)
    {
        ext_ = reinterpret_cast<PdObject<T>*>(f.createObject(gensym(name), args.size(), args.toPdData()));
    }

    ~ExtT() { pd_free(&ext_->pd_obj.te_g.g_pd); }

    T* impl() { return ext_->impl; }
    T* operator->() { return ext_->impl; }
};

TEST_CASE("ceammc_factory", "[core]")
{
    SECTION("new")
    {
        typedef PdObject<TestClass> PdExternal;

        test_reset();
        ObjectFactory<TestClass> f("test.new");
        f.parsePosProps(false);

        REQUIRE(f.checkFlag(OBJECT_FACTORY_PARSE_ARGS));
        REQUIRE(f.checkFlag(OBJECT_FACTORY_PARSE_PROPS));
        REQUIRE_FALSE(f.checkFlag(OBJECT_FACTORY_PARSE_POS_PROPS));

        t_atom args[2];
        SETFLOAT(&args[0], 2);
        SETSYMBOL(&args[1], gensym("a"));

        PdExternal* ext = reinterpret_cast<PdExternal*>(f.createObject(gensym("test.new"), 2, args));

        REQUIRE(ext != 0);
        REQUIRE(ext->impl != 0);
        REQUIRE(ext->impl->owner() == &ext->pd_obj);
        REQUIRE(ext->impl->className() == gensym("test.new"));
        REQUIRE(ext->impl->classPointer() == ext->pd_obj.te_g.g_pd);
        REQUIRE(ext->impl->args() == LA(2, "a"));
        REQUIRE(ext->impl->parsedPosArgs() == LA(2, "a"));

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

        AtomList args(2, "a", "@test_prop", 33);
        PdExternal* ext = reinterpret_cast<PdExternal*>(f.createObject(gensym("test.new"), args.size(), args.toPdData()));

        REQUIRE(ext != 0);
        REQUIRE(ext->impl != 0);
        REQUIRE(ext->impl->owner() == &ext->pd_obj);
        REQUIRE(ext->impl->className() == gensym("test.new"));
        REQUIRE(ext->impl->parsedPosArgs() == LA(2, "a"));
        REQUIRE(ext->impl->args() == LA(2, "a", "@test_prop", 33));
        REQUIRE_PROPERTY((*ext->impl), @test_prop, 33);

        pd_free(&ext->pd_obj.te_g.g_pd);

        // do not parse properties
        f.parseProps(false);
        f.parsePosProps(false);
        PdExternal* ext1 = reinterpret_cast<PdExternal*>(f.createObject(gensym("test.new"), args.size(), args.toPdData()));
        REQUIRE_PROPERTY((*ext1->impl), @test_prop, -1);
        pd_free(&ext1->pd_obj.te_g.g_pd);

        REQUIRE(destructor_called);
    }

    SECTION("parse")
    {
        SECTION("none")
        {
            test_reset();
            ObjectFactory<TestClass> f("test.new");
            f.parseArgs(false);
            f.parseProps(false);

            ExtT<TestClass> t(f, "test.new", LA(2, "a", "@test_prop", 33));

            REQUIRE(t->parsedPosArgs() == L());
            REQUIRE_PROPERTY((*t.impl()), @test_prop, -1);
        }

        SECTION("none")
        {
            test_reset();
            ObjectFactory<TestClass> f("test.new");
            f.parseArgs(true);
            f.parseProps(true);
            f.parseArgsMode(PdArgs::PARSE_NONE);
            f.parsePropsMode(PdArgs::PARSE_NONE);

            ExtT<TestClass> t(f, "test.new", LA(2, "a", "@test_prop", 33));

            REQUIRE(t->parsedPosArgs() == L());
            REQUIRE_PROPERTY((*t.impl()), @test_prop, -1);
        }

        SECTION("args copy")
        {
            test_reset();
            ObjectFactory<TestClass> f("test.new");
            f.parseArgs(true);
            f.parseProps(false);
            f.parseArgsMode(PdArgs::PARSE_COPY);

            ExtT<TestClass> t(f, "test.new", LA(2, "a", "\"b", "c\"", "pi()", "@test_prop", 33));

            REQUIRE(t->parsedPosArgs() == LA(2, "a", "\"b", "c\"", "pi()"));
            REQUIRE_PROPERTY((*t.impl()), @test_prop, -1);
        }

        SECTION("args unquote")
        {
            test_reset();
            ObjectFactory<TestClass> f("test.new");
            f.parseArgs(true);
            f.parseProps(false);
            f.parseArgsMode(PdArgs::PARSE_UNQUOTE);

            ExtT<TestClass> t(f, "test.new", LA(2, "a", "\"b", "c\"", "pi()", "@test_prop", 33));

            REQUIRE(t->parsedPosArgs() == LA(2, "a", "b c", "pi()"));
            REQUIRE_PROPERTY((*t.impl()), @test_prop, -1);
        }

        SECTION("args unquote invalid")
        {
            test_reset();
            ObjectFactory<TestClass> f("test.new");
            f.parseArgs(true);
            f.parseProps(false);
            f.parseArgsMode(PdArgs::PARSE_UNQUOTE);

            ExtT<TestClass> t(f, "test.new", LA(2, "a", "\"b", "c", "@test_prop", 33));

            REQUIRE(t->parsedPosArgs() == LA(2, "a", "\"b", "c"));
            REQUIRE_PROPERTY((*t.impl()), @test_prop, -1);
        }

        SECTION("args expr")
        {
            test_reset();
            ObjectFactory<TestClass> f("test.new");
            f.parseArgs(true);
            f.parseProps(false);
            f.parseArgsMode(PdArgs::PARSE_EXPR);

            ExtT<TestClass> t(f, "test.new", LA(2, "pi()", "\"b", "c\"", "@test_prop", 33));

            REQUIRE(t->parsedPosArgs() == LAX(2, std::acos((t_float)-1), "b c"));
            REQUIRE_PROPERTY((*t.impl()), @test_prop, -1);
        }

        SECTION("props copy")
        {
            test_reset();
            ObjectFactory<TestClass> f("test.new");
            f.parseArgs(false);
            f.parseProps(true);
            f.parsePropsMode(PdArgs::PARSE_COPY);

            ExtT<TestClass> t(f, "test.new", LA(2, "a", "@test_prop", 33));

            REQUIRE(t->parsedPosArgs() == L());
            REQUIRE_PROPERTY((*t.impl()), @test_prop, 33);
        }

        SECTION("props copy invalid")
        {
            test_reset();
            ObjectFactory<TestClass> f("test.new");
            f.parseArgs(false);
            f.parseProps(true);
            f.parsePropsMode(PdArgs::PARSE_COPY);

            ExtT<TestClass> t(f, "test.new", LA(2, "a", "@test_prop", "pi()"));

            REQUIRE(t->parsedPosArgs() == L());
            REQUIRE_PROPERTY((*t.impl()), @test_prop, -1);
        }

        SECTION("props expr")
        {
            test_reset();
            ObjectFactory<TestClass> f("test.new");
            f.parseArgs(false);
            f.parseProps(true);
            f.parsePropsMode(PdArgs::PARSE_EXPR);

            ExtT<TestClass> t(f, "test.new", LA(2, "a", "@test_prop", "expr(2^4)"));

            REQUIRE(t->parsedPosArgs() == L());
            REQUIRE_PROPERTY((*t.impl()), @test_prop, 16);
        }

        SECTION("props pos, but no parse args")
        {
            test_reset();
            ObjectFactory<TestClass> f("test.new");
            f.parseArgs(false);
            f.parseProps(false);
            f.parsePosProps(true);
            f.parsePropsMode(PdArgs::PARSE_COPY);

            ExtT<TestClass> t(f, "test.new", LA(2, "a", "@test_prop", "expr(2^4)"));

            REQUIRE(t->parsedPosArgs() == L());
            REQUIRE_PROPERTY((*t.impl()), @test_prop, -1);
        }

        SECTION("props pos")
        {
            test_reset();
            ObjectFactory<TestClass> f("test.new");
            f.parseArgs(true);
            f.parseProps(false);
            f.parsePosProps(true);
            f.parsePropsMode(PdArgs::PARSE_COPY);

            ExtT<TestClass> t(f, "test.new", LA(2, "a", "@test_prop", "expr(2^4)"));

            REQUIRE(t->parsedPosArgs() == LA(2, "a"));
            REQUIRE_PROPERTY((*t.impl()), @test_prop, 2);
        }

        SECTION("props pos")
        {
            test_reset();
            ObjectFactory<TestClass> f("test.new");
            f.parseArgs(true);
            f.parseProps(true);
            f.parsePosProps(true);
            f.parsePropsMode(PdArgs::PARSE_EXPR);

            ExtT<TestClass> t(f, "test.new", LA(2, "a", "@test_prop", "expr(2^4)"));

            REQUIRE(t->parsedPosArgs() == LA(2, "a"));
            REQUIRE_PROPERTY((*t.impl()), @test_prop, 16);
        }
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

    SECTION("dataT")
    {
        using Factory = ObjectFactory<TestDataClass>;
        using External = PdExternalT<TestDataClass>;
        using Int = DataAtom<IntData>;
        using Str = DataAtom<StrData>;

        SECTION("IntData")
        {
            // create object that support only IntData
            Factory f("test.data0");
            f.processData<IntData>();

            External t("test.data0");
            REQUIRE(!t.isNull());
            REQUIRE(t->d_any.isNone());

            // mlist goes to common data
            t.sendList(MListAtom(1, 2, 3));

            REQUIRE(t->d_int->value() == 0);
            REQUIRE(t->d_str->get() == "");
            REQUIRE(t->d_any.isA<DataTypeMList>());
            REQUIRE(MListAtom(1, 2, 3) == t->d_any);

            // string goes to common data
            t.sendList(Str("abcde"));

            REQUIRE(t->d_int->value() == 0);
            REQUIRE(t->d_str->get() == "");
            REQUIRE(t->d_any.isA<StrData>());
            REQUIRE(Str("abcde") == t->d_any);

            // int goes to int data
            t.sendList(Int(1000));

            REQUIRE(t->d_int->value() == 1000);
            REQUIRE(t->d_str->get() == "");
            REQUIRE(t->d_any.isA<StrData>());
        }

        SECTION("IntData, StrData")
        {
            // create object that support only IntData
            Factory f("test.data0");
            f.processData<IntData, StrData>();

            External t("test.data0");
            REQUIRE(!t.isNull());
            REQUIRE(t->d_any.isNone());

            // mlist goes to common data
            t.sendList(MListAtom(1, 2, 3));

            REQUIRE(t->d_int->value() == 0);
            REQUIRE(t->d_str->get() == "");
            REQUIRE(t->d_any.isA<DataTypeMList>());
            REQUIRE(MListAtom(1, 2, 3) == t->d_any);

            // string goes to str data
            t.sendList(Str("abcde"));

            REQUIRE(t->d_int->value() == 0);
            REQUIRE(t->d_str->get() == "abcde");
            REQUIRE(t->d_any.isA<DataTypeMList>());

            // int goes to int data
            t.sendList(Int(1000));

            REQUIRE(t->d_int->value() == 1000);
            REQUIRE(t->d_str->get() == "abcde");
            REQUIRE(t->d_any.isA<DataTypeMList>());
        }

        SECTION("<>")
        {
            // create object that support only IntData
            Factory f("test.data0");
            f.processData();

            External t("test.data0");
            REQUIRE(!t.isNull());
            REQUIRE(t->d_any.isNone());

            // mlist goes to common data
            t.sendList(MListAtom(1, 2, 3));

            REQUIRE(t->d_int->value() == 0);
            REQUIRE(t->d_str->get() == "");
            REQUIRE(t->d_any.isA<DataTypeMList>());
            REQUIRE(MListAtom(1, 2, 3) == t->d_any);

            // string goes to common data
            t.sendList(Str("abcde"));

            REQUIRE(t->d_int->value() == 0);
            REQUIRE(t->d_str->get() == "");
            REQUIRE(Str("abcde") == t->d_any);

            // int goes to common data
            t.sendList(Int(1000));

            REQUIRE(t->d_int->value() == 0);
            REQUIRE(t->d_str->get() == "");
            REQUIRE(Int(1000) == t->d_any);
        }
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

    SECTION("default pd handlers")
    {
        using Factory = ObjectFactory<TestDataClass>;
        using External = PdExternalT<TestDataClass>;

        SECTION("default ceammc")
        {
            Factory f("test.data0");
            f.finalize();
            REQUIRE(f.classPointer()->c_floatmethod == (void*)Factory::processFloat);

            External t("test.data0");
            REQUIRE(t->f == 0);
            t.sendFloat(100);
            REQUIRE(t->f == 100);
        }

        SECTION("default ceammc")
        {
            Factory f("test.data0");
            f.useDefaultPdFloatFn();
            f.finalize();
            REQUIRE(f.classPointer()->c_floatmethod != (void*)Factory::processFloat);

            External t("test.data0");
            REQUIRE(t->f == 0);
            t.sendFloat(100);
            REQUIRE(t->f == 0);
            REQUIRE(t->l == LF(100));
        }
    }
}
