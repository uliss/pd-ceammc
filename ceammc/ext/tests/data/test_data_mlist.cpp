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
#include "ceammc_pd.h"
#include "data_mlist.h"
#include "test_data_base.h"

#include <stdio.h>

#define REQUIRE_SIZE(obj, n) REQUIRE_PROPERTY(t, @size, float(n));
#define REQUIRE_NOT_EMPTY(obj) REQUIRE_PROPERTY(t, @empty, 0.f);
#define REQUIRE_EMPTY(obj) REQUIRE_PROPERTY(t, @empty, 1);

#define REQUIRE_LIST(obj, lst)               \
    {                                        \
        WHEN_SEND_BANG_TO(0, obj);           \
        REQUIRE_SIZE(obj, lst.size());       \
        REQUIRE_LIST_AT_OUTLET(0, obj, lst); \
    }

#define REQUIRE_DATA_AT(outlet, obj, data)                    \
    {                                                         \
        REQUIRE(obj.hasNewMessages(outlet));                  \
        REQUIRE(obj.lastMessage(outlet).isData());            \
        REQUIRE(obj.lastMessage(outlet).atomValue() == data); \
    }

PD_COMPLETE_TEST_SETUP(DataMList, data, mlist)

using Obj = TestDataMList;
using Ext = TestExtDataMList;
using ML = MListAtom;

TEST_CASE("data.mlist", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            Obj t("data.mlist");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_EMPTY(t);
            REQUIRE_SIZE(t, 0);
        }

        SECTION("args")
        {
            REQUIRE(parseDataList(LA("(", "a", "b", 1, 10, ")")) == ML("a", "b", 1, 10));
            Obj t("data.mlist", LA("(", "a", "b", 1, 10, ")"));
            REQUIRE_SIZE(t, 4);
            REQUIRE_PROPERTY_FLOAT(t, @empty, 0);
        }

        SECTION("quotes")
        {
            Obj t("data.mlist", LA("(\"", "b", "d", "\")"));
            REQUIRE_PROPERTY_FLOAT(t, @size, 1);
        }
    }

    SECTION("append")
    {
        Obj t("data.mlist");

        REQUIRE_PROPERTY_FLOAT(t, @size, 0);

        WHEN_CALL(t, append);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @size, 0);

        WHEN_CALL_N(t, append, 123);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @size, 1);

        WHEN_CALL_N(t, append, new IntData(-1000));
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @size, 2);
    }

    SECTION("onBang")
    {
        Obj t("data.mlist");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML());

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_DATA_AT(0, t, ML(1, 2, 3));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(1, 2, 3));

        SECTION("construct")
        {
            Obj t("data.mlist", LA("(1 2 3 4)"));
            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_DATA_AT(0, t, ML(1, 2, 3, 4));
        }
    }

    SECTION("onList")
    {
        Obj t("data.mlist");

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_DATA_AT(0, t, ML(1, 2, 3));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_DATA_AT(0, t, ML());
    }

    SECTION("clear")
    {
        Obj t("data.mlist", LA("(1 2 3 4 (1 2))"));

        REQUIRE_PROPERTY_FLOAT(t, @size, 5);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(1, 2, 3, 4, ML(1, 2)));

        WHEN_CALL(t, clear);
        REQUIRE_PROPERTY_FLOAT(t, @size, 0);
    }

    SECTION("set")
    {
        Obj t("data.mlist");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML());

        WHEN_CALL_N(t, set, 1, 2);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(1, 2));
    }

    SECTION("insert")
    {
        Obj t("data.mlist");

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
        REQUIRE_DATA_AT(0, t, ML(10, 20));

        WHEN_CALL_N(t, insert, 0.f, 30, 40);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(30, 40, 10, 20));

        WHEN_CALL_N(t, insert, 1, 50);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(30, 50, 40, 10, 20));

        WHEN_CALL_N(t, insert, -1, 60);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(30, 50, 40, 10, 60, 20));

        WHEN_CALL_N(t, insert, -10, 60);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(30, 50, 40, 10, 60, 20));
    }

    SECTION("pop")
    {
        Obj t("data.mlist");

        WHEN_CALL(t, pop);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(1, 2, 3, 4));

        WHEN_CALL(t, pop);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(1, 2, 3));

        WHEN_CALL(t, pop);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(1, 2));

        WHEN_CALL(t, pop);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(1));

        WHEN_CALL(t, pop);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML());

        WHEN_CALL(t, pop);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML());
    }

    SECTION("remove")
    {
        Obj t("data.mlist");

        // empty
        WHEN_CALL(t, pop);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(1, 2, 3, 4));

        WHEN_CALL(t, removeAt);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(1, 2, 3, 4));

        WHEN_CALL_N(t, removeAt, 0.f);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(2, 3, 4));

        WHEN_CALL_N(t, removeAt, 3);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(2, 3, 4));

        WHEN_CALL_N(t, removeAt, 1);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(2, 4));

        WHEN_CALL_N(t, removeAt, -1);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML(2));

        WHEN_CALL_N(t, removeAt, -1);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML());

        WHEN_CALL_N(t, removeAt, -1);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML());

        WHEN_CALL_N(t, removeAt, 0.f);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT(0, t, ML());
    }

    SECTION("ext")
    {
        Ext t("data.mlist");
        Ext t_alias("ml");
    }
}
