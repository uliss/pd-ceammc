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
#include "ceammc_object.h"
#include "ceammc_pd.h"
#include "test_external.h"

#include "catch.hpp"

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

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

class EXT_C : public BaseObject {
public:
    EXT_C(const PdArgs& a)
        : BaseObject(a)
    {
        createProperty(new FloatProperty("@c", 101));
        createInlet();
        createOutlet();
    }

    void onInlet(size_t n, const AtomList& l)
    {
        last_inlet = n;
        last_inlet_data = l;
    }

public:
    size_t last_inlet;
    AtomList last_inlet_data;
};

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

template <class T>
class TestPdExternal : private pd::External {
    std::vector<ExternalOutput*> outs_;

public:
    TestPdExternal(const char* name, const AtomList& args = AtomList())
        : pd::External(name, args)
    {
        REQUIRE(object());

        for (size_t i = 0; i < numOutlets(); i++) {
            ExternalOutput* e = new ExternalOutput;
            connectTo(i, e->object(), 0);
            outs_.push_back(e);
        }
    }

    T* operator->()
    {
        PdObject<T>* obj = (PdObject<T>*)object();
        return obj->impl;
    }

    void call(const char* method, const AtomList& l = AtomList())
    {
        clearAll();
        sendMessage(gensym(method), l);
    }

    void bang()
    {
        clearAll();
        External::bang();
    }

    void call(const char* method, float f)
    {
        clearAll();
        sendMessage(gensym(method), AtomList(Atom(f)));
    }

    void send(float f)
    {
        clearAll();
        sendFloat(f);
    }

    void send(const AtomList& lst)
    {
        clearAll();
        sendList(lst);
    }

    bool hasOutputAt(size_t n) const
    {
        return !outs_.at(n)->msg().isNone();
    }

    bool hasOutput() const
    {
        for (size_t i = 0; i < outs_.size(); i++) {
            if (!outs_.at(i)->msg().isNone())
                return true;
        }

        return false;
    }

    bool isOutputListAt(size_t n) const
    {
        return outs_.at(n)->msg().isList();
    }

    bool isOutputAnyAt(size_t n) const
    {
        return outs_.at(n)->msg().isAny();
    }

    bool isOutputFloatAt(size_t n) const
    {
        return outs_.at(n)->msg().isFloat();
    }

    bool isOutputSymbolAt(size_t n) const
    {
        return outs_.at(n)->msg().isSymbol();
    }

    bool isOutputBangAt(size_t n) const
    {
        return outs_.at(n)->msg().isBang();
    }

    AtomList outputListAt(size_t n) const
    {
        return outs_.at(n)->msg().listValue();
    }

    AtomList outputAnyAt(size_t n) const
    {
        return outs_.at(n)->msg().anyValue();
    }

    float outputFloatAt(size_t n) const
    {
        return outs_.at(n)->msg().atomValue().asFloat();
    }

    Atom outputAtomAt(size_t n) const
    {
        return outs_.at(n)->msg().atomValue();
    }

    t_symbol* outputSymbolAt(size_t n) const
    {
        return outs_.at(n)->msg().atomValue().asSymbol();
    }

    void clearAll()
    {
        for (size_t i = 0; i < outs_.size(); i++)
            outs_[i]->reset();
    }

    void clearAt(size_t n)
    {
        outs_.at(n)->reset();
    }

    ~TestPdExternal()
    {
        for (size_t i = 0; i < outs_.size(); i++)
            delete outs_[i];
    }
};

