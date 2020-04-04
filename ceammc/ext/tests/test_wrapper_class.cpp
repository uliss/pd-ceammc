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

#include "../class-wrapper/src/wrapper_macros.h"
#include "catch.hpp"
#include "datatype_dict.h"
#include "test_external.h"

#include <iostream>

using namespace wrapper;

PD_TEST_CANVAS();
PD_TEST_CORE_INIT()

class WrapperDataVoid : public DataIFace {
public:
    WrapperDataVoid() {}

    bool operator==(const WrapperDataVoid& d) const
    {
        return true;
    }

    std::string toString() const
    {
        return "void";
    }

    static const char* typeName()
    {
        return "DataVoid";
    }
};

class WrapperDataInt : public DataIFace {
    int v_;

public:
    WrapperDataInt(int v = 0)
        : v_(v)
    {
    }

    bool operator==(const WrapperDataInt& d) const
    {
        return v_ == d.v_;
    }

    std::string toString() const
    {
        return std::string("int: ") + std::to_string(v_);
    }

    Result setFromFloat(t_float v) override
    {
        v_ = static_cast<decltype(v_)>(v);
        return ok();
    }

    static const char* typeName()
    {
        return "DataInt";
    }

    int get() const
    {
        return v_;
    }
};

class WrapperDataPair : public DataIFace {
    std::pair<int, int> v_;

public:
    WrapperDataPair(int v0 = 0, int v1 = 0)
        : v_(v0, v1)
    {
    }

    bool operator==(const WrapperDataPair& d) const
    {
        return v_ == d.v_;
    }

    std::string toString() const
    {
        return std::string("pair: ") + std::to_string(v_.first) + " " + std::to_string(v_.second);
    }

    static const char* typeName()
    {
        return "DataPair";
    }

    std::pair<int, int> get() const
    {
        return v_;
    }

    Result setFromList(const AtomList& l) override
    {
        if (l.size() != 2)
            return error("v0 v1 expected");

        if (!(l[0].isFloat() && l[1].isFloat()))
            return error("float arguments required");

        v_.first = l[0].asFloat();
        v_.second = l[1].asFloat();

        return ok();
    }
};

