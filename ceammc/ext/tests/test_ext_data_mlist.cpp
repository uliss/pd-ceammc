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
#include "../data/data_mlist.h"
#include "ceammc_pd.h"

#include "test_external.h"

#include <stdio.h>

#define REQUIRE_SIZE(obj, n) REQUIRE_PROPERTY(t, @size, float(n));

#define REQUIRE_LIST(obj, lst)               \
    {                                        \
        WHEN_SEND_BANG_TO(0, obj);           \
        REQUIRE_SIZE(obj, lst.size());       \
        REQUIRE_LIST_AT_OUTLET(0, obj, lst); \
    }

PD_COMPLETE_TEST_SETUP(DataMList, data, mlist);

typedef TestExternal<DataMList> DataMListTest;

TEST_CASE("data.mlist", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            DataMListTest t("data.mlist", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @empty, 1);
            REQUIRE_PROPERTY_FLOAT(t, @size, 0);
        }

        SECTION("args")
        {
            DataMListTest t("data.mlist", LA("(", "a", "b", 1, 10, ")"));
            REQUIRE_PROPERTY_FLOAT(t, @size, 4);
            REQUIRE_PROPERTY_FLOAT(t, @empty, 0);
        }

        SECTION("quotes")
        {
            DataMListTest t("data.mlist", LA("(\"", "b", "d", "\")"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @size, 1);
            REQUIRE_PROPERTY_FLOAT(t, @empty, 0);
        }
    }

    SECTION("append")
    {
        DataMListTest t("data.mlist");

        REQUIRE_PROPERTY_FLOAT(t, @size, 0);

        WHEN_CALL(t, append);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @size, 0);

        WHEN_CALL_N(t, append, 123);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @size, 1);

        DataPtr d0(new IntData(-1000));
        WHEN_CALL_N(t, append, d0);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @size, 2);
    }

#define REQUIRE_DATA_EQUAL_AT_OUTLET(outlet, obj, data)       \
    {                                                         \
        REQUIRE(obj.hasNewMessages(outlet));                  \
        REQUIRE(obj.lastMessage(outlet).isData());            \
        REQUIRE(obj.lastMessage(outlet).dataValue() == data); \
    }

    SECTION("onBang")
    {
        DataMListTest t("data.mlist");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList()));

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(1, 2, 3))));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(1, 2, 3))));

        SECTION("construct")
        {
            DataMListTest t("data.mlist", LA("(1 2 3 4)"));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(1, 2, 3, 4))));
        }
    }

    SECTION("onList")
    {
        DataMListTest t("data.mlist");

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(1, 2, 3))));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList()));
    }

    SECTION("clear")
    {
        DataMListTest t("data.mlist", LA("(1 2 3 4 (1 2))"));

        REQUIRE_PROPERTY_FLOAT(t, @size, 5);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList("(1 2 3 4 ( 1 2 ))")));

        WHEN_CALL(t, clear);
        REQUIRE_PROPERTY_FLOAT(t, @size, 0);
    }

    SECTION("set")
    {
        DataMListTest t("data.mlist");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList()));

        WHEN_CALL_N(t, set, 1, 2);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(1, 2))));
    }

    SECTION("insert")
    {
        DataMListTest t("data.mlist");

        WHEN_CALL(t, insert);
        REQUIRE_NO_MSG(t);

        WHEN_CALL_N(t, insert, 0.f);
        REQUIRE_NO_MSG(t);

        WHEN_CALL_N(t, insert, 1, 10, 20);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @size, 0);

        WHEN_CALL_N(t, insert, 0.f, 10, 20);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @size, 2);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(10, 20))));

        WHEN_CALL_N(t, insert, 0.f, 30, 40);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(30, 40, 10, 20))));

        WHEN_CALL_N(t, insert, 1, 50);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LA(30, 50, 40, 10, 20))));

        WHEN_CALL_N(t, insert, -1, 60);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LA(30, 50, 40, 10, 60, 20))));

        WHEN_CALL_N(t, insert, -10, 60);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LA(30, 50, 40, 10, 60, 20))));
    }

    SECTION("pop")
    {
        DataMListTest t("data.mlist");

        WHEN_CALL(t, pop);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(1, 2, 3, 4))));

        WHEN_CALL(t, pop);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(1, 2, 3))));

        WHEN_CALL(t, pop);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(1, 2))));

        WHEN_CALL(t, pop);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(1))));

        WHEN_CALL(t, pop);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList()));

        WHEN_CALL(t, pop);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList()));
    }

    SECTION("remove")
    {
        DataMListTest t("data.mlist");

        // empty
        WHEN_CALL(t, pop);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(1, 2, 3, 4))));

        WHEN_CALL(t, removeAt);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(1, 2, 3, 4))));

        WHEN_CALL_N(t, removeAt, 0.f);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(2, 3, 4))));

        WHEN_CALL_N(t, removeAt, 3);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(2, 3, 4))));

        WHEN_CALL_N(t, removeAt, 1);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(2, 4))));

        WHEN_CALL_N(t, removeAt, -1);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LF(2))));

        WHEN_CALL_N(t, removeAt, -1);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList()));

        WHEN_CALL_N(t, removeAt, -1);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList()));

        WHEN_CALL_N(t, removeAt, 0.f);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_EQUAL_AT_OUTLET(0, t, DataPtr(new DataTypeMList()));
    }

    SECTION("ext")
    {
        TestExtDataMList t("data.mlist");
        TestExtDataMList t_alias("ml");
    }
}
