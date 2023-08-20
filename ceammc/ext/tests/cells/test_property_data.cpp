/*****************************************************************************
 * Copyright 2023 Serge Poltavsky. All rights reserved.
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
#include "ceammc_property_data.h"
#include "datatype_dict.h"
#include "datatype_string.h"
#include "test_property.h"

using StringProperty = DataPropertyT<DataTypeString>;
using StringListProperty = DataPropertyListT<DataTypeString>;
using StrVec = std::vector<DataTypeString>;

TEST_CASE("DataPropertyT", "[core]")
{
    test::pdPrintToStdError();

    SECTION("DataPropertyT")
    {
        StringProperty p("@data", DataTypeString("TEST"));

        SECTION("main")
        {
            REQUIRE(p.isReadWrite());
            REQUIRE_FALSE(p.isInitOnly());
            REQUIRE_FALSE(p.isReadOnly());
            REQUIRE(p.isPublic());
            REQUIRE_FALSE(p.isHidden());
            REQUIRE_FALSE(p.isInternal());

            REQUIRE(p.name() == SYM("@data"));
            REQUIRE(!p.isFloat());
            REQUIRE(!p.isBool());
            REQUIRE(p.isAtom());
            REQUIRE(!p.isInt());
            REQUIRE(!p.isSymbol());
            REQUIRE(!p.isList());
            REQUIRE(p.type() == PropValueType::ATOM);
            REQUIRE(p.equalUnit(PropValueUnits::NONE));
            REQUIRE(p.access() == PropValueAccess::READWRITE);
            REQUIRE(p.view() == PropValueView::ENTRY);
            REQUIRE(p.value() == DataTypeString("TEST"));
            REQUIRE(p.get() == StringAtom("TEST"));
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
            REQUIRE(p.set(StringAtom("test")));
            REQUIRE(p.value() == DataTypeString("test"));
            REQUIRE(p.set(LA("String(ABC)")));
            REQUIRE(p.value() == DataTypeString("ABC"));

            REQUIRE_FALSE(p.setFloatCheckFn(nullptr));
            REQUIRE_FALSE(p.setIntCheckFn(nullptr));
            REQUIRE(p.setAtomCheckFn(nullptr));
            REQUIRE_FALSE(p.setSymbolCheckFn(nullptr));
            REQUIRE_FALSE(p.setListCheckFn(nullptr));

            CHECK_READONLY(p, StringAtom(""));
            CHECK_INITONLY(p, StringAtom(""));
        }

        SECTION("ok")
        {
            int cnt = 0;
            p.setSuccessFn([&cnt](Property* p) { cnt++; });
            REQUIRE(p.set(StringAtom("++")));
            REQUIRE(cnt == 1);
            REQUIRE(p.set(StringAtom("__")));
            REQUIRE(cnt == 2);
        }

        SECTION("getAtom/getT")
        {
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
            REQUIRE(p.getT(a));
            REQUIRE(a == StringAtom("TEST"));
            REQUIRE_FALSE(p.getT(l));

            REQUIRE_FALSE(p.getBool(b));
            REQUIRE_FALSE(p.getFloat(f));
            REQUIRE_FALSE(p.getInt(i));
            REQUIRE_FALSE(p.getSymbol(s));
            REQUIRE_FALSE(p.getList(l));

            a = Atom();
            REQUIRE(p.getAtom(a));
            REQUIRE(a == StringAtom("TEST"));
        }

        SECTION("setAtom/setT")
        {
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

            REQUIRE_FALSE(p.setAtom(DictAtom("[dict: 1]")));
            REQUIRE(p.setAtom(StringAtom("@@@")));
        }
    }

    SECTION("DataPropertyListT")
    {
        StringListProperty p("@datalist", { DataTypeString("A"), DataTypeString("B") });

        SECTION("main")
        {
            REQUIRE(p.isReadWrite());
            REQUIRE_FALSE(p.isInitOnly());
            REQUIRE_FALSE(p.isReadOnly());
            REQUIRE(p.isPublic());
            REQUIRE_FALSE(p.isHidden());
            REQUIRE_FALSE(p.isInternal());

            REQUIRE(p.name() == SYM("@datalist"));
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
            REQUIRE(p.value() == StrVec { DataTypeString("A"), DataTypeString("B") });
            REQUIRE(p.get() == LA(StringAtom("A"), StringAtom("B")));
        }

        SECTION("invalid set")
        {
            REQUIRE(p.set(L()));
            REQUIRE_FALSE(p.set(LF(1, 2)));
            REQUIRE_FALSE(p.set(LA("A")));
            REQUIRE_FALSE(p.set(LA("A", "B")));
        }

        SECTION("misc")
        {
            REQUIRE(p.set(StringAtom("test")));
            REQUIRE(p.value() == StrVec { DataTypeString("test") });
            REQUIRE(p.set(LA("String(ABC)")));
            REQUIRE(p.value() == StrVec { DataTypeString("ABC") });

            REQUIRE(p.setFloatCheckFn(nullptr));
            REQUIRE(p.setIntCheckFn(nullptr));
            REQUIRE(p.setAtomCheckFn(nullptr));
            REQUIRE(p.setSymbolCheckFn(nullptr));
            REQUIRE(p.setListCheckFn(nullptr));

            CHECK_READONLY(p, StringAtom(""));
            CHECK_INITONLY(p, StringAtom(""));
        }

        SECTION("ok")
        {
            int cnt = 0;
            p.setSuccessFn([&cnt](Property* p) { cnt++; });
            REQUIRE(p.set(StringAtom("++")));
            REQUIRE(cnt == 1);
            REQUIRE(p.set(StringAtom("__")));
            REQUIRE(cnt == 2);
        }

        SECTION("getAtom/getT")
        {
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
            REQUIRE(l == LA(StringAtom("A"), StringAtom("B")));

            REQUIRE_FALSE(p.getBool(b));
            REQUIRE_FALSE(p.getFloat(f));
            REQUIRE_FALSE(p.getInt(i));
            REQUIRE_FALSE(p.getAtom(a));
            REQUIRE_FALSE(p.getSymbol(s));

            REQUIRE(p.getList(l));
            REQUIRE(l == LA(StringAtom("A"), StringAtom("B")));
        }

        SECTION("setList/setT")
        {
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

            REQUIRE(p.setList(L()));
            REQUIRE(p.value().empty());

            REQUIRE_FALSE(p.setList(LF(1)));
            REQUIRE_FALSE(p.setList(LA("ABC")));
            REQUIRE(p.setList(LA("S\"ABC\"")));
            REQUIRE(p.get() == LA(StringAtom("ABC")));
            REQUIRE(p.setList(LA("S\"ABC\"", "S\"DEF\"")));
            REQUIRE(p.get() == LA(StringAtom("ABC"), StringAtom("DEF")));
            REQUIRE_FALSE(p.setList(LA("S\"ABC\"", "DEF")));
            REQUIRE(p.setList(LA(StringAtom("ABC"))));
            REQUIRE(p.get() == LA(StringAtom("ABC")));
            REQUIRE_FALSE(p.setList(DictAtom("[dict: 1]")));
            REQUIRE_FALSE(p.setList(LA("[a: 1]")));
        }
    }
}
