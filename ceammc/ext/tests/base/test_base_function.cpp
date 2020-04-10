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
#include "../base/function.h"
#include "test_base.h"
#include "catch.hpp"
#include "ceammc_pd.h"
#include "test_external.h"

#include <stdio.h>

typedef TestExternal<Function> FunctionTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

void f1() { FunctionTest t("func"); }
void f2() { FunctionTest t("sqrt"); }

TEST_CASE("function", "[externals]")
{
    SECTION("init")
    {
        REQUIRE_THROWS(f1());

        FunctionTest t("func", LA("sqrt"));
        // already exists
        REQUIRE_THROWS(f2());
        // new name is ok
        FunctionTest t2("func", LA("sqrt2"));

        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_BANG_AT_OUTLET(1, t);

        WHEN_SEND_LIST_TO(0, t, LA("A", "B", "C"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_LIST_AT_OUTLET(1, t, LA("A", "B", "C"));

        WHEN_SEND_SYMBOL_TO(0, t, "TEST");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "TEST");
    }

    SECTION("test")
    {
        FunctionTest t("func", LA("sqrt"));
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 2);
    }

    SECTION("process")
    {
        function_setup();
        LogExternalOutput::setup();

        pd::External func("function", LA("sqrt"));
        REQUIRE(!func.isNull());
        pd::External sqrt("sqrt");
        REQUIRE(!sqrt.isNull());
        LogExternalOutput out;

        REQUIRE(func.connectTo(1, sqrt, 0));
        REQUIRE(sqrt.connectTo(0, func, 1));
        REQUIRE(func.connectTo(0, out, 0));

        func.sendFloat(4);
        REQUIRE(out.msg().atomValue().asFloat() == Approx(2));
    }
}
