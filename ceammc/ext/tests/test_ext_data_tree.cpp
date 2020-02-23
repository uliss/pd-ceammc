/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "../data/data_tree.h"
#include "ceammc_platform.h"
#include "datatype_tree_imp.h"
#include "test_external.h"

#include <iostream>

#define REQUIRE_SIZE(obj, n) REQUIRE_PROPERTY(t, @size, float(n));

#define REQUIRE_DATA_EQUAL_AT_OUTLET(outlet, obj, data)       \
    {                                                         \
        REQUIRE(obj.hasNewMessages(outlet));                  \
        REQUIRE(obj.lastMessage(outlet).isData());            \
        REQUIRE(obj.lastMessage(outlet).dataValue() == data); \
    }

#define REQUIRE_OUTPUT_TREE(obj, data)                   \
    {                                                    \
        REQUIRE(obj.hasNewMessages(0));                  \
        REQUIRE(obj.lastMessage(0).isData());            \
        REQUIRE(obj.lastMessage(0).dataValue() == data); \
    }

PD_COMPLETE_TEST_SETUP(DataTree, data, tree)

using TreePtr = DataTPtr<DataTypeTree>;

std::ostream& operator<<(std::ostream& os, const TreePtr& p)
{
    os << p->toString();
    return os;
}

