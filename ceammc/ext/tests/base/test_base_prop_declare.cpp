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
#include "prop.h"
#include "prop_declare.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(PropDeclare, prop, declare)

static const t_float FMIN = std::numeric_limits<t_float>::lowest();
static const t_float FMAX = std::numeric_limits<t_float>::max();
static const int IMIN = std::numeric_limits<int>::lowest();
static const int IMAX = std::numeric_limits<int>::max();

class TestPdAbstraction : public pd::External {
    typedef std::map<t_symbol*, ListenerExternal*> ListenerMap;

    std::vector<LogExternalOutput*> outs_;
    ListenerMap listeners_;

public:
    TestPdAbstraction(const char* name, const AtomList& args = L())
        : pd::External(name, args)
    {
        REQUIRE(object());

        for (size_t i = 0; i < numOutlets(); i++) {
            LogExternalOutput* e = new LogExternalOutput;
            connectTo(i, e->object(), 0);
            outs_.push_back(e);
        }

        loadbang();
    }

    t_object* pdObject()
    {
        return object();
    }

    void loadbang()
    {
        pd_vmess(&object()->te_g.g_pd, gensym("loadbang"), "f", 0);
    }

    void call(const char* method, const AtomList& l = L())
    {
        clearAll();
        sendMessage(gensym(method), l);
    }

    void clearAll()
    {
        for (size_t i = 0; i < outs_.size(); i++)
            outs_[i]->reset();
    }

    bool hasOutputAt(size_t n)
    {
        if (n >= outs_.size())
            return false;

        return !outs_[n]->isNull();
    }

    Message outputAt(size_t n) const
    {
        return outs_[n]->msg();
    }
};

