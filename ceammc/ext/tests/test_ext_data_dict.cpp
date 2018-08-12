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
#include "../data/data_dict.h"
#include "ceammc_platform.h"
#include "test_external.h"

#define REQUIRE_SIZE(obj, n) REQUIRE_PROPERTY(t, @size, float(n));

#define REQUIRE_DATA_EQUAL_AT_OUTLET(outlet, obj, data)       \
    {                                                         \
        REQUIRE(obj.hasNewMessages(outlet));                  \
        REQUIRE(obj.lastMessage(outlet).isData());            \
        REQUIRE(obj.lastMessage(outlet).dataValue() == data); \
    }

#define REQUIRE_OUTPUT_DICT(obj, data)                   \
    {                                                    \
        REQUIRE(obj.hasNewMessages(0));                  \
        REQUIRE(obj.lastMessage(0).isData());            \
        REQUIRE(obj.lastMessage(0).dataValue() == data); \
    }

#define REQUIRE_CONTAINS_ATOM(obj, key, value)                                           \
    {                                                                                    \
        REQUIRE(obj.dict().contains(test_atom_wrap(key)));                               \
        REQUIRE(*obj.dict().valueT<Atom>(test_atom_wrap(key)) == test_atom_wrap(value)); \
    }

#define REQUIRE_CONTAINS_LIST(obj, key, lst)                               \
    {                                                                      \
        REQUIRE(obj.dict().contains(test_atom_wrap(key)));                 \
        REQUIRE(*obj.dict().valueT<AtomList>(test_atom_wrap(key)) == lst); \
    }

PD_COMPLETE_TEST_SETUP(DataDict, data, dict);

TEST_CASE("data.mlist", "[externals]")
{
    pd_test_mod_init_data_dict();

    SECTION("create")
    {
        SECTION("empty")
        {
            TestDataDict t("data.dict", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_SIZE(t, 0);

            REQUIRE_PROPERTY_FLOAT(t, @size, 0);
            REQUIRE_PROPERTY_FLOAT(t, @empty, 1);
        }

        SECTION("args")
        {
            TestDataDict t("data.dict", LA("[a:", "b]", "[", "c", ":", "100]"));
            REQUIRE_SIZE(t, 2);
            REQUIRE_CONTAINS_ATOM(t, "a", "b");
            REQUIRE_CONTAINS_ATOM(t, "c", 100);

            REQUIRE_PROPERTY_FLOAT(t, @size, 2);
            REQUIRE_PROPERTY_FLOAT(t, @empty, 0);
            REQUIRE_PROPERTY_LIST(t, @keys, LA("a", "c"));
        }

        SECTION("quotes")
        {
            TestDataDict t("data.dict", LA("[", "key:", "\"the", "value\"", "]"));
            REQUIRE_SIZE(t, 1);
            REQUIRE_CONTAINS_ATOM(t, "key", "the value");
        }
    }

    SECTION("onBang")
    {
        TestDataDict t("data.dict");

        WHEN_SEND_BANG_TO(0, t);
        t.onBang();
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeDict()));

        WHEN_SEND_ANY_TO(t, LA("[a:b][c:d][e:2.81]"));
        REQUIRE_NO_MSG(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeDict("[a:b] [c : d] [ e : 2.81]")));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeDict("[a:b] [c : d] [ e : 2.81]")));

        SECTION("construct")
        {
            TestDataDict t("data.mlist", LA("[a : 100]"));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeDict("[a : 100]")));
        }
    }

    SECTION("add")
    {
        TestDataDict t("data.dict");
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
        TestDataDict t("data.dict");
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
        TestDataDict t("data.dict");
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
        test::pdPrintToStdError(true);

        TestDataDict t("data.dict");
        WHEN_CALL_N(t, add, "float", 1000);
        WHEN_CALL_N(t, add, "symbol", "b");
        WHEN_CALL_N(t, add, "list", 1, 2, 3);
        WHEN_CALL_N(t, add, 1, 2000);

        WHEN_CALL(t, write);
        REQUIRE_SIZE(t, 4);

        WHEN_CALL_N(t, write, TEST_DATA_DIR "/data_dict.json");
        REQUIRE(platform::path_exists(TEST_DATA_DIR "/data_dict.json"));

        // read to other dict
        TestDataDict t2("data.dict");
        WHEN_CALL_N(t2, read, TEST_DATA_DIR "/data_dict.json");
        REQUIRE_SIZE(t2, 4);
        REQUIRE_CONTAINS_ATOM(t2, 1, 2000);
        REQUIRE_CONTAINS_ATOM(t2, "float", 1000);
        REQUIRE_CONTAINS_ATOM(t2, "symbol", "b");
        REQUIRE_CONTAINS_LIST(t2, "list", LF(1, 2, 3));

        // remove json file
        REQUIRE(platform::remove(TEST_DATA_DIR "/data_dict.json"));

        // clear source
        WHEN_CALL(t, clear);
        // write empty JSON file
        WHEN_CALL_N(t, write, TEST_DATA_DIR "/data_dict.json");
        REQUIRE(platform::path_exists(TEST_DATA_DIR "/data_dict.json"));

        // read empty JSON file
        WHEN_CALL_N(t2, read, TEST_DATA_DIR "/data_dict.json");
        REQUIRE_SIZE(t2, 0);

        REQUIRE(platform::remove(TEST_DATA_DIR "/data_dict.json"));
    }

    SECTION("ext")
    {
        TestExtDataDict t("data.dict");
        TestExtDataDict t_alias("dict");
    }

    SECTION("list")
    {
        TestDataDict t("data.dict");

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_SIZE(t, 0);
        WHEN_SEND_LIST_TO(0, t, LF(1000));
        REQUIRE_SIZE(t, 0);

        WHEN_SEND_LIST_TO(0, t, LA("a", "b"));
        REQUIRE_SIZE(t, 1);
        REQUIRE_CONTAINS_ATOM(t, "a", "b");

        WHEN_SEND_LIST_TO(0, t, LF(100, 200));
        REQUIRE_SIZE(t, 1);
        REQUIRE_CONTAINS_ATOM(t, 100, 200);

        WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c", "d"));
        REQUIRE_SIZE(t, 2);
        REQUIRE_CONTAINS_ATOM(t, "a", "b");
        REQUIRE_CONTAINS_ATOM(t, "c", "d");
    }
}
