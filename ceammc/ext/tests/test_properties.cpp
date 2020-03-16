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
#include "ceammc_property.h"

#include "test_base.h"

using namespace ceammc;

struct prop_ro {
    AtomList get() const { return LF(100.f); }
};

struct prop_rw {
    AtomList lst;
    AtomList get() const { return lst; }
    void set(const AtomList& l) { lst = l; }
};

struct prop_t_rw {
    double sz;
    double getSize() const { return sz; }
    void setSize(const double& s) { sz = s; }
};

static inline std::ostream& operator<<(std::ostream& os, t_symbol s)
{
    os << s.s_name;
    return os;
}

#define CHECK_READONLY(t, val)                           \
    {                                                    \
        t.setReadOnly();                                 \
        REQUIRE_FALSE(t.set(val));                       \
        REQUIRE_FALSE(t.get() == val);                   \
        REQUIRE_FALSE(t.setInit(val));                   \
        REQUIRE_FALSE(t.get() == val);                   \
        REQUIRE_FALSE(t.setArgIndex(0));                 \
        REQUIRE_FALSE(t.setArgIndexNext(nullptr));       \
        REQUIRE_FALSE(t.setArgIndexNext(&t));            \
        REQUIRE_FALSE(t.setSuccessFn(nullptr));          \
        REQUIRE_FALSE(t.setSuccessFn([](Property*) {})); \
    }

#define CHECK_INITONLY(t, val)         \
    {                                  \
        t.setInitOnly();               \
        REQUIRE_FALSE(t.set(val));     \
        REQUIRE_FALSE(t.get() == val); \
        REQUIRE(t.setInit(val));       \
        REQUIRE(t.get() == val);       \
    }

#define CHECK_SUCCESS(t, ok, not_ok)                      \
    {                                                     \
        int cnt = 0;                                      \
        REQUIRE(t.setSuccessFn([&](Property* prop) { cnt++; REQUIRE((&t) == prop); })); \
        REQUIRE(t.set(ok));                               \
        REQUIRE(cnt == 1);                                \
        REQUIRE(t.setInit(ok));                           \
        REQUIRE(t.get() == ok);                           \
        REQUIRE(cnt == 2);                                \
        REQUIRE_FALSE(t.set(not_ok));                     \
        REQUIRE_FALSE(t.setInit(not_ok));                 \
        REQUIRE(cnt == 2);                                \
        REQUIRE(t.get() == ok);                           \
        REQUIRE(t.set(ok));                               \
        REQUIRE(cnt == 3);                                \
        REQUIRE(t.setInit(ok));                           \
        REQUIRE(cnt == 4);                                \
        t.setSuccessFn(nullptr);                          \
        REQUIRE_FALSE(t.set(not_ok));                     \
        REQUIRE_FALSE(t.setInit(not_ok));                 \
        REQUIRE(t.set(ok));                               \
        REQUIRE(t.setInit(ok));                           \
        REQUIRE(cnt == 4);                                \
    }

#define _ID(id) id
#define _DO_CHECK_SET(t, val)        \
    {                                \
        REQUIRE(t.set(LA(val)));     \
        REQUIRE(t.get() == LA(val)); \
    }

template <typename T>
void REQUIRE_ALL(Property& p, std::function<void(Property&, T)> fn, std::initializer_list<T> l)
{
    for (auto& v : l)
        fn(p, v);
}

#define _SET_FLOAT [](Property& p, t_float new_v) { \
    REQUIRE(p.setFloat(new_v));                     \
    t_float x;                                      \
    REQUIRE(p.getFloat(x));                         \
    REQUIRE(x == new_v);                            \
}

#define _SET_FLOAT_T [](Property& p, t_float new_v) { \
    REQUIRE(p.setT(new_v));                           \
    t_float x;                                        \
    REQUIRE(p.getT(x));                               \
    REQUIRE(x == new_v);                              \
}

#define _CANT_SET_FLOAT [](Property& p, t_float new_v) { \
    t_float v0, v1;                                      \
    REQUIRE(p.getFloat(v0));                             \
    REQUIRE_FALSE(p.setFloat(new_v));                    \
    REQUIRE(p.getFloat(v1));                             \
    REQUIRE(v0 == v1);                                   \
}

#define _CANT_SET_FLOAT_T [](Property& p, t_float new_v) { \
    t_float v0, v1;                                        \
    REQUIRE(p.getT(v0));                                   \
    REQUIRE_FALSE(p.setT(new_v));                          \
    REQUIRE(p.getT(v1));                                   \
    REQUIRE(v0 == v1);                                     \
}

