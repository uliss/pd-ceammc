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
#include "datatype_string.h"
#include "load_msg.h"
#include "test_base.h"
#include "test_catch2.hpp"
#include "test_datatypes.h"

PD_COMPLETE_TEST_SETUP(LoadMsg, load, msg)

TEST_CASE("msg.onload", "[extension]")
{
    pd_test_init();
    test::pdPrintToStdError(true);

    SECTION("construct")
    {
        SECTION("default")
        {
            TExt t("msg.onload");
            REQUIRE(t.numInlets() == 0);
            REQUIRE(t->numOutlets() == 1);

            t->onLoadBang();
            REQUIRE(t.isOutputBangAt(0));
        }

        SECTION("float")
        {
            TExt t("msg.onload", LF(-100));

            t->onLoadBang();
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == -100);
        }

        SECTION("symbol")
        {
            TExt t("msg.onload", LA("symbol", "ABC"));

            t->onLoadBang();
            REQUIRE(t.isOutputSymbolAt(0));
            REQUIRE(t.outputSymbolAt(0) == S("ABC"));
        }

        SECTION("list")
        {
            TExt t("msg.onload", LA(1, 2, 3));

            t->onLoadBang();
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LF(1, 2, 3));
        }

        SECTION("list2")
        {
            TExt t("msg.onload", LA("list", "A", "B", "C"));

            t->onLoadBang();
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LA("A", "B", "C"));
        }

        SECTION("any")
        {
            TExt t("msg.onload", LA("A", 2, 3));

            t->onLoadBang();
            REQUIRE(t.isOutputAnyAt(0));
            REQUIRE(t.outputAnyAt(0) == LA("A", 2, 3));
        }

        SECTION("data")
        {
            TExt t("msg.onload", LA("String(ABC)"));

            t->onLoadBang();
            REQUIRE(t.isOutputDataAt(0));
            REQUIRE(t.outputAtomAt(0) == StringAtom("ABC"));
        }
    }
}
