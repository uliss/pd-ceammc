/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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

TEST_CASE("ListProperty", "[ceammc::properties]")
{
    test::pdPrintToStdError();

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
        REQUIRE(p.equalUnit(PropValueUnits::NONE));
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
    }

    SECTION("check fn")
    {
        ListProperty p("@l", LF(1, 2));

        REQUIRE(p.setIntCheckFn(nullptr));
        REQUIRE(p.setAtomCheckFn(nullptr));
        REQUIRE(p.setSymbolCheckFn(nullptr));

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

    SECTION("checkRanges")
    {
        SECTION("check min")
        {
            ListProperty p("@l");
            REQUIRE(p.checkMinElementCount(3));

            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LF(1)));
            REQUIRE_FALSE(p.set(LF(1, 2)));

            REQUIRE(p.set(LF(1, 2, 3)));
            REQUIRE(p.get() == LF(1, 2, 3));

            REQUIRE(p.set(LF(1, 2, 3, 4)));
        }

        SECTION("check max")
        {
            ListProperty p("@l");
            REQUIRE(p.checkMaxElementCount(2));

            REQUIRE(p.set(LF(1)));
            REQUIRE(p.set(LF(1, 2)));
            REQUIRE(p.get() == LF(1, 2));
            REQUIRE(p.set(L()));
            REQUIRE(p.get() == L());

            REQUIRE_FALSE(p.set(LF(1, 2, 3)));
            REQUIRE(p.get() == L());
        }

        SECTION("check range")
        {
            ListProperty p("@l");
            REQUIRE(p.checkRangeElementCount(1, 1));

            REQUIRE(p.set(LF(2)));
            REQUIRE(p.get() == LF(2));

            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LF(1, 2)));
            REQUIRE(p.get() == LF(2));

            REQUIRE(p.checkRangeElementCount(1, 3));
            REQUIRE(p.set(LF(1)));
            REQUIRE(p.set(LF(1, 2)));
            REQUIRE(p.set(LF(1, 2, 3)));
            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LF(1, 2, 3, 4)));
        }
    }

    SECTION("filter")
    {
        ListProperty p("@l");

        SECTION("float")
        {
            p.setFilterAtomFn(isFloat);
            REQUIRE(p.set(LA(1, 2.5, "a", 3)));
            REQUIRE(p.get() == LF(1, 2.5, 3));
        }

        SECTION("int")
        {
            p.acceptIntegers();
            REQUIRE(p.set(LA(1, 2.5, "a", 3)));
            REQUIRE(p.get() == LF(1, 3));
        }

        SECTION("symbol")
        {
            p.acceptSymbols();
            REQUIRE(p.set(LA(1, 2.5, "a", 3)));
            REQUIRE(p.get() == LA("a"));
        }

        SECTION("float")
        {
            p.acceptFloats();
            REQUIRE(p.set(LA(1, 2.5, "a", 3)));
            REQUIRE(p.get() == LF(1, 2.5, 3));
        }
    }

    SECTION("apply")
    {
        ListProperty p("@l");

        SECTION("apply")
        {
            p.setMapAtomFn([](const Atom& a) { return a.isSymbol() ? A("S") : A("."); });
            REQUIRE(p.set(LA(1, 2.5, "a", 3)));
            REQUIRE(p.get() == LA(".", ".", "S", "."));
        }

        SECTION("round")
        {
            p.roundFloats();
            REQUIRE(p.set(LA(1, 2.6, "a", 3)));
            REQUIRE(p.get() == LA(1, 3, "a", 3));
        }

        SECTION("trunc")
        {
            p.truncateFloats();
            REQUIRE(p.set(LA(1, 2.6, "a", 3, -2.1)));
            REQUIRE(p.get() == LA(1, 2, "a", 3, -2));
        }
    }

    SECTION("json")
    {
        ListProperty p("@l");
        p.setDefault(LA(3, 2, "a"));

        std::string json;
        REQUIRE(p.infoT().getJSON(json));
        REQUIRE(json == R"({"access":"readwrite","default":[3,2,"a"],"name":"@l","type":"list","view":"entry","visibility":"public"})");
    }
}
