/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "ceammc_datatypes.h"
#include "datatype_property.h"
#include "test_common.h"

using namespace ceammc;

TEST_CASE("DataTypeProperty", "[ceammc::DataTypeProperty]")
{
    SECTION("init tests")
    {
        REQUIRE(DataTypeProperty::dataType == data::DATA_PROPERTY);
        DataTypeProperty t(SYM("test"));
        REQUIRE(t.type() == data::DATA_PROPERTY);

        REQUIRE(t.isFloat());
        REQUIRE_FALSE(t.isBool());
        REQUIRE_FALSE(t.isInt());
        REQUIRE_FALSE(t.isList());
        REQUIRE_FALSE(t.isSymbol());
    }

    SECTION("clone")
    {
        DataTypeProperty f1(SYM("f1"));
        REQUIRE(f1.name() == SYM("f1"));
        f1.setTypeInt(4);
        f1.setIntRange(3, 5);

        DataTypeProperty* f2 = f1.clone();
        REQUIRE(f1.name() == f2->name());
        REQUIRE(f1.intRange() == f2->intRange());
        REQUIRE(f1.floatRange() == f2->floatRange());
        REQUIRE(f1.enumValues() == f2->enumValues());

        REQUIRE(f2->isInt());
        int i;
        REQUIRE(f2->getInt(i));
        REQUIRE(i == 4);
        f2->setInt(6);
        REQUIRE(f2->getInt(i));
        REQUIRE(i == 5);
        f2->setInt(2);
        REQUIRE(f2->getInt(i));
        REQUIRE(i == 3);
        delete f2;
    }

    SECTION("float")
    {
        DataTypeProperty f1(SYM("f1"));
        t_float f1_;
        REQUIRE(f1.getFloat(f1_));
        REQUIRE(f1_ == 0.0);

        REQUIRE_FALSE(f1.setBool(0));
        REQUIRE_FALSE(f1.setInt(0));
        REQUIRE_FALSE(f1.setSymbol(&s_list));
        REQUIRE_FALSE(f1.setList(L()));

        REQUIRE(f1.setFloat(100.f));
        REQUIRE(f1.getFloat(f1_));
        REQUIRE(f1_ == 100.f);

        REQUIRE_FALSE(f1.setBool(false));
        REQUIRE_FALSE(f1.setInt(100));
        REQUIRE_FALSE(f1.setList(AtomList()));
        REQUIRE_FALSE(f1.setSymbol(SYM("100")));

        f1.restoreDefault();
        REQUIRE(f1.getFloat(f1_));
        REQUIRE(f1_ == 0.f);

        REQUIRE_FALSE(f1.setEnumValues(LF(1, 2)));
        REQUIRE_FALSE(f1.setIntRange(1, 2));
        REQUIRE(f1.setFloatRange(10, 20));
        f1.restoreDefault();
        REQUIRE(f1.getFloat(f1_));
        REQUIRE(f1_ == 10.f);

        REQUIRE(f1.setFloat(100.f));
        REQUIRE(f1.getFloat(f1_));
        REQUIRE(f1_ == 20.f);

        REQUIRE(f1.setFloat(19.99));
        REQUIRE(f1.getFloat(f1_));
        REQUIRE(f1_ == Approx(19.99));

        REQUIRE(f1.setFloat(10.01));
        REQUIRE(f1.getFloat(f1_));
        REQUIRE(f1_ == Approx(10.01));

        REQUIRE(f1.setFloat(9.99));
        REQUIRE(f1.getFloat(f1_));
        REQUIRE(f1_ == Approx(10));

        // no range check for new default value
        f1.setTypeFloat(4);
        REQUIRE(f1.getFloat(f1_));
        REQUIRE(f1_ == Approx(4));

        // adjust default value onnew range set
        REQUIRE(f1.setFloatRange(10, 20));
        f1.restoreDefault();
        REQUIRE(f1.getFloat(f1_));
        REQUIRE(f1_ == 10.f);

        f1.setTypeFloat(30);
        REQUIRE(f1.getFloat(f1_));
        REQUIRE(f1_ == Approx(30));

        REQUIRE(f1.setFloatRange(10, 20));
        f1.restoreDefault();
        REQUIRE(f1.getFloat(f1_));
        REQUIRE(f1_ == 20.f);
    }

    SECTION("int")
    {
        DataTypeProperty i1(SYM("i1"));
        i1.setTypeInt(1);

        REQUIRE_FALSE(i1.setFloat(0));
        REQUIRE_FALSE(i1.setBool(0));
        REQUIRE_FALSE(i1.setSymbol(&s_list));
        REQUIRE_FALSE(i1.setList(L()));

        REQUIRE(i1.propertyType() == PropValueType::INTEGER);
        i1.setInt(2000);
        int i1_;
        REQUIRE(i1.getInt(i1_));
        REQUIRE(i1_ == 2000);
        // default now is 1
        i1.setIntRange(-1, 1);
        REQUIRE(i1.getInt(i1_));
        REQUIRE(i1_ == 2000);

        i1.setInt(2000);
        REQUIRE(i1.getInt(i1_));
        REQUIRE(i1_ == 1);
        i1.setInt(0);
        REQUIRE(i1.getInt(i1_));
        REQUIRE(i1_ == 0);
        i1.setInt(-1);
        REQUIRE(i1.getInt(i1_));
        REQUIRE(i1_ == -1);

        i1.restoreDefault();
        REQUIRE(i1.getInt(i1_));
        REQUIRE(i1_ == 1);
    }

    SECTION("bool")
    {
        DataTypeProperty b1(SYM("b1"));
        b1.setTypeBool(true);

        REQUIRE_FALSE(b1.setFloat(0));
        REQUIRE_FALSE(b1.setInt(0));
        REQUIRE_FALSE(b1.setSymbol(&s_list));
        REQUIRE_FALSE(b1.setList(L()));

        REQUIRE(b1.propertyType() == PropValueType::BOOLEAN);
        b1.setBool(true);
        bool b1_;
        REQUIRE(b1.getBool(b1_));
        REQUIRE(b1_);
        b1.setBool(false);
        REQUIRE(b1.getBool(b1_));
        REQUIRE_FALSE(b1_);

        b1.restoreDefault();
        REQUIRE(b1.getBool(b1_));
        REQUIRE(b1_);
    }

    SECTION("symbol")
    {
        DataTypeProperty s1(SYM("s1"));
        s1.setTypeSymbol(SYM("z"));

        REQUIRE_FALSE(s1.setFloat(0));
        REQUIRE_FALSE(s1.setInt(0));
        REQUIRE_FALSE(s1.setBool(0));
        REQUIRE_FALSE(s1.setList(L()));

        REQUIRE(s1.propertyType() == PropValueType::SYMBOL);
        t_symbol* s;
        REQUIRE(s1.getSymbol(&s));
        REQUIRE(s == SYM("z"));

        s1.setSymbol(&s_list);
        REQUIRE(s1.getSymbol(&s));
        REQUIRE(s == &s_list);

        s1.restoreDefault();
        REQUIRE(s1.getSymbol(&s));
        REQUIRE(s == SYM("z"));

        s1.setEnumValues(LA("a", "b", "c", "d"));
        REQUIRE(s1.enumValues() == LA("z", "a", "b", "c", "d"));
        REQUIRE(!s1.setSymbol(SYM("e")));
        REQUIRE(!s1.setSymbol(SYM("f")));

        REQUIRE(s1.setSymbol(SYM("z")));
        REQUIRE(s1.setSymbol(SYM("a")));
        REQUIRE(s1.setSymbol(SYM("b")));
        REQUIRE(s1.setSymbol(SYM("c")));
        REQUIRE(s1.setSymbol(SYM("d")));

        s1.restoreDefault();
        REQUIRE(s1.getSymbol(&s));
        REQUIRE(s == SYM("z"));
    }

    SECTION("list")
    {
        DataTypeProperty l1(SYM("l1"));
        l1.setTypeList(LF(1, 2, 3));

        REQUIRE(l1.propertyType() == PropValueType::LIST);

        REQUIRE_FALSE(l1.setFloat(0));
        REQUIRE_FALSE(l1.setInt(0));
        REQUIRE_FALSE(l1.setBool(0));
        REQUIRE_FALSE(l1.setSymbol(&s_list));

        REQUIRE(l1.setList(LA("a", "b")));
        AtomList l;
        REQUIRE(l1.getList(l));
        REQUIRE(l == LA("a", "b"));

        l1.restoreDefault();
        REQUIRE(l1.getList(l));
        REQUIRE(l == LA(1, 2, 3));
    }

    SECTION("info")
    {
        SECTION("constrains")
        {
            SECTION("min")
            {
                DataTypeProperty p(SYM("c0"));

                REQUIRE(p.setFloat(0));
                REQUIRE(p.info().name() == gensym("c0"));
                REQUIRE(p.info().type() == PropValueType::FLOAT);
                REQUIRE(!p.info().hasConstraintsMin());
                REQUIRE(!p.info().hasConstraintsMax());
                REQUIRE(p.info().defaultFloat() == 0);

                p.setTypeFloat(0.5);
                REQUIRE(p.info().type() == PropValueType::FLOAT);
                REQUIRE(p.info().defaultFloat() == 0.5);

                p.setFloatRange(-1, 1);
                REQUIRE(p.info().hasConstraintsMin());
                REQUIRE(p.info().hasConstraintsMax());

                p.setTypeInt(12);
                REQUIRE(p.info().type() == PropValueType::INTEGER);
                REQUIRE(p.info().defaultInt() == 12);
                REQUIRE(!p.info().hasConstraintsMin());
                REQUIRE(!p.info().hasConstraintsMax());

                p.setIntRange(-100, 100);
                REQUIRE(p.info().hasConstraintsMin());
                REQUIRE(p.info().hasConstraintsMax());
            }
        }
    }
}
