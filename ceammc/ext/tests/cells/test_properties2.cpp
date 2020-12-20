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
#include "ceammc_property_callback.h"
#include "ceammc_property_enum.h"

#include "test_base.h"

using namespace ceammc;

TEST_CASE("Properties2", "[ceammc::properties]")
{
    test::pdPrintToStdError();

    SECTION("enum properties")
    {
        SECTION("symbol")
        {
            SymbolEnumProperty p("@symenum", { "a", "b", "c" });

            SECTION("common checks")
            {
                REQUIRE(p.isReadWrite());
                REQUIRE(p.isPublic());
                REQUIRE(p.isSymbol());

                REQUIRE(p.get() == LA("a"));
                REQUIRE(p.value() == SYM("a"));
                REQUIRE(p.defaultValue() == SYM("a"));
                REQUIRE(p.numEnums() == 3);
            }

            SECTION("invalid gets")
            {
                bool b = true;
                int i = 100;
                t_float f = 0.5;
                Atom a;
                AtomList l({ 1, 2, 3 });

                REQUIRE_FALSE(p.getAtom(a));
                REQUIRE_FALSE(p.getInt(i));
                REQUIRE_FALSE(p.getFloat(f));
                REQUIRE_FALSE(p.getBool(b));
                REQUIRE_FALSE(p.getList(l));
                REQUIRE_FALSE(p.getDefault(b));
                REQUIRE_FALSE(p.getDefault(i));
                REQUIRE_FALSE(p.getDefault(f));
                REQUIRE_FALSE(p.getDefault(a));
                REQUIRE_FALSE(p.getDefault(l));

                REQUIRE(b);
                REQUIRE(i == 100);
                REQUIRE(f == 0.5);
                REQUIRE(a.isNone());
                REQUIRE(l == LF(1, 2, 3));
            }

            SECTION("append")
            {
                REQUIRE(p.numEnums() == 3);
                REQUIRE(p.appendEnum(SYM("d")));
                REQUIRE(p.numEnums() == 4);
                REQUIRE_FALSE(p.appendEnum(SYM("d")));
                REQUIRE(p.numEnums() == 4);
                REQUIRE(p.value() == SYM("a"));

                REQUIRE(p.setValue(SYM("b")));
                REQUIRE(p.get() == LA("b"));
                REQUIRE(p.value() == SYM("b"));
                REQUIRE(p.defaultValue() == SYM("a"));
                REQUIRE(p.setValue(SYM("a")));
                REQUIRE(p.value() == SYM("a"));
                REQUIRE_FALSE(p.setValue(SYM("?")));
                REQUIRE(p.value() == SYM("a"));
            }

            SECTION("index")
            {
                REQUIRE(p.setList(AtomList::parseString("b")));
                REQUIRE(p.value() == SYM("b"));
                REQUIRE(p.setList(AtomList::parseString("2")));
                REQUIRE(p.value() == SYM("c"));
                REQUIRE(p.setList(AtomList::parseString("1")));
                REQUIRE(p.value() == SYM("b"));
                REQUIRE(p.setList(AtomList::parseString("0")));
                REQUIRE(p.value() == SYM("a"));
                REQUIRE_FALSE(p.setList(AtomList::parseString("3")));
                REQUIRE_FALSE(p.setList(AtomList::parseString("-1")));
            }
        }

        SECTION("int")
        {
            IntEnumProperty p("@intenum", { 32, 64, 128 });

            SECTION("common")
            {
                REQUIRE(p.isReadWrite());
                REQUIRE(p.isPublic());
                REQUIRE(p.isInt());

                REQUIRE(p.get() == LF(32));
                REQUIRE(p.value() == 32);
                REQUIRE(p.defaultValue() == 32);
            }

            SECTION("get/set")
            {
                int i = 0;
                REQUIRE(p.setSuccessFn([&i](Property*) { i++; }));
                REQUIRE(p.set(LF(32)));
                REQUIRE(i == 1);
                REQUIRE(p.set(LF(64)));
                REQUIRE(i == 2);
                REQUIRE_FALSE(p.set(LF(63)));
                REQUIRE(i == 2);
            }
        }

        SECTION("atom")
        {
            using AtomEnumProperty = EnumProperty<Atom>;
            AtomEnumProperty p("@atomenum", { Atom(1), Atom(gensym("e")) });

            REQUIRE(p.isAtom());
            REQUIRE(p.get() == Atom(1));
            REQUIRE(p.value() == Atom(1));
            REQUIRE(p.defaultValue() == Atom(1));

            REQUIRE(p.set(LA("e")));
            REQUIRE(p.value() == Atom(gensym("e")));

            REQUIRE_FALSE(p.set(LA("E")));
        }
    }

    SECTION("callback")
    {
        SECTION("bool")
        {
            SECTION("readonly")
            {
                bool value = true;
                CallbackProperty p(
                    "bool", [&]() -> bool { return value; }, nullptr);

                REQUIRE(!p.isReadWrite());
                REQUIRE(!p.isInitOnly());
                REQUIRE(p.isReadOnly());
                REQUIRE(p.isPublic());
                REQUIRE(!p.isHidden());
                REQUIRE(!p.isInternal());

                bool b = false;
                REQUIRE_FALSE(p.getDefault(b));
                p.updateDefault();
                REQUIRE(p.getDefault(b));
                REQUIRE(b == value);

                REQUIRE(p.name() == SYM("bool"));
                REQUIRE(!p.isFloat());
                REQUIRE(p.isBool());
                REQUIRE(!p.isAtom());
                REQUIRE(!p.isInt());
                REQUIRE(!p.isSymbol());
                REQUIRE(!p.isList());

                REQUIRE(p.get() == LF(1));
                value = false;
                REQUIRE(p.get() == LF(0));
                value = true;
                REQUIRE(p.get() == LF(1));

                REQUIRE_FALSE(p.set(L()));
                REQUIRE_FALSE(p.set(LF(1)));
                REQUIRE_FALSE(p.setInit(L()));
                REQUIRE_FALSE(p.setInit(LF(1)));
            }

            SECTION("rw")
            {
                bool value = true;
                CallbackProperty p(
                    "bool",
                    [&]() -> bool { return value; },
                    [&](bool v) -> bool { value = v; return true; });

                REQUIRE(p.isReadWrite());
                REQUIRE(!p.isInitOnly());
                REQUIRE(!p.isReadOnly());

                REQUIRE(p.get() == LF(1));
                value = false;
                REQUIRE(p.get() == LF(0));
                value = true;
                REQUIRE(p.get() == LF(1));

                REQUIRE_FALSE(p.set(L()));
                REQUIRE_FALSE(p.set(LA(1, 2, 3)));
                REQUIRE_FALSE(p.set(LA("TRUE")));

                REQUIRE(p.set(LF(1)));
                REQUIRE(value == true);
                REQUIRE(p.set(LF(0)));
                REQUIRE(value == false);
                REQUIRE(p.set(LA("true")));
                REQUIRE(value == true);
                REQUIRE(p.set(LA("false")));
                REQUIRE(value == false);

                REQUIRE(p.setInit(LF(1)));
                REQUIRE(value == true);
                REQUIRE(p.setInit(LF(0)));
                REQUIRE(value == false);
                REQUIRE(p.setInit(LA("true")));
                REQUIRE(value == true);
                REQUIRE(p.setInit(LA("false")));
                REQUIRE(value == false);

                REQUIRE(p.setList(AtomList::parseString("!")));
                REQUIRE(value == true);
                REQUIRE(p.setList(AtomList::parseString("!")));
                REQUIRE(value == false);

                REQUIRE(p.setList(AtomList::parseString("~")));
                REQUIRE(value == true);
                REQUIRE(p.setList(AtomList::parseString("~")));
                REQUIRE(value == false);
            }
        }

        SECTION("float")
        {
            t_float value = 100;
            CallbackProperty p(
                "float",
                [&]() -> t_float { return value; },
                [&](t_float v) -> bool { value = v; return true; });

            REQUIRE(p.isFloat());
            REQUIRE(p.isReadWrite());
            REQUIRE(!p.isInitOnly());
            REQUIRE(!p.isReadOnly());

            t_float def = -1;
            REQUIRE_FALSE(p.getDefault(def));
            p.updateDefault();
            REQUIRE(p.getDefault(def));
            REQUIRE(def == 100);

            REQUIRE(p.get() == LF(100));
            value = -2.5;
            REQUIRE(p.get() == LF(-2.5));

            REQUIRE(p.set(LF(1.5)));
            REQUIRE(value == 1.5);

            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LF(1, 2)));
            REQUIRE_FALSE(p.set(LA("ABC")));

            value = 0.5;

            SECTION("math")
            {
                SECTION("+")
                {
                    REQUIRE(p.setList(AtomList::parseString("+ 2")));
                    REQUIRE(value == 2.5);
                    REQUIRE(p.setList(AtomList::parseString("+ 2.5")));
                    REQUIRE(value == 5);
                    REQUIRE(p.setList(AtomList::parseString("+ -1")));
                    REQUIRE(value == 4);
                    REQUIRE(p.setList(AtomList::parseString("+ -2.5")));
                    REQUIRE(value == 1.5);
                    REQUIRE(p.setList(AtomList::parseString("+ 0")));
                    REQUIRE(value == 1.5);
                }

                SECTION("-")
                {
                    REQUIRE(p.setList(AtomList::parseString("- 2")));
                    REQUIRE(value == -1.5);
                    REQUIRE(p.setList(AtomList::parseString("- 0.5")));
                    REQUIRE(value == -2);
                    REQUIRE(p.setList(AtomList::parseString("- -1")));
                    REQUIRE(value == -1);
                    REQUIRE(p.setList(AtomList::parseString("- -2.5")));
                    REQUIRE(value == 1.5);
                    REQUIRE(p.setList(AtomList::parseString("- 0")));
                    REQUIRE(value == 1.5);
                }

                SECTION("*")
                {
                    REQUIRE(p.setList(AtomList::parseString("* 4")));
                    REQUIRE(value == 2);
                    REQUIRE(p.setList(AtomList::parseString("* 2.5")));
                    REQUIRE(value == 5);
                    REQUIRE(p.setList(AtomList::parseString("* -1")));
                    REQUIRE(value == -5);
                    REQUIRE(p.setList(AtomList::parseString("* -0.5")));
                    REQUIRE(value == 2.5);
                    REQUIRE(p.setList(AtomList::parseString("* 1")));
                    REQUIRE(value == 2.5);
                    REQUIRE(p.setList(AtomList::parseString("* 0")));
                    REQUIRE(value == 0);
                }

                SECTION("/")
                {
                    REQUIRE(p.setList(AtomList::parseString("/ 2")));
                    REQUIRE(value == 0.25);
                    REQUIRE(p.setList(AtomList::parseString("/ 0.125")));
                    REQUIRE(value == 2);
                    REQUIRE(p.setList(AtomList::parseString("/ -2")));
                    REQUIRE(value == -1);
                    REQUIRE(p.setList(AtomList::parseString("/ -1")));
                    REQUIRE(value == 1);
                    REQUIRE_FALSE(p.setList(AtomList::parseString("/ 0")));
                    REQUIRE(value == 1);
                }
            }
        }

        SECTION("int")
        {
            int value = -1;
            CallbackProperty p(
                "int",
                [&]() -> int { return value; },
                [&](int v) -> bool { value = v; return true; });

            REQUIRE(!p.isFloat());
            REQUIRE(p.isInt());
            REQUIRE(p.isReadWrite());
            REQUIRE(!p.isInitOnly());
            REQUIRE(!p.isReadOnly());

            int def = 1000;
            REQUIRE_FALSE(p.getDefault(def));
            p.updateDefault();
            REQUIRE(p.getDefault(def));
            REQUIRE(def == -1);

            REQUIRE(p.get() == LF(-1));
            value = 1000;
            REQUIRE(p.get() == LF(1000));

            REQUIRE(p.set(LF(1.2)));
            REQUIRE(value == 1);
            REQUIRE(p.set(LF(1.9)));
            REQUIRE(value == 2);
            REQUIRE(p.set(LF(-0.9)));
            REQUIRE(value == -1);

            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LF(1, 2)));
            REQUIRE_FALSE(p.set(LA("ABC")));
            REQUIRE_FALSE(p.setBool(true));
            REQUIRE_FALSE(p.setFloat(100));
            REQUIRE_FALSE(p.setSymbol(SYM("A")));
            REQUIRE_FALSE(p.setAtom(A(123)));
            REQUIRE_FALSE(p.setT(true));
            REQUIRE_FALSE(p.setT(100.f));
            REQUIRE_FALSE(p.setT(SYM("A")));
            REQUIRE_FALSE(p.setT(A(12)));

            REQUIRE(p.setInt(1000));
            REQUIRE(value == 1000);
            REQUIRE(p.setT(int(2000)));
            REQUIRE(value == 2000);

            // getInt/setInt
            int i_;
            REQUIRE(p.setT(1));
            REQUIRE(p.getT(i_));
            REQUIRE(i_ == 1);
            REQUIRE(value == 1);
            REQUIRE(p.setInt(0));
            REQUIRE(p.getInt(i_));
            REQUIRE(i_ == 0);
            REQUIRE(value == 0);

            // zero check
            REQUIRE(p.setIntCheck(PropValueConstraints::NON_ZERO));

            REQUIRE(p.setT(1));
            REQUIRE(p.getT(i_));
            REQUIRE(i_ == 1);
            REQUIRE(value == 1);

            REQUIRE_FALSE(p.setInt(0));
            REQUIRE_FALSE(p.setT(0));
            REQUIRE(p.getT(i_));
            REQUIRE(i_ == 1);
            REQUIRE(value == 1);

            SECTION("math")
            {
                value = 120;

                SECTION("+")
                {
                    REQUIRE(p.setList(AtomList::parseString("+ 2")));
                    REQUIRE(value == 122);
                    REQUIRE(p.setList(AtomList::parseString("+ 2.5")));
                    REQUIRE(value == 124);
                    REQUIRE(p.setList(AtomList::parseString("+ -1")));
                    REQUIRE(value == 123);
                    REQUIRE(p.setList(AtomList::parseString("+ 0")));
                    REQUIRE(value == 123);
                }

                SECTION("-")
                {
                    REQUIRE(p.setList(AtomList::parseString("- 2")));
                    REQUIRE(value == 118);
                    REQUIRE(p.setList(AtomList::parseString("- 0.5")));
                    REQUIRE(value == 118);
                    REQUIRE(p.setList(AtomList::parseString("- -2")));
                    REQUIRE(value == 120);
                    REQUIRE(p.setList(AtomList::parseString("- -2.5")));
                    REQUIRE(value == 122);
                    REQUIRE(p.setList(AtomList::parseString("- 0")));
                    REQUIRE(value == 122);
                }

                SECTION("*")
                {
                    REQUIRE(p.setList(AtomList::parseString("* 2")));
                    REQUIRE(value == 240);
                    REQUIRE(p.setList(AtomList::parseString("* 2.5")));
                    REQUIRE(value == 480);
                    REQUIRE(p.setList(AtomList::parseString("* -1")));
                    REQUIRE(value == -480);
                    REQUIRE(p.setList(AtomList::parseString("* 1")));
                    REQUIRE(value == -480);
                    REQUIRE_FALSE(p.setList(AtomList::parseString("* 0")));
                }

                SECTION("/")
                {
                    REQUIRE(p.setList(AtomList::parseString("/ 2")));
                    REQUIRE(value == 60);
                    REQUIRE(p.setList(AtomList::parseString("/ 5.5")));
                    REQUIRE(value == 12);
                    REQUIRE(p.setList(AtomList::parseString("/ -2")));
                    REQUIRE(value == -6);
                    REQUIRE(p.setList(AtomList::parseString("/ -1")));
                    REQUIRE(value == 6);
                    REQUIRE_FALSE(p.setList(AtomList::parseString("/ 0")));
                    REQUIRE(value == 6);
                    REQUIRE(p.setList(AtomList::parseString("/ 4")));
                    REQUIRE(value == 1);
                }
            }
        }

        SECTION("symbol")
        {
            t_symbol* value = SYM("ABC");
            CallbackProperty p(
                "symbol",
                [&]() -> t_symbol* { return value; },
                [&](t_symbol* v) -> bool { value = v; return true; });

            REQUIRE(!p.isFloat());
            REQUIRE(!p.isInt());
            REQUIRE(p.isSymbol());
            REQUIRE(p.isReadWrite());
            REQUIRE(!p.isInitOnly());
            REQUIRE(!p.isReadOnly());

            t_symbol* def = 0;
            REQUIRE_FALSE(p.getDefault(def));
            p.updateDefault();
            REQUIRE(p.getDefault(def));
            REQUIRE(def == SYM("ABC"));

            REQUIRE(p.get() == LA("ABC"));
            value = SYM("");
            REQUIRE(p.get() == LA(&s_));
            REQUIRE(p.set(LA("@value")));
            REQUIRE(value == SYM("@value"));

            REQUIRE_FALSE(p.set(L()));
            REQUIRE_FALSE(p.set(LA(1, 2)));
            REQUIRE_FALSE(p.set(LA(1)));
            REQUIRE_FALSE(p.set(LA("A", "B")));

            REQUIRE(value == SYM("@value"));
        }

        SECTION("atom")
        {
            Atom value(-200);
            CallbackProperty p(
                "atom",
                [&]() -> Atom { return value; },
                [&](const Atom& v) -> bool { value = v; return true; });

            REQUIRE(!p.isFloat());
            REQUIRE(!p.isInt());
            REQUIRE(!p.isSymbol());
            REQUIRE(p.isAtom());
            REQUIRE(p.isReadWrite());
            REQUIRE(!p.isInitOnly());
            REQUIRE(!p.isReadOnly());

            Atom def;
            REQUIRE_FALSE(p.getDefault(def));
            p.updateDefault();
            REQUIRE(p.getDefault(def));
            REQUIRE(def == Atom(-200));

            REQUIRE(p.get() == LA(-200));
            value = S("ABC");
            REQUIRE(p.get() == LA("ABC"));
            REQUIRE(p.set(LA("@value")));
            REQUIRE(value == S("@value"));
            REQUIRE_FALSE(p.set(LA(1, 2)));
            REQUIRE_FALSE(p.set(L()));

            REQUIRE_FALSE(p.setBool(true));
            REQUIRE_FALSE(p.setFloat(100.f));
            REQUIRE_FALSE(p.setInt(100));
            REQUIRE_FALSE(p.setSymbol(SYM("a")));
            REQUIRE_FALSE(p.setList(LF(1, 2)));

            REQUIRE(p.setList(LF(1)));
            REQUIRE(p.setList(LA("A")));
            REQUIRE(p.setAtom(A(123)));
            REQUIRE(value == A(123));

            p.setReadOnly();
            REQUIRE(p.setAtom(A(-100)));
            REQUIRE(value == A(-100));
            REQUIRE(p.setT(A(-200)));
            REQUIRE(value == A(-200));

            bool b;
            t_float f;
            int i;
            t_symbol* s;
            Atom a;
            AtomList l;
            REQUIRE_FALSE(p.getBool(b));
            REQUIRE_FALSE(p.getInt(i));
            REQUIRE_FALSE(p.getFloat(f));
            REQUIRE_FALSE(p.getSymbol(s));
            REQUIRE_FALSE(p.getList(l));
            REQUIRE_FALSE(p.getT(b));
            REQUIRE_FALSE(p.getT(i));
            REQUIRE_FALSE(p.getT(f));
            REQUIRE_FALSE(p.getT(s));
            REQUIRE_FALSE(p.getT(l));

            REQUIRE(p.getAtom(a));
            REQUIRE(a == value);
            REQUIRE(p.setT(A(1000)));
            REQUIRE(p.getT(a));
            REQUIRE(a == A(1000));

            SECTION("null callbacks")
            {
                Atom v2;
                CallbackProperty p(
                    "@atom-null", [&]() -> Atom { return v2; }, nullptr);
                REQUIRE(p.isReadOnly());
                REQUIRE_FALSE(p.setBool(true));
                REQUIRE_FALSE(p.setFloat(1));
                REQUIRE_FALSE(p.setInt(100));
                REQUIRE_FALSE(p.setSymbol(SYM("asd")));
                REQUIRE_FALSE(p.setAtom(A(123)));
                REQUIRE_FALSE(p.setList(LF(1)));
                REQUIRE(v2.isNone());
            }
        }

        SECTION("list")
        {
            AtomList lst = LF(1, 2, 3);
            CallbackProperty p(
                "@list",
                [&]() -> AtomList { return lst; },
                [&](const AtomList& v) -> bool { lst = v; return true; });

            REQUIRE(!p.isBool());
            REQUIRE(!p.isFloat());
            REQUIRE(!p.isInt());
            REQUIRE(!p.isSymbol());
            REQUIRE(!p.isAtom());
            REQUIRE(p.isList());
            REQUIRE(p.isReadWrite());
            REQUIRE(!p.isInitOnly());
            REQUIRE(!p.isReadOnly());

            AtomList def;
            REQUIRE_FALSE(p.getDefault(def));
            p.updateDefault();
            REQUIRE(p.getDefault(def));
            REQUIRE(def == LF(1, 2, 3));

            REQUIRE_FALSE(p.setBool(true));
            REQUIRE_FALSE(p.setFloat(10));
            REQUIRE_FALSE(p.setInt(-10));
            REQUIRE_FALSE(p.setSymbol(SYM("ABC")));
            REQUIRE_FALSE(p.setAtom(A(123)));

            REQUIRE_FALSE(p.setT(true));
            REQUIRE_FALSE(p.setT(10.f));
            REQUIRE_FALSE(p.setT(-10));
            REQUIRE_FALSE(p.setT(SYM("ABC")));
            REQUIRE_FALSE(p.setT(A(123)));

            REQUIRE(p.get() == LF(1, 2, 3));
            REQUIRE(p.setList(L()));
            REQUIRE(p.get() == L());
            REQUIRE(p.set(LF(1, 2)));
            REQUIRE(p.get() == LF(1, 2));
            REQUIRE(p.set(L()));
            REQUIRE(p.get() == L());

            bool b;
            t_float f;
            int i;
            t_symbol* s;
            Atom a;
            AtomList l;
            REQUIRE_FALSE(p.getBool(b));
            REQUIRE_FALSE(p.getInt(i));
            REQUIRE_FALSE(p.getFloat(f));
            REQUIRE_FALSE(p.getSymbol(s));
            REQUIRE_FALSE(p.getAtom(a));
            REQUIRE_FALSE(p.getT(b));
            REQUIRE_FALSE(p.getT(i));
            REQUIRE_FALSE(p.getT(f));
            REQUIRE_FALSE(p.getT(s));
            REQUIRE_FALSE(p.getT(a));
            REQUIRE(p.getList(l));
            REQUIRE(lst == l);
            lst.clear();
            REQUIRE(p.getT(l));
            REQUIRE(lst == l);
        }

        SECTION("check float")
        {
            FloatProperty p("@f", 100);

            p.setFloatCheck(PropValueConstraints::NON_ZERO);
            REQUIRE(p.setValue(0.001));
            REQUIRE_FALSE(p.setValue(0));
            REQUIRE(p.setValue(-0.0001));

            p.setFloatCheck(PropValueConstraints::NONE);
            REQUIRE(p.setValue(0.001));
            REQUIRE(p.setValue(0));
            REQUIRE(p.setValue(-0.0001));

            p.setFloatCheck(PropValueConstraints::GREATER_EQUAL, -2.5);
            REQUIRE_FALSE(p.setValue(-2.6));
            REQUIRE(p.setValue(-2.5));
            REQUIRE(p.setValue(-2));

            p.setFloatCheck(PropValueConstraints::GREATER_THEN, 1);
            REQUIRE_FALSE(p.setValue(0.9));
            REQUIRE_FALSE(p.setValue(1));
            REQUIRE(p.setValue(1.1));

            p.setFloatCheck(PropValueConstraints::LESS_THEN, 1);
            REQUIRE(p.setValue(0.9));
            REQUIRE_FALSE(p.setValue(1));
            REQUIRE_FALSE(p.setValue(1.1));

            p.setFloatCheck(PropValueConstraints::LESS_EQUAL, 1);
            REQUIRE(p.setValue(0.9));
            REQUIRE(p.setValue(1));
            REQUIRE_FALSE(p.setValue(1.1));

            p.setFloatCheck(PropValueConstraints::CLOSED_RANGE, -1, 1);
            REQUIRE_FALSE(p.setValue(-1.1));
            REQUIRE(p.setValue(-1));
            REQUIRE(p.setValue(-0.9));
            REQUIRE(p.setValue(0));
            REQUIRE(p.setValue(0.9));
            REQUIRE(p.setValue(1));
            REQUIRE_FALSE(p.setValue(1.1));

            p.setFloatCheck(PropValueConstraints::CLOSED_OPEN_RANGE, -1, 1);
            REQUIRE_FALSE(p.setValue(-1.1));
            REQUIRE(p.setValue(-1));
            REQUIRE(p.setValue(-0.9));
            REQUIRE(p.setValue(0));
            REQUIRE(p.setValue(0.9));
            REQUIRE_FALSE(p.setValue(1));
            REQUIRE_FALSE(p.setValue(1.1));

            p.setFloatCheck(PropValueConstraints::OPEN_RANGE, -1, 1);
            REQUIRE_FALSE(p.setValue(-1.1));
            REQUIRE_FALSE(p.setValue(-1));
            REQUIRE(p.setValue(-0.9));
            REQUIRE(p.setValue(0));
            REQUIRE(p.setValue(0.9));
            REQUIRE_FALSE(p.setValue(1));
            REQUIRE_FALSE(p.setValue(1.1));

            p.setFloatCheck(PropValueConstraints::OPEN_CLOSED_RANGE, -1, 1);
            REQUIRE_FALSE(p.setValue(-1.1));
            REQUIRE_FALSE(p.setValue(-1));
            REQUIRE(p.setValue(-0.9));
            REQUIRE(p.setValue(0));
            REQUIRE(p.setValue(0.9));
            REQUIRE(p.setValue(1));
            REQUIRE_FALSE(p.setValue(1.1));
        }

        SECTION("check int")
        {
            IntProperty p("@i", 100);

            p.setIntCheck(PropValueConstraints::NON_ZERO);
            REQUIRE(p.setValue(1));
            REQUIRE_FALSE(p.setValue(0));
            REQUIRE(p.setValue(-1));

            p.setIntCheck(PropValueConstraints::NONE);
            REQUIRE(p.setValue(-1));
            REQUIRE(p.setValue(0));
            REQUIRE(p.setValue(1));

            p.setIntCheck(PropValueConstraints::GREATER_EQUAL, -3);
            REQUIRE_FALSE(p.setValue(-4));
            REQUIRE(p.setValue(-3));
            REQUIRE(p.setValue(-2));

            p.setIntCheck(PropValueConstraints::GREATER_THEN, 1);
            REQUIRE_FALSE(p.setValue(0));
            REQUIRE_FALSE(p.setValue(1));
            REQUIRE(p.setValue(2));

            p.setIntCheck(PropValueConstraints::LESS_THEN, 1);
            REQUIRE(p.setValue(0));
            REQUIRE_FALSE(p.setValue(1));
            REQUIRE_FALSE(p.setValue(2));

            p.setIntCheck(PropValueConstraints::LESS_EQUAL, 1);
            REQUIRE(p.setValue(0));
            REQUIRE(p.setValue(1));
            REQUIRE_FALSE(p.setValue(2));

            p.setIntCheck(PropValueConstraints::CLOSED_RANGE, -1, 1);
            REQUIRE_FALSE(p.setValue(-2));
            REQUIRE(p.setValue(-1));
            REQUIRE(p.setValue(0));
            REQUIRE(p.setValue(1));
            REQUIRE_FALSE(p.setValue(2));

            p.setIntCheck(PropValueConstraints::CLOSED_OPEN_RANGE, -1, 1);
            REQUIRE_FALSE(p.setValue(-2));
            REQUIRE(p.setValue(-1));
            REQUIRE(p.setValue(0));
            REQUIRE_FALSE(p.setValue(1));
            REQUIRE_FALSE(p.setValue(2));

            p.setIntCheck(PropValueConstraints::OPEN_RANGE, -2, 2);
            REQUIRE_FALSE(p.setValue(-2));
            REQUIRE(p.setValue(-1));
            REQUIRE(p.setValue(0));
            REQUIRE(p.setValue(1));
            REQUIRE_FALSE(p.setValue(2));

            p.setIntCheck(PropValueConstraints::OPEN_CLOSED_RANGE, -1, 1);
            REQUIRE_FALSE(p.setValue(-2));
            REQUIRE_FALSE(p.setValue(-1));
            REQUIRE(p.setValue(0));
            REQUIRE(p.setValue(1));
            REQUIRE_FALSE(p.setValue(2));
        }
    }
}
