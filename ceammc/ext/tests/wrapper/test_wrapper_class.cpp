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

#include "wrapper_macros.h"
#include "test_wrapper_base.h"
#include "datatype_dict.h"

using namespace wrapper;

TEST_CASE("wrapper_class", "[class-wrapper]")
{
    SECTION("AbstractWrapper")
    {
        using W = AbstractDataWrapper<WrapperDataVoid>;
        W data;
        REQUIRE(data.toListStringContent() == "void");
        REQUIRE(data.toDictStringContent() == "value: void");
        REQUIRE(data.toListString() == "DataVoid(void)");
        REQUIRE(data.toDictString() == "DataVoid[value: void]");
        REQUIRE(data.toString() == "DataVoid(void)");
    }

    SECTION("constructor0")
    {
        using ExternalType = wrapper::ClassConstructorCustom<WrapperDataVoid>;
        using DataType = wrapper::ClassConstructorCustom<WrapperDataVoid>::TypeWrapped;
        using TestType = TestPdExternal<ExternalType>;
        using TestAtom = DataAtom<DataType>;

        SECTION("ctor")
        {
            WRAP_CLASS(WrapperDataVoid, "data0.new");
            REQUIRE_FALSE(WrapperDataVoid().setFromFloat(0).isOk());
            REQUIRE_FALSE(WrapperDataVoid().setFromSymbol(gensym("test")).isOk());
            REQUIRE_FALSE(WrapperDataVoid().setFromList(LF(1, 2, 3, 4)).isOk());

            REQUIRE(WrapperDataVoid().setFromFloat(0).error());
            REQUIRE(WrapperDataVoid().setFromSymbol(gensym("test")).error());
            REQUIRE(WrapperDataVoid().setFromList(LF(1, 2, 3, 4)).error());
        }

        SECTION("empty")
        {
            TestType t("data0.new");
            t << BANG;
            REQUIRE(t.isOutputDataAt(0));
            Atom r = t.outputAtomAt(0);
            REQUIRE(r.isData());
            REQUIRE(r.isA<DataType>());
            REQUIRE(r.asD<DataType>()->toString() == "DataVoid(void)");

            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            const DataType* p = r.asD<DataType>();
            REQUIRE(p);

            t.send(TestAtom());
            REQUIRE(t.isOutputDataAt(0));
        }
    }

    SECTION("constructor1")
    {
        using ExternalType = wrapper::ClassConstructorCustom<WrapperInt>;
        using DataType = wrapper::ClassConstructorCustom<WrapperInt>::TypeWrapped;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_CLASS(WrapperInt, "data1.new");
        }

        SECTION("empty")
        {
            TestType t("data1.new");
            t << BANG;

            REQUIRE(t.isOutputDataAt(0));
            Atom r = t.outputAtomAt(0);
            REQUIRE(r.isData());
            REQUIRE(r.asD<DataType>()->toString() == "DataInt(int: 0)");

            const DataType* p = r.asD<DataType>();
            REQUIRE(p);
            REQUIRE(p->value().get() == 0);
        }

        SECTION("float arg")
        {
            TestType t("data1.new", LF(100));

            t << BANG;
            REQUIRE(t.isOutputDataAt(0));
            Atom r = t.outputAtomAt(0);
            REQUIRE(r.isData());
            REQUIRE(r.asD<DataType>()->toString() == "DataInt(int: 100)");

            const DataType* p = r.asD<DataType>();
            REQUIRE(p);
            REQUIRE(p->value().get() == 100);
        }

        SECTION("symbol arg")
        {
            TestType t("data1.new", LA("A"));
            t << BANG;

            Atom r = t.outputAtomAt(0);
            REQUIRE(r.isData());
            REQUIRE(r.asD<DataType>()->toString() == "DataInt(int: 0)");
        }

        SECTION("list arg")
        {
            TestType t("data1.new", LA(10, 20, 30));
            t << BANG;

            Atom r = t.outputAtomAt(0);
            REQUIRE(r.isData());
            REQUIRE(r.asD<DataType>()->toString() == "DataInt(int: 0)");
        }

        SECTION("on float")
        {
            TestType t("data1.new");
            t << 100;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputDataAt(0));
            Atom r = t.outputAtomAt(0);
            const DataType* p = r.asD<DataType>();
            REQUIRE(p->value().get() == 100);
            REQUIRE(t->data()->value().get() == 100);

            t << BANG;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputDataAt(0));
            r = t.outputAtomAt(0);
            p = r.asD<DataType>();
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
            Atom r = t.outputAtomAt(0);
            const DataType* p = r.asD<DataType>();
            REQUIRE(p->value().get() == -1);

            // no float conversion: list is not supported
            t << LF(1, 2, 3, 4);
            REQUIRE_FALSE(t.hasOutputAt(0));

            // symbol is not supported
            t << LA("abc");
            REQUIRE_FALSE(t.hasOutputAt(0));

            // data is supported
            Atom dptr(new DataType(WrapperInt(300)));
            t.send(dptr);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputDataAt(0));
            r = t.outputAtomAt(0);
            p = r.asD<DataType>();
            REQUIRE(p->value().get() == 300);

            // invalid data id
            t.send(Atom(new AbstractDataWrapper<WrapperDataVoid>()));
            REQUIRE_FALSE(t.hasOutputAt(0));

            // invalid data type: non-wrapper data
            Atom dict(new DataTypeDict());
            ;
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
                Atom r = t.outputAtomAt(0);
                const DataType* p = r.asD<DataType>();

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
                Atom d0(new AbstractDataWrapper<WrapperInt>(300));
                t.sendMessage(gensym("set"), d0);
                REQUIRE_FALSE(t.hasOutputAt(0));

                // data not ok
                t.clearAll();
                Atom d1(new AbstractDataWrapper<WrapperDataVoid>());
                t.sendMessage(gensym("set"), d1);
                REQUIRE_FALSE(t.hasOutputAt(0));

                // data not ok
                t.clearAll();
                Atom d2(new DataTypeDict());
                t.sendMessage(gensym("set"), d2);
                REQUIRE_FALSE(t.hasOutputAt(0));
            }
        }
    }

    SECTION("constructor2")
    {
        using DataType = wrapper::ClassConstructorCustom<WrapperIntPair>::TypeWrapped;
        using TestType = TestPdExternal<WrapperIntPair>;
        using IntPair = std::pair<int, int>;

        SECTION("ctor")
        {
            WRAP_CLASS(WrapperIntPair, "data2.new");
        }

        SECTION("empty")
        {

            TestType t("data2.new");
            t << BANG;

            REQUIRE(t.isOutputDataAt(0));
            Atom r = t.outputAtomAt(0);
            REQUIRE(r.isData());
            REQUIRE(r.asD<DataType>()->toString() == "IntPair(pair: 0 0)");

            const DataType* p = r.asD<DataType>();
            REQUIRE(p);
            REQUIRE(p->value().get() == IntPair(0, 0));
        }

        SECTION("float arg")
        {
            TestType t("data2.new", LF(100));
            t << BANG;

            REQUIRE(t.isOutputDataAt(0));
            Atom r = t.outputAtomAt(0);
            REQUIRE(r.isData());
            REQUIRE(r.asD<DataType>()->toString() == "IntPair(pair: 0 0)");

            const DataType* p = r.asD<DataType>();
            REQUIRE(p);
            REQUIRE(p->value().get() == IntPair(0, 0));
        }

        SECTION("list arg")
        {
            TestType t("data2.new", LF(100, 200));
            t << BANG;

            REQUIRE(t.isOutputDataAt(0));
            Atom r = t.outputAtomAt(0);
            REQUIRE(r.isData());
            REQUIRE(r.asD<DataType>()->toString() == "IntPair(pair: 100 200)");

            const DataType* p = r.asD<DataType>();
            REQUIRE(p);
            REQUIRE(p->value().get() == IntPair(100, 200));
        }

        SECTION("type arg")
        {
            TestType t("data2.new", AtomList::parseString("IntPair(100 200)"));
            t << BANG;

            REQUIRE(t.isOutputDataAt(0));
            Atom r = t.outputAtomAt(0);
            REQUIRE(r.isData());
            REQUIRE(r.asD<DataType>()->toString() == "IntPair(pair: 100 200)");

            const DataType* p = r.asD<DataType>();
            REQUIRE(p);
            REQUIRE(p->value().get() == IntPair(100, 200));
        }

        SECTION("wrong type arg")
        {
            TestType t("data2.new", AtomList::parseString("IntPair(100 XXX)"));
            t << BANG;

            REQUIRE(t.isOutputDataAt(0));
            Atom r = t.outputAtomAt(0);
            REQUIRE(r.isData());
            REQUIRE(r.asD<DataType>()->toString() == "IntPair(pair: 0 0)");
        }
    }
}
