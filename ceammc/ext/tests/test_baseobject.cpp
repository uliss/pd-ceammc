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
#include "ceammc_object.h"
#include "ceammc_output.h"
#include "ceammc_pd.h"
#include "test_external.h"

#include "catch.hpp"

using namespace ceammc;

class EXT_A : public BaseObject {
public:
    EXT_A(const PdArgs& a)
        : BaseObject(a)
    {
        addProperty(new FloatProperty("@a", -1));
    }
};

class EXT_B : public EXT_A {
public:
    EXT_B(const PdArgs& a)
        : EXT_A(a)
    {
        addProperty(new FloatProperty("@b", -2));
    }
};

class EXT_C : public BaseObject {
public:
    EXT_C(const PdArgs& a)
        : BaseObject(a)
        , int_data(0)
    {
        addProperty(new FloatProperty("@c", 101))->setArgIndex(0);
        addProperty(new SizeTProperty("@d", 101))->setArgIndex(1);
        addProperty(new SymbolProperty("@s", gensym("empty")))->setArgIndex(2);
        addProperty(new ListProperty("@l", LF(-1)))->setArgIndex(3);

        createCbListProperty("@cb", []() -> AtomList { return { 1, 2, 3 }; });

        createInlet();
        createOutlet();
    }

    void onInlet(size_t n, const AtomListView& l)
    {
        last_inlet = n;
        last_inlet_data = l;
    }

    void onDataT(const DataAtom<IntData>& d)
    {
        int_data = *d;
    }

public:
    size_t last_inlet;
    AtomList last_inlet_data;
    IntData int_data;
};

static void setup_test_extc()
{
    ObjectFactory<EXT_C> obj("ext.c");
    obj.processData<IntData>();
}

class EXT_D : public BaseObject {
public:
    EXT_D(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onSymbol(t_symbol* s) override
    {
        auto path = findInStdPaths(s->s_name);
        symbolTo(0, gensym(path.c_str()));
    }
};

static void setup_test_extd()
{
    ObjectFactory<EXT_D> obj1("ext.d");
}

PD_TEST_TYPEDEF(EXT_C);
PD_TEST_MOD_INIT(test, extc);

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
        BaseObject b(PdArgs(L(), gensym("testname"), 0, gensym("testname")));
        REQUIRE(b.owner() == 0);
        REQUIRE(b.className() == gensym("testname"));
        REQUIRE(b.receive() == 0);
        REQUIRE(b.numOutlets() == 0);
        REQUIRE(b.numInlets() == 0);
        REQUIRE(b.canvas() != 0);
        REQUIRE(b.rootCanvas() != 0);

        REQUIRE_FALSE(b.hasProperty("@?"));
        REQUIRE(b.property("@?") == 0);

        b.addProperty(new IntProperty("@int"));
        REQUIRE(b.hasProperty("@int"));
        REQUIRE(b.property("@int") != 0);
        REQUIRE(b.setProperty("@int", LF(2)));
        REQUIRE(b.property("@int")->get() == LF(2));

        b.addProperty(new IntProperty("@int_ro", -10, PropValueAccess::READONLY));
        REQUIRE(b.hasProperty("@int_ro"));
        REQUIRE(b.property("@int_ro") != 0);
        REQUIRE(b.property("@int_ro")->isReadOnly());
        REQUIRE_FALSE(b.setProperty("@int_ro", LF(2)));

        auto cbf = b.createCbFloatProperty("@cbf", []() -> t_float { return 0.5; });
        REQUIRE(b.hasProperty("@cbf"));
        REQUIRE(b.property("@cbf")->isReadOnly());
        REQUIRE_PROPERTY(b, @cbf, 0.5);

        auto cbi = b.createCbIntProperty("@cbi", []() -> int { return -5; });
        REQUIRE(b.hasProperty("@cbi"));
        REQUIRE(b.property("@cbi") == cbi);
        REQUIRE(b.property("@cbi")->isReadOnly());
        REQUIRE_PROPERTY(b, @cbi, -5);