TEST_CASE("prop.declare", "[externals]")
{
    pd_test_init();
    setup_base_prop();
    test::pdPrintToStdError(true);

    SECTION("construct")
    {
        t_symbol* fn = &s_;

        {
            // default name
            TestExtPropDeclare t("prop.declare");
            REQUIRE(t.object());
            REQUIRE(t.numInlets() == 0);
            REQUIRE(t.numOutlets() == 0);
            REQUIRE(t->isFloat());
            REQUIRE(t->name() == SYM("@default"));
            fn = t->fullName();

            REQUIRE(PropertyStorage::storage().contains(fn));
        }

        REQUIRE_FALSE(PropertyStorage::storage().contains(fn));

        {
            // invalid name
            TestExtPropDeclare t("prop.declare", 1, 2, 3);
            REQUIRE(t.object());
            REQUIRE(t->name() == SYM("@default"));
        }

        {
            // valid name
            TestExtPropDeclare t("prop.declare", LA("a", "b", "c"));
            REQUIRE(t.object());
            REQUIRE(t->name() == SYM("@a"));
            REQUIRE(t->isFloat());
        }

        {
            // same name
            pd::External t0("prop.declare", LA("a"));
            pd::External t1("prop.declare", LA("a"));

            REQUIRE(t0.object());
            REQUIRE(!t1.object());
        }

        {
            // float type
            TestExtPropDeclare t("prop.declare", LA("a", "@type", "float"));
            REQUIRE(t.object());
            REQUIRE(t->isFloat());
        }

        {
            // float type
            TestExtPropDeclare t("prop.declare", LA("a", "@f"));
            REQUIRE(t.object());
            REQUIRE(t->isFloat());
        }

        {
            // int type
            TestExtPropDeclare t("prop.declare", LA("a", "@type", "int"));
            REQUIRE(t.object());
            REQUIRE(t->isInt());
        }

        {
            // int type
            TestExtPropDeclare t("prop.declare", LA("a", "@i"));
            REQUIRE(t.object());
            REQUIRE(t->isInt());
        }

        {
            // bool type
            TestExtPropDeclare t("prop.declare", LA("a", "@type", "bool"));
            REQUIRE(t.object());
            REQUIRE(t->isBool());
        }

        {
            // bool type
            TestExtPropDeclare t("prop.declare", LA("a", "@b"));
            REQUIRE(t.object());
            REQUIRE(t->isBool());
        }

        {
            // symbol type
            TestExtPropDeclare t("prop.declare", LA("a", "@type", "symbol"));
            REQUIRE(t.object());
            REQUIRE(t->isSymbol());
        }

        {
            // symbol type
            TestExtPropDeclare t("prop.declare", LA("a", "@s"));
            REQUIRE(t.object());
            REQUIRE(t->isSymbol());
        }

        {
            // list type
            TestExtPropDeclare t("prop.declare", LA("a", "@type", "list"));
            REQUIRE(t.object());
            REQUIRE(t->isList());
        }

        {
            // list type
            TestExtPropDeclare t("prop.declare", LA("a", "@type", "list"));
            REQUIRE(t.object());
            REQUIRE(t->isList());
        }
    }

    SECTION("enum")
    {
        // symbol type
        TestExtPropDeclare t("prop.declare", LA("s", "@s", "@enum", "A", "B", "C"));
        auto p = PropertyStorage::storage().acquire(t->fullName());
        REQUIRE(p->enumValues() == LA("", "A", "B", "C"));
        PropertyStorage::storage().release(t->fullName());
    }

    SECTION("min/max float")
    {
        {
            // float default
            TestExtPropDeclare t("prop.declare", LA("f"));
            auto p = PropertyStorage::storage().acquire(t->fullName());
            REQUIRE(p->floatRange() == std::make_pair(FMIN, FMAX));
            PropertyStorage::storage().release(t->fullName());
        }

        {
            // float min
            TestExtPropDeclare t("prop.declare", LA("f", "@min", 2.1));
            auto p = PropertyStorage::storage().acquire(t->fullName());
            auto r = p->floatRange();
            REQUIRE(r.first == Approx(2.1));
            REQUIRE(r.second == FMAX);
            PropertyStorage::storage().release(t->fullName());
        }

        {
            // float max
            TestExtPropDeclare t("prop.declare", LA("f", "@max", 16.1));
            auto p = PropertyStorage::storage().acquire(t->fullName());
            auto r = p->floatRange();
            REQUIRE(r.second == Approx(16.1));
            REQUIRE(r.first == FMIN);
            PropertyStorage::storage().release(t->fullName());
        }

        {
            // float both
            TestExtPropDeclare t("prop.declare", LA("f", "@max", 16.1, "@min", 1.1));
            auto p = PropertyStorage::storage().acquire(t->fullName());
            auto r = p->floatRange();
            REQUIRE(r.second == Approx(16.1));
            REQUIRE(r.first == Approx(1.1));
            PropertyStorage::storage().release(t->fullName());
        }

        {
            // float both invalid
            TestExtPropDeclare t("prop.declare", LA("f", "@min", 16.1, "@max", 1.1));
            auto p = PropertyStorage::storage().acquire(t->fullName());
            auto r = p->floatRange();
            REQUIRE(r.second == Approx(16.1));
            REQUIRE(r.first == Approx(1.1));
            PropertyStorage::storage().release(t->fullName());
        }
    }

    SECTION("min/max int")
    {
        {
            // int default
            TestExtPropDeclare t("prop.declare", LA("i", "@i"));
            auto p = PropertyStorage::storage().acquire(t->fullName());
            REQUIRE(p->intRange() == std::make_pair(IMIN, IMAX));
            PropertyStorage::storage().release(t->fullName());
        }

        {
            // int min
            TestExtPropDeclare t("prop.declare", LA("i", "@i", "@min", 2));
            auto p = PropertyStorage::storage().acquire(t->fullName());
            REQUIRE(p->intRange() == std::make_pair(int(2), IMAX));
            PropertyStorage::storage().release(t->fullName());
        }

        {
            // int max
            TestExtPropDeclare t("prop.declare", LA("i", "@i", "@max", 20));
            auto p = PropertyStorage::storage().acquire(t->fullName());
            REQUIRE(p->intRange() == std::make_pair(IMIN, int(20)));
            PropertyStorage::storage().release(t->fullName());
        }

        {
            // int both
            TestExtPropDeclare t("prop.declare", LA("i", "@i", "@max", 20, "@min", -2));
            auto p = PropertyStorage::storage().acquire(t->fullName());
            REQUIRE(p->intRange() == std::make_pair(int(-2), int(20)));
            PropertyStorage::storage().release(t->fullName());
        }

        {
            // int both invalid order
            TestExtPropDeclare t("prop.declare", LA("i", "@i", "@max", -2, "@min", 20));
            auto p = PropertyStorage::storage().acquire(t->fullName());
            REQUIRE(p->intRange() == std::make_pair(int(-2), int(20)));
            PropertyStorage::storage().release(t->fullName());
        }
    }

    SECTION("external")
    {
#define REQUIRE_ABSTRACTION_OUTPUT(t, v0, v1, v2, v3, v4)             \
    {                                                                 \
        REQUIRE(obj.hasOutputAt(0));                                  \
        REQUIRE(obj.outputAt(0).isFloat());                           \
        REQUIRE(obj.outputAt(0).atomValue().asFloat() == Approx(v0)); \
        REQUIRE(obj.hasOutputAt(1));                                  \
        REQUIRE(obj.outputAt(1).isFloat());                           \
        REQUIRE(obj.outputAt(1).atomValue().asFloat() == Approx(v1)); \
        REQUIRE(obj.hasOutputAt(2));                                  \
        REQUIRE(obj.outputAt(2).isFloat());                           \
        REQUIRE(obj.outputAt(2).atomValue().asFloat() == Approx(v2)); \
        REQUIRE(obj.hasOutputAt(3));                                  \
        REQUIRE(obj.outputAt(3).isSymbol());                          \
        REQUIRE(obj.outputAt(3).atomValue() == S(v3));                \
        REQUIRE(obj.hasOutputAt(4));                                  \
        REQUIRE(obj.outputAt(4).isList());                            \
        REQUIRE(obj.outputAt(4).listValue() == v4);                   \
    }

        PureData::instance().createTopCanvas(TEST_DATA_DIR "/test_props.pd");

        {
            TestPdAbstraction obj("prop_declare_test1");
            REQUIRE(obj.object());
            REQUIRE(obj.numOutlets() == 5);

            REQUIRE_ABSTRACTION_OUTPUT(t, 1000, 4, 1, "defaultName", LX(1, 2, 3));
        }

        {
            TestPdAbstraction obj("prop_declare_test1", LA("@freq", 250));
            obj.loadbang();

            REQUIRE_ABSTRACTION_OUTPUT(t, 250, 4, 1, "defaultName", LX(1, 2, 3));
        }
    }
}