TEST_CASE("BaseObject", "[ceammc::BaseObject]")
{
    ExternalOutput::setup();

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
        REQUIRE(b.canvas() != 0);
        REQUIRE(b.rootCanvas() != 0);

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
            REQUIRE(b.checkArg(A(0.0f), BaseObject::ARG_BYTE));
            REQUIRE(b.checkArg(A(1), BaseObject::ARG_BYTE));
            REQUIRE(b.checkArg(A(255), BaseObject::ARG_BYTE));
            REQUIRE_FALSE(b.checkArg(A(10), BaseObject::ARG_SYMBOL));
            REQUIRE_FALSE(b.checkArg(A(10), BaseObject::ARG_PROPERTY));
            REQUIRE_FALSE(b.checkArg(A(10), BaseObject::ARG_SNONPROPERTY));
            REQUIRE_FALSE(b.checkArg(A(10), BaseObject::ARG_BOOL));
            REQUIRE_FALSE(b.checkArg(A(0.1), BaseObject::ARG_BOOL));
            REQUIRE_FALSE(b.checkArg(A("ABC"), BaseObject::ARG_BOOL));
            REQUIRE_FALSE(b.checkArg(A(-1), BaseObject::ARG_BYTE));
            REQUIRE_FALSE(b.checkArg(A(256), BaseObject::ARG_BYTE));
            REQUIRE_FALSE(b.checkArg(A(1000), BaseObject::ARG_BYTE));
            REQUIRE_FALSE(b.checkArg(A("ABC"), BaseObject::ARG_BYTE));

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

    SECTION("isAbsolutePath")
    {
        REQUIRE_FALSE(BaseObject::isAbsolutePath(""));
        REQUIRE_FALSE(BaseObject::isAbsolutePath("file"));
        REQUIRE_FALSE(BaseObject::isAbsolutePath(" test"));
        REQUIRE(BaseObject::isAbsolutePath("~/test"));
        REQUIRE(BaseObject::isAbsolutePath("/test"));
        REQUIRE(BaseObject::isAbsolutePath("/"));

#ifdef _WIN32
        REQUIRE(BaseObject::isAbsolutePath("%"));
        REQUIRE(BaseObject::isAbsolutePath("C://"));
        REQUIRE(BaseObject::isAbsolutePath("D://data"));
#endif
    }

    SECTION("findInStdPaths")
    {
        BaseObject b1(PdArgs(AtomList(), gensym("testname"), 0));
        REQUIRE(b1.canvas());
        REQUIRE(b1.findInStdPaths("test") == "");
        REQUIRE(b1.rootCanvas() != 0);

        CanvasPtr cnv1 = PureData::instance().createTopCanvas("/dir/file.pd");

        BaseObject b2(PdArgs(AtomList(), gensym("test"), 0));
        REQUIRE(b2.canvas() == cnv1->pd_canvas());
        REQUIRE(b2.findInStdPaths("test") == "");
        REQUIRE(b2.rootCanvas() == b2.canvas());

        CanvasPtr cnv2 = PureData::instance().createTopCanvas(TEST_DATA_DIR "/test.pd");

        BaseObject b3(PdArgs(AtomList(), gensym("mtof"), 0));
        REQUIRE(b3.canvas() == cnv2->pd_canvas());
        REQUIRE(b3.findInStdPaths("unknown") == "");
        REQUIRE(b3.findInStdPaths("snd_mono_48k.wav") == TEST_DATA_DIR "/snd_mono_48k.wav");
    }

    SECTION("outletAt")
    {
        BaseObject b1(PdArgs(AtomList(), gensym("test"), 0));
        REQUIRE(b1.outletAt(0) == 0);
    }

    SECTION("createProperty")
    {
        BaseObject b(PdArgs(AtomList(), gensym("test"), 0));
        b.createProperty(new IntProperty("int1"));
        b.createProperty(new IntProperty("int1", 10));
        REQUIRE(b.property("int1")->get() == L1(10));

        Property* p = new BoolProperty("bool1", true);
        b.createProperty(p);
        b.createProperty(p);
    }

    SECTION("realOutput")
    {
        obj_init();
        pd_init();

        {
            ObjectFactory<EXT_C> obj("ext.c");
        }

        TestPdExternal<EXT_C> t("ext.c");
        REQUIRE(t->outletAt(0));
        REQUIRE(!t->outletAt(1));

        t->bangTo(100);
        REQUIRE(!t.hasOutput());
        t->bangTo(0);
        REQUIRE(t.hasOutputAt(0));

        t.clearAll();
        t->floatTo(0, 10);
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(0) == 10);

        t.clearAll();
        t->floatTo(1, 10);
        REQUIRE_FALSE(t.hasOutput());

        t.clearAll();
        t->symbolTo(0, gensym("ABC"));
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputSymbolAt(0) == gensym("ABC"));

        t.clearAll();
        t->symbolTo(1, gensym("ABC"));
        REQUIRE(!t.hasOutput());

        t.clearAll();
        t->listTo(0, L3(1, 2, 3));
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == L3(1, 2, 3));

        t.clearAll();
        t->listTo(1, L3(1, 2, 3));
        REQUIRE(!t.hasOutput());

        t.clearAll();
        t->atomTo(0, Atom(12));
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(0) == 12);

        t.clearAll();
        t->atomTo(0, Atom());
        REQUIRE(!t.hasOutput());

        t.clearAll();
        t->atomTo(1, Atom());
        REQUIRE(!t.hasOutput());

        t.clearAll();
        t->atomTo(0, Atom(gensym("CDE")));
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputSymbolAt(0) == gensym("CDE"));

        t.clearAll();
        t->anyTo(0, L4("any", 1, 2, 3));
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputAnyAt(0) == L4("any", 1, 2, 3));

        t.clearAll();
        t->anyTo(1, L4("any", 1, 2, 3));
        REQUIRE(!t.hasOutput());

        t.clearAll();
        t->messageTo(0, Message(22));
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(0) == 22);

        t.clearAll();
        t->messageTo(1, Message(22));
        REQUIRE(!t.hasOutput());

        t.clearAll();
        t->anyTo(0, gensym("B"), L3(1, 2, 3));
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputAnyAt(0) == L4("B", 1, 2, 3));

        t.clearAll();
        t->anyTo(1, gensym("B"), L3(1, 2, 3));
        REQUIRE(!t.hasOutput());

        t.clearAll();
        t->anyTo(0, gensym("B"), A(23));
        REQUIRE(t.hasOutput());
        REQUIRE(t.outputAnyAt(0) == L2("B", 23));

        t.clearAll();
        t->anyTo(1, gensym("B"), A(23));
        REQUIRE(!t.hasOutput());

        SECTION("process inlets")
        {
            REQUIRE(!t->processAnyInlets(gensym("ABC"), AtomList()));
            REQUIRE(!t->processAnyInlets(gensym("_"), AtomList()));
            REQUIRE(t->processAnyInlets(gensym("_inlet0"), L1(123)));
            REQUIRE(t->last_inlet == 1);
            REQUIRE(t->last_inlet_data == L1(123));
        }

        SECTION("process props")
        {
            REQUIRE(!t->processAnyProps(gensym("???"), AtomList()));
            REQUIRE(!t->processAnyProps(gensym("@abc"), AtomList()));

            t.clearAll();
            REQUIRE(t->processAnyProps(gensym("@c?"), AtomList()));
            REQUIRE(t.hasOutput());
            REQUIRE(t.outputAnyAt(0) == L2("@c", 101));

            t.clearAll();
            REQUIRE(t->processAnyProps(gensym("@c"), AtomList(200)));
            REQUIRE(!t.hasOutput());

            t.clearAll();
            REQUIRE(t->processAnyProps(gensym("@c?"), AtomList()));
            REQUIRE(t.hasOutput());
            REQUIRE(t.outputAnyAt(0) == L2("@c", 200));
        }
    }
}
