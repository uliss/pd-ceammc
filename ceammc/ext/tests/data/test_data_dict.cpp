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
#include "ceammc_platform.h"
#include "data_dict.h"
#include "test_data_base.h"

#define REQUIRE_SIZE(obj, n) REQUIRE_PROPERTY(t, @size, float(n));

#define REQUIRE_DATA_EQUAL_AT_OUTLET(outlet, obj, data)       \
    {                                                         \
        REQUIRE(obj.hasNewMessages(outlet));                  \
        REQUIRE(obj.lastMessage(outlet).isData());            \
        REQUIRE(obj.lastMessage(outlet).atomValue() == data); \
    }

#define REQUIRE_OUTPUT_DICT(obj, data)                   \
    {                                                    \
        REQUIRE(obj.hasNewMessages(0));                  \
        REQUIRE(obj.lastMessage(0).isData());            \
        REQUIRE(obj.lastMessage(0).atomValue() == data); \
    }

#define REQUIRE_CONTAINS_ATOM(obj, key, value)           \
    {                                                    \
        REQUIRE(obj.dict()->contains(key));              \
        REQUIRE(obj.dict()->at(key) == atomFrom(value)); \
    }

#define REQUIRE_CONTAINS_LIST(obj, key, lst) \
    {                                        \
        REQUIRE(obj.dict()->contains(key));  \
        REQUIRE(obj.dict()->at(key) == lst); \
    }

PD_COMPLETE_TEST_SETUP(DataDict, data, dict)

