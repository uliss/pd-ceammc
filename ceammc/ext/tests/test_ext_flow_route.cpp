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
#include "../data/datatype_dict.h"
#include "../data/datatype_set.h"
#include "../flow/flow_route.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(FlowRoute, flow, route);

TEST_CASE("flow.route", "[externals]")
{
    pd_test_mod_init_flow_route();

    SECTION("init")
    {
        SECTION("empty")
        {
            TestFlowRoute t("flow.route");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("args")
        {
            TestFlowRoute t("flow.route", LA("test"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
        }

        SECTION("args")
        {
            TestFlowRoute t("flow.route", LA("a", "b", "c", "d"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 5);
        }

        SECTION("props")
        {
            TestFlowRoute t("flow.route", LA("a", "@b", "c", "@d"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 5);
        }
    }

    SECTION("list")
    {
        SECTION("empty")
        {
            TestFlowRoute t("flow.route");
            WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
            REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 2, 3));
        }

        TestFlowRoute t("flow.route", LA("a", "b", 500));

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // no match
        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_LIST_AT_OUTLET(3, t, LF(1, 2, 3));

        // match first
        WHEN_SEND_LIST_TO(0, t, LA("a"));
        REQUIRE_LIST_AT_OUTLET(0, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_LIST_TO(0, t, LA("a", "b"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("b"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_LIST_TO(0, t, LA("a", "b", "c", "d"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("b", "c", "d"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match second
        WHEN_SEND_LIST_TO(0, t, LA("b"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST_AT_OUTLET(1, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_LIST_TO(0, t, LA("b", "c"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST_AT_OUTLET(1, t, LA("c"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_LIST_TO(0, t, LA("b", "c", "d"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST_AT_OUTLET(1, t, LA("c", "d"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match third
        WHEN_SEND_LIST_TO(0, t, LF(500));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_LIST_AT_OUTLET(2, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_LIST_TO(0, t, LA(500, "message"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_LIST_AT_OUTLET(2, t, LA("message"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        WHEN_SEND_LIST_TO(0, t, LA(500, "a", 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_LIST_AT_OUTLET(2, t, LA("a", 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);
    }

    SECTION("any")
    {
        SECTION("empty")
        {
            TestFlowRoute t("flow.route");
            WHEN_SEND_ANY_TO(t, LA("any"));
            REQUIRE_ANY_AT_OUTLET(0, t, LA("any"));
        }

        TestFlowRoute t("flow.route", LA("a", "b"));

        // no match
        WHEN_SEND_ANY_TO(t, LA("empty"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_ANY_AT_OUTLET(2, t, LA("empty"));

        WHEN_SEND_ANY_TO(t, LA("msg", "a", "b", "c"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_ANY_AT_OUTLET(2, t, LA("msg", "a", "b", "c"));

        // match first
        WHEN_SEND_ANY_TO(t, LA("a"));
        REQUIRE_LIST_AT_OUTLET(0, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);

        WHEN_SEND_ANY_TO(t, LA("a", "b", "c"));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("b", "c"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);

        // match second
        WHEN_SEND_ANY_TO(t, LA("b"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST_AT_OUTLET(1, t, L());
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);

        WHEN_SEND_ANY_TO(t, LA("b", "c", "d"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST_AT_OUTLET(1, t, LA("c", "d"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
    }

    SECTION("data:set")
    {
        SECTION("empty")
        {
            DataPtr dptr(new DataTypeSet(LA("a", "b", 100)));
            TestFlowRoute t("flow.route");

            WHEN_SEND_DATA_TO(0, t, dptr);
            REQUIRE_DATA_AT_OUTLET(0, t, dptr);
        }

        TestFlowRoute t("flow.route", LA("a", "b", 100));

        DataTPtr<DataTypeSet> dptr(new DataTypeSet());

        // no match
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_DATA_AT_OUTLET(3, t, dptr);

        // match 1
        dptr = new DataTypeSet(LA("b", "c", "d", 300));
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_BANG_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match 0 and 2
        dptr = new DataTypeSet(LA(100, 101, "a", 300));
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_BANG_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_BANG_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match all
        dptr = new DataTypeSet(LA("b", 101, "a", 100, "test"));
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_BANG_AT_OUTLET(0, t);
        REQUIRE_BANG_AT_OUTLET(1, t);
        REQUIRE_BANG_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);
    }

    SECTION("data:mlist")
    {
        SECTION("empty")
        {
            DataPtr dptr(new DataTypeMList(LA("a", "b", 100)));
            TestFlowRoute t("flow.route");

            WHEN_SEND_DATA_TO(0, t, dptr);
            REQUIRE_DATA_AT_OUTLET(0, t, dptr);
        }

        TestFlowRoute t("flow.route", LA("a", "b", 100));
        DataTPtr<DataTypeMList> dptr(new DataTypeMList());

        // empty list
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_NO_MSG(t);

        // no match
        dptr = new DataTypeMList(LF(1, 2, 3));
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_DATA_AT_OUTLET(3, t, dptr);

        // match first
        dptr = new DataTypeMList(LA("a"));
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_DATA_AT_OUTLET(0, t, DataPtr(new DataTypeMList()));
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        dptr = new DataTypeMList(LA("a", "b", "c"));
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_DATA_AT_OUTLET(0, t, DataPtr(new DataTypeMList(LA("b", "c"))));
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match second
        dptr = new DataTypeMList(LA("b"));
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_DATA_AT_OUTLET(1, t, DataPtr(new DataTypeMList()));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        dptr = new DataTypeMList(LA("b", "c", "d"));
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_DATA_AT_OUTLET(1, t, DataPtr(new DataTypeMList(LA("c", "d"))));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match third
        dptr = new DataTypeMList(LF(100));
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_DATA_AT_OUTLET(2, t, DataPtr(new DataTypeMList()));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        dptr = new DataTypeMList(LA(100, "c", "d"));
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_DATA_AT_OUTLET(2, t, DataPtr(new DataTypeMList(LA("c", "d"))));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);
    }

    SECTION("data:dict")
    {
        SECTION("empty")
        {
            DataPtr dptr(new DataTypeDict("[a:b][c:d]"));
            TestFlowRoute t("flow.route");

            WHEN_SEND_DATA_TO(0, t, dptr);
            REQUIRE_DATA_AT_OUTLET(0, t, dptr);
        }

        TestFlowRoute t("flow.route", LA("a", "b", 100));
        DataTPtr<DataTypeDict> dptr(new DataTypeDict());

        // no match
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_DATA_AT_OUTLET(3, t, dptr);

        dptr = new DataTypeDict("[1:20][3:40]");
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_DATA_AT_OUTLET(3, t, dptr);

        // match 1
        dptr = new DataTypeDict("[c:123][b:1000]");
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 1000);
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        dptr = new DataTypeDict("[c:123][b:1 2 3]");
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST_AT_OUTLET(1, t, LF(1, 2, 3));
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match 1 data
        auto ptr = new DataTypeDict;
        DataPtr int_ptr(new IntData(100));

        ptr->insert(A("b"), DataAtom(int_ptr));
        dptr = ptr;
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_DATA_AT_OUTLET(1, t, DataPtr(new IntData(100)));
        REQUIRE_NO_MESSAGES_AT_OUTLET(2, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);

        // match 0 and 2
        dptr = new DataTypeDict("[100:1 2 3][a:1000][tempo: 124]");
        WHEN_SEND_DATA_TO(0, t, (DataPtr&)dptr);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1000);
        REQUIRE_NO_MESSAGES_AT_OUTLET(1, t);
        REQUIRE_LIST_AT_OUTLET(2, t, LF(1, 2, 3));
        REQUIRE_NO_MESSAGES_AT_OUTLET(3, t);
    }

    SECTION("other data")
    {
        TestFlowRoute t("flow.route", LA("a", "b", 100));
        DataPtr dptr(new IntData(100));

        WHEN_SEND_DATA_TO(0, t, dptr);
        REQUIRE_DATA_AT_OUTLET(3, t, dptr);
    }

    SECTION("props")
    {
        TestExtFlowRoute t("flow.route", LA("@a", "@b"));

        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 3);

        t << LA("@a", 1, 2);

        REQUIRE(t.hasOutput());
        REQUIRE(t.outputListAt(0) == LF(1, 2));
    }
}
