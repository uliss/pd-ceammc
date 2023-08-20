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

TEST_CASE("SymbolProperty", "[core]")
{
    test::pdPrintToStdError();

    SymbolProperty p("@sym", SYM("AAAA"));

    SECTION("symbol property")
    {
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
        REQUIRE(p.equalUnit(PropValueUnits::NONE));
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
    }

    SECTION("check fn")
    {
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