TEST_CASE("data.tree", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("escape")
    {
        REQUIRE(DataTypeTreeImpl::treeToJson("") == "");
        REQUIRE(DataTypeTreeImpl::treeToJson("()") == "{}");
        REQUIRE(DataTypeTreeImpl::treeToJson(R"(abc)") == "abc");
        REQUIRE(DataTypeTreeImpl::treeToJson(R"("abc")") == "\"abc\"");
        REQUIRE(DataTypeTreeImpl::treeToJson("\"((())\"()") == "\"((())\"{}");
        REQUIRE(DataTypeTreeImpl::treeToJson("(\"\\\"\")") == R"({"\""})");

#define REQUIRE_ESC_TREE(str) REQUIRE(DataTypeTreeImpl::jsonToTree(DataTypeTreeImpl::treeToJson(str)) == str)
#define REQUIRE_ESC_JSON(str) REQUIRE(DataTypeTreeImpl::treeToJson(DataTypeTreeImpl::jsonToTree(str).c_str()) == str)

        REQUIRE_ESC_TREE("");
        REQUIRE_ESC_TREE("[]");
        REQUIRE_ESC_TREE("1 2 3");
        REQUIRE_ESC_TREE("[1,2,3]");
        REQUIRE_ESC_TREE("[\"a\",\"b\"]");
        REQUIRE_ESC_TREE("()");
        REQUIRE_ESC_TREE("123 123");
        REQUIRE_ESC_TREE("spaces     spaces");
        REQUIRE_ESC_TREE("abc");
        REQUIRE_ESC_TREE("\"{}}}}}\"");

        REQUIRE_ESC_JSON("");
        REQUIRE_ESC_JSON("[]");
        REQUIRE_ESC_JSON("1 2 3");
        REQUIRE_ESC_JSON("[1,2,3]");
        REQUIRE_ESC_JSON("[\"a\",\"b\"]");
        REQUIRE_ESC_JSON("\"()\"");
        REQUIRE_ESC_JSON("123 123");
        REQUIRE_ESC_JSON("spaces     spaces");
        REQUIRE_ESC_JSON("abc");
        REQUIRE_ESC_JSON("\"{}}}}}\"");

#undef REQUIRE_ESC_TREE
#undef REQUIRE_ESC_JSON
    }

    SECTION("DataTreeType")
    {
        SECTION("create")
        {
            DataTypeTree t;
            REQUIRE(t.size() == 0);
            REQUIRE(t.empty());
            REQUIRE(t.isNull());
            REQUIRE_FALSE(t.isArray());
            REQUIRE_FALSE(t.isFloat());
            REQUIRE_FALSE(t.isString());
            REQUIRE_FALSE(t.isObject());
            REQUIRE(t.toString() == "null");

            // copy tmp
            DataTypeTree t2(DataTypeTree(100));
            REQUIRE(t2.size() == 1);
            REQUIRE_FALSE(t2.empty());
            REQUIRE_FALSE(t2.isNull());
            REQUIRE_FALSE(t2.isArray());
            REQUIRE(t2.isFloat());
            REQUIRE_FALSE(t2.isString());
            REQUIRE_FALSE(t2.isObject());
            REQUIRE(t2.toString() == "100.0");

            // copy
            DataTypeTree t3(t2);
            REQUIRE(t3.size() == 1);
            t3.setFloat(2000); // cow detach

            REQUIRE_FALSE(t3.isNull());
            REQUIRE_FALSE(t3.isArray());
            REQUIRE(t3.isFloat());
            REQUIRE_FALSE(t3.isString());
            REQUIRE_FALSE(t3.isObject());
            REQUIRE(t3.toString() == "2000.0");

            // operator
            // self copy
            t2 = t2;
            REQUIRE_FALSE(t2.isNull());
            REQUIRE(t2.toString() == "100.0");
            // copy other
            DataTypeTree t4;
            t4 = t2;
            REQUIRE_FALSE(t4.isNull());
            REQUIRE(t4.toString() == "100.0");
            REQUIRE(t4 == t2);

            // std::swap
            DataTypeTree t6(1);
            DataTypeTree t7(2);
            std::swap(t6, t7);
            REQUIRE(t6.toString() == "2.0");
            REQUIRE(t7.toString() == "1.0");

            DataTypeTree a, b, c, d, e;

            DataTypeTree t8(gensym("test"));
            REQUIRE_FALSE(t8.isNull());
            REQUIRE_FALSE(t8.isArray());
            REQUIRE_FALSE(t8.isFloat());
            REQUIRE(t8.isString());
            REQUIRE_FALSE(t8.isObject());
            REQUIRE(t8.toString() == "\"test\"");

            DataTypeTree t9(FloatList({ 1, 2, 3, 4, 5 }));
            REQUIRE_FALSE(t9.isNull());
            REQUIRE(t9.isArray());
            REQUIRE_FALSE(t9.isFloat());
            REQUIRE_FALSE(t9.isString());
            REQUIRE_FALSE(t9.isObject());
            REQUIRE(t9.toString() == "[1.0,2.0,3.0,4.0,5.0]");
        }

        SECTION("==")
        {
            DataTypeTree t0;
            REQUIRE(t0 == t0);

            DataTypeTree t1(t0);
            REQUIRE(t0 == t1);

            DataTypeTree t2(1000);
            REQUIRE_FALSE(t2 == t1);

            REQUIRE(t0.isEqual(&t0));
            REQUIRE(t0.isEqual(&t1));
            REQUIRE_FALSE(t0.isEqual(nullptr));
            REQUIRE_FALSE(t0.isEqual(&t2));

            DataTypeString str("");
            REQUIRE_FALSE(t0.isEqual(&str));
        }

        SECTION("clone")
        {
            DataTypeTree t0("ABC");
            AbstractData& r0 = t0;
            auto p0 = r0.clone();
            REQUIRE(p0 != nullptr);
            REQUIRE(t0.isEqual(p0));
            REQUIRE(p0->toString() == "\"ABC\"");

            auto p1 = dynamic_cast<DataTypeTree*>(p0);
            REQUIRE(p1);
            p1->setSymbol(gensym("A B C"));
            REQUIRE(p0->toString() == "\"A B C\"");
            REQUIRE_FALSE(t0.isEqual(p0));

            delete p0;
        }

        SECTION("operator=")
        {
            DataTypeTree t0("ABC");
            t0 = DataTypeTree(1200);
            REQUIRE(t0.isFloat());
            REQUIRE(t0.toString() == "1200.0");

            t0 = t0;
            REQUIRE(t0.isFloat());
            REQUIRE(t0.toString() == "1200.0");
        }

        SECTION("clear")
        {
            DataTypeTree t0;
            REQUIRE(t0.empty());
            t0.clear();
            REQUIRE(t0.isNull());

            t0.arrayAdd(100);
            t0.arrayAdd(200);
            REQUIRE(t0.isArray());
            REQUIRE_FALSE(t0.empty());
            REQUIRE(t0.size() == 2);
            REQUIRE(t0.toString() == "[100.0,200.0]");

            t0.clear();
            REQUIRE(t0.empty());
            REQUIRE(t0.isArray());

            t0.setFloat(100);
            REQUIRE(t0.isFloat());
            REQUIRE(t0.asFloat(-1000) == 100);
            t0.clear();
            REQUIRE(t0.isFloat());
            REQUIRE(t0.asFloat(-1000) == 0);

            t0.setSymbol(SYM("ABC"));
            REQUIRE(t0.isString());
            t0.clear();
            REQUIRE(t0.isString());
            REQUIRE(t0.asAtom() == S(""));

            t0 = DataTypeTree::fromString(R"(("a": 1100))");
            REQUIRE(t0.isObject());
            REQUIRE(t0.size() == 1);
            t0.clear();
            REQUIRE(t0.isObject());
            REQUIRE(t0.empty());
        }

        SECTION("add")
        {
            DataTypeTree t0;
            REQUIRE(t0.arrayAdd(1));
            REQUIRE(t0.toString() == R"([1.0])");
            REQUIRE(t0.arrayAdd(gensym("A")));
            REQUIRE(t0.toString() == R"([1.0,"A"])");
            REQUIRE(t0.arrayAdd(LF(2, 3)));
            REQUIRE(t0.toString() == R"([1.0,"A",[2,3]])");
            REQUIRE(t0.addTree(t0));

            REQUIRE(t0.isArray());
            REQUIRE(t0.size() == 4);
            REQUIRE(t0.toString() == R"([1.0,"A",[2,3],[1.0,"A",[2,3]]])");

            DataTypeTree t1(100);
            REQUIRE_FALSE(t1.arrayAdd(1));
            REQUIRE_FALSE(t1.arrayAdd(gensym("A")));
            REQUIRE_FALSE(t1.arrayAdd(LF(1, 2)));
            REQUIRE_FALSE(t1.addTree(t1));

            DataTypeTree t2(gensym("B"));
            REQUIRE_FALSE(t2.arrayAdd(1));
            REQUIRE_FALSE(t2.arrayAdd(gensym("A")));
            REQUIRE_FALSE(t2.arrayAdd(LF(1, 2)));
            REQUIRE_FALSE(t2.addTree(t2));

            DataTypeTree t3(FloatList({}));
            REQUIRE(t3.toString() == "[]");
            REQUIRE(t3.arrayAdd(1));
            REQUIRE(t3.toString() == "[1.0]");
        }

        SECTION("set")
        {
            using TreePtr = DataTPtr<DataTypeTree>;

            DataTypeTree t0(100);
            REQUIRE_FALSE(t0.set(TreePtr(nullptr)));

            auto p0 = t0.clone();
            TreePtr p1(p0);
            REQUIRE(p1.isValid());

            p0->setSymbol(gensym("ABC"));

            REQUIRE(t0.set(p1));
            REQUIRE(t0.toString() == R"("ABC")");
        }

        SECTION("shared ptr test")
        {
            DataTypeTree t0;

            {
                DataTypeTree t1(100);
                t0 = t1;
            }

            REQUIRE(t0.isFloat());
            REQUIRE(t0.toString() == "100.0");
        }

        SECTION("get")
        {
            SECTION("null")
            {
                DataTypeTree t;
                REQUIRE(t.getFloat().isError());
                REQUIRE(t.getFloat().error().what() == "float expected instead of null");
                REQUIRE(t.getSymbol().isError());
                REQUIRE(t.getSymbol().error().what() == "string expected instead of null");
                REQUIRE(t.getList().isError());
                REQUIRE(t.getList().error().what() == "array expected instead of null");
                REQUIRE(t.getObject().isError());
                REQUIRE(t.getObject().error().what() == "object expected instead of null");
            }

            SECTION("float")
            {
                DataTypeTree t(1000);
                REQUIRE_FALSE(t.getFloat().isError());
                REQUIRE(t.getFloat().isOk());
                REQUIRE(t.getFloat().value() == Approx(1000));
                REQUIRE(t.getSymbol().isError());
                REQUIRE(t.getSymbol().error().what() == "string expected instead of number");
                REQUIRE(t.getList().isError());
                REQUIRE(t.getList().error().what() == "array expected instead of number");
                REQUIRE(t.getObject().isError());
                REQUIRE(t.getObject().error().what() == "object expected instead of number");
            }

            SECTION("symbol")
            {
                DataTypeTree t("\"A B C\"");
                REQUIRE(t.getFloat().isError());
                REQUIRE(t.getFloat().error().what() == "float expected instead of string");
                REQUIRE_FALSE(t.getSymbol().isError());
                REQUIRE(t.getSymbol().isOk());
                REQUIRE(t.getSymbol().value() == gensym("\"A B C\""));
                REQUIRE(t.getList().isError());
                REQUIRE(t.getList().error().what() == "array expected instead of string");
                REQUIRE(t.getObject().isError());
                REQUIRE(t.getObject().error().what() == "object expected instead of string");
            }

            SECTION("symbol 2")
            {
                DataTypeTree t(gensym("ABC"));
                REQUIRE(t.getFloat().isError());
                REQUIRE(t.getFloat().error().what() == "float expected instead of string");
                REQUIRE_FALSE(t.getSymbol().isError());
                REQUIRE(t.getSymbol().isOk());
                REQUIRE(t.getSymbol().value() == gensym("ABC"));
                REQUIRE(t.getList().isError());
                REQUIRE(t.getList().error().what() == "array expected instead of string");
                REQUIRE(t.getObject().isError());
                REQUIRE(t.getObject().error().what() == "object expected instead of string");
            }

            SECTION("list")
            {
                DataTypeTree t(FloatList({ 1, 2, 3 }));
                REQUIRE(t.getFloat().isError());
                REQUIRE(t.getFloat().error().what() == "float expected instead of array");
                REQUIRE(t.getSymbol().isError());
                REQUIRE(t.getSymbol().isError());
                REQUIRE(t.getSymbol().error().what() == "string expected instead of array");
                REQUIRE_FALSE(t.getList().isError());
                REQUIRE(t.getList().value() == DataTypeTree::fromString(R"([1,2,3])"));
                REQUIRE(t.getObject().isError());
                REQUIRE(t.getObject().error().what() == "object expected instead of array");
            }

            SECTION("list 2")
            {
                DataTypeTree t(LA("A", "B", 2));
                REQUIRE(t.getFloat().isError());
                REQUIRE(t.getFloat().error().what() == "float expected instead of array");
                REQUIRE(t.getSymbol().isError());
                REQUIRE(t.getSymbol().isError());
                REQUIRE(t.getSymbol().error().what() == "string expected instead of array");
                REQUIRE_FALSE(t.getList().isError());
                REQUIRE(t.getList().value() == DataTypeTree::fromString(R"(["A", "B", 2])"));
                REQUIRE(t.getObject().isError());
                REQUIRE(t.getObject().error().what() == "object expected instead of array");
            }

            SECTION("object")
            {
                DataTypeTree t = DataTypeTree::fromString(R"(("f": 100, "s": "ABC"))");
                REQUIRE(t.getFloat().isError());
                REQUIRE(t.getFloat().error().what() == "float expected instead of object");
                REQUIRE(t.getSymbol().isError());
                REQUIRE(t.getSymbol().isError());
                REQUIRE(t.getSymbol().error().what() == "string expected instead of object");
                REQUIRE(t.getList().isError());
                REQUIRE(t.getList().error().what() == "array expected instead of object");
                REQUIRE_FALSE(t.getObject().isError());
                REQUIRE(t.getObject().isOk());
                REQUIRE(t.getObject().value().size() == 2);
                REQUIRE(t.getObject().value().at(SYM("f")).isFloat());
                REQUIRE(t.getObject().value().at(SYM("f")).getFloat().isOk());
                REQUIRE(t.getObject().value().at(SYM("f")).getFloat().value() == Approx(100));
                REQUIRE(t.getObject().value().at(SYM("s")).isString());
                REQUIRE(t.getObject().value().at(SYM("s")).getSymbol().value() == SYM("ABC"));
            }
        }

        SECTION("keys")
        {
            SECTION("null")
            {
                DataTypeTree t;
                REQUIRE(t.keys().empty());
            }

            SECTION("float")
            {
                DataTypeTree t(100);
                REQUIRE(t.keys().empty());
            }

            SECTION("symbol")
            {
                DataTypeTree t("test");
                REQUIRE(t.keys().empty());
            }

            SECTION("list")
            {
                DataTypeTree t(LF(1, 2, 3));
                REQUIRE(t.keys().empty());
            }

            SECTION("object")
            {
                DataTypeTree t = DataTypeTree::fromString(R"(("a":1,"b":2,"c":[1,2,3]))");
                REQUIRE(t.keys() == LA("a", "b", "c"));
            }
        }

        SECTION("at")
        {
            SECTION("null")
            {
                DataTypeTree t;
                REQUIRE(t.at(0.f).isNull());
                REQUIRE(t.at(SYM("a")).isNull());
            }

            SECTION("float")
            {
                DataTypeTree t(1000);
                REQUIRE(t.at(0.f).isNull());
                REQUIRE(t.at(SYM("a")).isNull());
            }

            SECTION("string")
            {
                DataTypeTree t("ABC");
                REQUIRE(t.at(0.f).isNull());
                REQUIRE(t.at(SYM("a")).isNull());
            }

            SECTION("list")
            {
                DataTypeTree t(LF(1, 2, 3));
                REQUIRE(t.at(0.f).isFloat());
                REQUIRE(t.at(1).isFloat());
                REQUIRE(t.at(2).isFloat());
                REQUIRE(t.at(4).isNull());
                REQUIRE(t.at(SYM("a")).isNull());
            }

            SECTION("object")
            {
                DataTypeTree t = DataTypeTree::fromString(R"(("a":1,
                                                          "b":"ABC",
                                                          "c":[1,2,3],
                                                          "d":("a'":100,"b'":200),
                                                          "e": true))");
                REQUIRE(t.at(0.f).isNull());
                REQUIRE(t.at(1).isNull());
                REQUIRE(t.at(SYM("a")).isFloat());
                REQUIRE(t.at(SYM("a")).getFloat().isOk());
                REQUIRE(t.at(SYM("a")).getFloat().value() == 1);
                REQUIRE(t.at(SYM("b")).isString());
                REQUIRE(t.at(SYM("b")).getSymbol().isOk());
                REQUIRE(t.at(SYM("b")).getSymbol().value() == gensym("ABC"));
                REQUIRE(t.at(SYM("c")).isArray());
                REQUIRE(t.at(SYM("c")).getList().isOk());
                auto cv = t.at(SYM("c")).getList().value();
                REQUIRE(cv.size() == 3);
                REQUIRE(cv.at(0.f).getFloat().value() == 1);
                REQUIRE(cv.at(1).getFloat().value() == 2);
                REQUIRE(cv.at(2).getFloat().value() == 3);
                REQUIRE(t.at(SYM("d")).isObject());
                REQUIRE(t.at(SYM("e")).isFloat());
                REQUIRE(t.at(SYM("e")).getFloat().value() == 1);
            }
        }
    }

    SECTION("[object] create")
    {
        SECTION("default")
        {
            TestDataTree t("data.tree");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @size, 0);
            REQUIRE_PROPERTY_FLOAT(t, @empty, 1);

            t.onBang();
            REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataTypeTree::newEmpty());
        }

        SECTION("float")
        {
            TestDataTree t("data.tree", LF(-100.5));
            REQUIRE_PROPERTY_FLOAT(t, @size, 1);
            REQUIRE_PROPERTY_FLOAT(t, @empty, 0);

            t.onBang();
            REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, TreePtr(new DataTypeTree(-100.5)));
        }

        SECTION("symbol")
        {
            TestDataTree t("data.tree", LA("\"ABC\""));
            REQUIRE_PROPERTY_FLOAT(t, @size, 1);
            REQUIRE_PROPERTY_FLOAT(t, @empty, 0);

            t.onBang();
            REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, TreePtr(new DataTypeTree(std::string("ABC"))));
            REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, TreePtr(new DataTypeTree("ABC")));
        }

        SECTION("list")
        {
            TestDataTree t("data.tree", LA(R"([9,8,7,6,5,4,3,2,1])"));
            REQUIRE_PROPERTY_FLOAT(t, @size, 9);
            REQUIRE_PROPERTY_FLOAT(t, @empty, 0);

            t.onBang();
            REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, TreePtr(new DataTypeTree(LF(9, 8, 7, 6, 5, 4, 3, 2, 1))));
        }

        SECTION("object")
        {
            TestDataTree t("data.tree", LA(R"(("a": 1, "b": 2, "c": [1,2], "d": ("a'": 100)))"));
            REQUIRE_PROPERTY_FLOAT(t, @size, 4);
            REQUIRE_PROPERTY_FLOAT(t, @empty, 0);

            t.onBang();
            REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, TreePtr(DataTypeTree::newFromString(R"(("a": 1, "b": 2, "c": [1,2], "d": ("a'": 100)))")));
        }

        SECTION("clear")
        {
            TestDataTree t("data.tree", LA(R"([1,2,3])"));
            REQUIRE_PROPERTY_FLOAT(t, @size, 3);
            REQUIRE_PROPERTY_FLOAT(t, @empty, 0);

            t.onBang();
            REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, TreePtr(DataTypeTree(FloatList({ 1, 2, 3 }))));

            WHEN_CALL(t, clear);
            REQUIRE_NO_MSG(t);
            REQUIRE_PROPERTY_FLOAT(t, @size, 0);
            REQUIRE_PROPERTY_FLOAT(t, @empty, 1);
        }

        SECTION("at")
        {
            SECTION("object")
            {
                TestDataTree t("data.tree", LA(R"(("a": 1, "b": "ABC", "c": [1,2], "d": ("a'": 100), "e": true))"));

                WHEN_CALL_N(t, at, "a");
                REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, TreePtr(DataTypeTree(1)));

                WHEN_CALL_N(t, at, "b");
                REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, TreePtr(DataTypeTree("ABC")));

                WHEN_CALL_N(t, at, "c");
                REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, TreePtr(DataTypeTree::fromString("[1, 2]")));

                WHEN_CALL_N(t, at, "d");
                REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, TreePtr(DataTypeTree::fromString(R"(("a'":100))")));
            }
        }
    }
}
