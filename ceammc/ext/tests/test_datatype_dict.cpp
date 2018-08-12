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
#include "../data/datatype_dict.h"
#include "../data/datatype_mlist.h"
#include "../data/dict_parser_impl.h"
#include "../lib/ceammc_datatypes.h"

#include "../data/dict_parser.tab.h"

#include "test_common.h"

#include <boost/optional/optional_io.hpp>

using namespace ceammc;

extern "C" int dict_parse_string(t_dict* dict, const char* s);

TEST_CASE("DataTypeDict", "[ceammc::DataTypeDict]")
{
    SECTION("init")
    {
        DataTypeDict d;
        REQUIRE(d.type() == data::DATA_DICT);
        REQUIRE(d.size() == 0);
        REQUIRE_FALSE(d.contains(A("test")));
        d.clear();
        REQUIRE(d.size() == 0);

        REQUIRE(d.innerData().empty());
        REQUIRE(d.value(A("ABC")).which() == 0);

        DataTypeDict d1("[]");
        REQUIRE(d.size() == 0);

        REQUIRE(d.isEqual(&d));
        REQUIRE(d.toString() == "");

        REQUIRE(d.valueT<Atom>(A("key-not-exists")) == boost::none);
        REQUIRE(d.valueT<AtomList>(A("key-not-exists")) == boost::none);
    }

    SECTION("copy")
    {
        DataTypeDict d0;
        d0.insert("key0", "value0");
        d0.insert("key1", "value1");

        REQUIRE(d0.size() == 2);

        DataTypeDict d1(d0);

        REQUIRE(d1.size() == 2);
        REQUIRE(d1.contains(A("key0")));
        REQUIRE(d1.contains(A("key1")));
        REQUIRE(DataTypeDict::isAtom(d1.value(A("key0"))));
        REQUIRE(DataTypeDict::isAtom(d1.value(A("key1"))));

        REQUIRE(d0.isEqual(&d1));
        REQUIRE(d1.isEqual(&d0));
    }

    SECTION("clone")
    {
        DataTypeDict d0;
        d0.insert("key0", "value0");
        d0.insert("key1", "value1");

        REQUIRE(d0.size() == 2);

        DataTPtr<DataTypeDict> ptr(d0.clone());

        REQUIRE(ptr->size() == 2);
        REQUIRE(ptr->contains(A("key0")));
        REQUIRE(ptr->contains(A("key1")));

        REQUIRE(d0.isEqual(ptr.data()));
        REQUIRE(ptr->isEqual(&d0));

        d0.clear();
        REQUIRE(d0.size() == 0);
        REQUIRE(ptr->size() == 2);

        REQUIRE_FALSE(d0.isEqual(ptr.data()));
        REQUIRE_FALSE(ptr->isEqual(&d0));
    }

    SECTION("insert")
    {
        DataTypeDict d0;
        d0.insert("string", "string");
        d0.insert("float", 123);
        d0.insert("float1", 10.f);
        d0.insert("atom", Atom());
        d0.insert(A("atom_float1"), 1000);
        d0.insert(A("atom_float2"), 2000.f);
        d0.insert(A("atom_atom"), A("ABC"));
        d0.insert(A("atom_list"), LF(1, 2, 3, 4));
        DataPtr data_int(new IntData(100));
        DataPtr data_str(new StrData("string with spaces"));
        d0.insert(A("atom_data1"), DataAtom(data_int));
        d0.insert(A("atom data2"), DataAtom(data_str));
        d0.insert(A(123), A("numeric"));

        REQUIRE_FALSE(d0.isEqual(data_int.data()));
        REQUIRE(DataTypeDict::isDataAtom(d0.value(A("atom_data1"))));

        REQUIRE(d0.contains(A("string")));
        REQUIRE(d0.contains(A("float")));
        REQUIRE(d0.contains(A("float1")));
        REQUIRE(d0.contains(A("atom")));
        REQUIRE(d0.contains(A("atom_float1")));
        REQUIRE(d0.contains(A("atom_float2")));
        REQUIRE(d0.contains(A("atom_atom")));
        REQUIRE(d0.contains(A("atom_list")));
        REQUIRE(d0.contains(A("atom_data1")));
        REQUIRE(d0.contains(A("atom data2")));
        REQUIRE(d0.contains(A(123)));

        REQUIRE(d0.value(A("string")).type() == typeid(Atom));
        REQUIRE(d0.value(A("float")).type() == typeid(Atom));
        REQUIRE(d0.value(A("float1")).type() == typeid(Atom));
        REQUIRE(d0.value(A("atom")).type() == typeid(Atom));
        REQUIRE(d0.value(A("atom_float1")).type() == typeid(Atom));
        REQUIRE(d0.value(A("atom_float2")).type() == typeid(Atom));
        REQUIRE(d0.value(A("atom_atom")).type() == typeid(Atom));
        REQUIRE(d0.value(A("atom_list")).type() == typeid(AtomList));
        REQUIRE(d0.value(A("atom_data1")).type() == typeid(DataAtom));
        REQUIRE(d0.value(A("atom data2")).type() == typeid(DataAtom));

#ifdef __APPLE__
        REQUIRE(d0.toString() == "[123: numeric] "
                                 "[atom: NONE] "
                                 "[\"atom data2\": \"string with spaces\"] "
                                 "[atom_atom: ABC] "
                                 "[atom_data1: 100] "
                                 "[atom_float1: 1000] "
                                 "[atom_float2: 2000] "
                                 "[atom_list: 1 2 3 4] "
                                 "[float: 123] "
                                 "[float1: 10] "
                                 "[string: string]");
#endif
    }

    SECTION("remove")
    {
        DataTypeDict d0;

        REQUIRE_FALSE(d0.remove(A("test")));

        d0.insert("string", "string");
        d0.insert("float", 123);

        REQUIRE(d0.contains(A("string")));
        REQUIRE(d0.contains(A("float")));

        REQUIRE(d0.remove(A("string")));
        REQUIRE_FALSE(d0.contains(A("string")));
        REQUIRE(d0.contains(A("float")));

        REQUIRE(d0.remove(A("float")));
        REQUIRE_FALSE(d0.contains(A("float")));

        REQUIRE_FALSE(d0.remove(A("float")));
    }

    SECTION("parse")
    {
        DataTypeDict src("");
        DataTypeDict dest, res;

        dest.fromString(src.toString());
        REQUIRE(dest == src);

        t_dict* d = dict_new();
        // single key
        dict_parse_string(d, "[key: value]");
        res = dict_get(d);
        REQUIRE(res.size() == 1);
        REQUIRE(res.contains(A("key")));
        REQUIRE(res.valueT<Atom>(A("key")) == A("value"));
        // wrong type
        REQUIRE(res.valueT<AtomList>(A("key")) == boost::none);
        REQUIRE(res.valueT<Atom>(A("key-not-exists")) == boost::none);

        // float key
        dict_parse_string(d, "[100: value]");
        res = dict_get(d);
        REQUIRE(res.size() == 1);
        REQUIRE(res.contains(A(100)));
        REQUIRE(res.valueT<Atom>(A(100)) == A("value"));

        // single key no spaces
        dict_parse_string(d, "[key2:value]");
        res = dict_get(d);
        REQUIRE(res.size() == 1);
        REQUIRE(res.contains(A("key2")));
        REQUIRE(res.valueT<Atom>(A("key2")) == A("value"));

        // single key with spaces
        dict_parse_string(d, "[\"key with spaces\" : \"value with spaces\"]");
        res = dict_get(d);
        REQUIRE(res.size() == 1);
        REQUIRE(res.contains(A("key with spaces")));
        REQUIRE(res.valueT<Atom>(A("key with spaces")) == A("value with spaces"));

        // two keys
        dict_parse_string(d, "[key1: value1] [key2: value2]");
        res = dict_get(d);
        REQUIRE(res.size() == 2);
        REQUIRE(res.contains(A("key1")));
        REQUIRE(res.contains(A("key2")));

        // two keys
        dict_parse_string(d, "[a: 1] [b: 2] [10: 3]");
        res = dict_get(d);
        REQUIRE(res.size() == 3);
        REQUIRE(res.contains(A("a")));
        REQUIRE(res.contains(A("b")));
        REQUIRE(res.contains(A(10)));

        // value with spaces
        dict_parse_string(d, "[key1: \"a b c\"] [key2: value2]");
        res = dict_get(d);
        REQUIRE(res.size() == 2);
        REQUIRE(res.contains(A("key1")));
        REQUIRE(res.contains(A("key2")));
        REQUIRE(res.valueT<Atom>(A("key1")) == A("a b c"));

        // lists
        dict_parse_string(d, "[key: 1 2 3] [key2: a b c]");
        res = dict_get(d);
        REQUIRE(res.size() == 2);
        REQUIRE(res.contains(A("key")));
        REQUIRE(res.contains(A("key2")));
        REQUIRE(res.valueT<AtomList>(A("key")) == LF(1, 2, 3));
        REQUIRE(res.valueT<AtomList>(A("key2")) == LA("a", "b", "c"));

        // single key no quotes
        dict_parse_string(d, "a: 1");
        res = dict_get(d);
        REQUIRE(res.size() == 1);
        REQUIRE(res.contains(A("a")));
        REQUIRE(res.valueT<Atom>(A("a")) == A(1));

        // single key
        dict_parse_string(d, "a: \"1 2 3\"");
        res = dict_get(d);
        REQUIRE(res.size() == 1);
        REQUIRE(res.contains(A("a")));
        REQUIRE(res.valueT<Atom>(A("a")) == A("1 2 3"));

        // single key
        dict_parse_string(d, "\"a\": \"1\"");
        res = dict_get(d);
        REQUIRE(res.size() == 1);
        REQUIRE(res.contains(A("a")));
        REQUIRE(res.valueT<Atom>(A("a")) == A("1"));

        // single key
        dict_parse_string(d, "\"a\": 1");
        res = dict_get(d);
        REQUIRE(res.size() == 1);
        REQUIRE(res.contains(A("a")));
        REQUIRE(res.valueT<Atom>(A("a")) == A(1));

        // single key -> list
        dict_parse_string(d, "\"a\": 1 2 3");
        res = dict_get(d);
        REQUIRE(res.size() == 1);
        REQUIRE(res.contains(A("a")));
        REQUIRE(res.valueT<AtomList>(A("a")) == LF(1, 2, 3));
        REQUIRE(DataTypeDict::isAtomList(res.value(A("a"))));
        REQUIRE_FALSE(DataTypeDict::isDataAtom(res.value(A("a"))));

        // single key -> list
        dict_parse_string(d, "\"a\": 1 2 \"a test\"");
        res = dict_get(d);
        REQUIRE(res.size() == 1);
        REQUIRE(res.contains(A("a")));
        REQUIRE(res.valueT<AtomList>(A("a")) == LA(1, 2, "a test"));

        dict_free(d);
    }

    SECTION("toJSON")
    {
        DataTypeDict src;
        REQUIRE(src.toJSON() == boost::none);

        src.insert("a", "a b c");
        REQUIRE(src.toJSON());
        REQUIRE(*src.toJSON() == "{\"a\":\"a b c\"}");

        src.clear();
        src.insert("b", 1000);
        REQUIRE(src.toJSON());
        REQUIRE(*src.toJSON() == "{\"b\":1000.0}");

        src.clear();
        src.insert(A("c"), LF(1, 2, 3));
        REQUIRE(src.toJSON());
        REQUIRE(*src.toJSON() == "{\"c\":[1.0,2.0,3.0]}");

        src.clear();
        src.insert(A("d"), LA(100, "ABC"));
        REQUIRE(src.toJSON());
        REQUIRE(*src.toJSON() == "{\"d\":[100.0,\"ABC\"]}");

        src.clear();
        DataPtr int_data(new IntData(1000));
        src.insert(A("e"), LA(int_data, "ABC"));
        REQUIRE(src.toJSON());
        REQUIRE(*src.toJSON() == "{\"e\":[\"1000\",\"ABC\"]}");

        src.clear();
        src.insert(A("f"), DataAtom(int_data));
        REQUIRE(src.toJSON());
        REQUIRE(*src.toJSON() == "{\"f\":\"1000\"}");
    }

    SECTION("fromJSON")
    {
        DataTypeDict src;
        //        REQUIRE(src.fromJSON("") == false);

        REQUIRE(src.fromJSON("{\"a\":\"a b c\"}"));
        REQUIRE(src.size() == 1);
        REQUIRE(src.contains(A("a")));
        REQUIRE(src.valueT<Atom>(A("a")) == A("a b c"));

        REQUIRE(src.fromJSON("{\"b\":123}"));
        REQUIRE(src.size() == 1);
        REQUIRE(src.contains(A("b")));
        REQUIRE(src.valueT<Atom>(A("b")) == A(123));

        REQUIRE(src.fromJSON("{\"d\":true}"));
        REQUIRE(src.size() == 1);
        REQUIRE(src.contains(A("d")));
        REQUIRE(src.valueT<Atom>(A("d")) == A(1));

        REQUIRE(src.fromJSON("{\"d\":false}"));
        REQUIRE(src.size() == 1);
        REQUIRE(src.contains(A("d")));
        REQUIRE(src.valueT<Atom>(A("d")) == A(0.f));

        REQUIRE(src.fromJSON("{\"e\": [1, 2, 3]}"));
        REQUIRE(src.size() == 1);
        REQUIRE(src.contains(A("e")));
        REQUIRE(src.valueT<AtomList>(A("e")) == LF(1, 2, 3));

        REQUIRE(src.fromJSON("{\"f\": [1, 2, \"ABC\"]}"));
        REQUIRE(src.size() == 1);
        REQUIRE(src.contains(A("f")));
        REQUIRE(src.valueT<AtomList>(A("f")) == LA(1, 2, "ABC"));

        // complex array
        REQUIRE(src.fromJSON("{\"g\": [1, 2, [3,4,5], [6, 7], {\"a\": 200}]}"));
        REQUIRE(src.size() == 1);
        REQUIRE(src.contains(A("g")));
        REQUIRE(src.valueT<DataAtom>(A("g"))->isData());
        REQUIRE(src.valueT<DataAtom>(A("g"))->data().as<DataTypeMList>() != 0);
        auto lst0 = *src.valueT<DataAtom>(A("g"))->data().as<DataTypeMList>();
        REQUIRE(lst0.size() == 5);
        REQUIRE(lst0.at(0) == DataAtom(A(1)));
        REQUIRE(lst0.at(1) == DataAtom(A(2)));
        auto ml0 = DataTypeMList(LF(3, 4, 5));
        REQUIRE(lst0.at(2).data().as<DataTypeMList>()->isEqual(&ml0));
        auto ml1 = DataTypeMList(LF(6, 7));
        REQUIRE(lst0.at(3).data().as<DataTypeMList>()->isEqual(&ml1));
        REQUIRE(lst0.at(4).data().as<DataTypeDict>() != 0);
        auto dict0 = *lst0.at(4).data().as<DataTypeDict>();
        REQUIRE(dict0.contains(A("a")));
        REQUIRE(dict0.size() == 1);
        REQUIRE(dict0.valueT<Atom>(A("a"))->asFloat() == 200);

        // simple dict
        REQUIRE(src.fromJSON("{\"h\": {\"a'\": 1024, \"b'\": -100}}"));
        REQUIRE(src.size() == 1);
        REQUIRE(src.contains(A("h")));
        REQUIRE(src.valueT<DataAtom>(A("h"))->isData());
        REQUIRE(src.valueT<DataAtom>(A("h"))->data().as<DataTypeDict>() != 0);
        auto dict1 = *src.valueT<DataAtom>(A("h"))->data().as<DataTypeDict>();
        REQUIRE(dict1.size() == 2);
        REQUIRE(dict1.contains(A("a'")));
        REQUIRE(dict1.contains(A("b'")));
        REQUIRE(dict1.valueT<Atom>(A("a'"))->asFloat() == 1024);
        REQUIRE(dict1.valueT<Atom>(A("b'"))->asFloat() == -100);

        // spaces dict
        REQUIRE(src.fromJSON("{\"key space\": {\"a'\": \"value space\", \"b'\": -100}}"));
        REQUIRE(src.size() == 1);
        REQUIRE(src.contains(A("key space")));
        REQUIRE(src.valueT<DataAtom>(A("key space"))->isData());
        REQUIRE(src.valueT<DataAtom>(A("key space"))->data().as<DataTypeDict>() != 0);
        dict1 = *src.valueT<DataAtom>(A("key space"))->data().as<DataTypeDict>();
        REQUIRE(dict1.size() == 2);
        REQUIRE(dict1.contains(A("a'")));
        REQUIRE(dict1.contains(A("b'")));
        REQUIRE(dict1.valueT<Atom>(A("a'")) == A("value space"));
        REQUIRE(dict1.valueT<Atom>(A("b'"))->asFloat() == -100);
    }

    SECTION("toJSON")
    {
        DataTypeDict d;

        REQUIRE(d.toJSON() == boost::none);
        d.insert("a", "abc");
        REQUIRE(*d.toJSON() == "{\"a\":\"abc\"}");
        d.insert("f", 200);
        REQUIRE(*d.toJSON() == "{\"a\":\"abc\",\"f\":200.0}");

        d.clear();
        d.insert(A("l"), LA(1, 2, "a space"));
        REQUIRE(*d.toJSON() == "{\"l\":[1.0,2.0,\"a space\"]}");

        d.clear();
        DataPtr dict_data(new DataTypeDict("[a:abc]"));
        d.insert(A("d"), DataAtom(dict_data));
        REQUIRE(*d.toJSON() == "{\"d\":{\"a\":\"abc\"}}");
    }
}
