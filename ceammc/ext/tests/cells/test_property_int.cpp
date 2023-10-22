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

TEST_CASE("IntProperty", "[core]")
{
    test::pdPrintToStdError();
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
        REQUIRE(p.equalUnit(PropValueUnits::NONE));
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

    SECTION("+")
    {
        REQUIRE(p.setList(AtomList::parseString("+ 2")));
        REQUIRE(p.value() == 122);
        REQUIRE(p.setList(AtomList::parseString("+ 2.5")));
        REQUIRE(p.value() == 124);
        REQUIRE(p.setList(AtomList::parseString("+ -1")));
        REQUIRE(p.value() == 123);
        REQUIRE(p.setList(AtomList::parseString("+ 0")));
        REQUIRE(p.value() == 123);

        p.checkClosedRange(100, 200);
        REQUIRE_FALSE(p.setList(AtomList::parseString("+ 1000")));
        REQUIRE(p.value() == 123);
    }

    SECTION("-")
    {
        REQUIRE(p.setList(AtomList::parseString("- 2")));
        REQUIRE(p.value() == 118);
        REQUIRE(p.setList(AtomList::parseString("- 0.5")));
        REQUIRE(p.value() == 118);
        REQUIRE(p.setList(AtomList::parseString("- -2")));
        REQUIRE(p.value() == 120);
        REQUIRE(p.setList(AtomList::parseString("- -2.5")));
        REQUIRE(p.value() == 122);
        REQUIRE(p.setList(AtomList::parseString("- 0")));
        REQUIRE(p.value() == 122);
    }

    SECTION("*")
    {
        REQUIRE(p.setList(AtomList::parseString("* 2")));
        REQUIRE(p.value() == 240);
        REQUIRE(p.setList(AtomList::parseString("* 2.5")));
        REQUIRE(p.value() == 480);
        REQUIRE(p.setList(AtomList::parseString("* -1")));
        REQUIRE(p.value() == -480);
        REQUIRE(p.setList(AtomList::parseString("* 1")));
        REQUIRE(p.value() == -480);
        REQUIRE(p.setList(AtomList::parseString("* 0")));
        REQUIRE(p.value() == 0);
    }

    SECTION("/")
    {
        REQUIRE(p.setList(AtomList::parseString("/ 2")));
        REQUIRE(p.value() == 60);
        REQUIRE(p.setList(AtomList::parseString("/ 5.5")));
        REQUIRE(p.value() == 12);
        REQUIRE(p.setList(AtomList::parseString("/ -2")));
        REQUIRE(p.value() == -6);
        REQUIRE(p.setList(AtomList::parseString("/ -1")));
        REQUIRE(p.value() == 6);
        REQUIRE_FALSE(p.setList(AtomList::parseString("/ 0")));
        REQUIRE(p.value() == 6);
    }

    SECTION("%")
    {
        p.setValue(15);
        REQUIRE(p.setList(AtomList::parseString("% 10")));
        REQUIRE(p.value() == 5);

        REQUIRE(p.setList(AtomList::parseString("% 2")));
        REQUIRE(p.value() == 1);

        REQUIRE_FALSE(p.setList(AtomList::parseString("% 0")));
    }

    SECTION("random")
    {
        REQUIRE_FALSE(p.setList(AtomList::parseString("random")));

        p.setDefault(50);
        p.checkClosedRange(0, 1000);
        REQUIRE(p.setList(AtomList::parseString("random")));
        auto v = p.value();
        REQUIRE(p.setList(AtomList::parseString("random")));
        // hope so)
        REQUIRE(v != p.value());

        // wrong args
        REQUIRE_FALSE(p.setList(AtomList::parseString("random 10")));

        //
        REQUIRE(p.setList(AtomList::parseString("random 10 11")));
        REQUIRE((p.value() == 10 || p.value() == 11));
    }

    SECTION("default")
    {
        REQUIRE(p.setList(AtomList::parseString("125")));
        REQUIRE(p.value() == 125);
        REQUIRE(p.setList(AtomList::parseString("default")));
        REQUIRE(p.value() == 120);
        REQUIRE(p.setList(AtomList::parseString("30")));
        REQUIRE(p.value() == 30);
        REQUIRE(p.setList(AtomList::parseString("def")));
        REQUIRE(p.value() == 120);
    }
}