        auto cbs = b.createCbSymbolProperty("@cbs", []() -> t_symbol* { return gensym("ok"); });
        REQUIRE(b.hasProperty("@cbs"));
        REQUIRE(b.property("@cbs") == cbs);
        REQUIRE(b.property("@cbs")->isReadOnly());
        REQUIRE_PROPERTY(b, @cbs, "ok");
    }

    SECTION("test check args")
    {
        SECTION("single")
        {
            BaseObject b(PdArgs(L(), gensym("testname"), 0, gensym("testname")));
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
            BaseObject b(PdArgs(L(), gensym("testname"), 0, gensym("testname")));
            REQUIRE(b.checkArgs(LF(2), BaseObject::ARG_FLOAT));
            REQUIRE(b.checkArgs(LA("a"), BaseObject::ARG_SYMBOL));
            REQUIRE_FALSE(b.checkArgs(LF(2), BaseObject::ARG_SYMBOL));
            REQUIRE_FALSE(b.checkArgs(LA("a"), BaseObject::ARG_FLOAT));

            REQUIRE(b.checkArgs(LF(2, 2), BaseObject::ARG_FLOAT, BaseObject::ARG_FLOAT));
            REQUIRE(b.checkArgs(LA(2, "a"), BaseObject::ARG_FLOAT, BaseObject::ARG_SYMBOL));
            REQUIRE(b.checkArgs(LA(2, "a", "@a"), BaseObject::ARG_FLOAT,
                BaseObject::ARG_SYMBOL, BaseObject::ARG_PROPERTY));
            REQUIRE(b.checkArgs(LA(-22, "a", "@a", 100), BaseObject::ARG_FLOAT,
                BaseObject::ARG_SYMBOL, BaseObject::ARG_PROPERTY, BaseObject::ARG_NATURAL));

            REQUIRE_FALSE(b.checkArgs(LA(-2, "a", "@a", 100.1f), BaseObject::ARG_NATURAL,
                BaseObject::ARG_SYMBOL, BaseObject::ARG_PROPERTY, BaseObject::ARG_FLOAT));
            REQUIRE_FALSE(b.checkArgs(LA(-2, "a", "@a", 100.1f), BaseObject::ARG_INT,
                BaseObject::ARG_INT, BaseObject::ARG_PROPERTY, BaseObject::ARG_FLOAT));
            REQUIRE_FALSE(b.checkArgs(LA(-2, "a", "@a", 100.1f), BaseObject::ARG_INT,
                BaseObject::ARG_SYMBOL, BaseObject::ARG_INT, BaseObject::ARG_FLOAT));
            REQUIRE_FALSE(b.checkArgs(LA(-2, "a", "@a", 100.1f), BaseObject::ARG_INT,
                BaseObject::ARG_INT, BaseObject::ARG_SNONPROPERTY, BaseObject::ARG_NATURAL));
        }
    }

    SECTION("parseProperties")
    {
        SECTION("only props")
        {
            BaseObject b(PdArgs(LA("@p1", 1, "@p2", 2, 3), gensym("testname"), 0, gensym("testname")));
            REQUIRE(b.parsedPosArgs() == L());
            REQUIRE(b.unparsedPosArgs().isNull());
            // synthehic test
            REQUIRE(b.binbufArgs() == L());

            b.addProperty(new FloatProperty("@p1", -1));
            b.addProperty(new ListProperty("@p2"));
            b.addProperty(new FloatProperty("@p3", -1));
            b.parseProperties();

            REQUIRE_PROPERTY_FLOAT(b, @p1, 1);
            REQUIRE_PROPERTY_LIST(b, @p2, LF(2, 3));
            REQUIRE_PROPERTY(b, @p3, -1);

            REQUIRE(b.parsedPosArgs() == L());
        }

        SECTION("only raw args")
        {
            BaseObject b(PdArgs(LA(1, 2, "a", "b", "c"), gensym("testname"), 0, gensym("testname")));
            REQUIRE(b.parsedPosArgs() == LA(1, 2, "a", "b", "c"));
            REQUIRE(b.unparsedPosArgs() == LA(1, 2, "a", "b", "c"));
            // synthehic test
            REQUIRE(b.binbufArgs() == L());

            b.addProperty(new FloatProperty("@p1", -1));
            b.addProperty(new ListProperty("@p2"));
            b.addProperty(new FloatProperty("@p3", -1));
            b.parseProperties();

            REQUIRE_PROPERTY(b, @p1, -1);
            REQUIRE(b.hasProperty(gensym("@p2")));
            REQUIRE(b.property(gensym("@p2"))->get() == L());
            REQUIRE_PROPERTY(b, @p3, -1);

            REQUIRE(b.parsedPosArgs() == LA(1, 2, "a", "b", "c"));
            REQUIRE(b.unparsedPosArgs() == LA(1, 2, "a", "b", "c"));
        }

        SECTION("props and raw args")
        {
            BaseObject b(PdArgs(LA(1, 2, "@p1", "@p2", "c"), gensym("testname"), 0, gensym("testname")));
            REQUIRE(b.parsedPosArgs() == LF(1, 2));
            REQUIRE(b.unparsedPosArgs() == LF(1, 2));
            // synthehic test
            REQUIRE(b.binbufArgs() == L());

            b.addProperty(new FloatProperty("@p1", -1));
            b.addProperty(new ListProperty("@p2"));
            b.parseProperties();

            REQUIRE_PROPERTY(b, @p1, -1);
            REQUIRE(b.hasProperty(gensym("@p2")));
            REQUIRE(b.property(gensym("@p2"))->get() == LA("c"));

            REQUIRE(b.parsedPosArgs() == LF(1, 2));
            REQUIRE(b.unparsedPosArgs() == LF(1, 2));
        }

        SECTION("args, functions and quotes")
        {
            BaseObject b(PdArgs(LA(1, 2, "bs()", "\"a", "string\""), gensym("testname"), 0, gensym("testname")));
            REQUIRE(b.parsedPosArgs() == LA(1, 2, 64, "a string"));
            REQUIRE(b.unparsedPosArgs() == LA(1, 2, "bs()", "\"a", "string\""));
        }

        SECTION("args, functions and quotes")
        {
            BaseObject b(PdArgs(LA(1, 2, "expr(20*20)", "\"@a\""), gensym("testname"), 0, gensym("testname")));
            REQUIRE(b.parsedPosArgs() == LA(1, 2, 400, "@a"));
            REQUIRE(b.unparsedPosArgs() == LA(1, 2, "expr(20*20)", "\"@a\""));
        }
    }

    SECTION("inheritance")
    {
        SECTION("B")
        {
            EXT_B b(PdArgs(L(), gensym("ext.b"), 0, gensym("ext.b")));
            b.parseProperties();
            REQUIRE_PROPERTY(b, @a, -1);
            REQUIRE_PROPERTY(b, @b, -2);
        }

        {
            EXT_B b(PdArgs(LA("@a", 100), gensym("ext.b"), 0, gensym("ext.b")));
            b.parseProperties();
            REQUIRE_PROPERTY(b, @a, 100);
            REQUIRE_PROPERTY(b, @b, -2);
        }

        {
            EXT_B b(PdArgs(LA("@b", 200), gensym("ext.b"), 0, gensym("ext.b")));
            b.parseProperties();
            REQUIRE_PROPERTY(b, @a, -1);
            REQUIRE_PROPERTY(b, @b, 200);
        }

        {
            EXT_B b(PdArgs(LA("@b", 200, "@a", -100), gensym("ext.b"), 0, gensym("ext.b")));
            b.parseProperties();
            REQUIRE_PROPERTY(b, @a, -100);
            REQUIRE_PROPERTY(b, @b, 200);
        }

        {
            EXT_B b(PdArgs(LA(1, 2.5, "@b", 200, "@a", -100), gensym("ext.b"), 0, gensym("ext.b")));
            b.parseProperties();
            REQUIRE_PROPERTY(b, @a, -100);
            REQUIRE_PROPERTY(b, @b, 200);
        }

        {
            EXT_B b(PdArgs(LA("first", "second", "@b", 200, "@a", -100), gensym("ext.b"), 0, gensym("ext.b")));
            b.parseProperties();
            REQUIRE_PROPERTY(b, @a, -100);
            REQUIRE_PROPERTY(b, @b, 200);
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
        BaseObject b1(PdArgs(L(), gensym("testname"), 0, gensym("testname")));
        REQUIRE(b1.canvas());
        REQUIRE(b1.findInStdPaths("test") == "");
        REQUIRE(b1.rootCanvas() != 0);

        CanvasPtr cnv1 = PureData::instance().createTopCanvas("/dir/file.pd");

        BaseObject b2(PdArgs(L(), gensym("test"), 0, gensym("test")));
        REQUIRE(b2.canvas() == cnv1->pd_canvas());
        REQUIRE(b2.findInStdPaths("test") == "");
        REQUIRE(b2.rootCanvas() == b2.canvas());

        CanvasPtr cnv2 = PureData::instance().createTopCanvas(TEST_DATA_DIR "/test.pd");

        BaseObject b3(PdArgs(L(), gensym("mtof"), 0, gensym("mtof")));
        REQUIRE(b3.canvas() == cnv2->pd_canvas());
        REQUIRE(b3.findInStdPaths("unknown") == "");
        REQUIRE(b3.findInStdPaths("snd_mono_48k.wav") == TEST_DATA_DIR "/snd_mono_48k.wav");

    }

    SECTION("findInStdPaths")
    {
        setup_test_extd();

        SECTION("abstraction") {
            External x0("test_std_path0");
            REQUIRE(x0.object());
            REQUIRE(x0.numInlets() == 1);
            REQUIRE(x0.numOutlets() == 1);

            External in0("symbol");
            REQUIRE(in0.connectTo(0, x0, 0));

            ListenerExternal o0("out0");
            REQUIRE(x0.connectTo(0, o0, 0));

            in0.sendSymbol("???");
            REQUIRE(o0.msg() == Message(&s_));

            in0.sendSymbol("test_std_path0.pd");
            REQUIRE(o0.msg().atomValue().asSymbol() == gensym(TEST_DATA_DIR "/test_std_path0.pd"));

            in0.sendSymbol("snd_mono_48k.wav");
            REQUIRE(o0.msg().atomValue().asSymbol() == gensym(TEST_DATA_DIR "/snd_mono_48k.wav"));
        }

        SECTION("subpatch") {
            External x0("test_std_path1");
            REQUIRE(x0.object());
            REQUIRE(x0.numInlets() == 1);
            REQUIRE(x0.numOutlets() == 1);

            External in0("symbol");
            REQUIRE(in0.connectTo(0, x0, 0));

            ListenerExternal o0("out0");
            REQUIRE(x0.connectTo(0, o0, 0));

            in0.sendSymbol("???");
            REQUIRE(o0.msg() == Message(&s_));

            in0.sendSymbol("test_std_path0.pd");
            REQUIRE(o0.msg().atomValue().asSymbol() == gensym(TEST_DATA_DIR "/test_std_path0.pd"));

            in0.sendSymbol("snd_mono_48k.wav");
            REQUIRE(o0.msg().atomValue().asSymbol() == gensym(TEST_DATA_DIR "/snd_mono_48k.wav"));
        }

        SECTION("subpatch abstraction") {
            External x0("test_std_path2");
            REQUIRE(x0.object());
            REQUIRE(x0.numInlets() == 1);
            REQUIRE(x0.numOutlets() == 1);

            External in0("symbol");
            REQUIRE(in0.connectTo(0, x0, 0));

            ListenerExternal o0("out0");
            REQUIRE(x0.connectTo(0, o0, 0));

            in0.sendSymbol("???");
            REQUIRE(o0.msg() == Message(&s_));

            in0.sendSymbol("test_std_path0.pd");
            REQUIRE(o0.msg().atomValue().asSymbol() == gensym(TEST_DATA_DIR "/test_std_path0.pd"));

            in0.sendSymbol("snd_mono_48k.wav");
            REQUIRE(o0.msg().atomValue().asSymbol() == gensym(TEST_DATA_DIR "/snd_mono_48k.wav"));
        }

        SECTION("abstraction") {
            External x0("base/test_std_path3");
            REQUIRE(x0.object());
            REQUIRE(x0.numInlets() == 1);
            REQUIRE(x0.numOutlets() == 1);

            External in0("symbol");
            REQUIRE(in0.connectTo(0, x0, 0));

            ListenerExternal o0("out0");
            REQUIRE(x0.connectTo(0, o0, 0));

            in0.sendSymbol("???");
            REQUIRE(o0.msg() == Message(&s_));

            // not found in parent directory
            in0.sendSymbol("snd_mono_48k.wav");
            REQUIRE(o0.msg().atomValue().asSymbol() == &s_);

            // found in current directory
            in0.sendSymbol("snd0_ch01_44.1k_441samp.wav");
            REQUIRE(o0.msg().atomValue().asSymbol() == gensym(TEST_DATA_DIR "/base/snd0_ch01_44.1k_441samp.wav"));

            // found in parent via relative path
            in0.sendSymbol("../snd_mono_48k.wav");
            REQUIRE(o0.msg().atomValue().asSymbol() == gensym(TEST_DATA_DIR "/base/../snd_mono_48k.wav"));
        }

        SECTION("abstraction") {
            External x0("base/test_std_path4");
            REQUIRE(x0.object());
            REQUIRE(x0.numInlets() == 1);
            REQUIRE(x0.numOutlets() == 1);

            External in0("symbol");
            REQUIRE(in0.connectTo(0, x0, 0));

            ListenerExternal o0("out0");
            REQUIRE(x0.connectTo(0, o0, 0));

            in0.sendSymbol("snd_mono_48k.wav");
            REQUIRE(o0.msg().atomValue().asSymbol()->s_name == std::string(TEST_DATA_DIR "/base/../snd_mono_48k.wav"));
        }
    }

    SECTION("outletAt")
    {
        BaseObject b1(PdArgs(L(), gensym("test"), 0, gensym("test")));
        REQUIRE(b1.outletAt(0) == 0);
    }

    SECTION("addProperty")
    {
        BaseObject b(PdArgs(L(), gensym("test"), 0, gensym("test")));
        b.addProperty(new IntProperty("int1", 1000));
        b.addProperty(new IntProperty("int1", 10));
        REQUIRE(b.property("int1")->get() == LF(10));
        REQUIRE(b.properties().size() == 1);

        Property* p = new BoolProperty("bool1", true);
        b.addProperty(p);
        b.addProperty(p);
        REQUIRE(b.properties().size() == 2);
    }

    SECTION("realOutput")
    {
        SECTION("EXT_C")
        {
            pd_test_mod_init_test_extc();
            TestExtEXT_C t("ext.c");

            SECTION("general out")
            {
                REQUIRE(t->numOutlets() == 1);
                REQUIRE(t->outletAt(0));
                REQUIRE(t->outletAt(1) == nullptr);

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
                t->listTo(0, LF(1, 2, 3));
                REQUIRE(t.hasOutput());
                REQUIRE(t.outputListAt(0) == LF(1, 2, 3));

                t.clearAll();
                t->listTo(1, LF(1, 2, 3));
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
                t->anyTo(0, LA("any", 1, 2, 3));
                REQUIRE(t.hasOutput());
                REQUIRE(t.outputAnyAt(0) == LA("any", 1, 2, 3));

                t.clearAll();
                t->anyTo(1, LA("any", 1, 2, 3));
                REQUIRE(!t.hasOutput());

                t.clearAll();
                t->messageTo(0, Message(22));
                REQUIRE(t.hasOutput());
                REQUIRE(t.outputFloatAt(0) == 22);

                t.clearAll();
                t->messageTo(1, Message(22));
                REQUIRE(!t.hasOutput());

                t.clearAll();
                t->anyTo(0, gensym("B"), LF(1, 2, 3));
                REQUIRE(t.hasOutput());
                REQUIRE(t.outputAnyAt(0) == LA("B", 1, 2, 3));

                t.clearAll();
                t->anyTo(1, gensym("B"), LF(1, 2, 3));
                REQUIRE(!t.hasOutput());

                t.clearAll();
                t->anyTo(0, gensym("B"), A(23));
                REQUIRE(t.hasOutput());
                REQUIRE(t.outputAnyAt(0) == LA("B", 23));

                t.clearAll();
                t->anyTo(1, gensym("B"), A(23));
                REQUIRE(!t.hasOutput());
            }

            SECTION("process inlets")
            {
                REQUIRE(!t->processAnyInlets(gensym("ABC"), L()));
                REQUIRE(!t->processAnyInlets(gensym("_"), L()));
                REQUIRE(t->processAnyInlets(gensym("_:01"), LF(123)));
                REQUIRE(t->last_inlet == 1);
                REQUIRE(t->last_inlet_data == LF(123));
            }

            SECTION("process props")
            {
                REQUIRE(!t->processAnyProps(gensym("???"), L()));
                REQUIRE(!t->processAnyProps(gensym("@abc"), L()));

                t.clearAll();
                REQUIRE(t->processAnyProps(gensym("@c?"), L()));
                REQUIRE(t.hasOutput());
                REQUIRE(t.outputAnyAt(0) == LA("@c", 101));

                t.clearAll();
                REQUIRE(t->processAnyProps(gensym("@c"), LF(200)));
                REQUIRE(!t.hasOutput());

                t.clearAll();
                REQUIRE(t->processAnyProps(gensym("@c?"), L()));
                REQUIRE(t.hasOutput());
                REQUIRE(t.outputAnyAt(0) == LA("@c", 200));
            }
        }

        SECTION("props")
        {
            SECTION("empty")
            {
                TestExtEXT_C t("ext.c");
                REQUIRE_PROPERTY(t, @c, 101);
                REQUIRE_PROPERTY(t, @d, 101);
                REQUIRE_PROPERTY(t, @s, "empty");
                REQUIRE_PROPERTY(t, @l, -1);
                REQUIRE_PROPERTY(t, @cb, 1, 2, 3);
            }

            SECTION("args")
            {
                SECTION("1")
                {
                    TestExtEXT_C t("ext.c", 0xBEEF);
                    REQUIRE_PROPERTY(t, @c, 0xBEEF);
                    REQUIRE_PROPERTY(t, @d, 101);
                    REQUIRE_PROPERTY(t, @s, "empty");
                    REQUIRE_PROPERTY(t, @l, -1);
                    REQUIRE_PROPERTY(t, @cb, 1, 2, 3);
                }

                SECTION("2")
                {
                    TestExtEXT_C t("ext.c", 0xBEEF, 10);
                    REQUIRE_PROPERTY(t, @c, 0xBEEF);
                    REQUIRE_PROPERTY(t, @d, 10);
                    REQUIRE_PROPERTY(t, @s, "empty");
                    REQUIRE_PROPERTY(t, @l, -1);
                    REQUIRE_PROPERTY(t, @cb, 1, 2, 3);
                }

                SECTION("3")
                {
                    TestExtEXT_C t("ext.c", 0xBEEF, 10, "abc");
                    REQUIRE_PROPERTY(t, @c, 0xBEEF);
                    REQUIRE_PROPERTY(t, @d, 10);
                    REQUIRE_PROPERTY(t, @s, "abc");
                    REQUIRE_PROPERTY(t, @l, -1);
                    REQUIRE_PROPERTY(t, @cb, 1, 2, 3);
                }

                SECTION("3 quoted")
                {
                    TestExtEXT_C t("ext.c", 0xBEEF, 10, "\"a space\"");
                    REQUIRE_PROPERTY(t, @c, 0xBEEF);
                    REQUIRE_PROPERTY(t, @d, 10);
                    REQUIRE_PROPERTY(t, @s, "a space");
                    REQUIRE_PROPERTY(t, @l, -1);
                    REQUIRE_PROPERTY(t, @cb, 1, 2, 3);
                }

                SECTION("3 quoted")
                {
                    TestExtEXT_C t("ext.c", 0xBEEF, 10, "\"quoted", "string\"");
                    REQUIRE_PROPERTY(t, @c, 0xBEEF);
                    REQUIRE_PROPERTY(t, @d, 10);
                    REQUIRE_PROPERTY(t, @s, "quoted string");
                    REQUIRE_PROPERTY(t, @l, -1);
                    REQUIRE_PROPERTY(t, @cb, 1, 2, 3);
                }

                SECTION("3 quoted property")
                {
                    TestExtEXT_C t("ext.c", 0xBEEF, 10, "\"@esc_prop?\"");
                    REQUIRE_PROPERTY(t, @c, 0xBEEF);
                    REQUIRE_PROPERTY(t, @d, 10);
                    REQUIRE_PROPERTY(t, @s, "@esc_prop?");
                    REQUIRE_PROPERTY(t, @l, -1);
                    REQUIRE_PROPERTY(t, @cb, 1, 2, 3);
                }

                SECTION("4")
                {
                    TestExtEXT_C t("ext.c", 0xBEEF, 10, "\"@esc_prop?\"", 1000);
                    REQUIRE_PROPERTY(t, @c, 0xBEEF);
                    REQUIRE_PROPERTY(t, @d, 10);
                    REQUIRE_PROPERTY(t, @s, "@esc_prop?");
                    REQUIRE_PROPERTY(t, @l, 1000);
                    REQUIRE_PROPERTY(t, @cb, 1, 2, 3);
                }

                SECTION("5")
                {
                    TestExtEXT_C t("ext.c", 0xBEEF, 10, "\"@esc_prop?\"", 1000, 2000);
                    REQUIRE_PROPERTY(t, @c, 0xBEEF);
                    REQUIRE_PROPERTY(t, @d, 10);
                    REQUIRE_PROPERTY(t, @s, "@esc_prop?");
                    REQUIRE_PROPERTY(t, @l, 1000, 2000);
                    REQUIRE_PROPERTY(t, @cb, 1, 2, 3);
                }

                SECTION("4: quoted")
                {
                    TestExtEXT_C t("ext.c", 0xBEEF, 10, "\"@esc_prop?\"", "\"a", "b\"");
                    REQUIRE_PROPERTY(t, @c, 0xBEEF);
                    REQUIRE_PROPERTY(t, @d, 10);
                    REQUIRE_PROPERTY(t, @s, "@esc_prop?");
                    REQUIRE_PROPERTY(t, @l, "a b");
                    REQUIRE_PROPERTY(t, @cb, 1, 2, 3);
                }

                SECTION("4: quote property")
                {
                    TestExtEXT_C t("ext.c", 0xBEEF, 10, "\"@esc_prop?\"", "\"@a\"");
                    REQUIRE_PROPERTY(t, @c, 0xBEEF);
                    REQUIRE_PROPERTY(t, @d, 10);
                    REQUIRE_PROPERTY(t, @s, "@esc_prop?");
                    REQUIRE_PROPERTY(t, @l, "@a");
                    REQUIRE_PROPERTY(t, @cb, 1, 2, 3);
                }

                SECTION("4: quote property")
                {
                    TestExtEXT_C t("ext.c", 0xBEEF, 10, "@s", "\"@s\"");
                    REQUIRE_PROPERTY(t, @c, 0xBEEF);
                    REQUIRE_PROPERTY(t, @d, 10);
                    REQUIRE_PROPERTY(t, @s, "@s");
                    REQUIRE_PROPERTY(t, @l, -1);
                    REQUIRE_PROPERTY(t, @cb, 1, 2, 3);
                }

                SECTION("4: quote property")
                {
                    TestExtEXT_C t("ext.c", 10, "@s", "\"a", "space\"");
                    REQUIRE_PROPERTY(t, @s, "a space");
                }

                SECTION("4: quote property")
                {
                    TestExtEXT_C t("ext.c", 10, "@s", "\"@a", "space\"");
                    REQUIRE_PROPERTY(t, @s, "@a space");
                }

                SECTION("parse pos args")
                {
                    TestExtEXT_C t("ext.c", "bs()");
                    REQUIRE_PROPERTY(t, @c, 64);
                }

                SECTION("parse pos args")
                {
                    TestExtEXT_C t("ext.c", "@c", "bs()");
                    REQUIRE_PROPERTY(t, @c, 64);
                }

                SECTION("parse pos args")
                {
                    TestExtEXT_C t("ext.c", "@c", "expr(3*9)");
                    REQUIRE_PROPERTY(t, @c, 27);
                }
            }
        }
    }

    SECTION("dataT")
    {
        using IntA = DataAtom<IntData>;

        TestExtEXT_C t("ext.c");
        t.sendList(LF(1, 2, 3));
        t.sendList(IntA(100));

        REQUIRE(t->int_data == IntData(100));
    }

    SECTION("output")
    {
        SECTION("outletAtom")
        {
            TestExtEXT_C t("ext.c");

            REQUIRE(outletAtom(t->outletAt(0), Atom(1)));
            REQUIRE(t.outputFloatAt(0) == 1);

            REQUIRE(outletAtom(t->outletAt(0), A("abc")));
            REQUIRE(t.outputSymbolAt(0) == A("abc"));

            REQUIRE(outletAtom(t->outletAt(0), Atom(new IntData(200))));
            REQUIRE(t.outputAtomAt(0) == Atom(new IntData(200)));
        }

        SECTION("outletAtomList no simplify")
        {
            TestExtEXT_C t("ext.c");

            REQUIRE(outletAtomList(t->outletAt(0), L(), false));
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == L());

            REQUIRE(outletAtomList(t->outletAt(0), LF(11), false));
            REQUIRE(t.outputListAt(0) == LF(11));

            REQUIRE(outletAtomList(t->outletAt(0), LA("abc"), false));
            REQUIRE(t.outputListAt(0) == LA("abc"));

            REQUIRE(outletAtomList(t->outletAt(0), LF(1, 2, 3), false));
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LF(1, 2, 3));
        }

        SECTION("outletAtomList simplify")
        {
            TestExtEXT_C t("ext.c");

            REQUIRE_FALSE(outletAtomList(nullptr, L()));

            REQUIRE(outletAtomList(t->outletAt(0), L(), true));
            REQUIRE(t.isOutputBangAt(0));
            REQUIRE(t.outputListAt(0) == L());

            REQUIRE(outletAtomList(t->outletAt(0), LF(11), true));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 11);

            REQUIRE(outletAtomList(t->outletAt(0), LA("abc"), true));
            REQUIRE(t.isOutputSymbolAt(0));
            REQUIRE(t.outputSymbolAt(0) == SYM("abc"));

            REQUIRE(outletAtomList(t->outletAt(0), LF(1, 2, 3), true));
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LF(1, 2, 3));
        }

        SECTION("outletAtomListView ")
        {
            TestExtEXT_C t("ext.c");

            REQUIRE_FALSE(outletAtomListView(nullptr, L().view()));

            REQUIRE(outletAtomListView(t->outletAt(0), L().view(), true));
            REQUIRE(t.isOutputBangAt(0));
            REQUIRE(t.outputListAt(0) == L());

            REQUIRE(outletAtomListView(t->outletAt(0), LF(11).view(), true));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 11);

            REQUIRE(outletAtomListView(t->outletAt(0), LA("abc").view(), true));
            REQUIRE(t.isOutputSymbolAt(0));
            REQUIRE(t.outputSymbolAt(0) == SYM("abc"));

            REQUIRE(outletAtomListView(t->outletAt(0), LF(1, 2, 3).view(), true));
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LF(1, 2, 3));
        }

        SECTION("outletAtomListView ")
        {
            TestExtEXT_C t("ext.c");

            REQUIRE(outletAtomListView(t->outletAt(0), L().view(), false));
            REQUIRE(t.outputListAt(0) == L());

            REQUIRE(outletAtomListView(t->outletAt(0), LF(11).view(), false));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 11);

            REQUIRE(outletAtomListView(t->outletAt(0), LA("abc").view(), false));
            REQUIRE(t.isOutputSymbolAt(0));
            REQUIRE(t.outputSymbolAt(0) == SYM("abc"));

            REQUIRE(outletAtomListView(t->outletAt(0), LF(1, 2, 3).view(), false));
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LF(1, 2, 3));
        }
    }
}
