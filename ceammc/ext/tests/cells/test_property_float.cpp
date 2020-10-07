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

TEST_CASE("FloatProperty", "[core]")
{
    test::pdPrintToStdError();

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
        REQUIRE(p.units() == PropValueUnits::NONE);
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

    SECTION("+")
    {
        REQUIRE(p.setList(AtomList::parseString("+ 2")));
        REQUIRE(p.value() == 2.5);
        REQUIRE(p.setList(AtomList::parseString("+ 2.5")));
        REQUIRE(p.value() == 5);
        REQUIRE(p.setList(AtomList::parseString("+ -1")));
        REQUIRE(p.value() == 4);
        REQUIRE(p.setList(AtomList::parseString("+ -2.5")));
        REQUIRE(p.value() == 1.5);
        REQUIRE(p.setList(AtomList::parseString("+ 0")));
        REQUIRE(p.value() == 1.5);

        p.checkClosedRange(1, 2);
        REQUIRE(p.setList(AtomList::parseString("+ 0.5")));
        REQUIRE(p.value() == 2);
        REQUIRE_FALSE(p.setList(AtomList::parseString("+ 0.001")));
        REQUIRE_FALSE(p.setList(AtomList::parseString("+ 1000")));
    }

    SECTION("-")
    {
        REQUIRE(p.setList(AtomList::parseString("- 2")));
        REQUIRE(p.value() == -1.5);
        REQUIRE(p.setList(AtomList::parseString("- 0.5")));
        REQUIRE(p.value() == -2);
        REQUIRE(p.setList(AtomList::parseString("- -1")));
        REQUIRE(p.value() == -1);
        REQUIRE(p.setList(AtomList::parseString("- -2.5")));
        REQUIRE(p.value() == 1.5);
        REQUIRE(p.setList(AtomList::parseString("- 0")));
        REQUIRE(p.value() == 1.5);
    }

    SECTION("*")
    {
        REQUIRE(p.setList(AtomList::parseString("* 4")));
        REQUIRE(p.value() == 2);
        REQUIRE(p.setList(AtomList::parseString("* 2.5")));
        REQUIRE(p.value() == 5);
        REQUIRE(p.setList(AtomList::parseString("* -1")));
        REQUIRE(p.value() == -5);
        REQUIRE(p.setList(AtomList::parseString("* -0.5")));
        REQUIRE(p.value() == 2.5);
        REQUIRE(p.setList(AtomList::parseString("* 1")));
        REQUIRE(p.value() == 2.5);
        REQUIRE(p.setList(AtomList::parseString("* 0")));
        REQUIRE(p.value() == 0);
    }

    SECTION("/")
    {
        REQUIRE(p.setList(AtomList::parseString("/ 2")));
        REQUIRE(p.value() == 0.25);
        REQUIRE(p.setList(AtomList::parseString("/ 0.125")));
        REQUIRE(p.value() == 2);
        REQUIRE(p.setList(AtomList::parseString("/ -2")));
        REQUIRE(p.value() == -1);
        REQUIRE(p.setList(AtomList::parseString("/ -1")));
        REQUIRE(p.value() == 1);
        REQUIRE_FALSE(p.setList(AtomList::parseString("/ 0")));
        REQUIRE(p.value() == 1);
    }

    SECTION("denormals")
    {
        REQUIRE_FALSE(p.setList(LF(std::numeric_limits<t_float>::infinity())));
        REQUIRE(p.value() == 0.5);
        REQUIRE_FALSE(p.setList(LF(std::numeric_limits<t_float>::signaling_NaN())));
        REQUIRE(p.value() == 0.5);
        REQUIRE_FALSE(p.setList(LF(std::numeric_limits<t_float>::quiet_NaN())));
        REQUIRE(p.value() == 0.5);
        REQUIRE_FALSE(p.setList(LF(std::numeric_limits<t_float>::min() / 2)));
        REQUIRE(p.value() == 0.5);
    }
}
