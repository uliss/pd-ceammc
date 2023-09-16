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
#include "catch.hpp"
#include "function.h"
#include "function_call.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(FunctionCall, function, call)

TEST_CASE("function.call", "[externals]")
{
    pd_test_init();
    setup_base_function();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("function.call");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @name, &s_);
        }

        SECTION("prop")
        {
            TExt t("function.call", LA("@name", "f0"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @name, LA("f0"));
        }

        SECTION("args")
        {
            TExt t("function.call", LA("f1"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @name, LA("f1"));
        }

        SECTION("alias")
        {
            TExt t("func.call", LA("f2"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @name, LA("f2"));
        }
    }

    SECTION("call")
    {
        pd::External fn0("function", LA("sqrt"));
        REQUIRE(!fn0.isNull());
        pd::External sqrt("sqrt");
        REQUIRE(!sqrt.isNull());

        REQUIRE(fn0.connectTo(1, sqrt, 0));
        REQUIRE(sqrt.connectTo(0, fn0, 1));

        pd::External fn1("function", LA("mul2"));
        REQUIRE(!fn1.isNull());
        pd::External mul("*", LF(2));
        REQUIRE(!mul.isNull());

        REQUIRE(fn1.connectTo(1, mul, 0));
        REQUIRE(mul.connectTo(0, fn1, 1));

        TExt t("func.call", LA("sqrt"));
        t << 9;
        REQUIRE_FLOAT_AT_OUTLET(0, t, 3);
        t << 16;
        REQUIRE_FLOAT_AT_OUTLET(0, t, 4);

        t.sendSymbolTo("mul2", 1);
        REQUIRE_PROPERTY(t, @name, LA("mul2"));

        t << 9;
        REQUIRE_FLOAT_AT_OUTLET(0, t, 18);
        t << 16;
        REQUIRE_FLOAT_AT_OUTLET(0, t, 32);
    }
}
