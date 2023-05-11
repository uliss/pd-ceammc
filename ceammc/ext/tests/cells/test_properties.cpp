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
#include "test_property.h"

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

TEST_CASE("Properties", "[ceammc::properties]")
{
    test::pdPrintToStdError();

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
        REQUIRE(p.type() == PropValueType::ATOM);
        REQUIRE(p.units() == PropValueUnits::NONE);
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
            REQUIRE(p.units() == PropValueUnits::NONE);
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
            REQUIRE(p.units() == PropValueUnits::NONE);
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
            REQUIRE(p.units() == PropValueUnits::NONE);
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

    SECTION("callSuccessFn")
    {
        FloatProperty p("float", 100);
        REQUIRE_FALSE(p.callSuccessFn());

        REQUIRE(p.setSuccessFn(nullptr));
        REQUIRE_FALSE(p.callSuccessFn());
    }
}