TEST_CASE("wrapper class", "[class-wrapper]")
{
    test::pdPrintToStdError(true);
    pd_test_core_init();

    SECTION("constructor0")
    {
        WRAP_CLASS(WrapperDataVoid, "data0.new");

        using ExternalType = wrapper::ClassConstructorCustom<WrapperDataVoid>;
        using DataType = wrapper::ClassConstructorCustom<WrapperDataVoid>::TypeWrapped;
        using TestType = TestPdExternal<ExternalType>;

        REQUIRE_FALSE(WrapperDataVoid().setFromFloat(0).isOk());
        REQUIRE_FALSE(WrapperDataVoid().setFromSymbol(gensym("test")).isOk());
        REQUIRE_FALSE(WrapperDataVoid().setFromList(LF(1, 2, 3, 4)).isOk());

        REQUIRE(WrapperDataVoid().setFromFloat(0).error());
        REQUIRE(WrapperDataVoid().setFromSymbol(gensym("test")).error());
        REQUIRE(WrapperDataVoid().setFromList(LF(1, 2, 3, 4)).error());

        SECTION("empty")
        {
            TestType t("data0.new");
            t << BANG;
            REQUIRE(t.isOutputDataAt(0));
            DataPtr r = t.outputAtomAt(0);
            REQUIRE(r.isValid());
            REQUIRE(r->toString() == "void");
            REQUIRE(r->type() == data::DATA_WRAPPER);

            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            const DataType* p = r.as<DataType>();
            REQUIRE(p);
            REQUIRE(p->dataTypeId() == 1001);

            t.send(DataPtr(new DataType()));
            REQUIRE(t.isOutputDataAt(0));
        }
    }

    SECTION("constructor1")
    {
        WRAP_CLASS(WrapperDataInt, "data1.new");

        using ExternalType = wrapper::ClassConstructorCustom<WrapperDataInt>;
        using DataType = wrapper::ClassConstructorCustom<WrapperDataInt>::TypeWrapped;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("empty")
        {
            TestType t("data1.new");
            t << BANG;

            REQUIRE(t.isOutputDataAt(0));
            DataPtr r = t.outputAtomAt(0);
            REQUIRE(r.isValid());
            REQUIRE(r->toString() == "int: 0");
            REQUIRE(r->type() == data::DATA_WRAPPER);

            const DataType* p = r.as<DataType>();
            REQUIRE(p);
            REQUIRE(p->dataTypeId() == 1002);
            REQUIRE(p->value().get() == 0);
        }

        SECTION("float arg")
        {
            TestType t("data1.new", LF(100));
            t << BANG;

            REQUIRE(t.isOutputDataAt(0));
            DataPtr r = t.outputAtomAt(0);
            REQUIRE(r.isValid());
            REQUIRE(r->toString() == "int: 100");
            REQUIRE(r->type() == data::DATA_WRAPPER);

            const DataType* p = r.as<DataType>();
            REQUIRE(p);
            REQUIRE(p->value().get() == 100);
        }

        SECTION("symbol arg")
        {
            TestType t("data1.new", LA("A"));
            t << BANG;

            DataPtr r = t.outputAtomAt(0);
            REQUIRE(r.isValid());
            REQUIRE(r->toString() == "int: 0");
        }

        SECTION("list arg")
        {
            TestType t("data1.new", LA(10, 20, 30));
            t << BANG;

            DataPtr r = t.outputAtomAt(0);
            REQUIRE(r.isValid());
            REQUIRE(r->toString() == "int: 0");
        }

        SECTION("on float")
        {
            TestType t("data1.new");
            t << 100;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputDataAt(0));
            DataPtr r = t.outputAtomAt(0);
            const DataType* p = r.as<DataType>();
            REQUIRE(p->value().get() == 100);
            REQUIRE(t->data()->value().get() == 100);

            t << BANG;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputDataAt(0));
            r = t.outputAtomAt(0);
            p = r.as<DataType>();
            REQUIRE(p->value().get() == 100);
        }

        SECTION("on symbol")
        {
            TestType t("data1.new");
            t << "SYMBOL";

            REQUIRE_FALSE(t.hasOutputAt(0));
            REQUIRE_FALSE(t.isOutputDataAt(0));
        }

        SECTION("on list")
        {
            TestType t("data1.new");

            // converted to float
            t.sendList(LF(-1));

            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputDataAt(0));
            DataPtr r = t.outputAtomAt(0);
            const DataType* p = r.as<DataType>();
            REQUIRE(p->value().get() == -1);

            // no float conversion: list is not supported
            t << LF(1, 2, 3, 4);
            REQUIRE_FALSE(t.hasOutputAt(0));

            // symbol is not supported
            t << LA("abc");
            REQUIRE_FALSE(t.hasOutputAt(0));

            // data is supported
            DataPtr dptr(new DataType(WrapperDataInt(300)));
            t.send(dptr);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputDataAt(0));
            r = t.outputAtomAt(0);
            p = r.as<DataType>();
            REQUIRE(p->value().get() == 300);

            // invalid data id
            t.send(DataPtr(new AbstractDataWrapper<WrapperDataVoid>()));
            REQUIRE_FALSE(t.hasOutputAt(0));

            // invalid data type: non-wrapper data
            DataPtr dict(new DataTypeDict());
            REQUIRE(dict.isValid());
            t.send(dict);
            REQUIRE_FALSE(t.hasOutputAt(0));
        }

        SECTION("set message")
        {
            SECTION("float")
            {
                TestType t("data1.new", LF(100));

                // empty not ok
                t.sendMessage(gensym("set"), AtomList());
                REQUIRE_FALSE(t.hasOutputAt(0));
                REQUIRE(t->data()->value().get() == 100);

                // float ok
                t.sendMessage(gensym("set"), LF(200));
                REQUIRE_FALSE(t.hasOutputAt(0));
                REQUIRE(t->data()->value().get() == 200);

                t << BANG;
                REQUIRE(t.hasOutputAt(0));
                REQUIRE(t.isOutputDataAt(0));
                DataPtr r = t.outputAtomAt(0);
                const DataType* p = r.as<DataType>();

                // symbol not ok
                t.clearAll();
                t.sendMessage(gensym("set"), LA("ABC"));
                REQUIRE_FALSE(t.hasOutputAt(0));
                REQUIRE(t->data()->value().get() == 200);

                // list not ok
                t.clearAll();
                t.sendMessage(gensym("set"), LF(10, 20, 30));
                REQUIRE_FALSE(t.hasOutputAt(0));
                REQUIRE(t->data()->value().get() == 200);

                // data ok
                t.clearAll();
                DataPtr d0(new AbstractDataWrapper<WrapperDataInt>(300));
                t.sendMessage(gensym("set"), d0.asAtom());
                REQUIRE_FALSE(t.hasOutputAt(0));

                // data not ok
                t.clearAll();
                DataPtr d1(new AbstractDataWrapper<WrapperDataVoid>());
                t.sendMessage(gensym("set"), d1.asAtom());
                REQUIRE_FALSE(t.hasOutputAt(0));

                // data not ok
                t.clearAll();
                DataPtr d2(new DataTypeDict());
                t.sendMessage(gensym("set"), d2.asAtom());
                REQUIRE_FALSE(t.hasOutputAt(0));
            }
        }
    }

    SECTION("constructor2")
    {
        WRAP_CLASS(WrapperDataPair, "data2.new");

        using ExternalType = wrapper::ClassConstructorCustom<WrapperDataPair>;
        using DataType = wrapper::ClassConstructorCustom<WrapperDataPair>::TypeWrapped;
        using TestType = TestPdExternal<WrapperDataPair>;
        using IntPair = std::pair<int, int>;

        SECTION("empty")
        {

            TestType t("data2.new");
            t << BANG;

            REQUIRE(t.isOutputDataAt(0));
            DataPtr r = t.outputAtomAt(0);
            REQUIRE(r.isValid());
            REQUIRE(r->toString() == "pair: 0 0");
            REQUIRE(r->type() == data::DATA_WRAPPER);

            const DataType* p = r.as<DataType>();
            REQUIRE(p);
            REQUIRE(p->dataTypeId() == 1003);
            REQUIRE(p->value().get() == IntPair(0, 0));
        }

        SECTION("float arg")
        {
            TestType t("data2.new", LF(100));
            t << BANG;

            REQUIRE(t.isOutputDataAt(0));
            DataPtr r = t.outputAtomAt(0);
            REQUIRE(r.isValid());
            REQUIRE(r->toString() == "pair: 0 0");
            REQUIRE(r->type() == data::DATA_WRAPPER);

            const DataType* p = r.as<DataType>();
            REQUIRE(p);
            REQUIRE(p->dataTypeId() == 1003);
            REQUIRE(p->value().get() == IntPair(0, 0));
        }

        SECTION("list arg")
        {
            TestType t("data2.new", LF(100, 200));
            t << BANG;

            REQUIRE(t.isOutputDataAt(0));
            DataPtr r = t.outputAtomAt(0);
            REQUIRE(r.isValid());
            REQUIRE(r->toString() == "pair: 100 200");
            REQUIRE(r->type() == data::DATA_WRAPPER);

            const DataType* p = r.as<DataType>();
            REQUIRE(p);
            REQUIRE(p->dataTypeId() == 1003);
            REQUIRE(p->value().get() == IntPair(100, 200));
        }

        SECTION("type arg")
        {
            TestType t("data2.new", LA("DataPair", 100, 200));
            t << BANG;

            REQUIRE(t.isOutputDataAt(0));
            DataPtr r = t.outputAtomAt(0);
            REQUIRE(r.isValid());
            REQUIRE(r->toString() == "pair: 100 200");
            REQUIRE(r->type() == data::DATA_WRAPPER);

            const DataType* p = r.as<DataType>();
            REQUIRE(p);
            REQUIRE(p->dataTypeId() == 1003);
            REQUIRE(p->value().get() == IntPair(100, 200));
        }

        SECTION("wrong type arg")
        {
            TestType t("data2.new", LA("DataInt", 200));
            t << BANG;

            REQUIRE(t.isOutputDataAt(0));
            DataPtr r = t.outputAtomAt(0);
            REQUIRE(r.isValid());
            REQUIRE(r->toString() == "pair: 0 0");
            REQUIRE(r->type() == data::DATA_WRAPPER);
        }
    }
}
