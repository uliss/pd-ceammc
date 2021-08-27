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

TEST_CASE("BoolProperty", "[core]")
{
    test::pdPrintToStdError();

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
        REQUIRE(p.units() == PropValueUnits::NONE);
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

    SECTION("misc")
    {
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
    }

    SECTION("ok")
    {
        int cnt = 0;
        p.setSuccessFn([&cnt](Property* p) { cnt++; });
        REQUIRE(p.set(LA("true")));
        REQUIRE(cnt == 1);
        REQUIRE(p.set(LA("true")));
        REQUIRE(cnt == 2);
    }

    SECTION("getBool/getT")
    {
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

    SECTION("!")
    {
        REQUIRE(p.setList(AtomList::parseString("!")));
        REQUIRE(p.value() == false);
        REQUIRE(p.setList(AtomList::parseString("!")));
        REQUIRE(p.value() == true);
        REQUIRE(p.setList(AtomList::parseString("~")));
        REQUIRE(p.value() == false);
        REQUIRE(p.setList(AtomList::parseString("~")));
        REQUIRE(p.value() == true);
    }

    SECTION("random")
    {
        REQUIRE(p.setList(AtomList::parseString("random")));
    }
}