TEST_CASE("Properties", "[ceammc::properties]")
{
    test::pdPrintToStdError();

    SECTION("float property")
    {
        FloatProperty p("float", 0.5);

        SECTION("main props")
        {
            REQUIRE(p.isReadWrite());
            REQUIRE_FALSE(p.isInitOnly());
            REQUIRE_FALSE(p.isReadOnly());
            REQUIRE(p.isPublic());
            REQUIRE_FALSE(p.isHidden());
            REQUIRE_FALSE(p.isInternal());
            REQUIRE_FALSE(p.hasArgIndex());

            p.setArgIndex(0);
            REQUIRE(p.hasArgIndex());
            REQUIRE(p.argIndex() == 0);

            REQUIRE(p.name() == SYM("float"));
            REQUIRE(p.isFloat());
            REQUIRE(!p.isBool());
            REQUIRE(!p.isAtom());
            REQUIRE(!p.isInt());
            REQUIRE(!p.isSymbol());
            REQUIRE(!p.isList());
            REQUIRE(p.type() == PropValueType::FLOAT);
            REQUIRE(p.units() == PropValueUnits::UNKNOWN);
            REQUIRE(p.access() == PropValueAccess::READWRITE);
            REQUIRE(p.view() == PropValueView::SLIDER);
            REQUIRE(p.value() == Approx(0.5));
            REQUIRE(p.defaultValue() == Approx(0.5));
            REQUIRE(p.get() == LX(0.5));
        }

        SECTION("get*")
        {
            REQUIRE(p.get() == LF(0.5));
            REQUIRE(p.value() == LF(0.5));

            t_float f;
            REQUIRE(p.getFloat(f));
            REQUIRE(f == 0.5);
            f = 0;
            REQUIRE(p.getT(f));
            REQUIRE(f == 0.5);
        }

        SECTION("set")
        {
            p.setValue(3);
            REQUIRE(p.value() == Approx(3));
            REQUIRE(p.defaultValue() == Approx(0.5));

            REQUIRE(p.set(LF(2)));
            REQUIRE(p.get() == LF(2));
            REQUIRE(p.defaultValue() == Approx(0.5));

            t_float f = 0;

            REQUIRE(p.setFloat(4.5));
            REQUIRE(p.getFloat(f));
            REQUIRE(f == 4.5);
            REQUIRE(p.setInt(200));
            REQUIRE(p.getFloat(f));
            REQUIRE(f == 200);

            REQUIRE(p.setT(t_float(-1.5)));
            REQUIRE(p.getT(f));
            REQUIRE(f == -1.5);
            REQUIRE(p.setT(int(-1)));
            REQUIRE(p.value() == -1);

            REQUIRE(p.setInit(LF(1000)));
            REQUIRE(p.get() == LF(1000));

            REQUIRE(p.reset());
            REQUIRE(p.value() == 0.5);
        }

        SECTION("set invalid")
        {
            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LF(1, 2)));
            REQUIRE_FALSE(p.set(LA("A")));
            REQUIRE_FALSE(p.set(LA("A", "B")));
            REQUIRE_FALSE(p.setBool(true));
            REQUIRE_FALSE(p.setSymbol(SYM("ABC")));
            REQUIRE_FALSE(p.setAtom(S("A")));
            REQUIRE_FALSE(p.setList(LF(1, 2, 3)));
            REQUIRE_FALSE(p.setT(true));
            REQUIRE_FALSE(p.setT(SYM("ABC")));
            REQUIRE_FALSE(p.setT(S("A")));
            REQUIRE_FALSE(p.setT(LF(1, 2, 3)));
        }

        SECTION("invalid getT/*")
        {
            bool b;
            int i;
            t_symbol* s;
            Atom a;
            AtomList l;

            REQUIRE_FALSE(p.getT(b));
            REQUIRE_FALSE(p.getT(i));
            REQUIRE_FALSE(p.getT(s));
            REQUIRE_FALSE(p.getT(a));
            REQUIRE_FALSE(p.getT(l));

            REQUIRE_FALSE(p.getBool(b));
            REQUIRE_FALSE(p.getInt(i));
            REQUIRE_FALSE(p.getSymbol(s));
            REQUIRE_FALSE(p.getAtom(a));
            REQUIRE_FALSE(p.getList(l));
        }

        SECTION("readonly")
        {
            CHECK_READONLY(p, LF(1000.5));

            // readonly only for set method
            p.setValue(-5);
            REQUIRE(p.value() == -5);
            p.setFloat(-2.5);
            REQUIRE(p.value() == -2.5);
        }

        SECTION("initonly")
        {
            CHECK_INITONLY(p, LF(-1000.5));

            // initonly only for set method
            p.setValue(-5);
            REQUIRE(p.value() == -5);
            p.setFloat(-2.5);
            REQUIRE(p.value() == -2.5);
        }

        SECTION("success")
        {
            CHECK_SUCCESS(p, LF(1.5), LA("ABCDE"));
        }

        SECTION("check invalid fn")
        {
            REQUIRE_FALSE(p.setIntCheckFn(nullptr));
            REQUIRE_FALSE(p.setAtomCheckFn(nullptr));
            REQUIRE_FALSE(p.setSymbolCheckFn(nullptr));
            REQUIRE_FALSE(p.setListCheckFn(nullptr));
        }

        SECTION("check float fn")
        {
            REQUIRE(p.setFloatCheckFn([](t_float v) { return v < 0; }));

            REQUIRE_ALL<t_float>(p, _SET_FLOAT, { -0.001, -1, -100 });
            REQUIRE_ALL<t_float>(p, _SET_FLOAT_T, { -0.001, -1, -100 });

            REQUIRE_ALL<t_float>(p, _CANT_SET_FLOAT, { 0, 0.001, 2 });
            REQUIRE_ALL<t_float>(p, _CANT_SET_FLOAT_T, { 0, 0.001, 2 });

            REQUIRE(p.setFloatCheckFn([](t_float v) { return v > 0; }));

            REQUIRE_ALL<t_float>(p, _SET_FLOAT, { 0.001, 1, 100 });
            REQUIRE_ALL<t_float>(p, _SET_FLOAT_T, { 1, 2, 3 });

            REQUIRE_ALL<t_float>(p, _CANT_SET_FLOAT, { 0, -0.001, -2 });
            REQUIRE_ALL<t_float>(p, _CANT_SET_FLOAT_T, { 0, -0.001, -2 });

            CHECK_SUCCESS(p, LF(2), LF(-2));
        }
    }

    SECTION("int property")
    {
        IntProperty p("int", 120);

        SECTION("main checks")
        {
            REQUIRE(p.isReadWrite());
            REQUIRE_FALSE(p.isInitOnly());
            REQUIRE_FALSE(p.isReadOnly());
            REQUIRE(p.isPublic());
            REQUIRE_FALSE(p.isHidden());
            REQUIRE_FALSE(p.isInternal());

            REQUIRE(p.name() == SYM("int"));
            REQUIRE(!p.isFloat());
            REQUIRE(!p.isBool());
            REQUIRE(!p.isAtom());
            REQUIRE(p.isInt());
            REQUIRE(!p.isSymbol());
            REQUIRE(!p.isList());
            REQUIRE(p.type() == PropValueType::INTEGER);
            REQUIRE(p.units() == PropValueUnits::UNKNOWN);
            REQUIRE(p.access() == PropValueAccess::READWRITE);
            REQUIRE(p.view() == PropValueView::NUMBOX);
            REQUIRE(p.value() == 120);
            REQUIRE(p.defaultValue() == 120);
            REQUIRE(p.get() == LF(120));
        }

        SECTION("set*")
        {
            REQUIRE(p.set(LF(2)));
            REQUIRE(p.value() == 2);

            p.setValue(3);
            REQUIRE(p.value() == 3);

            REQUIRE(p.set(LF(20.1)));
            REQUIRE(p.value() == 20);

            p.setValue(t_float(1.2));
            REQUIRE(p.value() == 1);

            REQUIRE(p.setInit(LF(300)));
            REQUIRE(p.value() == 300);

            REQUIRE(p.setInt(100));
            REQUIRE(p.value() == 100);
            REQUIRE(p.setT(50));
            REQUIRE(p.value() == 50);
        }

        SECTION("invalid set")
        {
            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LF(1, 2)));
            REQUIRE_FALSE(p.set(LA("A")));
            REQUIRE_FALSE(p.set(LA("A", "B")));
        }

        SECTION("readonly")
        {
            CHECK_READONLY(p, LF(1000));
        }

        SECTION("initonly")
        {
            CHECK_INITONLY(p, LF(-100));
        }

        SECTION("success")
        {
            CHECK_SUCCESS(p, LF(-2), LF(1, 2));
        }

        SECTION("getInt/getT")
        {
            bool b;
            t_float f;
            int i;
            t_symbol* s;
            Atom a;
            AtomList l;

            REQUIRE_FALSE(p.getT(b));
            REQUIRE_FALSE(p.getT(f));
            REQUIRE_FALSE(p.getT(s));
            REQUIRE_FALSE(p.getT(a));
            REQUIRE_FALSE(p.getT(l));

            REQUIRE(p.getT(i));
            REQUIRE(i == 120);

            REQUIRE_FALSE(p.getBool(b));
            REQUIRE_FALSE(p.getFloat(f));
            REQUIRE_FALSE(p.getSymbol(s));
            REQUIRE_FALSE(p.getAtom(a));
            REQUIRE_FALSE(p.getList(l));

            i = 0;
            REQUIRE(p.getInt(i));
            REQUIRE(i == 120);
        }

        SECTION("setInt/setT")
        {
            REQUIRE_FALSE(p.setT(true));
            REQUIRE_FALSE(p.setT(t_float(1)));
            REQUIRE_FALSE(p.setT(SYM("a")));
            REQUIRE_FALSE(p.setT(A(123)));

            REQUIRE(p.value() == 120);
            REQUIRE(p.setT(100));
            REQUIRE(p.value() == 100);

            REQUIRE_FALSE(p.setBool(true));
            REQUIRE_FALSE(p.setFloat(1));
            REQUIRE_FALSE(p.setSymbol(SYM("a")));
            REQUIRE_FALSE(p.setAtom(A(123)));

            REQUIRE(p.value() == 100);
            REQUIRE(p.setInt(10));
            REQUIRE(p.value() == 10);
        }

        SECTION("check fn")
        {
            REQUIRE_FALSE(p.setFloatCheckFn(nullptr));
            REQUIRE_FALSE(p.setAtomCheckFn(nullptr));
            REQUIRE_FALSE(p.setSymbolCheckFn(nullptr));
            REQUIRE_FALSE(p.setListCheckFn(nullptr));

            p.setIntCheckFn([](int v) { return v < 0; });
            REQUIRE(p.set(LF(-2)));
            REQUIRE(p.value() == -2);
            REQUIRE(p.set(LF(-1)));
            REQUIRE(p.value() == -1);
            REQUIRE_FALSE(p.set(LF(0)));
            REQUIRE(p.value() == -1);
            REQUIRE_FALSE(p.set(LF(1)));
            REQUIRE(p.value() == -1);

            p.setIntCheckFn([](int v) { return v > 0; });
            REQUIRE_FALSE(p.set(LF(-2)));
            REQUIRE(p.value() == -1);
            REQUIRE(p.set(LF(100)));
            REQUIRE(p.value() == 100);

            CHECK_SUCCESS(p, LF(2), LF(-2));
        }
    }

    SECTION("bool property")
    {
        BoolProperty p("bool", true);

        SECTION("main")
        {
            REQUIRE(p.isReadWrite());
            REQUIRE_FALSE(p.isInitOnly());
            REQUIRE_FALSE(p.isReadOnly());
            REQUIRE(p.isPublic());
            REQUIRE_FALSE(p.isHidden());
            REQUIRE_FALSE(p.isInternal());

            REQUIRE(p.name() == SYM("bool"));
            REQUIRE(!p.isFloat());
            REQUIRE(p.isBool());
            REQUIRE(!p.isAtom());
            REQUIRE(!p.isInt());
            REQUIRE(!p.isSymbol());
            REQUIRE(!p.isList());
            REQUIRE(p.type() == PropValueType::BOOLEAN);
            REQUIRE(p.units() == PropValueUnits::UNKNOWN);
            REQUIRE(p.access() == PropValueAccess::READWRITE);
            REQUIRE(p.view() == PropValueView::TOGGLE);
            REQUIRE(p.value() == true);
            REQUIRE(p.defaultValue() == true);
            REQUIRE(p.get() == LF(1));
        }

        SECTION("invalid set")
        {
            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LF(1, 2)));
            REQUIRE_FALSE(p.set(LA("A")));
            REQUIRE_FALSE(p.set(LA("A", "B")));
        }

        REQUIRE(p.set(LF(1)));
        REQUIRE(p.value() == true);
        REQUIRE(p.set(LF(0)));
        REQUIRE(p.value() == false);
        REQUIRE(p.set(LA("true")));
        REQUIRE(p.value() == true);
        REQUIRE(p.set(LA("false")));
        REQUIRE(p.value() == false);
        REQUIRE(p.set(LA("True")));
        REQUIRE(p.value() == true);
        REQUIRE(p.set(LA("False")));
        REQUIRE(p.value() == false);

        p.setValue(true);
        REQUIRE(p.value() == true);
        p.setValue(false);
        REQUIRE(p.value() == false);

        REQUIRE_FALSE(p.setFloatCheckFn(nullptr));
        REQUIRE_FALSE(p.setIntCheckFn(nullptr));
        REQUIRE_FALSE(p.setAtomCheckFn(nullptr));
        REQUIRE_FALSE(p.setSymbolCheckFn(nullptr));
        REQUIRE_FALSE(p.setListCheckFn(nullptr));

        CHECK_READONLY(p, LF(1));
        CHECK_INITONLY(p, LF(1));

        SECTION("ok")
        {
            BoolProperty p("bool", true);
            int cnt = 0;
            p.setSuccessFn([&cnt](Property* p) { cnt++; });
            REQUIRE(p.set(LA("true")));
            REQUIRE(cnt == 1);
            REQUIRE(p.set(LA("true")));
            REQUIRE(cnt == 2);
        }

        SECTION("getBool/getT")
        {
            BoolProperty p("@b", true);

            bool b = false;
            t_float f;
            int i;
            t_symbol* s;
            Atom a;
            AtomList l;

            REQUIRE(p.getT(b));
            REQUIRE(b == true);
            REQUIRE_FALSE(p.getT(f));
            REQUIRE_FALSE(p.getT(i));
            REQUIRE_FALSE(p.getT(s));
            REQUIRE_FALSE(p.getT(a));
            REQUIRE_FALSE(p.getT(l));

            b = false;
            REQUIRE(p.getBool(b));
            REQUIRE(b == true);

            REQUIRE_FALSE(p.getFloat(f));
            REQUIRE_FALSE(p.getInt(i));
            REQUIRE_FALSE(p.getSymbol(s));
            REQUIRE_FALSE(p.getAtom(a));
            REQUIRE_FALSE(p.getList(l));
        }

        SECTION("setBool/setT")
        {
            BoolProperty p("@b", true);

            REQUIRE(p.setT(false));
            REQUIRE(p.value() == false);

            REQUIRE_FALSE(p.setT(t_float(1)));
            REQUIRE_FALSE(p.setT(100));
            REQUIRE_FALSE(p.setT(SYM("a")));
            REQUIRE_FALSE(p.setT(A(123)));

            REQUIRE(p.setBool(true));
            REQUIRE(p.value() == true);

            REQUIRE_FALSE(p.setFloat(1));
            REQUIRE_FALSE(p.setInt(1));
            REQUIRE_FALSE(p.setSymbol(SYM("a")));
            REQUIRE_FALSE(p.setAtom(A(123)));
        }
    }

    SECTION("list property")
    {
        ListProperty p("list", LF(-1, -2, -3));

        REQUIRE(p.isReadWrite());
        REQUIRE_FALSE(p.isInitOnly());
        REQUIRE_FALSE(p.isReadOnly());
        REQUIRE(p.isPublic());
        REQUIRE_FALSE(p.isHidden());
        REQUIRE_FALSE(p.isInternal());

        REQUIRE(p.name() == SYM("list"));
        REQUIRE(!p.isFloat());
        REQUIRE(!p.isBool());
        REQUIRE(!p.isAtom());
        REQUIRE(!p.isInt());
        REQUIRE(!p.isSymbol());
        REQUIRE(p.isList());
        REQUIRE(p.type() == PropValueType::LIST);
        REQUIRE(p.units() == PropValueUnits::UNKNOWN);
        REQUIRE(p.access() == PropValueAccess::READWRITE);
        REQUIRE(p.view() == PropValueView::ENTRY);
        REQUIRE(p.defaultValue() == LF(-1, -2, -3));
        REQUIRE(p.value() == LF(-1, -2, -3));

        REQUIRE(p.set(L()));
        REQUIRE(p.value().empty());
        REQUIRE(p.set(LA("A", "B", "C")));
        REQUIRE(p.value() == LA("A", "B", "C"));
        p.setValue(LF(1, 2));
        REQUIRE(p.value() == LF(1, 2));
        REQUIRE(p.get() == LF(1, 2));

        p.setValue(LF(1));
        REQUIRE(p.value() == LF(1));

        CHECK_READONLY(p, LA(1, 2, 3));
        CHECK_INITONLY(p, LA("A", 100));

        SECTION("check fn")
        {
            ListProperty p("@l", LF(1, 2));

            REQUIRE_FALSE(p.setIntCheckFn(nullptr));
            REQUIRE_FALSE(p.setAtomCheckFn(nullptr));
            REQUIRE_FALSE(p.setSymbolCheckFn(nullptr));

            p.setListCheckFn([](const AtomList& l) { return l.size() == 2 && l.allOf(isFloat); });
            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LA(1)));
            REQUIRE_FALSE(p.set(LA("A")));
            REQUIRE_FALSE(p.set(LA("A", "B")));
            REQUIRE_FALSE(p.set(LA("A", 1)));
            REQUIRE_FALSE(p.set(LA(200, "B")));
            REQUIRE_FALSE(p.set(LA(1, 2, 3)));

            REQUIRE(p.set(LA(10, 20)));
            REQUIRE(p.value() == LF(10, 20));

            CHECK_SUCCESS(p, LF(1000, 2000), LF(1, 2, 3));

            p.setListCheckFn(nullptr);
            REQUIRE(p.set(LA(1, 2, 3)));
            REQUIRE(p.get() == LA(1, 2, 3));

            REQUIRE(p.checkPositive());
            REQUIRE(p.set(LF(1, 2, 3, 4, 5)));
            REQUIRE(p.get() == LF(1, 2, 3, 4, 5));
            REQUIRE_FALSE(p.set(LF(-1, 2, 3, 4, 5)));
            REQUIRE_FALSE(p.set(LA(1, 2, "ABC")));
            REQUIRE(p.get() == LF(1, 2, 3, 4, 5));
        }

        SECTION("getList/getT")
        {
            ListProperty p("@l", LF(1, 2));

            bool b = false;
            t_float f;
            int i;
            t_symbol* s;
            Atom a;
            AtomList l;

            REQUIRE_FALSE(p.getT(b));
            REQUIRE_FALSE(p.getT(f));
            REQUIRE_FALSE(p.getT(i));
            REQUIRE_FALSE(p.getT(s));
            REQUIRE_FALSE(p.getT(a));

            REQUIRE(p.getT(l));
            REQUIRE(l == LF(1, 2));
            l.clear();

            REQUIRE_FALSE(p.getBool(b));
            REQUIRE_FALSE(p.getFloat(f));
            REQUIRE_FALSE(p.getInt(i));
            REQUIRE_FALSE(p.getSymbol(s));
            REQUIRE_FALSE(p.getAtom(a));

            REQUIRE(l.empty());
            REQUIRE(p.getList(l));
            REQUIRE(l == LF(1, 2));
        }

        SECTION("setList/setT")
        {
            ListProperty p("@l", LF(1));

            REQUIRE_FALSE(p.setT(false));
            REQUIRE_FALSE(p.setT(t_float(1)));
            REQUIRE_FALSE(p.setT(100));
            REQUIRE_FALSE(p.setT(SYM("a")));
            REQUIRE_FALSE(p.setT(A(123)));

            REQUIRE_FALSE(p.setBool(true));
            REQUIRE_FALSE(p.setFloat(1));
            REQUIRE_FALSE(p.setInt(1));
            REQUIRE_FALSE(p.setSymbol(SYM("a")));
            REQUIRE_FALSE(p.setAtom(A(123)));

            REQUIRE(p.value() == LF(1));
        }

        SECTION("fromArgs")
        {
            ListProperty p("@l");
            p.setArgIndex(0);
            p.initFromArgList(LA(1, 2, 3, "@b", "@c"));
            REQUIRE(p.get() == LF(1, 2, 3));
            p.setArgIndex(1);
            p.initFromArgList(LA(1, 2, 3, "@b", "@c"));
            REQUIRE(p.get() == LF(2, 3));
            p.setArgIndex(2);
            p.initFromArgList(LA(1, 2, 3, "@b", "@c"));
            REQUIRE(p.get() == LF(3));
            REQUIRE(p.set(L()));
            p.setArgIndex(3);
            p.initFromArgList(LA(1, 2, 3, "@b", "@c"));
            REQUIRE(p.get() == L());
        }
    }

    SECTION("symbol property")
    {
        SymbolProperty p("@sym", SYM("AAAA"));

        REQUIRE(p.isReadWrite());
        REQUIRE_FALSE(p.isInitOnly());
        REQUIRE_FALSE(p.isReadOnly());
        REQUIRE(p.isPublic());
        REQUIRE_FALSE(p.isHidden());
        REQUIRE_FALSE(p.isInternal());

        REQUIRE(p.name() == SYM("@sym"));
        REQUIRE(!p.isFloat());
        REQUIRE(!p.isBool());
        REQUIRE(!p.isAtom());
        REQUIRE(!p.isInt());
        REQUIRE(p.isSymbol());
        REQUIRE(!p.isList());
        REQUIRE(p.type() == PropValueType::SYMBOL);
        REQUIRE(p.units() == PropValueUnits::UNKNOWN);
        REQUIRE(p.access() == PropValueAccess::READWRITE);
        REQUIRE(p.view() == PropValueView::ENTRY);
        REQUIRE(p.defaultValue() == SYM("AAAA"));
        REQUIRE(p.value() == SYM("AAAA"));

        REQUIRE_FALSE(p.set(L()));
        REQUIRE_FALSE(p.set(LF(100)));
        REQUIRE_FALSE(p.set(LF(1, 2)));
        REQUIRE_FALSE(p.set(LA("A", "B")));

        REQUIRE(p.set(LA("test")));
        REQUIRE(p.get() == LA("test"));
        REQUIRE(p.value() == SYM("test"));
        REQUIRE(p.set(LA("ABC")));
        REQUIRE(p.value() == S("ABC"));

        CHECK_SUCCESS(p, LA("ABC"), LF(1000));
        CHECK_READONLY(p, LA("A"));
        CHECK_INITONLY(p, LA("B"));

        SECTION("check fn")
        {
            SymbolProperty p("@sym", SYM("AAAA"));

            REQUIRE_FALSE(p.setFloatCheckFn(nullptr));
            REQUIRE_FALSE(p.setIntCheckFn(nullptr));
            REQUIRE_FALSE(p.setAtomCheckFn(nullptr));
            REQUIRE_FALSE(p.setListCheckFn(nullptr));

            p.setSymbolCheckFn([](t_symbol* s) { return s->s_name[0] == '#'; });
            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LA(1)));
            REQUIRE_FALSE(p.set(LA("A")));
            REQUIRE_FALSE(p.set(LA("A", "B")));
            REQUIRE_FALSE(p.set(LA("A", 1)));
            REQUIRE_FALSE(p.set(LA(200, "B")));
            REQUIRE_FALSE(p.set(LA(1, 2, 3)));
            REQUIRE_FALSE(p.set(LA("@A")));

            REQUIRE(p.set(LA("#a")));
            REQUIRE(p.value() == LA("#a"));
            CHECK_SUCCESS(p, LA("#b"), LA("B"));

            p.setSymbolCheckFn(nullptr);
            REQUIRE(p.set(LA("B")));
            REQUIRE(p.value() == LA("B"));
        }

        SECTION("getSymbol/getT")
        {
            SymbolProperty p("@s", SYM("A"));

            bool b = false;
            t_float f;
            int i;
            t_symbol* s;
            Atom a;
            AtomList l;

            REQUIRE_FALSE(p.getT(b));
            REQUIRE_FALSE(p.getT(f));
            REQUIRE_FALSE(p.getT(i));
            REQUIRE_FALSE(p.getT(l));

            REQUIRE(p.getT(s));
            REQUIRE(s == SYM("A"));

            s = nullptr;

            REQUIRE_FALSE(p.getBool(b));
            REQUIRE_FALSE(p.getFloat(f));
            REQUIRE_FALSE(p.getInt(i));
            REQUIRE_FALSE(p.getAtom(a));
            REQUIRE_FALSE(p.getList(l));

            REQUIRE(p.getSymbol(s));
            REQUIRE(s == SYM("A"));
        }

        SECTION("setList/setT")
        {
            SymbolProperty p("@s", SYM("B"));

            REQUIRE_FALSE(p.setT(false));
            REQUIRE_FALSE(p.setT(t_float(1)));
            REQUIRE_FALSE(p.setT(100));
            REQUIRE_FALSE(p.setT(A(123)));

            REQUIRE(p.setT(SYM("a")));
            REQUIRE(p.value() == SYM("a"));

            REQUIRE_FALSE(p.setBool(true));
            REQUIRE_FALSE(p.setFloat(1));
            REQUIRE_FALSE(p.setInt(1));
            REQUIRE_FALSE(p.setAtom(A(123)));

            REQUIRE(p.setSymbol(SYM("b")));
            REQUIRE(p.value() == SYM("b"));
        }
    }

    SECTION("atom property")
    {
        AtomProperty p("atom", Atom(1.5f));

        REQUIRE(p.isReadWrite());
        REQUIRE_FALSE(p.isInitOnly());
        REQUIRE_FALSE(p.isReadOnly());
        REQUIRE(p.isPublic());
        REQUIRE_FALSE(p.isHidden());
        REQUIRE_FALSE(p.isInternal());

        REQUIRE(p.name() == SYM("atom"));
        REQUIRE(!p.isFloat());
        REQUIRE(!p.isBool());
        REQUIRE(p.isAtom());
        REQUIRE(!p.isInt());
        REQUIRE(!p.isSymbol());
        REQUIRE(!p.isList());
        REQUIRE(p.type() == PropValueType::VARIANT);
        REQUIRE(p.units() == PropValueUnits::UNKNOWN);
        REQUIRE(p.access() == PropValueAccess::READWRITE);
        REQUIRE(p.view() == PropValueView::ENTRY);
        REQUIRE(p.defaultValue() == Atom(1.5f));
        REQUIRE(p.value() == Atom(1.5f));

        REQUIRE_FALSE(p.set(L()));
        REQUIRE_FALSE(p.set(LF(1, 2)));
        REQUIRE_FALSE(p.set(LA("A", "B")));

        REQUIRE(p.set(LF(200)));
        REQUIRE(p.get() == LF(200));
        REQUIRE(p.value() == Atom(200));
        REQUIRE(p.set(LA("ABC")));
        REQUIRE(p.value() == S("ABC"));

        p.setValue(Atom(10));
        REQUIRE(p.value() == Atom(10));

        CHECK_SUCCESS(p, LA("ABC"), LF(1, 2));
        CHECK_READONLY(p, LA(1));
        CHECK_INITONLY(p, LA("A"));

        SECTION("check fn")
        {
            AtomProperty p("@a", S("abc"));
            p.setAtomCheckFn([](const Atom& a) { return a.isProperty(); });
            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LA(1)));
            REQUIRE_FALSE(p.set(LA("A")));
            REQUIRE_FALSE(p.set(LA("A", "B")));
            REQUIRE_FALSE(p.set(LA("A", 1)));
            REQUIRE_FALSE(p.set(LA(200, "B")));
            REQUIRE_FALSE(p.set(LA(1, 2, 3)));

            REQUIRE(p.set(LA("@a")));
            REQUIRE(p.value() == LA("@a"));
            CHECK_SUCCESS(p, LA("@prop"), LA("non-prop"));

            p.setAtomCheckFn(nullptr);
            REQUIRE(p.set(LA(1)));
            REQUIRE(p.value() == LA(1));

            REQUIRE_FALSE(p.checkNegative());
            REQUIRE_FALSE(p.checkNonNegative());
            REQUIRE_FALSE(p.checkPositive());
        }

        SECTION("getAtom/getT")
        {
            AtomProperty p("@a", S("test"));

            bool b = false;
            t_float f;
            int i;
            t_symbol* s;
            Atom a;
            AtomList l;

            REQUIRE_FALSE(p.getT(b));
            REQUIRE_FALSE(p.getT(f));
            REQUIRE_FALSE(p.getT(i));
            REQUIRE_FALSE(p.getT(s));
            REQUIRE_FALSE(p.getT(l));

            REQUIRE(p.getT(a));
            REQUIRE(a == S("test"));

            a = Atom();

            REQUIRE_FALSE(p.getBool(b));
            REQUIRE_FALSE(p.getFloat(f));
            REQUIRE_FALSE(p.getInt(i));
            REQUIRE_FALSE(p.getSymbol(s));
            REQUIRE_FALSE(p.getList(l));

            REQUIRE(p.getAtom(a));
            REQUIRE(a == S("test"));
        }

        SECTION("setAtom/setT")
        {
            AtomProperty p("@a", S("B"));

            REQUIRE(p.setT(false));
            REQUIRE(p.value() == A(0.f));
            REQUIRE(p.setT(true));
            REQUIRE(p.value() == A(1));

            REQUIRE(p.setT(t_float(10.5)));
            REQUIRE(p.value() == A(10.5));
            REQUIRE(p.setT(1000));
            REQUIRE(p.value() == A(1000));
            REQUIRE(p.setT(SYM("123")));
            REQUIRE(p.value() == S("123"));
            REQUIRE(p.setT(S("a")));
            REQUIRE(p.value() == S("a"));
            REQUIRE(p.setT(A(123)));
            REQUIRE(p.value() == A(123));

            REQUIRE(p.setBool(true));
            REQUIRE(p.value() == A(1));
            REQUIRE(p.setFloat(2.5));
            REQUIRE(p.value() == A(2.5));
            REQUIRE(p.setInt(3));
            REQUIRE(p.value() == A(3));
            REQUIRE(p.setSymbol(SYM("ABC")));
            REQUIRE(p.value() == S("ABC"));
            REQUIRE(p.setAtom(A(1000)));
            REQUIRE(p.value() == A(1000));
        }
    }

    SECTION("SizeT property")
    {
        SizeTProperty p("size_t", 12);

        SECTION("main")
        {
            REQUIRE(p.isReadWrite());
            REQUIRE_FALSE(p.isInitOnly());
            REQUIRE_FALSE(p.isReadOnly());
            REQUIRE(p.isPublic());
            REQUIRE_FALSE(p.isHidden());
            REQUIRE_FALSE(p.isInternal());

            REQUIRE(p.name() == SYM("size_t"));
            REQUIRE(!p.isFloat());
            REQUIRE(!p.isBool());
            REQUIRE(!p.isAtom());
            REQUIRE(p.isInt());
            REQUIRE(!p.isSymbol());
            REQUIRE(!p.isList());
            REQUIRE(p.type() == PropValueType::INTEGER);
            REQUIRE(p.units() == PropValueUnits::UNKNOWN);
            REQUIRE(p.access() == PropValueAccess::READWRITE);
            REQUIRE(p.view() == PropValueView::NUMBOX);
            REQUIRE(p.defaultValue() == 12);
            REQUIRE(p.value() == 12);
            REQUIRE(p.get() == LF(12));
        }

        SECTION("invalid set")
        {
            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LF(-10)));
            REQUIRE_FALSE(p.set(LF(-0.1)));
            REQUIRE_FALSE(p.set(LF(1, 2)));
            REQUIRE_FALSE(p.set(LA("A", "B")));
            REQUIRE_FALSE(p.setBool(true));
            REQUIRE_FALSE(p.setFloat(100));
            REQUIRE_FALSE(p.setInt(-1));
            REQUIRE_FALSE(p.setSymbol(SYM("asd")));
            REQUIRE_FALSE(p.setAtom(Atom(100)));
        }

        SECTION("set")
        {
            REQUIRE(p.set(LF(0)));
            REQUIRE(p.get() == LF(0));
            REQUIRE(p.set(LF(0.1)));
            REQUIRE(p.get() == LF(0));
            REQUIRE(p.set(LF(0.51)));
            REQUIRE(p.get() == LF(1));
            REQUIRE(p.set(LF(0.9)));
            REQUIRE(p.get() == LF(1));
            REQUIRE(p.set(LF(2)));
            REQUIRE(p.get() == LF(2));
        }

        SECTION("check fn")
        {
            REQUIRE_FALSE(p.setFloatCheckFn(nullptr));
            REQUIRE_FALSE(p.setAtomCheckFn(nullptr));
            REQUIRE_FALSE(p.setSymbolCheckFn(nullptr));
            REQUIRE_FALSE(p.setListCheckFn(nullptr));

            REQUIRE(p.setIntCheckFn([](int v) { return v > 200; }));
            REQUIRE_FALSE(p.setValue((size_t)0));
            REQUIRE_FALSE(p.setValue((size_t)199));
            REQUIRE_FALSE(p.setValue((size_t)200));
            REQUIRE(p.setValue((size_t)201));
            REQUIRE(p.value() == 201);

            CHECK_SUCCESS(p, LF(201), LF(200));
        }

        SECTION("non rw")
        {
            CHECK_READONLY(p, LA(100));
            CHECK_INITONLY(p, LA(101));
        }
    }

    SECTION("combined property")
    {
        FloatProperty pf("f", 1);
        IntProperty pi("i", -100);
        SymbolProperty ps("s", SYM("A"));
        CombinedProperty p("comb", { &pf, &pi, &ps });

        SECTION("main")
        {
            REQUIRE_FALSE(p.isReadWrite());
            REQUIRE_FALSE(p.isInitOnly());
            REQUIRE(p.isReadOnly());
            REQUIRE_FALSE(p.isPublic());
            REQUIRE(p.isHidden());
            REQUIRE_FALSE(p.isInternal());

            REQUIRE(p.name() == SYM("comb"));
            REQUIRE(!p.isFloat());
            REQUIRE(!p.isBool());
            REQUIRE(!p.isAtom());
            REQUIRE(!p.isInt());
            REQUIRE(!p.isSymbol());
            REQUIRE(p.isList());
            REQUIRE(p.type() == PropValueType::LIST);
            REQUIRE(p.units() == PropValueUnits::UNKNOWN);
            REQUIRE(p.access() == PropValueAccess::READONLY);
            REQUIRE(p.view() == PropValueView::ENTRY);

            REQUIRE(p.get() == LA(1, -100, "A"));
            REQUIRE(pf.setValue(2.5));
            REQUIRE(p.get() == LA(2.5, -100, "A"));
        }

        SECTION("invalid set")
        {
            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LA(1, 2, "ABC")));
            REQUIRE(p.get() == LA(1, -100, "A"));
        }

        SECTION("getT")
        {
            bool b = false;
            t_float f;
            int i;
            t_symbol* s;
            Atom a;

            REQUIRE_FALSE(p.getT(b));
            REQUIRE_FALSE(p.getT(f));
            REQUIRE_FALSE(p.getT(i));
            REQUIRE_FALSE(p.getT(s));
            REQUIRE_FALSE(p.getT(a));

            REQUIRE_FALSE(p.getBool(b));
            REQUIRE_FALSE(p.getFloat(f));
            REQUIRE_FALSE(p.getInt(i));
            REQUIRE_FALSE(p.getSymbol(s));
            REQUIRE_FALSE(p.getAtom(a));
        }

        SECTION("getList")
        {
            AtomList l;
            REQUIRE(p.getT(l));
            REQUIRE(p.get() == LA(1, -100, "A"));
            l.clear();

            REQUIRE(p.getList(l));
            REQUIRE(p.get() == LA(1, -100, "A"));
        }
    }

    SECTION("check ok")
    {
        int cnt = 0;
        FloatProperty p("float", 100);
        p.setSuccessFn([&cnt](Property*) { cnt++; });

        REQUIRE(p.set(LF(1)));
        REQUIRE(cnt == 1);
        REQUIRE(p.set(LF(1)));
        REQUIRE(cnt == 2);
        REQUIRE(p.set(LF(1)));
        REQUIRE(cnt == 3);

        REQUIRE_FALSE(p.set(LA("ABC")));
        REQUIRE(cnt == 3);
        REQUIRE(p.set(LF(4)));
        REQUIRE(cnt == 4);

        p.setFloatCheckFn([](t_float f) { return f > 4; });
        REQUIRE_FALSE(p.setValue(1));
        REQUIRE_FALSE(p.setValue(2));
        REQUIRE_FALSE(p.setValue(4));
        REQUIRE(cnt == 4);
        REQUIRE(p.setValue(4.1));
        REQUIRE(cnt == 4);

        REQUIRE_FALSE(p.set(LF(4)));
        REQUIRE(cnt == 4);
        REQUIRE(p.set(LF(4.05)));
        REQUIRE(cnt == 5);
    }

    SECTION("pointer property")
    {
        using IntPtrProperty = PointerProperty<int>;
        int data = -100;
        IntPtrProperty p("&int", &data, PropValueAccess::READWRITE);

        SECTION("default ro")
        {
            p.setReadOnly();
            REQUIRE(!p.isReadWrite());
            REQUIRE(!p.isInitOnly());
            REQUIRE(p.isReadOnly());
            REQUIRE(p.isPublic());
            REQUIRE_FALSE(p.isHidden());
            REQUIRE_FALSE(p.isInternal());

            REQUIRE(p.name() == SYM("&int"));
            REQUIRE(!p.isFloat());
            REQUIRE(!p.isBool());
            REQUIRE(!p.isAtom());
            REQUIRE(p.isInt());
            REQUIRE(!p.isSymbol());
            REQUIRE(!p.isList());
            REQUIRE(p.type() == PropValueType::INTEGER);
            REQUIRE(p.units() == PropValueUnits::UNKNOWN);
            REQUIRE(p.access() == PropValueAccess::READONLY);
            REQUIRE(p.view() == PropValueView::NUMBOX);
            REQUIRE(p.value() == -100);
            REQUIRE(p.get() == LF(-100));

            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LA("ABC")));
            REQUIRE_FALSE(p.set(LF(1, 2)));
            REQUIRE_FALSE(p.set(LF(10)));

            data = 1000;
            REQUIRE(p.value() == 1000);
            REQUIRE(p.get() == LF(1000));

            REQUIRE(p.setInt(-10));
            REQUIRE(data == -10);
        }

        SECTION("default rw")
        {
            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LA("ABC")));
            REQUIRE_FALSE(p.set(LF(1, 2)));

            REQUIRE(p.set(LF(10)));
            REQUIRE(p.value() == 10);

            p.setIntCheckFn([](int v) { return v > 0; });
            REQUIRE(p.set(LF(1)));
            REQUIRE(p.value() == 1);
            REQUIRE_FALSE(p.set(LF(0)));
            REQUIRE(p.value() == 1);
            p.setIntCheckFn(nullptr);

            CHECK_SUCCESS(p, LF(-200), LA("b"));
            CHECK_READONLY(p, LA(100));
            CHECK_INITONLY(p, LA(101));
        }
    }

    SECTION("symbol alias property")
    {
        using IntAliasProperty = AliasProperty<IntProperty>;

        IntProperty pi("@int", 0);
        pi.setUnits(PropValueUnits::HZ);
        IntAliasProperty p("@one", &pi, 1);
        IntAliasProperty p0("@zero", &pi, 0);

        REQUIRE(p.isReadWrite());
        REQUIRE(!p.isInitOnly());
        REQUIRE(!p.isReadOnly());
        REQUIRE(!p.isPublic());
        REQUIRE(p.isHidden());
        REQUIRE(!p.isInternal());

        REQUIRE(p.name() == SYM("@one"));
        REQUIRE(!p.isFloat());
        REQUIRE(!p.isBool());
        REQUIRE(!p.isAtom());
        REQUIRE(p.isInt());
        REQUIRE(!p.isSymbol());
        REQUIRE(!p.isList());
        REQUIRE(p.type() == PropValueType::INTEGER);
        REQUIRE(p.units() == PropValueUnits::HZ);
        REQUIRE(p.view() == PropValueView::NUMBOX);
        REQUIRE(p.value() == 1);

        REQUIRE(p.set(L()));
        REQUIRE(pi.value() == 1);
        REQUIRE(p0.set(L()));
        REQUIRE(pi.value() == 0);

        int cnt = 0;
        p0.setSuccessFn([&cnt](Property*) { cnt++; });
        REQUIRE(p0.set(L()));
        REQUIRE(cnt == 1);
        REQUIRE(p0.setInit(L()));
        REQUIRE(cnt == 2);
    }

    SECTION("FlagProperty")
    {
        FlagProperty p("flag");
        REQUIRE(p.isInitOnly());
        REQUIRE(p.isBool());
        REQUIRE(p.isPublic());

        REQUIRE(p.get() == LF(0));
        REQUIRE(p.value() == false);
        REQUIRE_FALSE(p.set(L()));
        REQUIRE(p.setInit(L()));
        REQUIRE(p.value() == true);
        REQUIRE(p.setInit(LF(0)));
        REQUIRE(p.get() == LF(1));

        bool b = false;
        REQUIRE(p.getBool(b));
        REQUIRE(b == true);
        b = false;
        REQUIRE(p.getT(b));
        REQUIRE(b == true);

        int i = -1;
        REQUIRE(p.getInt(i));
        REQUIRE(i == 1);
        i = -100;
        REQUIRE(p.getT(i));
        REQUIRE(i == 1);
    }

    SECTION("init from args")
    {
        SECTION("float")
        {
            FloatProperty p("@f0", 100);

            REQUIRE_FALSE(p.initFromArgList(L()));
            // simple case
            REQUIRE(p.initFromArgList(LA("@f0", -1)));
            REQUIRE(p.value() == -1);

            REQUIRE(p.initFromArgList(LA("@f0", -2, "@f1", 200)));
            REQUIRE(p.value() == -2);

            REQUIRE_FALSE(p.initFromArgList(LA("@f0", -2, 3, "@f1", 200)));
            REQUIRE_FALSE(p.initFromArgList(LA("@f0", "@f1", 200)));
            REQUIRE_FALSE(p.initFromArgList(LA("@f0", -2, 3, "@f1", 200)));
            REQUIRE_FALSE(p.initFromArgList(LA("@f1", 200, "@f0")));
            REQUIRE_FALSE(p.initFromArgList(LA("@f1", 200, "@f0", 1, 2)));

            REQUIRE(p.initFromArgList(LA("@f1", -2, "@f0", 11)));
            REQUIRE(p.value() == 11);
            REQUIRE(p.initFromArgList(LA("@f1", "@f0", 12)));
            REQUIRE(p.value() == 12);

            p.setInternal();
            REQUIRE_FALSE(p.initFromArgList(LA("@f0", 200)));

            p.setReadOnly();
            REQUIRE_FALSE(p.initFromArgList(LA("@f0", 300)));
        }

        SECTION("float positional")
        {
            FloatProperty p("@f1", 100);

            p.setArgIndex(2);
            REQUIRE_FALSE(p.initFromArgList(L()));
            REQUIRE_FALSE(p.initFromArgList(LF(1)));
            REQUIRE_FALSE(p.initFromArgList(LF(1, 2)));
            REQUIRE(p.initFromArgList(LF(1, 2, 3)));
            REQUIRE(p.value() == 3);

            REQUIRE(p.initFromArgList(LA(1, 2, 30, "@p", 100)));
            REQUIRE(p.value() == 30);
            REQUIRE_FALSE(p.initFromArgList(LA(1, 2, "@p", 100)));
            REQUIRE(p.value() == 30);

            p.setArgIndex(0);
            REQUIRE_FALSE(p.initFromArgList(L()));
            REQUIRE(p.initFromArgList(LF(1)));
            REQUIRE(p.initFromArgList(LF(1, 2)));
            REQUIRE(p.initFromArgList(LF(1, 2, 3)));
            REQUIRE(p.value() == 1);
            REQUIRE_FALSE(p.initFromArgList(LA("A", "B")));

            p.setValue(-1000);
            p.setArgIndex(-1);
            REQUIRE_FALSE(p.initFromArgList(L()));
            REQUIRE_FALSE(p.initFromArgList(LF(1)));
            REQUIRE_FALSE(p.initFromArgList(LF(1, 2)));
            REQUIRE_FALSE(p.initFromArgList(LF(1, 2, 3)));
            REQUIRE(p.value() == -1000);
        }

        SECTION("float positional & named")
        {
            FloatProperty p("@f2", 100);

            p.setArgIndex(1);
            REQUIRE(p.initFromArgList(LA(1, 2, "@f1", 3, "@f2", 4)));
            REQUIRE(p.value() == 4);
            p.setArgIndex(0);
            REQUIRE(p.initFromArgList(LA(1, 2, "@f1", 3, "@f2", 4)));
            REQUIRE(p.value() == 4);
            p.setArgIndex(2);
            REQUIRE(p.initFromArgList(LA(1, 2, "@f1", 3, "@f2", 4)));
            REQUIRE(p.value() == 4);
        }
    }
}
