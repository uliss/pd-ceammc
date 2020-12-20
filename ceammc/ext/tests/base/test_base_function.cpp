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
#include "ceammc_pd.h"
#include "function.h"
#include "function_call.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(Function, base, function)

TEST_CASE("function", "[externals]")
{
    pd_test_init();
    setup_function_call();

    SECTION("init")
    {
        pd::External f0("function");
        REQUIRE(f0.isNull());

        TExt t("func", LA("sqrt"));
        // already exists
        pd::External t1("function", LA("sqrt"));
        REQUIRE(t1.isNull());
        // new name is ok
        TExt t2("func", LA("sqrt2"));

        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);

        t << 1;
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 1);

        t.bang();
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_BANG_AT_OUTLET(1, t);

        t << LA("A", "B", "C");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST_AT_OUTLET(1, t, LA("A", "B", "C"));

        t << "TEST";
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "TEST");
    }

    SECTION("test")
    {
        TExt t("func", LA("sqrt4"));

        t << 4;
        REQUIRE(t.messagesAt(1)[0].atomValue() == A(4.));
    }

    SECTION("process float")
    {
        LogExternalOutput::setup();

        pd::External func("function", LA("sqrt5"));
        REQUIRE(!func.isNull());
        pd::External sqrt("sqrt");
        REQUIRE(!sqrt.isNull());
        LogExternalOutput out;

        REQUIRE(func.connectTo(1, sqrt, 0));
        REQUIRE(sqrt.connectTo(0, func, 1));
        REQUIRE(func.connectTo(0, out, 0));

        func.sendFloat(4);
        REQUIRE(out.msg().atomValue().asFloat() == Approx(2));

        func.sendFloat(9);
        REQUIRE(out.msg().atomValue().asFloat() == Approx(3));

        pd::External fx("function.call", "sqrt5");
        REQUIRE(!fx.isNull());
        LogExternalOutput fx_out;

        fx.connectTo(0, fx_out, 0);
        fx.sendFloat(9);
        REQUIRE(fx_out.msg().atomValue().asFloat() == Approx(3));
        fx.sendFloat(16);
        REQUIRE(fx_out.msg().atomValue().asFloat() == Approx(4));
    }

    SECTION("process bang")
    {
        pd::External func("function", LA("bang"));
        REQUIRE(!func.isNull());
        pd::External f0("f", -100);
        REQUIRE(!f0.isNull());
        LogExternalOutput out;

        REQUIRE(func.connectTo(1, f0, 0));
        REQUIRE(f0.connectTo(0, func, 1));
        REQUIRE(func.connectTo(0, out, 0));

        func.sendBang();
        REQUIRE(out.msg().atomValue().asFloat() == Approx(-100));

        // call
        pd::External fx("function.call", "bang");
        REQUIRE(!fx.isNull());
        LogExternalOutput fx_out;

        fx.connectTo(0, fx_out, 0);
        fx.sendBang();

        REQUIRE(fx_out.msg().atomValue().asFloat() == Approx(-100));
    }

    SECTION("process symbol")
    {
        pd::External func("function", LA("symbol"));
        REQUIRE(!func.isNull());
        pd::External s0("symbol", "ABC");
        REQUIRE(!s0.isNull());
        LogExternalOutput out;

        REQUIRE(func.connectTo(1, s0, 0));
        REQUIRE(s0.connectTo(0, func, 1));
        REQUIRE(func.connectTo(0, out, 0));

        func.sendBang();
        REQUIRE(out.msg().atomValue().asSymbol() == SYM("ABC"));

        func.sendSymbol("DEF");
        REQUIRE(out.msg().atomValue().asSymbol() == SYM("DEF"));

        // call
        pd::External fx("function.call", "symbol");
        REQUIRE(!fx.isNull());
        LogExternalOutput fx_out;

        fx.connectTo(0, fx_out, 0);

        fx.sendBang();
        REQUIRE(fx_out.msg().atomValue().asSymbol() == SYM("DEF"));
        fx.sendSymbol("IJK");
        REQUIRE(fx_out.msg().atomValue().asSymbol() == SYM("IJK"));
    }

    SECTION("process list")
    {
        pd::External func("function", LA("list"));
        REQUIRE(!func.isNull());
        pd::External l0("list");
        REQUIRE(!l0.isNull());
        LogExternalOutput out;

        REQUIRE(func.connectTo(1, l0, 0));
        REQUIRE(l0.connectTo(0, func, 1));
        REQUIRE(func.connectTo(0, out, 0));

        func.sendList(LF(1, 2, 3));
        REQUIRE(out.msg().listValue() == LF(1, 2, 3));
        func.sendList(LF(3, 2, 1));
        REQUIRE(out.msg().listValue() == LF(3, 2, 1));

        // call
        pd::External fx("function.call", "list");
        REQUIRE(!fx.isNull());
        LogExternalOutput fx_out;

        fx.connectTo(0, fx_out, 0);

        fx.sendList(LF(3, 2, 1));
        REQUIRE(fx_out.msg().listValue() == LF(3, 2, 1));
        fx.sendList(LA("A", "B", "C"));
        REQUIRE(fx_out.msg().listValue() == LA("A", "B", "C"));
    }
}