TEST_CASE("data.dict", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TObj t("data.dict", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_SIZE(t, 0);

            REQUIRE_PROPERTY(t, @size, 0);
            REQUIRE_PROPERTY(t, @empty, 1);
        }

        SECTION("args")
        {
            TObj t("data.dict", LA("[a:", "b", "c:", "100]"));
            REQUIRE_SIZE(t, 2);
            REQUIRE_CONTAINS_ATOM(t, "a", "b");
            REQUIRE_CONTAINS_ATOM(t, "c", 100);

            REQUIRE_PROPERTY(t, @size, 2);
            REQUIRE_PROPERTY(t, @empty, 0);
            // order is undefined
            //            REQUIRE_PROPERTY(t, @keys, "a", "c");
        }

        SECTION("quotes")
        {
            TObj t("data.dict", LA("[", "key:", "\"the", "value\"", "]"));
            REQUIRE_SIZE(t, 1);
            REQUIRE_CONTAINS_ATOM(t, "key", "the value");
        }

        SECTION("nesting")
        {
            DataTypeDict t("[a: ([b: c d] [e: f])]");
            REQUIRE(t.size() == 1);
            REQUIRE(t.contains("a"));
            REQUIRE(*t.toJSON(-1) == "{\"a\":[{\"b\":[\"c\",\"d\"]},{\"e\":\"f\"}]}");
        }
    }

    SECTION("onBang")
    {
        TObj t("data.dict");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DictA());

        WHEN_SEND_ANY_TO(t, LP("[a: b c: d e: 2.81]"));
        REQUIRE_NO_MSG(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DictA("[a: b c: d e: 2.81]"));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DictA("[a: b c: d  e: 2.81]"));

        SECTION("construct")
        {
            TObj t("data.mlist", LP("[a: 100]"));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DictA("[a: 100]"));
        }
    }

    SECTION("add")
    {
        TObj t("data.dict");
        REQUIRE_SIZE(t, 0);

        WHEN_CALL_N(t, add, "key", "value");
        REQUIRE_SIZE(t, 1);
        REQUIRE_CONTAINS_ATOM(t, "key", "value");

        // replace key
        WHEN_CALL_N(t, add, "key", 1, 2, 3);
        REQUIRE_SIZE(t, 1);
        REQUIRE_CONTAINS_LIST(t, "key", LF(1, 2, 3));

        // new key
        WHEN_CALL_N(t, add, "key2", 100);
        REQUIRE_SIZE(t, 2);
        REQUIRE_CONTAINS_LIST(t, "key", LF(1, 2, 3));
        REQUIRE_CONTAINS_ATOM(t, "key2", 100);
    }

    SECTION("clear")
    {
        TObj t("data.dict");
        REQUIRE_SIZE(t, 0);

        WHEN_CALL(t, clear);
        REQUIRE_NO_MSG(t);

        WHEN_CALL_N(t, add, "key", "value");
        REQUIRE_SIZE(t, 1);
        REQUIRE_CONTAINS_ATOM(t, "key", "value");

        WHEN_CALL(t, clear);
        REQUIRE_NO_MSG(t);
        REQUIRE_SIZE(t, 0);
    }

    SECTION("remove")
    {
        TObj t("data.dict");
        REQUIRE_SIZE(t, 0);

        WHEN_CALL(t, remove);
        REQUIRE_NO_MSG(t);

        WHEN_CALL_N(t, add, "a", "abc");
        WHEN_CALL_N(t, add, "b", 1, 2, 3);
        REQUIRE_SIZE(t, 2);

        WHEN_CALL_N(t, remove, "k");
        REQUIRE_SIZE(t, 2);
        WHEN_CALL_N(t, remove, "a");
        REQUIRE_CONTAINS_LIST(t, "b", LF(1, 2, 3));
        REQUIRE_SIZE(t, 1);
        WHEN_CALL_N(t, remove, "b");
        REQUIRE_SIZE(t, 0);
    }

    SECTION("JSON")
    {
        TObj t("data.dict");
        WHEN_CALL_N(t, add, "float", 1000);
        WHEN_CALL_N(t, add, "symbol", "b");
        WHEN_CALL_N(t, add, "list", 1, 2, 3);
        // invalid key
        WHEN_CALL_N(t, add, 1, 2000);
        REQUIRE_SIZE(t, 3);
        REQUIRE(t.dict() == DictA("[float: 1000 symbol: b list: 1 2 3]"));

        // write to file error
        WHEN_CALL(t, write);
        REQUIRE_FALSE(platform::path_exists(TEST_BIN_DIR "/data_dict.json"));

        // write to file success
        WHEN_CALL_N(t, write, TEST_BIN_DIR "/data_dict.json");
        REQUIRE(platform::path_exists(TEST_BIN_DIR "/data_dict.json"));

        // read to other dict
        TObj t2("data.dict");
        WHEN_CALL_N(t2, read, TEST_BIN_DIR "/data_dict.json");

        // remove json file
        REQUIRE(platform::remove(TEST_BIN_DIR "/data_dict.json"));

        REQUIRE_SIZE(t2, 3);
        REQUIRE(t2.dict() == DictA("[float: 1000 symbol: b list: 1 2 3]"));
        REQUIRE_CONTAINS_ATOM(t2, "float", 1000);
        REQUIRE_CONTAINS_ATOM(t2, "symbol", "b");
        REQUIRE_CONTAINS_LIST(t2, "list", LF(1, 2, 3));

        // clear source
        WHEN_CALL(t, clear);
        // write empty JSON file
        WHEN_CALL_N(t, write, TEST_BIN_DIR "/data_dict.json");
        REQUIRE(platform::path_exists(TEST_BIN_DIR "/data_dict.json"));

        // read empty JSON file
        WHEN_CALL_N(t2, read, TEST_BIN_DIR "/data_dict.json");
        REQUIRE_SIZE(t2, 0);

        REQUIRE(platform::remove(TEST_BIN_DIR "/data_dict.json"));
    }

    SECTION("ext")
    {
        TestExtDataDict t("data.dict");
        TestExtDataDict t_alias("dict");
        REQUIRE(t.object());
        REQUIRE(t_alias.object());
    }

    SECTION("list")
    {
        TObj t("data.dict");

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_SIZE(t, 0);
        WHEN_SEND_LIST_TO(0, t, LF(1000));
        REQUIRE_SIZE(t, 0);

        WHEN_SEND_LIST_TO(0, t, LA("a", "b"));
        REQUIRE_SIZE(t, 1);
        REQUIRE_CONTAINS_ATOM(t, "a", "b");

        WHEN_SEND_LIST_TO(0, t, LF(100, 200));
        REQUIRE_SIZE(t, 1);

        WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c", "d"));
        REQUIRE_SIZE(t, 2);
        REQUIRE_CONTAINS_ATOM(t, "a", "b");
        REQUIRE_CONTAINS_ATOM(t, "c", "d");
    }

    SECTION("iface")
    {
        TestExtDataDict t("data.dict");
        t.call("add", LA("a", "b"));

        REQUIRE(t.property("@keys")->get() == LA("a"));

#define REQUIRE_KEY_LIST(obj, key, val)      \
    {                                        \
        obj.call("get_key", A(key));         \
        REQUIRE(obj.hasOutput());            \
        REQUIRE(obj.isOutputListAt(0));      \
        REQUIRE(obj.outputListAt(0) == val); \
    }

#define REQUIRE_KEY_SYMBOL(obj, key, val)         \
    {                                             \
        REQUIRE(obj->dict()->at(key) == LA(val)); \
    }

        t.call("set_key");
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_KEY_SYMBOL(t, "a", "b");
        t.call("set_key", LA("b"));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_KEY_SYMBOL(t, "a", "b");
        t.call("set_key", LA("b", "c"));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_KEY_SYMBOL(t, "a", "b");
        t.call("set_key", LA("a"));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE(t->dict()->at("a") == L());

        t.call("set_key", LA("a", 1, 2, 3));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_KEY_LIST(t, "a", LF(1, 2, 3));

        t.call("get_key");
        REQUIRE_FALSE(t.hasOutput());
        t.call("get_key", LA("????"));
        REQUIRE_FALSE(t.hasOutput());
    }
}
