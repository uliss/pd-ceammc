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
#include "../list/list_foldleft.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"
#include "test_external.h"

#include <stdio.h>

typedef TestExtension<ListFoldLeft> ListFoldTest;
static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

using namespace ceammc::pd;

TEST_CASE("list.foldl", "[externals]")
{
    pd_init();
    ExternalOutput::setup();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            ListFoldTest t("list.foldl", AtomList());
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @init, 0.f);

            WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
            REQUIRE(t.messageCount(1) == 3);
            REQUIRE(t.messageAt(0, 1).listValue() == ListApprox(0, 1));
            REQUIRE(t.messageAt(1, 1).listValue() == ListApprox(0, 2));
            REQUIRE(t.messageAt(2, 1).listValue() == ListApprox(0, 3));
        }

        SECTION("properties")
        {
            ListFoldTest t("list.foldl", L2("@init", 2));
            REQUIRE_PROPERTY(t, @init, 2);

            WHEN_SEND_LIST_TO(0, t, L2(10, 20));
            REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(2));
            REQUIRE(t.messageCount(1) == 2);
            REQUIRE(t.messageAt(0, 1).listValue() == ListApprox(2, 10));
            REQUIRE(t.messageAt(1, 1).listValue() == ListApprox(2, 20));

            t.cleanAllMessages();
        }

        SECTION("positional arguments")
        {
            ListFoldTest t("list.foldl", L1(-12));
            REQUIRE_PROPERTY(t, @init, -12);
        }

        SECTION("positional arguments and props mixed")
        {
            ListFoldTest t("list.foldl", L3(-12, "@init", 100));
            REQUIRE_PROPERTY(t, @init, 100);

            // property arguments have priority
            REQUIRE_PROPERTY(t, @init, 100);
        }
    }

    SECTION("pd external")
    {
        setup_list_foldl();

        SECTION("+")
        {
            External foldl("list.foldl", L2("@init", 0.f));
            External plus("+");
            ExternalOutput out;

            REQUIRE(!foldl.isNull());
            REQUIRE(!plus.isNull());

            foldl.connectTo(1, plus, 0);
            foldl.connectFrom(0, plus, 1);
            foldl.connectTo(0, out, 0);

            foldl.sendList(L3(1, 2, 3));
            REQUIRE(out.msg().atomValue().asFloat() == 6.f);
        }

        SECTION("+")
        {
            External foldl("list.foldl", L2("@init", 0.f));
            External minus("-");
            ExternalOutput out;

            REQUIRE(!foldl.isNull());
            REQUIRE(!minus.isNull());

            foldl.connectTo(1, minus, 0);
            foldl.connectFrom(0, minus, 1);
            foldl.connectTo(0, out, 0);

            foldl.sendList(L3(1, 2, 3));
            REQUIRE(out.msg().atomValue().asFloat() == Approx(-6.f));
        }

        SECTION("*")
        {
            External foldl("list.foldl", L2("@init", 1));
            External mul("*");
            ExternalOutput out;

            REQUIRE(!foldl.isNull());
            REQUIRE(!mul.isNull());

            foldl.connectTo(1, mul, 0);
            foldl.connectFrom(0, mul, 1);
            foldl.connectTo(0, out, 0);

            foldl.sendList(L3(2, 3, 4));
            REQUIRE(out.msg().atomValue().asFloat() == 24.f);
        }
    }
}
