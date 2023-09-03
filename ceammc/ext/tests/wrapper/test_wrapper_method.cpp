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

#include "test_wrapper_base.h"
#include "wrapper_macros.h"

TEST_CASE("wrapper_method", "[class-wrapper]")
{
    SECTION("is_negative")
    {
        using mtype = std::tuple<bool (WrapperInt::*)() const>;
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;
        using DataType = ExternalType::DataType;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.is_neg", method_id,
                SINGLE_ARG(bool (WrapperInt::*)() const),
                SINGLE_ARG(&WrapperInt::is_negative));
        }

        SECTION("construct")
        {
            SECTION("empty args")
            {
                TestType t("int.is_neg");
                REQUIRE(t.numInlets() == 1);
                REQUIRE(t.numOutlets() == 1);

                t << BANG;
                REQUIRE_FALSE(t.hasOutput());

                t << -100;
                REQUIRE(t.hasOutputAt(0));
                REQUIRE(t.isOutputFloatAt(0));
                REQUIRE(t.outputFloatAt(0) == 1);

                t.send(Atom(new DataType(WrapperInt(4))));
                REQUIRE(t.hasOutputAt(0));
                REQUIRE(t.isOutputFloatAt(0));
                REQUIRE(t.outputFloatAt(0) == 0);

                // invalid data
                t.send(Atom(new DataTypeDict()));
                REQUIRE_FALSE(t.hasOutputAt(0));

                // invalid data wrapper with different id
                t.send(Atom(new AbstractDataWrapper<WrapperInt2>(WrapperInt2())));
                REQUIRE_FALSE(t.hasOutputAt(0));

                // symbol not ok
                t << "ABC";
                REQUIRE_FALSE(t.hasOutputAt(0));

                // list not ok
                t << LF(1, 2, 3);
                REQUIRE_FALSE(t.hasOutputAt(0));
            }
        }

        SECTION("args")
        {
            TestType t("int.is_neg", LF(100));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }
    }

    SECTION("dup2")
    {
        using mtype = std::tuple<std::tuple<int, int> (WrapperInt::*)() const>;
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.dup2", method_id,
                SINGLE_ARG(std::tuple<int, int>(WrapperInt::*)() const),
                SINGLE_ARG(&WrapperInt::dup2));
        }

        SECTION("empty args")
        {
            TestType t("int.dup2");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);

            t << 100;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.hasOutputAt(1));
            REQUIRE(t.outputFloatAt(0) == 100);
            REQUIRE(t.outputFloatAt(1) == 100);

            t << 200;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.hasOutputAt(1));
            REQUIRE(t.outputFloatAt(0) == 200);
            REQUIRE(t.outputFloatAt(1) == 200);
        }
    }

    SECTION("id")
    {
        using mtype = std::tuple<WrapperInt (WrapperInt::*)() const>;
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;
        using DataType = ExternalType::DataType;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.id", method_id,
                SINGLE_ARG(WrapperInt(WrapperInt::*)() const),
                SINGLE_ARG(&WrapperInt::id));
        }

        SECTION("empty args")
        {
            TestType t("int.id");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            t << 100;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputDataAt(0));
            REQUIRE(t.outputAtomAt(0).asD<DataType>()->value().get() == 100);
        }
    }

    SECTION("dup")
    {
        using mtype = std::tuple<std::vector<float> (WrapperInt::*)(int) const>;
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;
        using DataType = ExternalType::DataType;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.dup", method_id,
                SINGLE_ARG(std::vector<float>(WrapperInt::*)(int) const),
                SINGLE_ARG(&WrapperInt::dup));
        }

        SECTION("empty args")
        {
            TestType t("int.dup");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t << 100;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == AtomList());
        }

        SECTION("float args")
        {
            TestType t("int.dup", LF(3));

            t.send(Atom(new DataType(WrapperInt(100))));
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LF(100, 100, 100));

            t << -10;
            REQUIRE(t.outputListAt(0) == LF(-10, -10, -10));

            t.clearAll();
            pd::External f("f");
            REQUIRE(f.connectTo(0, t, 1));
            f.sendFloat(2);
            REQUIRE(!t.hasOutput());

            t << 2;
            REQUIRE(t.outputListAt(0) == LF(2, 2));

            pd::External sym("symbol");
            REQUIRE(sym.connectTo(0, t, 1));
            sym.sendSymbol(gensym("ABC"));
        }
    }

    SECTION("multiply")
    {
        using mtype = std::tuple<int (WrapperInt::*)(const WrapperInt&) const,
            int (WrapperInt::*)(int) const>;
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;
        using DataType = ExternalType::DataType;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.multiply", method_id,
                SINGLE_ARG(int (WrapperInt::*)(const WrapperInt&) const, int (WrapperInt::*)(int) const),
                SINGLE_ARG(&WrapperInt::multiply, &WrapperInt::multiply));

            WRAP_METHOD_ALIAS(method_id, "int.*");
        }

        SECTION("empty args")
        {
            TestType t("int.*");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t << 100;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 0);

            t.send(Atom(new DataType(WrapperInt(20))));
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 0);
        }

        SECTION("float args")
        {
            TestType t("int.*", LF(10));

            t << 2;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 20);

            t.send(Atom(new DataType(WrapperInt(30))));
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 300);

            // data type send via [trigger]
            pd::External tr("t", LA("a"));
            REQUIRE(tr.connectTo(0, t, 1));
            Atom d0(new DataType(WrapperInt(14)));
            tr.sendList(d0);

            t << 10;
            REQUIRE(t.outputFloatAt(0) == 140);

            // invalid datatype
            Atom d1(new DataTypeDict());
            tr.sendList(d1);
            t << 10;
            REQUIRE(t.outputFloatAt(0) == 140);

            Atom d2(new AbstractDataWrapper<WrapperInt2>(WrapperInt2()));
            tr.sendList(d2);
            t << 10;
            REQUIRE(t.outputFloatAt(0) == 140);

            pd::External f("f");
            REQUIRE(f.connectTo(0, t, 1));
            f.sendFloat(3);

            t << 2;
            REQUIRE(t.outputFloatAt(0) == 6);
        }

        SECTION("typed args")
        {
            TestType t("int.*", AtomList::parseString("DataInt(30)"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t << 100;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 3000);

            t.sendListTo(parseDataString("DataInt(20)").result(), 1);

            t << 100;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 2000);
        }
    }

    SECTION("muladd")
    {
        auto mptr = &WrapperInt::muladd;
        using mtype = decltype(mptr);
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.muladd", mtype,
                SINGLE_ARG(mtype),
                SINGLE_ARG(&WrapperInt::muladd));
        }

        SECTION("empty args")
        {
            TestType t("int.muladd");
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 0);
        }

        SECTION("single arg")
        {
            TestType t("int.muladd", LF(2));

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 40);
        }

        SECTION("two args")
        {
            TestType t("int.muladd", LF(2, 3));

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 43);

            t.call("dump");
        }

        SECTION("three args")
        {
            TestType t("int.muladd", LF(2, 3, 4));

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 43);
        }
    }

    SECTION("add_float")
    {
        auto mptr = &WrapperInt::add_float;
        using mtype = decltype(mptr);
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.add_float", method_id,
                SINGLE_ARG(mtype),
                SINGLE_ARG(&WrapperInt::add_float));
        }

        SECTION("empty args")
        {
            TestType t("int.add_float");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 20);
        }

        SECTION("single arg")
        {
            TestType t("int.add_float", LF(2.1));

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == Approx(22.1f));
        }
    }

    SECTION("add_double")
    {
        auto mptr = &WrapperInt::add_double;
        using mtype = decltype(mptr);
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.add_double", method_id,
                SINGLE_ARG(mtype),
                SINGLE_ARG(&WrapperInt::add_double));
        }

        SECTION("empty args")
        {
            TestType t("int.add_double");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 20);
        }

        SECTION("single arg")
        {
            TestType t("int.add_double", LF(1.5));

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == Approx(21.5));
        }
    }

    SECTION("add_long")
    {
        auto mptr = &WrapperInt::add_long;
        using mtype = decltype(mptr);
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.add_long", method_id,
                SINGLE_ARG(mtype),
                SINGLE_ARG(&WrapperInt::add_long));
        }

        SECTION("empty args")
        {
            TestType t("int.add_long");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 20);
        }

        SECTION("single arg")
        {
            TestType t("int.add_long", LF(1.1));

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == Approx(21));
        }
    }

    SECTION("add_uint")
    {
        auto mptr = &WrapperInt::add_uint;
        using mtype = decltype(mptr);
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.add_uint", method_id,
                SINGLE_ARG(mtype),
                SINGLE_ARG(&WrapperInt::add_uint));
        }

        SECTION("empty args")
        {
            TestType t("int.add_uint");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 20);
        }

        SECTION("single arg")
        {
            TestType t("int.add_uint", LF(1.1));

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == Approx(21));
        }
    }

    SECTION("add_ulong")
    {
        auto mptr = &WrapperInt::add_ulong;
        using mtype = decltype(mptr);
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.add_ulong", method_id,
                SINGLE_ARG(mtype),
                SINGLE_ARG(&WrapperInt::add_ulong));
        }

        SECTION("empty args")
        {
            TestType t("int.add_ulong");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 20);
        }

        SECTION("single arg")
        {
            TestType t("int.add_ulong", LF(1.1));

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == Approx(21));
        }
    }

    SECTION("sub")
    {
        using mtype = std::tuple<
            int (WrapperInt::*)(int) const,
            int (WrapperInt::*)(float) const,
            int (WrapperInt::*)(double) const,
            int (WrapperInt::*)(unsigned int) const,
            int (WrapperInt::*)(unsigned long) const>;

        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.sub", mtype,
                SINGLE_ARG(int (WrapperInt::*)(int) const,
                    int (WrapperInt::*)(float) const,
                    int (WrapperInt::*)(double) const,
                    int (WrapperInt::*)(unsigned int) const,
                    int (WrapperInt::*)(unsigned long) const),
                SINGLE_ARG(&WrapperInt::sub,
                    &WrapperInt::sub,
                    &WrapperInt::sub,
                    &WrapperInt::sub,
                    &WrapperInt::sub));
        }

        SECTION("empty args")
        {
            TestType t("int.sub");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 20);
        }

        SECTION("float args")
        {
            TestType t("int.sub", LF(5));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 15);
        }
    }

    SECTION("on_bool")
    {
        auto mptr = &WrapperInt::on_bool;
        using mtype = decltype(mptr);
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.on_bool", method_id,
                SINGLE_ARG(mtype),
                SINGLE_ARG(&WrapperInt::on_bool));
        }

        SECTION("empty args")
        {
            TestType t("int.on_bool");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendFloat(20);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 0);
        }

        SECTION("float args")
        {
            TestType t1("int.on_bool", LF(1));
            t1.sendFloat(1);
            REQUIRE(t1.outputFloatAt(0) == 1);

            TestType t2("int.on_bool", LF(0));
            t2.sendFloat(1);
            REQUIRE(t2.outputFloatAt(0) == 0);

            TestType t3("int.on_bool", LF(-100));
            t3.sendFloat(1);
            REQUIRE(t3.outputFloatAt(0) == 0);

            TestType t4("int.on_bool", LF(0.1));
            t4.sendFloat(1);
            REQUIRE(t4.outputFloatAt(0) == 0);
        }

        SECTION("symbol args")
        {
            TestType t1("int.on_bool", LA("true"));
            t1.sendFloat(1);
            REQUIRE(t1.outputFloatAt(0) == 1);

            TestType t2("int.on_bool", LA("false"));
            t2.sendFloat(1);
            REQUIRE(t2.outputFloatAt(0) == 0);

            TestType t3("int.on_bool", LA("???"));
            t3.sendFloat(1);
            REQUIRE(t3.outputFloatAt(0) == 0);
        }
    }

    SECTION("list_mul")
    {
        auto mptr = &WrapperInt::list_mul;
        using mtype = decltype(mptr);
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.list_mul", method_id,
                SINGLE_ARG(mtype),
                SINGLE_ARG(&WrapperInt::list_mul));
        }

        SECTION("empty args")
        {
            TestType t("int.list_mul");
            t.sendFloat(1);

            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == AtomList());
        }

        SECTION("list args")
        {
            TestType t("int.list_mul", LF(1.5, 2, 3, 4));
            t.sendFloat(2);

            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LF(2, 4, 6, 8));

            t.sendFloat(2);
            REQUIRE(t.outputListAt(0) == LF(2, 4, 6, 8));

            pd::External l("list", LF(10, 20));
            REQUIRE(l.connectTo(0, t, 1));
            l.sendBang();

            t.sendFloat(3);
            REQUIRE(t.outputListAt(0) == LF(30, 60));

            t.call("dump");
        }
    }

    SECTION("list_mulf")
    {
        auto mptr = &WrapperInt::list_mulf;
        using mtype = decltype(mptr);
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.list_mulf", method_id,
                SINGLE_ARG(mtype),
                SINGLE_ARG(&WrapperInt::list_mulf));
        }

        SECTION("empty args")
        {
            TestType t("int.list_mulf");
            t.sendFloat(1);

            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == AtomList());
        }

        SECTION("list args")
        {
            TestType t("int.list_mulf", LF(1.5, 2.5, 3.5, 4.5));
            t.sendFloat(2);

            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LF(3, 5, 7, 9));

            pd::External l("list", LF(2.5, 5));
            REQUIRE(l.connectTo(0, t, 1));
            l.sendBang();

            t.sendFloat(2);
            REQUIRE(t.outputListAt(0) == LF(5, 10));

            t.call("dump");
        }
    }

    SECTION("pair")
    {
        auto mptr = &WrapperIntPair::add;
        using mtype = decltype(mptr);
        using ExternalType = wrapper::ClassMethod<WrapperIntPair, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperIntPair, "pair.+", method_id,
                SINGLE_ARG(mtype),
                SINGLE_ARG(&WrapperIntPair::add));
        }

        SECTION("empty args")
        {
            TestType t("pair.+");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            t.sendList(LF(1, 2));
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.outputListAt(0) == LF(1, 2));
        }

        SECTION("float args")
        {
            TestType t("pair.+", LF(10));

            t.sendList(LF(1, 2));
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.outputListAt(0) == LF(1, 2));
        }

        SECTION("pair args")
        {
            TestType t("pair.+", LF(10, 20));

            t.sendList(LF(1, 2));
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.outputListAt(0) == LF(11, 22));
        }

        SECTION("more args")
        {
            TestType t("pair.+", LF(10, 20, 30));

            t.sendList(LF(1, 2));
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.outputListAt(0) == LF(1, 2));
        }
    }

    SECTION("pair")
    {
        auto mptr = &WrapperInt::addP;
        using mtype = decltype(mptr);
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.addP", method_id,
                SINGLE_ARG(mtype),
                SINGLE_ARG(&WrapperInt::addP));
        }

        SECTION("empty args")
        {
            TestType t("int.addP");
            t.sendFloat(12);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.outputFloatAt(0) == 12);
        }

        SECTION("single args")
        {
            TestType t("int.addP", LF(10));
            t.sendFloat(12);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.outputFloatAt(0) == 12);
        }

        SECTION("two args")
        {
            TestType t("int.addP", LF(10, 20));
            t.sendFloat(12);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.outputFloatAt(0) == 42);

            pd::External l("list");
            REQUIRE(l.connectTo(0, t, 1));
            l.sendList(LF(1, 2));

            t.sendFloat(10);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.outputFloatAt(0) == 13);

            l.sendList(LF(10, 40));
            t.sendFloat(10);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.outputFloatAt(0) == 60);
        }
    }

    SECTION("strtoi")
    {
        auto mptr = &WrapperInt::strtoi;
        using mtype = decltype(mptr);
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.strtoi", method_id,
                SINGLE_ARG(mtype),
                SINGLE_ARG(&WrapperInt::strtoi));
        }

        SECTION("empty")
        {
            TestType t("int.strtoi");
            t.sendFloat(22);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 0);
        }

        SECTION("float arg")
        {
            TestType t("int.strtoi", LF(123.1));
            t.sendFloat(22);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 123);
        }

        SECTION("symbol arg")
        {
            TestType t("int.strtoi", LA("1024"));
            t.sendFloat(22);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 1024);
        }

        SECTION("list arg")
        {
            TestType t("int.strtoi", LA("1024", "A"));
            t.sendFloat(22);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 1024);
        }
    }

    SECTION("strtoi")
    {
        auto mptr = &WrapperInt::str;
        using mtype = decltype(mptr);
        using ExternalType = wrapper::ClassMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("ctor")
        {
            WRAP_METHOD(WrapperInt, "int.str", method_id,
                SINGLE_ARG(mtype),
                SINGLE_ARG(&WrapperInt::str));
        }

        SECTION("empty")
        {
            TestType t("int.str");
            t.sendFloat(22);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputSymbolAt(0));
            REQUIRE(t.outputSymbolAt(0)->s_name == std::string("22"));
        }
    }
}
