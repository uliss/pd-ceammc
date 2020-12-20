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
#include "list_repeat.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListRepeat, list, repeat)

using TExt = TestExtListRepeat;
using TObj = TestListRepeat;

TEST_CASE("list.repeat", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("list.repeat");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @times, 1);
        }

        SECTION("args")
        {
            TObj t("list.repeat", LF(3));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @times, 3);
        }

        SECTION("args")
        {
            TObj t("list.repeat", LF(0.f));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @times, 0.f);
        }

        SECTION("props")
        {
            TObj t("list.repeat", LA("@times", 5));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @times, 5);
        }

        SECTION("negative")
        {
            TObj t("list.repeat", LF(-1));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @times, 1);
        }

        SECTION("huge")
        {
            TObj t("list.repeat", LF(10000));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @times, 10000);
        }

        SECTION("huge")
        {
            TObj t("list.repeat", LF(10001));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @times, 1);
        }
    }

    SECTION("inlets")
    {
        TObj t("list.repeat", LF(10));
        REQUIRE_PROPERTY_FLOAT(t, @times, 10);

        WHEN_SEND_FLOAT_TO(1, t, 5);
        REQUIRE_PROPERTY_FLOAT(t, @times, 5);

        WHEN_SEND_FLOAT_TO(1, t, 1);
        REQUIRE_PROPERTY_FLOAT(t, @times, 1);
        WHEN_SEND_FLOAT_TO(1, t, 0);
        REQUIRE_PROPERTY_FLOAT(t, @times, 0);
        WHEN_SEND_FLOAT_TO(1, t, -1);
        REQUIRE_PROPERTY_FLOAT(t, @times, 0);

        WHEN_SEND_FLOAT_TO(1, t, 10000);
        REQUIRE_PROPERTY_FLOAT(t, @times, 10000);
        WHEN_SEND_FLOAT_TO(1, t, 10001);
        REQUIRE_PROPERTY_FLOAT(t, @times, 10000);
    }

    SECTION("float")
    {
        TObj t("list.repeat", LF(4));

        WHEN_SEND_FLOAT_TO(0, t, 11);
        REQUIRE_LIST_AT_OUTLET(0, t, LF(11, 11, 11, 11));

        WHEN_SEND_FLOAT_TO(1, t, 0);
        WHEN_SEND_FLOAT_TO(0, t, 11);
        REQUIRE_LIST_AT_OUTLET(0, t, L());
    }

    SECTION("symbol")
    {
        TObj t("list.repeat", LF(3));

        WHEN_SEND_SYMBOL_TO(0, t, "A");
        REQUIRE_LIST_AT_OUTLET(0, t, LA("A", "A", "A"));

        WHEN_SEND_FLOAT_TO(1, t, 0);
        WHEN_SEND_SYMBOL_TO(0, t, "B");
        REQUIRE_LIST_AT_OUTLET(0, t, L());
    }

    SECTION("list")
    {
        TObj t("list.repeat", LF(3));

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(1, 2, 1, 2, 1, 2));
    }

    SECTION("data")
    {
        TObj t("list.repeat", LF(2));

        WHEN_SEND_DATA_TO(0, t, IntData(100));
        REQUIRE_LIST_AT_OUTLET(0, t, LA(IntA(100), IntA(100)));
    }

    SECTION("mlist")
    {
        TExt t("list.repeat", LF(2));

        t.send(MLA());
        REQUIRE(t.outputAtomAt(0) == MLA());

        t.send(MLA("abc"));
        REQUIRE(t.outputAtomAt(0) == MLA("abc", "abc"));

        t.send(MLA(1, 2, 3));
        REQUIRE(t.outputAtomAt(0) == MLA(1, 2, 3, 1, 2, 3));

        t.send(MLA(MLA()));
        REQUIRE(t.outputAtomAt(0) == MLA());
    }
}
