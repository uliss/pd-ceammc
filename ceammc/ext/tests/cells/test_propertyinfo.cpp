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
#include "ceammc_property_info.h"

#include "catch.hpp"
#include "datatype_dict.h"
#include "test_base.h"

using namespace ceammc;

TEST_CASE("PropertyInfo", "[core]")
{
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("bool")
        {
            PropertyInfo pi(gensym("@bool"), PropertyInfo::toType<bool>());
            REQUIRE(pi.name() == gensym("@bool"));
            REQUIRE(pi.type() == PropValueType::BOOLEAN);
            REQUIRE(pi.access() == PropValueAccess::READWRITE);
            REQUIRE(pi.view() == PropValueView::TOGGLE);
            REQUIRE(pi.units() == PropValueUnits::NONE);
            REQUIRE(pi.visibility() == PropValueVis::PUBLIC);
            REQUIRE(pi.constraints() == PropValueConstraints::NONE);

            REQUIRE(pi.isPublic());
            REQUIRE(pi.isReadWrite());
            REQUIRE_FALSE(pi.isFloat());
            REQUIRE_FALSE(pi.isNumeric());
            REQUIRE_FALSE(pi.isInt());
            REQUIRE(pi.isBool());
            REQUIRE_FALSE(pi.isSymbol());
            REQUIRE_FALSE(pi.isVariant());
            REQUIRE_FALSE(pi.isList());
            REQUIRE_FALSE(pi.isInitOnly());
            REQUIRE_FALSE(pi.isReadOnly());
            REQUIRE_FALSE(pi.isInternal());
            REQUIRE_FALSE(pi.isHidden());

            REQUIRE_FALSE(pi.hasStep());
            REQUIRE_FALSE(pi.hasEnumLimit());
            REQUIRE_FALSE(pi.hasConstraintsMin());
            REQUIRE_FALSE(pi.hasConstraintsMax());
            REQUIRE(pi.enumValues().empty());

            REQUIRE(pi.noDefault());
            REQUIRE(pi.defaultFloat(-100) == -100);
            REQUIRE(pi.defaultBool(false) == false);
            REQUIRE(pi.defaultBool(true) == true);
            REQUIRE(pi.defaultInt(-100) == -100);
            REQUIRE(pi.defaultAtom().isNone());
            REQUIRE(pi.defaultList().empty());
            REQUIRE(pi.defaultSymbol() == &s_);

            pi.setDefault(1000);
            REQUIRE(pi.defaultBool(false) == false);
            pi.setDefault(4.f);
            REQUIRE(pi.defaultBool(false) == false);
            pi.setDefault(2.0);
            REQUIRE(pi.defaultBool(false) == false);
            pi.setDefault(false);
            REQUIRE(pi.defaultBool(true) == false);
            pi.setDefault(&s_list);
            REQUIRE(pi.defaultBool(true) == false);
            pi.setDefault(Atom(123));
            REQUIRE(pi.defaultBool(true) == false);
            pi.setDefault(LF(1, 2, 3));
            REQUIRE(pi.defaultBool(true) == false);

            // set min
            REQUIRE_FALSE(pi.setMinFloat(10));
            REQUIRE_FALSE(pi.setMaxFloat(20));
            REQUIRE_FALSE(pi.setRangeFloat(0, 1));
            REQUIRE_FALSE(pi.setMinInt(10));
            REQUIRE_FALSE(pi.setMaxInt(20));
            REQUIRE_FALSE(pi.setRangeInt(0, 1));
            REQUIRE_FALSE(pi.setStep(1));

            // set view
            REQUIRE(pi.setView(PropValueView::TOGGLE));
            REQUIRE_FALSE(pi.setView(PropValueView::COLOR));
            REQUIRE_FALSE(pi.setView(PropValueView::ENTRY));
            REQUIRE_FALSE(pi.setView(PropValueView::KNOB));
            REQUIRE_FALSE(pi.setView(PropValueView::MENU));
            REQUIRE_FALSE(pi.setView(PropValueView::NUMBOX));
            REQUIRE_FALSE(pi.setView(PropValueView::SLIDER));
            REQUIRE_FALSE(pi.setView(PropValueView::SPINBOX));

            // enum
            REQUIRE_FALSE(pi.addEnum(10));
            REQUIRE_FALSE(pi.addEnum("true"));
            REQUIRE_FALSE(pi.addEnum(SYM("false")));
            REQUIRE_FALSE(pi.hasEnumLimit());
            REQUIRE(pi.enumValues().empty());
        }

        SECTION("float")
        {
            PropertyInfo pi("@float", PropertyInfo::toType<t_float>());
            REQUIRE(pi.name() == gensym("@float"));
            REQUIRE(pi.type() == PropValueType::FLOAT);
            REQUIRE(pi.access() == PropValueAccess::READWRITE);
            REQUIRE(pi.view() == PropValueView::SLIDER);
            REQUIRE(pi.units() == PropValueUnits::NONE);
            REQUIRE(pi.visibility() == PropValueVis::PUBLIC);

            REQUIRE(pi.isPublic());
            REQUIRE(pi.isReadWrite());
            REQUIRE(pi.isFloat());
            REQUIRE(pi.isNumeric());
            REQUIRE_FALSE(pi.isInt());
            REQUIRE_FALSE(pi.isBool());
            REQUIRE_FALSE(pi.isSymbol());
            REQUIRE_FALSE(pi.isVariant());
            REQUIRE_FALSE(pi.isList());
            REQUIRE_FALSE(pi.isInitOnly());
            REQUIRE_FALSE(pi.isReadOnly());
            REQUIRE_FALSE(pi.isInternal());
            REQUIRE_FALSE(pi.isHidden());

            REQUIRE_FALSE(pi.hasStep());
            REQUIRE_FALSE(pi.hasEnumLimit());
            REQUIRE_FALSE(pi.hasConstraintsMax());
            REQUIRE_FALSE(pi.hasConstraintsMin());
            REQUIRE(pi.enumValues().empty());

            REQUIRE(pi.defaultFloat(-100) == -100);
            REQUIRE(pi.defaultBool(false) == false);
            REQUIRE(pi.defaultBool(true) == true);
            REQUIRE(pi.defaultInt(-100) == -100);
            REQUIRE(pi.defaultAtom().isNone());
            REQUIRE(pi.defaultList().empty());
            REQUIRE(pi.defaultSymbol() == &s_);

            pi.setDefault(1000);
            REQUIRE(pi.defaultFloat(-100) == 1000);
            pi.setDefault(4.f);
            REQUIRE(pi.defaultFloat(-100) == 4);
            pi.setDefault(2.0);
            REQUIRE(pi.defaultFloat(-100) == 2);
            pi.setDefault(true);
            REQUIRE(pi.defaultFloat(-100) == 2);
            pi.setDefault(&s_list);
            REQUIRE(pi.defaultFloat(-100) == 2);
            pi.setDefault(Atom(123));
            REQUIRE(pi.defaultFloat(-100) == 2);
            pi.setDefault(LF(1, 2, 3));
            REQUIRE(pi.defaultFloat(-100) == 2);

            // set min
            // invalid type
            REQUIRE_FALSE(pi.setMinInt(10));
            REQUIRE_FALSE(pi.setMaxInt(10));

            REQUIRE_FALSE(pi.setMinFloat(10));
            REQUIRE_FALSE(pi.setMaxFloat(20));
            REQUIRE(pi.minFloat() != 10);
            REQUIRE(pi.maxFloat() != 20);

            // no constraints
            REQUIRE_FALSE(pi.hasConstraintsMin());
            REQUIRE_FALSE(pi.hasConstraintsMax());

            // set > constraint
            pi.clearRangeFloat();
            REQUIRE(pi.setConstraints(PropValueConstraints::GREATER_THEN));
            REQUIRE(pi.hasConstraintsMin());
            REQUIRE_FALSE(pi.hasConstraintsMax());

            REQUIRE(pi.setMinFloat(10));
            REQUIRE(pi.minFloat() == 10);
            REQUIRE_FALSE(pi.setMaxFloat(20));
            REQUIRE(pi.minFloat() != 20);
            REQUIRE_FALSE(pi.setRangeFloat(10, 20));

            // set < constraint
            pi.clearRangeFloat();
            REQUIRE(pi.setConstraints(PropValueConstraints::LESS_EQUAL));
            REQUIRE_FALSE(pi.hasConstraintsMin());
            REQUIRE(pi.hasConstraintsMax());

            REQUIRE_FALSE(pi.setMinFloat(30));
            REQUIRE(pi.minFloat() != 30);
            REQUIRE(pi.setMaxFloat(40));
            REQUIRE(pi.maxFloat() == 40);
            REQUIRE_FALSE(pi.setRangeFloat(30, 40));

            // set [] constraints
            pi.clearRangeFloat();
            REQUIRE(pi.setConstraints(PropValueConstraints::CLOSED_RANGE));
            REQUIRE(pi.hasConstraintsMin());
            REQUIRE(pi.hasConstraintsMax());

            REQUIRE(pi.setMinFloat(50));
            REQUIRE(pi.minFloat() == 50);
            REQUIRE(pi.setMaxFloat(60));
            REQUIRE(pi.maxFloat() == 60);
            REQUIRE(pi.setRangeFloat(70, 80));
            REQUIRE(pi.minFloat() == 70);
            REQUIRE(pi.maxFloat() == 80);
            REQUIRE(pi.setRangeFloat(80, 10));
            REQUIRE(pi.minFloat() == 10);
            REQUIRE(pi.maxFloat() == 80);

            // set default
            pi.setDefault(9.5);
            REQUIRE(pi.defaultFloat(-100) == 9.5);
            pi.setDefault(10);
            REQUIRE(pi.defaultFloat(-100) == 10);
            pi.setDefault(11);
            REQUIRE(pi.defaultFloat(-100) == 11);
            pi.setDefault(12);
            REQUIRE(pi.defaultFloat(-100) == 12);
            pi.setDefault(12.25);
            REQUIRE(pi.defaultFloat(-100) == 12.25);

            // set view
            REQUIRE(pi.setView(PropValueView::TOGGLE));
            REQUIRE_FALSE(pi.setView(PropValueView::COLOR));
            REQUIRE_FALSE(pi.setView(PropValueView::ENTRY));
            REQUIRE(pi.setView(PropValueView::KNOB));
            REQUIRE_FALSE(pi.setView(PropValueView::MENU));
            REQUIRE(pi.setView(PropValueView::NUMBOX));
            REQUIRE(pi.setView(PropValueView::SLIDER));
            REQUIRE(pi.setView(PropValueView::SPINBOX));

            // step
            REQUIRE(pi.setStep(1));
            REQUIRE(pi.step() == 1);
            REQUIRE(pi.hasStep());
            REQUIRE(pi.setStep(0));
            REQUIRE(pi.step() == 0);
            REQUIRE_FALSE(pi.hasStep());
            REQUIRE_FALSE(pi.setStep(-1));

            // enum
            REQUIRE_FALSE(pi.addEnum(10));
            REQUIRE_FALSE(pi.addEnum("true"));
            REQUIRE_FALSE(pi.addEnum(SYM("false")));
            REQUIRE_FALSE(pi.hasEnumLimit());
            REQUIRE(pi.enumValues().empty());
        }

        SECTION("int")
        {
            PropertyInfo pi("@int", PropertyInfo::toType<int>());
            REQUIRE(pi.name() == gensym("@int"));
            REQUIRE(pi.type() == PropValueType::INTEGER);
            REQUIRE(pi.access() == PropValueAccess::READWRITE);
            REQUIRE(pi.view() == PropValueView::NUMBOX);
            REQUIRE(pi.units() == PropValueUnits::NONE);
            REQUIRE(pi.visibility() == PropValueVis::PUBLIC);

            REQUIRE(pi.isPublic());
            REQUIRE(pi.isReadWrite());
            REQUIRE_FALSE(pi.isFloat());
            REQUIRE(pi.isNumeric());
            REQUIRE(pi.isInt());
            REQUIRE_FALSE(pi.isBool());
            REQUIRE_FALSE(pi.isSymbol());
            REQUIRE_FALSE(pi.isVariant());
            REQUIRE_FALSE(pi.isList());
            REQUIRE_FALSE(pi.isInitOnly());
            REQUIRE_FALSE(pi.isReadOnly());
            REQUIRE_FALSE(pi.isInternal());
            REQUIRE_FALSE(pi.isHidden());

            REQUIRE_FALSE(pi.hasStep());
            REQUIRE_FALSE(pi.hasEnumLimit());
            REQUIRE_FALSE(pi.hasConstraintsMax());
            REQUIRE_FALSE(pi.hasConstraintsMin());
            REQUIRE(pi.enumValues().empty());

            REQUIRE(pi.noDefault());
            REQUIRE(pi.defaultFloat(-100) == -100);
            REQUIRE(pi.defaultBool(false) == false);
            REQUIRE(pi.defaultBool(true) == true);
            REQUIRE(pi.defaultInt(-200) == -200);
            REQUIRE(pi.defaultAtom().isNone());
            REQUIRE(pi.defaultList().empty());
            REQUIRE(pi.defaultSymbol() == &s_);

            pi.setDefault(1000);
            REQUIRE(pi.defaultInt(-100) == 1000);
            pi.setDefault(4.f);
            REQUIRE(pi.defaultInt(-100) == 1000);
            pi.setDefault(2.0);
            REQUIRE(pi.defaultInt(-100) == 1000);
            pi.setDefault(2);
            REQUIRE(pi.defaultInt(-100) == 2);
            pi.setDefault(true);
            REQUIRE(pi.defaultInt(-100) == 2);
            pi.setDefault(&s_list);
            REQUIRE(pi.defaultInt(-100) == 2);
            pi.setDefault(Atom(123));
            REQUIRE(pi.defaultInt(-100) == 2);
            pi.setDefault(LF(1, 2, 3));
            REQUIRE(pi.defaultInt(-100) == 2);

            // no constraints
            REQUIRE_FALSE(pi.hasConstraintsMin());
            REQUIRE_FALSE(pi.hasConstraintsMax());

            // set > constraint
            pi.clearRangeInt();
            REQUIRE(pi.setConstraints(PropValueConstraints::GREATER_THEN));
            REQUIRE(pi.hasConstraintsMin());
            REQUIRE_FALSE(pi.hasConstraintsMax());

            REQUIRE(pi.setMinInt(10));
            REQUIRE(pi.minInt() == 10);
            REQUIRE_FALSE(pi.setMaxInt(20));
            REQUIRE(pi.minInt() != 20);
            REQUIRE_FALSE(pi.setRangeInt(10, 20));

            // set < constraint
            pi.clearRangeInt();
            REQUIRE(pi.setConstraints(PropValueConstraints::LESS_EQUAL));
            REQUIRE_FALSE(pi.hasConstraintsMin());
            REQUIRE(pi.hasConstraintsMax());

            REQUIRE_FALSE(pi.setMinInt(30));
            REQUIRE(pi.minInt() != 30);
            REQUIRE(pi.setMaxInt(40));
            REQUIRE(pi.maxInt() == 40);
            REQUIRE_FALSE(pi.setRangeInt(30, 40));

            // set [] constraints
            pi.clearRangeInt();
            REQUIRE(pi.setConstraints(PropValueConstraints::CLOSED_RANGE));
            REQUIRE(pi.hasConstraintsMin());
            REQUIRE(pi.hasConstraintsMax());

            REQUIRE(pi.setMinInt(50));
            REQUIRE(pi.minInt() == 50);
            REQUIRE(pi.setMaxInt(60));
            REQUIRE(pi.maxInt() == 60);
            REQUIRE(pi.setRangeInt(70, 80));
            REQUIRE(pi.minInt() == 70);
            REQUIRE(pi.maxInt() == 80);
            REQUIRE(pi.setRangeInt(80, 10));
            REQUIRE(pi.minInt() == 10);
            REQUIRE(pi.maxInt() == 80);

            // set default
            pi.setDefault(9);
            REQUIRE(pi.defaultInt(-100) == 9);
            pi.setDefault(10);
            REQUIRE(pi.defaultInt(-100) == 10);
            pi.setDefault(11);
            REQUIRE(pi.defaultInt(-100) == 11);
            pi.setDefault(12);
            REQUIRE(pi.defaultInt(-100) == 12);
            pi.setDefault(13);
            REQUIRE(pi.defaultInt(-100) == 13);

            // set view
            REQUIRE(pi.setView(PropValueView::TOGGLE));
            REQUIRE_FALSE(pi.setView(PropValueView::COLOR));
            REQUIRE_FALSE(pi.setView(PropValueView::ENTRY));
            REQUIRE(pi.setView(PropValueView::KNOB));
            REQUIRE(pi.setView(PropValueView::MENU));
            REQUIRE(pi.setView(PropValueView::NUMBOX));
            REQUIRE(pi.setView(PropValueView::SLIDER));
            REQUIRE(pi.setView(PropValueView::SPINBOX));

            // enum
            REQUIRE(pi.setRangeInt(10, 20));
            REQUIRE(pi.hasConstraintsMax());
            REQUIRE(pi.hasConstraintsMin());

            REQUIRE_FALSE(pi.hasEnumLimit());
            REQUIRE_FALSE(pi.addEnum(10));
            REQUIRE(pi.setConstraints(PropValueConstraints::ENUM));
            REQUIRE_FALSE(pi.hasConstraintsMax());
            REQUIRE_FALSE(pi.hasConstraintsMin());
            REQUIRE(pi.hasEnumLimit());

            REQUIRE(pi.addEnum(10));
            REQUIRE(pi.addEnum(12));
            REQUIRE_FALSE(pi.addEnum("A"));
            REQUIRE_FALSE(pi.addEnum(SYM("B")));
            REQUIRE(pi.enumValues() == LF(10, 12));

            REQUIRE_FALSE(pi.setMinInt(10));
        }

        SECTION("symbol")
        {
            PropertyInfo pi("@symbol", PropertyInfo::toType<t_symbol*>());
            REQUIRE(pi.name() == gensym("@symbol"));
            REQUIRE(pi.type() == PropValueType::SYMBOL);
            REQUIRE(pi.access() == PropValueAccess::READWRITE);
            REQUIRE(pi.view() == PropValueView::ENTRY);
            REQUIRE(pi.units() == PropValueUnits::NONE);
            REQUIRE(pi.visibility() == PropValueVis::PUBLIC);

            REQUIRE(pi.isPublic());
            REQUIRE(pi.isReadWrite());
            REQUIRE_FALSE(pi.isFloat());
            REQUIRE_FALSE(pi.isNumeric());
            REQUIRE_FALSE(pi.isInt());
            REQUIRE_FALSE(pi.isBool());
            REQUIRE(pi.isSymbol());
            REQUIRE_FALSE(pi.isVariant());
            REQUIRE_FALSE(pi.isList());
            REQUIRE_FALSE(pi.isInitOnly());
            REQUIRE_FALSE(pi.isReadOnly());
            REQUIRE_FALSE(pi.isInternal());
            REQUIRE_FALSE(pi.isHidden());

            REQUIRE_FALSE(pi.hasStep());
            REQUIRE_FALSE(pi.hasEnumLimit());
            REQUIRE_FALSE(pi.hasConstraintsMax());
            REQUIRE_FALSE(pi.hasConstraintsMin());
            REQUIRE(pi.enumValues().empty());

            REQUIRE(pi.noDefault());
            REQUIRE(pi.defaultFloat(-100) == -100);
            REQUIRE(pi.defaultBool(false) == false);
            REQUIRE(pi.defaultBool(true) == true);
            REQUIRE(pi.defaultInt(-100) == -100);
            REQUIRE(pi.defaultAtom().isNone());
            REQUIRE(pi.defaultList().empty());
            REQUIRE(pi.defaultSymbol(SYM("test")) == SYM("test"));

            pi.setDefault(1000);
            REQUIRE(pi.defaultSymbol() == &s_);
            pi.setDefault(SYM("a"));
            REQUIRE(pi.defaultSymbol() == SYM("a"));

            // set min/max
            REQUIRE_FALSE(pi.setMinFloat(10));
            REQUIRE_FALSE(pi.setMaxFloat(20));
            REQUIRE_FALSE(pi.setRangeFloat(10, 20));
            REQUIRE_FALSE(pi.hasConstraintsMin());
            REQUIRE_FALSE(pi.hasConstraintsMax());
            REQUIRE_FALSE(pi.setStep(1));

            // set view
            REQUIRE(pi.setView(PropValueView::ENTRY));
            REQUIRE(pi.setView(PropValueView::MENU));
            REQUIRE_FALSE(pi.setView(PropValueView::COLOR));
            REQUIRE_FALSE(pi.setView(PropValueView::KNOB));
            REQUIRE_FALSE(pi.setView(PropValueView::NUMBOX));
            REQUIRE_FALSE(pi.setView(PropValueView::SLIDER));
            REQUIRE_FALSE(pi.setView(PropValueView::SPINBOX));
            REQUIRE_FALSE(pi.setView(PropValueView::TOGGLE));

            // enum
            REQUIRE_FALSE(pi.hasEnumLimit());
            REQUIRE_FALSE(pi.addEnum(10));

            REQUIRE(pi.setConstraints(PropValueConstraints::ENUM));
            REQUIRE(pi.hasEnumLimit());
            REQUIRE(pi.addEnum("A"));
            REQUIRE(pi.addEnum(SYM("B")));
            REQUIRE_FALSE(pi.addEnum(123));
            REQUIRE(pi.enumValues() == LA("A", "B"));
        }
    }

    SECTION("json")
    {
        SECTION("float")
        {
            PropertyInfo pi("@float", PropertyInfo::toType<t_float>());

            DataTypeDict d;
            REQUIRE(pi.getDict(d));
            REQUIRE(*d.toJSON()
                == R"({"access":"readwrite","name":"@float","type":"float","view":"slider","visibility":"public"})");

            pi.setAccess(PropValueAccess::READONLY);
            REQUIRE(pi.getDict(d));
            REQUIRE(*d.toJSON()
                == R"({"access":"readonly","name":"@float","type":"float","view":"slider","visibility":"public"})");

            pi.setDefault(100);
            REQUIRE(pi.getDict(d));
            REQUIRE(*d.toJSON()
                == R"({"access":"readonly","default":100,"name":"@float","type":"float","view":"slider","visibility":"public"})");

            pi.setUnits(PropValueUnits::BPM);
            REQUIRE(pi.getDict(d));
            REQUIRE(*d.toJSON()
                == R"({"access":"readonly","default":100,"name":"@float","type":"float","units":"bpm","view":"slider","visibility":"public"})");

            pi.setConstraints(PropValueConstraints::GREATER_EQUAL);
            REQUIRE(pi.setMinFloat(-10));

            REQUIRE(pi.getDict(d));
            REQUIRE(*d.toJSON()
                == R"({"access":"readonly","constraints":">=","default":100,"min":-10,"name":"@float","type":"float","units":"bpm","view":"slider","visibility":"public"})");
        }
    }
}
