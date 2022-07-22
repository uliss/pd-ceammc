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
#include "ceammc_data.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "datatype_string.h"
#include "test_common.h"

#include <boost/optional/optional_io.hpp>

using namespace ceammc;
using Dict = DataTypeDict;

TEST_CASE("DataTypeDict", "[core]")
{
    SECTION("init")
    {
        SECTION("empty")
        {
            DataTypeDict d;
            REQUIRE(d.size() == 0);
            REQUIRE(d.innerData().empty());
            REQUIRE(d.keys().empty());
            REQUIRE(d.flattenToList().empty());
            REQUIRE(d.toString() == "[]");
            REQUIRE(d.toListStringContent() == "");
            REQUIRE(d.toListString() == "Dict()");
        }

        SECTION("[]")
        {
            DataTypeDict d("[]");
            REQUIRE(d.size() == 0);
            REQUIRE(d.innerData().empty());
            REQUIRE(d.keys().empty());
            REQUIRE(d.flattenToList().empty());
            REQUIRE(d.toString() == "[]");
        }

        SECTION("[a: b]")
        {
            DataTypeDict d("[a: b]");
            REQUIRE(d.size() == 1);
            REQUIRE(d.innerData().size() == 1);
            REQUIRE(d.keys() == LA("a"));
            REQUIRE(d.flattenToList() == LA("a", "b"));
            REQUIRE(d.toString() == "[a: b]");
        }

        SECTION("[a: b c]")
        {
            DataTypeDict d("[a: b c]");
            REQUIRE(d.size() == 1);
            REQUIRE(d.innerData().size() == 1);
            REQUIRE(d.keys() == LA("a"));
            REQUIRE(d.flattenToList() == LA("a", "b", "c"));
            REQUIRE(d.toString() == "[a: b c]");
        }

        SECTION("[a: [b: c]]")
        {
            DataTypeDict d("[a: [b: c]]");
            REQUIRE(d.size() == 1);
            REQUIRE(d.innerData().size() == 1);
            REQUIRE(d.keys() == LA("a"));
            REQUIRE(d.flattenToList() == LA("a", DictAtom("[b: c]")));
            REQUIRE(d.toString() == "[a: [b: c]]");
        }
    }

    SECTION("kv")
    {
        using TD = DataTypeDict;
        using DE = TD::DictEntry;
        using DA = DictAtom;

        REQUIRE(TD({ DE("a") }) == TD("[a: ]"));
        REQUIRE(TD({ DE("a", "b") }) == TD("[a: b]"));
        REQUIRE(TD({ DE("a", 1, 2) }) == TD("[a: 1 2]"));
        REQUIRE(TD({ DE("a", 1), DE("b", 2) }) == TD("[a: 1 b: 2]"));
    }

    SECTION("copy")
    {
        DataTypeDict d0;
        d0.insert("key0", A("value0"));
        d0.insert("key1", A("value1"));

        REQUIRE(d0.size() == 2);

        DataTypeDict d1(d0);

        REQUIRE(d1.size() == 2);
        REQUIRE(d1.contains("key0"));
        REQUIRE(d1.contains("key1"));

        REQUIRE(d0.isEqual(&d1));
        REQUIRE(d1.isEqual(&d0));
    }

    SECTION("clone")
    {
        DataTypeDict d0;
        d0.insert("key0", A("value0"));
        d0.insert("key1", A("value1"));

        DataTypeDict* d1 = d0.cloneT<DataTypeDict>();
        REQUIRE(*d1 == d0);
        REQUIRE(d1->size() == 2);
        delete d1;
    }

    SECTION("insert")
    {
        DataTypeDict d0;
        d0.insert("none", Atom());
        d0.insert("s0", A("string"));
        d0.insert("s1", A("a string"));
        d0.insert("f0", -10);
        d0.insert("f1", 1234.5);
        d0.insert("int_data", Atom(new IntData(100)));
        d0.insert("str_data", Atom(new DataTypeString("string with spaces")));
        d0.insert("l0", L());
        d0.insert("l1", LF(1));
        d0.insert("l2", LA("a", "b"));

        CHECK(d0.contains("none"));
        CHECK(d0.contains("s0"));
        CHECK(d0.contains("s1"));
        CHECK(d0.contains("f1"));
        CHECK(d0.contains("f1"));
        CHECK(d0.contains("l0"));
        CHECK(d0.contains("l1"));
        CHECK(d0.contains("l2"));
        CHECK(d0.contains("int_data"));
        CHECK(d0.contains("str_data"));

        //#ifdef __APPLE__
        //        REQUIRE(d0.toString() == "[l2: a b "
        //                                 "none: NONE "
        //                                 "s0: string "
        //                                 "s1: \"a string\" "
        //                                 "f0: -10 "
        //                                 "f1: 1234.5 "
        //                                 "int_data: 100 "
        //                                 "str_data: \"string with spaces\" "
        //                                 "l0: "
        //                                 "l1: 1]");
        //#endif
    }

    SECTION("remove")
    {
        DataTypeDict d0;

        REQUIRE_FALSE(d0.remove("test"));

        d0.insert("string", A("string"));
        d0.insert("float", 123);

        REQUIRE(d0.contains("string"));
        REQUIRE(d0.contains("float"));

        REQUIRE(d0.remove("string"));
        REQUIRE_FALSE(d0.contains("string"));
        REQUIRE(d0.contains("float"));

        REQUIRE(d0.remove("float"));
        REQUIRE_FALSE(d0.contains("float"));

        REQUIRE_FALSE(d0.remove("float"));
    }

    SECTION("toJSON")
    {
        using IntA = DataAtom<IntData>;

        DataTypeDict src;
        REQUIRE(!src.toJSON());

        src.insert("a", A("a b c"));
        REQUIRE(src.toJSON());
        REQUIRE(*src.toJSON() == "{\"a\":\"a b c\"}");

        src.clear();
        src.insert("b", 1000);
        REQUIRE(src.toJSON());
        REQUIRE(*src.toJSON() == "{\"b\":1000}");

        src.clear();
        src.insert("c", LF(1, 2, 3));
        REQUIRE(src.toJSON());
        REQUIRE(*src.toJSON() == "{\"c\":[1,2,3]}");

        src.clear();
        src.insert("d", LA(100.5, "ABC"));
        REQUIRE(src.toJSON());
        REQUIRE(*src.toJSON() == "{\"d\":[100.5,\"ABC\"]}");

        src.clear();
        src.insert("e", LA(IntA(-1), "ABC"));
        REQUIRE(src.toJSON());
        REQUIRE(*src.toJSON() == "{\"e\":[-1,\"ABC\"]}");

        src.clear();
        src.insert("f", LA(MListAtom(1, 2), "ABC"));
        REQUIRE(src.toJSON());
        REQUIRE(*src.toJSON() == "{\"f\":[[1,2],\"ABC\"]}");
    }

    //    SECTION("fromJSON")
    //    {
    //        DataTypeDict src;
    //        //        REQUIRE(src.fromJSON("") == false);

    //        REQUIRE(src.fromJSON("{\"a\":\"a b c\"}"));
    //        REQUIRE(src.size() == 1);
    //        REQUIRE(src.contains(A("a")));
    //        REQUIRE(src.valueT<Atom>(A("a")) == A("a b c"));

    //        REQUIRE(src.fromJSON("{\"b\":123}"));
    //        REQUIRE(src.size() == 1);
    //        REQUIRE(src.contains(A("b")));
    //        REQUIRE(src.valueT<Atom>(A("b")) == A(123));

    //        REQUIRE(src.fromJSON("{\"d\":true}"));
    //        REQUIRE(src.size() == 1);
    //        REQUIRE(src.contains(A("d")));
    //        REQUIRE(src.valueT<Atom>(A("d")) == A(1));

    //        REQUIRE(src.fromJSON("{\"d\":false}"));
    //        REQUIRE(src.size() == 1);
    //        REQUIRE(src.contains(A("d")));
    //        REQUIRE(src.valueT<Atom>(A("d")) == A(0.f));

    //        REQUIRE(src.fromJSON("{\"e\": [1, 2, 3]}"));
    //        REQUIRE(src.size() == 1);
    //        REQUIRE(src.contains(A("e")));
    //        REQUIRE(src.valueT<AtomList>(A("e")) == LF(1, 2, 3));

    //        REQUIRE(src.fromJSON("{\"f\": [1, 2, \"ABC\"]}"));
    //        REQUIRE(src.size() == 1);
    //        REQUIRE(src.contains(A("f")));
    //        REQUIRE(src.valueT<AtomList>(A("f")) == LA(1, 2, "ABC"));

    //        // complex array
    //        REQUIRE(src.fromJSON("{\"g\": [1, 2, [3,4,5], [6, 7], {\"a\": 200}]}"));
    //        REQUIRE(src.size() == 1);
    //        REQUIRE(src.contains(A("g")));
    //        REQUIRE(src.valueT<DataAtom>(A("g"))->isData());
    //        REQUIRE(src.valueT<DataAtom>(A("g"))->data().as<DataTypeMList>() != 0);
    //        auto lst0 = *src.valueT<DataAtom>(A("g"))->data().as<DataTypeMList>();
    //        REQUIRE(lst0.size() == 5);
    //        REQUIRE(lst0.at(0) == DataAtom(A(1)));
    //        REQUIRE(lst0.at(1) == DataAtom(A(2)));
    //        auto ml0 = DataTypeMList(LF(3, 4, 5));
    //        REQUIRE(lst0.at(2).data().as<DataTypeMList>()->isEqual(&ml0));
    //        auto ml1 = DataTypeMList(LF(6, 7));
    //        REQUIRE(lst0.at(3).data().as<DataTypeMList>()->isEqual(&ml1));
    //        REQUIRE(lst0.at(4).data().as<DataTypeDict>() != 0);
    //        auto dict0 = *lst0.at(4).data().as<DataTypeDict>();
    //        REQUIRE(dict0.contains(A("a")));
    //        REQUIRE(dict0.size() == 1);
    //        REQUIRE(dict0.valueT<Atom>(A("a"))->asFloat() == 200);

    // simple dict
    //        REQUIRE(src.fromJSON("{\"h\": {\"a'\": 1024, \"b'\": -100}}"));
    //        REQUIRE(src.size() == 1);
    //        REQUIRE(src.contains(A("h")));
    //        REQUIRE(src.valueT<DataAtom>(A("h"))->isData());
    //        REQUIRE(src.valueT<DataAtom>(A("h"))->data().as<DataTypeDict>() != 0);
    //        auto dict1 = *src.valueT<DataAtom>(A("h"))->data().as<DataTypeDict>();
    //        REQUIRE(dict1.size() == 2);
    //        REQUIRE(dict1.contains(A("a'")));
    //        REQUIRE(dict1.contains(A("b'")));
    //        REQUIRE(dict1.valueT<Atom>(A("a'"))->asFloat() == 1024);
    //        REQUIRE(dict1.valueT<Atom>(A("b'"))->asFloat() == -100);

    //        // spaces dict
    //        REQUIRE(src.fromJSON("{\"key space\": {\"a'\": \"value space\", \"b'\": -100}}"));
    //        REQUIRE(src.size() == 1);
    //        REQUIRE(src.contains(A("key space")));
    //        REQUIRE(src.valueT<DataAtom>(A("key space"))->isData());
    //        REQUIRE(src.valueT<DataAtom>(A("key space"))->data().as<DataTypeDict>() != 0);
    //        dict1 = *src.valueT<DataAtom>(A("key space"))->data().as<DataTypeDict>();
    //        REQUIRE(dict1.size() == 2);
    //        REQUIRE(dict1.contains(A("a'")));
    //        REQUIRE(dict1.contains(A("b'")));
    //        REQUIRE(dict1.valueT<Atom>(A("a'")) == A("value space"));
    //        REQUIRE(dict1.valueT<Atom>(A("b'"))->asFloat() == -100);
    //    }

    //    SECTION("toJSON")
    //    {
    //        DataTypeDict d;

    //        REQUIRE(d.toJSON() == boost::none);
    //        d.insert("a", "abc");
    //        REQUIRE(*d.toJSON() == "{\"a\":\"abc\"}");
    //        d.insert("f", 200);
    //        REQUIRE(*d.toJSON() == "{\"a\":\"abc\",\"f\":200.0}");

    //        d.clear();
    //        d.insert(A("l"), LA(1, 2, "a space"));
    //        REQUIRE(*d.toJSON() == "{\"l\":[1.0,2.0,\"a space\"]}");

    //        d.clear();
    //        DataPtr dict_data(new DataTypeDict("[a:abc]"));
    //        d.insert(A("d"), DataAtom(dict_data));
    //        REQUIRE(*d.toJSON() == "{\"d\":{\"a\":\"abc\"}}");
    //    }

    SECTION("fromList")
    {
        REQUIRE(Atom() == Atom());
        REQUIRE(Dict::fromList(LF(1, 2, 3, 4), 0) == Dict());
        REQUIRE(Dict::fromList(LA("a", "b", "c"), 1) == Dict("[a: b: c:]"));
        REQUIRE(Dict::fromList(LA("a", "b", "c", "d"), 2) == Dict("[a: b c: d]"));
    }

    SECTION("removeIf")
    {
        Dict d("[a: 1 b: 2 c: 3]");
        d.removeIf([](const Atom& k) { return k == A("a"); });
        REQUIRE(d == Dict("[c: 3 b: 2]"));
    }

    SECTION("fromDataString")
    {
        Dict d;
        REQUIRE(d.setFromDataString("[]"));
        REQUIRE(d.size() == 0);
        REQUIRE(d.setFromDataString("[a: 12]"));
        REQUIRE(d.size() == 1);
        REQUIRE(d.keys() == LA("a"));
        REQUIRE(d.at("a") == LF(12));

        REQUIRE(d.setFromDataString("[@a: 12]"));
        REQUIRE(d.size() == 1);
        REQUIRE(d.keys() == LA("@a"));
        REQUIRE(d.at("@a") == LF(12));

        REQUIRE(d.setFromDataString("[@a: 12 @b: 15]"));
        REQUIRE(d.size() == 2);
        REQUIRE(d.at("@a") == LF(12));
        REQUIRE(d.at("@b") == LF(15));

        REQUIRE(d.setFromDataString("[@ABC: абвгд]"));
        REQUIRE(d.size() == 1);
        REQUIRE(d.at("@ABC") == LA("абвгд"));

        REQUIRE(d.setFromDataString("Dict[a:]"));
        REQUIRE(d.size() == 1);
        REQUIRE(d.at("a") == L());

        REQUIRE(d.setFromDataString("Dict[123: 4]"));
        REQUIRE(d.size() == 1);
        REQUIRE(d.at("123") == LF(4));
    }

    SECTION("toString")
    {
#define CHECK_TO_DSC(src_str, res_str)             \
    {                                              \
        DataTypeDict d;                            \
        d.setFromDataString(src_str);              \
        CHECK(d.toDictStringContent() == res_str); \
    }

#define CHECK_TO_DS(src_str, res_str)       \
    {                                       \
        DataTypeDict d;                     \
        d.setFromDataString(src_str);       \
        CHECK(d.toDictString() == res_str); \
    }

#define CHECK_TO_STRING(src_str, res_str) \
    {                                     \
        DataTypeDict d;                   \
        d.setFromDataString(src_str);     \
        CHECK(d.toString() == res_str);   \
    }

#define CHECK_TO_LSC(src_str, res_str)             \
    {                                              \
        DataTypeDict d;                            \
        d.setFromDataString(src_str);              \
        CHECK(d.toListStringContent() == res_str); \
    }

#define CHECK_TO_LS(src_str, res_str)       \
    {                                       \
        DataTypeDict d;                     \
        d.setFromDataString(src_str);       \
        CHECK(d.toListString() == res_str); \
    }

        SECTION("toListStringContent")
        {
            CHECK_TO_LSC("Dict[a:]", "a ()");
            CHECK_TO_LSC("Dict[a: 1]", "a (1)");
            CHECK_TO_LSC("Dict[a: -0.25]", "a (-0.25)");
            CHECK_TO_LSC("Dict[a: 1/4]", "a (0.25)");
            CHECK_TO_LSC("Dict[a: 0xFF]", "a (255)");
            CHECK_TO_LSC("Dict[a: 0b0101]", "a (5)");
            CHECK_TO_LSC("Dict[a: 1 2 3]", "a (1 2 3)");
            CHECK_TO_LSC("Dict[a: A B C]", "a (A B C)");
            CHECK_TO_LSC("Dict[a: \"A B C\"]", "a (\"A B C\")");
            CHECK_TO_LSC("Dict[a: \"`\"A B C`\"\"]", "a (\"`\"A B C`\"\")");
            CHECK_TO_LSC("Dict[a: MList(1 2 3)]", "a ((1 2 3))");
            CHECK_TO_LSC("Dict[a: S\"1 2 3\"]", "a (S\"1 2 3\")");
            CHECK_TO_LSC("Dict[a: String(\"``a\")]", "a (S\"``a\")");
            CHECK_TO_LSC("Dict[a: @a]", "a (\"@a\")");
        }

        SECTION("toListString")
        {
            CHECK_TO_LS("Dict[a:]", "Dict(a ())");
        }

        SECTION("toString")
        {
            CHECK_TO_STRING("", "[]");
            CHECK_TO_STRING(" ", "[]");
            CHECK_TO_STRING("Dict[a:]", "[a:]");
            CHECK_TO_STRING("Dict[a: 1]", "[a: 1]");
            CHECK_TO_STRING("Dict[a: -0.25]", "[a: -0.25]");
            CHECK_TO_STRING("Dict[a: 1/4]", "[a: 0.25]");
            CHECK_TO_STRING("Dict[a: 0xFF]", "[a: 255]");
            CHECK_TO_STRING("Dict[a: 0b0101]", "[a: 5]");
            CHECK_TO_STRING("Dict[a: 1 2 3]", "[a: 1 2 3]");
            CHECK_TO_STRING("Dict[a: A B C]", "[a: A B C]");
            CHECK_TO_STRING("Dict[a: \"A B C\"]", "[a: \"A B C\"]");
            CHECK_TO_STRING("Dict[a: \"`\"A B C`\"\"]", "[a: \"`\"A B C`\"\"]");
            CHECK_TO_STRING("Dict[a: MList(1 2 3)]", "[a: (1 2 3)]");
            CHECK_TO_STRING("Dict[a: S\"1 2 3\"]", "[a: S\"1 2 3\"]");
            CHECK_TO_STRING("Dict[a: String(\"``a\")]", "[a: S\"``a\"]");
            CHECK_TO_STRING("Dict[a: @a]", "[a: \"@a\"]");
        }

        SECTION("toDictString")
        {
            CHECK_TO_DS("", "Dict[]");
            CHECK_TO_DS(" ", "Dict[]");
            CHECK_TO_DS("Dict[a:]", "Dict[a:]");
            CHECK_TO_DS("Dict[a: 1]", "Dict[a: 1]");
            CHECK_TO_DS("Dict[a: -0.25]", "Dict[a: -0.25]");
            CHECK_TO_DS("Dict[a: 1/4]", "Dict[a: 0.25]");
            CHECK_TO_DS("Dict[a: 0xFF]", "Dict[a: 255]");
            CHECK_TO_DS("Dict[a: 0b0101]", "Dict[a: 5]");
            CHECK_TO_DS("Dict[a: 1 2 3]", "Dict[a: 1 2 3]");
            CHECK_TO_DS("Dict[a: A B C]", "Dict[a: A B C]");
            CHECK_TO_DS("Dict[a: \"A B C\"]", "Dict[a: \"A B C\"]");
            CHECK_TO_DS("Dict[a: \"`\"A B C`\"\"]", "Dict[a: \"`\"A B C`\"\"]");
            CHECK_TO_DS("Dict[a: MList(1 2 3)]", "Dict[a: (1 2 3)]");
            CHECK_TO_DS("Dict[a: S\"1 2 3\"]", "Dict[a: S\"1 2 3\"]");
            CHECK_TO_DS("Dict[a: String(\"``a\")]", "Dict[a: S\"``a\"]");
            CHECK_TO_DS("Dict[a: @a]", "Dict[a: \"@a\"]");
        }

        SECTION("toDictStringContent")
        {
            CHECK_TO_DSC("", "");
            CHECK_TO_DSC(" ", "");
            CHECK_TO_DSC("Dict[a:]", "a:");
            CHECK_TO_DSC("Dict[a: 1]", "a: 1");
            CHECK_TO_DSC("Dict[a: -0.25]", "a: -0.25");
            CHECK_TO_DSC("Dict[a: 1/4]", "a: 0.25");
            CHECK_TO_DSC("Dict[a: 0xFF]", "a: 255");
            CHECK_TO_DSC("Dict[a: 0b0101]", "a: 5");
            CHECK_TO_DSC("Dict[a: 1 2 3]", "a: 1 2 3");
            CHECK_TO_DSC("Dict[a: A B C]", "a: A B C");
            CHECK_TO_DSC("Dict[a: \"A B C\"]", "a: \"A B C\"");
            CHECK_TO_DSC("Dict[a: \"`\"A B C`\"\"]", "a: \"`\"A B C`\"\"");
            CHECK_TO_DSC("Dict[a: MList(1 2 3)]", "a: (1 2 3)");
            CHECK_TO_DSC("Dict[a: S\"1 2 3\"]", "a: S\"1 2 3\"");
            CHECK_TO_DSC("Dict[a: String(\"``a\")]", "a: S\"``a\"");
            CHECK_TO_DSC("Dict[a: @a]", "a: \"@a\"");
        }
    }
}
